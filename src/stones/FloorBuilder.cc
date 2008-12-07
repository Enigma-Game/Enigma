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

#include "stones/FloorBuilder.hh"
#include "errors.hh"
//#include "main.hh"

namespace enigma {
    FloorBuilder::FloorBuilder(int subtyp, int initState) : Stone() {
        objFlags |= (subtyp << 25);
        if (subtyp == ROCK)
            objFlags |= OBJBIT_BLOCKFIRE;
        state = initState;
    }
    
    FloorBuilder* FloorBuilder::clone() {
        if ((FloorBuilderTyp)((objFlags & OBJBIT_SUBTYP) >> 25) == WOOD) { 
             // When st_wood is created it randomly becomes st_wood1 or st_wood2.
             if (IntegerRand(0, 1) == 0)
                return new FloorBuilder(WOOD1, state);
            else
                return new FloorBuilder(WOOD2, state);
        } else {
            return new FloorBuilder(*this);
        }
    }
    
    void FloorBuilder::dispose() {
        delete this;
    }
    
    std::string FloorBuilder::getClass() const {
        FloorBuilderTyp typ = (FloorBuilderTyp)((objFlags & OBJBIT_SUBTYP) >> 25);
        switch (typ) {
            case WOOD:
            case WOOD1:
            case WOOD2:
                return "st_wood";
            case HAY:
                return "st_flhay";
            case ROCK:
                return "st_flrock";
        }
    }
    
    const char *FloorBuilder::get_kind() const {
        FloorBuilderTyp typ = (FloorBuilderTyp)((objFlags & OBJBIT_SUBTYP) >> 25);
        switch (typ) {
            case WOOD:
                return "st_wood";
            case WOOD1:
                return "st_wood_h";
            case WOOD2:
                return "st_wood_v";
            case HAY:
                return "st_flhay";
            case ROCK:
                return "st_flrock";
        }
    }

    Value FloorBuilder::message(const Message &m) {
        if (m.message == "fire" && !(objFlags & OBJBIT_BLOCKFIRE)) {
            KillStone(get_pos());
            return true;  // allow fire to spread
        } else if (m.message == "heat") {
            return (objFlags & OBJBIT_BLOCKFIRE) != 0;  // block fire
        } else if (m.message == "fall") {
            maybe_fall_or_stopfire();
            return Value();
        }
        return Stone::message(m);
    }

    void FloorBuilder::setState(int extState) {
        // no external states
    }
    
    void FloorBuilder::init_model() {
        FloorBuilderTyp typ = (FloorBuilderTyp)((objFlags & OBJBIT_SUBTYP) >> 25);
        switch (typ) {
            case WOOD:
                ASSERT(false, XLevelRuntime, "Wood stone init model state error");
            case WOOD1:
                if (state == GROWING)
                    set_anim("st-wood-growing");
                else
                    set_model("st-wood1");
                break;
            case WOOD2:
                if (state == GROWING)
                    set_anim("st-wood-growing");
                else
                    set_model("st-wood2");
                break;
            case HAY:
                set_model("st-flhay"); break;
                break;
            case ROCK:
                set_model("st-flrock"); break;
        }
    }
    
    void FloorBuilder::animcb() {
        state = IDLE;
        init_model();
        maybe_fall_or_stopfire(); // instantly builds a bridge on fl_swamp etc
    }

    void FloorBuilder::actor_hit(const StoneContact &sc) {
        if (state == GROWING)
            SendMessage(sc.actor, "shatter");
        else
            Stone::actor_hit(sc);
    }
    
    void FloorBuilder::actor_inside(Actor *a) {
        if (state == GROWING)
            SendMessage(a, "shatter");
    }
    
    void FloorBuilder::actor_contact(Actor *a) {
        if (state == GROWING)
            SendMessage(a, "shatter");
    }
    
    void FloorBuilder::on_move() {
        // in oxyd1 only fall when moving
        Stone::on_move();
        maybe_fall_or_stopfire(true);
    }
    
    void FloorBuilder::on_floor_change() {
        // other oxyds versions: fall everytime the floor changes
        maybe_fall_or_stopfire(false, true);
    }
    
    void FloorBuilder::maybe_fall_or_stopfire(bool onMove, bool onFloorChange) {
        FloorBuilderTyp typ = (FloorBuilderTyp)((objFlags & OBJBIT_SUBTYP) >> 25);
        GridPos p = get_pos();
        if (server::GameCompatibility != GAMET_ENIGMA && IsLevelBorder(p))
            return;
        if (Floor *fl = GetFloor(p)) {
            const std::string &k = fl->get_kind();
            if (objFlags & OBJBIT_BLOCKFIRE)
                SendMessage(fl, "stopfire");
            if (k == "fl_abyss" || k == "fl_water" || k == "fl_swamp") {
                if (onMove)
                    // just mark - can not kill stone yet - this will be done on floor change event
                    state = FALLING;  // keep the stone from moving any longer
                else if ((server::GameCompatibility != GAMET_OXYD1) || state == FALLING || !onFloorChange) {
                    state = FALLEN;
                    Floor *fl = MakeFloor((typ == HAY) ? "fl-hay" : ((typ == ROCK) ? "fl-rock" : ((typ == WOOD1) ? "fl-stwood1" : "fl-stwood2")));
                    transferIdentity(fl);
                    SetFloor(p, fl); 
                    KillStone(p);
                }
            }
        }
    }
    
    int FloorBuilder::traitsIdx() const {
        return (state == IDLE) ? 1 : 0;
    }

    StoneTraits FloorBuilder::traits[2] = {
        {"st_floorbuilder", st_floorbuilder, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_floorbuilder", st_floorbuilder, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
    };

    BOOT_REGISTER_START
        BootRegister(new FloorBuilder(0), "st_wood");
        BootRegister(new FloorBuilder(0, 1), "st_wood_growing");
        BootRegister(new FloorBuilder(1), "st_wood_h");
        BootRegister(new FloorBuilder(2), "st_wood_v");
        BootRegister(new FloorBuilder(3), "st_flhay");
        BootRegister(new FloorBuilder(4), "st_flrock");
    BOOT_REGISTER_END

} // namespace enigma
