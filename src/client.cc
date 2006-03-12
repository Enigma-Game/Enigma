/*
 * Copyright (C) 2004 Daniel Heck
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

#include "client.hh"
#include "display.hh"
#include "options.hh"
#include "server.hh"
#include "help.hh"
#include "main.hh"
#include "gui/menus.hh"
#include "sound.hh"
#include "player.hh"
#include "world.hh"
#include "nls.hh"

#include "ecl_sdl.hh"

#include "enet/enet.h"
#include "lev/Proxy.hh"

#include <cctype>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <iostream>

using namespace enigma::client;
using namespace ecl;
using namespace std;

#include "client_internal.hh"

using levels::LevelInfo;
using levels::LevelPack;


/* -------------------- Auxiliary functions -------------------- */

namespace
{
    /*! Display a message and change the current mouse speed. */
    void set_mousespeed (double speed)
    {
        int s = round_nearest<int>(speed);
        options::SetMouseSpeed (s);
        s = round_nearest<int> (options::GetMouseSpeed ());
        Msg_ShowText(strf(_("Mouse speed: %d"), s), false, 2.0);
    }

    /*! Generate the message that is displayed when the level starts. */
    string displayedLevelInfo (const LevelInfo &info)
    {
        string text;
        lev::Proxy *level = lev::Proxy::loadedLevel();
        // after complete switch to Proxy as levelloader the following
        // conditional can be abolished
        if (level) {
            std::string tmp;
            text = string("\"")+ level->getLocalizedString("titel")+"\"" +
                _(" by ") + level->getAuthor();
            tmp = level->getLocalizedString("subtitel");
            if (!tmp.empty() && tmp != "subtitel")
                text += string(" - ")+ tmp; 
        } else {
            text = (info.name.empty()) ?
                _("Another nameless level") : string("\"")+info.name+"\"";
            if (!info.author.empty())
                text += _(" by ") + info.author;
        }
        return text;
    }
}


/* -------------------- Variables -------------------- */

namespace
{
    Client     client_instance;
    const char HSEP = '^'; // history separator (use character that user cannot use)
}

#define CLIENT client_instance


/* -------------------- GameMenu -------------------- */

GameMenu::GameMenu (int zoomxpos_, int zoomypos_)
: zoomed(0),
  zoomxpos(zoomxpos_),
  zoomypos(zoomypos_)
{
    resume  = new gui::StaticTextButton(N_("Resume Level"), this);
    restart = new gui::StaticTextButton(N_("Restart Level"), this);
    options = new gui::StaticTextButton(N_("Options"), this);
    abort   = new gui::StaticTextButton(N_("Abort Level"), this);

    add(resume,     Rect(0,0,150,40));
    add(restart,    Rect(0,45,150,40));
#if 1
    add(options,    Rect(0,90,150,40));
    add(abort,      Rect(0,135,150,40));
#else
    add(abort,    Rect(0,90,150,40));
#endif
    center();
}

GameMenu::~GameMenu() {
    delete(zoomed);
}

void GameMenu::draw_background(ecl::GC &gc) 
{
    const video::VMInfo *vminfo = video::GetInfo();

    if (!zoomed) {
        Rect game_area   = display::GetGameArea();
        int  part_width  = game_area.w/3;
        int  part_height = (part_width*vminfo->height)/vminfo->width;

        if (part_height > game_area.h) {
            part_height = game_area.h/3;
            part_width  = (part_height*vminfo->width)/vminfo->height;
            assert(part_width <= game_area.w);
        }

        // randomly choose ball offset
        int x, y;
        for (int trials = 5; trials; --trials) {
            x = IntegerRand(0, 5);
            y = IntegerRand(0, 3);

            // try to avoid menu-ball overlap:
            if (x<2 || x>3 || y<1 || y>2 || (trials == 1)) {
                int ax = zoomxpos-game_area.x;
                int ay = zoomypos-game_area.y;

                // upper left corner of part
                x = ax/32-1-x;
                y = ay/32-1-y;

                // ensure part is inside game_area
                x = max(0, min(x, (game_area.w-part_width)/32-1));
                y = max(0, min(y, (game_area.h-part_height)/32-1));

                // adjust to game fields
                x = x*32+24;
                y = y*32+16;

                break;
            }
        }

        // Be sure to redraw everything, or actors may appear on top
        // of the stones (actors are drawn in one pass and only
        // clipped to the screen boundary).
        display::RedrawAll(video::GetScreen());

        // get the selected part from screen
//         SDL_Surface *back = video::GetScreen()->get_surface();
        Rect     src_area(game_area.x+x, game_area.y+y, part_width, part_height);
        Surface *src = Grab(video::GetScreen()->get_surface(), src_area);

        // zoom multiple times for softer image
//         const double stepsize = 0.3;
//         for (double zoom = 0.4; zoom < 0.9; zoom += stepsize) {
//             int      sx  = round_down<int>(zoom * vminfo->width);
//             int      sy  = round_down<int>(zoom * vminfo->height);
//             Surface *tmp = src->zoom(sx, sy);

//             delete src;
//             src = tmp;
//         }
        zoomed = src->zoom(vminfo->width, vminfo->height);
        delete src;
    }

    ecl::blit(gc, 0,0, zoomed);
}

bool GameMenu::on_event (const SDL_Event &e) 
{
    if (e.type == SDL_MOUSEBUTTONDOWN
        && e.button.button == SDL_BUTTON_RIGHT)
    {
        Menu::quit();
        return true;
    }
    return false;
}

void GameMenu::on_action(gui::Widget *w) {
    if (w == resume) {
        Menu::quit();
    }
    else if (w == abort) {
        client::Msg_Command("abort");
        Menu::quit();
    }
    else if (w == restart) {
        client::Stop ();
        server::Msg_LoadLevel (server::CurrentLevel);
        Menu::quit();
    }
    else if (w == options) {
        enigma::gui::ShowOptionsMenu (0);
        Menu::quit();
    }
}




/* -------------------- Client class -------------------- */

Client::Client() 
: m_state (cls_idle),
  m_levelname(),
  m_hunt_against_time(0),
  m_cheater(false), 
  m_user_input()
{
    m_network_host = 0;
}

Client::~Client() 
{
    network_stop();
}

bool Client::network_start()
{
    if (m_network_host)
        return true;

    m_network_host = enet_host_create (NULL,
                                       1 /* only allow 1 outgoing connection */,
                                       57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
                                       14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

    if (m_network_host == NULL) {
        fprintf (stderr, 
                 "An error occurred while trying to create an ENet client host.\n");
        return false;
    }


    // ----- Connect to server

    ENetAddress sv_address;
    ENetPeer *m_server;

    /* Connect to some.server.net:1234. */
    enet_address_set_host (&sv_address, "localhost");
    sv_address.port = 12345;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    m_server = enet_host_connect (m_network_host, &sv_address, 2);    
    
    if (m_server == NULL) {
       fprintf (stderr, 
                "No available peers for initiating an ENet connection.\n");
       return false;
    }
    
    // Wait up to 5 seconds for the connection attempt to succeed.
    ENetEvent event;
    if (enet_host_service (m_network_host, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        fprintf (stderr, "Connection to some.server.net:1234 succeeded.");
        return true;
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset (m_server);
        m_server = 0;

        fprintf (stderr, "Connection to localhost:12345 failed.");
        return false;
    }
}

void Client::network_stop ()
{
    if (m_network_host)
        enet_host_destroy (m_network_host);
    if (m_server)
        enet_peer_reset (m_server);
}


/* ---------- Event handling ---------- */

void Client::handle_events() 
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_KEYDOWN:
            on_keydown(e);
            break;
        case SDL_MOUSEMOTION:
            if (abs(e.motion.xrel) > 300 || abs(e.motion.yrel) > 300) {
                fprintf(stderr, "mouse event with %i, %i\n", e.motion.xrel, e.motion.yrel);
            }
            else
                server::Msg_MouseForce (options::GetDouble("MouseSpeed") *
                                        V2 (e.motion.xrel, e.motion.yrel));
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            on_mousebutton(e);
            break;
        case SDL_ACTIVEEVENT: {
            update_mouse_button_state();
            if (e.active.gain == 0 && !video::IsFullScreen())
                show_menu();
            break;
        }

        case SDL_VIDEOEXPOSE: {
            display::RedrawAll(video::GetScreen());
            break;
        }

        case SDL_QUIT:
            client::Msg_Command("abort");
            break;
        }
    }
}

void Client::update_mouse_button_state() 
{
    int b = SDL_GetMouseState(0, 0);
    player::InhibitPickup((b & SDL_BUTTON(1)) || (b & SDL_BUTTON(3)));
}

void Client::on_mousebutton(SDL_Event &e) 
{
    if (e.button.state == SDL_PRESSED) {
        if (e.button.button == 1) {
            // left mousebutton -> activate first item in inventory
            server::Msg_ActivateItem ();
        }
        else if (e.button.button == 3|| e.button.button == 4) {
            // right mousebutton, wheel down -> rotate inventory
            rotate_inventory(+1);
        }
        else if (e.button.button == 5) {
            // wheel down -> inverse rotate inventory
            rotate_inventory(-1);
        }
    }
    update_mouse_button_state();
}

void Client::rotate_inventory (int direction)
{
    m_user_input = "";
    STATUSBAR->hide_text();
    player::RotateInventory(direction);
}


/* -------------------- Console related -------------------- */

class HistoryProxy {
    static int    instances;
public:
    static string content;

    HistoryProxy();
    ~HistoryProxy() {
        if (!--instances) options::SetOption("History", content);
    }
};

string HistoryProxy::content;
int    HistoryProxy::instances = 0;

HistoryProxy::HistoryProxy() {
    if (!instances++) {
        content = options::GetString("History");
        if (content.find(HSEP) == string::npos) content = string(1, HSEP);
    }
}

static void user_input_history_append(const string& text, bool at_end = true) {
    HistoryProxy history;
    size_t       old_pos = history.content.find(string(1, HSEP)+text+HSEP);

    if (old_pos != string::npos)
        history.content.erase(old_pos, text.length()+1);

    if (at_end)
        history.content += text+HSEP;
    else
        history.content = string(1, HSEP)+text+history.content;
}

void Client::process_userinput()
{
    if (m_user_input != "") {
        STATUSBAR->hide_text();
        string commands = m_user_input;

        user_input_history_append(m_user_input);
        m_user_input = "";

        size_t sep_pos;
        while ((sep_pos = commands.find_first_of(';')) != string::npos) {
            string first_command = commands.substr(0, sep_pos);
            commands.erase(0, sep_pos+1);
            server::Msg_Command (first_command);
        }
        server::Msg_Command (commands); // last command
    }
}

void Client::user_input_append (char c) {
    m_user_input += c;
    Msg_ShowText (m_user_input, false);
}

void Client::user_input_backspace ()
{
    if (!m_user_input.empty()) {
        m_user_input.erase (m_user_input.size()-1, 1);
        if (!m_user_input.empty()) {
            // still not empty
            Msg_ShowText (m_user_input, false);
        } else {
            // empty
            STATUSBAR->hide_text();
        }
    }
}
void Client::user_input_previous ()
{
    HistoryProxy history;
    size_t       last_start = history.content.find_last_of(HSEP, history.content.length()-2);

    if (last_start != string::npos) {
        string prev_input = history.content.substr(last_start+1, history.content.length()-last_start-2);
        history.content.erase(last_start+1);
        user_input_history_append(m_user_input, false);
        m_user_input      = prev_input;

        if (m_user_input.empty())
            STATUSBAR->hide_text();
        else
            Msg_ShowText (m_user_input, false);
    }
}
void Client::user_input_next ()
{
    HistoryProxy history;
    size_t       first_end = history.content.find_first_of(HSEP, 1);

    if (first_end != string::npos) {
        string next_input = history.content.substr(1, first_end-1);
        history.content.erase(0, first_end);
        user_input_history_append(m_user_input);
        m_user_input      = next_input;

        if (m_user_input.empty())
            STATUSBAR->hide_text();
        else
            Msg_ShowText (m_user_input, false);
    }
}

void Client::on_keydown(SDL_Event &e) 
{
    SDLKey keysym = e.key.keysym.sym;
    SDLMod keymod = e.key.keysym.mod;

    if (keymod & KMOD_CTRL) {
        switch (keysym) {
        case SDLK_a:
            server::Msg_Command ("restart");
            break;
        default:
            break;
        };
    }
    else if (keymod & KMOD_ALT) {
        switch (keysym) {
        case SDLK_x: abort(); break;
        case SDLK_t:
            if (enigma::WizardMode) {
                Screen *scr = video::GetScreen();
                ecl::TintRect(scr->get_surface (), display::GetGameArea(), 
                             100, 100, 100, 0);
                scr->update_all();
            }
            break;
        case SDLK_s:
            if (enigma::WizardMode) {
                server::Msg_Command ("god");
            }
            break;
        case SDLK_RETURN:
            {
                video::TempInputGrab (false);
                video::ToggleFullscreen ();
                sdl::FlushEvents();
            }
            break;
        default:
            break;
        };
    }
    else {
        switch (keysym) {
        case SDLK_ESCAPE: show_menu(); break;
        case SDLK_LEFT:   set_mousespeed(options::GetMouseSpeed() - 1); break;
        case SDLK_RIGHT:  set_mousespeed(options::GetMouseSpeed() + 1); break;
        case SDLK_TAB:    rotate_inventory(+1); break;
        case SDLK_F1:     show_help(); break;
        case SDLK_F2:
            // display hint
            break;
        case SDLK_F3: 
            if (keymod & KMOD_SHIFT)
                server::Msg_Command ("restart");
            else
                server::Msg_Command ("suicide"); 
            break;

        case SDLK_F4: Msg_AdvanceLevel(levels::advance_strictly_next); break;
        case SDLK_F5: Msg_AdvanceLevel(levels::advance_unsolved); break;

        case SDLK_F10: {
            string fname = string("screenshots/");
            fname += server::CurrentLevelPack->get_info(server::CurrentLevel).filename;
            fname += ".png";
            video::Screenshot(fname);
            break;
        }
        case SDLK_RETURN: process_userinput(); break;
        case SDLK_BACKSPACE: user_input_backspace(); break;
        case SDLK_UP: user_input_previous(); break;
        case SDLK_DOWN: user_input_next(); break;
        default:
            if (e.key.keysym.unicode  && (e.key.keysym.unicode & 0xff80) == 0) {
                char ascii = static_cast<char>(e.key.keysym.unicode & 0x7f);
                if (isalnum (ascii) ||
                    strchr(" .-!\"$%&/()=?{[]}\\#'+*~_,;.:<>|", ascii)) // don't add '^' or change history code
                {
                    user_input_append(ascii);
                }
            }

            break;
        }
    }
}

static const char *helptext_ingame[] = {
    N_("Left mouse button:"),    N_("Activate/drop leftmost inventory item"),
    N_("Right mouse button:"),      N_("Rotate inventory items"),
    N_("Escape:"),                  N_("Show game menu"),
    N_("F1:"),                      N_("Show this help"),
    N_("F3:"),                      N_("Kill current marble"),
    N_("Shift+F3:"),                N_("Restart the current level"),
    N_("F4:"),                      N_("Skip to next level"),
    N_("F5:"),                      0, // see below
    N_("F10:"),                     N_("Make screenshot"),
    N_("Left/right arrow:"),        N_("Change mouse speed"),
    N_("Alt+x:"),                   N_("Return to level menu"),
    N_("Alt+Return:"),              N_("Switch between fullscreen and window"),
    0
};

void Client::show_help() 
{
    server::Msg_Pause (true);
    video::TempInputGrab grab(false);

    helptext_ingame[15] = options::GetBool("TimeHunting")
        ? _("Skip to next non-par level")
        : _("Skip to next unsolved level");

    video::ShowMouse();
    displayHelp(helptext_ingame, 200);
    video::HideMouse();

    update_mouse_button_state();
    if (m_state == cls_game)
        display::RedrawAll(video::GetScreen());

    server::Msg_Pause (false);
    game::ResetGameTimer();

    if (options::GetBool("TimeHunting")) 
        server::Msg_Command ("restart"); // inhibit cheating

}


void Client::show_menu() 
{
    server::Msg_Pause (true);

    ecl::Screen *screen = video::GetScreen();

    video::TempInputGrab grab (false);

    video::ShowMouse();
    {
        int x, y;
        display::GetReferencePointCoordinates(&x, &y);
        GameMenu(x, y).manage();
    }
    video::HideMouse();
    update_mouse_button_state();
    if (m_state == cls_game)
        display::RedrawAll(screen);

    server::Msg_Pause (false);
    game::ResetGameTimer();

    if (options::GetBool("TimeHunting")) 
        server::Msg_Command ("restart"); // inhibit cheating

}

void Client::draw_screen()
{
    switch (m_state) {
    case cls_error: {
        Screen *scr = video::GetScreen();
        GC gc (scr->get_surface());
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
        Font *f = enigma::GetFont("menufont");

        vector<string> lines;

        ecl::split_copy (m_error_message, '\n', back_inserter(lines));
        int x     = 60;
        int y     = 60;
        int yskip = 25;
        for (unsigned i=0; i<lines.size(); ++i) {
            f->render(gc, x,  y, lines[i].c_str());
            y += yskip;
        }
        scr->update_all();
        scr->flush_updates();
        break;
    }
    default:
        break;
    }
}


void Client::init_hunted_time(int ilevel, string& hunted) 
{
    m_hunt_against_time = 0;
    if (options::GetBool("TimeHunting")) {
        Level level(server::CurrentLevelPack, ilevel);
        int   difficulty     = options::GetDifficulty();
        int   par_time       = level.get_par_time(difficulty);
        int   best_user_time = level.get_best_user_time(difficulty);

        if (best_user_time>0 && (par_time == -1 || best_user_time<par_time)) {
            m_hunt_against_time = best_user_time;
            hunted              = "you";
        }
        else if (par_time>0) {
            m_hunt_against_time = par_time;
            hunted              = level.get_par_holder(difficulty);
        }

        // STATUSBAR->set_timerstart(-m_hunt_against_time);
    }
}

void Client::tick (double dtime) 
{
    const double timestep = 0.01; // 10ms

    switch (m_state) {
    case cls_idle:
        break;

    case cls_preparing_game: {
        video::TransitionEffect *fx = m_effect.get();
        if (fx && !fx->finished()) {
            fx->tick (dtime);
        }
        else {
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
        if (options::GetBool("TimeHunting")) {
            int old_second = round_nearest<int> (m_total_game_time);
            int second     = round_nearest<int> (m_total_game_time + dtime);

            if (m_hunt_against_time && old_second <= m_hunt_against_time) {
                if (second > m_hunt_against_time) { // happens exactly once when par has passed by
                    Level  level(server::CurrentLevelPack, server::CurrentLevel);
                    int    difficulty     = options::GetDifficulty();
                    int    par_time       = level.get_par_time (difficulty);
                    int    best_user_time = level.get_best_user_time (difficulty);
                    string message;

                    if (par_time>0 && (best_user_time<0 || best_user_time>par_time)) {
                        message = string(_("Too slow for "))+level.get_par_holder(difficulty)+".. [Ctrl-A]";
                    }
                    else {
                        message = string(_("You are slow today.. [Ctrl-A]"));
                    }

                    client::Msg_PlaySound("shatter", 1.0);
                    Msg_ShowText(message, false, 2.0);
                }
                else {
                    if (old_second<second && // tick every second
                        (second >= (m_hunt_against_time-5) || // at least 5 seconds
                         second >= round_nearest<int> (m_hunt_against_time*.8))) // or the last 20% before par
                    {
                        client::Msg_PlaySound("pickup", 1.0);
                    }
                }
            }
        }

        m_total_game_time += dtime;
        STATUSBAR->set_time (m_total_game_time);
        // fall through
    case cls_finished: {
        m_timeaccu += dtime;
        for (;m_timeaccu >= timestep; m_timeaccu -= timestep) {
            display::Tick (timestep);
        }
        display::Redraw(video::GetScreen());
        handle_events();
        break;
    }

    case cls_gamemenu:
        break;
    case cls_gamehelp:
        break;
    case cls_abort:
        break;
    case cls_error: 
        {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                switch (e.type) {
                case SDL_KEYDOWN:
                case SDL_QUIT:
                    client::Msg_Command("abort");
                    break;
                }
            }
        }
        break;
    }
}

void Client::level_finished() 
{
    Level level (server::CurrentLevelPack, server::CurrentLevel);

    int difficulty = options::GetDifficulty();

    int    level_time     = round_nearest<int> (m_total_game_time);
    int    par_time       = level.get_par_time (difficulty);
    string par_name       = level.get_par_holder (difficulty);
    int    best_user_time = level.get_best_user_time (difficulty);

    string      text;
    bool        timehunt_restart = false;

    if (par_time > 0) {
        if (best_user_time<0 || best_user_time>par_time) {
            if (level_time == par_time)
                text = string(_("Exactly the par time by "))+par_name+"!";
            else if (level_time<par_time)
                text = _("Great! A new par time!");
        }
    }
    if (text.length() == 0 && best_user_time>0) {
        if (level_time == best_user_time) {
            text = _("Again your personal best time...");
            timehunt_restart = true; // when hunting yourself: Equal is too slow
        }
        else if (level_time<best_user_time)
            text = _("New personal best time!");
    }

    if (options::GetBool("TimeHunting") &&
        (par_time>0 || best_user_time>0))
    {
        bool with_par = best_user_time == -1 || (par_time >0 && par_time<best_user_time);
        int  behind   = level_time - (with_par ? par_time : best_user_time);

        if (behind>0) {
            if (best_user_time>0 && level_time<best_user_time && with_par) {
                text = _("Your best, ");
            }
            else {
                text = "";
            }
            text += strf("%d:%02d ", static_cast<int> (behind/60)%100, behind%60);
            if (with_par) 
                text += _("behind par time.");
            else
                text += _("behind best time.");

            timehunt_restart = true; // time hunt failed -> repeat level
        }
    }

    if (text.length() == 0)
        text = _("Level finished!");
    if (m_cheater)
        text += _(" Cheater!");

    Msg_ShowText (text, false);

    if (!m_cheater) {
        level.set_level_time (options::GetDifficulty(), level_time);

        if (options::LevelStatusChanged) {
            // save options (just in case Enigma crashes when loading next level)
            options::Save();
        }
    }

    if (timehunt_restart)
        server::Msg_Command("restart");
    else 
        m_state = cls_finished;
}

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

void Client::level_loaded (unsigned ilevel)
{
    LevelPack *lp = server::CurrentLevelPack;

    // update window title
    const LevelInfo &info = lp->get_info(ilevel);

    char       buffer[100];
    snprintf (buffer, sizeof(buffer), _("Enigma level #%d: %s"), 
              ilevel+1, info.name.c_str());
    video::SetCaption(buffer);

    string hunted;
    init_hunted_time(ilevel, hunted);   // sets m_hunt_against_time (used below)

    // show level information (name, author, etc.)
    {
        string displayed_info = "";
        if (m_hunt_against_time>0) {
            if (hunted == "you")
                displayed_info = _("Your best time: ");
            else
                displayed_info = _("Par to beat: ");
            displayed_info += strf("%d:%02d", (m_hunt_against_time/60)%100, m_hunt_against_time%60);
//+ _(" by ") +hunted;
// makes the string too long in many levels
            Msg_ShowText (displayed_info, false, 4.0);
        }
        else {
            displayed_info = displayedLevelInfo(info);
            Msg_ShowText (displayed_info, true, 2.0);
        }
    }

    sound::FadeoutMusic();
    if (options::GetBool("InGameMusic")) {
        sound::PlayMusic (options::GetString("LevelMusicFile"));
    } else {
        sound::StopMusic();
    }

    // start screen transition

    GC gc(video::BackBuffer());
    display::DrawAll(gc);

    m_effect.reset (video::MakeEffect (video::TM_PUSH_RANDOM, video::BackBuffer()));
    m_cheater = false;
    m_state   = cls_preparing_game;
}


void Client::handle_message (Message *m) { // @@@ unused
    switch (m->type) {
    case CLMSG_LEVEL_LOADED:

        break;
    default:
        fprintf (stderr, "Unhandled client event: %d\n", m->type);
        break;
    }
}

void Client::error (const string &text) 
{
    m_error_message = text;
    m_state = cls_error;
    draw_screen();
}


/* -------------------- Functions -------------------- */


bool client::NetworkStart() 
{
    return CLIENT.network_start();
}

void client::Msg_LevelLoaded (unsigned levelidx) 
{
    CLIENT.level_loaded(levelidx);
}

void client::Tick (double dtime) {
    CLIENT.tick (dtime);
    sound::Tick (dtime);
}

void client::Stop() {
    CLIENT.stop ();
}

void client::Msg_AdvanceLevel (levels::LevelAdvanceMode mode) {
    Level level (server::CurrentLevelPack, server::CurrentLevel);

    if (advance_level (level, mode)) {
        // log last played level
        levels::AddHistory(server::CurrentLevelPack, server::CurrentLevel);
        // now we may advance
        server::Msg_LoadLevel (level.get_index());
    }
    else
        client::Msg_Command("abort");
}

bool client::AbortGameP() {
    return CLIENT.abort_p();
}

void client::Msg_Command(const string& cmd) {
    if (cmd == "abort") {
        CLIENT.abort();
    }
    else if (cmd == "level_finished") {
        client::Msg_PlaySound("finished", 1.0);
        CLIENT.level_finished();
    }
    else if (cmd == "cheater") {
        CLIENT.mark_cheater();
    }
    else if (cmd == "easy_going") {
        CLIENT.easy_going();
    }
    else {
        enigma::Log << "Warning: Client received unknown command '" << cmd << "'\n";
    }
}

void client::Msg_PlayerPosition (unsigned iplayer, const V2 &pos) 
{
    if (iplayer == (unsigned)player::CurrentPlayer()) {
        sound::SetListenerPosition (pos);
        display::SetReferencePoint (pos);
    }
}

void client::Msg_PlaySound (const std::string &wavfile, 
                            const ecl::V2 &pos,
                            double relative_volume)
{
    sound::SoundEvent (wavfile.c_str(), pos, relative_volume);
}

void client::Msg_PlaySound (const std::string &wavfile, double relative_volume)
{
    sound::SoundEvent (wavfile.c_str(), V2(), relative_volume);
}

void client::Msg_Sparkle (const ecl::V2 &pos) {
    display::AddEffect (pos, "ring-anim");
}


void client::Msg_ShowText
(const std::string &text, bool scrolling, double duration)
{
    STATUSBAR->show_text (text, scrolling, duration);
}

void client::Msg_Error (const std::string &text)
{
    CLIENT.error (text);
}
