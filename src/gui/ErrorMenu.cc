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
 *
 */

#include "gui/ErrorMenu.hh"
#include "enigma.hh"
#include "video.hh"

using namespace ecl;
using namespace std;

namespace enigma { namespace gui {

    /* -------------------- HelpMenu -------------------- */

    ErrorMenu::ErrorMenu(std::string message, std::string quitTitle) : 
        text (message), quit (new gui::StaticTextButton(quitTitle, this)),
        rejectQuit (false), laterQuit (false) {
        const video::VMInfo *vminfo = video::GetInfo();
        const int vshrink = vminfo->width < 640 ? 1 : 0;
        add(quit, Rect(vminfo->width-(vshrink?85:170), vminfo->height-(vshrink?30:60), 
                       vshrink?75:150, vshrink?42:40));
    }
    
    ErrorMenu::ErrorMenu(std::string message, std::string quitTitle, std::string rejectTitle) : 
            text (message), 
            quit (new gui::StaticTextButton(quitTitle, this)),
            reject (new gui::StaticTextButton(rejectTitle, this)),
            rejectQuit (false), laterQuit (false) {
        const video::VMInfo *vminfo = video::GetInfo();
        const int vshrink = vminfo->width < 640 ? 1 : 0;
        add(quit, Rect(vminfo->width-(vshrink?85:170), vminfo->height-(vshrink?30:60), vshrink?75:150, vshrink?42:40));
        add(reject, Rect(vminfo->width-(vshrink?170:340), vminfo->height-(vshrink?30:60), vshrink?75:150, vshrink?42:40));
    }
    
    ErrorMenu::ErrorMenu(std::string message, std::string quitTitle, std::string rejectTitle,
            std::string laterTitle) : 
            text (message), quit (new gui::StaticTextButton(quitTitle, this)),
            reject (new gui::StaticTextButton(rejectTitle, this)),
            later (new gui::StaticTextButton(laterTitle, this)),
            rejectQuit (false), laterQuit (false) {
        const video::VMInfo *vminfo = video::GetInfo();
        const int vshrink = vminfo->width < 640 ? 1 : 0;
        add(quit, Rect(vminfo->width-(vshrink?85:170), vminfo->height-(vshrink?30:60), vshrink?75:150, vshrink?42:40));
        add(later, Rect(vminfo->width-(vshrink?170:340), vminfo->height-(vshrink?30:60), vshrink?75:150, vshrink?42:40));
        add(reject, Rect(vminfo->width-(vshrink?255:510), vminfo->height-(vshrink?30:60), vshrink?75:150, vshrink?42:40));
    }
    
    bool ErrorMenu::isRejectQuit() {
        return rejectQuit;
    }
    
    bool ErrorMenu::isLaterQuit() {
        return laterQuit;
    }
    
    bool ErrorMenu::on_event (const SDL_Event &e) 
    {
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT) {
            Menu::quit();
            return true;
        }
        return false;
    }
    
    void ErrorMenu::on_action (gui::Widget *w) {
        if (w == quit) {
            Menu::quit();
        } else if (w == reject) {
            rejectQuit = true;
            Menu::quit();
        } else if (w == later) {
            laterQuit = true;
            Menu::quit();
        }
    }
    
    void ErrorMenu::draw_background (ecl::GC &gc) {    
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
        Font *f = enigma::GetFont("menufont");

        vector<string> lines;

        ecl::split_copy (text, '\n', back_inserter(lines));
        int x     = 60;
        int y     = 60;
        int yskip = 25;
        const video::VMInfo *vminfo = video::GetInfo();
        int width = vminfo->width - 120;
        for (unsigned i=0; i<lines.size(); ) {
            std::string::size_type breakPos = breakString(f, lines[i], 
                    " ", width);
            f->render(gc, x,  y, lines[i].substr(0,breakPos).c_str());
            y += yskip;
            if (breakPos != lines[i].size()) {
                // process rest of line
                lines[i] = lines[i].substr(breakPos);
            } else {
                // process next line
                i++;
            }
        }
    }    
}} // namespace enigma::gui
