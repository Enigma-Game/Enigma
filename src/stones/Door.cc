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

#include "stones/Door.hh"
#include "errors.hh"
//#include "main.hh"
#include "laser.hh"

namespace enigma {
    Door::Door(std::string flavor, int initState, std::string faces) : Stone () {
        state = initState;
        setAttr("flavor", flavor);
        setAttr("faces", faces);
    }

    std::string Door::getClass() const {
        return "st_door";
    }
        
    void Door::setAttr(const string& key, const Value &val) {
        if (key == "flavor") { 
            Stone::setAttr(key, val);
            if (val.to_string() != "d")
                Stone::setAttr("faces", "nesw");
            if (isDisplayable() && state <= OPEN)
                init_model();    // need to redisplay after attribute set
            return;
        } else if (key == "faces" && getAttr("flavor").to_string() == "d") {
            Stone::setAttr(key, val);
            std::string faces = Stone::getAttr(key).to_string();
            ASSERT(faces == "ns" || faces == "ew", XLevelRuntime, "Door type d supports only face 'ns' and 'ew'");
        }
        Stone::setAttr(key, val);
    }
    
    Value Door::message(const Message &m) {
        if (m.message == "ignite" && getAttr("flavor").to_string() == "c" && server::GameCompatibility == GAMET_OXYD1) {
            if (isDisplayable())
                KillStone(get_pos());  // TODO animation & sound
            return Value();
        } else
            return Stone::message(m);
    }

    int Door::externalState() const {
        return (state == OPEN) ? 1 : 0;
    }
    
    void Door::setState(int extState) {
        if (isDisplayable()) {
            if (extState == 1 && (state == CLOSED || state == CLOSING))
                set_iState(OPENING);
            else if (extState == 0 && (state == OPEN || state == OPENING))
                set_iState(CLOSING);
        } else
            state = extState;
    }
    
    void Door::toggleState() {
        set_iState((state == OPEN || state == OPENING) ? CLOSING : OPENING);
    }
        
    void Door::init_model() {
        string mname = model_basename();
        if (state == CLOSED)
            mname += "_closed";
        else if (state==OPEN)
            mname += "_open";
        set_model(mname);
    }

    void Door::animcb() {
        if (state == OPENING)
            set_iState(OPEN);
        else if (state == CLOSING)
            set_iState(CLOSED);
    }
    
    bool Door::is_floating() const {
        return true;
    }
    
    bool Door::is_transparent(Direction d) const {
        if (state == OPEN)
            return true;
        else {
            DirectionBits faces = (DirectionBits)(ALL_DIRECTIONS ^ getConnections());
            return !has_dir(faces, d) && !has_dir(faces, reverse(d)); 
        }
    }
    
    bool Door::allowsSpreading(Direction dir, bool isFlood) const {
        return state != CLOSED || (dir != NODIR ? !has_dir(getFaces(), dir) : (getAttr("flavor").to_string() == "d"));
    }
    
    StoneResponse Door::collision_response(const StoneContact &sc) {
        Direction cf = contact_face(sc);
        if (state == OPEN)
            return STONE_PASS;
        else {
            DirectionBits faces = (DirectionBits)(ALL_DIRECTIONS ^ getConnections());
            StoneResponse result = STONE_PASS;
            if ((has_dir(faces, WEST) && (get_pos().x >= sc.actor->get_pos()[0])) ||
                    (has_dir(faces, SOUTH) && (get_pos().y + 1 <= sc.actor->get_pos()[1])) ||
                    (has_dir(faces, EAST) && (get_pos().x + 1 <= sc.actor->get_pos()[0])) ||
                    (has_dir(faces, NORTH) && (get_pos().y >= sc.actor->get_pos()[1])))
                result = STONE_REBOUND;
            return result;
        }
    }
    
    void Door::actor_hit(const StoneContact &sc) {
        if (getAttr("flavor").to_string() == "d") {
            // door knocking
            Item *it = GetItem(get_pos());
            if (it != NULL && server::GameCompatibility != GAMET_PEROXYD 
                    && (server::GameCompatibility != GAMET_ENIGMA || server::EnigmaCompatibility < 1.10 ))
                SendMessage(it, "_hit", sc.actor);
            else
                performAction(sc.actor);
        }
    }
    
    const char *Door::collision_sound() { 
        return (getAttr("flavor").to_string() == "d") ? "electric" : Stone::collision_sound(); 
    }
    
    FreezeStatusBits Door::get_freeze_bits() {
        return FREEZEBIT_NO_STONE;
    }
    
    std::string Door::model_basename() {
        std::string flavor = getAttr("flavor").to_string();
        if (flavor != "d")
            return std::string("st_door_") + flavor;
        else
            return (getConnections() == (WESTBIT | EASTBIT)) ? "st_door_d_ns" : "st_door_d_ew";
    }
    
    void Door::set_iState(int newState) {
        if (isDisplayable()) {
            std::string basename = model_basename();
            switch (newState) {
                case OPEN:
                    set_model(basename+"_open");
                    MaybeRecalcLight(get_pos());
                    SendMessage(GetFloor(get_pos()), "_checkflood");
                    for (Direction d = NORTH; d != NODIR; d = previous(d))
                        SendMessage(GetFloor(move(get_pos(), d)), "_checkflood");
                    break;
                case CLOSED:
                    set_model(basename+"_closed");
                    ShatterActorsInsideField (get_pos());
                    MaybeRecalcLight(get_pos()); // maybe superfluous
                    break;
                case OPENING:
                    sound_event((getAttr("flavor").to_string() == "d") ? "dooropen" : "");
                    if (state == CLOSING)
                        get_model()->reverse();
                    else
                        set_anim(basename+"_opening");
                    break;
                case CLOSING:
                    sound_event((getAttr("flavor").to_string() == "d") ? "doorclose" : "");
                    if (state == OPENING)
                        get_model()->reverse();
                    else
                        set_anim(basename+"_closing");
                    ShatterActorsInsideField (get_pos());
                    MaybeRecalcLight(get_pos());
                    break;
            }
        }
        state = newState;
    }

    
    DEF_TRAITS(Door, "st_door", st_door);
    
    BOOT_REGISTER_START
        BootRegister(new Door("d", 0, "ns"), "st_door");
        BootRegister(new Door("d", 0, "ns"), "st_door_d");
        BootRegister(new Door("a", 0), "st_door_a");
        BootRegister(new Door("b", 0), "st_door_b");
        BootRegister(new Door("c", 0), "st_door_c");
        BootRegister(new Door("c", 1), "st_door_c_open");
        BootRegister(new Door("d", 1, "ns"), "st_door_d_open");
        BootRegister(new Door("d", 0, "ew"), "st_door_d_ew");
        BootRegister(new Door("d", 1, "ew"), "st_door_d_ew_open");
    BOOT_REGISTER_END

} // namespace enigma
