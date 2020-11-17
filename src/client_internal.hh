/*
 * Copyright (C) 2003,2004,2005 Daniel Heck
 * Copyright (C) 2009 Ronald Lamprecht
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
#include "gui/Menu.hh"
#include "video_effects.hh"
#include "ecl_buffer.hh"
#include "enet/enet.h"
#include <vector>

namespace enigma {
namespace client {

/* -------------------- Server -> Client messages -------------------- */

enum ClientCommand {
    CLMSG_NOOP,
    CLMSG_NEW_WORLD,
    CLMSG_LEVEL_LOADED,
    CLMSG_CHANGE_FIELD,
    CLMSG_ADD_ACTOR,
    CLMSG_MOVE_ACTOR,
    CLMSG_FOCUS_ACTOR,
    CLMSG_CHANGE_LINE,
    CLMSG_PLAY_SOUND,
    CLMSG_SHOW_TEXT,
    CLMSG_ERROR  // error occurred
};

struct Message {
    Message(ClientCommand type_ = CLMSG_NOOP) : type(type_) {}

    ClientCommand type;
};

struct Cl_NewWorld {
    std::string levelname;
    int width;
    int height;
};

struct Cl_LevelLoaded : public Message {
    Cl_LevelLoaded() : Message(CLMSG_LEVEL_LOADED) {}
};

inline ecl::Buffer &operator<<(ecl::Buffer &b, const Cl_LevelLoaded &) {
    return b << Uint8(CLMSG_LEVEL_LOADED);
}

struct Cl_ChangeField {};

struct Cl_AddActor {};

struct Cl_MoveActor {};

struct Cl_FocusActor {};

struct Cl_ShowText : public Message {
    Cl_ShowText() : Message(CLMSG_SHOW_TEXT) {}

    std::string text;
    float duration;
    bool scrolling;
    bool interruptible;
};

struct Cl_AddEffect {
    float x, y;
};

struct Cl_PlaySound {
    std::string soundname;
    float x, y;
    int priority;
};

/* -------------------- Client class -------------------- */

enum ClientState {
    cls_idle,
    cls_preparing_game,  // level loaded, currently updating the screen
    cls_game,
    cls_finished,  // level finished, waiting for next one
    cls_gamehelp,
    cls_gamemenu,
    cls_abort,
    cls_error
};

class Client {
public:
    Client();
    ~Client();

    void init();
    void shutdown();

    void tick(double dtime);
    void stop() { m_state = cls_idle; }
    bool network_start();
    void network_stop();

    void handle_message(Message *msg);

    void level_loaded(bool isRestart);
    void level_finished();

    void error(const std::string &text);

    void abort() { m_state = cls_abort; }
    bool abort_p() const { return m_state == cls_abort; }

    void mark_cheater() { m_cheater = true; }
    void easy_going() { m_hunt_against_time = false; }
    void registerDocument(std::string text);
    void finishedText();

private:
    std::string init_hunted_time();

    /* ---------- Private methods ---------- */

    void show_menu(bool isESC);
    void show_help();

    // Screen update (state dependant)
    void draw_screen();

    // Event handling
    void handle_events();
    void on_keydown(SDL_Event &e);
    void on_mousebutton(SDL_Event &e);
    void update_mouse_button_state();

    // Inventory & command line
    void rotate_inventory(int direction);

    void process_userinput();
    void user_input_append(char c);
    void user_input_backspace();
    void user_input_previous();
    void user_input_next();

    // Variables
    ClientState m_state;
    std::string m_levelname;
    double m_timeaccu;

    double m_total_game_time;
    int m_hunt_against_time;
    bool m_cheater;

    std::string newCommand;
    std::vector<std::string> commandHistory;
    std::vector<std::string> documentHistory;
    int consoleIndex;

    std::string m_user_input;
    std::string m_error_message;

    std::unique_ptr<video::TransitionEffect> m_effect;
    ENetHost *m_network_host;
    ENetPeer *m_server;

private:
    Client(const Client &);
    Client &operator=(const Client &);
};

}  // namespace client
}  // namespace enigma
