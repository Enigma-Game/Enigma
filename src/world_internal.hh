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

#include "ecl_array2.hh"
#include <list>
#include <memory>
#include <vector>

namespace enigma {

struct Field;
struct Signal;

typedef ecl::Array2<Field> FieldArray;

typedef std::vector<ForceField *> ForceList;
typedef std::list<Other *> OtherList;
typedef std::list<Rubberband *> RubberbandList;
typedef std::vector<Actor *> ActorList;
typedef std::vector<Signal> SignalList;

/* -------------------- MouseForce -------------------- */

/*! This class implements the "force field" that accelerates
  objects when the mouse is moved.  Only objects that have the
  "mouseforce" and the "controllers" attributes set are affected
  by this force field. */
class MouseForce {
public:
    void set_force(V2 f) { force = f; }
    void add_force(V2 f) { force += f; }

    V2 get_force(Actor *a) {
        if (a->is_flying() || a->is_dead())
            return V2();
        else
            return force * a->get_mouseforce();
    }

    void tick(double /*dtime*/) { force = V2(); }

private:
    V2 force;
};

/* -------------------- Scramble -------------------- */

/*! Stores all positions plus direction where puzzle scrambling
  should be performed */
struct Scramble {
    // Variables
    GridPos pos;
    Direction dir;
    int intensity;

    Scramble(GridPos p, Direction d, int i) : pos(p), dir(d), intensity(i) {}

    bool expired() const { return intensity < 1; }
};

/* -------------------- DelayedImpulse -------------------- */

class DelayedImpulse {
    Impulse impulse;
    double delay;
    const Stone *receiver;  // to test if stone has changed
    bool isReferenced;      // an itereator references this impulse
    bool isObsolete;        // the impulse should be deleted

    DelayedImpulse &operator=(const DelayedImpulse &other);  // forbidden
public:
    DelayedImpulse(const Impulse &impulse_, double delay_, const Stone *receiver_)
    : impulse(impulse_),
      delay(delay_),
      receiver(receiver_),
      isReferenced(false),
      isObsolete(false) {}

    DelayedImpulse(const DelayedImpulse &other)
    : impulse(other.impulse),
      delay(other.delay),
      receiver(other.receiver),
      isReferenced(other.isReferenced),
      isObsolete(other.isObsolete) {}

    bool tick(double dtime) {  // returns true if Impulse has to be sent NOW
        delay -= dtime;
        return delay <= 0;
    }

    const GridPos &destination() const { return impulse.dest; }

    bool is_receiver(const Stone *target) const { return target == receiver; }

    bool is_sender(const Stone *target) const { return target == impulse.sender; }

    bool is_referenced() const { return isReferenced; }

    void mark_referenced(bool state) { isReferenced = state; }

    bool is_obsolete() const { return isObsolete; }

    void mark_obsolete() { isObsolete = true; }

    void send_impulse(Stone *target) const {

        // @@@ FIXME: the test for equality of Stones sometimes fails
        // (e.g. with turnstiles rotated by rotators)
        //
        // I guess this happens, because when the turnstile-pivot
        // removes and add arms during turn, it may happen, that
        // the Stone receives the same memory address
        //
        // Possible fix : add unique ID to all objects

        if (is_receiver(target)) {
            // if object did not change since impulse was initiated
            target->on_impulse(impulse);
        }
    }
};

typedef list<DelayedImpulse> ImpulseList;

/* -------------------- Layer -------------------- */

template <class T>
class Layer {
    T *defaultval;

public:
    Layer(T *deflt = 0) : defaultval(deflt) {}
    virtual ~Layer() {}

    T *get(GridPos p);
    virtual T *yield(GridPos p);
    virtual void set(GridPos p, T *x);
    void kill(GridPos p) { dispose(yield(p)); }

    virtual T *raw_get(Field &) = 0;
    virtual void raw_set(Field &, T *) = 0;

private:
    virtual void dispose(T *x) {
        if (x)
            DisposeObject(x);
    }
};

/*
** Floor layer
*/
class FloorLayer : public Layer<Floor> {
public:
    Floor *yield(GridPos p) {
        Floor *f = Layer<Floor>::yield(p);
        if (f != NULL) {
            if (Value v = f->getAttr("name")) {
                NamePosition(p, v.to_string());
            }
        }
        return f;
    }

    void set(GridPos p, Floor *x) {
        Floor *f = get(p);
        if (f != NULL) {
            if (Value v = f->getAttr("name")) {
                NamePosition(p, v.to_string());
            }
        }
        Layer<Floor>::set(p, x);
    }

    Floor *raw_get(Field &f) { return f.floor; }
    void raw_set(Field &f, Floor *x) { f.floor = x; }
};

/*
** Item layer
*/
class ItemLayer : public Layer<Item> {
private:
    Item *raw_get(Field &f) { return f.item; }
    void raw_set(Field &f, Item *x) { f.item = x; }
};

/*
** Stone layer
*/
class StoneLayer : public Layer<Stone> {
public:
    StoneLayer() : Layer<Stone>(&borderstone) {}

private:
    Stone *raw_get(Field &f) { return f.stone; }
    void raw_set(Field &f, Stone *st) { f.stone = st; }
    void dispose(Stone *st) {
        if (st) {
            SendMessage(st, "disconnect");
            DisposeObject(st);
        }
    }

    /* This stone is used as the virtual border of the playing area.
       It is immovable and indestructible and makes sure the player's
       marble cannot leave the level. */
    class BorderStone : public Stone {
    public:
        BorderStone() : Stone("borderstone") {}
        Stone *clone() { return this; }
        void dispose() {}
        virtual const StoneTraits &get_traits() const {
            static StoneTraits border_traits = {"INVALID", st_borderstone, stf_none, material_stone,
                                                1.0};
            return border_traits;
        }
    };

    BorderStone borderstone;
};

/* ------------- Sound Damping List -------------- */

typedef list<sound::SoundDamping> SoundDampingList;

/* -------------------- World -------------------- */

/*! Contains the level information (in theory everything that is
  local to the current level; in practice a lot of things are
  still stored in global variables). */
class World {
public:
    World(int ww, int hh);
    ~World();

    bool contains(const GridPos &p) { return (p.x >= 0 && p.y >= 0 && p.x < w && p.y < h); }

    bool contains(const ecl::V2 &p) { return (p[0] >= 0 && p[1] >= 0 && p[0] < w && p[1] < h); }

    bool is_border(const GridPos &p);

    Field *get_field(GridPos p) {
        if (this->contains(p))
            return &fields(p.x, p.y);
        else
            return 0;
    }

    void name_object(Object *obj, const string &name);
    void unname(Object *);
    Object *get_named(const string &);
    std::list<Object *> get_group(const std::string &tmpl, Object *reference = NULL);
    void namePosition(Value po, const std::string &name);
    Value getNamedPosition(const std::string &name);
    PositionList getPositionList(const std::string &tmpl, Object *reference = NULL);

    void dispose_object(Object *obj) {
        if (obj) {
            unname(obj);
            obj->dispose();
        }
    }
    
    void tick(double dtime);
    void remove(ForceField *ff);

    void add_scramble(GridPos p, Direction dir);
    void scramble_puzzles();

    void add_actor(Actor *a);
    void add_actor(Actor *a, const V2 &pos);
    Actor *yield_actor(Actor *a);
    void exchange_actors(Actor *a1, Actor *a2);
    void did_move_actor(Actor *a);
    void tick_actor(Actor *a, double dtime);

private:
    /* ---------- Private methods ---------- */

    ecl::V2 drunkenMouseforce(Actor *a, V2 &mforce);
    V2 get_local_force(Actor *a);
    V2 get_global_force(Actor *a);

    void advance_actor(Actor *a, double &dt);
    void move_actors(double dtime);
    void find_contact_with_stone(Actor *a, GridPos p, StoneContact &c,
                                 DirectionBits winFacesActorStone = NODIRBIT, bool isRounded = true,
                                 Stone *st = NULL);
    void find_contact_with_edge(Actor *a, GridPos pe, GridPos p1, GridPos p2, StoneContact &c0,
                                StoneContact &c1, StoneContact &c2,
                                DirectionBits winFacesActorStone = NODIRBIT);
    void find_contact_with_window(Actor *a, GridPos p, StoneContact &c0, StoneContact &c1,
                                  DirectionBits winFacesActorStone);
    void find_stone_contacts(Actor *a, StoneContact &c0, StoneContact &c1, StoneContact &c2);
    void handle_stone_contact(StoneContact &sc);
    void handle_actor_contacts();
    void handle_actor_contact(Actor *actor1, Actor *actor2);
    void handle_stone_contacts(unsigned actoridx);
    void stone_change(GridPos p);
    void tick_sound_dampings();
    void doPerformPendingActions();

public:
    static const double contact_e;  // epsilon distant limit for contacts

    /* ---------- Variables ---------- */

    FieldArray fields;  // Contains floors, items, etc.
    int w, h;           // Width and height of the level
    ForceList forces;
    ActorList actorlist;    // List of movable, dynamic objects
    Actor *leftmost_actor;  // sorted double linked list of actors
    Actor *rightmost_actor;
    OtherList others;
    RubberbandList rubberbands;
    MouseForce m_mouseforce;
    ecl::V2 globalForce;
    int scrambleIntensity;
    int numMeditatists;
    int indispensableHollows;
    int engagedIndispensableHollows;
    int engagedDispensableHollows;

    //! True if game is not running yet
    bool preparing_level;

    std::vector<GridPos> changed_stones;
    std::list<GridPos> collisionCriticalPositions;  // stones set to positions after collision check
    bool registerCriticalPositions;
    std::list<Action> actionList;  // pending delayed actions for secure performance

    SoundDampingList sound_dampings;  // see SoundEffectManager for details

    FloorLayer fl_layer;
    ItemLayer it_layer;
    StoneLayer st_layer;

private:
    ecl::Dict<Object *> m_objnames;   // Name -> object mapping
    ecl::Dict<Value> namedPositions;  // Name -> position mapping

    list<Scramble> scrambles;
};

}  // namespace enigma
