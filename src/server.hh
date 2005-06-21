/*
 * Copyright (C) 2003,2004,2005 Daniel Heck
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
 * $Id: server.hh,v 1.4 2004/03/15 20:15:34 dheck Exp $
 */

/*! @file
  The client->server interface 
*/
#ifndef SERVER_HH_INCLUDED
#define SERVER_HH_INCLUDED

#include "levels.hh"

namespace enigma_server 
{
    using namespace enigma;
    using levels::LevelPack;

/* -------------------- Global variables -------------------- */

    /*! Pointer to the current level pack. */
    extern LevelPack *CurrentLevelPack;

    /*! Index of the current level. */
    extern unsigned CurrentLevel;

    /*! True if the level is only being loaded for the purpose of
      making a preview image. */
    extern bool CreatingPreview;


/* -------------------- Cheats -------------------- */

    extern bool NoCollisions;

/* -------------------- Per-level settings -------------------- */

    // True: do not reset level when player dies
    extern bool     ConserveLevel;

    extern bool     TwoPlayerGame;

    extern bool     SingleComputerGame;

    // True: allow to control 2nd player (e.g. after first died)
    extern bool     AllowTogglePlayer;

    // True -> show move counter (Sokoban style)
    extern bool     ShowMoves;

    // Behave like Oxyd/Enigma version
    extern GameType GameCompatibility;

    // Default brittleness of the floor: 0 = stable..1=unstable.
    // Really: probability that a floor tile will crack when an actor
    // enters or leaves it.
    extern double   Brittleness;

    extern double   WaterSinkSpeed;

    extern double   SwampSinkSpeed;

/* --------------------  Force multipliers... -------------------- */

    // ...for floor tiles that look sloped
    extern double   SlopeForce;

    // ...for floor tiles that DON'T look sloped
    extern double   FlatForce;

    // ...for friction on certain floor types
    extern double   FrictionFactor;

    // ...for friction on ice
    extern double   IceFriction;

    // ...for electrostatic forces between actors (default: 15)
    extern double   ElectricForce;

    // ...for the bumber stones (st-actorimpulse*)
    extern double   BumperForce;

    // ...for magnets
    extern double   MagnetForce;
    extern double   MagnetRange;

    // ...for wormholes
    extern double   WormholeForce;
    extern double   WormholeRange;

    // ...for holes
    extern double   HoleForce;

/* -------------------- Functions -------------------- */

    void Init();
    void Shutdown();

    void Tick (double dtime);
    void GameReset();
    void RestartLevel();
    void FinishLevel();

    void SetCompatibility(const char *version); // set compatibility (from lua)

    enigma::Difficulty GetDifficulty();

    // move counter
    void InitMoveCounter();
    int  IncMoveCounter(int increment = 1);
    int  GetMoveCounter();

    void RaiseError (const std::string &msg);

/* -------------------- Client -> Server messages -------------------- */

    void Msg_SetLevelPack (const std::string &name);

    void Msg_LoadLevel(unsigned levelindex);

    /*! After loading the level, the server sends a "LevelLoaded"
      message to all clients.  The game only starts after they have
      answered with a "StartGame" message. */
    void Msg_StartGame();

    void Msg_RestartGame();

    void Msg_Command (const std::string &command);

    void Msg_Pause (bool onoff);
    
    void Msg_Panic (bool onoff);

    void Msg_MouseForce (const px::V2 &f);


    void Msg_ActivateItem();
}

namespace enigma
{
    namespace server = enigma_server;
}

#endif
