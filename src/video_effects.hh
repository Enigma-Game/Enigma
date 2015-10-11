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
#ifndef VIDEO_EFFECTS_HH_INCLUDED
#define VIDEO_EFFECTS_HH_INCLUDED

#include <memory>
#include "ecl_video.hh"

namespace video {

class TransitionEffect {
public:
    virtual ~TransitionEffect() {}
    virtual void tick(double dtime) = 0;
    virtual bool finished() const = 0;
};

enum FadeMode { FADEIN, FADEOUT };
void FX_Fade(FadeMode mode);
void FX_Fly(ecl::Surface *newscr, int originx, int originy);

enum TransitionModes {
    TM_NONE,
    TM_FLY_N,
    TM_FLY_S,
    TM_FLY_W,
    TM_FLY_E,
    TM_FLY_NW,
    TM_FLY_NE,
    TM_FLY_SE,
    TM_FLY_SW,
    TM_PUSH_RANDOM,
    TM_PUSH_N,
    TM_PUSH_S,
    TM_PUSH_W,
    TM_PUSH_E
};

void ShowScreen(TransitionModes tm, ecl::Surface *newscr);

// Creates a new transition effect for displaying new_screen.
std::unique_ptr<TransitionEffect> CreateEffect(TransitionModes tm, ecl::Surface *new_screen);

}  // namespace video

#endif
