/*
 * Copyright (C) 2004 Daniel Heck
 * Copyright (C) 2006,2007,2008,2009,2010 Ronald Lamprecht
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

#include "client.hh"
#include "game.hh"
#include "display.hh"
#include "options.hh"
#include "server.hh"
#include "gui/HelpMenu.hh"
#include "main.hh"
#include "gui/GameMenu.hh"
#include "SoundEngine.hh"
#include "SoundEffectManager.hh"
#include "MusicManager.hh"
#include "player.hh"
#include "resource_cache.hh"
#include "video.hh"
#include "world.hh"
#include "nls.hh"
#include "StateManager.hh"
#include "lev/Index.hh"
#include "lev/PersistentIndex.hh"
#include "lev/Proxy.hh"
#include "lev/RatingManager.hh"
#include "lev/ScoreManager.hh"

#include "ecl_font.hh"
#include "ecl_sdl.hh"
#include "ecl_video.hh"

#include "enet/enet.h"
#include "enet_ver.hh"

#include <cctype>
#include <cstring>

#include "client_internal.hh"

namespace enigma {
namespace client {

/* -------------------- Auxiliary functions -------------------- */

namespace {

/*! Display a message and change the current mouse speed. */
void set_mousespeed(double speed) {
    int s = ecl::round_nearest<int>(speed);
    options::SetMouseSpeed(s);
    s = ecl::round_nearest<int>(options::GetMouseSpeed());
    Msg_ShowText(ecl::strf(_("Mouse speed: %d"), s), false, 2.0);
}

/*! Generate the message that is displayed when the level starts. */
std::string displayedLevelInfo(lev::Proxy *level) {
    std::string text;
    std::string tmp;

    tmp = level->getLocalizedString("title");
    if (tmp.empty())
        tmp = _("Another nameless level");
    text = std::string("\"") + tmp + "\"";
    tmp = level->getAuthor();
    if (!tmp.empty())
        text += _(" by ") + tmp;
    tmp = level->getLocalizedString("subtitle");
    if (!tmp.empty() && tmp != "subtitle")
        text += std::string(" - \"") + tmp + "\"";
    tmp = level->getCredits(false);
    if (!tmp.empty())
        text += std::string(" - Credits: ") + tmp;
    tmp = level->getDedication(false);
    if (!tmp.empty())
        text += std::string(" - Dedication: ") + tmp;
    return text;
}

}  // namespace

/* -------------------- Variables -------------------- */

namespace {

Client client_instance;
const char HSEP = '^';  // history separator (use character that user cannot use)

}  // namespace

/* -------------------- Client class -------------------- */

Client::Client()
: m_state(cls_idle), m_levelname(), m_hunt_against_time(0), m_cheater(false), m_user_input() {
    m_network_host = 0;
}

Client::~Client() {
    network_stop();
}

void Client::init() {
    std::string command;
    for (int i = 0; i < 10; i++) {
        command = app.state->getString(ecl::strf("CommandHistory#%d", i).c_str());
        if (command.size() > 0)
            commandHistory.push_back(command);
        else
            break;
    }
}

void Client::shutdown() {
    for (int i = 0; i < commandHistory.size(); i++)
        app.state->setProperty(ecl::strf("CommandHistory#%d", i).c_str(),
                               commandHistory[i].c_str());
}

bool Client::network_start() {
    if (m_network_host)
        return true;
    m_network_host = enet_host_create(NULL, 1 /* only allow 1 outgoing connection */,
#ifdef ENET_VER_EQ_GT_13
                                      2 /* 2 channels are sufficient */,
#endif
                                      57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
                                      14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

    if (m_network_host == NULL) {
        fprintf(stderr, "An error occurred while trying to create an ENet client host.\n");
        return false;
    }

    // ----- Connect to server

    ENetAddress sv_address;
    ENetPeer *m_server;

    /* Connect to some.server.net:1234. */
    enet_address_set_host(&sv_address, "localhost");
    sv_address.port = 12345;

/* Initiate the connection, allocating the two channels 0 and 1. */
#ifdef ENET_VER_EQ_GT_13
    m_server = enet_host_connect(m_network_host, &sv_address, 2, 57600);
#else
    m_server = enet_host_connect(m_network_host, &sv_address, 2);
#endif

    if (m_server == NULL) {
        fprintf(stderr, "No available peers for initiating an ENet connection.\n");
        return false;
    }

    // Wait up to 5 seconds for the connection attempt to succeed.
    ENetEvent event;
    if (enet_host_service(m_network_host, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {
        fprintf(stderr, "Connection to some.server.net:1234 succeeded.");
        return true;
    } else {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset(m_server);
        m_server = 0;

        fprintf(stderr, "Connection to localhost:12345 failed.");
        return false;
    }
}

void Client::network_stop() {
    if (m_network_host)
        enet_host_destroy(m_network_host);
    if (m_server)
        enet_peer_reset(m_server);
}

/* ---------- Event handling ---------- */

void Client::handle_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        // TODO: If we want umlauts and other special characters ingame,
        //       we need to add SDL_TEXTINPUT and SDL_TEXTEDITING here.
        case SDL_KEYDOWN: on_keydown(e); break;
        case SDL_MOUSEMOTION:
            if (abs(e.motion.xrel) > 300 || abs(e.motion.yrel) > 300) {
                fprintf(stderr, "mouse event with %i, %i\n", e.motion.xrel, e.motion.yrel);
            } else
                server::Msg_MouseForce(options::GetDouble("MouseSpeed") *
                        ecl::V2(e.motion.xrel, e.motion.yrel));
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: on_mousebutton(e); break;
        case SDL_MOUSEWHEEL:
            if (e.wheel.y < 0) // mousewheel down: rotate inventory
                rotate_inventory(+1);
            if (e.wheel.y > 0) // mousewheel up: inverse rotate inventory
                rotate_inventory(+1);
            break;
        case SDL_WINDOWEVENT: {
            update_mouse_button_state();
            if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                // TODO(SDL2): is this sthe right event? The old code had
                // !video::IsFullScreen() as an additional check - necessary?
                show_menu(false);
            } else if (e.window.event == SDL_WINDOWEVENT_EXPOSED) {
                display::RedrawAll(video_engine->GetScreen());
            }
            break;
        }

        case SDL_QUIT:
            client::Msg_Command("abort");
            app.bossKeyPressed = true;
            break;
        }
    }
}

void Client::update_mouse_button_state() {
    int b = SDL_GetMouseState(0, 0);
    // TODO(sdl2): Should we replace SDL_BUTTON(1) by SDL_BUTTON(SDL_BUTTON_LEFT) etc.?
    player::InhibitPickup((b & SDL_BUTTON(1)) || (b & SDL_BUTTON(3)));
}

void Client::on_mousebutton(SDL_Event &e) {
    if (e.button.state == SDL_PRESSED) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            // left mousebutton -> activate first item in inventory
            server::Msg_ActivateItem();
        } else if (e.button.button == SDL_BUTTON_RIGHT) {
            // right mousebutton -> rotate inventory
            rotate_inventory(+1);
        }
    }
    update_mouse_button_state();
}

void Client::rotate_inventory(int direction) {
    m_user_input = "";
    display::GetStatusBar()->hide_text();
    player::RotateInventory(direction);
}

/* -------------------- Console related -------------------- */

void Client::process_userinput() {
    // no addition of existing commands to history
    if (consoleIndex == 1) {
        for (int i = 0; i < commandHistory.size(); i++) {
            if (newCommand == commandHistory[i]) {
                // take existing history command instead of new command
                consoleIndex = i + 2;
                break;
            }
        }
    }
    // resort history with selected command at bottom
    if (consoleIndex == 1) {
        if (commandHistory.size() < 10)
            commandHistory.push_back(std::string(""));
        for (int i = 8; i >= 0; i--) {
            if (i < commandHistory.size() - 1)
                commandHistory[i + 1] = commandHistory[i];
        }
    } else if (consoleIndex > 1) {
        newCommand = commandHistory[consoleIndex - 2];
        for (int i = consoleIndex - 3; i >= 0; i--) {
            if (i < commandHistory.size())
                commandHistory[i + 1] = commandHistory[i];
        }
    } else {  // document history or inventory
        return;
    }
    commandHistory[0] = newCommand;
    newCommand = "";
    consoleIndex = 0;
    display::GetStatusBar()->hide_text();
    server::Msg_Command(commandHistory[0]);
}

void Client::user_input_append(char c) {
    if (consoleIndex <= 0) {
        consoleIndex = 1;
        newCommand = c;
    } else if (consoleIndex == 1) {
        newCommand += c;
    } else {
        newCommand = commandHistory[consoleIndex - 2] + c;
        consoleIndex = 1;
    }
    Msg_ShowText(newCommand, false);
}

void Client::user_input_backspace() {
    if (consoleIndex == 1) {
        newCommand.erase(newCommand.size() - 1, 1);
        if (!newCommand.empty())
            Msg_ShowText(newCommand, false);
        else {
            consoleIndex = 0;
            display::GetStatusBar()->hide_text();
        }
    } else if (consoleIndex > 1) {
        newCommand = commandHistory[consoleIndex - 2];
        newCommand.erase(newCommand.size() - 1, 1);
        if (!newCommand.empty()) {
            consoleIndex = 1;
            Msg_ShowText(newCommand, false);
        } else {
            consoleIndex = 0;
            display::GetStatusBar()->hide_text();
        }
    }
}

void Client::user_input_previous() {
    if (consoleIndex < 0) {
        ++consoleIndex;
        int docIndex = documentHistory.size() + consoleIndex;
        if (docIndex < documentHistory.size()) {
            Msg_ShowText(documentHistory[docIndex], true);
        } else {
            consoleIndex = 0;
            display::GetStatusBar()->hide_text();
        }
    } else if (consoleIndex == 0) {
        if (newCommand.length() > 0) {
            consoleIndex = 1;
            Msg_ShowText(newCommand, false);
        } else if (commandHistory.size() > 0) {
            consoleIndex = 2;
            Msg_ShowText(commandHistory[0], false);
        }
    } else if (consoleIndex <= commandHistory.size()) {
        ++consoleIndex;
        Msg_ShowText(commandHistory[consoleIndex - 2], false);
    } else {  // top of history or new command without history
        consoleIndex = 0;
        display::GetStatusBar()->hide_text();
    }
}

void Client::user_input_next() {
    if (consoleIndex <= 0) {
        --consoleIndex;
        int docIndex = documentHistory.size() + consoleIndex;
        if (docIndex >= 0) {
            Msg_ShowText(documentHistory[docIndex], true);
        } else {
            consoleIndex = 0;
            display::GetStatusBar()->hide_text();
        }
    } else if (consoleIndex == 1 || (consoleIndex == 2 && newCommand.size() == 0)) {
        consoleIndex = 0;
        display::GetStatusBar()->hide_text();
    } else if (consoleIndex > 1) {
        --consoleIndex;
        Msg_ShowText(consoleIndex == 1 ? newCommand : commandHistory[consoleIndex - 2], false);
    }
}

void Client::on_keydown(SDL_Event &e) {
    SDL_Keycode keysym = e.key.keysym.sym;
    Uint16 keymod = e.key.keysym.mod;

    if (keymod & KMOD_CTRL) {
        switch (keysym) {
        case SDLK_a: server::Msg_Command("restart"); break;
        case SDLK_F3:
            if (keymod & KMOD_SHIFT) {
                // force a reload from file
                lev::Proxy::releaseCache();
                server::Msg_Command("restart");
            }
        default: break;
        };
    } else if (keymod & KMOD_ALT) {
        switch (keysym) {
        case SDLK_x: abort(); break;
        case SDLK_t:
            if (enigma::WizardMode) {
                ecl::Screen *scr = video_engine->GetScreen();
                ecl::TintRect(scr->get_surface(), display::GetGameArea(), 100, 100, 100, 0);
                scr->update_all();
            }
            break;
        case SDLK_s:
            if (enigma::WizardMode) {
                server::Msg_Command("god");
            }
            break;
        case SDLK_RETURN: {
            ScopedInputGrab(false);
            video_engine->ToggleFullscreen();
            sdl::FlushEvents();
        } break;
        default: break;
        };
    } else if (keymod & (KMOD_LGUI | KMOD_RGUI)) {
        switch (keysym) {
        case SDLK_q:  // Mac OS X application quit sequence
            app.bossKeyPressed = true;
            abort();
            break;
        default: break;
        };
    } else {
        switch (keysym) {
        case SDLK_ESCAPE:
            if (keymod & KMOD_SHIFT) {
                app.bossKeyPressed = true;
                abort();
            } else {
                show_menu(true);
            }
            break;
        case SDLK_LEFT: set_mousespeed(options::GetMouseSpeed() - 1); break;
        case SDLK_RIGHT: set_mousespeed(options::GetMouseSpeed() + 1); break;
        case SDLK_TAB: rotate_inventory(+1); break;
        case SDLK_F1: show_help(); break;
        case SDLK_F2:
            // display hint
            break;
        case SDLK_F3:
            if (keymod & KMOD_SHIFT)
                server::Msg_Command("restart");
            else
                server::Msg_Command("suicide");
            break;

        case SDLK_F4: Msg_AdvanceLevel(lev::ADVANCE_STRICTLY); break;
        case SDLK_F5: Msg_AdvanceLevel(lev::ADVANCE_UNSOLVED); break;
        case SDLK_F6: Msg_JumpBack(); break;

        case SDLK_F10: {
            lev::Proxy *level = server::LoadedProxy;
            std::string basename =
                std::string("screenshots/") + level->getLocalSubstitutionLevelPath();
            std::string fname = basename + ".png";
            std::string fullPath;
            int i = 1;
            while (app.resourceFS->findFile(fname, fullPath)) {
                fname = basename + ecl::strf("#%d", i++) + ".png";
            }
            std::string savePath = app.userImagePath + "/" + fname;
            video_engine->Screenshot(savePath);
            break;
        }
        case SDLK_RETURN: process_userinput(); break;
        case SDLK_BACKSPACE: user_input_backspace(); break;
        case SDLK_UP: user_input_previous(); break;
        case SDLK_DOWN: user_input_next(); break;
        default: {
            // SDL2's GetKeyName only returns uppercase keys.
            const char *key = SDL_GetKeyName(e.key.keysym.sym);
            const bool is_ascii = strlen(key) == 1 && ((key[0] & 0x7f) == key[0]);
            const bool capslock = keymod & KMOD_CAPS;
            if (is_ascii) {
                char ascii = key[0];
                if (isalnum(ascii) || strchr(" .-!\"$%&/()=?{[]}\\#'+*~_,;.:<>|",
                                             ascii)) { // don't add '^' or change history code
                    if (keymod & (KMOD_LSHIFT | KMOD_RSHIFT)) {
                        user_input_append(capslock ? std::tolower(ascii) : ascii);
                    } else {
                        user_input_append(capslock ? ascii : std::tolower(ascii));
                    }
                }
            }
            break;
        }
        }
    }
}

static const char *helptext_ingame[] = {
    N_("Left mouse button:"), N_("Activate/drop leftmost inventory item"),
    N_("Right mouse button:"), N_("Rotate inventory items"), N_("Escape:"), N_("Show game menu"),
    N_("Shift+Escape:"), N_("Quit game immediately"), N_("F1:"), N_("Show this help"), N_("F3:"),
    N_("Kill current marble"), N_("Shift+F3:"), N_("Restart the current level"), N_("F4:"),
    N_("Skip to next level"), N_("F5:"), 0,  // see below
    N_("F6:"), N_("Jump back to last level"), N_("F10:"), N_("Make screenshot"),
    N_("Left/right arrow:"), N_("Change mouse speed"), N_("Alt+x:"), N_("Return to level menu"),
    //    N_("Alt+Return:"),              N_("Switch between fullscreen and window"),
    0};

void Client::show_help() {
    server::Msg_Pause(true);
    ScopedInputGrab grab(false);

    helptext_ingame[17] = app.state->getInt("NextLevelMode") == lev::NEXT_LEVEL_NOT_BEST
                              ? _("Skip to next level for best score hunt")
                              : _("Skip to next unsolved level");

    video_engine->ShowMouse();
    gui::displayHelp(helptext_ingame, 200);
    video_engine->HideMouse();

    update_mouse_button_state();
    if (m_state == cls_game)
        display::RedrawAll(video_engine->GetScreen());

    server::Msg_Pause(false);
    game::ResetGameTimer();

    if (app.state->getInt("NextLevelMode") == lev::NEXT_LEVEL_NOT_BEST)
        server::Msg_Command("restart");  // inhibit cheating
}

void Client::show_menu(bool isESC) {
    if (isESC && server::LastMenuTime != 0.0 && server::LevelTime - server::LastMenuTime < 0.3) {
        return;  // protection against ESC D.o.S. attacks
    }
    if (isESC && server::LastMenuTime != 0.0 && server::LevelTime - server::LastMenuTime < 0.35) {
        server::MenuCount++;
        if (server::MenuCount > 10)
            mark_cheater();
    }

    server::Msg_Pause(true);

    ecl::Screen *screen = video_engine->GetScreen();

    ScopedInputGrab grab(false);

    video_engine->ShowMouse();
    {
        int x, y;
        display::GetReferencePointCoordinates(&x, &y);
        enigma::gui::GameMenu(x, y).manage();
    }
    video_engine->HideMouse();
    update_mouse_button_state();
    if (m_state == cls_game)
        display::RedrawAll(screen);

    server::Msg_Pause(false);
    game::ResetGameTimer();

    if (isESC)  // protection against ESC D.o.S. attacks
        server::LastMenuTime = server::LevelTime;
}

void Client::draw_screen() {
    switch (m_state) {
    case cls_error: {
        ecl::Screen *scr = video_engine->GetScreen();
        ecl::GC gc(scr->get_surface());
        blit(gc, 0, 0, enigma::GetImage("menu_bg", ".jpg"));
        ecl::Font *f = enigma::GetFont("menufont");

        std::vector<std::string> lines;

        ecl::split_copy(m_error_message, '\n', back_inserter(lines));
        int x = 60;
        int y = 60;
        int yskip = 25;
        const VMInfo *vminfo = video_engine->GetInfo();
        int width = vminfo->width - 120;
        for (unsigned i = 0; i < lines.size();) {
            std::string::size_type breakPos = ecl::breakString(f, lines[i], " ", width);
            f->render(gc, x, y, lines[i].substr(0, breakPos).c_str());
            y += yskip;
            if (breakPos != lines[i].size()) {
                // process rest of line
                lines[i] = lines[i].substr(breakPos);
            } else {
                // process next line
                i++;
            }
        }
        scr->update_all();
        scr->flush_updates();
        break;
    }
    default: break;
    }
}

std::string Client::init_hunted_time() {
    std::string hunted;
    m_hunt_against_time = 0;
    if (app.state->getInt("NextLevelMode") == lev::NEXT_LEVEL_NOT_BEST) {
        lev::Index *ind = lev::Index::getCurrentIndex();
        lev::ScoreManager *scm = lev::ScoreManager::instance();
        lev::Proxy *curProxy = ind->getCurrent();
        lev::RatingManager *ratingMgr = lev::RatingManager::instance();

        int difficulty = app.state->getInt("Difficulty");
        int wr_time = ratingMgr->getBestScore(curProxy, difficulty);
        int best_user_time = scm->getBestUserScore(curProxy, difficulty);

        if (best_user_time > 0 && (wr_time == -1 || best_user_time < wr_time)) {
            m_hunt_against_time = best_user_time;
            hunted = "you";
        } else if (wr_time > 0) {
            m_hunt_against_time = wr_time;
            hunted = ratingMgr->getBestScoreHolder(curProxy, difficulty);
        }

        // STATUSBAR->set_timerstart(-m_hunt_against_time);
    }
    return hunted;
}

void Client::tick(double dtime) {
    const double timestep = 0.01;  // 10ms

    switch (m_state) {
    case cls_idle: break;

    case cls_preparing_game: {
        if (m_effect && !m_effect->finished()) {
            m_effect->tick(dtime);
        } else {
            m_effect.reset();
            server::Msg_StartGame();

            m_state = cls_game;
            m_timeaccu = 0;
            m_total_game_time = 0;
            sdl::FlushEvents();
        }
        break;
    }

    case cls_game:
        if (app.state->getInt("NextLevelMode") == lev::NEXT_LEVEL_NOT_BEST) {
            int old_second = ecl::round_nearest<int>(m_total_game_time);
            int second = ecl::round_nearest<int>(m_total_game_time + dtime);

            if (m_hunt_against_time && old_second <= m_hunt_against_time) {
                if (second > m_hunt_against_time) {  // happens exactly once when par has passed by
                    lev::Index *ind = lev::Index::getCurrentIndex();
                    lev::ScoreManager *scm = lev::ScoreManager::instance();
                    lev::Proxy *curProxy = ind->getCurrent();
                    lev::RatingManager *ratingMgr = lev::RatingManager::instance();
                    int difficulty = app.state->getInt("Difficulty");
                    int wr_time = ratingMgr->getBestScore(curProxy, difficulty);
                    int best_user_time = scm->getBestUserScore(curProxy, difficulty);
                    std::string message;

                    if (wr_time > 0 && (best_user_time < 0 || best_user_time > wr_time)) {
                        message = std::string(_("Too slow for ")) +
                                  ratingMgr->getBestScoreHolder(curProxy, difficulty) +
                                  "... Ctrl-A";
                    } else {
                        message = std::string(_("You are slow today ... Ctrl-A"));
                    }

                    client::Msg_PlaySound("shatter", 1.0);
                    Msg_ShowText(message, true, 2.0);
                } else {
                    if (old_second < second &&                   // tick every second
                        (second >= (m_hunt_against_time - 5) ||  // at least 5 seconds
                                second >= ecl::round_nearest<int>(m_hunt_against_time * .8)))  // or the last
                                                                                   // 20% before par
                    {
                        client::Msg_PlaySound("pickup", 1.0);
                    }
                }
            }
        }

        m_total_game_time += dtime;
        display::GetStatusBar()->set_time(m_total_game_time);
    // fall through
    case cls_finished: {
        m_timeaccu += dtime;
        for (; m_timeaccu >= timestep; m_timeaccu -= timestep) {
            display::Tick(timestep);
        }
        display::Redraw(video_engine->GetScreen());
        handle_events();
        break;
    }

    case cls_gamemenu: break;
    case cls_gamehelp: break;
    case cls_abort: break;
    case cls_error: {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                app.bossKeyPressed = true;
            // fall through
            case SDL_KEYDOWN: client::Msg_Command("abort"); break;
            }
        }
    } break;
    }
}

void Client::level_finished() {
    lev::Index *ind = lev::Index::getCurrentIndex();
    lev::ScoreManager *scm = lev::ScoreManager::instance();
    lev::Proxy *curProxy = ind->getCurrent();
    lev::RatingManager *ratingMgr = lev::RatingManager::instance();
    int difficulty = app.state->getInt("Difficulty");
    int wr_time = ratingMgr->getBestScore(curProxy, difficulty);
    int best_user_time = scm->getBestUserScore(curProxy, difficulty);
    int par_time = ratingMgr->getParScore(curProxy, difficulty);

    int level_time = ecl::round_nearest<int>(m_total_game_time);

    std::string text;
    bool timehunt_restart = false;

    std::string par_name = ratingMgr->getBestScoreHolder(curProxy, difficulty);
    for (int cut = 2; par_name.length() > 55; cut++)
        par_name = ratingMgr->getBestScoreHolder(curProxy, difficulty, cut);

    if (wr_time > 0) {
        if (best_user_time < 0 || best_user_time > wr_time) {
            if (level_time == wr_time)
                text = std::string(_("Exactly the world record of ")) + par_name + "!";
            else if (level_time < wr_time)
                text = _("Great! A new world record!");
        }
    }
    if (text.length() == 0 && best_user_time > 0) {
        if (level_time == best_user_time) {
            text = _("Again your personal record...");
            if (app.state->getInt("NextLevelMode") == lev::NEXT_LEVEL_NOT_BEST)
                timehunt_restart = true;  // when hunting yourself: Equal is too slow
        } else if (level_time < best_user_time) {
            if (par_time >= 0 && level_time <= par_time)
                text = _("New personal record - better than par!");
            // Uncomment the following lines to show the "but over par!"-part.
            // (This has been criticised as demoralizing.)
            // else if (par_time >= 0)
            //    text = _("New personal record, but over par!");
            else
                text = _("New personal record!");
        }
    }

    if (app.state->getInt("NextLevelMode") == lev::NEXT_LEVEL_NOT_BEST &&
        (wr_time > 0 || best_user_time > 0)) {
        bool with_par = best_user_time == -1 || (wr_time > 0 && wr_time < best_user_time);
        int behind = level_time - (with_par ? wr_time : best_user_time);

        if (behind > 0) {
            if (best_user_time > 0 && level_time < best_user_time && with_par) {
                text = _("Your record, ");
            } else {
                text = "";
            }
            text += ecl::timeformat(behind);
            if (with_par)
                text += _("behind world record.");
            else
                text += _("behind your record.");

            timehunt_restart = true;  // time hunt failed -> repeat level
        }
    }

    if (text.length() == 0) {
        if (par_time >= 0 && level_time <= par_time)
            text = _("Level finished - better than par!");
        // Uncomment the following lines to show the "but over par!"-part.
        // (This has been criticised as demoralizing.)
        // else if (par_time >= 0)
        //    text = _("Level finished, but over par!");
        else
            text = _("Level finished!");
    }
    if (m_cheater)
        text += _(" Cheater!");

    Msg_ShowText(text, false);

    if (!m_cheater) {
        scm->updateUserScore(curProxy, difficulty, level_time);

        // save score (just in case Enigma crashes when loading next level)
        lev::ScoreManager::instance()->save();
    }

    if (timehunt_restart)
        server::Msg_Command("restart");
    else
        m_state = cls_finished;
}

void Client::level_loaded(bool isRestart) {
    lev::Index *ind = lev::Index::getCurrentIndex();
    lev::ScoreManager *scm = lev::ScoreManager::instance();
    lev::Proxy *curProxy = ind->getCurrent();

    // update window title
    video_engine->SetCaption(ecl::strf(_("Enigma pack %s - level #%d: %s"), ind->getName().c_str(),
                                       ind->getCurrentLevel(),
                                       curProxy->getTitle().c_str()).c_str());

    std::string hunted = init_hunted_time();  // sets m_hunt_against_time (used below)
    documentHistory.clear();
    consoleIndex = 0;

    // show level information (name, author, etc.)
    std::string displayed_info = "";
    if (m_hunt_against_time > 0) {
        if (hunted == "you")
            displayed_info = _("Your record: ");
        else
            displayed_info = _("World record to beat: ");
        displayed_info += ecl::timeformat(m_hunt_against_time);
        //+ _(" by ") +hunted;
        // makes the string too long in many levels
        Msg_ShowDocument(displayed_info, true, 4.0);
    } else {
        displayed_info = displayedLevelInfo(curProxy);
        Msg_ShowDocument(displayed_info, true, 2.0);
    }

    sound::StartLevelMusic();

    // start screen transition
    ecl::GC gc(video_engine->BackBuffer());
    display::DrawAll(gc);

    m_effect = video::CreateEffect((isRestart ? video::TM_NONE : video::TM_PUSH_RANDOM),
                                   video_engine->BackBuffer());
    m_cheater = false;
    m_state = cls_preparing_game;
}

void Client::handle_message(Message *m) {  // @@@ unused
    switch (m->type) {
    case CLMSG_LEVEL_LOADED: break;
    default: fprintf(stderr, "Unhandled client event: %d\n", m->type); break;
    }
}

void Client::error(const std::string &text) {
    m_error_message = text;
    m_state = cls_error;
    draw_screen();
}

void Client::registerDocument(std::string text) {
    documentHistory.push_back(text);
    consoleIndex = -1;
}

void Client::finishedText() {
    consoleIndex = 0;
}

/* -------------------- Functions -------------------- */

void ClientInit() {
    client_instance.init();
}

void ClientShutdown() {
    client_instance.shutdown();
}

bool NetworkStart() {
    return client_instance.network_start();
}

void Msg_LevelLoaded(bool isRestart) {
    client_instance.level_loaded(isRestart);
}

void Tick(double dtime) {
    client_instance.tick(dtime);
    sound::Tick(dtime);
}

void Stop() {
    client_instance.stop();
}

void Msg_AdvanceLevel(lev::LevelAdvanceMode mode) {
    lev::Index *level_index = lev::Index::getCurrentIndex();
    // log last played level
    lev::PersistentIndex::addCurrentToHistory();

    if (level_index->advanceLevel(mode)) {
        // now we may advance
        server::Msg_LoadLevel(level_index->getCurrent(), false);
    } else
        client::Msg_Command("abort");
}

void Msg_JumpBack() {
    // log last played level
    lev::PersistentIndex::addCurrentToHistory();
    server::Msg_JumpBack();
}

bool AbortGameP() {
    return client_instance.abort_p();
}

void Msg_Command(const std::string &cmd) {
    if (cmd == "abort") {
        client_instance.abort();
    } else if (cmd == "level_finished") {
        client::Msg_PlaySound("finished", 1.0);
        client_instance.level_finished();
    } else if (cmd == "cheater") {
        client_instance.mark_cheater();
    } else if (cmd == "easy_going") {
        client_instance.easy_going();
    } else {
        enigma::Log << "Warning: Client received unknown command '" << cmd << "'\n";
    }
}

void Msg_PlayerPosition(unsigned iplayer, const ecl::V2 &pos) {
    if (iplayer == (unsigned)player::CurrentPlayer()) {
        sound::SetListenerPosition(pos);
        display::SetReferencePoint(pos);
    }
}

void Msg_PlaySound(const std::string &wavfile, const ecl::V2 &pos, double relative_volume) {
    sound::EmitSoundEvent(wavfile.c_str(), pos, relative_volume);
}

void Msg_PlaySound(const std::string &wavfile, double relative_volume) {
    sound::EmitSoundEvent(wavfile.c_str(), ecl::V2(), relative_volume);
}

void Msg_Sparkle(const ecl::V2 &pos) {
    display::AddEffect(pos, "ring-anim", true);
}

void Msg_ShowText(const std::string &text, bool scrolling, double duration) {
    display::GetStatusBar()->show_text(text, scrolling, duration);
}

void Msg_ShowDocument(const std::string &text, bool scrolling, double duration) {
    client_instance.registerDocument(text);
    Msg_ShowText(text, scrolling, duration);
}

void Msg_FinishedText() {
    client_instance.finishedText();
}

void Msg_Error(const std::string &text) {
    client_instance.error(text);
}

}  // namespace client
}  // namespace enigma
