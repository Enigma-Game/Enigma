/*
 * Copyright (C) 2007 Andreas Lochmann
 * Copyright (C) 2007 Raoul Bourquin
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

#include "floors/ThiefFloor.hh"
#include "errors.hh"
#include "Inventory.hh"
#include "items/GlassesItem.hh"
#include "player.hh"
#include "world.hh"
//#include "main.hh"

namespace enigma {
    ThiefFloor::ThiefFloor() : Floor("fl_thief", 4.5, 1.5), victimId (0), bag (NULL) {
        
    }
    
    ThiefFloor::~ThiefFloor() {
        if (bag != NULL)
            delete bag;
    }

    std::string ThiefFloor::getClass() const {
        return "fl_thief";
    }
    
    Value ThiefFloor::message(const Message &m) {
        if (m.message == "_capture" && (state == IDLE || state == DRUNKEN) && isDisplayable()) {            
            // add items on grid pos that can be picked up to our bag
            Item * it =  GetItem(get_pos());
            if (it != NULL && !(it->get_traits().flags & itf_static) && bag != NULL) {
                dynamic_cast<ItemHolder *>(bag)->add_item(YieldItem(get_pos()));
            }
            // drop bag if pos is not occupied by a static item
            if (GetItem(get_pos()) == NULL) {
                SetItem(get_pos(), bag);
                bag = NULL;
            }
            state = (state == IDLE) ? CAPTURE : DRUNKENCAPTURE;
            init_model();
            return true;
        }
        return Floor::message(m);
    }
   
    void ThiefFloor::setState(int extState) {
        // block all state writes
    }
    
    void ThiefFloor::on_creation(GridPos p) {
        objFlags |= (IntegerRand(0, 3) << 24);
        Floor::on_creation(p);
    }
    
    std::string ThiefFloor::getModelName() const {
        return ecl::strf("fl_thief%d", ((objFlags & OBJBIT_MODEL) >> 24) + 1);
    }
    
    void ThiefFloor::init_model() {
        std::string basename = getModelName();
        switch (state) {
            case IDLE:
            case CAPTURED:
                set_model(basename); 
                break;
            case EMERGING:
                set_anim(basename + "_emerge");
                break;
            case RETREATING:
                set_anim(basename + "_retreat");
                break;
            case CAPTURE:
                set_anim(basename + "_capture");
                break;
            case DRUNKEN:
                set_anim(basename + "_drunken");
                break;
            case DRUNKENCAPTURE:
                set_anim(basename + "_capture");
                break;
        }
    }
    
    void ThiefFloor::actor_enter(Actor *a) {
        if (state == IDLE && a->is_on_floor()) {
            state = EMERGING;
            victimId = a->getId();
            init_model();
        }
    }

    void ThiefFloor::animcb() {
        Floor *newfloor;
        switch (state) {
            case EMERGING:
                doSteal();
                if (state != DRUNKEN)
                    state = RETREATING;
                init_model();
                break;
            case RETREATING:
                state = IDLE;
                init_model();
                break;
            case CAPTURE:
            case DRUNKENCAPTURE:
                state = CAPTURED;
                init_model();
                break;
            default:
                ASSERT(0, XLevelRuntime, "ThiefFloor: animcb called with inconsistent state");
        }
    }

    void ThiefFloor::doSteal() {
        bool didSteal = false;
        
        // the actor that hit the thief may no longer exist!
        if (Actor *victim = dynamic_cast<Actor *>(Object::getObject(victimId))) {
            if (Value owner = victim->getAttr("owner")) {
                if (!(victim->has_shield())) {
                    enigma::Inventory *inv = player::GetInventory(owner);
                    if (inv && inv->size() > 0) {
                        if (bag == NULL) {
                            bag = MakeItem("it_bag");
                            bag->setOwnerPos(get_pos());
                        }
                        int i = IntegerRand(0, int (inv->size()-1));
                        Item *it = inv->yield_item(i);
                        dynamic_cast<ItemHolder *>(bag)->add_item(it);
                        didSteal = true;
                        Glasses::updateGlasses();
                        player::RedrawInventory(inv);
                        if (it->getKind() == "it_bottle_idle")
                            state = DRUNKEN;
                    }
                }
            }
        }
        // steal from grid
        if(Item *it = GetItem(get_pos())) {
            if (!(it->get_traits().flags & itf_static)) {
                if (bag == NULL) {
                    bag = MakeItem("it_bag");
                    bag->setOwnerPos(get_pos());                
                }
                Item *it = YieldItem(get_pos());
                dynamic_cast<ItemHolder *>(bag)->add_item(it);
                didSteal = true;
                if (it->getKind() == "it_bottle_idle")
                    state = DRUNKEN;
            }
        }
        if (didSteal)
            sound_event("thief");
    }
        
    BOOT_REGISTER_START
        BootRegister(new ThiefFloor(), "fl_thief");
    BOOT_REGISTER_END

} // namespace enigma
