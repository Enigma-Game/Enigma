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
#include "gui/InfoMenu.hh"
#include "ecl_font.hh"
#include "ecl_video.hh"
#include "resource_cache.hh"
#include "video.hh"
#include "video_effects.hh"
#include "nls.hh"

using namespace ecl;
using namespace std;

namespace enigma { namespace gui {
    InfoMenu::InfoMenu(const char **infotext, int pages) : info (infotext),
            curPage (0), numPages (pages) {
        const VMInfo &vminfo = *video_engine->GetInfo();
        const int vshrink = vminfo.width < 640 ? 1 : 0;

        but_ok   = new StaticTextButton(N_("Ok"), this);
        pgup     = new ImageButton("ic-up", "ic-up1", this);
        pgdown   = new ImageButton("ic-down", "ic-down1", this);

        if (pages > 1) {
            add(but_ok, Rect(vminfo.width-(vshrink?80:130), vminfo.height-(vshrink?30:60), vshrink?70:110, vshrink?20:40));
            add(pgup, Rect(vminfo.width-(vshrink?15:30), vminfo.height/2, vshrink?10:20, vshrink?25:50));
            add(pgdown, Rect(vminfo.width-(vshrink?15:30), vminfo.height/2 +(vshrink?35:70), vshrink?10:20, vshrink?25:50));
        }
    }
    
    void InfoMenu::draw_background(ecl::GC &gc) {
        const VMInfo &vminfo = *video_engine->GetInfo();
        const int vshrink = vminfo.width < 640 ? 1 : 0;
        blit(gc, vminfo.mbg_offsetx, vminfo.mbg_offsety, enigma::GetImage("menu_bg", ".jpg"));
        
        Font *f = enigma::GetFont("menufont");
        int row = 0;
        int yoff[] = {0, 0, -20, -40, -40};
        int ygap[] = {0, 0, 4, 6, 6};
        for (int p=0; p<curPage; p++) {
            while (info[row])
                row++;
            // select first row of next page
            row++;
        }
        int linewidth = vshrink ? 290 : 580;
        int x = (vminfo.width - linewidth)/2;
        int y = (vshrink?10:20) + yoff[vminfo.tt] + (vminfo.height-(vshrink?240:480))/2;
        for (int i = 0; info[row]; row++, i++) {
            const char *t = _(info[row]);
            std::vector<std::string> lines = ecl::breakToLines(f, t, " ", linewidth);
            for (auto it = lines.begin(); it != lines.end(); it++) {
                f->render (gc, x, y, *it);
                y += f->get_height() + ygap[vminfo.tt];
            }
        }
    }
    
    void InfoMenu::on_action (gui::Widget *w) {
        if (w == but_ok) {
            Menu::quit();
        } else if (w == pgup) {
            if (curPage > 0) {
                curPage--;
                invalidate_all();
            }
        } else if (w == pgdown) {
            if (curPage < numPages - 1) {
                curPage++;
                invalidate_all();
            }
        }
    }
    
    void displayInfo(const char **infotext, int pages) {
        FX_Fade (video::FADEOUT);
        InfoMenu menu(infotext, pages);
        menu.draw_all();
        FX_Fade (video::FADEIN);
        menu.manage();
    }

}} // namespace enigma::gui
