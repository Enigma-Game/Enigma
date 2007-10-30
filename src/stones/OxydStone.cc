/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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

#include "stones/OxydStone.hh"
#include "server.hh"
#include "world.hh"

namespace enigma {
 
    OxydStone::InstanceList OxydStone::instances;
    
    OxydStone * OxydStone::clone() { 
        OxydStone *o = new OxydStone(*this); 
        instances.push_back(o);
        if (server::EnigmaCompatibility >= 1.10) {
            int color = ((instances.size() -1) / 2) % 8;
            o->set_attrib("color", color);
        }
        return o;
    }
    
    OxydStone::OxydStone()
    : PhotoStone("st-oxyd"),
      state(CLOSED)
    {
        set_attrib("flavor", "b");
        set_attrib("color", 0);
    }
    
    Value OxydStone::message(const string &m, const Value &val) 
    {
        if (m=="closeall") {
            for (unsigned i=0; i<instances.size(); ++i)
                instances[i]->change_state(CLOSING);
        }
        else if (m=="shuffle") {
            shuffle_colors();
        }
        else if (m=="trigger" || m=="spitter") {
            maybe_open_stone();
        }
        else if (m=="signal" && to_int(val) != 0) {
            maybe_open_stone();
        }
        else if (m=="init") {
            // odd number of oxyd stones in the level? no problem, turn a
            // random one into a fake oxyd
    
            if (instances.size() % 2) {
                // "odd number of oxyd stones";
                // TODO
            }
        }
        return Value();
    }
    
    void OxydStone::shuffle_colors() 
    {
        std::vector<size_t> closed_oxyds;
        size_t         isize = instances.size();
        for (size_t i=0; i<isize; ++i) {
            if (instances[i]->state == CLOSED) {
                closed_oxyds.push_back(i);
            }
        }
    
        unsigned size = closed_oxyds.size();
        if (size>1) {
            for (unsigned i = 0; i<size; ++i) {
                unsigned a = IntegerRand(0, static_cast<int> (size-2));
                if (a >= i) ++a;        // make a always different from j
    
                OxydStone *o1 = instances[closed_oxyds[i]];
                OxydStone *o2 = instances[closed_oxyds[a]];
    
                Value icolor = o1->getAttr("color"); 
    
                o1->set_attrib("color", o2->getAttr("color"));
                o2->set_attrib("color", icolor);
            }
        }
    }
    
    void OxydStone::change_state(State newstate) 
    {
        string flavor(getAttr("flavor","a"));
        string color(getAttr("color", 0));
    
        string modelname = string("st-oxyd") + flavor + color;
    
        State oldstate = state;
        state = newstate;
    
        switch (newstate) {
        case CLOSED:
            set_model(string("st-oxyd")+flavor);
            break;
    
        case BLINKING:
            set_model(modelname + "-blink");
            break;
    
        case OPEN:
            if (oldstate == CLOSED) {
                sound_event("oxydopen");
                sound_event("oxydopened");
                set_anim(modelname+"-opening");
            } else {
                set_model(modelname + "-open");
            }
            /* If this was the last closed oxyd stone, finish the
               level */
            if (find_if(instances.begin(),instances.end(),not_open)
                    == instances.end()) {
                server::FinishLevel();
            }
            break;
    
        case OPENING:
            sound_event("oxydopen");
            if (oldstate == CLOSED)
                set_anim(modelname + "-opening");
            else if (oldstate == CLOSING)
                get_model()->reverse();
    
            break;
    
        case CLOSING:
            if (oldstate == CLOSED || oldstate==CLOSING) {
                state = oldstate;
                return;
            }
    
            sound_event("oxydclose");
            if (oldstate == OPENING)
                get_model()->reverse();
            else if (oldstate == BLINKING || oldstate == OPEN) {
                set_anim(modelname + "-closing");
            }
            break;
        }
    }
    
    void OxydStone::animcb() {
        if (state == CLOSING)
            change_state(CLOSED);
        else if (state == OPENING)
            change_state(BLINKING);
        else if (state == OPEN)
            change_state(OPEN); // set the right model
    }
    
    void OxydStone::maybe_open_stone() {
        if (state == CLOSED || state == CLOSING) {
            Value mycolor = getAttr("color");
    
            // Is another oxyd stone currently blinking?
            InstanceList::iterator i;
            i=find_if(instances.begin(), instances.end(), blinking_or_opening);
    
            if (i != instances.end()) {
    
                bool can_open;
    
                if (server::GameCompatibility != GAMET_ENIGMA) {
                    // If colors match and stone (*i) is already blinking,
                    // open both stones. Close one of them otherwise.
                    // (This is the Oxyd behaviour; it doesn't work with
                    // some Enigma levels.)
                    can_open = (mycolor == (*i)->getAttr("color") && (*i)->state==BLINKING);
                }
                else 
                    can_open = (mycolor == (*i)->getAttr("color"));
    
                if (can_open) {
                    change_state(OPEN);
                    (*i)->change_state(OPEN);
                } else {
                    (*i)->change_state(CLOSING);
                    change_state(OPENING);
                }
            }
            else {
                // no blinking stone? -> make this one blink
                change_state(OPENING);
            }
        }
    }
    
    void OxydStone::actor_hit(const StoneContact &/*sc*/) {
        maybe_open_stone();
    }
    
    void OxydStone::on_creation (GridPos) 
    {
        string flavor(getAttr("flavor", "a"));
        set_model(string("st-oxyd") + flavor);
        photo_activate();
    }
    
    bool OxydStone::is_removable() const {
        return !getAttr("static").to_bool();
    }
    
    void OxydStone::on_removal(GridPos p) 
    {
        photo_deactivate();
        kill_model (p);
    }

    BOOT_REGISTER_START
        BootRegister(new OxydStone);
    BOOT_REGISTER_END

} // namespace enigma
