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
#ifndef SWAPSTONE_HH
#define SWAPSTONE_HH

#include "stones/YieldingStone.hh"

#include "stones_internal.hh"
#include "util.hh"

namespace enigma {

    /** 
     * 
     */
    class SwapStone : public YieldingStone, public TimeHandler {
        DECL_TRAITS;
        
    private:
        enum iState {
            IDLE,       ///<  
            APPEARING,  ///< 
            VANISHING   ///< 
        };
        
    public:
        SwapStone();
        ~SwapStone();
        SwapStone* clone();
        
        // Object interface
        virtual std::string getClass() const;
        
        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        
        // Stone interface
        virtual bool is_floating() const;
        virtual bool is_sticky(const Actor *a) const;
        virtual bool is_removable() const;
        virtual void on_impulse(const Impulse& impulse);
        
        // TimeHandler interface
        virtual void alarm();

    private:
        
    };

} // namespace enigma

#endif
