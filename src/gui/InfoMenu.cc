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
    static struct {
        int linesPerColumn, columnsPerPage, xOffset, lineWidth, columnGap, yOffset, yGap;
    } pInfo[] = {
        {  // VTS_16 (320x240)
            18, 1, 10, 290,  0, 7, 0
        },
        {  // VTS_32 (640x480)
            21, 1, 20, 580,  0, 14, 0
        },
        {  // VTS_40 (800x600)
            21, 1, 35, 720,  0, 35, 4
        },
        {  // VTS_48 (960x720)  VM_1024x768
            24, 2, 25, 445, 15, 40, 6
        },
        {  // VTS_64 (1280x960)
            33, 2, 25, 580, 20, 40, 6
        }
    };

    InfoMenu::InfoMenu(const char **infotext) : info (infotext),
            curPage (0), current_gc (0) {
        const VMInfo &vminfo = *video_engine->GetInfo();
        const int vshrink = vminfo.width < 640 ? 1 : 0;

        numPages = process_infotext(false);

        but_ok   = new StaticTextButton(N_("Ok"), this);

        add(but_ok, Rect(vminfo.width-(vshrink?80:130), vminfo.height-(vshrink?30:60), vshrink?70:110, vshrink?20:40));
        if (numPages > pInfo[vminfo.tt].columnsPerPage) {
            pgup     = new ImageButton("ic-up", "ic-up1", this);
            pgdown   = new ImageButton("ic-down", "ic-down1", this);
            add(pgup, Rect(vminfo.width-(vshrink?15:30), vminfo.height/2, vshrink?10:20, vshrink?25:50));
            add(pgdown, Rect(vminfo.width-(vshrink?15:30), vminfo.height/2 +(vshrink?35:70), vshrink?10:20, vshrink?25:50));
        }
    }

    void InfoMenu::draw_background(ecl::GC &gc) {
        const VMInfo &vminfo = *video_engine->GetInfo();
        blit(gc, vminfo.mbg_offsetx, vminfo.mbg_offsety, enigma::GetImage("menu_bg", ".jpg"));
        current_gc = &gc;
        process_infotext(true);
    }

    // process_infotext is used during construction to count the
    // total number of pages, and later in draw_background to actually
    // render the current page.
    int InfoMenu::process_infotext(bool render) {
        VideoTileType vtt = (video_engine->GetInfo())->tt;
        Font *f = enigma::GetFont("menufont");
        int page = 1;
        int column = 0;
        int linecount = 0;
        int linewidth = pInfo[vtt].lineWidth;
        int colwidth = linewidth + pInfo[vtt].columnGap;
        int x = pInfo[vtt].xOffset;
        int y = pInfo[vtt].yOffset;
        for (int row = 0; info[row]; row++)
            if (info[row][0] == '\n') {
                column += 1;
                linecount = 0;
                y = pInfo[vtt].yOffset;
                if (column > pInfo[vtt].columnsPerPage - 1) {
                    page += 1;
                    column = 0;
                }
            } else {
                std::string t = _(info[row]);
                std::vector<std::string> lines = ecl::breakToLines(f, t, " ", linewidth);
                for (auto it = lines.begin(); it != lines.end(); it++) {
                    linecount += 1;
                    if (linecount > pInfo[vtt].linesPerColumn) {
                        column += 1;
                        linecount = 1;
                        y = pInfo[vtt].yOffset;
                        if (column > pInfo[vtt].columnsPerPage - 1) {
                            page += 1;
                            column = 0;
                        }
                    }
                    if (render && (page == curPage + 1))
                        f->render (*current_gc, x + column * colwidth, y, *it);
                    y += f->get_height() + pInfo[vtt].yGap;
                }
            }
        return page;
    }

    void InfoMenu::on_action (gui::Widget *w) {
        if (w == but_ok) {
            Menu::quit();
        } else if (pgup && w == pgup) {
            if (curPage > 0) {
                curPage--;
                invalidate_all();
            }
        } else if (pgdown && w == pgdown) {
            if (curPage < numPages - 1) {
                curPage++;
                invalidate_all();
            }
        }
    }
    
    void displayInfo(const char **infotext) {
        FX_Fade (video::FADEOUT);
        InfoMenu menu(infotext);
        menu.draw_all();
        FX_Fade (video::FADEIN);
        menu.manage();
    }

}} // namespace enigma::gui
