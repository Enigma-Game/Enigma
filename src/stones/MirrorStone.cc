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

#include "stones/MirrorStone.hh"

//#include "main.hh"
#include "laser.hh"
#include "player.hh"

namespace enigma {

    MirrorStone::MirrorStone(bool isTriangle, bool isMovable, bool paneTransparent, bool sideTransparent, 
            Direction dir, bool counterclock) : Stone() {
        state = dir;
        if (isTriangle)
            objFlags |= OBJBIT_TRIANGLE;
        if (isMovable)
            objFlags |= OBJBIT_MOVABLE;
        if (paneTransparent)
            objFlags |= OBJBIT_PANE;
        if (sideTransparent)
            objFlags |= OBJBIT_SIDE;
        if (counterclock)
            setAttr("counterclock", true);
    }
    
    std::string MirrorStone::getClass() const {
        return "st_mirror";
    }
    
    void MirrorStone::setAttr(const string& key, const Value &val) {
        bool newModel = false;
        bool lightChange = false;
        bool handled = false;
        if (key == "movable") {
            if (val.to_bool() != ((objFlags & OBJBIT_MOVABLE) != 0)) {
                objFlags ^=  OBJBIT_MOVABLE;
                newModel = true;
                handled = true;
            }
        } else if (key == "flavor") {
            if (val.to_string() == "triangle" && !(objFlags & OBJBIT_TRIANGLE)) {
                objFlags |= OBJBIT_TRIANGLE;
                objFlags &= ~OBJBIT_SIDE;
                newModel = true;
                lightChange = true;
                handled = true;
            } else if (val.to_string() == "slab" && ((objFlags & OBJBIT_TRIANGLE) || (objFlags & OBJBIT_SIDE))) {
                objFlags &= ~OBJBIT_TRIANGLE;
                objFlags &= ~OBJBIT_SIDE;
                newModel = true;
                lightChange = true;
                handled = true;
            } else if (val.to_string() == "sheets" && ((objFlags & OBJBIT_TRIANGLE) || !(objFlags & OBJBIT_SIDE))) {
                objFlags &= ~OBJBIT_TRIANGLE;
                objFlags |= OBJBIT_SIDE;
                objFlags |= OBJBIT_PANE;  // sheets are always semi transparent
                newModel = true;
                lightChange = true;
                handled = true;
            }
        } else if (key == "orientation") {
            int orientation = val;
            // random orientation
            if (orientation == -2)
                orientation = IntegerRand(minState(), maxState());
            
            if (orientation >= minState() && orientation <= maxState()) {
                setState(orientation);
                handled = true;
            }
        } else if (key == "transparent") {
            if ((val.to_bool() != ((objFlags & OBJBIT_PANE) != 0)) && !(objFlags & OBJBIT_SIDE)) {
                objFlags ^=  OBJBIT_PANE;
                newModel = true;
                lightChange = true;
                handled = true;
            }
        }
        
        if (isDisplayable()) {
            if (newModel)
                init_model();
            if (lightChange)
                RecalcLight();
        }
        
        if (!handled)
            Stone::setAttr(key, val);
    }
    
    Value MirrorStone::getAttr(const std::string &key) const {
        if (key == "movable") {
            return (bool)(objFlags & OBJBIT_MOVABLE);
        } else if (key == "flavor") {
            return (objFlags & OBJBIT_TRIANGLE) ? "triangle" : ((objFlags & OBJBIT_SIDE) ? "sheets" : "slab");
        } else if (key == "orientation") {
            return state;
        } else if (key == "transparent") {
            return (bool)(objFlags & OBJBIT_PANE);
        }
        return Stone::getAttr(key);
    }
    
    Value MirrorStone::message(const Message &m) {
        if (m.message == "orientate") {
            setAttr("state", m.value);   // enforce value check
            return Value();
        } else if ((m.message == "turn" || m.message == "turnback")) {
            setAttr("state", ((m.message == "turn" && !getAttr("counterclock").to_bool()) || 
                              (m.message == "turnback" && getAttr("counterclock").to_bool()))
                    ? rotate_cw((Direction)state) : rotate_ccw((Direction)state));
            return Value();
        } else if (m.message == "signal") {
            if (m.value == 1)
                toggleState();
            else if (m.value == 0 && server::GameCompatibility != GAMET_ENIGMA)
                // rotate mirror in opposite direction
                setState(!getAttr("counterclock").to_bool() ? rotate_ccw((Direction)state) : rotate_cw((Direction)state));
            return Value();
        } else if (m.message == "_trigger") {
            if (m.value == 1)
                toggleState();                
            return Value();
        }
        return Stone::message(m);
    }
    

    int MirrorStone::maxState() const {
        return 3;  // dir representing state
    }

    void MirrorStone::setState(int extState) {
        // bool changed = (extState != state);
        
        state = extState;
        
        if (isDisplayable()) {
            init_model();
            RecalcLight();
            sound_event ("mirrorturn");
        }
     }
    
    void MirrorStone::toggleState() {
        setState(!getAttr("counterclock").to_bool() ? rotate_cw((Direction)state) : rotate_ccw((Direction)state));
    }

    void MirrorStone::init_model()  {
        // the models are still encoded in the old API notation
        set_model(ecl::strf("st-%cmirror-%c%c%d", 
                (objFlags & OBJBIT_TRIANGLE) ? '3' : 'p',
                (objFlags & OBJBIT_MOVABLE)  ? 'm' : 's',
                (objFlags & OBJBIT_PANE) ? ((objFlags & OBJBIT_SIDE) ? 'f' : 't') : 'o',
                ((5 - state) % 4) + 1));
    }
    
    void MirrorStone::on_creation(GridPos p) {
        activatePhoto();
        Stone::on_creation(p);
    }
        
    void MirrorStone::processLight(Direction d) {
        DirectionBits oldOutDirs = emissionDirections();
        Stone::processLight(d);
        DirectionBits newOutDirs = emissionDirections();
        if (oldOutDirs != newOutDirs) {
            // emit light to new dirs
            for (Direction newDir = WEST; newDir != NODIR; newDir = next(newDir)) {
                 DirectionBits newDirBit = to_bits(newDir);
                 if (!(oldOutDirs & newDirBit) && (newOutDirs & newDirBit))
                    LaserBeam::emit_from(get_pos(), newDir);
            }
        }
    }
    
    DirectionBits MirrorStone::emissionDirections() const {
        static uint16_t outDirs[] = {
            ((WESTBIT)<<12) + ((SOUTHBIT)<<8) + ((EASTBIT)<<4) + (NORTHBIT),   // no side, no pane, planar, '\'
            ((SOUTHBIT)<<12) + ((NODIRBIT)<<8) + ((NORTHBIT)<<4) + (NODIRBIT), // no side, no pane, planar, '-'
            ((EASTBIT)<<12) + ((NORTHBIT)<<8) + ((WESTBIT)<<4) + (SOUTHBIT),   // no side, no pane, planar, '/'
            ((NODIRBIT)<<12) + ((WESTBIT)<<8) + ((NODIRBIT)<<4) + (EASTBIT),   // no side, no pane, planar, '|'
            ((WESTBIT)<<12) + ((NORTHBIT+SOUTHBIT)<<8) + ((WESTBIT)<<4) + (EASTBIT),    // no side, no pane, triangle,'<'
            ((WESTBIT+EASTBIT)<<12) + ((SOUTHBIT)<<8) + ((NORTHBIT)<<4) + (SOUTHBIT),   // no side, no pane, triangle,'v'
            ((EASTBIT)<<12) + ((WESTBIT)<<8) + ((EASTBIT)<<4) + (NORTHBIT+SOUTHBIT),    // no side, no pane, triangle,'>'
            ((SOUTHBIT)<<12) + ((NORTHBIT)<<8) + ((WESTBIT+EASTBIT)<<4) + (NORTHBIT),   // no side, no pane, triangle,'^'
            ((WESTBIT+NORTHBIT)<<12) + ((SOUTHBIT+EASTBIT)<<8) + ((EASTBIT+SOUTHBIT)<<4) + (NORTHBIT+WESTBIT),   // no side, pane, planar, '\'
            ((SOUTHBIT+NORTHBIT)<<12) + ((NODIRBIT)<<8) + ((NORTHBIT+SOUTHBIT)<<4) + (NODIRBIT), // no side, pane, planar, '-'
            ((EASTBIT+NORTHBIT)<<12) + ((NORTHBIT+EASTBIT)<<8) + ((WESTBIT+SOUTHBIT)<<4) + (SOUTHBIT+WESTBIT),   // no side, pane, planar, '/'
            ((NODIRBIT)<<12) + ((WESTBIT+EASTBIT)<<8) + ((NODIRBIT)<<4) + (EASTBIT+WESTBIT),   // no side, pane, planar, '|'
            ((WESTBIT+NORTHBIT)<<12) + ((NORTHBIT+SOUTHBIT+EASTBIT)<<8) + ((WESTBIT+SOUTHBIT)<<4) + (EASTBIT+WESTBIT),     // no side, pane, triangle,'<'
            ((WESTBIT+EASTBIT+NORTHBIT)<<12) + ((SOUTHBIT+EASTBIT)<<8) + ((NORTHBIT+SOUTHBIT)<<4) + (SOUTHBIT+WESTBIT),   // no side, pane, triangle,'v'
            ((EASTBIT+NORTHBIT)<<12) + ((WESTBIT+EASTBIT)<<8) + ((EASTBIT+SOUTHBIT)<<4) + (NORTHBIT+SOUTHBIT+WESTBIT),    // no side, pane, triangle,'>'
            ((SOUTHBIT+NORTHBIT)<<12) + ((NORTHBIT+EASTBIT)<<8) + ((WESTBIT+EASTBIT+SOUTHBIT)<<4) + (NORTHBIT+WESTBIT),   // no side, pane, triangle,'^'
            0,0,0,0,  // side, no pane, planar
            0,0,0,0,  // side, no pane, triangle
            ((WESTBIT+NORTHBIT)<<12) + ((SOUTHBIT+EASTBIT)<<8) + ((EASTBIT+SOUTHBIT)<<4) + (NORTHBIT+WESTBIT),   // side, pane, planar, '\'
            ((SOUTHBIT+NORTHBIT)<<12) + ((EASTBIT)<<8) + ((NORTHBIT+SOUTHBIT)<<4) + (WESTBIT), // side, pane, planar, '-'
            ((EASTBIT+NORTHBIT)<<12) + ((NORTHBIT+EASTBIT)<<8) + ((WESTBIT+SOUTHBIT)<<4) + (SOUTHBIT+WESTBIT),   // side, pane, planar, '/'
            ((NORTHBIT)<<12) + ((WESTBIT+EASTBIT)<<8) + ((SOUTHBIT)<<4) + (EASTBIT+WESTBIT),   // side, pane, planar, '|'
            0,0,0,0  // side, pane, triangle
        };
        int index = ((objFlags>>22) & 0x1C) | state; // 5 bit index: OBJBIT_SIDE - OBJBIT_PANE - OBJBIT_TRIANGLE - 2bit orientation
        uint16_t outCandidates = outDirs[index];
        DirectionBits result = NODIRBIT;
        if (objFlags & NORTHBIT)
            result = (DirectionBits)(result | ((outCandidates>>12) & ALL_DIRECTIONS));
        if (objFlags & EASTBIT)
            result = (DirectionBits)(result | ((outCandidates>>8) & ALL_DIRECTIONS));
        if (objFlags & SOUTHBIT)
            result = (DirectionBits)(result | ((outCandidates>>4) & ALL_DIRECTIONS));
        if (objFlags & WESTBIT)
            result = (DirectionBits)(result | ((outCandidates) & ALL_DIRECTIONS));
        
        return result;
    }

    void MirrorStone::actor_hit(const StoneContact &sc) {
        if (objFlags & OBJBIT_MOVABLE)
            maybe_push_stone(sc);
        if (player::WieldedItemIs (sc.actor, "it_wrench")) 
            setState(!getAttr("counterclock").to_bool() ? rotate_ccw((Direction)state) : rotate_cw((Direction)state));
        else
            toggleState();
    }

    int MirrorStone::traitsIdx() const {
        return (objFlags & OBJBIT_MOVABLE) ? 1 : 0;
    }

    StoneTraits MirrorStone::traits[2] = {
        {"st_mirror", st_mirror, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_mirror", st_mirror, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
    };
    BOOT_REGISTER_START
        BootRegister(new MirrorStone(false, false, false, false, NORTH), "st_mirror");
        BootRegister(new MirrorStone(false, false, false, false, NORTH), "st_mirror_slab");
        BootRegister(new MirrorStone(true, false, false, false,  NORTH), "st_mirror_triangle");
        BootRegister(new MirrorStone(false, false, true, true, NORTH), "st_mirror_sheets");
        BootRegister(new MirrorStone(false, false, false, false, NORTH), "st_mirror_slab_n");
        BootRegister(new MirrorStone(false, false, false, false, EAST), "st_mirror_slab_e");
        BootRegister(new MirrorStone(false, false, false, false, SOUTH), "st_mirror_slab_s");
        BootRegister(new MirrorStone(false, false, false, false, WEST), "st_mirror_slab_w");
        BootRegister(new MirrorStone(false, false, true, false, NORTH), "st_mirror_slab_nt");
        BootRegister(new MirrorStone(false, false, true, false, EAST), "st_mirror_slab_et");
        BootRegister(new MirrorStone(false, false, true, false, SOUTH), "st_mirror_slab_st");
        BootRegister(new MirrorStone(false, false, true, false, WEST), "st_mirror_slab_wt");
        BootRegister(new MirrorStone(false, true, false, false, NORTH), "st_mirror_slab_nm");
        BootRegister(new MirrorStone(false, true, false, false, EAST), "st_mirror_slab_em");
        BootRegister(new MirrorStone(false, true, false, false, SOUTH), "st_mirror_slab_sm");
        BootRegister(new MirrorStone(false, true, false, false, WEST), "st_mirror_slab_wm");
        BootRegister(new MirrorStone(false, true, true, false, NORTH), "st_mirror_slab_ntm");
        BootRegister(new MirrorStone(false, true, true, false, EAST), "st_mirror_slab_etm");
        BootRegister(new MirrorStone(false, true, true, false, SOUTH), "st_mirror_slab_stm");
        BootRegister(new MirrorStone(false, true, true, false, WEST), "st_mirror_slab_wtm");
        BootRegister(new MirrorStone(true, false, false, false, NORTH), "st_mirror_triangle_n");
        BootRegister(new MirrorStone(true, false, false, false, EAST), "st_mirror_triangle_e");
        BootRegister(new MirrorStone(true, false, false, false, SOUTH), "st_mirror_triangle_s");
        BootRegister(new MirrorStone(true, false, false, false, WEST), "st_mirror_triangle_w");
        BootRegister(new MirrorStone(true, false, true, false, NORTH), "st_mirror_triangle_nt");
        BootRegister(new MirrorStone(true, false, true, false, EAST), "st_mirror_triangle_et");
        BootRegister(new MirrorStone(true, false, true, false, SOUTH), "st_mirror_triangle_st");
        BootRegister(new MirrorStone(true, false, true, false, WEST), "st_mirror_triangle_wt");
        BootRegister(new MirrorStone(true, true, false, false, NORTH), "st_mirror_triangle_nm");
        BootRegister(new MirrorStone(true, true, false, false, EAST), "st_mirror_triangle_em");
        BootRegister(new MirrorStone(true, true, false, false, SOUTH), "st_mirror_triangle_sm");
        BootRegister(new MirrorStone(true, true, false, false, WEST), "st_mirror_triangle_wm");
        BootRegister(new MirrorStone(true, true, true, false, NORTH), "st_mirror_triangle_ntm");
        BootRegister(new MirrorStone(true, true, true, false, EAST), "st_mirror_triangle_etm");
        BootRegister(new MirrorStone(true, true, true, false, SOUTH), "st_mirror_triangle_stm");
        BootRegister(new MirrorStone(true, true, true, false, WEST), "st_mirror_triangle_wtm");
        BootRegister(new MirrorStone(true, false, false, false, NORTH, true), "st_mirror_triangle_nc");
        BootRegister(new MirrorStone(true, false, false, false, EAST, true), "st_mirror_triangle_ec");
        BootRegister(new MirrorStone(true, false, false, false, SOUTH, true), "st_mirror_triangle_sc");
        BootRegister(new MirrorStone(true, false, false, false, WEST, true), "st_mirror_triangle_wc");
        BootRegister(new MirrorStone(true, false, true, false, NORTH, true), "st_mirror_triangle_ntc");
        BootRegister(new MirrorStone(true, false, true, false, EAST, true), "st_mirror_triangle_etc");
        BootRegister(new MirrorStone(true, false, true, false, SOUTH, true), "st_mirror_triangle_stc");
        BootRegister(new MirrorStone(true, false, true, false, WEST, true), "st_mirror_triangle_wtc");
        BootRegister(new MirrorStone(true, true, false, false, NORTH, true), "st_mirror_triangle_nmc");
        BootRegister(new MirrorStone(true, true, false, false, EAST, true), "st_mirror_triangle_emc");
        BootRegister(new MirrorStone(true, true, false, false, SOUTH, true), "st_mirror_triangle_smc");
        BootRegister(new MirrorStone(true, true, false, false, WEST, true), "st_mirror_triangle_wmc");
        BootRegister(new MirrorStone(true, true, true, false, NORTH, true), "st_mirror_triangle_ntmc");
        BootRegister(new MirrorStone(true, true, true, false, EAST, true), "st_mirror_triangle_etmc");
        BootRegister(new MirrorStone(true, true, true, false, SOUTH, true), "st_mirror_triangle_stmc");
        BootRegister(new MirrorStone(true, true, true, false, WEST, true), "st_mirror_triangle_wtmc");
    BOOT_REGISTER_END

} // namespace enigma
