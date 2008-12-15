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
#ifndef PASSAGESTONE_HH
#define PASSAGESTONE_HH

#include "stones.hh"
#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     * 
     */
    class PassageStone : public Stone {
         CLONEOBJ(PassageStone);
         DECL_TRAITS_ARRAY(8, traitsIdx());
    private:
        enum iState {
            BLACK,
            WHITE
        };
        enum ObjectPrivatFlagsBits {
            OBJBIT_FLAVOR   =   3<<24,    ///< 
        };

    public:
        PassageStone(int color, int flavor); 

        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);
        
        // GridObject interface
        virtual void init_model();
        
        // Stone interface
        virtual bool is_floating() const;
        virtual bool is_transparent(Direction d) const;
        virtual StoneResponse collision_response(const StoneContact &sc);
               
    private:
        // Private methods.
        int traitsIdx() const;
    };

} // namespace enigma

#endif
