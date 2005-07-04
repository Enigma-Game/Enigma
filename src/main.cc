/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
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

#include "main.hh"
#include "display.hh"
#include "lua.hh"
#include "menus.hh"
#include "options.hh"
#include "oxyd.hh"
#include "sound.hh"
#include "video.hh"
#include "ecl_argp.hh"
#include "world.hh"
#include "nls.hh"

#include "enet/enet.h"

#include <locale.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

#ifdef MACOSX
// for CFLocale
#include "CoreFoundation/CoreFoundation.h"
// for search paths
#include "NSSystemDirectories.h"
#endif

using namespace std;
using namespace ecl;
using namespace enigma;

#ifdef WIN32
// LoadImage is a Syscall on Windows, which gets defined to LoadImageA
// or LoadImageW in winuser.h so we simply undefine it to use this
// name for one of the methods
#undef LoadImage
#endif

namespace
{
    class Nulbuf : public std::streambuf {};
    Nulbuf* nullbuffer = new Nulbuf;
}

/* -------------------- Variables -------------------- */

namespace enigma
{
    Application app;

    bool   WizardMode        = false;

    //! If true, do not ``grab'' the mouse and keyboard
    bool   Nograb            = false;
}

/*! The stream object that is used for logging messages.  As defined
  here, it is not connected to any file or buffer.
  
  (Note: I think writing to a stream without a streambuffer *should*
  be a no-op, but it leads to crashes with g++ 2.95.  to circumvent
  this, Log is initialized with a dummy streambuf in init(). ) */
std::ostream enigma::Log(0);

/*! This is the stream object that is used when logging to a file.  In
  this case, enigma::Log uses this object's streambuffer. */
static std::fstream logfile;


/* -------------------- Application -------------------- */

Application::Application()
: wizard_mode   (false),
  nograb        (false),
  language      ("")
{
}


void Application::init(int argc, char **argv) 
{
    progname = argv[0];
    copy(argv+1, argv+argc, back_inserter(args));
}



/* -------------------- Functions -------------------- */

static void usage()
{
    printf("Usage: %s [options] [level files]\n\n"
           "Available options :\n\n"
           "    --nosound      Disable music and sound\n"
           "    --nomusic      Disable music\n"
           "    --window       Run in a window; do not enter fullscreen mode\n"
           "    --help -h      Show this help\n"
           "    --version      Print the executable's version number\n"
           "    --nograb       Do not use exclusive mouse/keyboard access\n"
           "    --data -d path Load data from additional directory\n"
           "    --lang -l lang Set game language\n"
           "\n",
           app.progname.c_str()
           );
}

namespace
{
    struct AP : public ecl::ArgParser {
    public:
        // Constructor
        AP();

        // Variables.
        bool nosound, nomusic, show_help, show_version, do_log;
        bool dumpinfo;
        string gamename;
        std::vector<string> levelnames;

    private:
        enum {
            OPT_WINDOW, OPT_GAME, OPT_DATA, OPT_LANG
        };

        // ArgParser interface.
        void on_error (ErrorType t, const string &option) {
            cout << errormsg(t, option) << endl;
            show_help = true;
        }

        void on_option (int id, const string &param);
        void on_argument (const string &arg);

    };
}

AP::AP() : ArgParser (app.args.begin(), app.args.end())
{
    nosound  = nomusic = show_help = show_version = do_log = false;
    dumpinfo = false;
    gamename = "";

    def (&nosound,		"nosound");
    def (&nomusic,		"nomusic");
    def (&show_version, 	"version");
    def (&show_help,    	"help", 'h');
    def (&WizardMode,           "wizard");
    def (&Nograb,               "nograb");
    def (&do_log,       	"log", 'l');
    def (&dumpinfo,		"dumpinfo");
    def (OPT_WINDOW,    	"window",  'w');
    def (OPT_GAME,      	"game", 'l', true);
    def (OPT_DATA,              "data", 'd', true);
    def (OPT_LANG,              "lang", 'l', true);
}

void AP::on_option (int id, const string &param) 
{
    switch (id) {
    case OPT_WINDOW:  
        options::SetOption("FullScreen", false);
        break;
    case OPT_GAME:
        gamename = param; 
        break;
    case OPT_DATA:
        file::AddDataPath (param);
        break;
    case OPT_LANG:
        app.language = param;
        break;
    }
}

void AP::on_argument (const string &arg) 
{
    levelnames.push_back (arg);
}



/*! Initialize enough of the game to be able to show error messages in
  the window, not on the console. */
static void init_basic()
{
    // Initialize logfile 
    enigma::Log.rdbuf(::nullbuffer);

    // Set list of file search paths from compile option
    file::SetDataPath (DEFAULT_DATA_PATH);

#ifdef MACOSX
    // set user-visible data paths
    NSSearchPathEnumerationState cur=NSStartSearchPathEnumeration(NSLibraryDirectory, NSAllDomainsMask);
    char path[PATH_MAX];

    while(cur) {
        cur=NSGetNextSearchPathEnumeration(cur, path);
        file::AddDataPath(string(path)+"/Application Support/Enigma");
    }
#endif
	
    lua_State *L = lua::GlobalState();

    // Run initialization scripts
    if (lua::Dofile(L, "startup.lua") != 0) {
        fprintf(stderr, _("There was an error loading 'startup.lua'.\n"));
        fprintf(stderr, _("Your installation may be incomplete or invalid.\n"));
        exit (1);
    }

    // Load preferences
    if (!options::Load()) {
        fprintf(stderr, _("Error in configuration file.\n"));
  	fprintf(stderr, lua::LastError (lua::GlobalState()).c_str());
     }
}


static std::string get_system_locale ()
{
    string language;
#ifdef MACOSX
    // get the locale and export it to the environment
    CFLocaleRef locale = CFLocaleCopyCurrent();
    CFStringRef name = CFLocaleGetIdentifier(locale);
    language = CFStringGetCStringPtr(name, NULL);
    CFRelease(locale);
#endif //MACOSX
    return language;
}


/*! Initialize the internationalization subsystem */
static void init_i18n ()
{
    nls::Init();

    string language;
    if (language == "") {
        options::GetOption ("Language", language);
    }
    if (language == "") {
        language = get_system_locale ();
    }

#if defined(ENABLE_NLS)

    nls::SetMessageLocale (language);

    bindtextdomain (PACKAGE_NAME, LOCALEDIR);

    // SDL_ttf does not handle arbitrary encodings, so use UTF-8
    bind_textdomain_codeset (PACKAGE_NAME, "utf-8");
    textdomain (PACKAGE_NAME);
#endif

}

/*! Initialize the rest of the game */
static void init() 
{
    lua_State *L = lua::GlobalState();

    init_i18n();

    // ----- Evaluate command line arguments.
    AP ap;
    ap.parse();

    if (ap.show_help || ap.show_version) {
        printf("Enigma %s\n",PACKAGE_VERSION);
        if (ap.show_help) usage();
        exit(0);
    }

    if (ap.do_log) 
        enigma::Log.rdbuf(cout.rdbuf());

    // ----- Load level packs
    if (ap.levelnames.empty()) {
        lua::Dofile(L, "levels/index.lua");
        lua::DoSubfolderfile(L, "levels", "index.lua");
        lua::Dofile(L, "levels/index_user.lua");
    }
    else {
        levels::AddSimpleLevelPack (ap.levelnames, "Quick Test Levels");
    }



    // ----- Initialize object repositories
    world::Init();
    if (ap.dumpinfo) {
        world::DumpObjectInfo();
        exit(0);
    }

    // ----- Initialize SDL library
    int sdl_flags = SDL_INIT_VIDEO;
    if (enigma::WizardMode)
        sdl_flags |= SDL_INIT_NOPARACHUTE;
    if (SDL_Init(sdl_flags) < 0) {
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);
    SDL_EnableUNICODE(1);

    // ----- Initialize video subsystem
    video::Init();
    video::SetCaption ("Enigma v" PACKAGE_VERSION);
    video::SetMouseCursor(enigma::LoadImage("cur-magic"), 4, 4);
    video::ShowMouse();
    SDL_ShowCursor(0);

    // ----- Initialize sound subsystem
    lua::DoSubfolderfile (L, "sound", "sound.lua");
    if (ap.nosound)
        sound::DisableSound();
    else if (ap.nomusic)
        sound::DisableMusic();
    sound::Init();

    // ----- Initialize network layer
    if (enet_initialize () != 0) {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
        exit (1);
    }

    // ----- Load models
    display::Init();

    // ----- Load oxyd data files
    oxyd::Init();
    oxyd::ChangeSoundset(options::GetInt("SoundSet"), 1);


    enigma::Randomize();
}

static void shutdown() 
{
    oxyd::Shutdown();
    world::Shutdown();
    video::Shutdown();
    display::Shutdown();
    sound::Shutdown();
    enet_deinitialize();
    options::Save();
    lua::ShutdownGlobal();
    delete_sequence(levels::LevelPacks.begin(),
                    levels::LevelPacks.end());
    delete ::nullbuffer;
}

int main(int argc, char** argv) 
{

#ifdef MACOSX
    // Mac OS X applications are self-contained bundles,
    // i.e., directories like "Enigma.app".  Resources are
    // placed in those bundles under "Enigma.app/Contents/Resources",
    // the main executable would be "Enigma.app/Contents/MacOS/enigma".
    // Here, we get the executable name, clip off the last bit, chdir into it,
    // then chdir to ../Resources. The original SDL implementation chdirs to
    // "../../..", i.e. the directory the bundle is placed in. This breaks
    // the self-containedness.
    char parentdir[1024];
    char *c;

    strncpy ( parentdir, argv[0], sizeof(parentdir) );
    c = (char*) parentdir;

    while (*c != '\0')     /* go to end */
        c++;

    while (*c != '/')      /* back up to parent */
        c--;

    *c++ = '\0';           /* cut off last part (binary name) */

    chdir (parentdir);  /* chdir to the binary app's parent */
    chdir ("../Resources/"); /* chdir to the .app's parent */
#endif //MACOSX

    try {
        app.init(argc,argv);
        init_basic();
        init();
        ShowMainMenu();
        shutdown();
        return 0;
    }
    catch (ecl::XGeneric &e) {
        cerr << "Error: " << e.what() << endl;
    }
    catch (std::exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
    catch (...) {
        cerr << "Uncaught exception...\n";
    }
    return 1;
}
