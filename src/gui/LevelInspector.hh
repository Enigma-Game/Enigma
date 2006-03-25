/*
 * Copyright (C) 2006 Ronald Lamprecht
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
 
#ifndef GUI_LEVELINSPECTOR_HH_INCLUDED
#define GUI_LEVELINSPECTOR_HH_INCLUDED

#include "ecl.hh"
#include "gui/widgets.hh"
#include "gui/TextField.hh"
#include "lev/Proxy.hh"

namespace enigma { namespace gui {
    class LevelInspector : public gui::Menu {
    public:
        LevelInspector (lev::Proxy *aLevel, ecl::Surface *preview);
        ~LevelInspector ();
    private:
        // ActionListener interface.                    
        bool on_event (const SDL_Event &e);
        void on_action(gui::Widget *w);

        // Menu interface.
        void draw_background(ecl::GC &gc);
        void tick(double dtime);
        
        //
        std::string ratingToString(int value);
        std::string scoreToString(int easy, int difficult, lev::Proxy *aLevel);
        void dispatchBottomLines(int &bestScoreHolderLines, 
            int &creditsLines, int &dedicationLines, int &levelPathLines,
            int &annotationLines, int numLines, int width);
        std::string::size_type breakString(const std::string &theString, 
                const std::string &breakChar, const int width, 
                ecl::Font * const theFont);
        // Variables.
        gui::Widget *back;
        gui::TextField *annotation;
        ecl::Surface *previewImage;
        lev::Proxy  *levelProxy;
        std::string levelPathString;
        int vspacing;
        int vmargin;
        int hmargin;
        bool withEasy;
    };

}} // namespace enigma::gui
#endif

