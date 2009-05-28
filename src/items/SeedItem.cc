/*
 * Copyright (C) 2008,2009 Ronald Lamprecht
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

#include "items/SeedItem.hh"
#include "errors.hh"
//#include "main.hh"
#include "server.hh"
#include "stones.hh"
#include "world.hh"

namespace enigma {

    SeedItem::SeedItem(int flavor) {
        objFlags |= flavor << 24;
    }

    std::string SeedItem::getClass() const {
        return "it_seed";
    }
    
    void SeedItem::setAttr(const string& key, const Value &val) {
        if (key == "flavor") {
            std::string flavor = val.to_string();
            int code;
            if (flavor == "wood") code = 0;
            else if (flavor == "greenbrown") code = 1;
            else if (flavor == "volcano") code = 2;
            else if (flavor == "hay") code = 3;
            else
                ASSERT(false, XLevelRuntime, ecl::strf("Seed: illegal flavor value %s", flavor.c_str()).c_str());
                
            objFlags = (objFlags & ~OBJBIT_FLAVOR) | (code << 24);
            if (isDisplayable()) {
                init_model();
            }
            return;
        }
        Item::setAttr(key, val);
    }
    
    Value SeedItem::getAttr(const string &key) const {
        if (key == "flavor") {
            int flavor = (objFlags & OBJBIT_FLAVOR) >> 24;
            switch (flavor) {
                case 0 : return "wood"; break;
                case 1 : return "greenbrown"; break;
                case 2 : return "volcano"; break;
                case 3 : return "hay"; break;
            }
        } else
            return Item::getAttr(key);
    }
    
    Value SeedItem::message(const Message &m) {
        if (m.message == "grow" || m.message == "signal") {
            if (isDisplayable())
                startGrowing();
            return Value();
        }
        return Item::message(m);
    }
    
    void SeedItem::setState(int extState) {
        state = extState;
        if (state == ACTIVE && isDisplayable())
            startGrowing();
        
    }
    
    void SeedItem::init_model() {
        if (state == IDLE)
            set_model("it_seed");
        else
            set_anim("it_seed_growing");
    }
    
    void SeedItem::processLight(Direction d) {
        startGrowing();
    }
    
    void SeedItem::animcb() {
        GridPos p= get_pos();
        int flavor = (objFlags & OBJBIT_FLAVOR) >> 24;
        if ((server::GameCompatibility == GAMET_OXYDMAGNUM || server::GameCompatibility == GAMET_OXYD1) &&
            (flavor == 0 && GetStone(p))) {
            if (GetStone(p)->getKind() == "st_grate_cross") {
                SetFloor(p, MakeFloor("fl_wood_framed"));
                kill();
                return;
           }
       }
       Stone *st = MakeStone(flavor == 0 ? "st_box_wood_growing" : (flavor == 1 ? "st_greenbrown_growing" :
               (flavor == 2 ? "st_volcano_growing" : "st_box_hay_growing")));
       ASSERT(st != NULL, XLevelRuntime, "SeedItem - failure on stone creation");
       transferIdentity(st);
       if (Value v = getAttr("secure"))
           st->setAttr("secure", v);
       SetStone(p, st);
       kill();
    }
    
    std::string SeedItem::get_inventory_model() {
        return "it_seed";
    }
    
    bool SeedItem::isStatic() const {
        return state == ACTIVE;  // active seed is static
    }
    
    void SeedItem::on_drop(Actor *a) {
        startGrowing();
    }
    
    void SeedItem::on_stonehit(Stone *st) {
        startGrowing();
    }
    
    bool SeedItem::actor_hit(Actor *a) {
        if (state == ACTIVE)
            return false;   // do not pickup growing seed
        return Item::actor_hit(a);
    }

    
    void SeedItem::startGrowing() {
        if (state == IDLE) {
            state = ACTIVE;
            sound_event("seedgrow");
            set_anim("it_seed_growing");
        }
    }
        
    int SeedItem::traitsIdx() const {
        return (objFlags & OBJBIT_FLAVOR) >> 24;  // access should be fast for traits radius access
    }
    

    ItemTraits SeedItem::traits[3] = {
        {"it_seed_wood", it_seed_wood, itf_static, 0.2},
        {"it_seed_greenbrown", it_seed_fake, itf_static, 0.2},
        {"it_seed_volcano", it_seed_volcano, itf_static, 0.2}
    };

    BOOT_REGISTER_START
        BootRegister(new SeedItem(0), "it_seed");
        BootRegister(new SeedItem(0), "it_seed_wood");
        BootRegister(new SeedItem(1), "it_seed_greenbrown");
        BootRegister(new SeedItem(2), "it_seed_volcano");
        BootRegister(new SeedItem(3), "it_seed_hay");
    BOOT_REGISTER_END

} // namespace enigma
