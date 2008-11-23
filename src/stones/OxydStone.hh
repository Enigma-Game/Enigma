/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007,2008 Ronald Lamprecht
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
    class OxydStone : public Stone {
    public:
        enum RuleType {RULE_SINGLE_MIN, RULE_SINGLE_MAX, RULE_PAIR_MIN, RULE_PAIR_MAX};
        enum Color {BLUE = 0, RED, GREEN, YELLOW, CYAN, PURPLE, WHITE, BLACK, 
            GREY, ORANGE, PINE, BROWN,
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
        
        OxydStone(std::string flavor = "b");
        
        // Object interface
        virtual OxydStone * clone();
        virtual void dispose();
        virtual Value message(const Message &m);
        virtual void setAttr(const string& key, const Value &val);

        // StateObject interface
        virtual int maxState() const;
        virtual void toggleState();
        virtual int externalState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void on_removal (GridPos p);
        virtual void init_model();
        virtual void lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs);

        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
        virtual const char *collision_sound() { return "stone"; }
        virtual bool is_removable() const;
        
        // ModelCallback interface  - Animation callback
        virtual void animcb();
    
    private:
        enum iState { CLOSED, OPEN_PAIR, OPENING, CLOSING, OPEN_SINGLE };
        
        enum ObjectPrivatFlagsBits {
            OBJBIT_CLOSED   =  1<<24,  ///< force a closed state on grid set
            OBJBIT_OPENPAIR =  1<<25   ///< force an open state on grid set
        };

        typedef std::vector<OxydStone *> InstanceVector;
        
        typedef struct {
            unsigned short ruleId;
            unsigned short groupId1;
            unsigned short groupId2;
        } Rule;
        
        /**
         * The data frame of the fair shuffle algorithm that describe the remaining limitations
         * and degrees of freedom for the distribution of the remaining oxyds. For every oxyd
         * pair and every pseudo oxyd a new frame is generated which lists the oxyds and the
         * remaining constraints.
         */
        typedef struct {
            uint32_t freeOxydsMask;              ///< a 1 for every oxyd that needs to be distributed
            unsigned short freePseudoCount;      ///< number of pseudo oxyds that need to be distributed
            unsigned short freePairsCount;       ///< number of oxyd pairs that need to be distributed
            std::vector<unsigned short> oxydsCandidatesCount; ///< number of possible pair partners per oxyd
            std::vector<uint32_t> oxydsCandidatesMask;        ///< mask of possible pair partners per oxyd
            std::vector<short> rulesLimit;       ///< remaining number of pairs to reach limit per rule.
                                                 ///< -1 means rule is fulfilled and needs no further checks
            uint32_t selOxyd1Mask;               ///< selected first oxyd for this frame coded as bitmask, 
                                                 ///< 0 = no selection
            uint32_t selOxyd2Mask;               ///< selected second oxyd for this frame coded as bitmask, 
                                                 ///< 0 = no selection
            unsigned short openedOxydIndex;      ///< levelOxyds index for next oxyd that needs to be checked
                                                 ///< for being a standard colored opened pair oxyd
            unsigned short fixedcolorOxydIndex;  ///< levelOxyds index for next oxyd that needs to be checked
                                                 ///< for being a part of a unopend pair with a fixed color 
            bool isColored;                      ///< flag set and used by the final coloring to mark frames
                                                 ///< with already colored oxyds
        } ShuffleFrame;

        static InstanceVector levelOxyds;
        static bool isInit;
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
        
        static bool not_open(OxydStone *a) {
            return !(a->state == OPEN_PAIR || (int)a->getAttr("oxydcolor") < AUTO);
        }


        // Private methods
        void tryOpen();
        void closeAllStandardOxyds();
        void set_iState(iState newState);
    };
} // namespace enigma

#endif
