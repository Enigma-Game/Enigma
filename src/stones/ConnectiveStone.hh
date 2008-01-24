/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
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
#ifndef CONNECTIVESTONE_HH_INCLUDED
#define CONNECTIVESTONE_HH_INCLUDED

#include "stones.hh"

/* -------------------- ConnectiveStone -------------------- */

// base class for PuzzleStone and BigBrick

namespace enigma {
    
    class ConnectiveStone : public Stone {
    public:
        ConnectiveStone(const char *kind, int connections): Stone(kind) {
            set_attrib("connections", connections);
        }
        ConnectiveStone(int connections) {
            set_attrib("connections", connections);
        }

        DirectionBits get_connections() const {
            int conn = (int)getAttr("connections") - 1;
            if (conn >=0 && conn <16)
                return DirectionBits(conn);
            else
                return NODIRBIT;
        }

        // temp. workarounds until this class gets eliminated
        virtual void set_attrib(const string& key, const Value &val) {
            if (key == "connections")
                Object::set_attrib(key, val);  // bypass GridObject
            else
                Stone::set_attrib(key, val);
        }
        
        virtual Value getAttr(const string &key) const {
            if (key == "connections")
                return Object::getAttr(key);
            else
                return Stone::getAttr(key);
        }


    protected:
        virtual void init_model() {
            set_model(get_kind()+ecl::strf("%d", get_modelno()));
        }

        virtual int get_modelno() const {
            return getAttr("connections");
        }
    };

} // namespace enigma

#endif
