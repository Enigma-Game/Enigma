/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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
#include "main.hh"
#include "options.hh"
#include "video.hh"
#include "client.hh"
#include "server.hh"
#include "world.hh"
#include "sound.hh"

#include "ecl_sdl.hh"
#include <cassert>

using namespace enigma;
using namespace std;


/* -------------------- Global variables -------------------- */

namespace
{
    Uint32      last_tick_time;
}


/* -------------------- Level previews -------------------- */

bool game::DrawLevelPreview (ecl::GC &gc, const Level &l)
{
    LevelPack   *lp       = l.get_levelpack();
    unsigned     levelidx = l.get_index();
    bool success = false;

    server::CreatingPreview = true;
    sound::TempDisableSound();
    try {
        server::Msg_SetLevelPack(lp->get_name());
        server::Msg_LoadLevel (levelidx);


        display::DrawAll(gc);
        success = true;
    }
    catch (levels::XLevelLoading &) {
        ;
    }
    server::CreatingPreview = false;
    sound::TempReEnableSound();
    return success;
}



/* -------------------- Functions -------------------- */

void game::StartGame (levels::LevelPack *lp, unsigned ilevel)
{
    server::InitNewGame();

    video::HideMouse();
    sdl::TempInputGrab grab(enigma::Nograb ? SDL_GRAB_OFF : SDL_GRAB_ON);

    server::CurrentLevelPack = lp;
    server::CurrentLevel = ilevel; // XXX

    server::Msg_SetLevelPack (lp->get_name());
    server::Msg_LoadLevel (ilevel);

    double dtime = 0;
    last_tick_time=SDL_GetTicks();
    while (!client::AbortGameP()) {
        try {
            client::Tick (dtime);
            server::Tick (dtime);
        }
        catch (enigma_levels::XLevelRuntime& err) {        
            client::Msg_Error (string("Server Error: level runtime error:\n")
                               + err.what());
            server::Msg_Panic(true);
        }

        int sleeptime = 10 - (SDL_GetTicks()-last_tick_time);
        if (sleeptime >= 3)      // only sleep if relatively idle
            SDL_Delay(sleeptime);

        Uint32 current_tick_time = SDL_GetTicks();
        dtime = (current_tick_time - last_tick_time)/1000.0;

        if (abs(1-dtime/0.01) < 0.2) {
            // less than 20% deviation from desired frame time?
            dtime = 0.01;
            last_tick_time += 10;
        } else {
            last_tick_time = current_tick_time;
        }

	if (dtime > 500.0) /* Time has done something strange, perhaps
			      run backwards */
            dtime = 0.0;
// 	else if (dtime > 0.5)
//             dtime = 0.5;
    }
    // add last played level
    levels::AddHistory(server::CurrentLevelPack, server::CurrentLevel);

    video::ShowMouse();
}

void game::ResetGameTimer() 
{
    last_tick_time=SDL_GetTicks();
}


