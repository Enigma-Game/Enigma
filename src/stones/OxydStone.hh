/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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
 *
 */
#ifndef OXYDSTONE_HH_INCLUDED
#define OXYDSTONE_HH_INCLUDED

#include "stones.hh"
#include "laser.hh"
#include <stdint.h>

/* -------------------- Oxyd stone -------------------- */

/** \page st-oxyd Oxyd Stone

Oxyd stones are characterized by two attributes: Their flavor and
their color.  The flavor only affects the visual representation of
the stone; it can be either 'a' (opening like a flower) or 'b'
(displaying a fade-in animation).  The color attribute determines
the shape on the oxyd stone.

\b Note: You should usually not to create Oxyd stones manually
with \c set_stone(). Use the predefined \c oxyd() function instead.

\subsection oxyda Attributes

- \b flavor      "a", "b", "c", or "d"
- \b color       number between 0 and 7

\subsection oxydm Messages

- \b closeall    close all oxyd stones
- \b shuffle     interchange the colors of the oxyd stones in the current landscape
- \b trigger     open the stone

<table><tr>
<td>\image html st-oxyda.png "flavor A"
<td>\image html st-oxydb.png "flavor B"
<td>\image html st-oxydc.png "flavor C"
<td>\image html st-oxydd.png "flavor D"
</table>
*/

namespace enigma {

    /**
     * 
     */
    class OxydStone : public PhotoStone {
    public:
        enum RuleType {RULE_SINGLE_MIN, RULE_SINGLE_MAX, RULE_PAIR_MIN, RULE_PAIR_MAX};
        enum Color {BLUE = 0, RED, GREEN, YELLOW, CYAN, PURPLE, WHITE, BLACK, 
            COLORCOUNT,
            AUTO = -1,
            FAKE = -2,
            FART = -3,
            BOLD = -4,
            MIN  = -4
        };
        enum LogType {NOTHING, SOLUTION, COUNT, ALL};
            
        static unsigned short numColorsAvailable();
        static void addShuffleRule(RuleType type, unsigned short limit, Value group1, Value group2 = Value());
        static void shuffleColors(LogType logFlag = NOTHING);
        
        OxydStone();
        
        // Object interface
        virtual OxydStone * clone();
        virtual void dispose();
        virtual Value message(const string &m, const Value &);

        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
        virtual const char *collision_sound() { return "stone"; }
        virtual bool is_removable() const;
        
        // PhotoStone interface
        virtual void notify_laseron() { maybe_open_stone(); }
        virtual void notify_laseroff() {}

        // ModelCallback interface  - Animation callback
        virtual void animcb();
    
    protected:
        // GridObject interface
        void on_creation (GridPos p);
        void on_removal (GridPos p);
            
    private:
        enum State { CLOSED, OPEN, OPENING, CLOSING, BLINKING };
        typedef std::vector<OxydStone *> InstanceVector;
        typedef struct {
            unsigned short ruleId;
            unsigned short groupId1;
            unsigned short groupId2;
        } Rule;
        typedef struct {
                uint32_t freeOxydsMask;
                unsigned short freePseudoCount;
                unsigned short freePairsCount;
                std::vector<unsigned short> oxydsCandidatesCount;
                std::vector<uint32_t> oxydsCandidatesMask;
                std::vector<short> rulesLimit;  // -1 means rule is fulfilled
                uint32_t selOxyd1Mask;
                uint32_t selOxyd2Mask;
                unsigned short openedOxydIndex;
                bool isColored;
        } ShuffleFrame;

        static InstanceVector levelOxyds;
        static std::vector<unsigned short> colorsUsageCount;
        static unsigned short shuffledFakeCount;
        static unsigned short shuffledFartCount;
        static unsigned short shuffledBoldCount;
        static bool oddUnshuffledColor;
        
        static std::vector<unsigned short> randomOxydIds;  // random sequence of oxyds
        static std::vector<uint32_t> groupsMembers;
        static std::vector<uint32_t> groupsSharedMembers;
        static std::list<Rule> singleRulesMin;
        static std::list<Rule> singleRulesMax;
        static std::list<Rule> pairRulesMin;
        static std::list<Rule> pairRulesMax;
        static std::list<ShuffleFrame> shuffleStack;
        static unsigned int logBadFrameCount;
        
        
        static void initColors();
        static void simpleShuffleColors();
        static void fairShuffleColors(LogType logFlag = NOTHING);
        static int  evaluateTopFrame(LogType logFlag = NOTHING);
        static void colorShuffleDistribtion(LogType logFlag = NOTHING);
        static void invalidateShuffle();
        static unsigned short groupId(Value v);
        static unsigned short countOxyds(uint32_t members);
        static uint32_t randomMember(uint32_t members);
        static unsigned short oxydId(uint32_t mask);
        
        static void log_shuffle_basis();
        static void log_shuffle_stack();
        
        static bool blinking(OxydStone *a) {
            return (a->animState == BLINKING);
        }
        static bool blinking_or_opening(OxydStone *a) {
            return (a->animState == BLINKING || a->animState == OPENING);
        }
        static bool not_open(OxydStone *a) {
            return !(a->animState == OPEN || a->animState == OPENING);
        }

        State animState;

        // Private methods
        void maybe_open_stone();
        void change_state(State newstate);
    };
} // namespace enigma

#endif
