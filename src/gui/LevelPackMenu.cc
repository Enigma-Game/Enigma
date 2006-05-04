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
 
#include "gui/LevelPackMenu.hh"
#include "ecl.hh"
#include "nls.hh"
#include "video.hh"

#include "levels.hh"
using enigma::levels::LevelStatus;
using enigma::levels::LevelInfo;
using enigma::levels::LevelPack;
using enigma::levels::LevelPacks;


using namespace ecl;
using namespace std;

namespace enigma { namespace gui {
/* -------------------- Level Pack Menu -------------------- */
    
    LevelPackMenu::LevelPackMenu()
        : m_selection(-1)
    {
#define PACKMODES 3
        struct ppm { int rectw, recth; } packmodeparam[PACKMODES] = {
            { 160, 40 },            // up to 3*10 = 30 levelpacks (640x480)
            { 150, 30 },            // up to 4*13 = 52 levelpacks
            { 120, 25 },            // up to 5*15 = 75 levelpacks
        };
    
        unsigned packcount = LevelPacks.size();
    
        for (unsigned i=0; i<packcount; ++i) {
            LevelPack *lp = LevelPacks[i];
            buttons.push_back(new UntranslatedStaticTextButton(lp->get_name(), this));
        }
    
        Rect buttonarea = ecl::Screen::get_instance()->size();
        bool finished   = false;
    
        for (int mode = 0; !finished && mode < PACKMODES; ++mode) {
            ppm&          ppm = packmodeparam[mode];
            VTableBuilder builder(this, buttonarea, Rect(0,0,ppm.rectw,ppm.recth), 5, 5);
    
            finished = builder.finish(buttons);
        }
    
        Assert <XGeneric> (finished, strf("LevelpackMenu Layout error (too many levelpacks: %i - try in higher resolution)", packcount));
#undef PACKMODES
    }
    
    void LevelPackMenu::on_action(Widget *w) {
        for (unsigned i=0; i<buttons.size(); ++i)
            if (buttons[i]==w) {
                m_selection = i;
                Menu::quit();
            }
    }
    
    void LevelPackMenu::draw_background(ecl::GC &gc) {
        video::SetCaption(("Enigma - Level Pack Menu"));
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
    }

}} // namespace enigma::gui
