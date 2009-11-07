/*
 * Copyright (C) 2009 Ronald Lamprecht
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
#ifndef SCALESFLOOR_HH
#define SCALESFLOOR_HH

#include "floors.hh"

namespace enigma {

    /** 
     * 
     */
    class ScalesFloor : public Floor {
        CLONEOBJ(ScalesFloor);
        
    private:
        enum iState {
            OFF,    ///< 
            ON      ///< 
        };

        enum ObjectPrivatFlagsBits {
            OBJBIT_STONE =   1<<24,   ///< pressed by a stone
        };
    public:
        ScalesFloor(std::string flavor);

        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const string& key, const Value &val);
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual std::string getModelName() const;
        virtual void on_creation(GridPos p);
        virtual void actor_enter(Actor *a);
        virtual void actor_leave(Actor *a);
                
        // Floor interface
        virtual void stone_change(Stone *st);

    private:
        // Methods
        bool weightActors();
        void updateIState(bool refuseAction = false);
    };

} // namespace enigma

#endif
