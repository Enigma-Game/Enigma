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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id: help.cc,v 1.5 2004/05/22 13:04:31 dheck Exp $
 */

#include "enigma.hh"
#include "gui.hh"
#include "ecl.hh"
#include "help.hh"
#include "video.hh"
#include "nls.hh"

using namespace enigma;
using namespace ecl;
using namespace std;

struct HelpMenuConfig {
    int         x0, x1;         // x coordinates of first and second column
    int         y0;             // y coordinate
    int         yskip;
    std::string fontname;

    HelpMenuConfig (int xoffset) {
        x0       = 60;
        x1       = x0 + xoffset;
        y0       = 60;
        yskip    = 30;
        fontname = "menufont";
    }
};

class HelpMenu : public gui::Menu {
public:
    HelpMenu (const char **helptext_, int xoffset);
private:
    bool on_event           (const SDL_Event &e);
    void on_action          (gui::Widget *w);
    void draw_background    (ecl::GC &gc);

    const char     **helptext;
    gui::Widget     *ok;
    HelpMenuConfig   cfg;
};

/* -------------------- HelpMenu -------------------- */

HelpMenu::HelpMenu (const char **helptext_, int xoffset_) : 
    helptext    (helptext_),
    ok          (new gui::StaticTextButton("Ok", this)),
    cfg         (xoffset_)
{
    add(ok, Rect(640-170,480-60,150,40));
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
    blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
    Font *f = enigma::GetFont(cfg.fontname.c_str());

    int y = cfg.y0;
    for (int i = 0; helptext[i]; i += 2) 
    {
        f->render (gc, cfg.x0, y, _(helptext[i]));
        f->render (gc, cfg.x1, y, _(helptext[i+1]));
        y += cfg.yskip;
    }
}

/* -------------------- Functions -------------------- */

void enigma::displayHelp(const char **helptext, int xoffset) 
{
    FX_Fade (video::FADEOUT);
    HelpMenu menu(helptext, xoffset);
    menu.draw_all();
    FX_Fade (video::FADEIN);
    menu.manage();
}

