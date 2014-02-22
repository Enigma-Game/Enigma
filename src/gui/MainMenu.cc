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
 *
 */

#include "gui/MainMenu.hh"
#include "gui/LevelMenu.hh"
#include "gui/OptionsMenu.hh"
#include "gui/InfoMenu.hh"
#include "gui/LevelPackMenu.hh"
#include "gui/LevelPreviewCache.hh"
#include "display.hh"
#include "ecl.hh"
#include "main.hh"
#include "nls.hh"
#include "options.hh"
#include "MusicManager.hh"
#include "video.hh"
#include "world.hh"

#include "netgame.hh"


using namespace ecl;
using namespace std;

namespace enigma { namespace gui {
/* -------------------- Helper routines -------------------- */

    /*! Change the video mode.  Because this opens a new screen with a
      new resolution, the display engine must be re-initialized to
      load the appropriate models. */
    void ChangeVideoMode()
    {
        enigma::WorldPrepareLevel();      // make sure no references to models remain
        enigma::ClearFontCache();
        video::ChangeVideoMode();
        LevelPreviewCache::instance()->clear();
        enigma::ClearImageCache();
        display::Shutdown();
        display::Init();
    }


    /* -------------------- NetworkMenu -------------------- */

    NetworkMenu::NetworkMenu ()
    {
        const video::VMInfo *vminfo = video::GetInfo();

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
        video::SetCaption (("Enigma - Network Menu"));
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
    }

    void NetworkMenu::tick(double dtime)
    {
    }


    /* -------------------- Help menu -------------------- */
    static const char *credit_text[] = {
        N_("Main developers of the 1.20 release:"),
        " ",
        N_("  RONALD LAMPRECHT (lead)"),
        "  RAOUL BOURQUIN",
        "  ANDREAS LOCHMANN",
        " ",
        N_("Special Thanks To:"),
        " ",
        N_("  DANIEL HECK (project founder)"),
        N_("  MEINOLF SCHNEIDER (game idea, level design)"),
        " ",
        N_("Please refer to the manual or the next pages for full credits."),
        " ",
        N_("Home Page: http://www.nongnu.org/enigma"),
        N_("Contact: enigma-devel@nongnu.org"),
        " ",
        N_("Enigma is free software and may be distributed under the"),
        N_("terms of the GNU General Public License, version 2."),
        " ",
        N_("Copyright (C) 2002-2014 Daniel Heck and contributors."),
        0,
        N_("Main developers of all releases:"),
        " ",
        "  Raoul Bourquin  (Level admin. & design, all over invaluable additions)",
        "  Siegfried Fennig  (Level design, graphics)",
        "  Martin Hawlisch  (Level design, graphics, programming)",
        "  Daniel Heck  (Main developer up to 0.92, graphics, documentation)",
        "  Ronald Lamprecht  (XML, new API, engine, portability, GUI, documentation)",
        "  Andreas Lochmann  (Programming, level admin. & design, documentation)",
        "  Petr Machata  (Level design, programming)",
        "  Nat Pryce  (Level design)",
        "  Jacob Scott  (Level design)",
        "  Sven Siggelkow  (Level design and special Oxyd expertise)",
        "  Ralf Westram  (Programming, level design)",
        0,
        N_("Special Thanks:"),
        " ",
        "  Johannes Fortmann  (Mac OS X port, some programming, graphics)",
        "  illmind  (Forum mag-heut.net administration, Level design)",
        // waiting for licence to add the sounds
        // "  \'Cellar of Rats\'  (Sound effects)",
        "  Jennifer Robertson  (Graphics second generation)",
        "  Jeremy Sawicki  (Oxydlib)",
        "  Erich Schubert  (Debian/Ubuntu packages, level design)",
        "  Andrew \'Necros\' Sega  (Menu music \'Pentagonal Dreams\')",
        "  David W. Skinner  (Many Sokoban Levels)",
        "  Clifford J. Tasner  (Music second generation, Proof reading)",
        0,
        N_("Contributors:"),
        " ",
        "  Roberto Bardin  (Level design)",
        "  Thomas Bernhardt  (Level design)",
        "  Helge Blohmer  (Level design)",
        "  Nathan Bronecke  (Level design)",
        "  Harry Bruder  (Level design)",
        "  Alain Busser  (Level design, French translation, manual)",
        "  Guy Busser  (Level design)",
        "  Richi Bützer  (Level design)",
        "  capkoh  (Level design)",
        "  Manfredi Carta  (Level design)",
        "  Christoph & Anita  (Level design)",
        "  daydreamer  (Level design)",
        "  Дремук Сергей - Serge Dremuk  (Russian translation)",
        "  Joseph Dunne  (Level design)",
        "  Xerxes M. Dynatos  (Level design)",
        "  Edward  (Level design)",
        "  Manuel Eisentraut  (Level design)",
        "  Stephanie Fabian  (Invaluable bug reports)",
        0,
        "  Johann Freymuth  (Invaluable bug reports)",
        "  Roberto García (Spanish translation)",
        "  Andy Geldmacher  (Level design)",
        "  Edwin Groothuis  (FreeBSD port)",
        "  Daniel Hadas  (Level design)",
        "  Jonatan Hadas  (Level design)",
        "  Hairball  (Level design)",
        "  Immanuel Herrmann  (Level design)",
        "  Brian Huffman  (Level design)",
        "  Johannes Hüsing  (Level design)",
        "  Máté Lehel Juhász  (Hungarian translation)",
        "  Samuele Kaplun  (Italian translation)",
        "  Manuel König  (Level design, bug reports)",
        "  Jens-Christian Korth  (Level design)",
        "  Johannes Laire  (Level design)",
        "  Joona Laire  (Level design)",
        "  Markus Laire  (Level design)",
        "  Dominik Lehmann  (Level design)",
        "  Dominik Leipold  (Level design)",
        "  Edward Leuf  (Feedback, bug reports)",
        0,
        "  Ingo van Lil  (Feedback, bug reports)",
        "  Frank van der Loo  (Dutch translation)",
        "  Lurcane  (Level design)",
        "  Sidney Markowitz  (Mac OS X port)",
        "  Barry & Lori Mead  (Level design)",
        "  Linda Mihalic  (English proof reading)",
        "  moonpearl  (Level design)",
        "  Stephen Morley (Safalra)  (Level design)",
        "  Krishnamurti Lelis Lima Vieira Nunes  (Portuguese translation)",
        "  Daniel Nylander  (Swedish translation)",
        "  Andreas Persenius  (Level design)",
        "  Mark Pulley  (Level design)",
        "  Rudolf  (Level design)",
        "  Peter Santo  (Level design)",
        "  Tobias Schmidbauer  (Windows installer and icon)",
        "  Lukas Schüller  (Level design)",
        "  Achim Settelmeier  (RPM specfile)",
        "  ShadowPhrogg32642342 (Dániel Borbély)  (Level design)",
        "  Alex Smith  (Level design)",
        "  Alan Smithee  (Level design)",
        0,
        "  Jon Sneyers  (Level design)",
        "  Spaceman  (Level design)",
        "  Ulf Stegemann  (Level design)",
        "  Jürgen Sticht  (Level design)",
        "  Mikke Surakka  (Finnish translation)",
        "  Andrzej Szombierski  (Level design)",
        "  Tacvek  (Lua 5.1 upgrade)",
        "  James Taylor  (Level design)",
        "  Michael Terry  (.desktop file)",
        "  Ray Wick  (Level design)",
        "  Joe Wreschnig  (Manual page)",
        "  Zephyr  (Level design)",
        "  Юрий Жиромский - Yuriy Zhyromskiy  (Russian Manual)",
        "  /dev/null  (Level design)",
        0,
    };

    MainHelpMenu::MainHelpMenu () {
        const video::VMInfo *vminfo = video::GetInfo();
        const bool vshrink = vminfo->width < 640 ;

        BuildVList b = vshrink? BuildVList(this, Rect(40, 40, 100, 25), 3)
                : BuildVList(this, Rect((vminfo->width - 150)/2, vminfo->tt ? 80 : 150, 150, 40), 7);
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
            video::SetFullscreen(false);
            ecl::BrowseUrl("http://www.enigma-game.org");
        } else if (w == docs) {
            video::SetFullscreen(false);
            ecl::BrowseUrl("file://" + app.docPath + "/index.html");
        } else if (w == paths) {
            showPaths();
        } else if (w == autofolder) {
            video::SetFullscreen(false);
            ecl::ExploreFolder(ecl::BeautifyPath(app.userPath + "/levels/auto"));
        } else if (w == scorefolder) {
            video::SetFullscreen(false);
            ecl::ExploreFolder(ecl::BeautifyPath(app.userPath));
        } else if (w == credits) {
            displayInfo(credit_text, 7);
        } else if (w == back) {
            Menu::quit();
        } else
            return;
        invalidate_all();
    }

    void MainHelpMenu::draw_background(ecl::GC &gc) {
        video::SetCaption (("Enigma - Help Menu"));
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
    }

    void MainHelpMenu::tick(double dtime) {
    }

    void MainHelpMenu::showPaths() {
        const char *pathtext[25];
        std::string pathstrings[25];
        std::string work;
        Font *menufont = enigma::GetFont("menufont");
        const video::VMInfo *vminfo = video::GetInfo();
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
        displayInfo(pathtext, 1);
    }

    /* -------------------- Main menu -------------------- */
    MainMenu::MainMenu()
    {
        build_menu();
    }

    void MainMenu::build_menu()
    {
        const video::VMInfo *vminfo = video::GetInfo();
        const int vshrink = vminfo->width < 640 ? 1 : 0;
        int y[] = {75, 170, 190, 220, 220};
        // parameters to use when flags are not at top: {75, 150, 170, 200, 200};
#ifdef ENABLE_EXPERIMENTAL
        y[1] = 150;
#endif
        BuildVList b(this, Rect(vshrink?40:(vminfo->width - 150)/2, vshrink?120:y[vminfo->tt], vshrink?100:150, vshrink?25:40), vshrink?3:6);
        BuildVList br = vshrink? BuildVList(this, Rect(180, 120, 100, 25), 3) : BuildVList(this, Rect(0, 0, 0, 0), 0);
        BuildVList *brp = vshrink? &br : &b;
        startgame = b.add(new StaticTextButton(N_("Start Game"), this));
        levelpack = b.add(new StaticTextButton(N_("Level Pack"), this));
#ifdef ENABLE_EXPERIMENTAL
        m_netgame   = b.add(new StaticTextButton(N_("Network Game"), this));
#endif
        options     = b.add(new StaticTextButton(N_("Options"), this));
#if 0
        update      = brp->add(new StaticTextButton(N_("Update"), this));
#endif
        help        = brp->add(new StaticTextButton(N_("Help"), this));
        quit        = brp->add(new StaticTextButton(N_("Quit"), this));

        int ly = vminfo->width - 5 - 35*(NUMENTRIES(nls::languages) - 1);
        //BuildHList l(this, Rect(ly, (vminfo->height) - 30, 30, 20), 5);
        BuildHList l(this, Rect(ly, 10, 30, 20), 5);
        //BuildVList l(this, Rect(vminfo->width - 45, 15, 30, 20), 5);
        flags.clear();  // remove old flags on screen resolution changes
        if(!vshrink) {
            std::string curname = ecl::SysMessageLocaleName();
            curname = curname.substr(0, curname.find('.'));
            for (size_t i=1; i<NUMENTRIES(nls::languages); ++i) {
                BorderlessImageButton *but = new BorderlessImageButton(
                    nls::languages[i].flagimage + string("-shaded"),
                    nls::languages[i].flagimage,
                    nls::languages[i].flagimage, curname == nls::languages[i].localename, this);
                l.add(but);
                flags.push_back(but);
            }
        }
    }

    void MainMenu::draw_background(ecl::GC &gc)
    {
        const video::VMInfo *vminfo = video::GetInfo();

        video::SetCaption (("Enigma - Main Menu"));
        sound::StartMenuMusic();

        blit(gc, vminfo->mbg_offsetx, vminfo->mbg_offsety, enigma::GetImage("menu_bg", ".jpg"));

        Font *f = enigma::GetFont("levelmenu");
        Surface * logo(enigma::GetImage("enigma_logo3"));
        int x0=(vminfo->width - logo->width())/2;
        int y0[] = {0, 50, 60, 70, 80};
        // parameters to use when flags are not at top: {0, 30, 40, 50, 60};
#ifdef ENABLE_EXPERIMENTAL
        y0[1] = 30;
#endif
        blit(gc, x0, y0[vminfo->tt], logo);
        f->render (gc, 5, vminfo->height - 20, app.getVersionInfo().c_str());
    }

    bool MainMenu::on_event (const SDL_Event &e) {
        switch (e.type) {
            case SDL_KEYDOWN:
                SDLKey keysym = e.key.keysym.sym;
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
        } else if (w == options) {
            ShowOptionsMenu(0);
        } else if (w == help) {
            MainHelpMenu m;
            m.manage();

    #ifdef ENABLE_EXPERIMENTAL
        } else if (w == m_netgame) {
            ShowNetworkMenu();
    #endif
        } else if (w == quit) {
            Menu::quit();
        } else if (flags.size() > 0) {
            for (size_t i=1; i<NUMENTRIES(nls::languages); ++i)
                if (w == flags[i-1]) {
                    options::SetOption ("Language", nls::languages[i].localename);
                    app.setLanguage(nls::languages[i].localename);
                }
        } else
            return;
        // need to update flags
        std::string curname = ecl::SysMessageLocaleName();
        curname = curname.substr(0, curname.find('.'));
        for (unsigned int i = 0; i < flags.size(); i++)
            flags[i]->setState(curname == nls::languages[i+1].localename);

        invalidate_all();
    }

    void MainMenu::tick(double /* dtime */)
    {
        bool isFullScreen = app.prefs->getBool("FullScreen");
        if (app.selectedVideoMode != video::GetVideoMode()
                || isFullScreen != video::IsFullScreen())
        {
            ChangeVideoMode();
            clear();
            reset_active_widget ();
            build_menu();
            invalidate_all();
        }
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
