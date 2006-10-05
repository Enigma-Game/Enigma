/*
 * Copyright (C) 2002,2003,2004,2005,2006 Daniel Heck
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
 
#include "gui/MainMenu.hh"
#include "gui/LevelMenu.hh"
#include "gui/OptionsMenu.hh"
#include "gui/HelpMenu.hh"
#include "gui/LevelPackMenu.hh"
#include "gui/LevelPreviewCache.hh"
#include "display.hh"
#include "ecl.hh"
#include "main.hh"
#include "nls.hh"
#include "options.hh"
#include "sound.hh"
#include "video.hh"
#include "world.hh"

#include "netgame.hh"
#include "editor.hh"


using namespace ecl;
using namespace std;

namespace enigma { namespace gui {
/* -------------------- Helper routines -------------------- */
    
//    namespace
//    {
    /*! Change the video mode.  Because this opens a new screen with a
      new resolution, the display engine must be re-initialized to
      load the appropriate models. */
    void ChangeVideoMode()
    {
        world::PrepareLevel();      // make sure no references to models remain
        video::ChangeVideoMode();
        LevelPreviewCache::instance()->clear();
        enigma::ClearImageCache();
        display::Shutdown();
        display::Init();
    }
//    }
    
    
    /* -------------------- NetworkMenu -------------------- */
    
    NetworkMenu::NetworkMenu ()
    {
        const video::VMInfo *vminfo = video::GetInfo();
    
        BuildVList b(this, Rect((vminfo->width - 150)/2,150,150,40), 5);
        m_startgame = b.add(new StaticTextButton(N_("Start Game"), this));
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
        if (w == m_startgame) {
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
    
    
    
    /* -------------------- Main menu -------------------- */
    
    MainMenu::MainMenu() 
    {
        build_menu();
    }
    
    void MainMenu::build_menu() 
    {
        const video::VMInfo *vminfo = video::GetInfo();
    
        BuildVList b(this, Rect((vminfo->width - 150)/2,150,150,40), 5);
        m_startgame = b.add(new StaticTextButton(N_("Start Game"), this));
        m_levelpack = b.add(new StaticTextButton(N_("Level Pack"), this));
    #ifdef ENABLE_EXPERIMENTAL
        m_netgame   = b.add (new StaticTextButton (N_("Network Game"), this));
        leveled     = b.add(new StaticTextButton(N_("Editor"), this));
    #endif
    //    manual      = b.add(new StaticTextButton("Manual", this));
        options     = b.add(new StaticTextButton(N_("Options"), this));
        credits     = b.add(new StaticTextButton(N_("Credits"), this));
        quit        = b.add(new StaticTextButton(N_("Quit"), this));
    }
    
    void MainMenu::draw_background(ecl::GC &gc) 
    {
        const video::VMInfo *vminfo = video::GetInfo();
    
        video::SetCaption (("Enigma - Main Menu"));
        sound::PlayMusic (options::GetString("MenuMusicFile"));
    
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
    
        Font *f = enigma::GetFont("levelmenu");
        Surface * logo(enigma::GetImage("enigma_logo3"));
        int x0=(vminfo->width - logo->width())/2;
        int y0=30;
        blit(gc, x0, y0, logo);
        f->render (gc, 5, vminfo->height - 20, app.getVersionInfo().c_str());
    }
    
    bool MainMenu::on_event (const SDL_Event &e) {
        switch (e.type) {
            case SDL_KEYDOWN:
                SDLKey keysym = e.key.keysym.sym;
                switch (keysym) {
                case SDLK_F2:     
                    show_paths();
                    invalidate_all();
                    return true;
                default:
                    break;
                }
                break;
        }
        return false;
    }

    void MainMenu::on_action(Widget *w) 
    {
        if (w == m_startgame) {
//            LevelMenu m(LevelPacks[0], options::GetInt("LevelMenuPosition"));
//            LevelMenu m;  
//            m.manage();
            
            LevelPackMenu m;
            m.manageLevelMenu();
//            options::SetOption("LevelMenuPosition", m.get_position());
            invalidate_all();
    //     }
    //     else if (w == manual) {
    //         show_help ();
        } else if (w == m_levelpack) {
            LevelPackMenu m;
            m.manage();
            invalidate_all();
        } else if (w == credits) {
            show_credits ();
        } else if (w == options) {
            ShowOptionsMenu(0);
    
    #ifdef ENABLE_EXPERIMENTAL
        } else if (w == m_netgame) {
            ShowNetworkMenu();
        } else if (w == leveled) {
            editor::Run();
    #endif
        } else if (w == quit) {
            Menu::quit();
        } else
            return;
        invalidate_all();
    }
    
    void MainMenu::tick(double /* dtime */) 
    {
        bool option_fullscreen = options::GetInt ("FullScreen") != 0;
        if (options::GetInt ("VideoMode") != video::GetVideoMode()
            || option_fullscreen != video::IsFullScreen())
        {
            ChangeVideoMode();
            clear();
            reset_active_widget ();
            build_menu();
            invalidate_all();
        }
    }
    
    
    void MainMenu::show_text( const char *text[]) 
    {
        Screen *scr = video::GetScreen ();
        GC gc (scr->get_surface());
        blit (gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
    
    
        Font *f = enigma::GetFont("menufont");
        for (int i=0; text[i]; ++i)
        {
            const char *t = _(text[i]);
            f->render (gc, 40, 20+i*f->get_height(), t);
        }
        scr->update_all ();
        scr->flush_updates();
    
        SDL_Event e;
        for (;;) {
            SDL_WaitEvent(&e);
            if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
                break;
        }
    }
    
    void MainMenu::show_credits () 
    {
        static const char *credit_text[] = {
            N_("Main developers:"),
            N_("  RONALD LAMPRECHT (maintainer)"),
            N_("  RAOUL BOURQUIN"),
            N_("  ANDREAS LOCHMANN"),
            " ",
            N_("Special Thanks To:"),
            N_("  DANIEL HECK (project founder)"),
            N_("  MEINOLF SCHNEIDER (game idea, level design)"),
            " ",
            N_("Please refer to the manual for full credits."),
            " ",
            N_("Home Page: http://www.nongnu.org/enigma"),
            N_("Contact: enigma-devel@nongnu.org"),
            " ",
            N_("Enigma is free software and may be distributed under the"),
            N_("terms of the GNU General Public License, version 2."),
            " ",
            N_("Copyright (C) 2002-2006 Daniel Heck and contributors."),
            0
        };
    
        show_text(credit_text);
    }
    
    void MainMenu::show_paths() {
        const char *pathtext[25];
        std::string pathstrings[25];
        std::string work;
        Font *menufont = enigma::GetFont("menufont");
        const video::VMInfo *vminfo = video::GetInfo();
        int width = vminfo->width - 80;
        int i = 0;
        
        pathtext[i++] = N_("Preferences Path:");
        work = app.prefPath;
        do {
            std::string::size_type breakPos = breakString(menufont, work,"/\\", width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = " ";
        pathtext[i++] = N_("User Path:");
        work = app.userPath;
        do {
            std::string::size_type breakPos = breakString(menufont, work,"/\\", width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = " ";
        pathtext[i++] = N_("User Image Path:");
        work = app.userImagePath;
        do {
            std::string::size_type breakPos = breakString(menufont, work,"/\\", width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = " ";
        pathtext[i++] = N_("System Path:");
        work = app.systemFS->getDataPath();
        do {
            std::string::size_type breakPos = breakString(menufont, work,"/\\", width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = " ";
        pathtext[i++] = N_("Resource Paths:");
        work = app.resourceFS->getDataPath();
        do {
            std::string::size_type breakPos = breakString(menufont, work,"/\\", width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = " ";
        pathtext[i++] = N_("L10n Path:");
        work = app.l10nPath;
        do {
            std::string::size_type breakPos = breakString(menufont, work,"/\\", width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = 0;
        show_text(pathtext);
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
