/*
 * Copyright (C) 2021 Andreas Lochmann
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

#include "gui/LanguageMenu.hh"
#include "ecl_video.hh"
#include "ecl_math.hh"
#include "enigma.hh"
#include "main.hh"
#include "nls.hh"
#include "options.hh"
#include "video.hh"
#include "resource_cache.hh"

#include <cassert>
#include <cmath>

using namespace ecl;
using namespace std;

namespace enigma { namespace gui {
/* -------------------- LanguageMenu -------------------- */
    
    LanguageMenu::LanguageMenu () :
        ok (new gui::StaticTextButton(N_("Ok"), this))
    {
        const VMInfo &vminfo = *video_engine->GetInfo();
        const int vshrink = vminfo.width < 640 ? 1 : 0;

        const int num_langs = NUMENTRIES(nls::languages) - 1;
        const int listWidth = vminfo.width - (vshrink ? 40 : 80);
        const int listHeight = vminfo.height - (vshrink ? 80 : 160);
        const int buttonWidth = (vminfo.tt == 0) ? 80 : 160;
        const int buttonHeight = (vminfo.tt == 0) ? 17 : 35;
        const int bbDist = vshrink ? 5 : 10;
        // Distribute the buttons in a rectangle with roughly the
        // same proportions as the available space.
        // (Add 1 for tweaking.)
        const int langs_per_column = ecl::round_up<int>(std::sqrt(
            ((float) num_langs * (buttonWidth + bbDist) * listHeight)
              / ((buttonHeight + bbDist) * listWidth))) + 1;
        const int num_columns = ecl::round_up<int>(((float) num_langs) / langs_per_column);
        std::string curname = app.language;
        language_buttons.clear();

        add(ok, Rect(vminfo.width  - (vshrink ? 85 : 170),
                     vminfo.height - (vshrink ? 30 : 60),
                     vshrink ? 75 : 150,
                     vshrink ? 20 : 40));

        cols_hlist = new HList;
        cols_hlist->set_spacing(bbDist);
        cols_hlist->set_alignment(HALIGN_LEFT, VALIGN_TOP);
        cols_hlist->set_default_size(buttonWidth, listHeight);
        VList* buttons_vlist;
        int k = 1;
        for (size_t i = 0; i < num_columns; i++) {
            buttons_vlist = new VList;
            buttons_vlist->set_spacing(bbDist);
            buttons_vlist->set_alignment(HALIGN_LEFT, VALIGN_TOP);
            buttons_vlist->set_default_size(buttonWidth, buttonHeight);
            for (size_t j = 0; j < langs_per_column; j++, k++) {
                if (k < num_langs + 1) {
                    TextButton * button = new UntranslatedStaticTextButton(nls::languages[k].name, this);
                    button->setHighlight(curname == nls::languages[k].localename);
                    language_buttons.push_back(button);
                    buttons_vlist->add_back(button);
                }
            }
            cols_hlist->add_back(buttons_vlist);
        }
        const int realWidth = num_columns * (buttonWidth + bbDist) - bbDist;
        const int realHeight = langs_per_column * (buttonHeight + bbDist) - bbDist;
        int xOffset = (vminfo.width - realWidth) / 2;
        int yOffset = (vminfo.height - (vshrink ? 30 : 60) - realHeight) / 2;
        this->add(cols_hlist, Rect(xOffset, yOffset, realWidth, realHeight));
    }

    void LanguageMenu::draw_background(ecl::GC &gc) 
    {
        const VMInfo *vminfo = video_engine->GetInfo();
        set_caption(_("Enigma - Language Selection"));
        blit(gc, vminfo->mbg_offsetx, vminfo->mbg_offsety, enigma::GetImage("menu_bg", ".jpg"));
    }

    void LanguageMenu::on_action(gui::Widget *w) {
        if (w == ok) {
            Menu::quit();
        } else if (language_buttons.size() > 0) {
            for (size_t i = 0; i < language_buttons.size(); i++)
                if (w == language_buttons[i]) {
                    options::SetOption ("Language", nls::languages[i + 1].localename);
                    app.setLanguage(nls::languages[i + 1].localename);
                    break;
                }
            // update highlighting of buttons
            std::string curname = app.language;
            for (size_t i = 0; i < language_buttons.size(); i++)
                language_buttons[i]->setHighlight(nls::languages[i + 1].localename == curname);
            invalidate_all();
        }
    }

}} // namespace enigma::gui
