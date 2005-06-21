#include "gui.hh"
#include "video.hh"

namespace
{

/* -------------------- Server -> Client messages -------------------- */

    enum ClientCommand {
        cl_noop,
        cl_new_world,
        cl_level_loaded,
        cl_change_field,
        cl_add_actor,
        cl_move_actor,
        cl_focus_actor,
        cl_change_line,
        cl_play_sound,
        cl_show_text,
        cl_error                // error occurred
    };

    struct Message {
        Message (ClientCommand type_ = cl_noop) : type (type_) {
        }

        ClientCommand type;
    };

    enum ClientEffect {
    
    };

    struct Cl_NewWorld {
        string levelname;
        int width;
        int height;
    };

    struct Cl_LevelLoaded : public Message {
        Cl_LevelLoaded() : Message (cl_level_loaded) {
        }
    };

    struct Cl_ChangeField {


    };

    struct Cl_AddActor {

    };

    struct Cl_MoveActor {

    };

    struct Cl_FocusActor {
    
    };

    struct Cl_ShowText : public Message {
        Cl_ShowText() : Message(cl_show_text) {
        }

        string text;
        float duration;
        bool scrolling;
        bool interruptible;
    };

    struct Cl_AddEffect {
    
        float x, y;
    };

    struct Cl_PlaySound {
        string soundname;
        float x, y;
        int priority;
    };


    class GameMenu : public gui::Menu {
    public:
        GameMenu(int zoomxpos_, int zoomypos_);
        virtual ~GameMenu();
    private:
        bool on_event (const SDL_Event &e);
        void on_action(gui::Widget *w);
        void draw_background(px::GC &gc);

        gui::Widget *resume, *restart, *options, *abort;
        px::Surface *zoomed;
        int          zoomxpos, zoomypos; // position to be zoomed
    };


/* -------------------- Client class -------------------- */

    enum ClientState {
        cls_idle,
        cls_preparing_game,     // level loaded, currently updating the screen
        cls_game,
        cls_finished,            // level finished, waiting for next one
        cls_gamehelp,
        cls_gamemenu,
        cls_abort,
        cls_error
    };


    class Client {
    public:
        Client();
        ~Client();

        void tick (double dtime);
        void stop() { m_state = cls_idle; }

        void handle_message(Message *msg);

        void level_loaded(unsigned levelidx);
        void level_finished();

        void error (const std::string &text);

        void abort() { m_state = cls_abort; }
        bool abort_p() const { return m_state == cls_abort; }

        void mark_cheater() { m_cheater = true; }
        void easy_going() { m_hunt_against_time = false; }
    private:

        void init_hunted_time(int ilevel, string& hunted);


        /* ---------- Private methods ---------- */

        void show_menu();
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
        void user_input_append (char c);
        void user_input_backspace ();
        void user_input_previous ();
        void user_input_next ();


        // Variables
        ClientState m_state;
        string      m_levelname;
        double      m_timeaccu;

        double      m_total_game_time;
        int         m_hunt_against_time;
        bool        m_cheater;

        string      m_user_input;
        string      m_error_message;

        std::auto_ptr<video::TransitionEffect> m_effect;

    private:
        Client (const Client&);
        Client &operator = (const Client &);
    };
}

