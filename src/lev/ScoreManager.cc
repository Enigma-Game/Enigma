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
 *
 */

#include "lev/ScoreManager.hh"
#include "enigma.hh"
#include "errors.hh"

#include "main.hh"
#include "options.hh"

using namespace std;
using namespace enigma;



namespace enigma { namespace lev {
    ScoreManager *ScoreManager::theSingleton = 0;
    
    ScoreManager* ScoreManager::instance() {
        if (theSingleton == 0) {
            theSingleton = new ScoreManager();
        }
        return theSingleton;
    }

    ScoreManager::ScoreManager() {
        ratingMgr = lev::RatingManager::instance();
    }

    ScoreManager::~ScoreManager() {
    }

    bool ScoreManager::isSolved(lev::Proxy *levelProxy, int difficulty) {
        ecl::Assert <XFrontend> (difficulty >= DIFFICULTY_EASY &&  
                difficulty <= DIFFICULTY_ANY, "ScoreManager::isSolved illegal difficulty");
        return isSolvedLegacy(levelProxy, difficulty);
    }

    bool ScoreManager::isOutdated(lev::Proxy *levelProxy, int difficulty) {
        ecl::Assert <XFrontend> (difficulty >= DIFFICULTY_EASY &&  
                difficulty <= DIFFICULTY_HARD, "ScoreManager::isOutdated illegal difficulty");
        return isOutdatedLegacy(levelProxy, difficulty);
    }

    int ScoreManager::getBestUserScore(lev::Proxy *levelProxy, int difficulty) {
        ecl::Assert <XFrontend> (difficulty >= DIFFICULTY_EASY &&  
                difficulty <= DIFFICULTY_HARD, "ScoreManager::getBestUserScore illegal difficulty");
        return getBestUserScoreLegacy(levelProxy, difficulty);
    }

    void ScoreManager::updateUserScore(lev::Proxy *levelProxy, int difficulty, int score) {
        ecl::Assert <XFrontend> (difficulty >= DIFFICULTY_EASY &&  
                difficulty <= DIFFICULTY_HARD, "ScoreManager::updateUserScore illegal difficulty");
        updateUserScoreLegacy(levelProxy, difficulty, score);
    }
    
    bool ScoreManager::bestScoreReached(lev::Proxy *levelProxy, int difficulty) {
        int bestUserScore = getBestUserScore(levelProxy, difficulty);
        if (ratingMgr == NULL)
            Log << "ratingMgr is NULL\n";
        int bestScore = ratingMgr->getBestScore(levelProxy, difficulty);
        return bestUserScore>0 && (bestScore<0 || bestUserScore <= bestScore);
    }
    
    void ScoreManager::markUnsolved(lev::Proxy *levelProxy, int difficulty) {
        ecl::Assert <XFrontend> (difficulty >= DIFFICULTY_EASY &&  
                difficulty <= DIFFICULTY_ANY, "ScoreManager::markUnsolved illegal difficulty");
        if (!levelProxy->hasEasymode()) {
            difficulty = DIFFICULTY_ANY;
        }
        markUnsolvedLegacy(levelProxy, difficulty);
    }
    
    void ScoreManager::markSolved(lev::Proxy *levelProxy, int difficulty) {
        ecl::Assert <XFrontend> (difficulty >= DIFFICULTY_EASY &&  
                difficulty <= DIFFICULTY_ANY, "ScoreManager::markSolved illegal difficulty");
        if (enigma::WizardMode) {
            if (!levelProxy->hasEasymode()) {
                difficulty = DIFFICULTY_HARD;
            }
            markSolvedLegacy(levelProxy, difficulty);
        }
    }

    int ScoreManager::countSolved(lev::Index *ind, int difficulty) {
        int i;
        int size = ind->size();
        int num = 0;
        for (i=0 ; i < size; i++) {
            if (isSolved(ind->getProxy(i),difficulty))
                num++;
        }
        return num;
    }
    
    int ScoreManager::countBestScore(lev::Index *ind, int difficulty) {
        int i;
        int size = ind->size();
        int num = 0;
        for (i=0 ; i < size; i++) {
            if (bestScoreReached(ind->getProxy(i),difficulty))
                num++;
        }
        return num;
    }  

    // Legacy 0.92 
    bool ScoreManager::isSolvedLegacy(lev::Proxy *levelProxy, int difficulty) {
        options::LevelStatus levelstat;
    
        if (options::GetLevelStatus (levelProxy->getId(), levelstat))
            return ((levelstat.finished & difficulty) ||
                    (!levelProxy->hasEasymode() && levelstat.finished));
        return false;
    }

    bool ScoreManager::isOutdatedLegacy(lev::Proxy *levelProxy, int difficulty) {
        if (isSolvedLegacy(levelProxy, difficulty)) {
            options::LevelStatus levelstat;
            if (options::GetLevelStatus (levelProxy->getId(), levelstat))
                return levelProxy->getScoreVersion() > levelstat.solved_revision;
        }
        return false;
    }
    
    int ScoreManager::getBestUserScoreLegacy(lev::Proxy *levelProxy, int difficulty) {
        options::LevelStatus levelstat;
    
        int best_user_time = -1;
        if (options::GetLevelStatus(levelProxy->getId(), levelstat)) {
            if (levelProxy->hasEasymode()) {
                if ((levelstat.finished & difficulty) != 0) {
                    best_user_time = (difficulty == DIFFICULTY_HARD) // XXX should use difficulty parameter
                        ? levelstat.time_hard 
                        : levelstat.time_easy;
                }
            }
            else {
                // If level is the same in easy and hard mode use the user's
                // best time in either difficulty level.
                if (levelstat.finished) {
                    if (levelstat.time_hard > 0) 
                        best_user_time = levelstat.time_hard;
                    if (levelstat.time_easy > 0 && (levelstat.time_easy < best_user_time || best_user_time<0))
                        best_user_time = levelstat.time_easy;
                }
            }
            if (levelstat.solved_revision < levelProxy->getScoreVersion())
                best_user_time = -1;
        }
        ecl::Assert <XFrontend> (best_user_time >= -1, "ScoreManager::getBestUserScoreLegacy illegal score");
        return best_user_time;
    }

    void ScoreManager::updateUserScoreLegacy(lev::Proxy *levelProxy, int difficulty, int time) {
        int revision = levelProxy->getScoreVersion();
        options::LevelStatus levelstat (-1, -1, difficulty, revision);
    
        if (options::GetLevelStatus(levelProxy->getId(), levelstat)) {
            if (levelstat.finished == 0 || 
                (levelstat.solved_revision >= 1 && levelstat.solved_revision < revision))
            {
                // reset level status if level marked as not finished (the
                // user can do so explicitly in the level menu) or if only
                // an old revision of the level was solved.
                levelstat.time_hard = -1;
                levelstat.time_easy = -1;
                levelstat.finished  = 0;
            }
            levelstat.finished        |= difficulty;
            levelstat.solved_revision  = revision;
        }
    
        bool new_record = false;
        int& time_curr  = (difficulty == enigma::DIFFICULTY_EASY) 
            ? levelstat.time_easy : levelstat.time_hard;
    
        if (time_curr > time || time_curr == -1) {
            time_curr  = time;
            new_record = true;
        }
        options::SetLevelStatus(levelProxy->getId(), levelstat);
    }

    void ScoreManager::markUnsolvedLegacy(lev::Proxy *levelProxy, int difficulty) {
        options::LevelStatus levelstat;
        if (options::GetLevelStatus(levelProxy->getId(), levelstat)) {
            if ((levelstat.finished & difficulty) != 0) {
                levelstat.finished  &= ~difficulty;
                if (difficulty & DIFFICULTY_EASY)
                    levelstat.time_easy = -1;
                if (difficulty & DIFFICULTY_HARD)
                    levelstat.time_hard = -1;
                options::SetLevelStatus(levelProxy->getId(), levelstat);
            }
        }
    }
    
    void ScoreManager::markSolvedLegacy(lev::Proxy *levelProxy, int difficulty) {
        options::LevelStatus levelstat;
        options::GetLevelStatus(levelProxy->getId(), levelstat);
        if (levelstat.solved_revision == levelProxy->getScoreVersion()) { 
            if ((levelstat.finished & difficulty) != difficulty ) {
                levelstat.finished  |= difficulty;
                // Note: we should reset the time_hard/easy too but 0.92 did not do
                // - in 1.00 this problem will not occur!
                if (difficulty & DIFFICULTY_EASY)
                    levelstat.time_easy = -1;
                if (difficulty & DIFFICULTY_HARD)
                    levelstat.time_hard = -1;
                options::SetLevelStatus(levelProxy->getId(), levelstat);
            }
        } else {
            levelstat.solved_revision = levelProxy->getScoreVersion();
            levelstat.finished = difficulty;
            // Note: we should reset the time_hard/easy too but 0.92 did not do
            // - in 1.00 this problem will not occur!
            if (difficulty & DIFFICULTY_EASY)
                levelstat.time_easy = -1;
            if (difficulty & DIFFICULTY_HARD)
                levelstat.time_hard = -1;
            options::SetLevelStatus(levelProxy->getId(), levelstat);
        }
    }

}} // namespace enigma::lev
