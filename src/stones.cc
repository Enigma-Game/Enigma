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

void Stone::on_creation(GridPos p) {
    // notify rubberbands that may now exceed max/min limits
    ObjectList olist = getAttr("rubbers");   // a private deletion resistant copy
    for (ObjectList::iterator itr = olist.begin(); itr != olist.end(); ++itr)
        SendMessage(*itr, "_recheck");
    GridObject::on_creation(p);
}

void Stone::transform(std::string kind) {
    Stone *newStone = MakeStone(kind.c_str());
    transferIdentity(newStone);          // subclasses may hook this call
    SetStone(get_pos(), newStone);
}



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


Stone::Stone() : freeze_check_running (false) {
}

Stone::Stone(const char * kind) : GridObject (kind), freeze_check_running (false) {
}

Stone::~Stone() {
}

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
    if (is_movable()) {
        int id = getId();
        move_stone(impulse.dir);    // may kill the stone!
        
        if (Object::getObject(id) != NULL)   // not killed?
            propagateImpulse(impulse);
    }
}

void Stone::propagateImpulse(const Impulse& impulse) {
    if (!impulse.byWire) {
        ObjectList olist = getAttr("fellows");
        for (ObjectList::iterator it = olist.begin(); it != olist.end(); ++it) {
            Stone *fellow = dynamic_cast<Stone *>(*it);
            if (fellow != NULL) {
                Impulse wireImpulse(this, fellow->get_pos(), impulse.dir, true);
                fellow->on_impulse(wireImpulse);
            }
        }
    }
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
    if (isDisplayable()) {
        GridPos p      = get_pos();
    
        if (!GetStone(newPos)) {
            sound_event (soundevent);
    
            MoveStone(p, newPos);
            server::IncMoveCounter();
    
            on_move();
            if (Item *it = GetItem(newPos))
                it->on_stonehit(this);
    
            return true;
        }
        return false;
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

/* -------------------- Cluster routines -------------------- */
    void Stone::autoJoinCluster() {
        GridPos p = get_pos();
        Value myCluster = getAttr("cluster");
        for (int i = WEST; i <= NORTH; i++) {
            Direction d = (Direction) i;
            Stone *neighbour = GetStone(move(p, d));
            if (isConnectable(neighbour)) {
                Value neighbourCluster = neighbour->getAttr("cluster");
                if (myCluster) {
                    if (myCluster == neighbourCluster) {
                        setAttr("$connections", getConnections() | to_bits(d));
                        neighbour->setAttr("$connections", neighbour->getConnections() | to_bits(reverse(d)));
                    } else if (!neighbourCluster && neighbour->getConnections() & to_bits(reverse(d))) {
                        setAttr("$connections", getConnections() | to_bits(d));
                    } else {
                        setAttr("$connections", getConnections() & (ALL_DIRECTIONS ^ to_bits(d))); // clear connection
                    }
                } else if (neighbourCluster)  {// I have fixed connections -> adapt neighbour
                    if (getConnections() & to_bits(d))
                        neighbour->setAttr("$connections", neighbour->getConnections() | to_bits(reverse(d)));
                    else
                        neighbour->setAttr("$connections", neighbour->getConnections() & (ALL_DIRECTIONS ^ to_bits(reverse(d))));
                }
            } else if (myCluster) { // no neighbour -> no connection
                setAttr("$connections", getConnections() & (ALL_DIRECTIONS ^ to_bits(d))); // clear connection
            }
        }
    }
    
    void Stone::autoLeaveCluster() {
        GridPos p = get_pos();
        for (int i = WEST; i <= NORTH; i++) {
            Direction d = (Direction) i;
            Stone *neighbour = GetStone(move(p, d));
            if (isConnectable(neighbour) && neighbour->getAttr("cluster")) {
                neighbour->setAttr("$connections", neighbour->getConnections() & (ALL_DIRECTIONS ^ to_bits(reverse(d))));
            }
        }
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
        ReplaceStone(this_pos, MakeStone("st_death"));
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
        Actor     *ball = MakeActor("ac-cannonball");
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


// --------------------------------------------------------------------------------

void InitStones() {

    // Register(new ...);

    Register (new SpitterStone);

}
} // namespace enigma
