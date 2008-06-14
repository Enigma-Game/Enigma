/*
 * Copyright (C) 2008 Ronald Lamprecht
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
#ifndef CLUSTERSTONE_HH
#define CLUSTERSTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     */
    class ClusterStone : public Stone {
        CLONEOBJ(ClusterStone);
    public:
        ClusterStone(std::string classname, std::string connections);
        
        // Object interface
        virtual std::string getClass() const;
        virtual const char *get_kind() const;  // for backward compatibility
        virtual void setAttr(const string& key, const Value &val);
        virtual Value message(const Message &m);
        
        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual std::string getModelName() const;
        
        // Stone interface
        virtual bool is_removable() const;
        
    private:
        void autoJoinCluster();
        void autoLeaveCluster();
    };

} // namespace enigma

#endif /*CLUSTERSTONE_HH_*/
