/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
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
#ifndef MOUSECURSOR_HH_INCLUDED
#define MOUSECURSOR_HH_INCLUDED

// This file contains code for displaying the mouse cursor.

#include <memory>
#include <cassert>
#include "ecl_video.hh"

namespace enigma {

class MouseCursor {
public:
    MouseCursor(ecl::Screen *screen) : screen(screen) { SDL_AddEventWatch(event_filter, this); }
    ~MouseCursor() { SDL_DelEventWatch(event_filter, this); }

    void set_image(ecl::Surface *s, int hotx_, int hoty_);
    void move(int newx, int newy);
    void redraw();  // Redraw if position/image changed
    void draw();    // Draw cursor if visible
    void show();
    void hide();
    ecl::Rect get_rect() const;
    ecl::Rect get_oldrect() const;

    bool has_changed() { return changed; }
    int get_x() const { return x; }
    int get_y() const { return y; }

private:
    // This function is installed as an event watch by the constructor. It
    // tracks mouse events to keep the position of the mouse cursor updated.
    static int event_filter(void *data, SDL_Event *e) {
        MouseCursor *cursor = static_cast<MouseCursor *>(data);
        if (e->type == SDL_MOUSEMOTION) {
            cursor->move(e->motion.x, e->motion.y);
            cursor->redraw();
        }
        return 1;
    }

    // Private methods
    void grab_bg();
    void init_bg();
    void restore_bg();

    // Variables
    ecl::Screen *screen;
    std::unique_ptr<ecl::Surface> background;  // Copy of screen contents behind cursor
    std::unique_ptr<ecl::Surface> cursor;      // Pixmap of the cursor

    int x = 0, y = 0;
    int oldx = 0, oldy = 0;

    // Coordinates of hotspot inside cursor image
    int hotx = 0, hoty = 0;
    int visible = 0;
    bool changed = true;
};

} // namespace enigma

#endif
