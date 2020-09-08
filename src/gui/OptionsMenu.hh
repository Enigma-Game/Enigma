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
#ifndef GUI_OPTIONSMENU_HH_INCLUDED
#define GUI_OPTIONSMENU_HH_INCLUDED

#include "gui/Menu.hh"
#include "gui/TextField.hh"

namespace enigma { namespace gui {
    class FullscreenModeButton;
    class WindowSizeButton;
    class FullscreenTilesetButton;
    class WindowTilesetButton;

/* -------------------- OptionsMenu -------------------- */

    class OptionsMenu : public Menu {
    public:
        OptionsMenu(ecl::Surface *background_, bool gameIsOngoing_);
        virtual void quit();

    private:
        // ActionListener interface.
        bool on_event (const SDL_Event &e);
        void on_action(gui::Widget *w);
        
        // Menu interface.
        void draw_background(ecl::GC &gc);
        void tick(double dtime) {}

        // Page structure.
        enum OptionsPage { OPTIONS_MAIN, OPTIONS_VIDEO, OPTIONS_AUDIO,
                           OPTIONS_CONFIG, OPTIONS_PATHS };
        void open_page(OptionsPage new_page);
        void close_page();

        // Variables.
        VList *pagesVList;
        HList *commandHList;
        VList *optionsVList;
        gui::Widget *back, *language;
        gui::StaticTextButton *but_main_options;
        gui::StaticTextButton *but_video_options;
        gui::StaticTextButton *but_audio_options;
        gui::StaticTextButton *but_config_options;
        gui::StaticTextButton *but_paths_options;
        gui::BoolOptionButton *fullscreen;
        gui::FullscreenModeButton *fullscreenmode;
        gui::WindowSizeButton *windowsize;
        gui::FullscreenTilesetButton *fullscreentileset;
        gui::WindowTilesetButton *windowtileset;
        gui::TextField *userNameTF;
        gui::TextField *userPathTF;
        gui::TextField *userImagePathTF;
        gui::TextField *menuMusicTF;
        ecl::Surface *background;
        bool gameIsOngoing;
    };


/* -------------------- Functions -------------------- */

    void ShowOptionsMenu(ecl::Surface *background, bool gameIsOngoing);

}} // namespace enigma::gui
#endif
