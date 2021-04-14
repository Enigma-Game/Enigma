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
#include "gui/HelpMenu.hh"
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
    : zoomed(0), complete(0),
      zoomxpos(zoomxpos_),
      zoomypos(zoomypos_)
    {
        const VMInfo *vminfo = video_engine->GetInfo();

        resume  = new gui::StaticTextButton(N_("Resume Level (ESC)"), this);
        restart = new gui::StaticTextButton(N_("Restart Level (Shift-F3)"), this);
        options = new gui::StaticTextButton(N_("Options"), this);
        info    = new gui::StaticTextButton(N_("Level Info"), this);
        scrshot = new gui::StaticTextButton(N_("Screenshot (F10)"), this);
        abort   = new gui::StaticTextButton(N_("Abort Level"), this);
        bosskey = new gui::StaticTextButton(N_("Exit Enigma (Shift-ESC)"), this);

        if (vminfo->width < 640) {
            add(resume,     Rect(0,0,145,40));
            add(restart,    Rect(0,45,145,40));
            add(options,    Rect(0,90,145,40));
            add(info,       Rect(150,0,145,40));
            add(abort,      Rect(150,45,145,40));
            add(bosskey,    Rect(150,90,145,40));
        } else {
            add(resume,     Rect(0,0,220,40));
            add(restart,    Rect(0,45,220,40));
            add(options,    Rect(0,90,220,40));
            add(info,       Rect(0,135,220,40));
            add(scrshot,    Rect(0,180,220,40));
            add(abort,      Rect(0,270,220,40));
            add(bosskey,    Rect(0,315,220,40));
        }
        center();
    }
    
    GameMenu::~GameMenu() {
        delete zoomed;
        delete complete;
    }
    
    void GameMenu::draw_background(ecl::GC &gc) 
    {
        const VMInfo *vminfo = video_engine->GetInfo();

        if (!zoomed) {
            Rect game_area   = display::GetGameArea();
            complete = Grab(video_engine->GetScreen()->get_surface(), game_area);
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

    static const char *helptext_gamemenu[] = {
        N_("F1:"), N_("Show this help"),
        N_("F3:"), N_("Kill current marble"),
        N_("Shift+F3:"), N_("Restart the current level"),
        N_("Shift+Ctrl+F3:"), N_("Reload and restart the current level"),
        N_("Right mouse button:"), N_("Return to level"),
        " ", " ",
        "FREE TEXT FOLLOWS",
        N_("For a list of shortcut keys during the game, press F1 during the game."),
        0
    };

    bool GameMenu::on_event (const SDL_Event &e) 
    {
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT) {
            Menu::quit();
            return true;
        }
        if (e.type == SDL_KEYDOWN) {
            Uint16 keymod = e.key.keysym.mod;
            SDL_Keycode keysym = e.key.keysym.sym;
            switch (keysym) {
            case SDLK_F3: {
                lev::Index *ind = lev::Index::getCurrentIndex();
                if (keymod & KMOD_SHIFT & KMOD_CTRL) {
                    lev::Proxy::releaseCache();
                    client::Stop ();
                    server::Msg_LoadLevel(ind->getCurrent(), false);
                } else if (keymod & KMOD_SHIFT) {
                    client::Stop ();
                    server::Msg_LoadLevel(ind->getCurrent(), false);
                } else {
                    server::Msg_Command("suicide");
                }
                Menu::quit();
                return true;
                break; }
            case SDLK_F1: {
                displayHelp(helptext_gamemenu, 200);
                draw_all();
                break; }
            default: break;
            };
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
        else if (w == bosskey) {
            client::Msg_Command("abort");
            app.bossKeyPressed = true;
        }
        else if (w == scrshot) {
            video_engine->Screenshot(server::LoadedProxy->getNextScreenshotPath(), complete);
        }
    }
}} // namespace enigma::gui
