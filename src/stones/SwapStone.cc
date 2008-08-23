/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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

#include "stones/SwapStone.hh"
#include "errors.hh"
//#include "main.hh"
#include "server.hh"
#include "world.hh"

namespace enigma {
    SwapStone::SwapStone() : YieldingStone() {
    }
    
    SwapStone::~SwapStone() {
        GameTimer.remove_alarm(this);
    }
    
    SwapStone* SwapStone::clone() {
        return new SwapStone(*this);
    }
    
    std::string SwapStone::getClass() const {
        return "st_swap";
    }

    void SwapStone::setState(int extState) {
        // reject any write attempts
    }
    
    void SwapStone::init_model() {
        set_model("st-swap");
    }
    
    bool SwapStone::is_floating() const {
        // a moving swap stone should press just one of the two affected grids at each
        // point of time. The vanishing part is the proxy of the other exchanged stone.
        // Thus it inherits the floating behaviour from it. 
        return (state == VANISHING && 
                (yieldedStone == NULL || yieldedStone->is_floating())); 
    }
    
    bool SwapStone::is_sticky(const Actor *a) const {
        return true;   // independent of floating behaviour
    }
    
    bool SwapStone::is_removable() const {
        return state == IDLE;
    }
    
    bool SwapStone::is_transparent(Direction d) const {
        // a moving swap stone should be intransparent just on of the two affected grids at each
        // point of time. The vanishing part is the proxy of the other exchanged stone.
        // Thus it inherits the transparency behaviour from it. 
        return (state == VANISHING && 
                (yieldedStone != NULL && yieldedStone->is_transparent(d))); 
    }
    void SwapStone::on_impulse(const Impulse& impulse) {
        if (state != IDLE)
            return;

        GridPos oldPos = get_pos();
        GridPos newPos = move(oldPos, impulse.dir);
        Stone *otherStone = GetStone(newPos);
    
        if (!IsInsideLevel(newPos) || !otherStone || !otherStone->is_removable() || 
                (IsLevelBorder(newPos) && server::GameCompatibility != GAMET_ENIGMA)) {
            propagateImpulse(impulse);
            return;                 // avoid unremoveable and border stones
        }
        
        SwapStone *vanishStone = dynamic_cast<SwapStone *>(MakeObject("st_swap"));
        vanishStone->state = VANISHING;
        
        YieldStone(oldPos);
        SetStone(oldPos, vanishStone);
        vanishStone->yieldStone(otherStone);
        vanishStone->set_model(std::string("st-swap") + to_suffix(reverse(impulse.dir)));
        GameTimer.set_alarm(vanishStone, 0.1, false);
        
        SetStone(newPos, this);
        state = APPEARING;
        set_model(std::string("st-swap") + to_suffix(impulse.dir));
        GameTimer.set_alarm(this, 0.1, false);


        sound_event("moveslow");
        server::IncMoveCounter(1);
        propagateImpulse(impulse);
    }
    
    void SwapStone::alarm() {
        if (state == APPEARING) {
            state = IDLE;
            if (isDisplayable())
                init_model();
        } else if (state == VANISHING) {
            setStone();
        } else
            ASSERT(false, XLevelRuntime, "SwapStone: alarm called with inconsistent state");
    }
        
    DEF_TRAITSM(SwapStone, "st_swap", st_swap, MOVABLE_IRREGULAR);
    
    BOOT_REGISTER_START
        BootRegister(new SwapStone(), "st_swap");
    BOOT_REGISTER_END

} // namespace enigma
