// ======================================================================== //
//                                                                          //
//   File      : EnigmaNames.h                                              //
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
#ifndef ENIGMANAMES_H
#define ENIGMANAMES_H

namespace oxyd { // "faked" namespace - is not part of oxyd namespace in enigma itself
    extern const char *oxyd1_item_map[];
    extern const char *oxyd1_floor_map[];
    extern const char *oxyd1_stone_map[];

    extern const char *peroxyd_item_map[];
    extern const char *peroxyd_floor_map[];
    extern const char *peroxyd_stone_map[];

    extern const char *oxydmag_item_map[];
    extern const char *oxydmag_floor_map[];
    extern const char *oxydmag_stone_map[];

    extern const char *oxydextra_item_map[];
    extern const char *oxydextra_floor_map[];
    extern const char *oxydextra_stone_map[];
};

namespace enigma_names {
    class Mapping {
        const char **item_map;
        const char **floor_map;
        const char **stone_map;

    public:
        Mapping(OxydLib::OxydVersion version);

        const char* floorName(int idx) const { return floor_map[idx]; }
        const char* itemName(int idx) const { return item_map[idx]; }
        const char* stoneName(int idx) const { return stone_map[idx]; }
    };
};

#else
#error EnigmaNames.h included twice
#endif // ENIGMANAMES_H

