/*
 * Copyright (C) 2002,2003,2004,2005,2006 Daniel Heck
 * Copyright (C) 2006,2007,2008,2009 Ronald Lamprecht
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

#include "gui/MainMenu.hh"
#include "gui/LevelMenu.hh"
#include "gui/SearchMenu.hh"
#include "gui/OptionsMenu.hh"
#include "gui/InfoMenu.hh"
#include "gui/LanguageMenu.hh"
#include "gui/LevelPackMenu.hh"
#include "gui/LevelPreviewCache.hh"
#include "display.hh"
#include "ecl_font.hh"
#include "ecl_system.hh"
#include "main.hh"
#include "nls.hh"
#include "options.hh"
#include "resource_cache.hh"
#include "MusicManager.hh"
#include "world.hh"

#include "netgame.hh"

using namespace ecl;
using namespace std;

namespace enigma { namespace gui {

    /* -------------------- NetworkMenu -------------------- */

    NetworkMenu::NetworkMenu ()
    {
        const VMInfo *vminfo = video_engine->GetInfo();

        BuildVList b(this, Rect((vminfo->width - 150)/2,150,150,40), 5);
        startgame = b.add(new StaticTextButton(N_("Start Game"), this));
        m_joingame = b.add(new StaticTextButton(N_("Join Game"), this));
        m_back = b.add(new StaticTextButton(N_("Back"), this));
    }

    NetworkMenu::~NetworkMenu ()
    {
    }

    bool NetworkMenu::on_event (const SDL_Event &e)
    {
        return false;
    }

    void NetworkMenu::on_action(gui::Widget *w)
    {
        if (w == startgame) {
            netgame::Start();
        }
        else if (w == m_joingame) {
            netgame::Join("localhost", 12345);
        }
        if (w == m_back)
            Menu::quit();
    }

    void NetworkMenu::draw_background(ecl::GC &gc)
    {
        set_caption(_("Enigma - Network Menu"));
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
    }

    void NetworkMenu::tick(double dtime)
    {
    }


    /* -------------------- Help menu -------------------- */
    static const char *credit_text[] = {
        N_("Main developers of the 1.30 release:"),
        " ",
        "  THOMAS BERNHARDT",
        "  RAOUL BOURQUIN",
        N_("  DANIEL HECK (project founder)"),
        "  GEORG HONOLD",
        N_("  ANDREAS LOCHMANN (lead)"),
        "  NOBBY",
        " ",
        N_("Special Thanks To:"),
        " ",
        N_("  RONALD LAMPRECHT (lead up to 1.1)"),
        N_("  MEINOLF SCHNEIDER (game idea, level design)"),
        " ",
        N_("Please refer to the manual or the next pages for full credits."),
        " ",
        N_("Home Page: https://www.nongnu.org/enigma   Contact: enigma-devel@nongnu.org"),
        " ",
        N_("Enigma is free software and may be distributed under the terms of the GNU General Public License, version 2."),
        N_("Copyright (C) 2002-2021 Daniel Heck and contributors."),
        "\n",
        N_("Main developers of all releases:"),
        " ",
        "  Thomas Bernhardt  (Level design and testing, movies)",
        "  Raoul Bourquin  (Level admin. & design, all over invaluable additions)",
        "  Siegfried Fennig  (Level design, graphics)",
        "  Martin Hawlisch  (Level design, graphics, programming)",
        "  Daniel Heck  (Main developer up to 0.92, graphics, documentation)",
        "  Georg Honold  (Level administration and design)",
        "  Ronald Lamprecht  (Main developer up to 1.1, XML, new API, engine, portability, GUI, documentation)",
        "  Andreas Lochmann  (Programming, level admin. & design, documentation)",
        "  Petr Machata  (Level design, programming)",
        "  Nobby  (Level design and testing, score evaluation)",
        "  Nat Pryce  (Level design)",
        "  Jacob Scott  (Level design)",
        "  Sven Siggelkow  (Level design and special Oxyd expertise)",
        "  Ralf Westram  (Programming, level design)",
        "\n",
        N_("Special Thanks:"),
        " ",
        "  Johannes Fortmann  (Mac OS X port, some programming, graphics)",
        "  illmind  (forum mag-heut.net administration, level design)",
        // waiting for licence to add the sounds
        // "  \'Cellar of Rats\'  (Sound effects)",
        "  Sidney Markowitz  (Mac OS X port and Homebrew Formula)",
        "  Jennifer Robertson  (graphics second generation)",
        "  Jeremy Sawicki  (Oxydlib)",
        "  Erich Schubert  (Debian/Ubuntu packages, level design)",
        "  Andrew \'Necros\' Sega  (menu music \'Pentagonal Dreams\')",
        "  David W. Skinner  (many Sokoban Levels)",
        "  Clifford J. Tasner  (music second generation, proof reading)",
        "  Michał Trzebiatowski  (translation administration, German and Polish translations)",
        "  Юрий Жиромский - Yuriy Zhyromskiy  (Russian and Ukrainian translations, Russian manual and homepage translation, Slackware package)",
        "\n",
        N_("Contributors"),
        " ",
        "  Andreas Abraham  (German translation, code clean-up, bug fixes)",
        "  Samuel Elías Martínez Álvarez  (Spanish translation for 0.92)",
        "  Another  (Level design)",
        "  Γιάννης Ανθυμίδης - Yannis Anthymidis  (Greek translation)",
        "  Sébastien Aperghis-Tramoni  (French translation)",
        "  Ash  (Level design)",
        "  Claudio B.  (Italian translation)",
        "  Андрій Бандура  (Ukrainian translation)",
        "  Roberto Bardin  (Level design)",
        "  C.W. Betts  (Updates to .gitignore and .DS_Store)",
        "  George Birbilis  (Greek translation)",
        "  Helge Blohmer  (Level design)",
        "  Nathan Bronecke  (Level design)",
        "  Harry Bruder  (Level design, LotM articles)",  // daydreamer
        "  Alain Busser  (Level design, French translation, manual)",
        "  Guy Busser  (Level design)",
        "  Richi Bützer  (Level design)",
        "  Espen Bye  (Norwegian translation)",
        "  Guglielmo Cafaggi  (Italian translation)",
        "  Calinou  (French translation)",
        "  capkoh  (Level design)",
        "  Manfredi Carta  (Level design)",
        "  Yuri Chornoivan  (Ukrainian translation)",
        "  Christoph & Anita  (Level design)",
        "  Dan Church (Fix locale directory)",
        "  David D  (French translation)",
        "  Dawid Job  (Polish translation)",
        "  Дремук Сергей - Serge Dremuk  (Russian and Ukrainian translations)",
        "  Joseph Dunne  (Level design)",
        "  Xerxes M. Dynatos  (Level design)",
        "  Edward  (Level design)",
        "  Els van Eijk  (Dutch translation)",
        "  Manuel Eisentraut  (Level design)",
        "  Stephanie Fabian  (Invaluable bug reports)",
        "  Irina Fedulova  (Russian translation)",
        "  Johann Freymuth  (Level design, invaluable bug reports)",
        "  fri  (Czech translation)",
        "  firespin  (Greek translation)",
        "  Stefan Gagner  (Swedish translation)",
        "  Roberto García  (Spanish translation)",
        "  João Gaspar  (Portuguese translation)",
        "  Andy Geldmacher  (Level design)",
        "  Helmut Grohne  (Patch to ease cross compilation)",
        "  Edwin Groothuis  (FreeBSD port)",
        "  GunChleoc  (Scottish Gaelic translation, fixes to localization)",
        "  Daniel Hadas  (Level design)",
        "  Jonatan Hadas  (Level design)",
        "  Hairball  (Level design)",
        "  Joe Hansen  (Danish translation)",
        "  Haris  (Bosnian translation)",
        "  hasufell  (Improved enet support)",
        "  heiko123abc  (Higher resolution modes)",
        "  Immanuel Herrmann  (Level design)",
        "  Zoltan Hoppár  (Hungarian translation)",
        "  Tea Horvatic  (Croatian translation)",
        "  Michal Hriň  (Slovak translation)",
        "  Brian Huffman  (Level design)",
        "  Johannes Hüsing  (Level design)",
        "  Hypocrite  (Finnish translation)",
        "  Icewing  (Level design)",
        "  ItTheIt  (Level design)",
        "  jokker87  (Fixed original oxyd shuffle)",
        "  Máté Lehel Juhász  (Hungarian translation)",
        "  Stian Jørgensrud  (Norwegian translation)",
        "  Samuele Kaplun  (Italian translation)",
        "  Jorma Karvonen  (Finnish translation)",
        "  Ivica Kolić  (Croatian translation)",
        "  Manuel König  (Level design, bug reports)",
        "  Jens-Christian Korth  (Level design)",
        "  Håvard Kristoffersen  (Norwegian translation)",
        "  Johannes Laire  (Level design)",
        "  Joona Laire  (Level design)",
        "  Markus Laire  (Level design)",
        "  Jürgen Lange  (Level design)",
        "  Legimet  (Fix build error with GCC 6)",
        "  Dominik Lehmann  (Level design)",
        "  Roger Leigh  (Fix invalid conversions to double)",
        "  Dominik Leipold  (Level design)",
        "  Ryan Lerch  (AppData file)",
        "  Edward Leuf  (Feedback, bug reports)",
        "  Christophe Lherieau  (French translation)",
        "  Ricky Qi Li  (Chinese translation)",
        "  Lasse Liehu  (Finnish translation)",
        "  Ingo van Lil  (Feedback, bug reports)",
        "  Hugo Locurcio  (French translation)",
        "  Frank van der Loo  (Dutch translation)",
        "  Ľuboslava  (Slovak translation)",
        "  luigif  (Italian translation)",
        "  Dzmitry Ludzik  (Belorussian and Russian translations)",
        "  Lurcane  (Level design)",
        "  Riccardo Di Maio  (Italian translation)",
        "  Wasilis Mandratzis-Walz  (Greek translation)",
        "  Dmitry Marakasov  (Fix to meditation bug)",
        "  Sérgio Marques  (Portuguese translation)",
        "  Mat Mat  (Swedish translation)",
        "  MatteoMat01  (Italian translation)",
        "  Barry & Lori Mead  (Level design)",
        "  Adnan Memija  (Bosnian translation)",
        "  Merarom  (Swedish translation)",
        "  Linda Mihalic  (English proof reading)",
        "  moonpearl  (Level design)",
        "  Carlos Moreira  (Portuguese translation)",
        "  Stephen Morley (Safalra)  (Level design)",
        "  Fumio Nakayama  (Japanese translation)",
        "  nanook  (Level design)",
        "  Krishnamurti Lelis Lima Vieira Nunes  (Portuguese translation)",
        "  Daniel Nylander  (Swedish translation)",
        "  Nikola Parsifal  (Croatian translation)",
        "  Markéta Pecoldová  (Czech translation)",
        "  Andreas Persenius  (Level design)",
        "  Karen Pouelle  (Level data clean-up)",
        "  Grzegorz Pruchniakowski  (Polish translation)",
        "  Mark Pulley  (Level design)",
        "  pzykosiz  (Level design)",
        "  Bruno Queiros  (Portuguese translation)",
        "  Quipyowert2  (Fixing leaks and bugs)",
        "  Vincenzo Reale  (Italian translation)",
        "  A. Regnander  (Swedish translation)",
        "  Thomas De Rocker  (Dutch translation)",
        "  Rudolf  (Level design)",
        "  Rui  (Portuguese translation)",
        "  Peter Santo  (Level design)",
        "  Sadie  (Level design)",
        "  Tobias Schmidbauer  (Windows installer and icon)",
        "  Lukas Schüller  (Level design)",
        "  Michael Schwarz  (Update to Enigma logo)",
        "  Stefan Schwoon  (Level design)",
        "  Achim Settelmeier  (RPM specfile)",
        "  ShadowPhrogg32642342  (Level design, Hungarian translation)",  // Dániel Borbély
        "  Gonçalo Silva  (Portuguese translation)",
        "  Manuela Silva  (Portuguese translation)",
        "  Alex Smith  (Level design)",
        "  Alan Smithee  (Level design)",
        "  Jon Sneyers  (Level design)",
        "  Spaceman  (Level design)",
        "  Ulf Stegemann  (Level design)",
        "  Jürgen Sticht  (Level design)",  // JuSt
        "  Gabe Stilez  (Hungarian translation)",
        "  Mikke Surakka  (Finnish translation)",
        "  Andrzej Szombierski  (Level design)",
        "  Tacvek  (Lua 5.1 upgrade)",
        "  James Taylor  (Level design)",
        "  Michael Terry  (.desktop file)",
        "  theschitz  (Swedish translation)",
        "  Vince38  (French translation)",
        "  Ray Wick  (Level design)",
        "  Martin Woyda-Wolf  (Level design)",
        "  Joe Wreschnig  (Manual page)",
        "  Heikki Ylipaavalniemi  (Finnish translation)",
        "  Francisco Zamorano  (Spanish translation)",
        "  Zephyr  (Level design)",
        "  郝中飞  (Chinese translation)",
        "  Unknown contributor (Slovenian translation)",
        0,
    };

    MainHelpMenu::MainHelpMenu () {
        const VMInfo *vminfo = video_engine->GetInfo();
        const bool vshrink = vminfo->width < 640 ;

        BuildVList b = vshrink? BuildVList(this, Rect(40, 40, 100, 25), 3)
                : BuildVList(this, Rect((vminfo->width - 180)/2, vminfo->tt ? 80 : 150, 180, 40), 7);
        BuildVList br = vshrink? BuildVList(this, Rect(180, 40, 100, 25), 4) : BuildVList(this, Rect(0, 0, 0, 0), 0);
        BuildVList *brp = vshrink? &br : &b;

        homepage = b.add(new StaticTextButton(N_("Homepage"), this));
        docs = b.add(new StaticTextButton(N_("Documentation"), this));
        paths = b.add(new StaticTextButton(N_("Paths"), this));
        autofolder = brp->add(new StaticTextButton(N_("Locate Auto"), this));
        scorefolder = brp->add(new StaticTextButton(N_("Locate Score"), this));
        credits = brp->add(new StaticTextButton(N_("Credits"), this));
        back = brp->add(new StaticTextButton(N_("Back"), this));
    }

    MainHelpMenu::~MainHelpMenu () {
    }

    bool MainHelpMenu::on_event (const SDL_Event &e)  {
        return false;
    }

    void MainHelpMenu::on_action(gui::Widget *w) {
        if (w == homepage) {
            video_engine->SetFullscreen(false);
            ecl::BrowseUrl("https://www.enigma-game.org");
        } else if (w == docs) {
            video_engine->SetFullscreen(false);
            ecl::BrowseUrl("file://" + app.docPath + "/index.html");
        } else if (w == paths) {
            showPaths();
        } else if (w == autofolder) {
            video_engine->SetFullscreen(false);
            ecl::ExploreFolder(ecl::BeautifyPath(app.userPath + "/levels/auto"));
        } else if (w == scorefolder) {
            video_engine->SetFullscreen(false);
            ecl::ExploreFolder(ecl::BeautifyPath(app.userPath));
        } else if (w == credits) {
            displayInfo(credit_text);
        } else if (w == back) {
            Menu::quit();
        } else
            return;
        invalidate_all();
    }

    void MainHelpMenu::draw_background(ecl::GC &gc) {
        set_caption(_("Enigma - Help Menu"));
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
    }

    void MainHelpMenu::tick(double dtime) {
    }

    void MainHelpMenu::showPaths() {
        const char *pathtext[25];
        std::string pathstrings[25];
        std::string work;
        Font *menufont = enigma::GetFont("menufont");
        const VMInfo *vminfo = video_engine->GetInfo();
        int width = vminfo->width - 80;
        int i = 0;

        pathtext[i++] = N_("Preferences Path:");
        work = ecl::BeautifyPath(app.prefPath);
        do {
            std::string::size_type breakPos = breakString(menufont, work, ecl::PathSeparators, width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = " ";
        pathtext[i++] = N_("User Path:");
        work = ecl::BeautifyPath(app.userPath);
        do {
            std::string::size_type breakPos = breakString(menufont, work, ecl::PathSeparators, width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = " ";
        pathtext[i++] = N_("User Image Path:");
        work = ecl::BeautifyPath(app.userImagePath);
        do {
            std::string::size_type breakPos = breakString(menufont, work, ecl::PathSeparators, width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = " ";
        pathtext[i++] = N_("System Path:");
        work = ecl::BeautifyPath(app.systemFS->getDataPath());
        do {
            std::string::size_type breakPos = breakString(menufont, work, ecl::PathSeparators, width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = " ";
        pathtext[i++] = N_("Resource Paths:");
        work = ecl::BeautifyPath(app.resourceFS->getDataPath());
        do {
            std::string::size_type breakPos = breakString(menufont, work, ecl::PathSeparators, width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = " ";
        pathtext[i++] = N_("L10n Path:");
        work = ecl::BeautifyPath(app.l10nPath);
        do {
            std::string::size_type breakPos = breakString(menufont, work, ecl::PathSeparators, width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = 0;
        displayInfo(pathtext);
    }

    /* -------------------- Main menu -------------------- */
    MainMenu::MainMenu()
    {
        build_menu();
    }

    void MainMenu::build_menu()
    {
        const VMInfo *vminfo = video_engine->GetInfo();
        const int vshrink = vminfo->width < 640 ? 1 : 0;
        const int vsmall = vminfo->width < 800 ? 1 : 0;
        int y[] = {75, 170, 205, 220, 220}; // y[0] and y[1] are actually not used
        BuildVList  b(this, vshrink ? Rect(40, 120, 100, 25)
                           : vsmall ? Rect(80, 200, 200, 50)
                                    : Rect((vminfo->width - 160)/2, y[vminfo->tt], 160, 40),
                            vshrink ? 4 : vsmall ? 10 : 8);
        BuildVList br(this, vshrink ? Rect(180, 120, 100, 25)
                                    : Rect(360, 200, 200, 50),
                            vshrink ? 4 : vsmall ? 10 : 8);
        BuildVList *brp = vsmall ? &br : &b;
        startgame = b.add(new StaticTextButton(N_("Start Game"), this));
        levelpack = b.add(new StaticTextButton(N_("Level Pack"), this));
#ifdef ENABLE_EXPERIMENTAL
        m_netgame   = b.add(new StaticTextButton(N_("Network Game"), this));
#endif
        search      = b.add(new StaticTextButton(N_("Search"), this));
        options     = brp->add(new StaticTextButton(N_("Options"), this));
#if 0
        update      = brp->add(new StaticTextButton(N_("Update"), this));
#endif
        help        = brp->add(new StaticTextButton(N_("Help"), this));
        quit        = brp->add(new StaticTextButton(N_("Quit"), this));

        const int xoffset_upper = vminfo->width - 65;
        BuildHList l_upper(this, Rect(xoffset_upper, 10, 60, 40), 5);
        if(vshrink) {
            languagemenu = new BorderlessImageButton(
                string("translation_icon_shrink"),
                string("translation_icon_shrink_hl"),
                string("translation_icon_shrink_hl"),
                true, this);
            l_upper.add(languagemenu);
        } else {
            languagemenu = new BorderlessImageButton(
                string("translation_icon"),
                string("translation_icon_hl"),
                string("translation_icon_hl"),
                true, this);
            l_upper.add(languagemenu);
        }
    }

    void MainMenu::draw_background(ecl::GC &gc)
    {
        const VMInfo *vminfo = video_engine->GetInfo();

        set_caption(_("Enigma - Main Menu"));
        sound::StartMenuMusic();

        blit(gc, vminfo->mbg_offsetx, vminfo->mbg_offsety, enigma::GetImage("menu_bg", ".jpg"));

        Font *f = enigma::GetFont("levelmenu");
        Surface * logo(enigma::GetImage((vminfo->width < 640) ? "enigma_logo3_shrink" : "enigma_logo3"));
        int x0=(vminfo->width - logo->width())/2;
        int y0[] = {30, 57, 60, 70, 80};
#ifdef ENABLE_EXPERIMENTAL
        y0[1] = 57;  // might need adaptation when more buttons are added
#endif
        blit(gc, x0, y0[vminfo->tt], logo);
        f->render (gc, 5, vminfo->height - 20, app.getVersionInfo());
    }

    bool MainMenu::on_event (const SDL_Event &e) {
        switch (e.type) {
            case SDL_KEYDOWN:
                SDL_Keycode keysym = e.key.keysym.sym;
                switch (keysym) {
                case SDLK_F1:
                case SDLK_F2: {
                    MainHelpMenu m;
                    m.manage();
                    invalidate_all();
                    return true;
                }
                default:
                    break;
                }
                break;
        }
        return false;
    }

    void MainMenu::on_action(Widget *w)
    {
        if (w == startgame) {
            LevelPackMenu m;
            m.manageLevelMenu();
        } else if (w == levelpack) {
            LevelPackMenu m;
            m.manage();
        } else if (w == search) {
            SearchMenu m;
            m.manage();
            if (m.isSearchQuit()) {
                // show search result levelpack
                LevelMenu ml;
                if (ml.manage() && !ml.isMainQuit()) {
                    // ESC in LevelMenu in cade we are a submenu of LevelMenu or
                    // Main button has been pressed in LevelMenu
                    LevelPackMenu mlp;
                    mlp.manage();
                }
            }
            invalidate_all();
        } else if (w == options) {
            ShowOptionsMenu(0, false);
            clear();
            reset_active_widget();
            build_menu();
            invalidate_all();
        } else if (w == help) {
            MainHelpMenu m;
            m.manage();

    #ifdef ENABLE_EXPERIMENTAL
        } else if (w == m_netgame) {
            ShowNetworkMenu();
    #endif
        } else if (w == quit) {
            Menu::quit();
        } else if (w == languagemenu) {
            LanguageMenu m;
            m.manage();
            invalidate_all();
        } else
            return;
        invalidate_all();
    }

    void MainMenu::tick(double /* dtime */) {
    }

/* -------------------- Functions -------------------- */

    void ShowMainMenu() {
        MainMenu m;
        m.manage();
    }

    void ShowNetworkMenu()
    {
        NetworkMenu m;
        m.manage();
    }

}} // namespace enigma::gui
