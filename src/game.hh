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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id: game.hh,v 1.30 2004/03/15 20:15:36 dheck Exp $
 */
#ifndef ENIGMA_GAME_HH
#define ENIGMA_GAME_HH

#include "fwd.hh"
#include "ecl_fwd.hh"

namespace enigma_game
{
    using enigma_levels::LevelPack;
    using enigma_levels::Level;

    void ResetGameTimer();

    void StartGame (LevelPack *lp, unsigned levelidx);

    /*! Draw a level preview into video::BackBuffer().  Return true if
      successful. */
    bool DrawLevelPreview (ecl::GC &, const Level &l);

}

namespace enigma
{
    namespace game = enigma_game;
}
#endif
