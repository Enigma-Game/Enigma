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

#include "lev/PersistentIndex.hh"
#include "lev/Proxy.hh"
#include "errors.hh"
#include "main.hh"
#include "file.hh"
#include "options.hh"
#include "oxyd.hh"



namespace enigma { namespace lev {

    void PersistentIndex::registerPersistentIndices() {
        // register dirs and zips with xml-indices incl auto 
        
        // register auto not yet registered new files
        PersistentIndex * autoIndex = new PersistentIndex("Auto",DEFAULT_INDEX_GROUP);
        DirIter * dirIter = DirIter::instance(app.userPath + "/levels/auto");
        DirEntry dirEntry;
        while (dirIter->get_next(dirEntry) && !dirEntry.is_dir) {
            if ((dirEntry.name.rfind(".xml") == dirEntry.name.size() - 4) ||
                    (dirEntry.name.rfind(".lua") == dirEntry.name.size() - 4)) {
                Proxy * newProxy = Proxy::autoRegisterLevel("auto", 
                        dirEntry.name.substr(0, dirEntry.name.size() - 4));
                if (newProxy != NULL) {
                    // first check that the proxy is not in the index
                    //  - may occur if the level is stored as .xml and .lua in the folder
                    
                    // is it is new, add it
                    autoIndex->appendProxy(newProxy);
                }
            }
        }
        delete dirIter;
        Index::registerIndex(autoIndex);
    }

    PersistentIndex::PersistentIndex(std::string anIndexName, 
            std::string aGroupName) : Index(anIndexName, aGroupName) {
    }
    
    PersistentIndex::~PersistentIndex() {}
    
    void PersistentIndex::clear() {
        proxies.clear();
    }
    


}} // namespace enigma::lev
