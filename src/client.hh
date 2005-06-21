/*
 * Copyright (C) 2003, 2004 Daniel Heck
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
 * $Id: client.hh,v 1.1 2004/02/15 11:32:52 dheck Exp $
 */
#ifndef CLIENT_HH_INCLUDED
#define CLIENT_HH_INCLUDED

#include "levels.hh"

namespace enigma_client 
{
    using namespace enigma;

/* -------------------- Functions -------------------- */

    void Tick (double dtime);

    bool AbortGameP();

    void Stop();

/* -------------------- Server->Client messages -------------------- */

    void Msg_Command(const std::string& cmd);
    
    void Msg_AdvanceLevel (levels::LevelAdvanceMode mode);

    void Msg_LevelLoaded(unsigned levelidx);

    void Msg_PlayerPosition (unsigned iplayer, const px::V2 &pos);

    void Msg_Sparkle (const px::V2 &pos);

    void Msg_ShowText (const std::string &text, bool scrolling, double duration=-1);

    //! Play sound emanating from an absolute position in the world
    void Msg_PlaySound (const std::string &soundname, 
                        const px::V2 &pos,
                        double relative_volume);

    //! Play a  sound emanating from the player's current position
    void Msg_PlaySound (const std::string &soundname, double relative_volume);

    //! Stop the game and display an error message instead
    void Msg_Error (const std::string &text);
}

namespace enigma
{
    namespace client = enigma_client;
}

#endif
