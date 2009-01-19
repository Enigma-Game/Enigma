/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2008 Andreas Lochmann
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

#include "stones/SimpleStones.hh"
#include "errors.hh"
//#include "main.hh"
#include "player.hh"
#include "server.hh"
#include "world.hh"

namespace enigma {

/* -------------------- Blur stone -------------------- */
    BlurStone::BlurStone(int type) : Stone ("st_blur") {
        state = type;
    }
    
    std::string BlurStone::getClass() const {
        return "st_blur";
    }

    void BlurStone::setAttr(const string& key, const Value &val) {
        if (key == "flavor" && isDisplayable()) {
            std::string fs = val.to_string();
            if (fs == "straight") state = STRAIGHT;
            else if (fs == "cross") state = CROSS;
            else if (fs == "magic") state = MAGIC;
            else ASSERT(false, XLevelRuntime, ecl::strf("Blur stone set known flavor%s", fs.c_str()).c_str());
            return; 
        } else
            Stone::setAttr(key, val);
    }

    Value BlurStone::getAttr(const std::string &key) const {
        if (key == "flavor") {             // used by validator to identify charge stone subkinds
            return (state == STRAIGHT) ? "straight" : (state == CROSS ? "cross" : "magic"); 
        } else
            return Stone::getAttr(key);
    }
    
    void BlurStone::setState(int extState) {
        // block any calls
    }

    void BlurStone::init_model() {
        set_model(state == STRAIGHT ? "st_blur_straight" : "st_blur_cross");
    }
    
    void BlurStone::actor_hit(const StoneContact &sc) {
        Value color = sc.actor->getAttr("color");
        if (state != MAGIC || player::WieldedItemIs (sc.actor, "it_magicwand") 
                || player::WieldedItemIs (sc.actor, "it_brush")) {
            if (color && ((state == STRAIGHT && color == WHITE) || (state >= CROSS && color == BLACK))) {
                sound_event("yinyang");
                transform("st_passage_white_square");
            } else if (color && ((state == STRAIGHT && color == BLACK) || (state >= CROSS && color == WHITE))) {
                sound_event("yinyang");
                transform("st_passage_black_square");
            }
        }
    }

/* -------------------- Charge stone -------------------- */

    ChargeStone::ChargeStone(double charge) : Stone ("st_charge") {
        Stone::setAttr("charge", charge);
    }
    
    std::string ChargeStone::getClass() const {
        return "st_charge";
    }

    void ChargeStone::setAttr(const string& key, const Value &val) {
        Stone::setAttr(key, val);
        if (key == "charge" && isDisplayable()) 
            init_model();
    }

    Value ChargeStone::getAttr(const std::string &key) const {
        if (key == "$chargesign") {             // used by validator to identify charge stone subkinds
            double charge = getAttr("charge");
            return (charge == 0) ? 0 : ((charge > 0.0) ? 1 : -1);
        } else
            return Stone::getAttr(key);
    }

    Value ChargeStone::message(const Message &m) {
        if (server::GameCompatibility == enigma::GAMET_PEROXYD && m.message == "signal") {
            performAction(m.value);
            return Value();
        }
        return Stone::message(m);
    }
    
    void ChargeStone::init_model() {
        double charge = getAttr("charge");
        set_model(ecl::strf("st-charge%s", (charge == 0) ? "zero" : ((charge > 0.0) ? "plus" : "minus")).c_str());
    }
    
    void ChargeStone::animcb() {
        init_model();
    }
    
    void ChargeStone::actor_hit(const StoneContact &sc) {
        double charge = getAttr("charge");
        sc.actor->setAttr("charge", charge);
        set_anim(ecl::strf("st-charge%s-anim", (charge == 0) ? "zero" : ((charge > 0.0) ? "plus" : "minus")).c_str());
    }
    

/* -------------------- Flash stone -------------------- */

    FlashStone::FlashStone() : Stone ("st_flash") {
    }
    
    std::string FlashStone::getClass() const {
        return "st_flash";
    }
        
    void FlashStone::actor_hit(const StoneContact &sc) {
        if (Actor *other = FindOtherMarble(sc.actor)) {
            other->add_force (distortedVelocity(sc.actor->get_vel(), 20));
        }
    }

/* -------------------- Grate stone -------------------- */
    GrateStone::GrateStone(int type) : Stone ("st_grate") {
        state = type;
    }
    
    std::string GrateStone::getClass() const {
        return "st_grate";
    }

    void GrateStone::setAttr(const string& key, const Value &val) {
        if (key == "flavor" && isDisplayable()) {
            std::string fs = val.to_string();
            if (fs == "cross") state = CROSS;
            else if (fs == "framed") state = FRAMED;
            else ASSERT(false, XLevelRuntime, ecl::strf("Grate stone set known flavor%s", fs.c_str()).c_str());
            return; 
        } else
            Stone::setAttr(key, val);
    }

    Value GrateStone::getAttr(const std::string &key) const {
        if (key == "flavor") {             // used by validator to identify charge stone subkinds
            return (state == CROSS) ? "cross" : "framed"; 
        } else
            return Stone::getAttr(key);
    }
    
    void GrateStone::setState(int extState) {
        // block any calls
    }

    void GrateStone::init_model() {
        set_model(state == CROSS ? "st_grate_cross" : "st_grate_framed");
    }
    
    bool GrateStone::is_floating() const {
        return true;
    }
    
    bool GrateStone::is_transparent(Direction) const {
        return true;
    }
    
    StoneResponse GrateStone::collision_response(const StoneContact &sc) {
        if (server::GameCompatibility == GAMET_OXYD1) {
            return STONE_PASS;
        } else
            return sc.actor->is_on_floor() ? STONE_PASS : STONE_REBOUND;
    }
    
    BOOT_REGISTER_START
        BootRegister(new BlurStone(0), "st_blur");
        BootRegister(new BlurStone(0), "st_blur_straight");
        BootRegister(new BlurStone(1), "st_blur_cross");
        BootRegister(new BlurStone(2), "st_blur_magic");
        BootRegister(new ChargeStone(0.0), "st_charge");
        BootRegister(new ChargeStone(0.0), "st_charge_zero");
        BootRegister(new ChargeStone(1.0), "st_charge_plus");
        BootRegister(new ChargeStone(-1.0), "st_charge_minus");
        BootRegister(new FlashStone(), "st_flash");
        BootRegister(new GrateStone(0), "st_grate");
        BootRegister(new GrateStone(0), "st_grate_cross");
        BootRegister(new GrateStone(1), "st_grate_framed");
    BOOT_REGISTER_END

} // namespace enigma

