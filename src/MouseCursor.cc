/*
 * Copyright (C) 2002,2004 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
 * Copyright (C) 2020-2022 Andreas Lochmann
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

#include "MouseCursor.hh"

using namespace ecl;
using namespace enigma;

/* -------------------- MouseCursor -------------------- */

void MouseCursor::set_image(ecl::Surface *s, int hx, int hy) {
    cursor.reset(s);
    hotx = hx;
    hoty = hy;

    if (visible > 0)
        init_bg();
}

void MouseCursor::draw() {
    if (visible > 0) {
        grab_bg();

        GC gc(screen->get_surface());
        blit(gc, x - hotx, y - hoty, cursor.get());

        changed = false;
    }
}

void MouseCursor::redraw() {
    if (visible > 0 && changed) {
        restore_bg();
        draw();
    }
}

void MouseCursor::move(int newx, int newy) {
    x = (int)((double) (newx * screen->size().w) / screen->window_size().w + 0.5);
    y = (int)((double) (newy * screen->size().h) / screen->window_size().h + 0.5);
    changed = true;
}

void MouseCursor::show() {
    if (++visible == 1) {
        init_bg();
        changed = true;
    }
}

void MouseCursor::hide() {
    if (--visible == 0) {
        changed = true;
        restore_bg();
        background.reset(nullptr);
    }
}

Rect MouseCursor::get_rect() const {
    return Rect(x - hotx, y - hoty, cursor->width(), cursor->height());
}

Rect MouseCursor::get_oldrect() const {
    return Rect(oldx - hotx, oldy - hoty, cursor->width(), cursor->height());
}

void MouseCursor::init_bg() {
    assert(visible > 0);
    assert(cursor != 0);

    background.reset(ecl::MakeSurface(cursor->width(), cursor->height()));
    assert(background);
    grab_bg();
}

void MouseCursor::grab_bg() {
    assert(background != 0);

    GC gc(background.get());
    blit(gc, 0, 0, screen->get_surface(), get_rect());

    oldx = x;
    oldy = y;
}

void MouseCursor::restore_bg() {
    if (background) {
        GC gc(screen->get_surface());
        blit(gc, oldx - hotx, oldy - hoty, background.get());
    }
}


