/*
 * Copyright (C) 2003, 2015 Daniel Heck
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
#ifndef ENIGMA_D_FOLLOWER_HH_INCLUDED
#define ENIGMA_D_FOLLOWER_HH_INCLUDED

#include "ecl_math.hh"

namespace display {

class DisplayEngine;

// Base class for different kinds of sprite followers.
class Follower {
public:
    Follower(DisplayEngine *e);
    virtual ~Follower() {}

    // This function is called by the display engine during each screen
    // update. 'dtime' is the time since the last frame in seconds and point
    // is the position of the sprite that is being followed.
    virtual void tick(double dtime, const ecl::V2 &point) = 0;

    virtual void center(const ecl::V2 &point);

    void set_boundary(double b) {
        m_boundary_x = b;
        m_boundary_y = b;
    }

protected:
    DisplayEngine *get_engine() const { return m_engine; }
    bool set_offset(ecl::V2 offs);
    double get_hoff() const;
    double get_voff() const;
    ecl::V2 get_scrollpos(const ecl::V2 &point);

    double m_boundary_x;
    double m_boundary_y;

private:
    DisplayEngine *m_engine;
};

// Follows a sprite by flipping to the next screen as soon as the sprite
// reaches the border of the current screen.
class Follower_Screen : public Follower {
public:
    Follower_Screen(DisplayEngine *e, double borderx = 0.5, double bordery = 0.5);
    void tick(double dtime, const ecl::V2 &point);
};

// Follows a sprite by softly scrolling the visible area to the next screen as
// soon as the sprite reaches the border of the current screen.
class Follower_Scrolling : public Follower {
public:
    Follower_Scrolling(DisplayEngine *e, bool screenwise, double borderx = 0.5,
                       double bordery = 0.5);
    void tick(double dtime, const ecl::V2 &point);
    void center(const ecl::V2 &point);

private:
    bool currently_scrolling;
    ecl::V2 curpos, destpos;
    ecl::V2 dir;
    double scrollspeed;
    double resttime;
    bool screenwise;
};

// Follows a sprite by keeping it centered on the screen at all times.
class Follower_Smooth : public Follower {
public:
    Follower_Smooth(DisplayEngine *e);
    void tick(double time, const ecl::V2 &point);
    void center(const ecl::V2 &point);
    virtual void set_boundary(double /*b*/) {}

    ecl::V2 calc_offset(const ecl::V2 &point);
};

}  // namespace display

#endif  // ENIGMA_D_FOLLOWER_HH_INCLUDED
