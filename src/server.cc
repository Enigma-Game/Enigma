/*
 * Copyright (C) 2004,2005 Daniel Heck
 * Copyright (C) 2007,2008,2009 Ronald Lamprecht
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "server.hh"

#include "errors.hh"
#include "game.hh"
#include "actors.hh"
#include "client.hh"
#include "lua.hh"
#include "lev/Index.hh"
#include "lev/Proxy.hh"
#include "main.hh"
#include "nls.hh"
#include "options.hh"
#include "player.hh"
#include "player.hh"
#include "StateManager.hh"
#include "world.hh"

#include "enet/enet.h"

#ifdef WIN32
// SendMessage is a Syscall on Windows, so we simply undefine it to use this
// name for one of the methods
#undef SendMessage
#endif

#include <cctype>

using namespace enigma::server;
using namespace world;
using namespace std;

namespace enigma_server
{
    enum ServerState {
        sv_idle,
        sv_waiting_for_clients,
        sv_running,
        sv_paused,
        sv_restart_level,
        sv_restart_game,
        sv_finishing,
        sv_finished,
    };

    class Server {
    public:


    private:

        static Server *instance;
    };

    void PrepareLevel();

}

/* -------------------- Global variables -------------------- */

double   server::LastMenuTime;
int      server::MenuCount;
unsigned server::SublevelNumber;
std::string server::SublevelTitle;

bool     server::NoCollisions = false;

bool     server::AllowSingleOxyds;
bool     server::AllowSuicide;
bool     server::AllowTogglePlayer;
bool     server::CreatingPreview = false;   // read only for Lua
bool     server::ConserveLevel;
bool     server::IsDifficult;               // read only for Lua
bool     server::IsLevelRestart;            // no Lua access
bool     server::ProvideExtralifes;
bool     server::InfiniteReincarnation;
bool     server::SurviveFinish;

Value    server::FollowAction;
bool     server::FollowGrid;
int      server::FollowMethod;
Value    server::FollowThreshold;

double   server::LevelTime;                 // read only for Lua (> 1.10)
bool     server::ShowMoves;
bool     server::SingleComputerGame;        // no Lua access
bool     server::TwoPlayerGame;             // no Lua access
GameType server::GameCompatibility;         // no Lua access
bool     server::WorldSized;                // no Lua access
bool     server::WorldInitialized;          // no Lua access
double   server::Brittleness;
double   server::Fragility;
double   server::CrackSpreading;
ecl::V2  server::ConstantForce;
double   server::BumperForce;
double   server::ElectricForce;
double   server::EnigmaCompatibility;       // no Lua access
double   server::FlatForce;
double   server::FrictionFactor;
int      server::GlassesVisibility;         // no Lua access
int      server::ExtralifeGlasses;
double   server::HoleForce;
lev::levelStatusType   server::LevelStatus; // no Lua access
double   server::MagnetForce;
double   server::MagnetRange;
int      server::MaxOxydColor;
int32_t  server::RandomState;               // no Lua access
double   server::RubberViolationStrength;
double   server::SlopeForce;
double   server::SwampSinkSpeed;
double   server::WaterSinkSpeed;
double   server::WormholeForce;
double   server::WormholeRange;

/* -------------------- Local variables -------------------- */

namespace
{
    ServerState        state               = sv_idle;
    double             time_accu           = 0;
    double             current_state_dtime = 0;
    int                 move_counter; // counts movements of stones
    lev::Index *currentIndex = NULL;  // volatile F6 jump back history
    int currentLevel;
    lev::Index *previousIndex = NULL;
    int previousLevel;
    ENetAddress        network_address;
    ENetHost           *network_host       = 0;
}

void load_level(lev::Proxy *levelProxy, bool isRestart)
{
    try {
        server::PrepareLevel();
        IsLevelRestart = isRestart;

        // clear inventory before level load and give us 2 extralives
        player::NewGame();

        levelProxy->loadLevel();  // sets the compatibility mode

        game::ResetGameTimer();

        WorldInitLevel();
        if (!CreatingPreview) {
                player::LevelLoaded(isRestart);
                client::Msg_LevelLoaded(isRestart);
        }
    }
    catch (XLevelLoading &err) {
        std::string levelPathString = 
            (levelProxy->getNormPathType() == lev::Proxy::pt_resource) ?
            levelProxy->getAbsLevelPath() : levelProxy->getNormFilePath();
        std::string msg = _("Server Error: could not load level '")
                               + levelPathString + "'\n"
                               + err.what();
        if (!CreatingPreview) {
            client::Msg_Error(msg);
            state = sv_idle;
        } else {
            throw XLevelLoading(msg);
        }
    }
    catch (XLevelRuntime &err) {
        std::string levelPathString = 
            (levelProxy->getNormPathType() == lev::Proxy::pt_resource) ?
            levelProxy->getAbsLevelPath() : levelProxy->getNormFilePath();
        std::string msg = _("Server Error: could not load level '")
                               + levelPathString + "'\n"
                               + err.what();
        if (!CreatingPreview) {
            client::Msg_Error(msg);
            state = sv_idle;
        } else {
            throw XLevelLoading(msg);
        }
    }
}


void server::RaiseError (const std::string &msg)
{
    throw XLevelLoading (msg);
}

void gametick(double dtime)
{
    const double timestep = 0.01; // 10ms
    int count = 0;

    time_accu += dtime;
    if (time_accu > 1.0) {
        fprintf (stderr, "Whoa, system overload!\n");
        time_accu = 1.0;
    }
    player::Tick (time_accu);
    for (;time_accu >= timestep; time_accu -= timestep, count++) {
        server::LevelTime += timestep;
        WorldTick(timestep);
//        if (lua::CallFunc (lua::LevelState(), "Tick", timestep, NULL) != 0) {
//            throw XLevelRuntime (string("Calling 'Tick' failed:\n")
//                                                + lua::LastError(lua::LevelState()));
//        }
    }
    TickFinished(count * timestep);
}


/* -------------------- Functions -------------------- */

void server::Init()
{
}

void server::Shutdown()
{
    lua::ShutdownLevel();
    if (network_host != 0)
        enet_host_destroy (network_host);
}


void server::InitNewGame()
{
    PrepareLevel();
}

bool server::NetworkStart()
{
    return true;
}



void server::PrepareLevel()
{
    state = sv_waiting_for_clients;
    
    server::SublevelNumber    = 1;
    server::SublevelTitle     = "";
    server::LastMenuTime      = 0.0;
    server::MenuCount         = 0;
    server::NoCollisions      = false;
    server::WorldInitialized  = false;
    server::LevelTime         = 0.0;
    server::ConserveLevel     = true;
    server::ProvideExtralifes = true;
    server::InfiniteReincarnation = false;
    server::SurviveFinish     = true;
    server::TwoPlayerGame     = false;
    server::SingleComputerGame= true;
    server::AllowSingleOxyds  = false;
    server::AllowSuicide      = true;
    server::AllowTogglePlayer = true;
    server::FollowAction      = GridPos(19, 12);   // inner space of a room
    server::FollowGrid        = true;
    server::FollowMethod      = display::FOLLOW_FLIP;      // FLIP
    server::FollowThreshold   = 0.5;
    server::ShowMoves         = false;
    server::Brittleness       = 0.5;
    server::Fragility         = 1.0;
    server::CrackSpreading    = 0.5;
    server::ConstantForce       = ecl::V2(0, 0);
    server::SlopeForce        = 25.0;
    server::FlatForce         = 0.0;
    server::FrictionFactor    = 1.0;
    server::GlassesVisibility = 0;     // nothing
    server::ExtralifeGlasses  = 19;  // death + hollow + lightpassenger
    server::ElectricForce     = 15.0;
    server::BumperForce       = 200.0;
    server::WaterSinkSpeed    = 10000;
    server::SwampSinkSpeed    = 4;
    server::MagnetForce       = 30;
    server::MagnetRange       = 10;
    server::MaxOxydColor      = 7;    // for compatibility
    server::RubberViolationStrength = 50;
    server::WormholeForce     = 30;
    server::WormholeRange     = 10;
    server::HoleForce         = 1.0;

    move_counter = 0;

    enigma::WorldPrepareLevel();
    server::WorldSized = false;

    player::PrepareLevel();
}

void server::PrepareLua() {
    server::IsDifficult = (GetDifficulty() == DIFFICULTY_HARD);
    // Restart the Lua environment so symbol definitions from
    // different levels do not get in each other's way.
    int api = (server::EnigmaCompatibility < 1.10) ? 1 : 2;
    lua::ShutdownLevel();
    lua_State *L = lua::InitLevel(api);
    if (api == 1 && lua::DoSysFile(L, "compat.lua") != lua::NO_LUAERROR) {
        throw XLevelLoading("While processing 'compat.lua':\n" + lua::LastError(L));
    }
    if (lua::DoSysFile(L, ecl::strf("api%dinit.lua", api)) != lua::NO_LUAERROR) {
        throw XLevelLoading(ecl::strf("While processing 'api%dinit.lua':\n", api) + lua::LastError(L));
    }
    if (lua::DoSysFile(L, "security.lua") != lua::NO_LUAERROR) {
        throw XLevelLoading("While processing 'security.lua':\n" + lua::LastError(L));
    }
}

void server::RestartLevel() 
{
    if (state == sv_running || state == sv_finished || state == sv_finishing) {
        state = sv_restart_level;
        current_state_dtime = 0;
    }
}

bool server::IsRestartingLevel() {
    return state == sv_restart_level;
}

void server::Msg_RestartGame() 
{
    if (state == sv_running || state == sv_finished || state == sv_finishing) {
        state = sv_restart_game;
        current_state_dtime = 0;
    }
}

void server::FinishLevel() {
    if (state == sv_running) {
        state = sv_finishing;
    }
}

void server::Tick (double dtime) {
    switch (state) {
        case sv_idle:
            break;
        case sv_paused:
            break;
        case sv_waiting_for_clients:
            break;
        case sv_running: 
            gametick (dtime);
            break;
        case sv_restart_level:
        case sv_restart_game:
            current_state_dtime += dtime;
            if (current_state_dtime >= 1.0) {
                lev::Index *ind = lev::Index::getCurrentIndex();
                load_level(ind->getCurrent(), (state == sv_restart_level));
            } else {
                gametick(dtime);
            }
            break;
        case sv_finishing:
            if (server::SurviveFinish) {
                player::LevelFinished(0);     // mark all shattered actors as dead
                player::CheckDeadActors();    // restart level or game if actors are dead!
            }
            if (state == sv_finishing) {
                state = sv_finished;
                current_state_dtime = 0;
                player::LevelFinished(1); // remove player-controlled actors
                client::Msg_Command("level_finished");
            }
            break;
        case sv_finished:
            current_state_dtime += dtime;
            if (current_state_dtime <= 2.5)
                gametick(dtime);
            else {
                client::Msg_AdvanceLevel (lev::ADVANCE_NEXT_MODE);
                state = sv_waiting_for_clients;
            }
            break;
    }
}

void server::Msg_SetLevelPack (const std::string &name) {
    lev::Index::setCurrentIndex(name);
}

void server::Msg_LoadLevel (lev::Proxy *levelProxy, bool isPreview) {
    server::CreatingPreview = isPreview;
    if (!isPreview) {
	// update F6 jump back history
	if (currentIndex != lev::Index::getCurrentIndex() ||
		currentLevel != currentIndex->getCurrentPosition()) {
	    previousIndex = currentIndex;
	    previousLevel = currentLevel;
	    currentIndex = lev::Index::getCurrentIndex();
	    currentLevel = currentIndex->getCurrentPosition();
	}
    }
    load_level(levelProxy, false);
}

void server::Msg_JumpBack() {
    if (previousIndex != NULL) {
	lev::Index::setCurrentIndex(previousIndex->getName());
	previousIndex->setCurrentPosition(previousLevel);
	currentIndex = previousIndex;
	currentLevel = previousLevel;
	Msg_LoadLevel(currentIndex->getProxy(currentLevel), false);
	Msg_Command("restart");
    }
}


void server::Msg_StartGame()
{
    if (state == sv_waiting_for_clients) {
        time_accu = 0;
        state = sv_running;
    }
    else {
        // Warning << "server: Received unexpected StartGame message.\n";
        // XXX discard message if not waiting for it?
    }
}

namespace enigma_server {

    void Msg_Command_jumpto(const string& dest) {
        // global level jump
        // e.g.:  dest = "Enigma IV,33" -> jump to level #33 of index "Enigma IV"
        // note: level counter start at 1 (not at 0)

        size_t comma = dest.find_first_of(',');
        string error;

        if (comma != string::npos) {
            std::string name = dest.substr(0, comma);
            Log << "Jumpto '" << name <<"'\n";
            int ilevel = atoi(dest.c_str()+comma+1)-1;
            
            if (lev::Index::setCurrentIndex(name)) {
                
                if (ilevel >= 0 && ilevel < lev::Index::getCurrentIndex()->size()) {
                    lev::Index * curInd = lev::Index::getCurrentIndex();
                    curInd->setCurrentPosition(ilevel);
                    Msg_LoadLevel (curInd->getProxy(ilevel), false);
                    Msg_Command("restart");
                } else {
                    error = ecl::strf("Illegal level %i (1-%i)", ilevel+1, lev::Index::getCurrentIndex()->size());
                    // May be we want to reset the current index ?
                }
            } else error = ecl::strf("Illegal level pack %s", name.c_str());
        } else if (lev::Index::setCurrentIndex(dest)) {
            lev::Index * curInd = lev::Index::getCurrentIndex();
            Msg_LoadLevel (curInd->getCurrent(), false);
            Msg_Command("restart");
        } else {
            error = "Syntax: jumpto pack[,level]";
        }

        if (!error.empty()) client::Msg_ShowText(error, false, 2);
    }

    void Msg_Command_find(const string& text) {
        std::string indName = lev::Proxy::search(text);
        if (!indName.empty()) {
            lev::Index::setCurrentIndex(indName);
            lev::Index * searchResult = lev::Index::getCurrentIndex();
            searchResult->setCurrentPosition(0);
            if (searchResult->size() == 1) {
                // play unique level directly 
                Msg_LoadLevel (searchResult->getProxy(0), false);
                Msg_Command("restart");
            } else {
                // display multiple levels as pack
                Msg_Command("abort");
            }
        } else {
            client::Msg_ShowText(string("Couldn't find '")+text+'\'', false, 2);
        }
    }
};

void server::Msg_Command (const string &cmd)
{
    lev::Index *ind = lev::Index::getCurrentIndex();
    lev::Proxy *curProxy = ind->getCurrent();
    
    // ------------------------------ normal commands
    if (cmd == "invrotate") {
        player::RotateInventory();
    }
    else if (cmd == "suicide") {
        player::Suicide();
        if (!AllowSuicide)
            server::Msg_RestartGame();
    }
    else if (cmd == "restart") {
        player::Suicide();
        server::Msg_RestartGame();
    }
    else if (cmd == "abort") {
        client::Msg_Command(cmd);
    }

    // ------------------------------ cheats
    else if (cmd == "god") {
        BroadcastMessage("_shield", player::CurrentPlayer(), GRID_NONE_BIT, true);
        client::Msg_Command("cheater");
    }
    else if (cmd == "collision") {
        server::NoCollisions = !server::NoCollisions;
        if (server::NoCollisions)
            client::Msg_ShowText ("collision handling disabled", false, 2);
        else
            client::Msg_ShowText ("collision handling enabled", false, 2);
        client::Msg_Command("cheater");
    }

    // ------------------------------ quick options
    else if (cmd == "easy") {
        if (app.state->getInt("Difficulty") == DIFFICULTY_HARD) {
            if (curProxy->hasEasyMode()) {
                client::Msg_ShowText("Restarting in easy mode", false, 2);
                app.state->setProperty("Difficulty", DIFFICULTY_EASY);
                server::Msg_Command("restart");
            }
            else
                client::Msg_ShowText("No easymode available.", false, 2);
        }
        else
            client::Msg_ShowText("Already in easy mode.", false, 2);
    }
    else if (cmd == "noeasy") {
        if (app.state->getInt("Difficulty") == DIFFICULTY_EASY) {
            app.state->setProperty("Difficulty", DIFFICULTY_HARD);
            if (curProxy->hasEasyMode()) {
                client::Msg_ShowText("Restarting in normal mode", false, 2);
                server::Msg_Command("restart");
            }
            else {
                client::Msg_ShowText("No difference between easy and normal.", false, 2);
            }
        }
        else
            client::Msg_ShowText("Already in normal mode.", false, 2);
    }
    else if (cmd == "time") {
        if (options::GetBool("TimeHunting") == false) {
            client::Msg_ShowText("Restarting in time-hunt mode", false, 2);
            options::SetOption("TimeHunting", true);
            server::Msg_Command("restart");
        }
        else
            client::Msg_ShowText("Already in time-hunt mode.", false, 2);
    }
    else if (cmd == "notime") {
        if (options::GetBool("TimeHunting")) {
            client::Msg_ShowText("Switched to easy-going mode", false, 2);
            options::SetOption("TimeHunting", false);
            client::Msg_Command("easy_going");
        }
        else
            client::Msg_ShowText("Already in easy-going mode.", false, 2);
    }
    else if (cmd == "info") {
        string infotext       = 
            ecl::strf("Level #%i of '", ind->getCurrentLevel()) + ind->getName()
            + "' (" + curProxy->getAbsLevelPath() + ")  -  \"" + curProxy->getTitle() 
            + "\" by " + curProxy->getAuthor()
            + ecl::strf(" (rev=%i,", curProxy->getReleaseVersion())
            + "id=\"" + curProxy->getId() + "\")";
            
        client::Msg_ShowText(infotext, true);
    }
    else if (cmd.substr(0, 5) == "find ") { // global level-jump
        string args = cmd.substr(5);
        server::Msg_Command_find(args);
    }
    else if (cmd.substr(0, 7) == "jumpto ") { // global level-jump
        string args = cmd.substr(7);
        server::Msg_Command_jumpto(args);
    }
    else if (cmd == "help") {
        client::Msg_ShowText("suicide, restart, abort, easy, noeasy, time, notime, jumpto, find, info", true);
    }
    else if (cmd == "cheats") {
        client::Msg_ShowText("god, collision  -- Be aware: you'll get no medals!", true);
    }
    else if (cmd == "April 1st") {
        client::Msg_ShowText("No pizza, and a cloudless sky.", true);
    }
   
    else {
        enigma::Log << "Warning: Server received unknown command '" << cmd << "'\n";
    }
}


void server::Msg_Pause (bool onoff) {
    if (onoff && state == sv_running)
        state = sv_paused;
    else if (!onoff && state == sv_paused)
        state = sv_running;
}

void server::Msg_Panic (bool onoff) {
    if (onoff && state == sv_running)
        state = sv_idle;
    else if (!onoff && state == sv_idle)
        state = sv_running;
}

void server::Msg_MouseForce (const ecl::V2 &f) {
    SetMouseForce (f);
}


void server::SetCompatibility(const char *version) {
    GameType type = GetGameType(version);

    if (type == GAMET_UNKNOWN) {
        fprintf(stderr, "Invalid compatibility mode '%s' (ignored. using enigma behavior)\n", version);
        fprintf(stderr, "Valid modes:");
        for (int v = 0; v<GAMET_COUNT; ++v)
            fprintf(stderr, " %s", GetGameTypeName((GameType)v).c_str());
        fprintf(stderr, "\n");
        type = GAMET_ENIGMA;
    }

    GameCompatibility = type;
}

void server::SetCompatibility(lev::Proxy *levelProxy) {
    server::GameCompatibility = levelProxy->getEngineCompatibility();
    if (server::GameCompatibility == GAMET_UNKNOWN)
        throw XLevelLoading("unknown engine compatibility");
}

enigma::Difficulty server::GetDifficulty()
{
    lev::Index *ind = lev::Index::getCurrentIndex();
    lev::Proxy *curProxy = ind->getCurrent();
    int i= app.state->getInt("Difficulty");
    if (i == DIFFICULTY_EASY && !server::CreatingPreview && curProxy->hasEasyMode())
        return DIFFICULTY_EASY;
    else
        return DIFFICULTY_HARD;
}

void server::InitMoveCounter() 
{
    move_counter = 0;
}

int server::IncMoveCounter(int increment) 
{
    move_counter += increment; 
    return move_counter;
}

int server::GetMoveCounter() 
{
    return move_counter;
}

void server::Msg_ActivateItem()
{
    player::ActivateFirstItem();
}
