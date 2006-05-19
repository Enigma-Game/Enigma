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
#include "gui/MainMenu.hh"
#include "options.hh"
#include "oxyd.hh"
#include "sound.hh"
#include "video.hh"
#include "ecl_argp.hh"
#include "ecl_system.hh"
#include "world.hh"
#include "nls.hh"
#include "PreferenceManager.hh"
#include "lev/RatingManager.hh"

#include "enet/enet.h"

#include <locale.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/XMLRecognizer.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XercesVersion.hpp>


#ifdef MACOSX
// for search paths
#include "NSSystemDirectories.h"
#endif

using namespace std;
using namespace ecl;
using namespace enigma;
XERCES_CPP_NAMESPACE_USE 

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

/* -------------------- Functions -------------------- */

static void usage()
{
    printf("Usage: %s [options] [level files]\n\n"
           "Available options :\n\n"
           "    --nosound      Disable music and sound\n"
           "    --nomusic      Disable music\n"
           "    --window -w    Run in a window; do not enter fullscreen mode\n"
           "    --help -h      Show this help\n"
           "    --version      Print the executable's version number\n"
           "    --nograb       Do not use exclusive mouse/keyboard access\n"
           "    --data -d path Load data from additional directory\n"
           "    --lang -l lang Set game language\n"
           "    --pref -p file Use filename for preferences\n"
           "\n",
           app.progCallPath.c_str()
           );
}

namespace
{
    struct AP : public ecl::ArgParser {
    public:
        // Constructor
        AP();

        // Variables.
        bool nosound, nomusic, show_help, show_version, do_log, force_window;
        bool dumpinfo;
        string gamename;
        string datapath;
        string preffilename;
        std::vector<string> levelnames;

    private:
        enum {
            OPT_WINDOW, OPT_GAME, OPT_DATA, OPT_LANG, OPT_PREF
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
    nosound  = nomusic = show_help = show_version = do_log = force_window = false;
    dumpinfo = false;
    gamename = "";
    datapath = "";
    preffilename = PREFFILENAME;

    def (&nosound,              "nosound");
    def (&nomusic,              "nomusic");
    def (&show_version,         "version");
    def (&show_help,            "help", 'h');
    def (&WizardMode,           "wizard");
    def (&Nograb,               "nograb");
    def (&do_log,               "log");
    def (&dumpinfo,             "dumpinfo");
    def (&force_window,         "window", 'w');
    def (OPT_GAME,              "game", true);
    def (OPT_DATA,              "data", 'd', true);
    def (OPT_LANG,              "lang", 'l', true);
    def (OPT_PREF,              "pref", 'p', true);
}

void AP::on_option (int id, const string &param) 
{
    switch (id) {
    case OPT_GAME:
        gamename = param; 
        break;
    case OPT_DATA:
// we should be able to add several paths -- file.cc has does not yet support this
//        if (datapath.empty())
            datapath = param;
//         else
//             datapath = param  + ":" + datapath;
        break;
    case OPT_LANG:
        app.argumentLanguage = param;
        break;
    case OPT_PREF:
        preffilename = param;
        break;
    }
}

void AP::on_argument (const string &arg) 
{
    levelnames.push_back (arg);
}



/*! Initialize enough of the game to be able to show error messages in
  the window, not on the console. */


/*! Initialize the internationalization subsystem */
static void init_i18n ()
{
    // priorities:
    // language: command-line --- user option --- system (environment)
    // defaultLanguage: command-line --- system (environment)
    app.language = app.argumentLanguage;
    app.defaultLanguage = app.argumentLanguage;
    if (app.language == "") {
        options::GetOption("Language", app.language);
    }
    if (app.defaultLanguage == "") {
        app.defaultLanguage = ecl::SysMessageLocaleName();
        if (app.language == "") {
            app.language = app.defaultLanguage;
        }
    }
    

#if defined(ENABLE_NLS)

    nls::SetMessageLocale (app.language);

    bindtextdomain (PACKAGE_NAME, app.l18nPath.c_str());

    // SDL_ttf does not handle arbitrary encodings, so use UTF-8
    bind_textdomain_codeset (PACKAGE_NAME, "utf-8");
    textdomain (PACKAGE_NAME);
#endif

}

/* -------------------- Application -------------------- */

Application::Application()
: wizard_mode   (false),
  nograb        (false),
  language      (""),
  defaultLanguage (""),
  argumentLanguage ("")
{
}


void Application::init(int argc, char **argv) 
{
    progCallPath = argv[0];
    copy(argv+1, argv+argc, back_inserter(args));
    
    // parse commandline arguments -- needs args
    AP ap;
    ap.parse();

    // ----- Evaluate command line arguments.
    // start with simple actions that do not need further initialization
    if (ap.show_help || ap.show_version) {
        printf("Enigma %s\n",PACKAGE_VERSION);
        if (ap.show_help) usage();
        exit(0);
    }

    // initialize logfile -- needs ap
    if (ap.do_log) 
        enigma::Log.rdbuf(cout.rdbuf());
    else
        enigma::Log.rdbuf(::nullbuffer);

    // initialize system datapaths -- needs ap, log
    systemCmdDataPath = ap.datapath;
    initSysDatapaths(ap.preffilename);

    // initialize XML -- needs log, datapaths
    initXerces();
    
    // initialize LUA - Run initialization scripts
    lua_State *L = lua::GlobalState();
    lua::CheckedDoFile(L, app.systemFS, "startup.lua");

    // initialize preferences -- needs LUA, XML
    if (!options::Load()) {
        fprintf(stderr, _("Error in configuration file.\n"));
  	fprintf(stderr, lua::LastError (lua::GlobalState()).c_str());
    }     
    prefs = PreferenceManager::instance();
    
    if (ap.force_window) {
        options::SetOption("FullScreen", false);
    }

    // initialize user data paths -- needs preferences, system datapaths
    initUserDatapaths();
    
    // set message language
    init_i18n();
    
    // ----- Load level packs
//    lua_State *L = lua::GlobalState();
    if (ap.levelnames.empty()) {
        lua::Dofile(L, "levels/index.lua");
        lua::DoSubfolderfile(L, "levels", "index.lua");
        lua::Dofile(L, "levels/index_user.lua");
        levels::AddHistoryLevelPack ();
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

void Application::initSysDatapaths(const std::string &prefFilename)
{
    std::string progDir;          // directory path part of args[0]
    std::string progName;         // filename part of args[0]
    bool progDirExists = split_path(progCallPath, &progDir, &progName);
    std::string systemPath = SYSTEM_DATA_DIR; // substituted by configure.ac
    bool haveHome = (getenv("HOME") != 0);
#ifdef __MINGW32__
    // windows standard user specific application data directory path
    std::string winAppDataPath = ecl::ApplicationDataPath() + "/Enigma";
#endif

    // systemFS
    systemFS = new GameFS();
#ifdef __MINGW32__
    if (!progDirExists) {
        // filename only -- working dir should be on enigma as enigma
        // should never be located on exec path on windows
        systemAppDataPath = "./data";
    } else {
        // a call from elsewhere -- absolute or relative path does not matter.
        // this enables calls from a commandline.
        systemAppDataPath = progDir + "/data";
    }
#elif MACOSX
    // Mac OS X applications are self-contained bundles,
    // i.e., directories like "Enigma.app".  Resources are
    // placed in those bundles under "Enigma.app/Contents/Resources",
    // the main executable would be "Enigma.app/Contents/MacOS/enigma".
    // Here, we get the executable name, clip off the last bit, chdir into it,
    // then chdir to ../Resources. The original SDL implementation chdirs to
    // "../../..", i.e. the directory the bundle is placed in. This breaks
    // the self-containedness.
	
    systemAppDataPath = progDir + "/../Resources/data";
    
#else
    // Unix -- we get our data path from the installation
    systemAppDataPath = systemPath;
#endif
    systemFS->append_dir(systemAppDataPath);
    if (!systemCmdDataPath.empty())
         systemFS->prepend_dir(systemCmdDataPath);
    Log << "systemFS = \"" << systemFS->getDataPath() << "\"\n"; 
    
    // l18nPath
    l18nPath = LOCALEDIR;    // defined in src/Makefile.am
#ifdef __MINGW32__
    if (progDirExists) {
        l18nPath = progDir + "/" + l18nPath;
    }
#elif MACOSX
    l18nPath = progDir + "/../Resources/locale";
#endif
    Log << "l18nPath = \"" << l18nPath << "\"\n"; 
    
    
    // prefPath
    if (haveHome) {
        prefPath = ecl::ExpandPath("~");
        if (!ecl::FolderExists(prefPath))
            // may happen on Windows
            if(!ecl::FolderCreate(prefPath)) {
                fprintf(stderr, _("Error Home directory does not exist.\n"));
                exit(1);
            }
        userStdPath = prefPath + "/.enigma";
        prefPath = prefPath + ecl::PathSeparator + "." + prefFilename;
#ifdef __MINGW32__
    } else if (!winAppDataPath.empty()) {
        if (!ecl::FolderExists(winAppDataPath))
            // may happen on Windows
            if(!ecl::FolderCreate(winAppDataPath)) {
                fprintf(stderr, _("Error Application Data directory does not exist.\n"));
                exit(1);
            }
        Log << "winAppDataPath " << winAppDataPath << "\n";
        userStdPath = winAppDataPath;
        prefPath = winAppDataPath + ecl::PathSeparator + "." + prefFilename;
#endif
    } else {
        fprintf(stderr, _("Error Home directory does not exist.\n"));
        exit(1);
    }
    Log << "prefPath = \"" << prefPath << "\"\n"; 
}

void Application::initXerces() {
    // init XML
    try {
        // Initialize to en_US - we don't know the user prefs yet
        // If more than the error messages should be influenced we would
        // have to terminate and reinit after reading the user prefs.
        XMLPlatformUtils::Initialize();
        
        // Initialize transcoding service for XML <-> utf8
        XMLTransService::Codes initResult;        
        xmlUtf8Transcoder = XMLPlatformUtils::fgTransService->
                makeNewTranscoderFor(XMLRecognizer::UTF_8, initResult, 
                4096); // the block size is irrelevant for utf-8                  
        if (initResult != XMLTransService::Ok) {
            fprintf(stderr, _("Error in XML initialization.\n"));
            exit(1);
        }
        
        static const XMLCh ls[] = { chLatin_L, chLatin_S, chNull };
        static const XMLCh core[] = { chLatin_C, chLatin_O,  chLatin_R, chLatin_E, chNull };
        domImplementationLS = (DOMImplementationLS*)
                (DOMImplementationRegistry::getDOMImplementation(ls));
        domImplementationCore = (DOMImplementation*)
                (DOMImplementationRegistry::getDOMImplementation(core));
        domParserErrorHandler = new DOMErrorReporter(&Log);
        domParserSchemaResolver = new DOMSchemaResolver();
        domSerErrorHandler = new DOMErrorReporter(&Log);

#if _XERCES_VERSION >= 30000
        domParser = domImplementationLS->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
        DOMConfiguration *config = domParser->getDomConfig();

        config->setParameter(XMLUni::fgDOMNamespaces, true);
        config->setParameter(XMLUni::fgXercesSchema, true);
        config->setParameter(XMLUni::fgXercesSchemaFullChecking, true);
        config->setParameter(XMLUni::fgDOMValidate, true);
        config->setParameter(XMLUni::fgDOMDatatypeNormalization, true);
        config->setParameter(XMLUni::fgDOMErrorHandler, domParserErrorHandler);
        config->setParameter(XMLUni::fgDOMResourceResolver, domParserSchemaResolver);

        domSer = domImplementationLS->createLSSerializer();
        config = domSer->getDomConfig();
        
        config->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
        config->setParameter(XMLUni::fgDOMErrorHandler, domSerErrorHandler);

#else    
        domParser = domImplementationLS->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);

        domParser->setFeature(XMLUni::fgDOMNamespaces, true);
        domParser->setFeature(XMLUni::fgXercesSchema, true);
        domParser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);
        domParser->setFeature(XMLUni::fgDOMValidation, true);
        domParser->setFeature(XMLUni::fgDOMDatatypeNormalization, true);
        domParser->setErrorHandler(domParserErrorHandler);
        domParser->setEntityResolver(domParserSchemaResolver);

        domSer = domImplementationLS->createDOMWriter();
        domSer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
        domSer->setErrorHandler(domSerErrorHandler);
#endif
    }
    catch (...) {
        fprintf(stderr, _("Error in XML initialization.\n"));
        exit(1);
    }     
}	

void Application::setLanguage(std::string newLanguage)
{
    if (newLanguage == "") {
        language = defaultLanguage;
    }
    else {
        language = newLanguage;
    }
    nls::SetMessageLocale(language);
}

void Application::initUserDatapaths() {
    // userPath
//  if (no preference for user path)
        userPath = userStdPath;
//  else
//      userPath = value of user preference
    Log << "userPath = \"" << userPath << "\"\n"; 
    
    // resourceFS
    resourceFS = new GameFS();
    resourceFS->append_dir(systemAppDataPath);    
#ifdef MACOSX
    // set user-visible data paths -- use it for resource paths
    NSSearchPathEnumerationState cur=NSStartSearchPathEnumeration(NSLibraryDirectory, NSAllDomainsMask);
    char path[PATH_MAX];

    while(cur) {
        cur=NSGetNextSearchPathEnumeration(cur, path);
        resourceFS->prepend_dir(std::string(path)+"/Application Support/Enigma");
    }
#endif
    if (!systemCmdDataPath.empty())
	resourceFS->prepend_dir(systemCmdDataPath);    
    resourceFS->prepend_dir(userPath);    
    Log << "resourceFS = \"" << resourceFS->getDataPath() << "\"\n"; 

    // tmpPath
//  if (preference keep tmp local)
//        tmpPath = userStdPath;
//  else
        tmpPath = userPath;
}

/* -------------------- Functions -------------------- */

static void shutdown() 
{
    oxyd::Shutdown();
    world::Shutdown();
    video::Shutdown();
    display::Shutdown();
    sound::Shutdown();
    enet_deinitialize();
    lev::RatingManager::instance()->save();
    app.prefs->shutdown();
    options::Save();
    lua::ShutdownGlobal();
    delete_sequence(levels::LevelPacks.begin(),
                    levels::LevelPacks.end());
    XMLPlatformUtils::Terminate();
    delete ::nullbuffer;
}

int main(int argc, char** argv) 
{
    try {
        app.init(argc,argv);
        gui::ShowMainMenu();
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
