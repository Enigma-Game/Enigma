/*
 * Copyright (C) 2006 Ronald Lamprecht
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
 
#ifndef RATINGMGR_HH_INCLUDED
#define RATINGMGR_HH_INCLUDED

#include "lev/Proxy.hh"

#include <map>
#include <string>

namespace enigma { namespace lev {
    struct Rating
    {
        // Constructor
        Rating();

        // Constants
        enum { DEFAULT_TIME = 99*60+59 };

        // Variables.
        int           bestScoreEasy;         // Best time in seconds or Minimum moves to solve level(for easy mode) 
        int           bestScoreDifficult;    // Best time in seconds or Minimum moves to solve level(for normal mode)
        std::string   bestScoreEasyHolder;       // player name(s) for 'best_time_easy'
        std::string   bestScoreDifficultHolder;  // same for 'best_time_normal'
        int      intelligence;
        int      dexterity;
        int      patience;
        int      knowledge;
        int      speed;
     };

    /**
     * A singelton manager for public ratings and score statistics
     */
    class RatingManager {
    public:
        static RatingManager *instance();
        ~RatingManager();
        void registerRating(std::string levelId, int levelScoreVersion,
                int intelligence, int dexterity, int patience,
                int knowledge, int speed, int bestScoreEasy, 
                std::string bestScoreEasyHolder, int bestScoreDifficult,
                std::string bestScoreDifficultHolder);
        int getIntelligence(Proxy *levelProxy);
        int getDexterity(Proxy *levelProxy);
        int getPatience(Proxy *levelProxy);
        int getKnowledge(Proxy *levelProxy);
        int getSpeed(Proxy *levelProxy);
        int getBestScoreEasy(Proxy *levelProxy);
        std::string getBestScoreEasyHolder(Proxy *levelProxy);
        int getBestScoreDifficult(Proxy *levelProxy);
        std::string getBestScoreDifficultHolder(Proxy *levelProxy);
    protected:
        RatingManager();
    private:
        static RatingManager *theSingleton;
        std::map<std::string, Rating *> cache;
        Rating * findRating(Proxy * levelProxy);
    };
}} // namespace enigma::lev

#endif
