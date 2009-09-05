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

#include "floors/StandardFloors.hh"

//#include "errors.hh"
#include "main.hh"
#include "world.hh"

namespace enigma {
    /**
     * This is a temporary workaround that allows standard floors to support objects.xml
     * and methods like isKind(). This mapping should vanish when floors and all other objects
     * do support a common flexible traits concept.
     */
    static struct StandardFloorNames {
        std::string classname;
        std::string submodel;
    } StandardFloorNames[] = {
        {"fl_adhesionless"},
        {"fl_aquamarine"},
        {"fl_bast"},
        {"fl_bluegray"},
        {"fl_bluegreen"},
        {"fl_blueslab"},
        {"fl_brick"},
        {"fl_bright"},
        {"fl_concrete"},
        {"fl_dark"},
        {"fl_darkgray"},
        {"fl_dunes"},
        {"fl_gravel"},
        {"fl_gray"},
        {"fl_himalaya"},
        {"fl_ivory"},
        {"fl_lawn"},
        {"fl_lawn", "fl_lawn_a"},
        {"fl_lawn", "fl_lawn_b"},
        {"fl_lawn", "fl_lawn_c1"},
        {"fl_lawn", "fl_lawn_c2"},
        {"fl_lawn", "fl_lawn_c3"},
        {"fl_lawn", "fl_lawn_c4"},
        {"fl_lawn", "fl_lawn_d1"},
        {"fl_lawn", "fl_lawn_d2"},
        {"fl_lawn", "fl_lawn_d3"},
        {"fl_lawn", "fl_lawn_d4"},
        {"fl_lawn", "fl_lawn_e1"},
        {"fl_lawn", "fl_lawn_e2"},
        {"fl_lawn", "fl_lawn_e3"},
        {"fl_lawn", "fl_lawn_e4"},
        {"fl_marble"},
        {"fl_metal"},
        {"fl_metal", "fl_metal_1"},
        {"fl_metal", "fl_metal_2"},
        {"fl_metal", "fl_metal_3"},
        {"fl_metal", "fl_metal_4"},
        {"fl_metal", "fl_metal_5"},
        {"fl_metal", "fl_metal_6"},
        {"fl_metal", "fl_metal_7"},
        {"fl_metal", "fl_metal_7"},  // fl_metal_7n
        {"fl_mortar"},
        {"fl_pinkbumps"},
        {"fl_plank"},
        {"fl_platinum"},
        {"fl_red"},
        {"fl_redslab"},
        {"fl_rough"},
        {"fl_sahara"},
        {"fl_samba"},
        {"fl_samba", "fl_samba_h"},
        {"fl_samba", "fl_samba_v"},
        {"fl_sand"},
        {"fl_stone"},
        {"fl_tigris"},
        {"fl_white"},
        {"fl_woven"}
    };
    
    StandardFloor::StandardFloor(int type, bool framed) : Floor(StandardFloorNames[type].classname.c_str()) {
        state = type;
        Floor::setAttr("faces", framed ? "nesw" : "");
        if  (type == 40) { // fl_metal_7n -- legacy normal floor with special values
            setAttr("friction", 4.0);
            setAttr("adhesion", 2.0);
        }
    }
    
    std::string StandardFloor::getClass() const {
        return StandardFloorNames[state].classname;
    }
    
    int StandardFloor::externalState() const {
        return 0;   // no external states
    }
    
    void StandardFloor::setState(int extState) {
        // no external states
    }
    
    std::string StandardFloor::getModelName() const {
        bool isFramed = (getAttr("faces").to_string() == "nesw");
        return ((StandardFloorNames[state].submodel.length() > 0) ? StandardFloorNames[state].submodel :
                StandardFloorNames[state].classname) +  (isFramed ? "_framed" : "");
    }
    
    BOOT_REGISTER_START
        BootRegister(new StandardFloor( 0), "fl_adhesionless");
        BootRegister(new StandardFloor( 1), "fl_aquamarine");
        BootRegister(new StandardFloor( 1, true), "fl_aquamarine_framed");
        BootRegister(new StandardFloor( 2), "fl_bast");
        BootRegister(new StandardFloor( 2, true), "fl_bast_framed");
        BootRegister(new StandardFloor( 3), "fl_bluegray");
        BootRegister(new StandardFloor( 3, true), "fl_bluegray_framed");
        BootRegister(new StandardFloor( 4), "fl_bluegreen");
        BootRegister(new StandardFloor( 4, true), "fl_bluegreen_framed");
        BootRegister(new StandardFloor( 5), "fl_blueslab");
        BootRegister(new StandardFloor( 5, true), "fl_blueslab_framed");
        BootRegister(new StandardFloor( 6), "fl_brick");
        BootRegister(new StandardFloor( 6, true), "fl_brick_framed");
        BootRegister(new StandardFloor( 7), "fl_bright");
        BootRegister(new StandardFloor( 7, true), "fl_bright_framed");
        BootRegister(new StandardFloor( 8), "fl_concrete");
        BootRegister(new StandardFloor( 8, true), "fl_concrete_framed");
        BootRegister(new StandardFloor( 9), "fl_dark");
        BootRegister(new StandardFloor( 9, true), "fl_dark_framed");
        BootRegister(new StandardFloor(10), "fl_darkgray");
        BootRegister(new StandardFloor(10, true), "fl_darkgray_framed");
        BootRegister(new StandardFloor(11), "fl_dunes");
        BootRegister(new StandardFloor(11, true), "fl_dunes_framed");
        BootRegister(new StandardFloor(12), "fl_gravel");
        BootRegister(new StandardFloor(12, true), "fl_gravel_framed");
        BootRegister(new StandardFloor(13), "fl_gray");
        BootRegister(new StandardFloor(13, true), "fl_gray_framed");
        BootRegister(new StandardFloor(14), "fl_himalaya");
        BootRegister(new StandardFloor(14, true), "fl_himalaya_framed");
        BootRegister(new StandardFloor(15), "fl_ivory");
        BootRegister(new StandardFloor(15, true), "fl_ivory_framed");
        BootRegister(new StandardFloor(16), "fl_lawn");
        BootRegister(new StandardFloor(17), "fl_lawn_a");
        BootRegister(new StandardFloor(18), "fl_lawn_b");
        BootRegister(new StandardFloor(19), "fl_lawn_c1");
        BootRegister(new StandardFloor(20), "fl_lawn_c2");
        BootRegister(new StandardFloor(21), "fl_lawn_c3");
        BootRegister(new StandardFloor(22), "fl_lawn_c4");
        BootRegister(new StandardFloor(23), "fl_lawn_d1");
        BootRegister(new StandardFloor(24), "fl_lawn_d2");
        BootRegister(new StandardFloor(25), "fl_lawn_d3");
        BootRegister(new StandardFloor(26), "fl_lawn_d4");
        BootRegister(new StandardFloor(27), "fl_lawn_e1");
        BootRegister(new StandardFloor(28), "fl_lawn_e2");
        BootRegister(new StandardFloor(29), "fl_lawn_e3");
        BootRegister(new StandardFloor(30), "fl_lawn_e4");
        BootRegister(new StandardFloor(31), "fl_marble");
        BootRegister(new StandardFloor(31, true), "fl_marble_framed");
        BootRegister(new StandardFloor(32), "fl_metal");
        BootRegister(new StandardFloor(32, true), "fl_metal_framed");
        BootRegister(new StandardFloor(33), "fl_metal_1");
        BootRegister(new StandardFloor(33, true), "fl_metal_1_framed");
        BootRegister(new StandardFloor(34), "fl_metal_2");
        BootRegister(new StandardFloor(34, true), "fl_metal_2_framed");
        BootRegister(new StandardFloor(35), "fl_metal_3");
        BootRegister(new StandardFloor(35, true), "fl_metal_3_framed");
        BootRegister(new StandardFloor(36), "fl_metal_4");
        BootRegister(new StandardFloor(36, true), "fl_metal_4_framed");
        BootRegister(new StandardFloor(37), "fl_metal_5");
        BootRegister(new StandardFloor(37, true), "fl_metal_5_framed");
        BootRegister(new StandardFloor(38), "fl_metal_6");
        BootRegister(new StandardFloor(38, true), "fl_metal_6_framed");
        BootRegister(new StandardFloor(39), "fl_metal_7");
        BootRegister(new StandardFloor(39, true), "fl_metal_7_framed");
        BootRegister(new StandardFloor(40), "fl_metal_7n");
        BootRegister(new StandardFloor(41), "fl_mortar");
        BootRegister(new StandardFloor(41, true), "fl_mortar_framed");
        BootRegister(new StandardFloor(42), "fl_pinkbumps");
        BootRegister(new StandardFloor(42, true), "fl_pinkbumps_framed");
        BootRegister(new StandardFloor(43), "fl_plank");
        BootRegister(new StandardFloor(43, true), "fl_plank_framed");
        BootRegister(new StandardFloor(44), "fl_platinum");
        BootRegister(new StandardFloor(44, true), "fl_platinum_framed");
        BootRegister(new StandardFloor(45), "fl_red");
        BootRegister(new StandardFloor(45, true), "fl_red_framed");
        BootRegister(new StandardFloor(46), "fl_redslab");
        BootRegister(new StandardFloor(46, true), "fl_redslab_framed");
        BootRegister(new StandardFloor(47), "fl_rough");
        BootRegister(new StandardFloor(47, true), "fl_rough_framed");
        BootRegister(new StandardFloor(48), "fl_sahara");
        BootRegister(new StandardFloor(48, true), "fl_sahara_framed");
        BootRegister(new StandardFloor(49), "fl_samba");
        BootRegister(new StandardFloor(49, true), "fl_samba_framed");
        BootRegister(new StandardFloor(50), "fl_samba_h");
        BootRegister(new StandardFloor(50, true), "fl_samba_h_framed");
        BootRegister(new StandardFloor(51), "fl_samba_v");
        BootRegister(new StandardFloor(51, true), "fl_samba_v_framed");
        BootRegister(new StandardFloor(52), "fl_sand");
        BootRegister(new StandardFloor(52, true), "fl_sand_framed");
        BootRegister(new StandardFloor(53), "fl_stone");
        BootRegister(new StandardFloor(53, true), "fl_stone_framed");
        BootRegister(new StandardFloor(54), "fl_tigris");
        BootRegister(new StandardFloor(54, true), "fl_tigris_framed");
        BootRegister(new StandardFloor(55), "fl_white");
        BootRegister(new StandardFloor(55, true), "fl_white_framed");
        BootRegister(new StandardFloor(56), "fl_woven");
        BootRegister(new StandardFloor(56, true), "fl_woven_framed");
    BOOT_REGISTER_END

} // namespace enigma
