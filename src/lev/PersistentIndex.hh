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
 
#ifndef LEV_PERSISTENTINDEX_HH_INCLUDED
#define LEV_PERSISTENTINDEX_HH_INCLUDED

#include "lev/Index.hh"

#include <string>
#include <istream>

#define INDEX_STD_FILENAME  "index.xml"

namespace enigma { namespace lev {    
    
    
    /**
     * 
     */
    class PersistentIndex : public Index  {
    public:
        static void registerPersistentIndices();
        
        /**
         * Convention: method names *Level() can take int pos or Proxy as arg.
         */
        PersistentIndex(std::string thePackPath,  std::string anIndexName = "", 
                std::string theIndexFilename = INDEX_STD_FILENAME, std::string aGroupName = INDEX_DEFAULT_GROUP);
        /**
         * Legacy 0.92 constructor - called once to convert the index to XML.
         * When the index has been stored as XML this constructor will not be
         * called again.
         */
        PersistentIndex(std::istream *legacyIndex, std::string thePackPath,  bool isZip = false,
                std::string anIndexName = "", std::string theIndexFilename = INDEX_STD_FILENAME);
        ~PersistentIndex();
        virtual void clear();
    protected:
        std::string packPath;
        std::string indexFilename;
    private:
        // legacy 0.92
        void parsePar(const string& par, int& par_value, std::string& par_text);
    };

    void AddLevelPack (const char *init_file, const char *name);
    void AddZippedLevelPack (const char *zipfile);

}} // namespace enigma::lev
#endif
