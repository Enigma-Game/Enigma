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

#include "lev/Index.hh"
#include "lev/VolatileIndex.hh"
#include "errors.hh"
#include "main.hh"
#include "options.hh"
#include "oxyd.hh"
#include "PreferenceManager.hh"
#include "lev/ScoreManager.hh"
#include "lev/RatingManager.hh"

#include "lev/AdapterIndex.hh"
#include "levels.hh"


namespace enigma { namespace lev {

    std::map<std::string, Index *> Index::indices;
    std::map<std::string, std::vector<Index *> *> Index::indexGroups;
    Index * Index::currentIndex = NULL;
    
    void Index::registerIndex(Index *anIndex) {
        if (anIndex == NULL)
            return;
        
        // check for uniqueness of index name
        
            
        indices.insert(std::make_pair(anIndex->getName(), anIndex));
        
        std::string groupName = "Default";  // used if no group is supplied
        std::vector<Index *> * group;
        // check preferences for assigned group
        
        // if no prefs ask for index default group
        
        std::map<std::string, std::vector<Index *> *>::iterator i = indexGroups.find(groupName);
        if (i != indexGroups.end()) {
            group = i->second;
        } else {
            // make the group
            group = new std::vector<Index *>;
            indexGroups.insert(std::make_pair(groupName, group));
        }
        
        // insert according to user prefs or index defaults
        
        // else append
        group->push_back(anIndex);
        anIndex->indexGroup = groupName;
        
    }
     
    Index * Index::findIndex(std::string anIndexName) {
        // stip of trailing and leading spaces
        std::string name = anIndexName.substr(0 , anIndexName.find_last_not_of(" ")+1);
        name = name.substr(anIndexName.find_first_not_of(" "));
        
        std::map<std::string, Index *>::iterator i = indices.find(name);
        if (i != indices.end())
            return i->second;
        else 
            return NULL;
    }

    std::vector<Index *> * Index::getGroup(Index * anIndex) {
        std::map<std::string, std::vector<Index *> *>::iterator i = indexGroups.find(anIndex->getGroupName());
        if (i != indexGroups.end()) {
            return i->second;
        } else {
            return NULL;
        }
    }
    
    Index * Index::getCurrentIndex() {
        if (currentIndex == NULL) {
            // first look for user preference
            if (setCurrentIndex("CurrentIndex"))
                currentIndex->setCurrentPosition(app.prefs->getInt("CurrentPosition"));
            
            // fallback to "Tutorial" pack
            else if (setCurrentIndex("Tutorial"));
                        
            // fallback to any pack
            else if (!indices.empty()) {
                setCurrentIndex(indices.begin()->second->getName());
            }
            // add empty pack
            else {
                std::vector<std::string> emptyList;
                registerIndex(new lev::VolatileIndex("Empty Index",
                        "Default", emptyList));
                setCurrentIndex("Empty Index");
            }
                
            oxyd::ChangeSoundset(app.prefs->getInt("SoundSet"), currentIndex->get_default_SoundSet());
        }
        return currentIndex;
    }

    bool Index::setCurrentIndex(std::string anIndexName) {
        Index * newIndex = findIndex(anIndexName);
        if (newIndex != NULL) {
            if (newIndex != currentIndex) {
                // set soundset!!
                app.prefs->setPref("CurrentIndex", anIndexName);
            }
            currentIndex = newIndex;
            return true;
        }
        return false;
    }
    
    Index * Index::nextGroupIndex() {
        std::vector<Index *> * curGroup = getGroup(currentIndex);
        ASSERT(curGroup != NULL, XFrontend, "");
        
        for (int i = 0; i < curGroup->size() - 1; i++) {
            if ((*curGroup)[i] == currentIndex)
                return (*curGroup)[i+1];
        }
        return currentIndex;
    }
    
    Index * Index::previousGroupIndex() {
        std::vector<Index *> * curGroup = getGroup(currentIndex);
        ASSERT(curGroup != NULL, XFrontend, "");

        for (int i = 1; i < curGroup->size(); i++) {
            if ((*curGroup)[i] == currentIndex)
                return (*curGroup)[i-1];
        }
        return currentIndex;
    }

    Proxy * Index::getCurrentProxy() {
        return getCurrentIndex()->getCurrent();
    }
    
    std::vector<Index *> * Index::getGroup(std::string groupName) {
        std::map<std::string, std::vector<Index *> *>::iterator i = indexGroups.find(groupName);
        if (i != indexGroups.end()) 
            return i->second;
        else 
            return NULL;
    }
    
    Index::Index() : currentPosition (0), screenFirstPosition (0),
            indexName ("Unnamed Pack") {
    }
    
    Index::~Index() {}
    
    std::string Index::getName() {
        return indexName;
    }

    std::string Index::getGroupName() {
        return indexGroup;
    }
    
    int Index::getCurrentPosition() {
        return currentPosition;
    }
    
    int Index::getCurrentLevel() {
        return currentPosition + 1;
    }
    
    Proxy * Index::getCurrent() {
        return getProxy(currentPosition);
    }
    
    void Index::setCurrentPosition(int newPos) {
        ecl::Assert <XFrontend> (newPos == 0 || (newPos > 0 && newPos < size()), 
                "Index::setCurrentPosition illegal position");

        //
        currentPosition = newPos;
        if (currentIndex == this)
            app.prefs->setPref("CurrentPosition",newPos);
    }

    int Index::getScreenFirstPosition() {
        return screenFirstPosition;
    }
    
    void Index::setScreenFirstPosition(int iFirstPos) {
        screenFirstPosition = iFirstPos;
    }
    
    bool Index::mayPlayLevel(int levelNumber) {
        return true;
    }
    
    Proxy * Index::getProxy(int pos) {
        if (pos >= 0 && pos < proxies.size())
            return proxies[pos];
        else
            return NULL;
    }
    
    bool Index::advanceLevel(LevelAdvanceMode advMode) {
        bool skip_solved  = options::GetBool("SkipSolvedLevels");
        bool take_non_par = options::GetBool("TimeHunting");
        NextLevelMode nextMode = NEXT_LEVEL_STRICTLY;
    
        switch (advMode) {
        case ADVANCE_NEXT_MODE:
            if (skip_solved) 
                nextMode = NEXT_LEVEL_UNSOLVED;
            if (take_non_par)
                nextMode = NEXT_LEVEL_NOT_BEST;
            break;
        case ADVANCE_STRICTLY:
            nextMode = NEXT_LEVEL_STRICTLY;
            break;
        case ADVANCE_UNSOLVED:
            nextMode = NEXT_LEVEL_UNSOLVED;
            break;
        };

        bool found = false;
        const int max = size();
        int newPos = currentPosition;
        lev::ScoreManager *scm = lev::ScoreManager::instance();
        lev::RatingManager *ratingMgr = lev::RatingManager::instance();
        int    difficulty     = options::GetDifficulty();
        
        while (newPos < max - 1 && !found) {
            ++newPos;
    
            if (nextMode == NEXT_LEVEL_UNSOLVED || nextMode == NEXT_LEVEL_NOT_BEST) {
                bool solved = scm->isSolved(proxies[newPos], difficulty);
                if (!solved) // always play unsolved levels
                    found = true;
                else { // solved levels
                    if (nextMode == NEXT_LEVEL_NOT_BEST) {
                        int  par_time       = ratingMgr->getBestScore(proxies[newPos], difficulty);
                        int  best_user_time = scm->getBestUserScore(proxies[newPos], difficulty);
                        bool need_par       = best_user_time<0 || (par_time>0 && best_user_time>par_time);
    
                        if (need_par)
                            found = true;
                    }
               }
            }
            else
                found = true;
        }
        if (!found)
            newPos = 0; // ?
            
        currentPosition = newPos;
        return found;        
    }
    
    int Index::size() const { 
        return proxies.size();
    }

    void Index::appendProxy(Proxy * newLevel) {
//        proxies.push_back(aProxy);
    }

    void Index::clear() {
//        proxies.clear();
    }

    /* ---------- LevelPack interface ---------- */
    

    /*! Return the default SoundSet (see options::SoundSet for meaning) */
    int Index::get_default_SoundSet() const { 
        return 0;
    }

    /*! Returns true if it's a twoplayer levelpack, but has no
      it-yinyang (needed to add it-yinyang to inventory if
      oxyd-linkgame is played as single-player) */
    bool Index::needs_twoplayers() const { 
        return false;
    }

// private
    void AddLevelPack (const char *init_file, const char *name) {
        Log << "Index AddLevelPack " << init_file << "\n";
        enigma::levels::LevelPack *lp = enigma::levels::AddLevelPack2(init_file, name);
        if (lp != NULL) 
            Index::registerIndex(new AdapterIndex(lp));
    }

    void AddZippedLevelPack (const char *zipfile) {
        Log << "Index AddZippedLevelPack " << zipfile << "\n";
        enigma::levels::LevelPack *lp = enigma::levels::AddZippedLevelPack2(zipfile);
        if (lp != NULL) 
            Index::registerIndex(new AdapterIndex(lp));
    }
}} // namespace enigma::lev
