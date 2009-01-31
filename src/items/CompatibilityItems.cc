/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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

#include "items/CompatibilityItems.hh"
//#include "errors.hh"
//#include "main.hh"
#include "server.hh"
#include "world.hh"

namespace enigma {

/* -------------------- EasyKeepStone -------------------- */

    EasyKeepStone::EasyKeepStone() {
    }
    
    Value EasyKeepStone::message(const Message &m) {
        if (m.message == "_init") {
            // does not work in on_creation() because items are created
            // before stones are created.
            if (server::GetDifficulty() == DIFFICULTY_HARD)
                KillStone(get_pos());
            kill();
            return Value();
        }
        return Item::message(m);
    }

    DEF_ITEMTRAITSF(EasyKeepStone, "it-easykeepstone", it_easykeepstone,  itf_invisible | itf_fireproof);

/* -------------------- EasyKillStone -------------------- */

/*
This item is never visible during the game.  Its only purpose is to
modify the level if the difficulty mode is set to easy *before* the
game starts.

In easy game mode this item removes the stone at its position.  Then
in both modes it kills itself.

E.g. it can be used to hide walls in easy game mode.

\ref st-easymode
*/

    EasyKillStone::EasyKillStone() {
    }
    
    Value EasyKillStone::message(const Message &m) {
        if (m.message == "_init") {
            // does not work in on_creation() because items are created
            // before stones are created.
            if (server::GetDifficulty() == DIFFICULTY_EASY) {
                if (Stone *st = GetStone (get_pos())) {
                    if (st->is_kind ("st_death") ||
                        st->is_kind ("st_flash") ||
                        st->is_kind ("st_thief"))
                    {
                        SetStone (get_pos(), MakeStone ("st_flat"));
                    }
                    else
                        KillStone(get_pos());
                }
            }
            kill();
            return Value();
        }
        return Item::message(m);
    }
    
    DEF_ITEMTRAITSF(EasyKillStone, "it-easykillstone", it_easykillstone, itf_invisible | itf_fireproof);

/* -------------------- SingleKillStone -------------------- */

    OnePKillStone::OnePKillStone() {
    }
    
    Value OnePKillStone::message(const Message &m) {
        if (m.message == "_init") {
            if (server::SingleComputerGame)
                KillStone (get_pos());
            kill();
            return Value();
        }
        return Item::message(m);
    }
    
    DEF_ITEMTRAITSF(OnePKillStone, "it-1pkillstone", it_1pkillstone, itf_invisible | itf_fireproof);

/* -------------------- NetworkKillStone -------------------- */

    TwoPKillStone::TwoPKillStone() {
    }
  
    Value TwoPKillStone::message(const Message &m) {
        if (m.message == "_init") {
            if (!server::SingleComputerGame)
                KillStone (get_pos());
            kill();
            return Value();
        }
        return Item::message(m);
    }

    DEF_ITEMTRAITSF(TwoPKillStone, "it-2pkillstone", it_2pkillstone, itf_invisible | itf_fireproof);

//----------------------------------------
// Bridge item (for Oxyd compatibility)
//
// Floor tiles seem to be static in Oxyd and cannot change dynamically
// or be animated.  For this reason, every bridge floor in Oxyd has to
// be combined with a bridge "item" that receives the signals, shows
// the animation and sets or removes the floor.
//----------------------------------------

    OxydBridge::OxydBridge() {
    }

    Value OxydBridge::message(const Message &m) {
        if (m.message == "signal") {
            int ival = m.value;
            Floor *floor = GetFloor (get_pos());
            if (ival > 0)
                SendMessage (floor, "close");
            else
                SendMessage (floor, "open");
            return Value();
        }
        return Item::message(m);
    }
    
    DEF_ITEMTRAITSF(OxydBridge, "it-bridge-oxyd", it_bridge_oxyd,
            itf_static | itf_indestructible | itf_invisible | itf_fireproof);

/* -------------------- OxydBridgeActive -------------------- */
    OxydBridgeActive::OxydBridgeActive() {
    }
    
    void OxydBridgeActive::on_creation (GridPos p) {
        Floor *floor = GetFloor (p);
        SendMessage (floor, "close");
    }

    DEF_ITEMTRAITSF(OxydBridgeActive, "it-bridge-oxyd_active", it_bridge_oxyd_active,
            itf_static | itf_indestructible | itf_invisible | itf_fireproof);

    BOOT_REGISTER_START
        BootRegister(new EasyKeepStone(), "it-easykeepstone");
        BootRegister(new EasyKillStone(), "it-easykillstone");
        BootRegister(new OnePKillStone(), "it-1pkillstone");
        BootRegister(new TwoPKillStone(), "it-2pkillstone");
        BootRegister(new OxydBridge(), "it-bridge-oxyd");
        BootRegister(new OxydBridgeActive(), "it-bridge-oxyd_active");
    BOOT_REGISTER_END

} // namespace enigma

