// ======================================================================== //
//                                                                          //
//   File      : EnigmaNames.cpp                                            //
//   Purpose   : Make enigma item names available                           //
//   Time-stamp: <Fri Nov/19/2004 14:25 MET amgine@reallysoft.de>           //
//                                                                          //
//   (C) November 2004 by Ralf Westram <amgine@rallysoft.de>                //
//                                                                          //
//   Permission to use, copy, modify, distribute and sell this software     //
//   and its documentation for any purpose is hereby granted without fee,   //
//   provided that the above copyright notice appear in all copies and      //
//   that both that copyright notice and this permission notice appear      //
//   in supporting documentation.                                           //
//                                                                          //
//   Ralf Westram makes no representations about the suitability of this    //
//   software for any purpose.  It is provided "as is" without express or   //
//   implied warranty.                                                      //
//                                                                          //
// ======================================================================== //
#include "EnigmaNames.h"
#include "OxydVersion.h"

#define PLAIN_SPEC_ONLY
#define UNUSED "<unused>"

#include "../../src/ox_oxyd1.cc"
#include "../../src/ox_extra.cc"
#include "../../src/ox_magnum.cc"
#include "../../src/ox_peroxyd.cc"

using namespace enigma_names;
using namespace oxyd;
using namespace OxydLib;

// ____________________________________________________________
// start of implementation of class Mapping:

Mapping::Mapping(OxydLib::OxydVersion version)
{
    switch (version) {
    case OxydVersion_Invalid: printf("Cannot generate mapping for OxydVersion_Invalid\n"); exit(1);
    case OxydVersion_Count: printf("Cannot generate mapping for OxydVersion_Count\n"); exit(1);

    case OxydVersion_Oxyd1:
        floor_map = oxyd::oxyd1_floor_map;
        stone_map = oxyd::oxyd1_stone_map;
        item_map  = oxyd::oxyd1_item_map;
        break;

    case OxydVersion_OxydExtra:
        floor_map = oxyd::oxydextra_floor_map;
        stone_map = oxyd::oxydextra_stone_map;
        item_map  = oxyd::oxydextra_item_map;
        break;

    case OxydVersion_OxydMagnum:
    case OxydVersion_OxydMagnumGold:
        floor_map = oxyd::oxydmag_floor_map;
        stone_map = oxyd::oxydmag_stone_map;
        item_map  = oxyd::oxydmag_item_map;
        break;

    case OxydVersion_PerOxyd:
        floor_map = oxyd::peroxyd_floor_map;
        stone_map = oxyd::peroxyd_stone_map;
        item_map  = oxyd::peroxyd_item_map;
        break;
    }
}

// -end- of implementation of class Mapping.

