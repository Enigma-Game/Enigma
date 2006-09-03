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
 
#ifndef LEV_INDEX_HH_INCLUDED
#define LEV_INDEX_HH_INCLUDED

#include "lev/Proxy.hh"

#include <map>
#include <string>
#include <vector>

#define INDEX_DEFAULT_GROUP "Default"

namespace enigma { namespace lev {    
    enum LevelAdvanceMode {
        ADVANCE_NEXT_MODE,       // honor NextLevelMode
        ADVANCE_STRICTLY,        // move to the next level in index
        ADVANCE_UNSOLVED         // move to next not yet solved level
    };

    enum NextLevelMode {
        NEXT_LEVEL_STRICTLY,    // move to the next level in index
        NEXT_LEVEL_UNSOLVED,    // move to next not yet solved level
        NEXT_LEVEL_NOT_BEST,    // move to next level where player is not best score holder
        NEXT_LEVEL_OVER_PAR     // move to next level with a score over PAR 
    };
    
    /**
     * 
     */
    class Index  {
    public:
        static void registerIndex(Index *anIndex);
        static Index * findIndex(std::string anIndexName);
        static Index * getCurrentIndex();
        static bool setCurrentIndex(std::string anIndexName);
        static Index * nextGroupIndex();
        static Index * previousGroupIndex();
        static Proxy * getCurrentProxy();
        static std::vector<Index *> * getGroup(std::string groupName);
        
        /**
         * Convention: method names *Level() can take int pos or Proxy as arg.
         */
        Index(std::string anIndexName = "Unnamed Pack", 
                std::string aGroupName = INDEX_DEFAULT_GROUP, 
                double defaultLocation = 200000);
        ~Index();
        
        std::string getName();
        std::string getGroupName();
        int getCurrentPosition(); // 0 .. size-1
        int getCurrentLevel(); // 1 .. size
        Proxy * getCurrent();
        void setCurrentPosition(int newPos);
        int getScreenFirstPosition();
        void setScreenFirstPosition(int iFirstPos);
        virtual bool mayPlayLevel(int levelNumber);
        Proxy * getProxy(int pos);
        virtual bool advanceLevel(LevelAdvanceMode advMode);
                
        /*! Return number of levels */
        virtual int size() const;
        virtual void appendProxy(Proxy * newLevel, controlType varCtrl = force,
                scoreUnitType varUnit = duration, std::string varTarget = "time",
                std::map<std::string, std::string> varExtensions = nullExtensions);
        virtual void clear();
        void updateFromProxies();

        // ---------- LevelPack legacy methods ---to be renamed ------- */
       /*! Return the default SoundSet (see options::SoundSet for meaning) */
        virtual int get_default_SoundSet() const;

        /*! Returns true if it's a twoplayer levelpack, but has no
          it-yinyang (needed to add it-yinyang to inventory if
          oxyd-linkgame is played as single-player) */
        virtual bool needs_twoplayers() const;


    protected:
        std::string indexName;
        std::string indexGroup;
        double indexDefaultLocation;
        int currentPosition; // 0,...
        int screenFirstPosition; // LevelWidget ifirst
        std::vector<Proxy *> proxies;
        static std::map<std::string, std::string> nullExtensions;
    
    private:
        /**
         * A map of index names to the indices themselves.
         */
        static std::map<std::string, Index *> indices;
        
        /**
         * A map of index group names to vectors of indices. The vectors
         * are sorted by the user sequence preference in the index group menu.
         * Every index is listed in the group the user asigned it to.
         */
        static std::map<std::string, std::vector<Index *> *> indexGroups;
        
        /**
         * Current active index. This index is selected in the Levelpack menu,
         * shown and used in the submenus and stored in the user preferences.
         * It's default is the "Tutorial" index.
         */
        static Index * currentIndex;
        static std::vector<Index *> * getGroup(Index * anIndex);

    };

}} // namespace enigma::lev
#endif
