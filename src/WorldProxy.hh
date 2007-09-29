/*
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
#ifndef WORLDPROXY_HH_INCLUDED
#define WORLDPROXY_HH_INCLUDED

#include "objects_decl.hh"

namespace world {

    class WorldProxy : public Object {
    public:
        static WorldProxy *instance();
        static void shutdown();
        virtual Value getAttr(const string& key) const;
        virtual void set_attrib(const string& key, const Value &val);
        
    protected:
        WorldProxy();
        virtual Object *clone() {return this;}; // dummy implementation
        virtual void dispose() {};             // dummy implementation
    private:
        static WorldProxy *theSingleton;
    };
} // namespace world

#endif
