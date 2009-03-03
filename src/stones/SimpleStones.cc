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
#include "client.hh"
#include "errors.hh"
#include "Inventory.hh"
#include "main.hh"
#include "player.hh"
#include "server.hh"
#include "SoundEffectManager.hh"
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
        if (key == "flavor") {
            std::string fs = val.to_string();
            if (fs == "straight") state = STRAIGHT;
            else if (fs == "cross") state = CROSS;
            else if (fs == "magic") state = MAGIC;
            else ASSERT(false, XLevelRuntime, ecl::strf("Blur stone set known flavor%s", fs.c_str()).c_str());
            if (isDisplayable())
                init_model();
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

/* -------------------- BrownPyramide stone -------------------- */
    BrownPyramide::BrownPyramide() : Stone ("st_brownpyramide") {
    }
    
    std::string BrownPyramide::getClass() const {
        return "st_brownpyramide";
    }

    Value BrownPyramide::message(const Message &m) {
        if (m.message == "_cannonball") {
            KillStone(get_pos());
            return Value();
        }
        return Stone::message(m);
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
        if (key == "flavor") {
            std::string fs = val.to_string();
            if (fs == "cross") state = CROSS;
            else if (fs == "framed") state = FRAMED;
            else ASSERT(false, XLevelRuntime, ecl::strf("Grate stone set known flavor%s", fs.c_str()).c_str());
            if (isDisplayable())
                init_model();
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
    
/* -------------------- Plop stone -------------------- */
    PlopStone::PlopStone() : Stone () {
    }
    
    std::string PlopStone::getClass() const {
        return "st_plop";
    }
        
    void PlopStone::init_model() {
        set_model("st_plop_slate");
    }
    
    void PlopStone::on_floor_change() {
        if (Floor *fl = GetFloor(get_pos())) {
            const std::string &k = fl->get_kind();
            if (k=="fl_water" || k=="fl_abyss" || k == "fl_swamp") {
                sound_event("drown");
                client::Msg_Sparkle(get_pos().center());
                KillStone(get_pos());
            }
        }
    }
    
    DEF_TRAITSM(PlopStone, "st_plop", st_plop, MOVABLE_STANDARD);

/* -------------------- SpitterStone -------------------- */

    SpitterStone::SpitterStone() {
    }
    
    void SpitterStone::setState(int extState) {
        // block any calls
    }
    
    Value SpitterStone::message(const Message &m) {
        if (m.message == "_cannonball") {
            if (!getAttr("secure").to_bool() || server::GameCompatibility == enigma::GAMET_PEROXYD) {
                state = BREAKING;
                init_model();
            }
            return Value();
        } else if (m.message == "spit" && state == IDLE && isDisplayable()) {
            ecl::V2 dest = m.value;
            if (!IsInsideLevel(dest)) {
                int idx = getDefaultedAttr("$hitdestindex", 0);
                if (!getDestinationByIndex(idx++, dest)) {
                    if (idx != 1) {
                        idx = 0;
                        if (!getDestinationByIndex(idx++, dest))
                            return Value();
                    } else
                        return Value();
                }
                setAttr("$hitdestindex", idx);
                if (!IsInsideLevel(dest))
                    return Value();
            }
            ecl::V2 vel = (GridPos(dest).center() - get_pos().center()) /0.55;
            setAttr("$ball_velocity", vel);
            state = LOADING;
            init_model();
            return Value();
        }
        return Stone::message(m);
    }
    
    void SpitterStone::init_model() {
        if (state == IDLE)
            set_model("st_spitter");
        else if (state == LOADING)
            set_anim("st_spitter_loading");
        else if (state == SPITTING)
            set_anim("st_spitter_spitting");
        else
            set_anim("st_break_plain-anim");
    }
    
    void SpitterStone::animcb() {
        switch (state) {
            case IDLE :
                ASSERT(false, XLevelRuntime, "SpitterStone: animcb called with inconsistent state"); 
            case LOADING : {
                Other *cb = dynamic_cast<Other *>(MakeObject("ot_cannonball"));
                ecl::V2 center  = get_pos().center();
                cb->setAttr("$position", center);
                cb->setAttr("$ball_velocity", getAttr("$ball_velocity"));
                AddOther(cb);
                
                state = SPITTING;
                init_model();
                break;
            }
            case SPITTING : 
                state = IDLE;
                init_model();
                break;
            case BREAKING :
                SetItem(get_pos(), MakeItem("it_meditation"));
                KillStone(get_pos());
                break;
        }
    }

    void SpitterStone::actor_hit (const StoneContact &sc) {
        if (state != IDLE)
            return;
    
        if (enigma::Inventory *inv = player::GetInventory(sc.actor)) {
            int lifepos = inv->find("it_extralife");
            if (lifepos != -1) {
                delete inv->yield_item(lifepos);
                player::RedrawInventory (inv);
                ecl::V2 vel = distortedVelocity(sc.actor->get_vel(), 1.0);
                double maxvel = (double)getAttr("range")/0.56;
                if (maxvel * maxvel < square(vel))
                    vel = maxvel * normalize(vel);
                setAttr("$ball_velocity", vel);
                state = LOADING;
                init_model();
            }
        }
    }

    DEF_TRAITSM(SpitterStone, "st_spitter", st_yinyang, MOVABLE_PERSISTENT);

/* -------------------- Yinyang stone -------------------- */
    YinyangStone::YinyangStone(int initState, bool isInstant) : Stone () {
        state = initState;
        if (isInstant)
            setAttr("instant", true);
    }
    
    std::string YinyangStone::getClass() const {
        return "st_yinyang";
    }
    
    Value YinyangStone::message(const Message &m) {
        if (m.message == "_cannonball") {
            if (state == IDLE) {
                state = ACTIVE;
                init_model();
                if (getAttr("instant").to_bool())
                    switchPlayer();
            }
            return Value();
        }
        return Stone::message(m);
    }
    
    int YinyangStone::maxState() const {
        return 2;
    }
    
    void YinyangStone::toggleState() {
        // just toggle IDLE states, do not stop ACTIVE, or reactive INACTIVE states
        if (state == IDLE)
           setState(ACTIVE);
    }
    
    void YinyangStone::setState(int extState) {
        if (!isDisplayable())
           state = extState;
        else if (state == IDLE) {
            state = extState;
            init_model();
        }
    }
        
    void YinyangStone::init_model() {
        if (state == IDLE)
            set_model("st_yinyang");
        else if (state == INACTIVE)
            set_model("st_yinyang_inactive");
        else
            set_anim("st_yinyang-anim");
    }
    
    void YinyangStone::animcb() {
        state =  (getAttr("loop").to_bool()) ? IDLE : INACTIVE;
        init_model();
        if (!getAttr("instant").to_bool())
            switchPlayer();
    }
    
    void YinyangStone::actor_hit(const StoneContact &sc) {
        if (state == IDLE) {
            state = ACTIVE;
            init_model();
            if (getAttr("instant").to_bool())
                switchPlayer();
        }
    }
    
    void YinyangStone::switchPlayer() {
        // Switch to other marble
        player::SwapPlayers();
        sound::EmitSoundEvent("switchplayer", get_pos().center());
        performAction(true);        
    }
    
    DEF_TRAITSM(YinyangStone, "st_yinyang", st_yinyang, MOVABLE_PERSISTENT);


    BOOT_REGISTER_START
        BootRegister(new BlurStone(0), "st_blur");
        BootRegister(new BlurStone(0), "st_blur_straight");
        BootRegister(new BlurStone(1), "st_blur_cross");
        BootRegister(new BlurStone(2), "st_blur_magic");
        BootRegister(new BrownPyramide(), "st_brownpyramide");
        BootRegister(new ChargeStone(0.0), "st_charge");
        BootRegister(new ChargeStone(0.0), "st_charge_zero");
        BootRegister(new ChargeStone(1.0), "st_charge_plus");
        BootRegister(new ChargeStone(-1.0), "st_charge_minus");
        BootRegister(new FlashStone(), "st_flash");
        BootRegister(new GrateStone(0), "st_grate");
        BootRegister(new GrateStone(0), "st_grate_cross");
        BootRegister(new GrateStone(1), "st_grate_framed");
        BootRegister(new PlopStone(), "st_plop");
        BootRegister(new PlopStone(), "st_plop_slate");
        BootRegister(new SpitterStone(), "st_spitter");
        BootRegister(new YinyangStone(0), "st_yinyang");
        BootRegister(new YinyangStone(0, true), "st_yinyang_instant");
        BootRegister(new YinyangStone(1), "st_yinyang_active");
        BootRegister(new YinyangStone(2), "st_yinyang_inactive");
    BOOT_REGISTER_END

} // namespace enigma

