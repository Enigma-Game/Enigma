/*
 * Copyright (C) 2021 Andreas Lochmann
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
 
#ifndef LEV_SEARCHCOMBINATION_HH_INCLUDED
#define LEV_SEARCHCOMBINATION_HH_INCLUDED

#include "enigma.hh"

#include <string>

namespace enigma { namespace lev {
    enum SCValueKey { SC_INT, SC_DEX, SC_PAT, SC_KNO, SC_SPE, SC_DIF, SC_AVR };
    enum SCValueMinMax { SC_MIN, SC_MAX };

    class Proxy;

    class SearchCombination {
    public:
        // Constructor
        SearchCombination(std::string s = std::string(""));

        // Methods
        void setSearchText(std::string text);
        void setValue(SCValueKey key, SCValueMinMax type, short value);
        short getValue(SCValueKey key, SCValueMinMax type);
        void setOnlyUnsolved(Difficulty diff, bool value);
        bool getOnlyUnsolved(Difficulty diff);
        bool toggleOnlyUnsolved(Difficulty diff);
        void prepareForSearch();
        bool fits(Proxy *p);

    private:
        struct LowerCaseString {
            std::string low;
            LowerCaseString(const std::string& s) : low(s) {
                for (std::string::iterator i = low.begin(); i != low.end(); ++i)
                    *i = tolower(*i);
            }
            bool containedBy(LowerCaseString other) const {
                return other.low.find(low) != std::string::npos;
            }
        };

        // Search parameters
        short int_min;  // intelligence
        short int_max;
        short dex_min;  // dexterity
        short dex_max;
        short pat_min;  // patience
        short pat_max;
        short kno_min;  // knowledge
        short kno_max;
        short spe_min;  // speed
        short spe_max;
        short dif_min;  // overall difficulty
        short dif_max;
        short avr_min;  // average rating * 10
        short avr_max;
        bool checkRatings;
        bool onlyUnsolvedEasy;
        bool onlyUnsolvedHard;
        bool onlyMainPacks;
        LowerCaseString searchText;
    };
}} // namespace enigma::lev
#endif

