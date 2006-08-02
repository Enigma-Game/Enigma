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
 
#ifndef LEV_ADAPTERINDEX_HH_INCLUDED
#define LEV_ADAPTERINDEX_HH_INCLUDED

#include "lev/Index.hh"

#include "levels.hh"

#include <string>

namespace enigma { namespace lev {    
    /**
     * 
     */
    class AdapterIndex : public Index  {
    public:
        /**
         * Convention: method names *Level() can take int pos or Proxy as arg.
         */
        AdapterIndex(enigma::levels::LevelPack *legacyPack);
        ~AdapterIndex();
        
        // ---------- LevelPack compatible methods ---------- */

        // ---------- LevelPack legacy methods ---------- */
// may be removed

        /*! Return the default SoundSet (see options::SoundSet for meaning) */
        virtual int get_default_SoundSet() const;

        /*! Returns true if it's a twoplayer levelpack, but has no
          it-yinyang (needed to add it-yinyang to inventory if
          oxyd-linkgame is played as single-player) */
        virtual bool needs_twoplayers() const;

    private:
        enigma::levels::LevelPack *adaptee;
    };

}} // namespace enigma::lev
#endif
