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

#include "stones/PullStone.hh"
#include "errors.hh"
//#include "main.hh"
#include "server.hh"
#include "world.hh"

namespace enigma {
    PullStone::PullStone() : YieldingStone() {
    }
    
    PullStone::~PullStone() {
        GameTimer.remove_alarm(this);
    }
    
    PullStone* PullStone::clone() {
        return new PullStone(*this);
    }
    
    std::string PullStone::getClass() const {
        return "st_pull";
    }

    void PullStone::setState(int extState) {
        // reject any write attempts
    }
    
    void PullStone::init_model() {
        set_model("st-pull");
    }
    
    bool PullStone::is_floating() const {
        // a moving pull stone should press just one of the two affected grids at each
        // point of time. The vanishing part is the proxy of the other exchanged stone.
        // Thus it inherits the floating behaviour from it. 
        return (state == VANISHING && 
                (yieldedStone == NULL || yieldedStone->is_floating())); 
    }
    
    bool PullStone::is_sticky(const Actor *a) const {
        return true;   // independent of floating behaviour
    }
    
    bool PullStone::is_removable() const {
        return state == IDLE;
    }
    
    bool PullStone::is_transparent(Direction d) const {
        // a moving pull stone should be transparent just on of the two affected grids at each
        // point of time. The vanishing part is the proxy of the other exchanged stone.
        // Thus it inherits the transparency behaviour from it. 
        return (state != VANISHING || 
                (yieldedStone != NULL && yieldedStone->is_transparent(d))); 
    }
    
    void PullStone::on_impulse(const Impulse& impulse) {
        if (state != IDLE)
            return;

        GridPos oldPos = get_pos();
        GridPos newPos = move(oldPos, reverse(impulse.dir));
        Stone *otherStone = GetStone(newPos);
    
        if (!IsInsideLevel(newPos) || (otherStone && (!otherStone->is_removable() || 
                (IsLevelBorder(newPos) && server::GameCompatibility != GAMET_ENIGMA)))) {
            propagateImpulse(impulse);
            return;                 // avoid unremoveable and border stones
        }
        
        PullStone *vanishStone = dynamic_cast<PullStone *>(MakeObject("st_pull"));
        vanishStone->state = VANISHING;
        
        YieldStone(oldPos);
        SetStone(oldPos, vanishStone);
        vanishStone->yieldStone(otherStone);
        vanishStone->set_model(std::string("st-pull") + to_suffix(impulse.dir));
        GameTimer.set_alarm(vanishStone, 0.09, false);
        
        SetStone(newPos, this);
        state = APPEARING;
        set_model(std::string("st-pull") + to_suffix(reverse(impulse.dir)));
        GameTimer.set_alarm(this, 0.09, false);

        // search for affected actors
        std::vector<Actor*> found_actors;
        const double   range_one_field = 1.415; // approx. 1 field [ > sqrt(1+1) ]
        GetActorsInRange(newPos.center(), range_one_field, found_actors);
        std::vector<Actor*>::iterator e = found_actors.end();
        ObjectList actors;
        for (std::vector<Actor*>::iterator i = found_actors.begin(); i != e; ++i) {
            Actor *actor = *i;
            GridPos actor_pos(actor->get_pos());
    
            if (actor_pos == newPos) { // if the actor is in the dest field
                actors.push_back(actor);
                SendMessage(actor, "_freeze");

                ecl::V2 mid_dest = actor->get_pos();
                mid_dest[0] = ecl::Clamp<double> (mid_dest[0], oldPos.x+0.01, oldPos.x+0.99);
                mid_dest[1] = ecl::Clamp<double> (mid_dest[1], oldPos.y+0.01, oldPos.y+0.99);
                WarpActor(actor, mid_dest[0], mid_dest[1], false);                
            }
        }
        vanishStone->setAttr("$frozen_actors", actors);

        sound_event("moveslow");
        propagateImpulse(impulse);
    }
    
    void PullStone::alarm() {
        if (state == APPEARING) {
            state = IDLE;
            if (isDisplayable())
                init_model();
        } else if (state == VANISHING) {
            ObjectList actors = getAttr("$frozen_actors");
            for (ObjectList::iterator itr = actors.begin(); itr != actors.end(); ++itr)
                SendMessage(*itr, "_revive");
            setStone();
        } else
            ASSERT(false, XLevelRuntime, "PullStone: alarm called with inconsistent state");
    }
        
    DEF_TRAITSM(PullStone, "st_pull", st_pull, MOVABLE_IRREGULAR);
    
    BOOT_REGISTER_START
        BootRegister(new PullStone(), "st_pull");
    BOOT_REGISTER_END

} // namespace enigma
