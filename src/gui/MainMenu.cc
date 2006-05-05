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
#include "display.hh"
#include "ecl.hh"
#include "nls.hh"
#include "options.hh"
#include "sound.hh"
#include "video.hh"
#include "world.hh"

#include "netgame.hh"
#include "editor.hh"

#include "levels.hh"
using enigma::levels::LevelStatus;
using enigma::levels::LevelInfo;
using enigma::levels::LevelPack;
using enigma::levels::LevelPacks;


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
        f->render (gc, 5, vminfo->height - 20, "v" PACKAGE_VERSION);
    }
    
    void MainMenu::on_action(Widget *w) 
    {
        if (w == m_startgame) {
            LevelMenu m(LevelPacks[0], options::GetInt("LevelMenuPosition"));
            m.manage();
            options::SetOption("LevelMenuPosition", m.get_position());
            invalidate_all();
    //     }
    //     else if (w == manual) {
    //         show_help ();
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
            N_("Project maintainer:"),
            "  DANIEL HECK",
            " ",
            N_("Main developers:"),
            "  SIEGFRIED FENNIG, MARTIN HAWLISCH, PETR MACHATA,",
            "  JACOB SCOTT, SVEN SIGGELKOW, RALF WESTRAM",
            " ",
            N_("Special Thanks To:"),
            N_("  JOHANNES FORTMANN (graphics), JEREMY SAWICKI (oxydlib),"),
            N_("  MEINOLF SCHNEIDER (game idea, level design), ANDREW \"NECROS\" SEGA (menu music)"),
            N_("  NAT PRYCE & JOHN 'WB' SNEYERS (levels)"),
            " ",
            N_("Please see the manual for more detailed credits."),
            " ",
            N_("Home Page: http://www.nongnu.org/enigma"),
            N_("Contact: enigma-devel@nongnu.org"),
            " ",
            N_("Enigma is free software and may be distributed under the"),
            N_("terms of the GNU General Public License, version 2.  See"),
            N_("the accompanying COPYING.GPL for details."),
            " ",
            N_("Copyright (C) 2002-2005 Daniel Heck and contributors."),
            0
        };
    
        show_text(credit_text);
    }
    
    void MainMenu::show_help () 
    {
        static const char *screen1[] = {
            "Introduction:",
            "",
            "The idea behind Enigma is simple: In most levels your job is to find",
            "pairs of \"Oxyd\" stones (you will recognize them when you see them)",
            "with matching colors. You have to open all oxyd stones but they only",
            "stay open when opening two stones of the same color one after",
            "another. Just play the first levels in the \"Oxyd & Co\" group and you",
            "will get the idea.",
            "In some other levels, called \"meditation landscapes\" you have a",
            "different job: You control a couple of small white marbles",
            "simultaneously and have to put each of them into a pit on the floor.",
            "",
            "Moving around:",
            "",
            "You control the marble by moving around the mouse into the desired",
            "direction. But be careful, because the marble has some mass and",
            "the floor some friction, the marble needs some time to accelerate or",
            "stop.",
            "Larger levels scroll when you reach the outermost part of the visible",
            "part on the screen.",
            "",
            0
        };
        static const char *screen2[] = {
            "The Floor:",
            "",
            "On most types of floor you can move around riskless but the speed",
            "of your marble may vary.",
            "When moving into abyss or water you will die.",
            "Ice is very slippery.",
            "You cannot control your marble in space.",
            "",
            "Items and Inventory:",
            "",
            "In many levels you can see different items. You can pick them up",
            "by simply rolling over them. The items are then stored in your",
            "inventory, which you control using the left and right mouse button.",
            "The leftmost item will be activated when pressing the left mouse",
            "button and you can rotate the items using the right mouse button.",
            "Some items can mutate when hit by a laser or crushed by a stone.",
            "",
            0
        };
        static const char *screen3[] = {
            "Stones:",
            "",
            "Most stones  are nothing more than walls.",
            "Many stones are movable when hit strong enough.",
            "The wooden stone will build new floor if moved into water, space or",
            "abyss.",
            "Others can be destroyed using a hammer, dynamite or a laser.",
            "Doors can be opened using a switch or trigger hidden somewhere",
            "around in the level.",
            "Some magic stones can be changed when hit using a magic wand.",
            "And some depend on the color of your marble.",
            "",
            0
        };
    
        show_text (screen1);
        show_text (screen2);
        show_text (screen3);
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
