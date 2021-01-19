/*
 * Copyright (C) 2002,2003,2004,2005,2006 Daniel Heck
 * Copyright (C) 2006,2007,2008,2009 Ronald Lamprecht
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
#ifndef GUI_MAINMENU_HH_INCLUDED
#define GUI_MAINMENU_HH_INCLUDED

#include "gui/Menu.hh"
#include "gui/widgets.hh"

#include <list>
#include <vector>

namespace enigma { namespace gui {
/* -------------------- MainMenu -------------------- */

    class MainMenu : public Menu {
    public:
        MainMenu();
    private:
        // Menu interface
        void draw_background(ecl::GC &gc);
        void tick(double dtime);

        // Widget interface
        virtual bool on_event(const SDL_Event &e);

        // ActionListener interface.
        void on_action(Widget *w);

        // Private methods.
        void build_menu();

        // Variables.
        Widget *startgame;
        Widget *levelpack;
        Widget *m_netgame;
        Widget *search;
        Widget *options;
        Widget *update;
        Widget *help;
        Widget *quit;
        Widget *lpack;
        std::vector<BorderlessImageButton*> flags;
    };

/* -------------------- MainHelpMenu -------------------- */
        
    class MainHelpMenu : public gui::Menu {
    public:
        MainHelpMenu ();
        ~MainHelpMenu ();
    private:
        // ActionListener interface.
        bool on_event (const SDL_Event &e);
        void on_action(gui::Widget *w);

        // Menu interface.
        void draw_background(ecl::GC &gc);
        void tick(double dtime);

        // Private methods.
        void showPaths();

        // Variables.
        gui::Widget *homepage;
        gui::Widget *docs;
        gui::Widget *paths;
        gui::Widget *autofolder;
        gui::Widget *scorefolder;
        gui::Widget *credits;
        gui::Widget *back;
    };

/* -------------------- NetworkMenu -------------------- */
        
    class NetworkMenu : public gui::Menu {
    public:
        NetworkMenu ();
        ~NetworkMenu ();
    private:
        // ActionListener interface.
        bool on_event (const SDL_Event &e);
        void on_action(gui::Widget *w);

        // Menu interface.
        void draw_background(ecl::GC &gc);
        void tick(double dtime);

        // Variables.
        gui::Widget *startgame;
        gui::Widget *m_joingame;
        gui::Widget *m_back;
    };

    
/* -------------------- Functions -------------------- */
    void ShowMainMenu();
    void ShowNetworkMenu();
}} // namespace enigma::gui
#endif
