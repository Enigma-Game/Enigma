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
#include "MusicManager.hh"

#include "enet/enet.h"

#ifdef WIN32
// SendMessage is a Syscall on Windows, so we simply undefine it to use this
// name for one of the methods
#undef SendMessage
#endif

#include <cctype>

using namespace std;

namespace enigma {
namespace server {

enum ServerState {
    sv_idle,
    sv_waiting_for_clients,
    sv_running,
    sv_paused,
    sv_teatime,
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

/* -------------------- Global variables -------------------- */

double LastMenuTime;
int MenuCount;
lev::Proxy *LoadedProxy;
unsigned SublevelNumber;
std::string SublevelTitle;

bool NoCollisions = false;

bool AllowSingleOxyds;
bool AllowSuicide;
bool AllowTogglePlayer;
bool AutoRespawn;
bool CreatingPreview = false;  // read only for Lua
bool ConserveLevel;
bool IsDifficult;     // read only for Lua
bool IsLevelRestart;  // no Lua access
bool ProvideExtralifes;
bool InfiniteReincarnation;
bool SurviveFinish;

Value FollowAction;
bool FollowGrid;
int FollowMethod;
Value FollowThreshold;

double LevelTime;  // read only for Lua (> 1.10)
bool ShowMoves;
bool SingleComputerGame;     // no Lua access
bool TwoPlayerGame;          // no Lua access
GameType GameCompatibility;  // no Lua access
bool WorldSized;             // no Lua access
bool WorldInitialized;       // no Lua access
double Brittleness;
double Fragility;
double CrackSpreading;
ecl::V2 ConstantForce;
double BumperForce;
double ElectricForce;
double EnigmaCompatibility;  // no Lua access
double FlatForce;
double FrictionFactor;
int GlassesVisibility;  // no Lua access
int ExtralifeGlasses;
std::string FallenPuzzle;
double HoleForce;
lev::levelStatusType LevelStatus;  // no Lua access
double MagnetForce;
double MagnetRange;
int MaxOxydColor;
int32_t RandomState;  // no Lua access
double RubberViolationStrength;
double SlopeForce;
int SubSoil;
double SwampSinkSpeed;
double WaterSinkSpeed;
double WormholeForce;
double WormholeRange;

/* -------------------- Local variables -------------------- */

namespace {

ServerState state = sv_idle;
ServerState state_before_teatime = sv_idle;
double time_accu = 0;
double current_state_dtime = 0;
int move_counter;                 // counts movements of stones
lev::Index *currentIndex = NULL;  // volatile F6 jump back history
int currentLevel;
lev::Index *previousIndex = NULL;
int previousLevel;
ENetAddress network_address;
ENetHost *network_host = 0;

}  // namespace

void load_level(lev::Proxy *levelProxy, bool isRestart) {
    try {
        Uint32 start_tick_time = SDL_GetTicks();  // meassure time for level loading
        if (!CreatingPreview)
            sound::StartLevelLoadMusic();

        LoadedProxy = levelProxy;
        IsLevelRestart = isRestart;
        PrepareLevel();

        // clear inventory before level load and give us 2 extralives
        player::NewGame();

        levelProxy->loadLevel();  // sets the compatibility mode

        game::ResetGameTimer();

        WorldInitLevel();
        if (!CreatingPreview) {
            player::LevelLoaded(isRestart);
            client::Msg_LevelLoaded(isRestart);
        }
        double exectime = (SDL_GetTicks() - start_tick_time) / 1000.0;
        Log << ecl::strf("Server load level did take %g seconds\n", exectime);
    } catch (XLevelLoading &err) {
        std::string levelPathString = (levelProxy->getNormPathType() == lev::Proxy::pt_resource)
                                          ? levelProxy->getAbsLevelPath()
                                          : levelProxy->getNormFilePath();
        std::string msg =
            _("Server Error: could not load level '") + levelPathString + "'\n" + err.what();
        if (!CreatingPreview) {
            client::Msg_Error(msg);
            state = sv_idle;
        } else {
            throw XLevelLoading(msg);
        }
    } catch (XLevelRuntime &err) {
        std::string levelPathString = (levelProxy->getNormPathType() == lev::Proxy::pt_resource)
                                          ? levelProxy->getAbsLevelPath()
                                          : levelProxy->getNormFilePath();
        std::string msg =
            _("Server Error: could not load level '") + levelPathString + "'\n" + err.what();
        if (!CreatingPreview) {
            client::Msg_Error(msg);
            state = sv_idle;
        } else {
            throw XLevelLoading(msg);
        }
    }
}

void RaiseError(const std::string &msg) {
    throw XLevelLoading(msg);
}

void gametick(double dtime) {
    const double timestep = 0.01;  // 10ms
    int count = 0;

    time_accu += dtime;
    if (time_accu > 1.0) {
        fprintf(stderr, "Whoa, system overload!\n");
        time_accu = 1.0;
    }
    player::Tick(time_accu);
    for (; time_accu >= timestep; time_accu -= timestep, count++) {
        LevelTime += timestep;
        WorldTick(timestep);
    }
    display::GetStatusBar()->set_counter(server::GetMoveCounter());
    player::MessagePlayerPositionsToClient();
    TickFinished(count * timestep);
}

/* -------------------- Functions -------------------- */

void Init() {
}

void Shutdown() {
    lua::ShutdownLevel();
    if (network_host != 0)
        enet_host_destroy(network_host);
}

void InitNewGame() {
    PrepareLevel();
}

bool NetworkStart() {
    return true;
}

void PrepareLevel() {
    state = sv_waiting_for_clients;

    SublevelNumber = 1;
    SublevelTitle = "";
    LastMenuTime = 0.0;
    MenuCount = 0;
    NoCollisions = false;
    WorldInitialized = false;
    LevelTime = 0.0;
    ConserveLevel = true;
    ProvideExtralifes = true;
    InfiniteReincarnation = false;
    SurviveFinish = true;
    TwoPlayerGame = false;
    SingleComputerGame = true;
    AllowSingleOxyds = false;
    AllowSuicide = true;
    AllowTogglePlayer = true;
    AutoRespawn = false;
    FollowAction = GridPos(19, 12);  // inner space of a room
    FollowGrid = true;
    FollowMethod = display::FOLLOW_FLIP;  // FLIP
    FollowThreshold = 0.5;
    ShowMoves = false;

    GlassesVisibility = 0;  // nothing
    FlatForce = 0.0;
    ConstantForce = ecl::V2(0, 0);

    // object class specific with Lua access
    Brittleness = 0.5;
    BumperForce = 200.0;
    CrackSpreading = 0.5;
    ElectricForce = 15.0;
    ExtralifeGlasses = 19;  // death + hollow + lightpassenger
    FallenPuzzle = "fl_gray";
    Fragility = 1.0;
    FrictionFactor = 1.0;
    HoleForce = 1.0;
    MagnetForce = 30;
    MagnetRange = 10;
    MaxOxydColor = 7;  // for compatibility
    RubberViolationStrength = 50;
    SlopeForce = 25.0;
    SubSoil = 0;
    SwampSinkSpeed = 4;
    WaterSinkSpeed = 10000;
    WormholeForce = 30;
    WormholeRange = 10;

    move_counter = 0;

    enigma::WorldPrepareLevel();
    WorldSized = false;

    player::PrepareLevel();
}

void PrepareLua() {
    IsDifficult = (GetDifficulty() == DIFFICULTY_HARD);
    // Restart the Lua environment so symbol definitions from
    // different levels do not get in each other's way.
    int api = (EnigmaCompatibility < 1.10) ? 1 : 2;
    lua::ShutdownLevel();
    lua_State *L = lua::InitLevel(api);
    if (api == 1 && lua::DoSysFile(L, "compat.lua") != lua::NO_LUAERROR) {
        throw XLevelLoading("While processing 'compat.lua':\n" + lua::LastError(L));
    }
    if (lua::DoSysFile(L, ecl::strf("api%dinit.lua", api)) != lua::NO_LUAERROR) {
        throw XLevelLoading(ecl::strf("While processing 'api%dinit.lua':\n", api) +
                            lua::LastError(L));
    }
    if (lua::DoSysFile(L, "security.lua") != lua::NO_LUAERROR) {
        throw XLevelLoading("While processing 'security.lua':\n" + lua::LastError(L));
    }
}

void RestartLevel() {
    if (state == sv_running || state == sv_finished || state == sv_finishing) {
        state = sv_restart_level;
        current_state_dtime = 0;
    }
}

bool IsRestartingLevel() {
    return state == sv_restart_level;
}

void Msg_RestartGame() {
    if (state == sv_running || state == sv_finished || state == sv_finishing) {
        state = sv_restart_game;
        current_state_dtime = 0;
    }
}

void FinishLevel() {
    if (state == sv_running) {
        state = sv_finishing;
    }
}

void Tick(double dtime) {
    switch (state) {
    case sv_idle: break;
    case sv_paused: break;
    case sv_teatime: break;
    case sv_waiting_for_clients: break;
    case sv_running: gametick(dtime); break;
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
        if (SurviveFinish) {
            player::LevelFinished(0);   // mark all shattered actors as dead
            player::CheckDeadActors();  // restart level or game if actors are dead!
        }
        if (state == sv_finishing) {
            state = sv_finished;
            current_state_dtime = 0;
            player::LevelFinished(1);  // remove player-controlled actors
            client::Msg_Command("level_finished");
        }
        break;
    case sv_finished:
        current_state_dtime += dtime;
        if (current_state_dtime <= 2.5)
            gametick(dtime);
        else {
            client::Msg_AdvanceLevel(lev::ADVANCE_NEXT_MODE);
            state = sv_waiting_for_clients;
        }
        break;
    }
}

void Msg_SetLevelPack(const std::string &name) {
    lev::Index::setCurrentIndex(name);
}

void Msg_LoadLevel(lev::Proxy *levelProxy, bool isPreview) {
    CreatingPreview = isPreview;
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

void Msg_JumpBack() {
    if (previousIndex != NULL) {
        lev::Index::setCurrentIndex(previousIndex->getName());
        previousIndex->setCurrentPosition(previousLevel);
        currentIndex = previousIndex;
        currentLevel = previousLevel;
        Msg_LoadLevel(currentIndex->getProxy(currentLevel), false);
        Msg_Command("restart");
    }
}

void Msg_StartGame() {
    if (state == sv_waiting_for_clients) {
        time_accu = 0;
        state = sv_running;
    } else {
        // Warning << "server: Received unexpected StartGame message.\n";
        // XXX discard message if not waiting for it?
    }
}

void Msg_Command_jumpto(const string &dest) {
    // global level jump
    // e.g.:  dest = "Enigma IV,33" -> jump to level #33 of index "Enigma IV"
    // note: level counter start at 1 (not at 0)

    size_t comma = dest.find_first_of(',');
    string error;

    if (comma != string::npos) {
        std::string name = dest.substr(0, comma);
        Log << "Jumpto '" << name << "'\n";
        int ilevel = atoi(dest.c_str() + comma + 1) - 1;

        if (lev::Index::setCurrentIndex(name)) {

            if (ilevel >= 0 && ilevel < lev::Index::getCurrentIndex()->size()) {
                lev::Index *curInd = lev::Index::getCurrentIndex();
                curInd->setCurrentPosition(ilevel);
                Msg_LoadLevel(curInd->getProxy(ilevel), false);
                Msg_Command("restart");
            } else {
                error = ecl::strf("Illegal level %i (1-%i)", ilevel + 1,
                                  lev::Index::getCurrentIndex()->size());
                // May be we want to reset the current index ?
            }
        } else
            error = ecl::strf("Illegal level pack %s", name.c_str());
    } else if (lev::Index::setCurrentIndex(dest)) {
        lev::Index *curInd = lev::Index::getCurrentIndex();
        Msg_LoadLevel(curInd->getCurrent(), false);
        Msg_Command("restart");
    } else {
        error = "Syntax: jumpto pack[,level]";
    }

    if (!error.empty())
        client::Msg_ShowText(error, false, 2);
}

void Msg_Command_find(const string &text) {
    std::string indName = lev::Proxy::search_shallow(text);
    if (!indName.empty()) {
        lev::Index::setCurrentIndex(indName);
        lev::Index *searchResult = lev::Index::getCurrentIndex();
        searchResult->setCurrentPosition(0);
        if (searchResult->size() == 1) {
            // play unique level directly
            Msg_LoadLevel(searchResult->getProxy(0), false);
            Msg_Command("restart");
        } else {
            // display multiple levels as pack
            Msg_Command("abort");
        }
    } else {
        client::Msg_ShowText(string("Couldn't find '") + text + '\'', false, 2);
    }
}

void Msg_Command(const string &cmd) {
    lev::Index *ind = lev::Index::getCurrentIndex();
    lev::Proxy *curProxy = ind->getCurrent();

    // ------------------------------ normal commands
    if (cmd == "invrotate") {
        player::RotateInventory();
    } else if (cmd == "suicide") {
        player::Suicide();
        if (!AllowSuicide)
            Msg_RestartGame();
    } else if (cmd == "restart") {
        player::Suicide();
        Msg_RestartGame();
    } else if (cmd == "abort") {
        client::Msg_Command(cmd);
    }

    // ------------------------------ cheats
    else if (cmd == "god") {
        BroadcastMessage("_shield", player::CurrentPlayer(), GRID_NONE_BIT, true);
        client::Msg_Command("cheater");
    } else if (cmd == "collision") {
        NoCollisions = !NoCollisions;
        if (NoCollisions)
            client::Msg_ShowText("collision handling disabled", false, 2);
        else
            client::Msg_ShowText("collision handling enabled", false, 2);
        client::Msg_Command("cheater");
        display::GetStatusBar()->setCMode(NoCollisions);
    }

    // ------------------------------ quick options
    else if (cmd == "easy") {
        if (app.state->getInt("Difficulty") == DIFFICULTY_HARD) {
            if (curProxy->hasEasyMode()) {
                client::Msg_ShowText("Restarting in easy mode", false, 2);
                app.state->setProperty("Difficulty", DIFFICULTY_EASY);
                Msg_Command("restart");
            } else
                client::Msg_ShowText("No easy mode available.", false, 2);
        } else
            client::Msg_ShowText("Already in easy mode.", false, 2);
    } else if (cmd == "regular" || cmd == "noeasy") {
        if (app.state->getInt("Difficulty") == DIFFICULTY_EASY) {
            app.state->setProperty("Difficulty", DIFFICULTY_HARD);
            if (curProxy->hasEasyMode()) {
                client::Msg_ShowText("Restarting in regular difficulty mode", false, 2);
                Msg_Command("restart");
            } else {
                client::Msg_ShowText("No difference between easy and regular difficulty.", false,
                                     2);
            }
        } else
            client::Msg_ShowText("Already in normal mode.", false, 2);
    } else if (cmd == "hunt") {
        if (app.state->getInt("NextLevelMode") != lev::NEXT_LEVEL_NOT_BEST) {
            client::Msg_ShowText("Restarting in world record hunt mode", false, 2);
            app.state->setProperty("NextLevelMode", lev::NEXT_LEVEL_NOT_BEST);
            Msg_Command("restart");
        } else
            client::Msg_ShowText("Already in world record hunt mode.", false, 2);
    } else if (cmd == "nohunt") {
        if (app.state->getInt("NextLevelMode") == lev::NEXT_LEVEL_NOT_BEST) {
            client::Msg_ShowText("Switched back to standard mode", false, 2);
            app.state->setProperty("NextLevelMode", lev::NEXT_LEVEL_STRICTLY);
            client::Msg_Command("easy_going");
        } else
            client::Msg_ShowText("Already in standard mode.", false, 2);
    } else if (cmd == "info") {
        string infotext = ecl::strf("Level #%i of '", ind->getCurrentLevel()) + ind->getName() +
                          "' (" + curProxy->getAbsLevelPath() + ")  -  \"" + curProxy->getTitle() +
                          "\" by " + curProxy->getAuthor() +
                          ecl::strf(" (rev=%i,", curProxy->getReleaseVersion()) + "id=\"" +
                          curProxy->getId() + "\")";

        client::Msg_ShowText(infotext, true);
    } else if (cmd.substr(0, 5) == "find ") {  // global level-jump
        string args = cmd.substr(5);
        Msg_Command_find(args);
    } else if (cmd.substr(0, 7) == "jumpto ") {  // global level-jump
        string args = cmd.substr(7);
        Msg_Command_jumpto(args);
    } else if (cmd == "help") {
        client::Msg_ShowText(
            "suicide, restart, abort, easy, regular, hunt, nohunt, jumpto, find, info", true);
    } else if (cmd == "cheats") {
        client::Msg_ShowText("god, collision  -- Be aware: you'll get no medals!", true);
    } else if (cmd == "April 1st") {
        client::Msg_ShowText("No pizza, and a cloudless sky.", true);
    }

    else {
        enigma::Log << "Warning: Server received unknown command '" << cmd << "'\n";
    }
}

void Msg_Pause(bool onoff) {
    if (onoff && state == sv_running)
        state = sv_paused;
    else if (onoff && state == sv_teatime)
        state = sv_paused;
    else if (!onoff && state == sv_paused)
        state = sv_running;
}

void Msg_Teatime(bool onoff) {
    if (onoff) {
        state_before_teatime = state;
        state = sv_teatime;
    } else
        state = state_before_teatime;
}

void Msg_Panic(bool onoff) {
    if (onoff && state == sv_running)
        state = sv_idle;
    else if (!onoff && state == sv_idle)
        state = sv_running;
}

void Msg_MouseForce(const ecl::V2 &f) {
    SetMouseForce(f);
}

void SetCompatibility(const char *version) {
    GameType type = GetGameType(version);

    if (type == GAMET_UNKNOWN) {
        fprintf(stderr, "Invalid compatibility mode '%s' (ignored. using enigma behavior)\n",
                version);
        fprintf(stderr, "Valid modes:");
        for (int v = 0; v < GAMET_COUNT; ++v)
            fprintf(stderr, " %s", GetGameTypeName((GameType)v).c_str());
        fprintf(stderr, "\n");
        type = GAMET_ENIGMA;
    }

    GameCompatibility = type;
}

void SetCompatibility(lev::Proxy *levelProxy) {
    GameCompatibility = levelProxy->getEngineCompatibility();
    if (GameCompatibility == GAMET_UNKNOWN)
        throw XLevelLoading("unknown engine compatibility");
}

enigma::Difficulty GetDifficulty() {
    if (CreatingPreview)
        return DIFFICULTY_HARD;  // we may not access the current index!

    lev::Index *ind = lev::Index::getCurrentIndex();
    lev::Proxy *curProxy = ind->getCurrent();
    int i = app.state->getInt("Difficulty");
    if (i == DIFFICULTY_EASY && curProxy->hasEasyMode())
        return DIFFICULTY_EASY;
    else
        return DIFFICULTY_HARD;
}

void InitMoveCounter() {
    move_counter = 0;
}

int IncMoveCounter(int increment) {
    move_counter += increment;
    return move_counter;
}

int GetMoveCounter() {
    return move_counter;
}

void Msg_ActivateItem() {
    player::ActivateFirstItem();
}

}  // namespace server
}  // namespace enigma
