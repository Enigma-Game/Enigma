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
#ifndef BRIDGEFLOOR_HH
#define BRIDGEFLOOR_HH

#include "floors.hh"

namespace enigma {

    /** 
     * 
     */
    class BridgeFloor : public Floor {
        CLONEOBJ(BridgeFloor);
        
    private:
        enum iState {
            CLOSED,    ///< 
            OPEN,      ///< 
            CLOSING,   ///< 
            OPENING,   ///< 
        };

        enum ObjectPrivatFlagsBits {
            OBJBIT_EXPLICIT =   1<<24,   ///< closed by message and not by a stone
        };
    public:
        BridgeFloor(std::string flavor);

        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const string& key, const Value &val);
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);
        virtual void toggleState();

        // GridObject interface
        virtual void init_model();
        virtual void on_creation(GridPos p);
                
        // ModelCallback interface  - Animation callback
        virtual void animcb();

        // Floor interface
        virtual void stone_change(Stone *st);
        virtual void actor_contact(Actor *a);

    protected:
        bool has_firetype(FloorFireType selector);
            
    private:
        // Private methods.
        std::string model_basename() const;
        void set_iState(int newState);
    };

} // namespace enigma

#endif
