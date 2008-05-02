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

#include "errors.hh"
#include "stones_internal.hh"
#include "server.hh"
#include "client.hh"
#include "player.hh"
#include "Inventory.hh"
#include "main.hh"

using namespace std;

namespace enigma {

/* -------------------- Helper routines -------------------- */

/*! Determine whether the actor hitting the stone can move stone
  and return either the direction the stone should move or NODIR. */
Direction get_push_direction (const StoneContact &sc) 
{
    ActorInfo *ai  = sc.actor->get_actorinfo();
    Direction  dir = contact_face(sc);

    // Make sure the speed component towards the face of the stone is
    // large enough and pointing towards the stone.
    if (dir!=enigma::NODIR && ai->vel * sc.normal < -4)
        return reverse(dir);
    return NODIR;
}

/* Move a stone (by sending an impulse) Called when an actor hits a
   stone. */
bool maybe_push_stone (const StoneContact &sc) 
{
    Direction dir = get_push_direction(sc);
    if (dir != enigma::NODIR) {
        sc.actor->send_impulse(sc.stonepos, dir);
        return GetStone(sc.stonepos) == 0; // return true only if stone vanished
    }
    return false;
}


//======================================================================
// STONES
//======================================================================

/** \page lstones  Available Stones

Oxyd Stones:
- \ref st-oxyd
- \ref st-fakeoxyd
- \ref st-fart

Movable stones:
- \ref st-brownie
- \ref st-wood
- \ref st-block

Stones that can trigger actions:
- \ref st-switch
- \ref st-fourswitch
- \ref st-laserswitch
- \ref st-key
- \ref st-coinslot
- \ref st-timer

Stones that can change their behaviour:
- \ref st-brick_magic
- \ref st-stonebrush
- \ref st-invisible_magic
- \ref st-break_invisible

Lasers and Mirrors:
- \ref st-laser
- \ref st-pmirror
- \ref st-3mirror

Other stones:
- \ref st-death
- \ref st-swap
- \ref st-bolder
- \ref st-puzzle
- \ref st-stone_break
- \ref st_window
- \ref st-break_acwhite
- \ref st-break_acblack
- \ref st-oneway
- \ref st-oneway_black
- \ref st-oneway_white
- \ref st-chameleon
*/

Stone::Stone()
: freeze_check_running (false)
{}

Stone::Stone(const char * kind)
: GridObject (kind), freeze_check_running (false)
{}

Stone::~Stone() {  }

const StoneTraits &Stone::get_traits() const
{
    static StoneTraits default_traits = {
        "INVALID", st_INVALID, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT
    };
    return default_traits;
}

const char *Stone::get_kind() const
{
    const StoneTraits &tr = get_traits();
    if (tr.id != st_INVALID)
        return tr.name;
    else
        return Object::get_kind();
}

StoneResponse Stone::collision_response(const StoneContact &) {
    return STONE_REBOUND;
}


void Stone::actor_hit(const StoneContact &sc) 
{
    if (is_movable())
        maybe_push_stone (sc);
}

void Stone::actor_touch(const StoneContact &sc) {
}

void Stone::on_impulse(const Impulse& impulse) {
    if (is_movable())
        move_stone(impulse.dir);
}

const char * Stone::collision_sound() {
    return "stone";
}

/* Move a stone (regardless whether it is_movable() or not) if
   the destination field is free.
   Returns: true if stone has been moved.

   Note: This should be used by on_impulse() to perform a move.
*/
bool Stone::move_stone(GridPos newPos, const char *soundevent) {
    GridPos p      = get_pos();

    if (!GetStone(newPos)) {
        sound_event (soundevent);

        MoveStone(p, newPos);
        server::IncMoveCounter();

        on_move();
        if (Item *it = GetItem(newPos)) it->on_stonehit(this);

        return true;
    }
    return false;
}
bool Stone::move_stone(Direction dir) {
    return move_stone(move(get_pos(), dir), "movesmall");
}

void Stone::on_move() {
    if (!is_floating()) 
        ShatterActorsInsideField (get_pos());
}

/* Multiplies velocity with the attribute-matrix
   hit_distortion_[xx,xy,yx,yy] and factor hit_factor
   If components are not set, use ((1,0),(0,1)) as
   default matrix, resp. defaultfactor as hit_factor. */
ecl::V2 Stone::distortedVelocity (ecl::V2 vel, double defaultfactor = 1.0) {
    ecl::V2 newvel;
    double factor = this->getDefaultedAttr("hit_factor", defaultfactor);
    newvel[0] = (double)(this->getDefaultedAttr("hit_distortion_xx", 1)) * vel[0]
                + (double)(this->getAttr("hit_distortion_xy")) * vel[1];
    newvel[1] = (double)(this->getAttr("hit_distortion_yx")) * vel[0]
                + (double)(this->getDefaultedAttr("hit_distortion_yy", 1)) * vel[1];
    return newvel * factor;
}

/* -------------------- Freeze check routines -------------------- */

FreezeStatusBits Stone::get_freeze_bits() {
    if(is_floating())
        return FREEZEBIT_HOLLOW;
    switch(get_traits().movable) {
        case MOVABLE_PERSISTENT:  return FREEZEBIT_PERSISTENT;
        case MOVABLE_BREAKABLE:   return FREEZEBIT_NO_STONE;
        case MOVABLE_STANDARD:    return FREEZEBIT_STANDARD;
        default:                  return FREEZEBIT_IRREGULAR;
    }
}

FreezeStatusBits Stone::get_freeze_bits(GridPos p) {
    Stone *st = GetStone(p);
    if(st == NULL) 
        return FREEZEBIT_NO_STONE;
    return st->get_freeze_bits();
}

bool Stone::freeze_check() {
    GridPos this_pos = this->get_pos();
    // Check if stone and floor ask for freeze_check
    if (!to_bool(this->getAttr("freeze_check")))
        return false;
    if (freeze_check_running)
        return false;
    Floor *fl = GetFloor(this_pos);
    if ((fl == NULL) || (!fl->is_freeze_check()))
        return false;
    // Do freeze checks only with standard movables
    if (this->get_freeze_bits() != FREEZEBIT_STANDARD)
        return false;

    // Query persistence status of neighboring stones
    FreezeStatusBits ms_n  = get_freeze_bits(move(this_pos, NORTH));
    FreezeStatusBits ms_nw = get_freeze_bits(move(move(this_pos, NORTH), WEST));
    FreezeStatusBits ms_ne = get_freeze_bits(move(move(this_pos, NORTH), EAST));
    FreezeStatusBits ms_w  = get_freeze_bits(move(this_pos, WEST));
    FreezeStatusBits ms_e  = get_freeze_bits(move(this_pos, EAST));
    FreezeStatusBits ms_s  = get_freeze_bits(move(this_pos, SOUTH));
    FreezeStatusBits ms_sw = get_freeze_bits(move(move(this_pos, SOUTH), WEST));
    FreezeStatusBits ms_se = get_freeze_bits(move(move(this_pos, SOUTH), EAST));

    // The following if-construction searches for freeze-patterns.
    // Each block is one pattern, each line represents one
    // orientation of this pattern.
    //
    // First block: #    Centered at the box "$", there are four orientations
    //              $#   of this pattern.
    // 
    // Second block: $$  Each of the "$" can be movable or persistent.
    //               $$  Centered at one of them, there are again four
    //                   different orientation.
    //
    // Third block: #$   This pattern has eight orientations: Fix one of
    //               $#  the boxes. The adjacent persistent stone has four
    //                   possibilities, the adjacent movable has two choices
    //                   for each position of the persistent. The final
    //                   persistent is fixed in its position by the other
    //                   two stones.
    //
    // The variables P and PM are abbreviations for "persistent"
    // and "persistent or movable". Example: "ms_e & p" checks
    // if the stone east of THIS is persistent.
    int p = FREEZEBIT_PERSISTENT;
    int pm = FREEZEBIT_PERSISTENT | FREEZEBIT_STANDARD;
    if(   ((ms_n & p) && (ms_e & p))
       || ((ms_n & p) && (ms_w & p))
       || ((ms_s & p) && (ms_e & p))
       || ((ms_s & p) && (ms_w & p))
       
       || ((ms_n & pm) && (ms_nw & pm) && (ms_w & pm))
       || ((ms_n & pm) && (ms_ne & pm) && (ms_e & pm))
       || ((ms_s & pm) && (ms_sw & pm) && (ms_w & pm))
       || ((ms_s & pm) && (ms_se & pm) && (ms_e & pm))
       
       || ((ms_n & pm) && (ms_e & p) && (ms_nw & p))
       || ((ms_n & pm) && (ms_w & p) && (ms_ne & p))
       || ((ms_s & pm) && (ms_e & p) && (ms_sw & p))
       || ((ms_s & pm) && (ms_w & p) && (ms_se & p))
       || ((ms_w & pm) && (ms_n & p) && (ms_sw & p))
       || ((ms_w & pm) && (ms_s & p) && (ms_nw & p))
       || ((ms_e & pm) && (ms_n & p) && (ms_se & p))
       || ((ms_e & pm) && (ms_s & p) && (ms_ne & p)))
    {
        ReplaceStone(this_pos, MakeStone("st-death"));
        // recheck neighboring stones
        // avoid endless loop with bool freeze_check_running
        if (Stone *st = GetStone(this_pos))
            st->freeze_check_running = true;
        if (Stone *stn = GetStone(move(this_pos, NORTH)))
            stn->freeze_check();
        if (Stone *stn = GetStone(move(this_pos, SOUTH)))
            stn->freeze_check();
        if (Stone *stn = GetStone(move(this_pos, EAST)))
            stn->freeze_check();
        if (Stone *stn = GetStone(move(this_pos, WEST)))
            stn->freeze_check();
        if (Stone *st = GetStone(this_pos))
            st->freeze_check_running = false;
        return true;
    }
    return false;
}




// *******************************************************************************
//  Stones under development :



/* -------------------- Explosion stone -------------------- */
namespace
{
    class ExplosionStone : public Stone {
        CLONEOBJ(ExplosionStone);
        DECL_TRAITS;

        StoneResponse collision_response(const StoneContact &) {
            return STONE_PASS;
        }

        void actor_contact (Actor *a) {
            SendMessage(a, "shatter");
        }
        void init_model() {
            set_anim("st-explosion");
        }

        void animcb() {
            KillStone(get_pos());
        }

    public:
        ExplosionStone()
        {}
    };
    DEF_TRAITSM(ExplosionStone, "st-explosion", st_explosion, MOVABLE_BREAKABLE);
}


/* -------------------- Charge stone -------------------- */

// Attributes:
//
// :charge  + - 0
namespace
{
    class ChargeStone : public Stone {
        CLONEOBJ(ChargeStone);
    public:
        ChargeStone(const char *kind, double charge)
        : Stone(kind)
        {
            setAttr("charge", charge);
        }
        virtual Value message(const Message &m) {
            if (server::GameCompatibility == enigma::GAMET_PEROXYD && m.message == "signal") {
                performAction(m.value);
                return Value();
            }
            return Stone::message(m);
        }
    private:
        double get_charge() {
            double q = getAttr("charge");
            return max(-1.0, min(1.0, q));
        }
        void animcb() { init_model(); }
        void actor_hit (const StoneContact &sc) {
            ActorInfo *ai = sc.actor->get_actorinfo();
            ai->charge = get_charge();
            set_anim(string(get_kind())+"-anim");
        }
    };
}


/* -------------------- SpitterStone -------------------- */

namespace
{
    class SpitterStone : public Stone {
        CLONEOBJ(SpitterStone);

        enum State { IDLE, LOADING, SPITTING };

        // Variables
        State state;
        V2    ball_velocity;

        // Functions.
        void animcb();
        void actor_hit (const StoneContact &sc);

    public:
        SpitterStone () : Stone("st-spitter"), state (IDLE) {
        }
    };
}

void SpitterStone::animcb() {
    switch (state) {
    case IDLE:
        ASSERT(0, XLevelRuntime, "SpitterStone: animcb called with inconsistent state"); 
    case LOADING: {
        Actor     *ball = MakeActor (ac_cannonball);
        ActorInfo *ai   = ball->get_actorinfo();
        V2         center  = get_pos().center();

        state = SPITTING;
        ai->vel = ball_velocity;
        AddActor (center[0], center[1], ball);
        set_anim ("st-spitter-spitting");
        break;
    }
    case SPITTING:
        init_model();
        state = IDLE;
        break;
    }
}

void SpitterStone::actor_hit (const StoneContact &sc) 
{
    if (state != IDLE)
        return;

    if (enigma::Inventory *inv = player::GetInventory(sc.actor)) {
        int lifepos = inv->find("it_extralife");
        if (lifepos != -1) {
            delete inv->yield_item(lifepos);
            player::RedrawInventory (inv);
            ball_velocity = distortedVelocity(sc.actor->get_vel(), 1.0);
            state = LOADING;
            set_anim ("st-spitter-loading");
        }
    }
}


/* -------------------- YieldedGridStone -------------------- */

YieldedGridStone::YieldedGridStone(Stone *st)
: stone(st)
{
    GridPos pos = stone->get_pos();
    model       = display::YieldModel(GridLoc(GRID_STONES, pos));
    YieldStone(pos);
}

YieldedGridStone::~YieldedGridStone() 
{
    ASSERT(!stone, XLevelRuntime,
        "YieldedGridStone: destructor called though stone still exists");
    ASSERT(!model, XLevelRuntime,
        "YieldedGridStone: destructor called though model still exists");
}

void YieldedGridStone::set_stone(GridPos pos) 
{
    SetStone(pos, stone);
    display::SetModel(GridLoc(GRID_STONES, stone->get_pos()), model);
    stone->on_move();    // continue animations -- this is buggy if the stone has another
                         // model on the new position like st-chameleon
    SendMessage(stone, "_model_reanimated");  // temp fix: reset bad models
    stone = 0;
    model = 0;
}

void YieldedGridStone::dispose() 
{
    stone->dispose();
    delete model;
    stone = 0;
    model = 0;
}


/* -------------------- Oxyd compatibility stones -------------------- */

namespace
{
    /* I have no idea what these stones are _really_ supposed to do;
       they seemingly do not appear in the landscape and they create
       normal floor tiles on creation.  Other than that... who
       knows... */
    class Peroxyd_0xb8 : public Stone {
        CLONEOBJ(Peroxyd_0xb8);
        DECL_TRAITS;
    public:
        Peroxyd_0xb8()
        {}

        void on_creation (GridPos p) {
            SetFloor (p, MakeFloor ("fl-normal"));
            KillStone(p);
        }
    };
    DEF_TRAITSM(Peroxyd_0xb8, "st-peroxyd-0xb8", st_peroxyd_0xb8, MOVABLE_BREAKABLE);
    
    class Peroxyd_0xb9 : public Stone {
        CLONEOBJ(Peroxyd_0xb9);
        DECL_TRAITS;
    public:
        Peroxyd_0xb9()
        {}

        void on_creation (GridPos p) {
            SetFloor (p, MakeFloor ("fl-normal"));
            KillStone(p);
        }
    };
    DEF_TRAITSM(Peroxyd_0xb9, "st-peroxyd-0xb9", st_peroxyd_0xb9, MOVABLE_BREAKABLE);
    
    class Oxyd_0x18 : public Stone {
        CLONEOBJ(Oxyd_0x18);
        DECL_TRAITS;
    public:
        Oxyd_0x18()
        {}
        
        void on_creation (GridPos p) {
            KillStone (p);
        }
    };
    DEF_TRAITSM(Oxyd_0x18, "st-oxyd-0x18", st_oxyd_0x18, MOVABLE_BREAKABLE);
}


/* -------------------- Flash stone -------------------- */
namespace
{
    class FlashStone : public Stone {
        CLONEOBJ(FlashStone);

        void actor_hit (const StoneContact &sc) {
            if (Actor *other = FindOtherMarble(sc.actor)) {
                other->add_force (distortedVelocity(sc.actor->get_vel(), 20));
            }
        }

    public:
        FlashStone() : Stone ("st-flash")
        {}
    };
}


/* -------------------- Surprise stone -------------------- */
namespace 
{
    class SurpriseStone : public Stone {
        CLONEOBJ (SurpriseStone);
        DECL_TRAITS;
    public:
        SurpriseStone()
        {}

        void actor_hit (const StoneContact &) {
            static const char *stonename[] = {
                "st-grate1",
                "st-death",
                "st-surprise",
                "st-glass1_hole",
                "st-magic",
                "st-knight",
                "st-thief",
                "st-plain_break",
                "st-plain_breaking"
            };
            int idx = enigma::IntegerRand (1, 9) - 1;
            sound_event ("stonetransform");
            ReplaceStone (get_pos(), MakeStone (stonename[idx]));
        }
    private:
        FreezeStatusBits get_freeze_bits() { return FREEZEBIT_IRREGULAR; }
    };
    DEF_TRAITS(SurpriseStone, "st-surprise", st_surprise);
}


/* -------------------- Coffee stone -------------------- */
namespace
{
    class CoffeeStone : public Stone {
        CLONEOBJ(CoffeeStone);
        DECL_TRAITS;
    public:
        CoffeeStone()
        {}

        void actor_hit (const StoneContact &) {
            sound_event ("stonetransform");
            ReplaceStone(get_pos(), MakeStone("st-glass_move"));
        }
    private:
        FreezeStatusBits get_freeze_bits() { return FREEZEBIT_STANDARD; }
    };
    DEF_TRAITS(CoffeeStone, "st-coffee", st_coffee);
}


/* -------------------- Breaking stone -------------------- */
namespace
{
    class BreakingStone : public Stone {
        CLONEOBJ(BreakingStone);
        DECL_TRAITS;

        void init_model() {
            sound_event("stonedestroy");
            set_anim("st-breaking");
        }

        void animcb() {
            KillStone(get_pos());
        }
    public:
        BreakingStone()
        {}
    };
    DEF_TRAITSM(BreakingStone, "st-breaking", st_breaking, MOVABLE_BREAKABLE);
}


/* -------------------- Bug stone -------------------- */
namespace
{
    class BugStone : public Stone {
        CLONEOBJ(BugStone);
        DECL_TRAITS;
    public:
        BugStone()
        {}

        void actor_hit (const StoneContact &sc) {
            if (get_id(sc.actor) == ac_bug) {
                ReplaceStone(get_pos(), MakeStone("st-breaking"));
            }
        }
    };
    DEF_TRAITSM(BugStone, "st-bug", st_bug, MOVABLE_BREAKABLE);
}


/* -------------------- Plain stones -------------------- */

/* These stones mimic the behaviour of the plain-looking stones in
   Oxyd. */
namespace
{
    class PlainStone : public Stone {
        CLONEOBJ(PlainStone);
        DECL_TRAITS;

        void processLight(Direction d) {
            ReplaceStone (get_pos(), MakeStone("st-plain_cracked"));
        }

        const char *collision_sound() {return "stone";}

        virtual Value message(const Message &m) {
            if (m.message == "_trigger" || m.message == "signal") {
                ReplaceStone(get_pos(), MakeStone("st-plain_hole"));
                return Value();
            }
            return Stone::message(m);
        }
        void actor_hit (const StoneContact &sc) {
            if (player::WieldedItemIs (sc.actor, "it-pencil")) {
                enigma::Inventory *inv = player::GetInventory(sc.actor);
                if (inv && inv->size() > 0) {
                    delete inv->yield_item(0);
                    player::RedrawInventory(inv);
                    sound_event("stonepaint");
                    ReplaceStone(get_pos(), MakeStone("st-firebreak"));
                }
            } else
                Stone::actor_hit(sc);
        }
    public:
        PlainStone()
        {}
    private:
        FreezeStatusBits get_freeze_bits() { return FREEZEBIT_PERSISTENT; }
    };
    DEF_TRAITSM(PlainStone, "st-plain", st_plain, MOVABLE_BREAKABLE);
    
    class PlainStone_Hollow : public Stone {
        CLONEOBJ(PlainStone_Hollow);
        DECL_TRAITS;

        virtual Value message(const Message &m) {
            if (m.message == "_trigger" || m.message == "signal") {
                ReplaceStone(get_pos(), MakeStone("st-plain"));
                return Value();
            }
            return Stone::message(m);
        }

        StoneResponse collision_response(const StoneContact &) 
        { return STONE_PASS; }

        bool is_floating() const { return true; }
    public:
        PlainStone_Hollow()
        {}
    };
    DEF_TRAITSM(PlainStone_Hollow, "st-plain_hole", st_plain_hole, MOVABLE_PERSISTENT);

    class PlainStone_Breaking : public Stone {
        CLONEOBJ(PlainStone_Breaking);
        DECL_TRAITS;

        void init_model() {
            set_anim("st-plain_breaking");
        }
        void animcb() {
            KillStone(get_pos());
        }
        const char *collision_sound() {return "metal";}
    public:
        PlainStone_Breaking()
        {}
    };
    DEF_TRAITSM(PlainStone_Breaking, "st-plain_breaking", st_plain_breaking,
                MOVABLE_BREAKABLE);
    
    class PlainStone_Breakable : public Stone {
        CLONEOBJ(PlainStone_Breakable);
        DECL_TRAITS;

        const char *collision_sound() {return "metal";}

        void break_me() {
            sound_event ("stonedestroy");
            ReplaceStone(get_pos(), MakeStone ("st-plain_breaking"));
        }
        void processLight(Direction d) {
            break_me();
        }
        virtual Value message(const Message &m) {
            if (m.message =="ignite" || m.message == "_explosion" || m.message == "_bombstone") {
                break_me();
                return Value();
            }
            return Stone::message(m);
        }
        void actor_hit (const StoneContact &sc) {
            if (player::WieldedItemIs (sc.actor, "it_hammer")) {
                break_me();
            }
        }

        void on_floor_change() {
            if (Floor *fl = GetFloor (get_pos()))
                if (fl->is_kind("fl-abyss"))
                    ReplaceStone (get_pos(), MakeStone("st-plain_falling"));
        }

    public:
        PlainStone_Breakable()
        {}
    };
    DEF_TRAITSM(PlainStone_Breakable, "st-plain_break", st_plain_break,
                MOVABLE_BREAKABLE);
    
    class PlainStone_Cracked : public Stone {
        CLONEOBJ(PlainStone_Cracked);
        DECL_TRAITS;

        void break_me() {
            sound_event ("stonedestroy");
            ReplaceStone(get_pos(), MakeStone("st-plain_breaking"));
        }

        void actor_hit (const StoneContact &sc) {
            if (player::WieldedItemIs (sc.actor, "it_hammer")) {
                break_me();
            }
        }

        virtual Value message(const Message &m) {
            if (m.message =="ignite" || m.message == "_explosion" || m.message == "_bombstone") {
                break_me();
                return Value();
            }
            return Stone::message(m);
        }
        const char *collision_sound() {return "metal";}
    public:
        PlainStone_Cracked()
        {}
    };
    DEF_TRAITSM(PlainStone_Cracked, "st-plain_cracked", st_plain_cracked,
                MOVABLE_BREAKABLE);
    
    class PlainStone_Falling : public Stone {
        CLONEOBJ(PlainStone_Falling);
        DECL_TRAITS;

        void init_model() {
            set_anim("st-plain_falling");
        }

        void animcb() {
            sound_event ("stonedestroy");
            KillStone(get_pos());
        }
    public:
        PlainStone_Falling()
        {}
    };
    DEF_TRAITSM(PlainStone_Falling, "st-plain_falling", st_plain_falling,
                MOVABLE_BREAKABLE);

    class PlainStone_Movable : public Stone {
        CLONEOBJ(PlainStone_Movable);
        DECL_TRAITS;

        void break_me() {
            sound_event ("stonedestroy");
            ReplaceStone(get_pos(), MakeStone ("st-plain_breaking"));
        }
        virtual Value message(const Message &m) {
            if (m.message =="ignite" || m.message == "_explosion" || m.message == "_bombstone") {
                break_me();
                return Value();
            }
            return Stone::message(m);
        }
        void on_move() {
            Stone::on_move();
            GridPos p = get_pos();
            if (Floor *fl = GetFloor (p)) {
                if (fl->is_kind("fl-abyss")) {
                    ReplaceStone (p, MakeStone("st-plain_falling"));
                }
                else if (fl->is_kind("fl-swamp") || fl->is_kind("fl-water")) {
                    sound_event ("drown");
                    client::Msg_Sparkle (p.center());
                    KillStone (p);
                }
            }
        }

        void actor_hit (const StoneContact &sc) {
            if (player::WieldedItemIs (sc.actor, "it-pencil")) {
                enigma::Inventory *inv = player::GetInventory(sc.actor);
                if (inv && inv->size() > 0) {
                    delete inv->yield_item(0);
                    player::RedrawInventory(inv);
                    sound_event("stonepaint");
                    ReplaceStone(get_pos(), MakeStone("st-firebreak_move"));
                }
            } else
                Stone::actor_hit(sc);
        }

    public:
        PlainStone_Movable()
        {}
    };
    DEF_TRAITSM(PlainStone_Movable, "st-plain_move", st_plain_move,
                MOVABLE_STANDARD);
}


/* -------------------- Black- and Whiteballs Stones -------------------- */

namespace
{
    class BlackBallsStone : public Stone {
        CLONEOBJ(BlackBallsStone);

        virtual Value message(const Message &m)
        {
            if (m.message == "signal" || m.message == "hit") {
                if (GridObject *sender = dynamic_cast<GridObject*>(m.sender)) {
                    GridPos p = get_pos();
                    Object *o;
                    if (m.message == "hit")
                        o = m.value;
                    else
                        o = SendMessage(m.sender, "_hitactor");
                    
                    Actor *a = dynamic_cast<Actor *>(o);
                    if ((a && get_id(a) == ac_blackball) || 
                            (m.sender->getObjectType() != Object::ITEM && m.message == "signal")) {
                        if (p.y == sender->get_pos().y) {
                            SendMessage (GetStone (move (p, EAST)),  "signal", 1.0);
                            SendMessage (GetStone (move (p, WEST)),  "signal", 1.0);
                            SendMessage (GetStone (move (p, NORTH)), "signal", 0.0);
                            SendMessage (GetStone (move (p, SOUTH)), "signal", 0.0);
                        }
                        else {
                            SendMessage (GetStone (move (p, EAST)),  "signal", 0.0);
                            SendMessage (GetStone (move (p, WEST)),  "signal", 0.0);
                            SendMessage (GetStone (move (p, NORTH)), "signal", 1.0);
                            SendMessage (GetStone (move (p, SOUTH)), "signal", 1.0);
                        }
                        return Value();
                    }
                }
            }
            return Stone::message(m);
        }
    public:
        BlackBallsStone() : Stone ("st-blackballs") {
        }
    };

    class WhiteBallsStone : public Stone {
        CLONEOBJ(WhiteBallsStone);

        virtual Value message(const Message &m)
        {
            if (m.message == "signal" || m.message == "hit") {
                if (GridObject *sender = dynamic_cast<GridObject*>(m.sender)) {
                    GridPos p = get_pos();
                    Object *o;
                    if (m.message == "hit")
                        o = m.value;
                    else
                        o = SendMessage(m.sender, "_hitactor");
                        
                    Actor *a = dynamic_cast<Actor *>(o);
                    if ((a && get_id(a) == ac_whiteball) ||
                             (m.sender->getObjectType() != Object::ITEM && m.message == "signal")) {
                        if (p.y == sender->get_pos().y) {
                            SendMessage (GetStone (move (p, EAST)),  "signal", 1.0);
                            SendMessage (GetStone (move (p, WEST)),  "signal", 1.0);
                            SendMessage (GetStone (move (p, NORTH)), "signal", 0.0);
                            SendMessage (GetStone (move (p, SOUTH)), "signal", 0.0);
                        }
                        else {
                            SendMessage (GetStone (move (p, EAST)),  "signal", 0.0);
                            SendMessage (GetStone (move (p, WEST)),  "signal", 0.0);
                            SendMessage (GetStone (move (p, NORTH)), "signal", 1.0);
                            SendMessage (GetStone (move (p, SOUTH)), "signal", 1.0);
                        }
                        return Value();
                    }
                }
            }
            return Stone::message(m);
        }

    public:
        WhiteBallsStone() : Stone ("st-whiteballs") {
        }

    };
}

/* -------------------- Unimplemented stones -------------------- */

namespace
{
    class FakeOxydA : public Stone {
        CLONEOBJ(FakeOxydA);
        DECL_TRAITS;
    public:
        FakeOxydA()
        {}

        void actor_hit (const StoneContact &) {
            sound_event ("stonetransform");
            ReplaceStone(get_pos(), MakeStone("st-glass1_move"));
        }
    private:
        FreezeStatusBits get_freeze_bits() { return FREEZEBIT_STANDARD; }
    };
    DEF_TRAITS(FakeOxydA, "st-fakeoxyda", st_fakeoxyda);
}


// --------------------------------------------------------------------------------

void InitStones() {

    // Register(new ...);

    Register (new ExplosionStone);
    Register (new ChargeStone ("st-chargeplus", +1.0));
    Register (new ChargeStone ("st-chargeminus", -1.0));
    Register (new ChargeStone ("st-chargezero", 0.0));
    Register (new SpitterStone);
    Register (new Peroxyd_0xb8);
    Register (new Peroxyd_0xb9);
    Register (new Oxyd_0x18);
    Register (new FlashStone);
    Register (new SurpriseStone);
    Register (new CoffeeStone);
    Register (new BlackBallsStone);
    Register (new WhiteBallsStone);
    Register (new FakeOxydA);
    Register (new BreakingStone);
    Register (new BugStone);
    Register (new PlainStone);
    Register (new PlainStone_Hollow);
    Register (new PlainStone_Breakable);
    Register (new PlainStone_Breaking);
    Register (new PlainStone_Cracked);
    Register (new PlainStone_Movable);
    Register (new PlainStone_Falling);

    // Init stones from stones_simple.cc and stones_complex.cc:
    Init_simple();
    Init_complex();
}
} // namespace enigma
