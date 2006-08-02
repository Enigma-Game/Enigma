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
 
#ifndef SCOREMGR_HH_INCLUDED
#define SCOREMGR_HH_INCLUDED

#include "lev/Proxy.hh"
#include "lev/RatingManager.hh"
#include "lev/Index.hh"

#include <string>

namespace enigma { namespace lev {
    // Constants
    enum { 
        SCORE_MAX = 99*60+59,
        SCORE_UNSOLVED  = -1,
        SCORE_SOLVED   = -2 
    };

    /**
     * A singelton manager for user scores.
     * 
     * There are 2 small paradigm shifts in 0.92 scores and 1.00 scores:<p>
     * There is no longer a separate "finished" flag but levels that have
     * been solved without a score value are marked as "SCORE_SOLVED".<p>
     * Easy mode scores for levels that have no separate easy mode are stored
     * as difficult mode scores. All requests for easy mode scores return the
     * difficult mode scores in this case.
     */
    class ScoreManager {
    public:
        static ScoreManager *instance();
        ~ScoreManager();
        /**
         * Returns true if the level has been solved for the given difficulty
         * in any score version.
         * @arg difficulty  DIFFICULTY_EASY, DIFFICULTY_HARD, DIFFICULTY_ANY
         */
        bool isSolved(lev::Proxy *levelProxy, int difficulty); 
        /**
         * Returns true if the level has only been solved for the given difficulty
         * in an outdated score version.
         * @arg difficulty  DIFFICULTY_EASY, DIFFICULTY_HARD
         */
        bool isOutdated(lev::Proxy *levelProxy, int difficulty);
        /**
         * Returns the best score that the user has reached for the given 
         * difficulty so far or -1 for not yet solved in the current score
         * version.
         * @arg difficulty  DIFFICULTY_EASY, DIFFICULTY_HARD
         */
        int getBestUserScore(lev::Proxy *levelProxy, int difficulty);
        /**
         * 
         */
        void updateUserScore(lev::Proxy *levelProxy, int difficulty, int score);
        /**
         * Returns true if the users best score is equal or better than the
         * official best score for the given difficulty
         * @arg difficulty  DIFFICULTY_EASY, DIFFICULTY_HARD
         */
        bool bestScoreReached(lev::Proxy *levelProxy, int difficulty);
        /**
         * Resets the user score status to unsolved for the given diffculty.
         * If the level has no easy mode the score for the difficult mode
         * will be reset to unsolved.
         */
        void markUnsolved(lev::Proxy *levelProxy, int difficulty);
        /**
         * Resets the score value and marks the level as solved for the given 
         * difficulty. If the level has no easy mode a request will mark the
         * difficult mode score as solved. This action is restriced to 
         * wizards.
         */
        void markSolved(lev::Proxy *levelProxy, int difficulty);
        int countSolved(lev::Index *ind, int difficulty);
        int countBestScore(lev::Index *ind, int difficulty);
    protected:
        ScoreManager();
    private:
        static ScoreManager *theSingleton;
        RatingManager *ratingMgr;
        bool isSolvedLegacy(lev::Proxy *levelProxy, int difficulty);  
        bool isOutdatedLegacy(lev::Proxy *levelProxy, int difficulty);      
        int getBestUserScoreLegacy(lev::Proxy *levelProxy, int difficulty);
        void updateUserScoreLegacy(lev::Proxy *levelProxy, int difficulty, int score);
        void markUnsolvedLegacy(lev::Proxy *levelProxy, int difficulty);
        void markSolvedLegacy(lev::Proxy *levelProxy, int difficulty);
    };
}} // namespace enigma::lev

#endif
