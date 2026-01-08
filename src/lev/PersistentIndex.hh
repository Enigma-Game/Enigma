/*
 * Copyright (C) 2006,2007,2008,2009 Ronald Lamprecht
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
 
#ifndef LEV_PERSISTENTINDEX_HH_INCLUDED
#define LEV_PERSISTENTINDEX_HH_INCLUDED

#include "lev/Index.hh"

#include <string>
#include <istream>
#include <memory>
#include <xercesc/dom/DOMDocument.hpp>


#define INDEX_STD_FILENAME  "index.xml"

namespace enigma { namespace lev {    
    
    struct Variation {
        // Constructor
        Variation(controlType ctrlValue = force, scoreUnitType unitValue = duration,
            std::string targetValue = "time");
        
        controlType     ctrl;
        scoreUnitType   unit;
        std::string     target;
        std::map<std::string, std::string> extensions;
        
        bool operator == (const Variation& otherVar);
    };


    /**
     * 
     */
    class PersistentIndex : public Index  {
    public:
        static void registerPersistentIndices(bool onlySystemIndices);
        static PersistentIndex * historyIndex;
        static void addCurrentToHistory();
        static void shutdown();
        
        /**
         * Convention: method names *Level() can take int pos or Proxy as arg.
         */
        /**
         * 
         * thePackPath " " for a new not yet defined path
         */
        PersistentIndex(std::string thePackPath, bool systemOnly, bool userOwned = true, bool isAuto = false,
                double defaultLocation = INDEX_DEFAULT_PACK_LOCATION,
                std::string anIndexName = "",
                std::string theIndexFilename = INDEX_STD_FILENAME, 
                std::string aDescription = "",
                std::string aGroupName = INDEX_DEFAULT_GROUP);
        /**
         * Legacy 0.92 constructor - called once to convert the index to XML.
         * When the index has been stored as XML this constructor will not be
         * called again.
         */
        PersistentIndex(std::stringstream &legacyIndex, std::string thePackPath, bool isZip = false,
                std::string anIndexName = "",
                std::string theIndexFilename = INDEX_STD_FILENAME,
                std::string aDescription = "");
        ~PersistentIndex();
        
        /**
         * 
         */
        void load(bool loadSystemFS = false, bool update = false);
        void loadDoc();
        std::string getPackPath();
        bool setName(std::string newName, bool isSokoball);
        std::string getOwner();
        void setOwner(std::string newOwner);
        int getRelease();
        void setRelease(int newRelease);
        int getRevision();
        void setRevision(int newRevision);
        double getCompatibility();
        void setCompatibility(double newCompatibility);
        bool isUserEditable();
        bool isUpdatable();
        bool isCross();
        void markNewAsCross();
        virtual void clear();
        virtual void appendProxy(Proxy * newLevel, controlType varCtrl = force,
                scoreUnitType varUnit = duration, std::string varTarget = "time",
                std::map<std::string, std::string> varExtensions = nullExtensions);
        void insertProxy(int pos, Proxy * newLevel, bool allowDuplicates = true,
                controlType varCtrl = force, scoreUnitType varUnit = duration, 
                std::string varTarget = "time",
                std::map<std::string, std::string> varExtensions = nullExtensions);
        Variation getVariation(int pos);
        void erase(int pos);
        void exchange(int pos1, int pos2);
        virtual bool isSource(Proxy * aProxy);
        virtual void updateFromFolder();
        bool save(bool allowOverwrite = true);
    protected:
        std::string packPath;  // "auto", "", "enigma_i", ...
        std::string indexFilename;
        std::string owner;
        int release;
        int revision;
        double compatibility;
        std::vector<Variation> variations;
        bool isModified;
        bool isUserOwned;
        bool isEditable;
        bool isAuto;
        std::string indexUrl;
    private:
        static std::vector<std::shared_ptr<PersistentIndex> > indexCandidates;
        std::string absIndexPath;
        XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc;
        XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *infoElem;
        XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *updateElem;
        XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *levelsElem;
        
//        static void checkCandidate(PersistentIndex * candidate);
        static void checkCandidate(std::string thePackPath, bool systemOnly, bool userOwned = true, 
                bool isAuto = false, bool isSystemCross = false, bool isUserCross = false, 
                double defaultLocation = INDEX_DEFAULT_PACK_LOCATION, std::string anIndexName = "",
                std::string theIndexFilename = INDEX_STD_FILENAME, std::string aDescription = "",
                std::string aGroupName = INDEX_DEFAULT_GROUP);
        // legacy 0.92
        void parsePar(const std::string& par, int& par_value, std::string& par_text);
    };

    void AddLevelPack (const char *init_file, const char *name);
    void AddZippedLevelPack (const char *zipfile);

}} // namespace enigma::lev
#endif
