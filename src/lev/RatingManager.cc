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

#include "lev/RatingManager.hh"

#include "main.hh"

namespace enigma { namespace lev {
    Rating::Rating() 
    {
        bestScoreEasy   = -1;          //DEFAULT_TIME;
        bestScoreDifficult = -1;       //DEFAULT_TIME;
        intelligence    = 0;
        dexterity       = 0;
        patience        = 0;
        knowledge       = 0;
        speed           = 0;
    
    }

    RatingManager *RatingManager::theSingleton = 0;
    
    RatingManager* RatingManager::instance() {
        if (theSingleton == 0) {
            theSingleton = new RatingManager();
        }
        return theSingleton;
    }

    RatingManager::RatingManager() {
        ;
    }
    
    RatingManager::~RatingManager() {
    }
    
    void RatingManager::registerRating(std::string levelId, int levelScoreVersion,
                int intelligence, int dexterity, int patience,
                int knowledge, int speed, int bestScoreEasy, 
                std::string bestScoreEasyHolder, int bestScoreDifficult,
                std::string bestScoreDifficultHolder) {
        char txt[5];
        snprintf(txt, sizeof(txt), "%d", levelScoreVersion);        
        std::string cacheKey = levelId + txt;
        std::map<std::string, Rating *>::iterator i = cache.find(cacheKey);
        if (i != cache.end()) {
//            Log << "Rating cache hit for " << levelId <<"\n";
            return;
        }
        Rating *theRating = new Rating();
        theRating->intelligence = intelligence;
        theRating->dexterity = dexterity;
        theRating->patience = patience;
        theRating->knowledge = knowledge;
        theRating->speed = speed;
        theRating->bestScoreEasy = bestScoreEasy;
        theRating->bestScoreEasyHolder = bestScoreEasyHolder;
        theRating->bestScoreDifficult = bestScoreDifficult;
        theRating->bestScoreDifficultHolder = bestScoreDifficultHolder;
        cache.insert(std::make_pair(cacheKey, theRating));
        return;
    }
    
    int RatingManager::getIntelligence(Proxy *levelProxy) {
        Rating * theRating = findRating(levelProxy);
        if (theRating != NULL) {
            return theRating->intelligence;
        }
       
        return 0;
    }

    int RatingManager::getDexterity(Proxy *levelProxy) {
        Rating * theRating = findRating(levelProxy);
        if (theRating != NULL) {
            return theRating->dexterity;
        }
       
        return 0;
    }
    
    int RatingManager::getPatience(Proxy *levelProxy) {
        Rating * theRating = findRating(levelProxy);
        if (theRating != NULL) {
            return theRating->patience;
        }
       
        return 0;
    }
    
    int RatingManager::getKnowledge(Proxy *levelProxy) {
        Rating * theRating = findRating(levelProxy);
        if (theRating != NULL) {
            return theRating->knowledge;
        }
       
        return 0;
    }
    
    int RatingManager::getSpeed(Proxy *levelProxy) {
        Rating * theRating = findRating(levelProxy);
        if (theRating != NULL) {
            return theRating->speed;
        }
       
        return 0;
    }

    int RatingManager::getBestScoreEasy(Proxy *levelProxy) {
        Rating * theRating = findRating(levelProxy);
        if (theRating != NULL) {
            return theRating->bestScoreEasy;
        }
        return -1;
    }

    std::string RatingManager::getBestScoreEasyHolder(Proxy *levelProxy) {
        Rating * theRating = findRating(levelProxy);
        if (theRating != NULL) {
            return theRating->bestScoreEasyHolder;
        }
        return "";
    }

    int RatingManager::getBestScoreDifficult(Proxy *levelProxy) {
        Rating * theRating = findRating(levelProxy);
        if (theRating != NULL) {
            return theRating->bestScoreDifficult;
        }
        return -1;
    }

    std::string RatingManager::getBestScoreDifficultHolder(Proxy *levelProxy) {
        Rating * theRating = findRating(levelProxy);
        if (theRating != NULL) {
            return theRating->bestScoreDifficultHolder;
        }
        return "";
    }
    Rating * RatingManager::findRating(Proxy * levelProxy) {
        char txt[5];
        snprintf(txt, sizeof(txt), "%d", levelProxy->getScoreVersion());        
        std::string cacheKey = levelProxy->getId() + txt;
        std::map<std::string, Rating *>::iterator i = cache.find(cacheKey);
        if (i != cache.end()) {
            Rating * theRating = i->second;
//            Log << "Rating cache hit for " << levelProxy->getId() <<"\n";
            return theRating;
        } else {
            return NULL;
        }
    }

}} // namespace enigma::lev
