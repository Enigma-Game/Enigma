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
#ifndef STONES_HH_INCLUDED
#define STONES_HH_INCLUDED

#include "GridObject.hh"

namespace enigma {

struct Impulse;
struct StoneContact;

enum StoneID {
    st_INVALID = -1,
    st_FIRST = 0,
    st_none = 0,

    st_actorimpulse,
    st_actorimpulse_invisible,
    st_barrier,
    st_borderstone,
    st_box,
    st_box_wood_growing,
    st_passage_black_square,
    st_passage_black_slash,
    st_passage_black_cross,
    st_passage_black_frame,
    st_boulder,
    st_brake,
    st_break_black,
    st_break_boulder,
    st_break_bug,
    st_break_laser,
    st_break_oxydc,
    st_break_plain,
    st_break_white,
    st_ghost_break,
    st_jamb,
    st_chameleon,
    st_chess,
    st_magic_oxydc,
    st_coinslot,
    st_death,
    st_death_invisible,
    st_death_movable,
    st_disco,
    st_dispenser,
    st_document,
    st_door,
    st_easymode,
    st_magic_oxyda,
    st_quake,
    st_floppy,
    st_fourswitch,
    st_greenbrown,
    st_ice,
    st_inkwell,
    st_key,
    st_knight,
    st_dongle,
    st_mirror,
    st_oneway,
    st_oxyd_0x18,
    st_pebble,
    st_peroxyd_0xb8,
    st_peroxyd_0xb9,
    st_plop,
    st_portal,
    st_pull,
    st_puzzle,
    st_rotator,
    st_rubberband,
    st_scissors,
    st_shogun,
    st_spitter,
    st_standard,
    st_stoneimpulse,
    st_stoneimpulse_steady,
    st_stoneimpulse_hollow,
    st_stoneimpulse_movable,
    st_surprise,
    st_swap,
    st_switch,
    st_thief,
    st_turnstile_red,
    st_turnstile_green,
    st_turnstilearm_w,
    st_turnstilearm_s,
    st_turnstilearm_e,
    st_turnstilearm_n,
    st_volcano,
    st_passage_white_square,
    st_passage_white_slash,
    st_passage_white_cross,
    st_passage_white_frame,
    st_window,
    st_lightpassenger,
    st_camouflage,
    st_polarswitch,
    st_volcano_growing,
    st_yinyang,

    st_LAST,
    st_COUNT = st_LAST
};

enum StoneFlags {
    stf_none = 0x0,
    stf_transparent = 0x1,  //< Laser beams can pass
};

enum Material {
    material_FIRST = 0,
    material_stone = 0,
    material_glass,
    material_metal,
    material_wood,
    material_cloth,
    material_LAST
};

enum StoneMovableStatus {
    MOVABLE_PERSISTENT,  // Stone is unmovable
    MOVABLE_BREAKABLE,   // Stone is breakable
    MOVABLE_STANDARD,    // Stone is movable in st-wood-way
    MOVABLE_IRREGULAR    // Stone moves in non-standard way
};

struct StoneTraits {
    const char *name;
    StoneID id;
    int flags;
    Material material;
    double restitution;
    StoneMovableStatus movable;
    // Note that many properties of stones are implemented as functions.
};

/*! Things that may happen when an actor hits a stone. */
enum StoneResponse {
    STONE_PASS,    // Actor may pass stone
    STONE_REBOUND  // Actor bounces off the stone
};

/*! Combined status bits, used in the freeze check routines. */
enum FreezeStatusBits {
    FREEZEBIT_NO_STONE = 1,    // Stone is NULL or easily breakable
    FREEZEBIT_HOLLOW = 2,      // Stone is hollow (no cherry)
    FREEZEBIT_PERSISTENT = 4,  // Stone is unmovable
    FREEZEBIT_STANDARD = 8,    // Stone is movable in st-wood-way
    FREEZEBIT_IRREGULAR = 16   // Stone moves in non-standard way
};

class Stone : public GridObject {

public:
    Stone();
    Stone(const char *kind);
    ~Stone();

    /* ---------- Virtual functions ---------- */
    virtual std::string getClass() const override;

    /* ---------- Stone interface (properties) ---------- */

    virtual const StoneTraits &get_traits() const;

    virtual const char *collision_sound();

    virtual StoneResponse collision_response(const StoneContact &sc);

    /*! Is this stone movable? Affects impulse-stones, fire, ordinary pushes... */
    virtual bool is_movable() const { return get_traits().movable >= MOVABLE_STANDARD; }

    /*! Can a swap-stone or pull-stone swap this stone? */
    virtual bool is_removable() const { return true; }

    /*! Is this stone floating above the floor (e.g. actors may pass)? */
    virtual bool is_floating() const { return false; }

    /*! Can laser beams pass through stone? Return is_floating() by default. */
    virtual bool is_transparent(Direction) const { return is_floating(); }

    /*! Do actors get stuck in this stone? */
    virtual bool is_sticky(const Actor *) const { return !is_floating(); }

    // Fire and water spreading that is face and state dependent for stones
    // like doors, window
    virtual bool allowsSpreading(Direction dir, bool isFlood = false) const {
        return is_floating();
    }

    virtual bool isConnectable(Stone *other) const {
        return other != nullptr && std::string(other->getClass()) == getClass();
    }

    /* ---------- Stone interface (events) ---------- */

    virtual void actor_hit(const StoneContact &sc);
    virtual void actor_touch(const StoneContact &sc);
    virtual void actor_inside(Actor *) {}
    virtual void actor_contact(Actor *) {}

    virtual bool freeze_check();

    virtual bool on_move(const GridPos &origin);
    virtual void on_floor_change() {}
    virtual void on_impulse(const Impulse &impulse);
    void propagateImpulse(const Impulse &impulse);

protected:
    virtual Object::ObjectType getObjectType() const override { return Object::STONE; }

    bool move_stone(GridPos newPos, const char *soundevent);
    bool move_stone(Direction dir);
    ecl::V2 distortedVelocity(ecl::V2 vel, double defaultfactor);
    void transform(std::string kind);

    // Cluster support
    void autoJoinCluster();
    void autoLeaveCluster();

protected:
    // GridObject interface
    virtual void on_creation(GridPos p) override;
    virtual void set_model(const std::string &mname) override {
        display::SetModel(GridLoc(GRID_STONES, get_pos()), mname);
    }

    virtual display::Model *get_model() override {
        return display::GetModel(GridLoc(GRID_STONES, get_pos()));
    }

    virtual void kill_model(GridPos p) override { display::KillModel(GridLoc(GRID_STONES, p)); }

private:
    // Help structure and routine for freeze_check()
    bool freeze_check_running;
    virtual FreezeStatusBits get_freeze_bits();           // own freeze bits
    virtual FreezeStatusBits get_freeze_bits(GridPos p);  // foreign freeze bits
};

inline StoneID get_id(Stone *st) {
    return st->get_traits().id;
}

}  // namespace enigma

#endif
