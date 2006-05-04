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
#ifndef GUI_LEVELPACKMENU_HH_INCLUDED
#define GUI_LEVELPACKMENU_HH_INCLUDED

#include "gui/Menu.hh"

namespace enigma { namespace gui {
/* -------------------- LevelPackMenu -------------------- */

    class LevelPackMenu : public Menu {
    public:
        LevelPackMenu();

        void on_action(Widget *w);
        void draw_background(ecl::GC &gc);
        int get_selection() const { return m_selection; }

    private:
        std::vector<Widget *> buttons;
        int m_selection;
    };
}} // namespace enigma::gui
#endif
