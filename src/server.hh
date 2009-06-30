/*
 * Copyright (C) 2003,2004,2005 Daniel Heck
 * Copyright (C) 2007,2008,2009 Ronald Lamprecht
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

/*! @file
  The client->server interface 
*/
#ifndef SERVER_HH_INCLUDED
#define SERVER_HH_INCLUDED

#include "lev/Proxy.hh"
#include "Value.hh"
#include "ecl_math.hh"

#include <stdint.h>

namespace enigma_server 
{
    using namespace enigma;

/* -------------------- Global variables -------------------- */

    /*! Pointer to the current level pack. */
//    extern LevelPack *CurrentLevelPack;

    /*! Index of the current level. */
//    extern unsigned CurrentLevel;

    /*! True if the level is only being loaded for the purpose of
      making a preview image. */
    extern bool CreatingPreview;

/* -------------------- Cheats -------------------- */

    extern bool NoCollisions;

/* -------------------- Per-level settings -------------------- */

    // The number of seconds passed since the level game started.
    extern double LevelTime;
    extern double LastMenuTime;
    extern int MenuCount;
    
    extern int32_t  RandomState;
    extern unsigned SublevelNumber;
    extern std::string SublevelTitle;

    // True: do not reset level when player dies
    extern bool     ConserveLevel;

    extern bool     IsDifficult;
    extern bool     IsLevelRestart;
    extern bool     ProvideExtralifes;
    extern bool     InfiniteReincarnation;  // do not remove extralife on respawn
    extern bool     SurviveFinish;
    extern bool     TwoPlayerGame;

    extern bool     SingleComputerGame;

    // True: allow to control 2nd player (e.g. after first died)
    extern bool     AllowTogglePlayer;

    // True -> show move counter (Sokoban style)
    extern bool     ShowMoves;

    // Behave like Oxyd/Enigma version
    extern GameType GameCompatibility;

    // Detect double world resize attempts
    extern bool     WorldSized;
    
    // Guarantee a fully initialized world, init messages has been sent
    extern bool     WorldInitialized;
    
    // Single oxyds make a level unsolvable, but sometimes they are just fake,... 
    extern bool     AllowSingleOxyds;
    
    // Allow F3
    extern bool     AllowSuicide;
    
    // level compatibility
    extern double   EnigmaCompatibility;
   
    // level compatibility
    extern lev::levelStatusType   LevelStatus;
   
    // Default brittleness of the floor: 0 = stable..1=unstable.
    // Really: probability that a floor tile will crack when an actor
    // enters or leaves it. - used up to 1.01
    extern double   Brittleness;
    // 1.10 crack variables
    extern double   Fragility;
    extern double   CrackSpreading;
    extern ecl::V2  ConstantForce;  // global gravitation
    
    extern int      MaxOxydColor;

    extern double   WaterSinkSpeed;

    extern double   SwampSinkSpeed;
    extern double   RubberViolationStrength;
    
    extern int      GlassesVisibility;
    extern int      ExtralifeGlasses;
    
    extern Value     FollowAction;
    extern bool      FollowGrid;
    extern int       FollowMethod;
    extern Value     FollowThreshold;

/* --------------------  Force multipliers... -------------------- */

    // ...for floor tiles that look sloped
    extern double   SlopeForce;

    // depreceated - vertical factor for space floor tiles - used in oxyd emulation only 
    extern double   FlatForce;

    // ...for friction on certain floor types
    extern double   FrictionFactor;

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

    /** Initialize the server at program start. */
    void Init();

    /** Shutdown the server at program end. */
    void Shutdown();

// TEST
    bool NetworkStart();

    /** Prepare the server for a new game. */
    void InitNewGame();
    void PrepareLua();

    void Tick (double dtime);
    void RestartLevel();
    bool IsRestartingLevel();
    void FinishLevel();

    void SetCompatibility(const char *version); // set compatibility (from lua)
    void SetCompatibility(lev::Proxy *levelProxy); // set compatibility from xml
    
    enigma::Difficulty GetDifficulty();

    // move counter
    void InitMoveCounter();
    int  IncMoveCounter(int increment = 1);
    int  GetMoveCounter();

    void RaiseError (const std::string &msg);

/* -------------------- Client -> Server messages -------------------- */

    void Msg_SetLevelPack (const std::string &name);

    void Msg_LoadLevel (lev::Proxy *levelProxy, bool isPreview);
     
    void Msg_JumpBack();

    /*! After loading the level, the server sends a "LevelLoaded"
      message to all clients.  The game only starts after they have
      answered with a "StartGame" message. */
    void Msg_StartGame();

    void Msg_RestartGame();

    void Msg_Command (const std::string &command);

    void Msg_Pause (bool onoff);
    
    void Msg_Panic (bool onoff);

    void Msg_MouseForce (const ecl::V2 &f);


    void Msg_ActivateItem();
}

namespace enigma
{
    namespace server = enigma_server;
}

#endif
