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
#include "gui/LevelMenu.hh"
#include "ecl.hh"
#include "nls.hh"
#include "video.hh"
#include "lev/Index.hh"


using namespace ecl;
using namespace std;

namespace enigma { namespace gui {
/* -------------------- Level Pack Menu -------------------- */
    
    LevelPackMenu::LevelPackMenu()
//        : m_selection(-1)
    {
        const video::VMInfo &vminfo = *video::GetInfo();
        HList *hl;

#define PACKMODES 3
        struct ppm { int rectw, recth; } packmodeparam[PACKMODES] = {
            { 160, 40 },            // up to 3*10 = 30 levelpacks (640x480)
            { 150, 30 },            // up to 4*13 = 52 levelpacks
            { 120, 25 },            // up to 5*15 = 75 levelpacks
        };
    
        std::vector<lev::Index *> * group = lev::Index::getGroup("Default");
        unsigned packcount = group->size();

        for (unsigned i=0; i<packcount; ++i) {
            lev::Index *lp = (*group)[i];
            buttons.push_back(new UntranslatedStaticTextButton(lp->getName(), this));
        }
            
        Rect buttonarea = ecl::Screen::get_instance()->size();
        buttonarea.h = buttonarea.h - 120;
        bool finished   = false;
    
        for (int mode = 0; !finished && mode < PACKMODES; ++mode) {
            ppm&          ppm = packmodeparam[mode];
            VTableBuilder builder(this, buttonarea, Rect(0,0,ppm.rectw,ppm.recth), 5, 5);
    
            finished = builder.finish(buttons);
        }
    
        Assert <XGeneric> (finished, strf("LevelpackMenu Layout error (too many levelpacks: %i - try in higher resolution)", packcount));
#undef PACKMODES

        center();
        
        // Create buttons
        but_search = new StaticTextButton(N_("Search"), this);
        but_edit = new StaticTextButton(N_("Edit"), this);
        but_level = new StaticTextButton(N_("Start Game"), this);
        but_main = new StaticTextButton(N_("Main"), this);
        
        hl = new HList;
        hl->set_spacing (10);
        hl->set_alignment (HALIGN_CENTER, VALIGN_TOP);
        hl->set_default_size (140, 40);
        hl->add_back (but_search);
        hl->add_back (but_edit);
        hl->add_back (but_level);
        hl->add_back (but_main);
        this->add (hl, Rect(10, vminfo.height-60, vminfo.width-10, 40));
    }
    
    bool LevelPackMenu::manageLevelMenu() {
        bool finished = false;
        while (!finished) {
            {        
                LevelMenu m;
                if (!m.manage() || m.isMainQuit()) {
                    // ESC or Main button has been pressed in LevelMenu -
                    // the user wants to return
                    finished = true;
                }
            }
            if (!finished) {
                // the user left LevelMenu via LevelPack button 
                if (this->manage()) {
                    // not ESC - the user pressed Main button
                    finished = true;
                } else {
                    // the user pressed ESC - return to LevelMenu
                }
            }
        }       
    }
    
    void LevelPackMenu::on_action(Widget *w) {
        if (w == but_main) {
            Menu::quit();
        } else if (w == but_edit) {
        } else if (w == but_level) {
            LevelMenu m;
            if (m.manage() && m.isMainQuit()) {
                // not ESC but Main button has been pressed in LevelMenu -
                Menu::quit();
            }
            invalidate_all();            
        } else if (w == but_search) {
        } else {
            lev::Index::setCurrentIndex(dynamic_cast<TextButton *>(w)->get_text());
            LevelMenu m;
            if (m.manage() && m.isMainQuit()) {
                // not ESC but Main button has been pressed in LevelMenu -
                Menu::quit();
            }
            invalidate_all();
        }   
    }
    
    void LevelPackMenu::draw_background(ecl::GC &gc) {
        video::SetCaption(("Enigma - Level Pack Menu"));
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
    }

}} // namespace enigma::gui
