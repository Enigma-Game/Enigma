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
#ifndef COUNTERGADGET_HH
#define COUNTERGADGET_HH

#include "others/Other.hh"
#include "display.hh"
#include "stones.hh"

/* -------------------- Other -------------------- */


namespace enigma {
    
    class CounterGadget : public Other {
        CLONEOBJ(CounterGadget)

    public:
        CounterGadget();
        
        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const std::string &key, const Value &val);
//        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);
        
    private:
    };

} // namespace enigma

#endif
