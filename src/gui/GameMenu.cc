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
 
#include "gui/GameMenu.hh"
#include "gui/OptionsMenu.hh"
#include "gui/LevelInspector.hh"
#include "client.hh"
#include "display.hh"
#include "ecl_video.hh"
#include "main.hh"
#include "nls.hh"
#include "server.hh"
#include "video.hh"
#include "lev/Index.hh"
#include "lev/Proxy.hh"

#include <cassert>

using namespace ecl;
using namespace std;

namespace enigma { namespace gui {
/* -------------------- GameMenu -------------------- */
    
    GameMenu::GameMenu (int zoomxpos_, int zoomypos_)
    : zoomed(0),
      zoomxpos(zoomxpos_),
      zoomypos(zoomypos_)
    {
        resume  = new gui::StaticTextButton(N_("Resume Level"), this);
        restart = new gui::StaticTextButton(N_("Restart Level"), this);
        options = new gui::StaticTextButton(N_("Options"), this);
        info    = new gui::StaticTextButton(N_("Level Info"), this);
        abort   = new gui::StaticTextButton(N_("Abort Level"), this);
    
        add(resume,     Rect(0,0,200,40));
        add(restart,    Rect(0,45,200,40));
        add(options,    Rect(0,90,200,40));
        add(info,       Rect(0,135,200,40));
        add(abort,      Rect(0,180,200,40));
        center();
    }
    
    GameMenu::~GameMenu() {
        delete zoomed;
    }
    
    void GameMenu::draw_background(ecl::GC &gc) 
    {
        const VMInfo *vminfo = video_engine->GetInfo();

        if (!zoomed) {
            Rect game_area   = display::GetGameArea();
            int  part_width  = game_area.w/3;
            int  part_height = (part_width*vminfo->height)/vminfo->width;
    
            if (part_height > game_area.h) {
                part_height = game_area.h/3;
                part_width  = (part_height*vminfo->width)/vminfo->height;
                assert(part_width <= game_area.w);
            }
    
            // randomly choose ball offset
            int x, y;
            for (int trials = 5; trials; --trials) {
                x = IntegerRand(0, 5, false);
                y = IntegerRand(0, 3, false);
    
                // try to avoid menu-ball overlap:
                if (x<2 || x>3 || y<1 || y>2 || (trials == 1)) {
                    int ax = zoomxpos-game_area.x;
                    int ay = zoomypos-game_area.y;
    
                    // upper left corner of part
                    x = ax/32-1-x;
                    y = ay/32-1-y;
    
                    // ensure part is inside game_area
                    x = max(0, min(x, (game_area.w-part_width)/32-1));
                    y = max(0, min(y, (game_area.h-part_height)/32-1));
    
                    // adjust to game fields
                    x = x*32+24;
                    y = y*32+16;
    
                    break;
                }
            }
    
            // Be sure to redraw everything, or actors may appear on top
            // of the stones (actors are drawn in one pass and only
            // clipped to the screen boundary).
            display::RedrawAll(video_engine->GetScreen());

            // get the selected part from screen
            Rect     src_area(game_area.x+x, game_area.y+y, part_width, part_height);
            Surface *src = Grab(video_engine->GetScreen()->get_surface(), src_area);

            zoomed = src->zoom(vminfo->width+1, vminfo->height+1);
            delete src;
        }
    
        ecl::blit(gc, 0,0, zoomed);
    }
    
    bool GameMenu::on_event (const SDL_Event &e) 
    {
        if (e.type == SDL_MOUSEBUTTONDOWN
            && e.button.button == SDL_BUTTON_RIGHT)
        {
            Menu::quit();
            return true;
        }
        return false;
    }
    
    void GameMenu::on_action(gui::Widget *w) {
        lev::Index *ind = lev::Index::getCurrentIndex();
        if (w == resume) {
            Menu::quit();
        }
        else if (w == abort) {
            client::Msg_Command("abort");
            Menu::quit();
        }
        else if (w == restart) {
            if (w->lastModifierKeys() & KMOD_CTRL && w->lastModifierKeys() & KMOD_SHIFT) {
                // force a reload from file
                lev::Proxy::releaseCache();
            }
            client::Stop ();
            server::Msg_LoadLevel(ind->getCurrent(), false);
            Menu::quit();
            
        }
        else if (w == options) {
            enigma::gui::ShowOptionsMenu (0, true);
            invalidate_all();
//            Menu::quit();
        }
        else if (w == info) {
            LevelInspector m(ind->getCurrent());
            m.manage();
            invalidate_all();
//            Menu::quit();
        }
    }
}} // namespace enigma::gui
