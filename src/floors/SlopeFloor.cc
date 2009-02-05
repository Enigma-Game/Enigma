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

#include "floors/SlopeFloor.hh"

#include "errors.hh"
#include "main.hh"
#include "stones.hh"
#include "world.hh"

namespace enigma {
    
    SlopeFloor::SlopeFloor(int slope, std::string shape) : Floor("fl_slope", 4, 2)  {
        state = slope;
        Floor::setAttr("shape", shape);
    }
    
    std::string SlopeFloor::getClass() const {
        return "fl_slope";
    }
        
    void SlopeFloor::setAttr(const string& key, const Value &val) {
        if (key == "shape") {
            std::string shape = val.to_string();
            if (shape == "" || shape == "pw" || shape == "ps" || shape == "pe" || shape == "pn" ||
                    shape == "inw" || shape == "isw" || shape == "ise" || shape == "ine" || 
                    shape == "onw" || shape == "osw" || shape == "ose" || shape == "one" || 
                    shape == "tw" || shape == "ts" || shape == "te" || shape == "tn" || 
                    shape == "twl" || shape == "tsl" || shape == "tel" || shape == "tnl" || 
                    shape == "twr" || shape == "tsr" || shape == "ter" || shape == "tnr") {
                Floor::setAttr("shape", shape);
                init_model();
            }
        } else if (key == "slope") {
            int slope = val;
            if (slope >= -1 && slope <= 7)
                state = slope;
        } else if (key == "strength") {
            var_floorforce[0] = val;
            objFlags = (objFlags & ~OBJBIT_FORCE) | OBJBIT_STRENGTH;
        } else if (key == "force_x") {
            var_floorforce[0] = val;
            objFlags = (objFlags & ~OBJBIT_STRENGTH) | OBJBIT_FORCE;
        } else if (key == "force_y") {
            var_floorforce[1] = val;
            objFlags = (objFlags & ~OBJBIT_STRENGTH) | OBJBIT_FORCE;
        } else
            Floor::setAttr(key, val);
    }
        
    Value SlopeFloor::getAttr(const std::string &key) const {
        if (key == "slope") {
            return state;
        } else if (key == "strength") {
            return var_floorforce[0];
       }
        return Floor::getAttr(key);
    }
    
    void SlopeFloor::setState(int extState) {
        // ignore any state change attempts
    }
    
    void SlopeFloor::init_model()  {
        std::string shape = getAttr("shape").to_string();
        if (shape == "")
            set_model("fl_slope");
        else
            set_model(std::string("fl_slope_") + shape);
    }
    
    void SlopeFloor::add_force(Actor *a, ecl::V2 &f) {
        static int xforce[8] = {
             -1,  0,  1, 0,
             -1, -1,  1, 1
        };
        static int yforce[8] = {
             0,  1,  0,  -1,
             -1,  1, 1, -1
        };
        
        if (objFlags & OBJBIT_FORCE)
            return Floor::add_force(a, f);
        else {
            ecl::V2 force(0,0);
            if (state != NODIR) {
                force = ecl::V2(xforce[state], yforce[state]);
                force.normalize();
            }            
            f += ((objFlags & OBJBIT_STRENGTH) ? var_floorforce[0] : server::SlopeForce) * force;
        }
    }
    

    BOOT_REGISTER_START
        BootRegister(new SlopeFloor(-1, ""), "fl_slope");
        BootRegister(new SlopeFloor( 0, "pw"), "fl_slope_pw");
        BootRegister(new SlopeFloor( 1, "ps"), "fl_slope_ps");
        BootRegister(new SlopeFloor( 2, "pe"), "fl_slope_pe");
        BootRegister(new SlopeFloor( 3, "pn"), "fl_slope_pn");
        BootRegister(new SlopeFloor( 4, "inw"), "fl_slope_inw");
        BootRegister(new SlopeFloor( 5, "isw"), "fl_slope_isw");
        BootRegister(new SlopeFloor( 6, "ise"), "fl_slope_ise");
        BootRegister(new SlopeFloor( 7, "ine"), "fl_slope_ine");
        BootRegister(new SlopeFloor( 4, "onw"), "fl_slope_onw");
        BootRegister(new SlopeFloor( 5, "osw"), "fl_slope_osw");
        BootRegister(new SlopeFloor( 6, "ose"), "fl_slope_ose");
        BootRegister(new SlopeFloor( 7, "one"), "fl_slope_one");
        BootRegister(new SlopeFloor( 0, "tw"), "fl_slope_tw");
        BootRegister(new SlopeFloor( 1, "ts"), "fl_slope_ts");
        BootRegister(new SlopeFloor( 2, "te"), "fl_slope_te");
        BootRegister(new SlopeFloor( 3, "tn"), "fl_slope_tn");
        BootRegister(new SlopeFloor( 4, "twl"), "fl_slope_twl");
        BootRegister(new SlopeFloor( 5, "tsl"), "fl_slope_tsl");
        BootRegister(new SlopeFloor( 6, "tel"), "fl_slope_tel");
        BootRegister(new SlopeFloor( 7, "tnl"), "fl_slope_tnl");
        BootRegister(new SlopeFloor( 5, "twr"), "fl_slope_twr");
        BootRegister(new SlopeFloor( 6, "tsr"), "fl_slope_tsr");
        BootRegister(new SlopeFloor( 7, "ter"), "fl_slope_ter");
        BootRegister(new SlopeFloor( 4, "tnr"), "fl_slope_tnr");
    BOOT_REGISTER_END

} // namespace enigma
