/*
 * Copyright (C) 2006,2007,2008,2009,2010 Ronald Lamprecht
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

#include "PropertyManager.hh"
#include "lev/Proxy.hh"
#include "lev/RatingManager.hh"
#include "lev/Index.hh"
#include "main.hh"

#include <string>
#include <map>
#include <xercesc/dom/DOMElement.hpp>

namespace enigma { namespace lev {
    // Constants
    enum { 
        SCORE_MAX1 = 99*60+59,
        SCORE_MAX2 = 32767,   // 9:06'07" (xs:short)
        SCORE_UNSOLVED  = -1,
        SCORE_SOLVED   = -2
    };

    /**
     * A singleton manager for user scores.
     * 
     * There are 2 small paradigm shifts in 0.92 scores and 1.00 scores:<p>
     * There is no longer a separate "finished" flag but levels that have
     * been solved without a score value are marked as "SCORE_SOLVED".<p>
     * Easy mode scores for levels that have no separate easy mode are stored
     * as difficult mode scores. All requests for easy mode scores return the
     * difficult mode scores in this case.
     */
    class ScoreManager : public PropertyManager{
    public:
        static ScoreManager *instance();
        ~ScoreManager();
        void markModified();
        virtual bool save();
        void shutdown();

        /**
         * Returns true if the level has been solved for the given difficulty
         * in any score version.
         * @arg difficulty  DIFFICULTY_EASY, DIFFICULTY_HARD, DIFFICULTY_ANY
         */
        bool isSolved(lev::Proxy *levelProxy, int difficulty);

        /**
         * Returns true when the level has only been solved for the given difficulty
         * in an obsolete level score version.
         * @arg difficulty  DIFFICULTY_EASY, DIFFICULTY_HARD
         */
        bool isObsolete(lev::Proxy *levelProxy, int difficulty);

        /**
         * Reports whether the score played for this level is out dated.
         * The score is out dated, when the level has not been solved with
         * an Enigma release, that is assured to be trusted by the upcoming
         * next release. In other words the user should replay this level
         * to avoid this level no longer being counted solved with the next
         * Enigma release.
         * @arg difficulty  DIFFICULTY_EASY, DIFFICULTY_HARD
         */
        bool isOutdated(lev::Proxy *levelProxy, int difficulty);

        /**
         * Reports the best trusted score that the user played for the given
         * level in its score release.
         */
        int getBestUserScore(lev::Proxy *levelProxy, int difficulty);

        /**
         * Update the saved score of the given level with a new value. The score may
         * either be stored as best ever score or as best score with recent Enigma version
         * depending on prior values.
         */
        void updateUserScore(lev::Proxy *levelProxy, int difficulty, int score,
                double enigmaRelease = app.getEnigmaVersion());

        /**
         * Returns true if the users best score is equal or better than the
         * official best score for the given difficulty
         * @arg difficulty  DIFFICULTY_EASY, DIFFICULTY_HARD
         */
        bool bestScoreReached(lev::Proxy *levelProxy, int difficulty);
        bool parScoreReached(lev::Proxy *levelProxy, int difficulty);
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
        int countParScore(lev::Index *ind, int difficulty);
        double calcHCP(lev::Index *ind, int difficulty);
        void setRating(lev::Proxy *levelProxy, int rating);
        int getRating(lev::Proxy *levelProxy);
        bool isRatingInherited(lev::Proxy *levelProxy);
    protected:
        ScoreManager();
    private:
        static ScoreManager *theSingleton;
        static unsigned ctab[256];
        static unsigned pol;
        RatingManager *ratingMgr;
        XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *levelsElem;
        XERCES_CPP_NAMESPACE_QUALIFIER DOMNodeList * levelList;
        std::map<std::string, XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *> allLevelScores; // all scoreversions for each level
        std::map<std::string, XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *> curLevelScores; // most current scoreversion for each level
        std::string userId;
        bool hasValidUserId;
        bool didUpgrade;
        bool isModified;
        
        void finishUserId(unsigned id3);
        std::string sec(std::string target);
        XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * getLevel(lev::Proxy *levelProxy);
        XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * getCreateLevel(lev::Proxy *levelProxy);
        
        unsigned idFromLegacyScore();
        std::string upgradeSum();
        bool upgradeLegacy();
    };
}} // namespace enigma::lev

#endif
