/*
 * Copyright (C) 2003,2004 Daniel Heck, Ralf Westram
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

#include "gui/HelpMenu.hh"
#include "ecl_font.hh"
#include "ecl_video.hh"
#include "resource_cache.hh"
#include "video.hh"
#include "video_effects.hh"
#include "nls.hh"

#include <cassert>

using namespace ecl;
using namespace std;

namespace enigma { namespace gui {

    /* -------------------- HelpMenu -------------------- */

    HelpMenu::HelpMenu (const char **helptext_, int xoffset_) : 
        helptext    (helptext_),
        ok          (new gui::StaticTextButton(N_("Ok"), this)),
        cfg         (xoffset_)
    {
        const VMInfo &vminfo = *video_engine->GetInfo();
        const int vshrink = vminfo.width < 640 ? 1 : 0;
        
        add(ok, Rect(vminfo.width  - (vshrink ? 85 : 170),
                     vminfo.height - (vshrink ? 30 : 60),
                     vshrink ? 75 : 150,
                     vshrink ? 20 : 40));
    }
    
    bool HelpMenu::on_event (const SDL_Event &e) 
    {
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT) 
        {
            Menu::quit();
            return true;
        }
        return false;
    }
    
    void HelpMenu::on_action (gui::Widget *w) 
    {
        if (w == ok)
            Menu::quit();
    }
    
    void HelpMenu::draw_background (ecl::GC &gc) 
    {
        const VMInfo &vminfo = *video_engine->GetInfo();
        const int vshrink = vminfo.width < 640 ? 1 : 0;

        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
        Font *f = enigma::GetFont(cfg.fontname.c_str());
    
        int x = (vminfo.width - (vshrink ? 320 : 640))/2;
        int y = (cfg.y0/(vshrink?2:1)) + (vminfo.height - (vshrink?240:480))/2;
        for (int i = 0; helptext[i]; i += 2) 
        {
            assert(helptext[i+1]);
            // If assert stops here, and you've worked on the game
            // help menu, check Client::show_help(): Here one of
            // the text lines is redefined. Correct the line number.
            if (helptext[i] == "FREE TEXT FOLLOWS") {
                // TODO: Break helptext[i+1], use ecl::breakStringToLines
                f->render (gc, cfg.x0/(vshrink?2:1) + x, y, _(helptext[i+1]));  // translate
                y += cfg.yskip/(vshrink?2:1);
            } else {
                f->render (gc, cfg.x0/(vshrink?2:1) + x, y, _(helptext[i]));    // translate
                f->render (gc, cfg.x1/(vshrink?2:1) + x, y, _(helptext[i+1]));  // translate
                y += cfg.yskip/(vshrink?2:1);
            }
        }
    }
    
    /* -------------------- Functions -------------------- */
    
    void displayHelp(const char **helptext, int xoffset) 
    {
        FX_Fade (video::FADEOUT);
        HelpMenu menu(helptext, xoffset);
        menu.draw_all();
        FX_Fade (video::FADEIN);
        menu.manage();
    }

}} // namespace enigma::gui
