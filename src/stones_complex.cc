/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
 *
 * This program is free software; you can redistribute it and/ or
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
#include "laser.hh"
#include "SoundEffectManager.hh"
#include "server.hh"
#include "player.hh"
#include "Inventory.hh"
#include "stones_internal.hh"
#include "stones/ConnectiveStone.hh"
#include "actors.hh"
#include "main.hh"
#include "Value.hh"

#include "ecl_util.hh"

#include <algorithm>
#include <iostream>

using namespace std;

namespace enigma { 


///* -------------------- Puzzle stones -------------------- */ 

/** \page st-puzzle Puzzle Stone

Puzzle stones can be connected to other stones of the same type.  Any
of the four faces of the stone can have ``socket''.  If the adjoining
faces of two neighboring stones both have a socket, the two stones
link up and henceforth move as group.

A cluster of puzzle stones may for example look like this:

\verbatim
+---+---+---+---+
|   |   |   |   |
| --+-+-+---+-+ |
|   | | |   | | |
+---+-+-+---+-+-+
    | | |   | | |
    | | |   | | |
    |   |   |   |
    +---+   +---+
\endverbatim

This example actually presents the special case of a ``complete''
cluster.  A cluster is complete if none of its stones has an
unconnected socket.

When touched with a magic wand the puzzle stones rotate
row- or columnwise.  

\subsection puzzlea Attributes

- \b connections
   number between 1 an 16.  Each bit in (connections-1) corresponds to
   a socket on one of the four faces.  You will normally simply use
   one of the Lua constants \c PUZ_0000 to \c PUZ_1111.

- \b oxyd
   If 1 then the puzzle stones act oxyd-compatible: Complete clusters
   explode, when they get touched. All other puzzle stones rotate row-
   or columnwise. Groups of oxyd-compatible puzzle stones are shuffled
   randomly at level startup.

\subsection puzzlee Example
<table>
<tr>
<td> \image html st-puzzletempl_0001.png "PUZ_0000"
<td> \image html st-puzzletempl_0002.png "PUZ_0001"
<td> \image html st-puzzletempl_0003.png "PUZ_0010"
<td> \image html st-puzzletempl_0004.png "PUZ_0011"
<tr>
<td> \image html st-puzzletempl_0005.png "PUZ_0100"
<td> \image html st-puzzletempl_0006.png "PUZ_0101"
<td> \image html st-puzzletempl_0007.png "PUZ_0110"
<td> \image html st-puzzletempl_0008.png "PUZ_0111"
<tr>
<td> \image html st-puzzletempl_0009.png "PUZ_1000"
<td> \image html st-puzzletempl_0010.png "PUZ_1001"
<td> \image html st-puzzletempl_0011.png "PUZ_1010"
<td> \image html st-puzzletempl_0012.png "PUZ_1011"
<tr>
<td> \image html st-puzzletempl_0013.png "PUZ_1100"
<td> \image html st-puzzletempl_0014.png "PUZ_1101"
<td> \image html st-puzzletempl_0015.png "PUZ_1110"
<td> \image html st-puzzletempl_0016.png "PUZ_1111"
</tr>
</table>
*/
namespace
{
    class PuzzleStone : public ConnectiveStone, public TimeHandler {
        INSTANCELISTOBJ(PuzzleStone);
        DECL_TRAITS;
    public:
        PuzzleStone(int connections, bool oxyd1_compatible_);  
        
        virtual void lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs);
        
    protected:
        virtual ~PuzzleStone() {
            GameTimer.remove_alarm (this);
        }     
    private:
        typedef vector<GridPos> Cluster;

        /* ---------- Private methods ---------- */

        bool oxyd1_compatible() const { return getAttr("oxyd") != 0; }

        static bool visit_dir(vector<GridPos> &stack, GridPos curpos,
                              Direction dir, int wanted_oxyd_attrib);
        static void visit_adjacent(vector<GridPos>& stack, GridPos curpos,
                                   Direction dir, int wanted_oxyd_attrib);

        bool find_cluster(Cluster &);
        void find_adjacents(Cluster &);
        void find_row_or_column_cluster(Cluster &c, GridPos startpos,
                                        Direction  dir, int wanted_oxyd_attrib);

        bool cluster_complete();
        bool can_move_cluster (Cluster &c, Direction dir);
        void maybe_move_cluster(Cluster &c, bool is_complete, bool actor_with_wand, 
                                Direction dir);
        void rotate_cluster(const Cluster &c);
        void maybe_rotate_cluster(Direction dir);

        int get_modelno() const;

        void        trigger_explosion(double delay);
        static void trigger_explosion_at(GridPos p, double delay, int wanted_oxyd_attrib);
        void        explode();
        bool        explode_complete_cluster();

        /* ---------- TimeHandler interface ---------- */

        void alarm();

        /* ---------- Stone interface ---------- */

        virtual Value message(const Message &m);

        void on_creation (GridPos p);
        void on_impulse (const Impulse& impulse);

        bool is_floating() const;

        StoneResponse collision_response(const StoneContact &sc);
        void actor_hit (const StoneContact &sc);
        void actor_contact (Actor *a);

        /* ---------- Variables ---------- */
        bool visited;           // flag for DFS
        enum { IDLE, EXPLODING } state;
    };
    DEF_TRAITSM(PuzzleStone, "st-puzzle", st_puzzle, MOVABLE_IRREGULAR);
}

PuzzleStone::InstanceList PuzzleStone::instances;

PuzzleStone::PuzzleStone(int connections, bool oxyd1_compatible_) : ConnectiveStone(connections), 
  state (IDLE)
//  illumination (NODIRBIT)
{
    setAttr("oxyd", int(oxyd1_compatible_));
}


bool PuzzleStone::visit_dir(vector<GridPos> &stack, GridPos curpos, 
        Direction dir, int wanted_oxyd_attrib)
{
    GridPos newpos = move(curpos, dir);
    PuzzleStone *pz = dynamic_cast<PuzzleStone*>(GetStone(newpos));

    if ((!pz) || (wanted_oxyd_attrib != (int)pz->getAttr("oxyd")))
        return false;

    DirectionBits cfaces = pz->get_connections();

    if (cfaces==NODIRBIT || has_dir(cfaces, reverse(dir))) {
        // Puzzle stone at newpos is connected to stone at curpos
        if (!pz->visited) {
            pz->visited = true;
            stack.push_back(newpos);
        }
        return true;
    } else {
        // The two stones are adjacent but not connected
        return false;
    }
}

/* Use a depth first search to determine the group of all stones that
   are connected to the current stone.  Returns true if the cluster is
   ``complete'' in the sense defined above. */
bool PuzzleStone::find_cluster(Cluster &cluster) {
    for (unsigned i=0; i<instances.size(); ++i)
        instances[i]->visited=false;

    vector<GridPos> pos_stack;
    bool is_complete = true;
    pos_stack.push_back(get_pos());
    this->visited = true;
    int wanted_oxyd_attrib = getAttr("oxyd");

    while (!pos_stack.empty())
    {
        GridPos curpos = pos_stack.back();
        pos_stack.pop_back();

        PuzzleStone *pz = dynamic_cast<PuzzleStone*>(GetStone(curpos));
        ASSERT(pz, XLevelRuntime, "PuzzleStone: missing stone in find_cluster");

        cluster.push_back(curpos);
        DirectionBits cfaces = pz->get_connections();

        if (cfaces==NODIRBIT)
            cfaces = DirectionBits(NORTHBIT | SOUTHBIT | EASTBIT | WESTBIT);

        if (has_dir(cfaces, NORTH))
            is_complete &= visit_dir(pos_stack, curpos, NORTH, wanted_oxyd_attrib);
        if (has_dir(cfaces, EAST))
            is_complete &= visit_dir(pos_stack, curpos, EAST, wanted_oxyd_attrib);
        if (has_dir(cfaces, SOUTH))
            is_complete &= visit_dir(pos_stack, curpos, SOUTH, wanted_oxyd_attrib);
        if (has_dir(cfaces, WEST))
            is_complete &= visit_dir(pos_stack, curpos, WEST, wanted_oxyd_attrib);
    }
    return is_complete;
}

void PuzzleStone::visit_adjacent (vector<GridPos>& stack, GridPos curpos, 
                                  Direction dir, int wanted_oxyd_attrib)
{
    GridPos newpos = move(curpos, dir);
    if (PuzzleStone *pz = dynamic_cast<PuzzleStone*>(GetStone(newpos))) {
        if (!pz->visited) {
            if (wanted_oxyd_attrib == (int)pz->getAttr("oxyd")) {
                pz->visited = true;
                stack.push_back(newpos);
            }
        }
    }
}

/* Use a depth first search to determine the group of all puzzle stones
   with the same "oxyd" attrib that are adjacent to the current stone
   (or to any other member of the group).
*/
void PuzzleStone::find_adjacents(Cluster &cluster) {
    for (unsigned i=0; i<instances.size(); ++i)
        instances[i]->visited=false;

    vector<GridPos> pos_stack;
    pos_stack.push_back(get_pos());
    this->visited = true;

    int wanted_oxyd_attrib = getAttr("oxyd");

    while (!pos_stack.empty()) {
        GridPos curpos = pos_stack.back();
        pos_stack.pop_back();

        cluster.push_back(curpos);
        visit_adjacent(pos_stack, curpos, NORTH, wanted_oxyd_attrib);
        visit_adjacent(pos_stack, curpos, SOUTH, wanted_oxyd_attrib);
        visit_adjacent(pos_stack, curpos, EAST, wanted_oxyd_attrib);
        visit_adjacent(pos_stack, curpos, WEST, wanted_oxyd_attrib);
    }
}

/* searches from 'startpos' into 'dir' for puzzle-stones.
   wanted_oxyd_attrib == -1 -> take any puzzle stone
                      else  -> take only puzzle stones of same type
*/

void PuzzleStone::find_row_or_column_cluster(Cluster &c, GridPos startpos, 
                                             Direction dir, int wanted_oxyd_attrib)
{
    ASSERT(dir != NODIR, XLevelRuntime,
        "PuzzleStone: no direction in find_row_or_column_cluster");

    GridPos p = startpos;
    while (Stone *puzz = dynamic_cast<PuzzleStone*>(GetStone(p))) {
        if (wanted_oxyd_attrib != -1 && wanted_oxyd_attrib != (int)puzz->getAttr("oxyd"))
            break; // stop when an unrequested puzzle stone type is readed
        c.push_back(p);
        p.move(dir);
    }
}

bool PuzzleStone::can_move_cluster (Cluster &c, Direction dir)
{
    sort(c.begin(), c.end());
    Cluster mc(c);              // Moved cluster
    Cluster diff;               // Difference between mc and c

    for (unsigned i=0; i<mc.size(); ++i)
        mc[i].move(dir);

    set_difference(mc.begin(), mc.end(), c.begin(), c.end(),
                   back_inserter(diff));

    // Now check whether all stones can be placed at their new
    // position
    bool move_ok = true;
    for (unsigned i=0; i<diff.size(); ++i)
        if (GetStone(diff[i]) != 0)
            move_ok = false;

    return move_ok;    
}

void PuzzleStone::maybe_move_cluster(Cluster &c, bool is_complete, 
                                     bool actor_with_wand, Direction dir)
{
    sort(c.begin(), c.end());
    Cluster mc(c);              // Moved cluster
    Cluster diff;               // Difference between mc and c

    for (unsigned i=0; i<mc.size(); ++i)
        mc[i].move(dir);

    set_difference(mc.begin(), mc.end(), c.begin(), c.end(),
                   back_inserter(diff));

    // Now check whether all stones can be placed at their new
    // position
    bool move_ok = true;
    for (unsigned i=0; i<diff.size(); ++i)
        if (GetStone(diff[i]) != 0)
            move_ok = false;

    if (!move_ok)
        return;

    // If the floor at a complete cluster's new position consists
    // exclusively of abyss or water, create a bridge instead of
    // moving the cluster.
    //
    // For partial clusters build bridges only on water and if the
    // wielded item is NOT the magic wand.

    bool create_bridge = true;

    for (unsigned i=0; create_bridge && i<mc.size(); ++i) {
        if (Floor *fl = GetFloor(mc[i])) {
            if (fl->is_kind("fl-abyss")) {
                if (!is_complete)
                    create_bridge = false;
            }
            else if (fl->is_kind("fl-water")) {
                if (!is_complete && actor_with_wand) 
                    create_bridge = false;
            }
            else
                create_bridge = false;
        }
    }

    // Finally, either move the whole cluster or create a bridge
    sound_event("movebig");
    if (create_bridge) {
        for (unsigned i=0; i<c.size(); ++i) {
            KillStone(c[i]);
            SetFloor(mc[i], MakeFloor("fl-gray"));
        }
    } 
    else {
        vector<Stone*> clusterstones;
        for (unsigned i=0; i<c.size(); ++i)
            clusterstones.push_back(YieldStone(c[i]));

        for (unsigned i=0; i<c.size(); ++i) {
            SetStone(mc[i], clusterstones[i]);
            clusterstones[i]->on_move();
        }
    }

    server::IncMoveCounter (static_cast<int> (c.size()));
}

bool PuzzleStone::cluster_complete() {
    Cluster c;
    return find_cluster(c);
}

int PuzzleStone::get_modelno() const {
    int modelno = getAttr("connections");
    if (oxyd1_compatible()) modelno += 16;
    return modelno;
}

void PuzzleStone::rotate_cluster(const Cluster &c) {
    size_t size = c.size();
    if (size > 1) {
        int cn = GetStone(c[size-1])->getAttr("connections");
        for (size_t i=size-1; i>0; --i) {
            PuzzleStone *st = dynamic_cast<PuzzleStone*> (GetStone (c[i]));
            st->setAttr("connections", GetStone(c[i-1])->getAttr("connections"));
            st->init_model();
        }
        GetStone(c[0])->setAttr("connections", cn);
        dynamic_cast<PuzzleStone*> (GetStone(c[0]))->init_model();
    }
}

StoneResponse PuzzleStone::collision_response(const StoneContact &/*sc*/) {
    if (get_connections() == NODIRBIT)
        return STONE_PASS;
    return STONE_REBOUND;
}

void PuzzleStone::trigger_explosion(double delay) {
    if (state == IDLE) {
        state = EXPLODING;
        GameTimer.set_alarm(this, delay, false);
    }
}

void PuzzleStone::trigger_explosion_at (GridPos p, double delay,
                                        int wanted_oxyd_attrib)
{
    PuzzleStone *puzz = dynamic_cast<PuzzleStone*>(GetStone(p));
    if (puzz && wanted_oxyd_attrib == (int)puzz->getAttr("oxyd")) {
        // explode adjacent puzzle stones of same type
        puzz->trigger_explosion(delay);
    }
}

void PuzzleStone::explode() {
    GridPos p       = get_pos();
    int     ox_attr = getAttr("oxyd");

    // exchange puzzle stone with explosion
    sound_event("stonedestroy");
    SetStone(p, MakeStone("st-explosion"));

    // trigger all adjacent puzzle stones :
    const double DEFAULT_DELAY = 0.2;
    trigger_explosion_at(move(p, NORTH), DEFAULT_DELAY, ox_attr);
    trigger_explosion_at(move(p, SOUTH), DEFAULT_DELAY, ox_attr);
    trigger_explosion_at(move(p, EAST),  DEFAULT_DELAY, ox_attr);
    trigger_explosion_at(move(p, WEST),  DEFAULT_DELAY, ox_attr);

    // @@@ FIXME: At the moment it's possible to push partial puzzle stones
    // next to an already exploding cluster. Then the part will explode as well.
    // Possible fix : mark whole cluster as "EXPLODING_SOON" when explosion is initiated

    // ignite adjacent fields
//     SendExplosionEffect(p, DYNAMITE);
}

void PuzzleStone::alarm() {
    explode();
}

Value PuzzleStone::message(const Message &m) {
    if (m.message == "scramble") {
        // oxyd levels contain explicit information on how to
        // scramble puzzle stones. According to that information
        // a "scramble" message is send to specific puzzle stones
        // together with information about the direction.
        //
        // enigma levels may create scramble messages using
        // AddScramble() and SetScrambleIntensity()

        Direction dir = to_direction(m.value);
        Cluster   c;
        find_row_or_column_cluster(c, get_pos(), dir, oxyd1_compatible());

        size_t size = c.size();

        // warning("received 'scramble'. dir=%s size=%i", to_suffix(dir).c_str(), size);

        if (size >= 2) {
            int count = IntegerRand(0, static_cast<int> (size-1));
            while (count--)
                rotate_cluster(c);
        }
        else {
            warning("useless scramble (cluster size=%i)", size);
        }
        return Value();
    }
    return ConnectiveStone::message(m);
}

void PuzzleStone::on_impulse(const Impulse& impulse) 
{
//    if (!oxyd1_compatible() && state == IDLE) {
    if (state == IDLE) {
        Cluster c;
        bool    is_complete     = find_cluster(c);
        bool    actor_with_wand = false;

        if (Actor *ac = dynamic_cast<Actor*>(impulse.sender)) 
            actor_with_wand = player::WieldedItemIs (ac, "it_magicwand");

        maybe_move_cluster(c, is_complete, actor_with_wand, impulse.dir);
    }
}

bool PuzzleStone::explode_complete_cluster() 
{
    // @@@ FIXME: explode_complete_cluster should mark the whole cluster
    // as "EXPLODING_SOON" (otherwise it may be changed before it explodes completely)

    ASSERT(state == IDLE, XLevelRuntime,
        "PuzzleStone: explode_complete_cluster called with inconsistent state");
    bool exploded = false;

    Cluster complete;
    if (find_cluster(complete)) {
        Cluster all;
        find_adjacents(all);

        // If all adjacent stones build one complete cluster explode it
        if (all.size() == complete.size()) {
            explode();          // explode complete cluster
            exploded = true;
        }
        else {
            ASSERT(all.size() > complete.size(), XLevelRuntime,
                "PuzzleStone: sizes don't match in explode_complete_cluster");
            if (!oxyd1_compatible()) {
                // check if 'all' is made up of complete clusters :

                sort(all.begin(), all.end());

                while (1) {
                    sort(complete.begin(), complete.end());

                    // remove one complete cluster from 'all'
                    {
                        Cluster rest;
                        set_symmetric_difference(all.begin(), all.end(),
                                                 complete.begin(), complete.end(),
                                                 back_inserter(rest));
                        // now rest contains 'all' minus 'complete'
                        swap(all, rest);
                    }

                    if (all.empty()) { // none left -> all were complete
                        exploded = true;
                        break;
                    }

                    // look for next complete cluster :
                    complete.clear();
                    {
                        PuzzleStone *pz = dynamic_cast<PuzzleStone*>(GetStone(all[0]));
                        ASSERT(pz, XLevelRuntime,
                            "PuzzleStone: missing stone in explode_complete_cluster");
                        if (!pz->find_cluster(complete)) {
                            break; // incomplete cluster found -> don't explode
                        }
                    }
                }

                if (exploded) {
//                     warning("exploding complete cluster");
                    explode();
                }
            }
        }
    }

    return exploded;
}

bool PuzzleStone::is_floating() const {
    return get_connections() == 0;
}

void PuzzleStone::maybe_rotate_cluster(Direction dir) 
{
    if (dir != NODIR) {
        Cluster c;
        find_row_or_column_cluster(c, get_pos(), dir, (int)getAttr("oxyd"));
        if (c.size() >= 2) {
//             warning("ok -> rotate");
            rotate_cluster(c);
        }
    }
}

void PuzzleStone::on_creation (GridPos p) {
    activatePhoto();
    ConnectiveStone::on_creation (p);
}


void PuzzleStone::lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs) {
    if (state == IDLE && newDirs != NODIRBIT) {
        if (!explode_complete_cluster() && oxyd1_compatible()) {
            if (newDirs & SOUTHBIT) maybe_rotate_cluster(SOUTH);
            if (newDirs & NORTHBIT) maybe_rotate_cluster(NORTH);
            if (newDirs & WESTBIT)  maybe_rotate_cluster(WEST);
            if (newDirs & EASTBIT)  maybe_rotate_cluster(EAST);
        }
    }
}

void PuzzleStone::actor_hit(const StoneContact &sc) 
{
    if (get_connections() == NODIRBIT)
        return;                 // Puzzle stone is hollow

    if (state == EXPLODING)
        return;

    Cluster c;
    find_cluster (c);

    Direction rotate_dir = reverse (contact_face (sc));
    Direction move_dir = get_push_direction(sc);

    if (oxyd1_compatible()) {
        // Oxyd 1

        if (explode_complete_cluster())
            return;

        // 1) If unconnected puzzle stones -> try to move it
        if (c.size() == 1 && move_dir != NODIR) {
            // if cluster contains single stone
            // -> move it if dest pos is free
            GridPos dest = move(c[0], move_dir);
            if (GetStone(dest) == 0) {
                Stone *puzz = YieldStone(c[0]);
                SetStone(dest, puzz);
                puzz->on_move();
                sound_event ("movesmall");
            } else
                maybe_rotate_cluster (rotate_dir);
        } 
        // 2) If more than one stone, 
        else 
            maybe_rotate_cluster (rotate_dir);
    }
    else {
        // Not Oxyd 1

        bool has_magic_wand = player::WieldedItemIs (sc.actor, "it_magicwand");

        // 1) Try to start explosion of complete cluster
        if (has_magic_wand && explode_complete_cluster())
            return;

        // 2) Failed? Try to move the cluster
        if (move_dir != NODIR && can_move_cluster (c, move_dir)) {
            sc.actor->send_impulse(get_pos(), move_dir);
            return;
        }

        // 3) Last chance: try to rotate the row or column 
        if (has_magic_wand)
            maybe_rotate_cluster (rotate_dir);
    }
}

void PuzzleStone::actor_contact (Actor *a)
{
    if (state == EXPLODING)
        SendMessage(a, "shatter");
}


/* -------------------- Stone impulse stones -------------------- */

// Messages:
//
// :trigger
namespace
{
    class StoneImpulse_Base : public Stone {
    protected:
        StoneImpulse_Base() : state(IDLE), incoming(NODIR), nobackfire (false)
        {}

        enum State { IDLE, PULSING, CLOSING };
        State     state;
        Direction incoming; // direction of incoming impulse (may be NODIR)
        bool      nobackfire;

        void change_state(State st);

        virtual void on_impulse(const Impulse& impulse) {
            incoming = impulse.dir;
            change_state(PULSING);
        }

    private:

        virtual void notify_state(State st) = 0;

        virtual Value message(const Message &m) {
            if (m.message == "_trigger" && m.value.to_bool()) {
                incoming = NODIR;
                if (m.sender != NULL)
                    incoming = direction_fromto(dynamic_cast<GridObject *>(m.sender)->get_pos(), get_pos());
                
                if (state == IDLE && incoming != NODIR) {
                    nobackfire = true;
                }
                change_state(PULSING);
                return Value();
            }
            else if (m.message == "signal" && (to_double(m.value) != 0 || 
                    (server::EnigmaCompatibility < 1.10 /*&& m.value.getType() == Value::NIL*/))) { // hack for old trigger without value
                incoming = NODIR;
                change_state (PULSING);
                return Value();
            }
            return Stone::message(m);
        }

        void animcb() {
            if (state == PULSING)
                change_state (CLOSING);
            else if (state == CLOSING)
                change_state (IDLE);
        }

        void processLight(Direction dir) {
            incoming = dir;
            change_state(PULSING);
        }

        FreezeStatusBits get_freeze_bits() { return FREEZEBIT_NO_STONE; }
    };
}

void StoneImpulse_Base::change_state(State new_state) {
    if (new_state == state) return;

    GridPos p = get_pos();
    switch (new_state) {
        case IDLE: {
            state = new_state;
            notify_state(state);
            break;
        }
        case PULSING:
            if (state != IDLE) {
                return;         // do not set new state
            }
            state = new_state;
            notify_state(state);
            sound_event("impulse");
            break;
        case CLOSING: {
            GridPos targetpos[4];
            bool    haveStone[4];

            // set CLOSING model _before_ sending impulses !!!
            // (any impulse might have side effects that move this stone)

            state = new_state;
            notify_state(state);

            for (int d = 0; d < 4; ++d) {
                targetpos[d] = move(p, Direction(d));
                haveStone[d] = GetStone(targetpos[d]) != 0;
            }

            for (int d = int(incoming)+1; d <= int(incoming)+4; ++d) {
                int D = d%4;
                if (!nobackfire || reverse(incoming) != D) {
                    if (haveStone[D]) {
                        send_impulse(targetpos[D], Direction(D));
                    }
                }
            }

            incoming = NODIR;   // forget impulse direction
            nobackfire = false;
            break;
        }
    }
}


namespace
{
    class StoneImpulseStone : public StoneImpulse_Base {
        CLONEOBJ(StoneImpulseStone);
        DECL_TRAITS;
    public:
        StoneImpulseStone()
        {}

    private:
        void notify_state(State st) {
            switch (st) {
            case IDLE:
                init_model();
                break;
            case PULSING:
                set_anim("st-stoneimpulse-anim1");
                break;
            case CLOSING:
                set_anim("st-stoneimpulse-anim2");
                break;
            }
        }

        void actor_hit(const StoneContact &/*sc*/) {
            change_state(PULSING);
        }
    };
    DEF_TRAITS(StoneImpulseStone, "st-stoneimpulse", st_stoneimpulse);

    class HollowStoneImpulseStone : public StoneImpulse_Base {
        CLONEOBJ(HollowStoneImpulseStone);
        DECL_TRAITS;
    public:
        HollowStoneImpulseStone()
        {}
    private:
        void notify_state(State st) {
            switch (st) {
            case IDLE:
                init_model();
                MaybeRecalcLight(get_pos());
                break;
            case PULSING:
                MaybeRecalcLight(get_pos());
                set_anim("st-stoneimpulse-hollow-anim1");
                break;
            case CLOSING:
                set_anim("st-stoneimpulse-hollow-anim2");
                break;
            }
        }

        StoneResponse collision_response(const StoneContact &/*sc*/) {
            return (state == IDLE) ? STONE_PASS : STONE_REBOUND;
        }
        void actor_inside(Actor *a) {
            if (state == PULSING || state == CLOSING)
                SendMessage(a, "shatter");
        }

        bool is_floating () const {
            return true;
        }

        void processLight(Direction d) {
            // hollow StoneImpulseStones cannot be activated using lasers
        }
    };
    DEF_TRAITS(HollowStoneImpulseStone, "st-stoneimpulse-hollow",
               st_stoneimpulse_hollow);

    class MovableImpulseStone : public StoneImpulse_Base {
        CLONEOBJ(MovableImpulseStone);
        DECL_TRAITS;
    public:
        MovableImpulseStone()
        : repulse(false)
        {}

    private:

        void notify_state(State st) {
            switch (st) {
            case IDLE:
                if (repulse) {
                    repulse = false;
                    change_state(PULSING);
                }
                else
                    init_model();
                break;
            case PULSING:
                set_anim("st-stoneimpulse-anim1");
                break;
            case CLOSING:
                set_anim("st-stoneimpulse-anim2");
                break;
            }
        }

        void init_model() {
            set_model("st-stoneimpulse");
        }

        // Stone interface:

        void actor_hit(const StoneContact &sc) {
            if (!maybe_push_stone (sc)) {
                incoming = NODIR; // bad, but no real problem!
                if (state == IDLE)
                    change_state(PULSING);
            }
        }

        void on_impulse(const Impulse& impulse) {
            State oldstate = state;

            if (move_stone(impulse.dir)) {
                notify_state(oldstate); // restart anim if it was animated before move

                Actor *hitman = dynamic_cast<Actor*>(impulse.sender);
                if (hitman && player::WieldedItemIs (hitman, "it_magicwand")) {
                    return;     // do not change state to PULSING
                }
            }

            if (state == IDLE)
                change_state(PULSING);
        }

        void on_move() {
            if (state != PULSING)
                repulse = true; // pulse again
            Stone::on_move();
        }

        // Variables.
        bool repulse;
    };
    DEF_TRAITSM(MovableImpulseStone, "st-stoneimpulse_movable",
                st_stoneimpulse_movable, MOVABLE_STANDARD);
}

// --------------------------------------------------------------------------------

void Init_complex()
{
    Register(new HollowStoneImpulseStone);

    Register(new MovableImpulseStone);

    // PerOxyd/Enigma compatible puzzle stones:
    Register(new PuzzleStone(0, false));
    Register("st-puzzle-hollow", new PuzzleStone(1, false));
    Register("st-puzzle-n", new PuzzleStone(9, false));
    Register("st-puzzle-e", new PuzzleStone(5, false));
    Register("st-puzzle-s", new PuzzleStone(3, false));
    Register("st-puzzle-w", new PuzzleStone(2, false));
    Register("st-puzzle-ne", new PuzzleStone(13, false));
    Register("st-puzzle-nw", new PuzzleStone(10, false));
    Register("st-puzzle-es", new PuzzleStone(7, false));
    Register("st-puzzle-sw", new PuzzleStone(4, false));
    Register("st-puzzle-ns", new PuzzleStone(11, false));
    Register("st-puzzle-ew", new PuzzleStone(6, false));
    Register("st-puzzle-nes", new PuzzleStone(15, false));
    Register("st-puzzle-new", new PuzzleStone(14, false));
    Register("st-puzzle-nsw", new PuzzleStone(12, false));
    Register("st-puzzle-esw", new PuzzleStone(8, false));
    Register("st-puzzle-nesw", new PuzzleStone(16, false));

    // Oxyd1 compatible puzzle stones:
    Register("st-puzzle2-hollow", new PuzzleStone(1, true));
    Register("st-puzzle2-n", new PuzzleStone(9, true));
    Register("st-puzzle2-e", new PuzzleStone(5, true));
    Register("st-puzzle2-s", new PuzzleStone(3, true));
    Register("st-puzzle2-w", new PuzzleStone(2, true));
    Register("st-puzzle2-ne", new PuzzleStone(13, true));
    Register("st-puzzle2-nw", new PuzzleStone(10, true));
    Register("st-puzzle2-es", new PuzzleStone(7, true));
    Register("st-puzzle2-sw", new PuzzleStone(4, true));
    Register("st-puzzle2-ns", new PuzzleStone(11, true));
    Register("st-puzzle2-ew", new PuzzleStone(6, true));
    Register("st-puzzle2-nes", new PuzzleStone(15, true));
    Register("st-puzzle2-new", new PuzzleStone(14, true));
    Register("st-puzzle2-nsw", new PuzzleStone(12, true));
    Register("st-puzzle2-esw", new PuzzleStone(8, true));
    Register("st-puzzle2-nesw", new PuzzleStone(16, true));

    Register(new StoneImpulseStone);

}

} // namespace enigma
