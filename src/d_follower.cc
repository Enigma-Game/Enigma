/*
 * Copyright (C) 2002,2003,2004,2005,2015 Daniel Heck
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
#include "d_follower.hh"
#include "d_engine.hh"
#include "ecl_math.hh"
#include "errors.hh"

#include <algorithm>

namespace display {

/* -------------------- Follower -------------------- */

Follower::Follower(DisplayEngine *e) : m_boundary_x(0.5), m_boundary_y(0.5), m_engine(e) {
}

void Follower::center(const ecl::V2 &point) {
    double borderh = m_boundary_x;
    double borderv = m_boundary_y;
    double hoff = get_hoff();
    double voff = get_voff();

    V2 off = point;
    off[0] = floor((off[0] - borderh) / hoff) * hoff;
    off[1] = floor((off[1] - borderv) / voff) * voff;

    set_offset(off);
}

double Follower::get_hoff() const {
    ScreenArea gamearea = m_engine->get_area();
    double result = gamearea.w / m_engine->get_tilew() - m_boundary_x * 2;
    ASSERT(result > 0, enigma::XLevelRuntime,
           "FollowThreshold must be less than half of level width/height and 10/6.5");
    return result;
}

double Follower::get_voff() const {
    ScreenArea gamearea = m_engine->get_area();
    double result = gamearea.h / m_engine->get_tileh() - m_boundary_y * 2;
    ASSERT(result > 0, enigma::XLevelRuntime,
           "FollowThreshold must be less than half of level width/height and 10/6.5");
    return result;
}

bool Follower::set_offset(V2 offs) {
    DisplayEngine *e = get_engine();
    offs[0] = std::max(offs[0], 0.0);
    offs[1] = std::max(offs[1], 0.0);
    offs[0] = std::min(offs[0], double(e->get_width() - get_hoff() - m_boundary_x * 2));
    offs[1] = std::min(offs[1], double(e->get_height() - get_voff() - m_boundary_y * 2));
    if (offs != e->get_offset()) {
        e->set_offset(offs);
        return true;
    }
    return false;
}

/* -------------------- Follower_Screen -------------------- */

// Determine whether the screen must be scrolled or not, and change the
// coordinate origin of the screen accordingly.
void Follower_Screen::tick(double, const ecl::V2 &point) {
    DisplayEngine *engine = get_engine();
    V2 oldoff = engine->get_offset();
    Follower::center(point);
    if (oldoff != engine->get_offset())
        engine->mark_redraw_screen();
}

Follower_Screen::Follower_Screen(DisplayEngine *e, double borderx, double bordery) : Follower(e) {
    m_boundary_x = borderx;
    m_boundary_y = bordery;
}

/* -------------------- Follower_Scrolling -------------------- */

Follower_Scrolling::Follower_Scrolling(DisplayEngine *e, bool screenwise_, double borderx,
                                       double bordery)
: Follower(e), currently_scrolling(false), scrollspeed(0), resttime(0), screenwise(screenwise_) {
    m_boundary_x = borderx;
    m_boundary_y = bordery;
}

void Follower_Scrolling::center(const ecl::V2 &point) {
    Follower::center(point);
    curpos = destpos = get_engine()->get_offset();
}

void Follower_Scrolling::tick(double dtime, const ecl::V2 &point) {
    DisplayEngine *engine = get_engine();

    if (!currently_scrolling) {
        ScreenArea gamearea = engine->get_area();
        int tilew = engine->get_tilew();
        int tileh = engine->get_tileh();

        int sx, sy;
        engine->world_to_screen(point, &sx, &sy);

        bool scrollx_p = (sx < gamearea.x + m_boundary_x * tilew) ||
                         (sx >= gamearea.x + gamearea.w - m_boundary_x * tilew);

        bool scrolly_p = (sy < gamearea.y + m_boundary_y * tileh) ||
                         (sy >= gamearea.y + gamearea.h - m_boundary_y * tileh);

        if (scrollx_p || scrolly_p) {
            V2 olddest = destpos;
            V2 scrollpos = engine->get_offset();

            currently_scrolling = true;

            // Move `point' to center of the screen
            curpos = scrollpos;

            if (screenwise) {
                double hoff = get_hoff();
                double voff = get_voff();
                destpos[0] = floor((point[0] - m_boundary_x) / hoff) * hoff;
                destpos[1] = floor((point[1] - m_boundary_y) / voff) * voff;
            } else {
                destpos = point - V2(gamearea.w / tilew, gamearea.h / tileh) / 2;
                // round to grid - a hack just for 20x13 screen TODO rewrite for Enigma 1.2
                // x scroll of "alternating" 10 and 9 grids, try to join this grid after warps
                double xmod = std::fmod(destpos[0], 19);
                if (xmod < 5 || (xmod > 10 && xmod < 14.5))
                    destpos[0] = std::floor(destpos[0]);
                else
                    destpos[0] = std::ceil(destpos[0]);
                // y scroll of stable 6 grids
                destpos[1] = ecl::round_nearest<int>(destpos[1]);
            }

            // Don't scroll off the game area
            destpos[0] = ecl::Clamp<double>(destpos[0], 0.0,
                                            (double)engine->get_width() - gamearea.w / tilew);
            destpos[1] = ecl::Clamp<double>(destpos[1], 0.0,
                                            (double)engine->get_height() - gamearea.h / tileh);
            if (!scrollx_p)
                destpos[0] = olddest[0];
            if (!scrolly_p)
                destpos[1] = olddest[1];
        }
    }

    if (currently_scrolling) {
        scrollspeed = 45.0;
        resttime = length(destpos - curpos) / scrollspeed;

        resttime -= dtime;
        if (resttime <= 0) {
            engine->move_offset(destpos);
            currently_scrolling = false;
        } else {
            dir = normalize(destpos - curpos);
            curpos += dir * scrollspeed * dtime;
            engine->move_offset(curpos);
        }
    }
}

/* -------------------- Follower_Smooth -------------------- */

Follower_Smooth::Follower_Smooth(DisplayEngine *e) : Follower(e) {
}

ecl::V2 Follower_Smooth::calc_offset(const ecl::V2 &point) {
    DisplayEngine *engine = get_engine();
    ScreenArea gamearea = engine->get_area();
    int tilew = engine->get_tilew();
    int tileh = engine->get_tileh();

    V2 destpos = point - V2(double(gamearea.w) / tilew, double(gamearea.h) / tileh) / 2;
    // Round to integer pixel offset
    destpos[0] = ecl::round_nearest<int>(destpos[0] * tilew) / double(tilew);
    destpos[1] = ecl::round_nearest<int>(destpos[1] * tileh) / double(tileh);
    destpos[0] = ecl::Clamp(destpos[0], 0.0, (double)engine->get_width() - gamearea.w / tilew);
    destpos[1] = ecl::Clamp(destpos[1], 0.0, (double)engine->get_height() - gamearea.h / tileh);
    return destpos;
}

void Follower_Smooth::tick(double /*time*/, const ecl::V2 &point) {
    DisplayEngine *engine = get_engine();
    engine->move_offset(calc_offset(point));
}

void Follower_Smooth::center(const ecl::V2 &point) {
    set_offset(calc_offset(point));
}

}  // namespace display
