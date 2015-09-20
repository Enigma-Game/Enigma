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
 */
#ifndef PULLERITEM_HH
#define PULLERITEM_HH

#include "items.hh"

namespace enigma {
    /**
     */
    class PullerItem : public Item {
        CLONEOBJ(PullerItem);
        DECL_ITEMTRAITS_ARRAY(4, orientation());

    private:
        enum iState {
            IDLE, 
            ACTIVE 
        };
        enum ObjectPrivatFlagsBits {
            OBJBIT_ORIENTATION =   3<<24,    ///<
        };
         
    public:
        PullerItem(int dir);

        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const std::string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;

        // ModelCallback interface
        virtual void animcb();
        
        // Item interface
        virtual bool isStatic() const;
        virtual void on_drop(Actor *a);
    
    private:
        int orientation() const;
    };
   
} // namespace enigma

#endif
