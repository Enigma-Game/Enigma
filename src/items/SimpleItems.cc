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

#include "items/SimpleItems.hh"
#include "errors.hh"
#include "Inventory.hh"
#include "player.hh"
#include "SoundEffectManager.hh"
#include "world.hh"
#include "client.hh"

namespace enigma {

/* -------------------- Axe -------------------- */

    Axe::Axe() {
    }

    DEF_ITEMTRAITSF(Axe, "it_axe", it_axe, itf_portable | itf_freezable);


/* -------------------- Banana -------------------- */

    Banana::Banana() {
    }

    void Banana::processLight(Direction d) {
        sound_event("itemtransform");
        transform("it_cherry");
    }

    void Banana::on_stonehit(Stone *) {
        transform("it_squashed");
    }

    DEF_ITEMTRAITS(Banana, "it_banana", it_banana);


/* -------------------- Brush -------------------- */

    Brush::Brush() {
    }

    ItemAction Brush::activate(Actor *a, GridPos p) {
        if (Item *it = GetItem(p))
            SendMessage (it, "_brush");
        return ITEM_DROP;
    }

    DEF_ITEMTRAITSF(Brush, "it_brush", it_brush, itf_inflammable);

/* -------------------- Cherry -------------------- */

    Cherry::Cherry() {
    }

    ItemAction Cherry::activate(Actor *actor, GridPos p) {
        if (SendMessage(actor, "_invisibility").to_bool())
            return ITEM_KILL;
        else
            // item not applied - dropped by an actor that does not become invisible
            return ITEM_DROP;
    }

    void Cherry::on_stonehit(Stone *) {
        transform("it_squashed");
    }

    void Cherry::on_drop(Actor *a) {
        transform("it_squashed");
    }

    DEF_ITEMTRAITS(Cherry, "it_cherry", it_cherry);

/* -------------------- Coffee -------------------- */
    Coffee::Coffee(int type) {
        state = type;
    }

    std::string Coffee::getClass() const {
        return "it_coffee";
    }
    void Coffee::setState(int extState) {
        // block all write attempts
    }

    ItemAction Coffee::activate(Actor *a, GridPos p) {
        if (state == TEATIME || server::GameCompatibility != GAMET_ENIGMA) {
            server::Msg_Teatime(true);
            client::Msg_Teatime(true);
            return ITEM_KILL;
        } else
            return ITEM_DROP;
    }

    int Coffee::traitsIdx() const {
        return ecl::Clamp<int>(state, 0, 1);
    }

    ItemTraits Coffee::traits[2] = {
        {"it_coffee_drop",    it_coffee_drop,    itf_animation | itf_portable | itf_freezable, 0.0},
        {"it_coffee_teatime", it_coffee_teatime, itf_animation | itf_portable | itf_freezable, 0.0},
    };

/* -------------------- Death Item  -------------------- */

    DeathItem::DeathItem() {
    }

    void DeathItem::animcb() {
        set_model("it_death");
        state = 0;
     }

    bool DeathItem::actor_hit(Actor *a) {
        ActorInfo &ai = * a->get_actorinfo();
        if (!ai.grabbed) {
            SendMessage(a, "_shatter");
            if (state == 0) {
                state = 1;
                set_anim("it_death_anim");
            }
        }
        return false;
    }

    DEF_ITEMTRAITSF(DeathItem, "it_death", it_death, itf_static | itf_indestructible);

/* -------------------- Debris -------------------- */

    Debris::Debris(int type) {
        state = type;
    }

    void Debris::animcb() {
        SetFloor(get_pos(), MakeFloor(state == 0 ? "fl_abyss" : "fl_water"));
        kill();
     }

    bool Debris::actor_hit(Actor *a) {
        SendMessage(a, "_fall");
        return false;
    }

    int Debris::traitsIdx() const {
        return ecl::Clamp<int>(state, 0, 1);
    }

    ItemTraits Debris::traits[2] = {
        {"it_debris", it_debris,  itf_static | itf_animation | itf_indestructible | itf_fireproof, 0.0},
        {"it_debris_water", it_debris_water,  itf_static | itf_animation | itf_indestructible | itf_fireproof, 0.0},
    };

/* -------------------- Explosion -------------------- */

    Explosion::Explosion(int strength) {
        state = strength;
    }

    std::string Explosion::getClass() const {
        return "it_explosion";
    }

    Value Explosion::message(const Message &m) {
        if (m.message == "_cannonball") {
            state = 3;
            return Value();
        } else if (m.message == "_freeze") {
            kill();   // The ice cleans the explosion
            return Value();
        }
        return Item::message(m);
    }

    void Explosion::setState(int extState) {
        // no state writes
    }

    void Explosion::init_model() {
        set_anim("it_explosion");
    }

    void Explosion::animcb() {
        Floor *fl = GetFloor(get_pos());
        if (state != 0 && fl->is_destructible())
            if (state == 1)
                replace("it_meditation_hollow");
            else if (state == 2)
                replace("it_crack_m_water");
            else {
                bool isAbyss = true;
                if (server::SubSoil == 1)
                    isAbyss = false;
                else if (server::SubSoil == 2) {
                    for (Direction d = NORTH; d != NODIR; d = previous(d)) {
                        Floor *thefl = GetFloor(move(get_pos(), d));
                        if (thefl != NULL && thefl->getClass() == "fl_water") {
                            isAbyss = false;
                            break;
                        }
                    }
                }
                replace(isAbyss ? "it_debris" : "it_debris_water");
            }
        else
            kill();
     }

    bool Explosion::actor_hit(Actor *actor) {
        SendMessage(actor, "_shatter");
        return false;
    }

    int Explosion::traitsIdx() const {
        return ecl::Clamp<int>(state, 0, 3);
    }

    ItemTraits Explosion::traits[4] = {
        {"it_explosion_nil",  it_explosion_nil, itf_static | itf_animation | itf_indestructible | itf_norespawn | itf_fireproof, 0.0},
        {"it_explosion_hollow",  it_explosion_hollow, itf_static | itf_animation | itf_indestructible | itf_norespawn | itf_fireproof, 0.0},
        {"it_explosion_crack",  it_explosion_crack, itf_static | itf_animation | itf_indestructible | itf_norespawn | itf_fireproof, 0.0},
        {"it_explosion_debris",  it_explosion_debris, itf_static | itf_animation | itf_indestructible | itf_norespawn | itf_fireproof, 0.0},
    };

/* -------------------- Flag -------------------- */

    FlagItem::FlagItem(int type) {
        Item::setAttr("color", type);
    }

    std::string FlagItem::getClass() const {
        return "it_flag";
    }

    void FlagItem::setAttr(const std::string& key, const Value &val) {
        if (key == "color") {
            if ((int)val < 0 || (int)val > 1)
                return;
        }
        Item::setAttr(key, val);
    }
    void FlagItem::on_drop(Actor *a) {
        player::SetRespawnPositions(get_pos(), getAttr("color"));
    }

    void FlagItem::on_pickup(Actor *a) {
        player::RemoveRespawnPositions(getAttr("color"));
    }

    int FlagItem::traitsIdx() const {
        return getAttr("color");
    }

    ItemTraits FlagItem::traits[2] = {
        {"it_flag_black", it_flag_black,  itf_none, 0.0},
        {"it_flag_white", it_flag_white,  itf_none, 0.0},
    };

/* -------------------- Key -------------------- */

    Key::Key() {
    }

    void Key::setAttr(const std::string& key, const Value &val) {
        Item::setAttr(key, val);
        if (key == "invisible" || key == "code") {
            player::RedrawInventory();
        }
    }

    std::string Key::get_inventory_model() {
        bool showCode = !getAttr("invisible").to_bool();
        int code = getAttr("code");
        if (showCode && code >= 1 && code <= 8)
            return ecl::strf("it_key_%d", code);
        else
            return "it_key";
    }

    DEF_ITEMTRAITS(Key, "it_key", it_key);

/* -------------------- Pencil -------------------- */

    Pencil::Pencil() {
    }

    ItemAction Pencil::activate(Actor *a, GridPos p) {
        if (server::GameCompatibility == GAMET_ENIGMA) {
            if (GetItem(p))
                return ITEM_KEEP;
            // If the actor is flying and tries to make a cross, drop the it-pencil
            if (a->is_flying())
                return ITEM_DROP;

            Floor *fl = GetFloor(p);
            std::string floor = fl->getClass();

            /* do not allow markings on this floortypes:
               fl_abyss, fl_water, fl_swamp
               fl-bridge[{-closed,-open}]?
               markings on fl_ice will result as it-crack1
            */
            if (floor == "fl_abyss" || floor == "fl_water" || floor == "fl_swamp" || floor == "fl_bridge" ) {
                return ITEM_KEEP;
            } else if (floor == "fl_ice") {
                SetItem(p, MakeItem("it_crack_s_water"));
            } else {
                Item *newItem = MakeItem("it_cross");
                transferIdentity(newItem);
                SetItem(p, newItem);
            }
            return ITEM_KILL;
        }
        return ITEM_KEEP;
    }

    DEF_ITEMTRAITS(Pencil, "it_pencil", it_pencil);

/* -------------------- Pin -------------------- */
    Pin::Pin() {
    }

    void Pin::setOwner(int player) {
        Value oldPlayer = getOwner();
        Item::setOwner(player);
        if (oldPlayer.getType() != Value::NIL && oldPlayer != -1 ) {
            BroadcastMessage("_update_pin", oldPlayer, GRID_NONE_BIT, true);
        }
        if (player != -1) {
            BroadcastMessage("_update_pin", player, GRID_NONE_BIT, true);
        }
    }
    DEF_ITEMTRAITS(Pin, "it_pin", it_pin);
/* -------------------- Ring -------------------- */

    Ring::Ring() {
    }

    ItemAction Ring::activate(Actor *a, GridPos p) {
        if (a->isMoribund())
            return ITEM_KEEP;
        else {
            if (ExchangeMarbles(a)) {
                sound_event("switchmarbles");
            }
            else {
                RespawnActor(a);
            }
            return ITEM_DROP;
        }
    }

    DEF_ITEMTRAITS(Ring, "it_ring", it_ring);

/* -------------------- Spade -------------------- */

    Spade::Spade() {
    }

    ItemAction Spade::activate(Actor *a, GridPos p) {
        if (Item *it=GetItem(p)) {
            sound::EmitSoundEvent("spade", p.center());
            SendMessage(it, "shovel");
            return ITEM_KEEP;
        }
        return ITEM_DROP;
    }

    DEF_ITEMTRAITS(Spade, "it_spade", it_spade);

/* -------------------- Spoon -------------------- */

    Spoon::Spoon() {
    }

    ItemAction Spoon::activate(Actor *a, GridPos) {
        SendMessage(a, "_suicide");
        return ITEM_DROP;
    }

    DEF_ITEMTRAITS(Spoon, "it_spoon", it_spoon);

/* -------------------- Spring -------------------- */
    Spring::Spring(int type) {
        state = type;
    }

    std::string Spring::getClass() const {
        return "it_spring";
    }
    void Spring::setState(int extState) {
        // block all write attempts
    }

    ItemAction Spring::activate(Actor *a, GridPos p) {
        if (state == KEEP) {
            SendMessage(a, "_jump");
            return ITEM_KEEP;
        } else if (state == DROP) {
            Item *it = GetItem(p);
            if (!it || has_flags(it, itf_static)) {
                SendMessage(a, "_jump");
                return ITEM_DROP;  // drop if grid has no item
            } else {
                // don't jump if a regular item is on the grid
                return ITEM_KEEP;
            }
        }
        return ITEM_KEEP;
    }

    int Spring::traitsIdx() const {
        return ecl::Clamp<int>(state, 0, 1);
    }

    ItemTraits Spring::traits[2] = {
        {"it_spring_keep", it_spring_keep,  itf_none, 0.0},
        {"it_spring_drop", it_spring_drop,  itf_none, 0.0},
    };

/* -------------------- Springboard -------------------- */
    Springboard::Springboard() {
    }

    bool Springboard::actor_hit(Actor *a) {
        const double MAXDIST = 0.3;
        double ycenter = get_pos().y + 0.5;
        double xcenter = get_pos().x + 0.5;
        ecl::V2 apos = a->get_pos();
        if (a->is_on_floor() && (fabs(apos[1] - ycenter) <= MAXDIST) && (fabs(apos[0] - xcenter) <= MAXDIST)) {
            set_anim("it_springboard_anim");
            SendMessage(a, "_jump");
        }
        return false;
    }

    void Springboard::animcb() {
        set_model("it_springboard");
    }

    DEF_ITEMTRAITSF(Springboard, "it_springboard", it_springboard, itf_static | itf_portable | itf_freezable);

/* -------------------- Squashed Cherry -------------------- */

    Squashed::Squashed() {
    }

    Value Squashed::message (const Message &m) {
        if (server::GameCompatibility == GAMET_ENIGMA) {
            if (m.message == "_brush" || m.message == "_freeze") {
                KillItem(this->get_pos());
                return Value();
            }
        }
        return Item::message(m);
    }

    DEF_ITEMTRAITSF(Squashed, "it_squashed", it_squashed, itf_static);

/* -------------------- Weight item -------------------- */
    Weight::Weight() {
    }

    void Weight::setAttr(const std::string& key, const Value &val) {
        if (key == "mass") {
            double oldMass = getAttr("mass");
            double newMass = val;
            if (newMass != oldMass && newMass > 0) {
                Item::setAttr("mass", newMass);
                Value owner = getOwner();
                if (owner.getType() != Value::NIL) {
                    Inventory *i = player::GetInventory(owner);
                    i->setAttr("mass", (double)i->getAttr("mass") + newMass - oldMass);
                    BroadcastMessage("_update_mass", owner, GRID_NONE_BIT, true);
                }
            }
            return;
        }
        Item::setAttr(key, val);
    }

    void Weight::setOwner(int player) {
        Value oldPlayer = getOwner();
        if (player == -1) {
            GridPos p = getOwnerPos();
            if (p.x >= 0)
                SendMessage(GetFloor(p), "_add_mass", -(double)getAttr("mass"), this);
        }
        Item::setOwner(player);
        if (oldPlayer.getType() != Value::NIL && oldPlayer != -1 ) {
            Inventory *i = player::GetInventory(oldPlayer);
            i->setAttr("mass", (double)i->getAttr("mass") - (double)getAttr("mass"));
            BroadcastMessage("_update_mass", oldPlayer, GRID_NONE_BIT, true);
        }
        if (player != -1) {
            Inventory *i = player::GetInventory(player);
            i->setAttr("mass", (double)i->getAttr("mass") + (double)getAttr("mass"));
            BroadcastMessage("_update_mass", player, GRID_NONE_BIT, true);
        }
    }

    void Weight::on_creation(GridPos p) {
        Item::on_creation(p);
        SendMessage(GetFloor(p), "_add_mass", getAttr("mass"), this);
    }

    void Weight::on_removal(GridPos p) {
        SendMessage(GetFloor(get_pos()), "_add_mass", -(double)getAttr("mass"), this);
        Item::on_removal(p);
    }

    void Weight::setOwnerPos(GridPos po) {
        if (po == GridPos(-1, -1)) {
            GridPos p = getOwnerPos();
            if (p.x >= 0)
                SendMessage(GetFloor(p), "_add_mass", -(double)getAttr("mass"), this);
        }
        Item::setOwnerPos(po);
        SendMessage(GetFloor(po), "_add_mass", getAttr("mass"), this);
    }

    ItemAction Weight::activate(Actor *, GridPos p) {
        return ITEM_KEEP;
    }

    DEF_ITEMTRAITS(Weight, "it_weight", it_weight);

/* -------------------- YinYang item -------------------- */
    Yinyang::Yinyang() {
    }

    std::string Yinyang::get_inventory_model() {
        if (player::CurrentPlayer() == 0)
            return "it_yinyang";
        else
            return "it_yangyin";
    }

    ItemAction Yinyang::activate(Actor *a, GridPos p) {
        // Switch to other marble
        player::SwapPlayers();
        sound::EmitSoundEvent("switchplayer", p.center());
        return ITEM_KEEP;
    }

    DEF_ITEMTRAITS(Yinyang, "it_yinyang", it_yinyang);


    BOOT_REGISTER_START
        BootRegister(new Axe(), "it_axe");
        BootRegister(new Banana(), "it_banana");
        BootRegister(new Brush(), "it_brush");
        BootRegister(new Cherry(), "it_cherry");
        BootRegister(new Coffee(0), "it_coffee");
        BootRegister(new Coffee(0), "it_coffee_drop");
        BootRegister(new Coffee(1), "it_coffee_teatime");
        BootRegister(new DeathItem(), "it_death");
        BootRegister(new Debris(0), "it_debris");
        BootRegister(new Debris(1), "it_debris_water");
        BootRegister(new Explosion(0), "it_explosion");
        BootRegister(new Explosion(0), "it_explosion_nil");
        BootRegister(new Explosion(1), "it_explosion_hollow");
        BootRegister(new Explosion(2), "it_explosion_crack");
        BootRegister(new Explosion(3), "it_explosion_debris");
        BootRegister(new FlagItem(0), "it_flag");
        BootRegister(new FlagItem(0), "it_flag_black");
        BootRegister(new FlagItem(1), "it_flag_white");
        BootRegister(new Floppy(), "it_floppy");
        BootRegister(new MagicWand(), "it_magicwand");
        BootRegister(new Key(), "it_key");
        BootRegister(new Pencil(), "it_pencil");
        BootRegister(new Pin(), "it_pin");
        BootRegister(new Ring(), "it_ring");
        BootRegister(new Spade(), "it_spade");
        BootRegister(new Spoon(), "it_spoon");
        BootRegister(new Spring(0), "it_spring");
        BootRegister(new Spring(0), "it_spring_keep");
        BootRegister(new Spring(1), "it_spring_drop");
        BootRegister(new Springboard(), "it_springboard");
        BootRegister(new Squashed(), "it_squashed");
        BootRegister(new Weight(), "it_weight");
        BootRegister(new Wrench(), "it_wrench");
        BootRegister(new Yinyang(), "it_yinyang");
    BOOT_REGISTER_END

} // namespace enigma
