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
 */
#ifndef ITEMS_HH_INCLUDED
#define ITEMS_HH_INCLUDED

#include "GridObject.hh"

namespace enigma {

enum ItemID {
    it_INVALID = -1,
    it_FIRST = 0,
    it_none = 0,
    it_1pkillstone,
    it_2pkillstone,
    it_axe,
    it_bag,
    it_banana,
    it_bomb,
    it_bomb_burning,
    it_blocker,
    it_bottle_idle,
    it_bottle_broken,
    it_brake,
    it_bridge_oxyd,
    it_bridge_oxyd_active,
    it_brush,
    it_burnable_invisible,
    it_burnable_fireproof,
    it_burnable_ignited,
    it_burnable_burning,
    it_burnable_ash,
    it_burnable_oil,
    it_cherry,
    it_cherry_crushed,
    it_coffee_drop,
    it_coffee_teatime,
    it_coin_s,
    it_coin_m,
    it_coin_l,
    it_crack_i,
    it_crack_s,
    it_crack_m,
    it_crack_l,
    it_cross,
    it_death,
    it_debris,
    it_debris_water,
    it_document,
    it_drop,
    it_dynamite,
    it_dummy,
    it_easykeepstone,
    it_easykillstone,
    it_explosion_nil,
    it_explosion_hollow,
    it_explosion_crack,
    it_explosion_debris,
    it_exitsensor,
    it_extinguisher,
    it_extinguisher_medium,
    it_extinguisher_empty,
    it_extralife,
    it_flag_black,
    it_flag_white,
    it_floppy,
    it_glasses,
    it_glasses_broken,
    it_hammer,
    it_hill,
    it_hollow,
    it_hstrip,
    it_key,
    it_landmine,
    it_laserbeam,
    it_magicwand,
    it_magnet_off,
    it_magnet_on,
    it_meditation_caldera,
    it_meditation_hollow,
    it_meditation_dent,
    it_meditation_bump,
    it_meditation_hill,
    it_meditation_volcano,
    it_odometer,
    it_pencil,
    it_pin,
    it_pipe_e,
    it_pipe_w,
    it_pipe_s,
    it_pipe_n,
    it_pipe_es,
    it_pipe_ne,
    it_pipe_sw,
    it_pipe_nw,
    it_pipe_ew,
    it_pipe_ns,
    it_puller_n,
    it_puller_e,
    it_puller_s,
    it_puller_w,
    it_ring,
    it_rubberband,
    it_seed_wood,
    it_seed_fake,
    it_seed_volcano,
    it_seed_hay,
    it_sensor,
    it_shogun_s,
    it_shogun_m,
    it_shogun_l,
    it_signalfilter0,
    it_signalfilter1,
    it_spade,
    it_spoon,
    it_spring_keep,
    it_spring_drop,
    it_springboard,
    it_squashed,
    it_strip,
    it_strip_w,
    it_strip_s,
    it_strip_sw,
    it_strip_e,
    it_strip_ew,
    it_strip_es,
    it_strip_esw,
    it_strip_n,
    it_strip_nw,
    it_strip_ns,
    it_strip_nsw,
    it_strip_ne,
    it_strip_new,
    it_strip_nes,
    it_strip_nesw,
    it_surprise,
    it_sword,
    it_tinyhill,
    it_tinyhollow,
    it_trap,
    it_trigger,
    it_umbrella,
    it_vortex_open,
    it_vortex_closed,
    it_vstrip,
    it_weight,
    it_wormhole_off,
    it_wormhole_on,
    it_wrench,
    it_yinyang,
    //  for DAT compatibility only
    it_inversesensor,
    it_LAST,
    it_COUNT
};

/*! What may happen to an item _after_ it was activated? */
enum ItemAction {
    ITEM_DROP,  //!< Drop it to the floor
    ITEM_KILL,  //!< Remove it from the inventory and dispose it
    ITEM_KEEP,  //!< Keep it in the inventory; do nothing further
};

enum ItemFlags {
    itf_none = 0,
    itf_static = 1,          //!< Cannot be picked up - not liftable
    itf_indestructible = 2,  //!< Non standard handling of explosions - may well be destructible!
    itf_animation = 4,       //!< Use set_anim() instead of set_model()
    itf_invisible = 8,       //!< Item has no visible model
    itf_inflammable = 16,    //!< Burns when hit by laser beam
    itf_norespawn = 32,      //!< Don't respawn balls on top of this item
    itf_fireproof = 64,      //!< This item can't burn by fire
    itf_portable = 128,      //!< This static item can be added to the inventory
    itf_freezable = 256,     //!< This static item can be frozen and carried by an st_ice
};

struct ItemTraits {
    const char *name;  //!< Name of the item, e.g., "it_hammer"
    ItemID id;
    int flags;     //!< Combination of ItemFlags
    float radius;  //!< Radius, 0.0 = default
};

class Stone;
class Item : public GridObject {
public:
    Item();

    /* ---------- Public methods ---------- */
    void kill();
    void replace(std::string kind);

    /* ---------- Virtual functions ---------- */
    virtual std::string getClass() const override;
    virtual Value getAttr(const std::string &key) const override;

    void init_model() override;
    virtual void processLight(Direction d) override;
    virtual double getFriction(ecl::V2 position, double defaultFriction, Actor *a);
    virtual ecl::V2 calcMouseforce(Actor *a, ecl::V2 mouseForce, ecl::V2 floorForce);

    /* ---------- Item interface ---------- */

    virtual const ItemTraits &get_traits() const = 0;

    virtual bool isStatic() const;  // not liftable
    virtual bool isPortable() const;
    virtual bool isFreezable() const;

    /*! Return true if item completely covers the floor. In this
      case the Floor::actor_contact() will not be called
      automatically; this must be done from `Item::actor_hit' (if
      at all). */
    virtual bool covers_floor(ecl::V2 position, Actor *a) const { return false; }

    /*! Return the force an item exerts on actor `a'.  This is
      used by sloped and hills for force fields that are local to
      the current field.  For global force fields you have to
      register a ForceField in the world. */
    virtual void add_force(Actor *a, ecl::V2 &f);

    virtual bool can_drop_at(GridPos p);

    virtual void drop(Actor *a, GridPos p);

    /*! Called when item is dropped by actor `a' */
    virtual void on_drop(Actor *a);

    /*! Called when item is picked up by actor `a' */
    virtual void on_pickup(Actor *a);

    /*! Called when stone above item changes. */
    virtual void stone_change(Stone *st);

    /*! Called when item is ``hit'' by a moving stone. */
    virtual void on_stonehit(Stone *st);

    /*! Called when item is ``hit'' by an actor.  Return true if
      the item should be picked up. */
    virtual bool actor_hit(Actor *a);

    /*! The model used for displaying this item in an
      inventory. */
    virtual std::string get_inventory_model();

    /* Called when item is activated by the owner of `a'. */
    virtual ItemAction activate(Actor *a, GridPos p);

    virtual std::list<GridPos> warpSpreadPos(bool isWater);

protected:
    virtual Object::ObjectType getObjectType() const override { return Object::ITEM; }

    // GridObject interface
    virtual void set_model(const std::string &mname) override {
        display::SetModel(GridLoc(GRID_ITEMS, get_pos()), mname);
    }

    virtual display::Model *get_model() override {
        return display::GetModel(GridLoc(GRID_ITEMS, get_pos()));
    }

    virtual void kill_model(GridPos p) override { display::KillModel(GridLoc(GRID_ITEMS, p)); }
    void transform(std::string kind);
    // replace template method hook
    virtual void setup_successor(Item *newitem) {}
};

/* -------------------- Inline functions -------------------- */

/*! Return unique item type identifier. */
inline ItemID get_id(Item *it) {
    if (it)
        return it->get_traits().id;
    return it_none;
}

inline bool has_flags(Item *it, ItemFlags flags) {
    return (it->get_traits().flags & flags) == flags;
}

/* --------------------  Item Macros -------------------- */

#define DEF_ITEM(classname, kindname, kindid) \
    class classname : public Item {           \
        CLONEOBJ(classname);                  \
        DECL_ITEMTRAITS;                      \
                                              \
    public:                                   \
        classname() {}                        \
    };                                        \
    DEF_ITEMTRAITS(classname, kindname, kindid)

#define DEF_ITEMF(classname, kindname, kindid, flags) \
    class classname : public Item {                   \
        CLONEOBJ(classname);                          \
        DECL_ITEMTRAITS;                              \
                                                      \
    public:                                           \
        classname() {}                                \
    };                                                \
    DEF_ITEMTRAITSF(classname, kindname, kindid, flags)

#define DECL_ITEMTRAITS       \
    static ItemTraits traits; \
    const ItemTraits &get_traits() const override { return traits; }

#define DECL_ITEMTRAITS_ARRAY(n, subtype_expr) \
    static ItemTraits traits[n];               \
    const ItemTraits &get_traits() const override { return traits[subtype_expr]; }

#define DEF_ITEMTRAITS(classname, name, id) ItemTraits classname::traits = {name, id, itf_none, 0.0}

#define DEF_ITEMTRAITSF(classname, name, id, flags) \
    ItemTraits classname::traits = {name, id, flags, 0.0}

#define DEF_ITEMTRAITSR(classname, name, id, radius) \
    ItemTraits classname::traits = {name, id, 0, radius}

}  // namespace enigma

#endif
