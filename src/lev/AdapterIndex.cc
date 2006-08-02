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

#include "lev/AdapterIndex.hh"
#include "errors.hh"
#include "main.hh"
#include "options.hh"
#include "oxyd.hh"



namespace enigma { namespace lev {

    AdapterIndex::AdapterIndex(enigma::levels::LevelPack *legacyPack) : adaptee (legacyPack) {
        for (int i=0 ; i< adaptee->size(); i++) {
            proxies.push_back(adaptee->get_info(i).proxy);
        }
        indexName = adaptee->get_name();
    }
    
    AdapterIndex::~AdapterIndex() {}
    
    
     /* ---------- LevelPack interface ---------- */
    

    /*! Return the default SoundSet (see options::SoundSet for meaning) */
    int AdapterIndex::get_default_SoundSet() const { 
        return adaptee->get_default_SoundSet();
    }

    /*! Returns true if it's a twoplayer levelpack, but has no
      it-yinyang (needed to add it-yinyang to inventory if
      oxyd-linkgame is played as single-player) */
    bool AdapterIndex::needs_twoplayers() const { 
        return adaptee->needs_twoplayers();
    }

}} // namespace enigma::lev
