/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
 * Copyright (C) 2007,2008,2009,2010 Ronald Lamprecht
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
#include "world.hh"
#include "world_internal.hh"

#include "errors.hh"
#include "laser.hh"
#include "player.hh"
#include "SoundEffectManager.hh"
#include "SoundEngine.hh"
#include "options.hh"
#include "server.hh"
#include "lua.hh"
#include "client.hh"
#include "main.hh"
#include "stones_internal.hh"
#include "WorldProxy.hh"
#include "stones/OxydStone.hh"

#include <iostream>
#include <algorithm>
#include <functional>
#include <map>
#include <numeric>

// remove comment from define below to switch on verbose messaging
// note: VERBOSE_MESSAGES is defined in multiple source files!
// #define VERBOSE_MESSAGES

using namespace std;
using namespace ecl;

namespace enigma {

// Size of one time step -- do not change!
const double ActorTimeStep = 0.0025;

/* -------------------- Auxiliary functions -------------------- */

namespace {

/*! Find an already existing contact point in the ContactList that is
  similar to the second argument. */
bool has_nearby_contact(const Contact *ca, int ca_count, const Contact &c) {
    double posdelta = 0.2;
    double normaldelta = 0.1;
    for (int i = 0; i < ca_count; ++i) {
        if (length(ca[i].pos - c.pos) < posdelta && length(ca[i].normal - c.normal) < normaldelta)
            return true;
    }
    return false;
}
}

/* -------------------- ActorInfo -------------------- */

ActorInfo::ActorInfo()
: pos(),
  gridpos(),
  field(nullptr),
  charge(0),
  mass(1),
  radius(1),
  grabbed(false),
  created(false),
  ignore_contacts(false),
  friction(0.0),
  contacts(&contacts_a[0]),
  last_contacts(&contacts_b[0]),
  contacts_count(0),
  last_contacts_count(0) {
}

/* -------------------- Messages -------------------- */

Message::Message() : sender(nullptr) {
}

Message::Message(std::string theMessage, const Value &theValue, Object *theSender)
: message(std::move(theMessage)), value(theValue), sender(theSender) {
}

/* -------------------- Field -------------------- */

Field::Field() {
    floor = nullptr;
    item = nullptr;
    stone = nullptr;
}

Field::~Field() {
    // the field should be empty at this point, but for security reasons ...
    DisposeObject(stone);
    stone = nullptr;
    DisposeObject(item);
    item = nullptr;
    DisposeObject(floor);
    floor = nullptr;
}

/* -------------------- StoneContact -------------------- */

StoneContact::StoneContact()
: is_collision(false), ignore(false), new_collision(false), is_contact(false), outerCorner(false) {
}

DirectionBits contact_faces(const StoneContact &sc) {
    int dirs = NODIRBIT;

    if (sc.normal[0] < 0)
        dirs |= WESTBIT;
    else if (sc.normal[0] > 0)
        dirs |= EASTBIT;
    if (sc.normal[1] < 0)
        dirs |= NORTHBIT;
    else if (sc.normal[1] > 0)
        dirs |= SOUTHBIT;

    return DirectionBits(dirs);
}

Direction contact_face(const StoneContact &sc) {
    using namespace enigma;
    if (sc.normal == V2(-1, 0))
        return WEST;
    else if (sc.normal == V2(1, 0))
        return EAST;
    else if (sc.normal == V2(0, -1))
        return NORTH;
    else if (sc.normal == V2(0, 1))
        return SOUTH;
    else
        return NODIR;
}

/* -------------------- Global variables -------------------- */

namespace {

std::unique_ptr<World> level;

}  // namespace

enigma::Timer GameTimer;
bool TrackMessages;

/* -------------------- Layer implementation -------------------- */

template <class T>
T *Layer<T>::get(GridPos p) {
    if (Field *f = level->get_field(p))
        return raw_get(*f);
    else
        return defaultval;
}

template <class T>
T *Layer<T>::yield(GridPos p) {
    if (Field *f = level->get_field(p)) {
        T *x = raw_get(*f);
        if (x) {
            raw_set(*f, nullptr);
            x->removal(p);
        }
        return x;
    } else
        return defaultval;
}

template <class T>
void Layer<T>::set(GridPos p, T *x) {
    if (x) {
        if (Field *f = level->get_field(p)) {
            if (T *old = raw_get(*f)) {
                old->removal(p);
                dispose(old);
            }
            raw_set(*f, x);
            x->creation(p);
        } else
            dispose(x);
    }
}

/* -------------------- World -------------------- */

const double World::contact_e = 0.02;  // epsilon distant limit for contacts

World::World(int ww, int hh)
: fields(ww, hh),
  leftmost_actor(nullptr),
  rightmost_actor(nullptr),
  scrambleIntensity(10),  // difficult default
  numMeditatists(0),
  indispensableHollows(0),
  engagedIndispensableHollows(0),
  engagedDispensableHollows(0),
  preparing_level(true),
  registerCriticalPositions(false) {
    w = ww;
    h = hh;
}

World::~World() {
    // remove wires, rubberbands first, as they disconnect from other objects
    while (!others.empty()) {
        Other *ot = others.back();
        others.pop_back();
        ot->dispose();
    }
    // dispose all grid objects without removing them to avoid side effects
    // keep the field array still valid for arbitrary access
    for (auto &field : fields) {
        dispose_object(field.stone);
        field.stone = nullptr;
        dispose_object(field.item);
        field.item = nullptr;
        dispose_object(field.floor);
        field.floor = nullptr;
    }
    // reset the fields
    fields = FieldArray(0, 0);
    for_each(actorlist.begin(), actorlist.end(), mem_fun(&Actor::dispose));
}

bool World::is_border(const GridPos &p) {
    return (p.x == 0 || p.y == 0 || p.x == w - 1 || p.y == h - 1);
}

void World::remove(ForceField *ff) {
    auto i = find(forces.begin(), forces.end(), ff);
    if (i != forces.end())
        forces.erase(i);
}

Object *World::get_named(const std::string &name) {
    std::string wanted = name;
    if (wanted.size() > 0 && wanted[0] == '@')
        wanted.erase(0, 1);  // erase a leading @
    if (wanted.size() > 0 && wanted[0] == '@')
        wanted.erase(0, 1);  // erase a leading @@
    ecl::Dict<Object *>::iterator found = m_objnames.find(wanted);
    if (found != m_objnames.end())
        return found->second;
    //    Log << "Did not find named object: " << name << '\n';
    return nullptr;
}

std::list<Object *> World::get_group(const std::string &tmpl, Object *reference) {
    std::list<Object *> result;
    ecl::Dict<Object *>::iterator it = m_objnames.begin();
    std::string pattern = tmpl;
    bool nearest = false;  // limit result to nearest object
    double mindist = -1;
    if (pattern.size() > 0 && pattern[0] == '@') {
        pattern.erase(0, 1);  // erase the leading @
        if (pattern.size() == 0) {
            // a single "@" is a self reference
            result.push_back(reference);
            return result;
        } else if (pattern[0] == '@') {
            pattern.erase(0, 1);  // erase the leading @@
        }
        if (reference != nullptr)
            nearest = true;
    }

    for (; it != m_objnames.end(); ++it) {
        if (string_match(it->first, pattern)) {
            if (!nearest || mindist < 0) {
                result.push_back(it->second);
                if (nearest) {
                    mindist = reference->squareDistance(it->second);
                }
            } else {
                double newdist = reference->squareDistance(it->second);

                // replace last candidate by new closer object, choose a unique candidate
                if (mindist > newdist ||
                    ((mindist == newdist) && it->second->isSouthOrEastOf(result.back()))) {
                    result.pop_back();
                    result.push_back(it->second);
                    mindist = newdist;
                }
            }
        }
    }
    return result;
}

void World::name_object(Object *obj, const std::string &name) {
    Object *old = get_named(name);
    if (old != nullptr)
        unname(old);

    std::string unique_name = name;
    if (name.size() > 0 && name[name.size() - 1] == '#') {
        // auto name object with a unique name
        int i;
        //        for (i = 1; get_named(name + ecl::strf("%d",i)) != NULL; i++);
        for (i = IntegerRand(1, 999999); get_named(name + ecl::strf("%d", i)) != nullptr;
             i = IntegerRand(1, 999999))
            ;
        unique_name = name + ecl::strf("%d", i);
    }
    m_objnames.insert(unique_name, obj);  // [name] = obj;
    obj->setAttr("name", unique_name);
}

void World::unname(Object *obj) {
    ASSERT(obj, XLevelRuntime, "unname: no object given");
    if (Value v = obj->getAttr("name")) {
        m_objnames.remove(v.to_string());
        obj->setAttr("name", Value());
    }
}

void World::namePosition(Value po, const std::string &name) {
    namedPositions.remove(name);  // need to remove to avoid duplicate inserts in ecl::dict
    namedPositions.insert(name, po);
}

Value World::getNamedPosition(const std::string &name) {
    if (Object *o = get_named(name)) {
        switch (o->getObjectType()) {
        case Object::STONE:
        case Object::FLOOR:
        case Object::ITEM: return dynamic_cast<GridObject *>(o)->get_pos();
        case Object::ACTOR: return dynamic_cast<Actor *>(o)->get_pos();
        default: return Value(GridPos(-1, -1));
        }
    }

    std::string wanted = name;
    if (wanted.size() > 0 && wanted[0] == '@')
        wanted.erase(0, 1);  // erase a leading @
    if (wanted.size() > 0 && wanted[0] == '@')
        wanted.erase(0, 1);  // erase a leading @@
    ecl::Dict<Value>::iterator found = namedPositions.find(wanted);
    if (found != namedPositions.end())
        return found->second;
    //    Log << "Did not find named position: " << name << '\n';
    return Value(GridPos(-1, -1));
}

PositionList World::getPositionList(const std::string &tmpl, Object *reference) {
    PositionList positions;
    ObjectList objects = get_group(tmpl, reference);
    for (auto &object : objects) {
        switch (object->getObjectType()) {
        case Object::STONE:
        case Object::FLOOR:
        case Object::ITEM:
            positions.push_back(dynamic_cast<GridObject *>(object)->get_pos());
            break;
        case Object::ACTOR: positions.push_back(dynamic_cast<Actor *>(object)->get_pos()); break;
        default:
            // ignore
            break;
        }
    }

    ecl::Dict<Value>::iterator it = namedPositions.begin();

    for (; it != namedPositions.end(); ++it) {
        if (string_match(it->first, tmpl) && !m_objnames.has_key(it->first)) {
            positions.push_back(it->second);
        }
    }
    return positions;
}

void World::add_actor(Actor *a) {
    add_actor(a, a->get_pos());
}

void World::add_actor(Actor *a, const V2 &pos) {
    actorlist.push_back(a);
    a->m_actorinfo.pos = pos;
    a->m_actorinfo.gridpos = GridPos(pos);
    a->m_actorinfo.field = get_field(a->m_actorinfo.gridpos);

    // Insert the actor as new rightmost_actor and (maybe) sort.
    // This makes use of did_move_actor. See version 1.1, rev.549
    // for explicit code without did_move_actor.

    Actor *oldright = rightmost_actor;

    a->left = oldright;  // might be NULL
    a->right = nullptr;
    rightmost_actor = a;
    if (leftmost_actor == nullptr)
        leftmost_actor = a;
    if (oldright != nullptr) {
        oldright->right = a;
        did_move_actor(a);
    }

    if (!preparing_level) {
        // if game is already running, call on_creation() from here
        a->on_creation(pos);
    }

    if (get_id(a) == ac_pearl_white || get_id(a) == ac_pearl_black)
        ChangeMeditation(+1, 0, 0, 0);
}

Actor *World::yield_actor(Actor *a) {
    auto i = find(actorlist.begin(), actorlist.end(), a);
    if (i != actorlist.end()) {
        actorlist.erase(i);

        if (a->left == nullptr)
            leftmost_actor = a->right;
        else
            a->left->right = a->right;

        if (a->right == nullptr)
            rightmost_actor = a->left;
        else
            a->right->left = a->left;

        a->left = nullptr;
        a->right = nullptr;

        GrabActor(a);
        return a;
    }
    return nullptr;
}

void World::exchange_actors(Actor *a1, Actor *a2) {
    // Exchange actor positions and sort via did_move_actor.
    // A version without did_move_actor is in version 1.1, rev.549.
    ecl::V2 oldpos_a1 = a1->get_actorinfo()->pos;
    a1->get_actorinfo()->pos = a2->get_actorinfo()->pos;
    did_move_actor(a1);
    a2->get_actorinfo()->pos = oldpos_a1;
    did_move_actor(a2);
}

void World::tick(double dtime) {
    // dtime is always 0.01 (cf. server.cc)

    move_actors(dtime);
    tick_sound_dampings();

    // Tell floors and items about new stones.
    for (auto &stone : changed_stones)
        stone_change(stone);
    changed_stones.clear();

    m_mouseforce.tick(dtime);
    for (auto &force : forces)
        force->tick(dtime);

    GameTimer.tick(dtime);

    PerformRecalcLight(false);  // recalculate laser beams if necessary
    doPerformPendingActions();
    // do kill lasered actors in same time step
    for (auto actor : actorlist) {
        Item *it = actor->get_actorinfo()->field->item;
        if (it != nullptr && get_id(it) == it_laserbeam) {
            it->actor_hit(actor);
        }
    }
}

void World::doPerformPendingActions() {
    for (auto &action : actionList) {
        if (action.isCallback) {
            if (lua::CallFunc(lua::LevelState(), action.name, action.val,
                              Object::getObject(action.senderId),
                              action.targetId) != lua::NO_LUAERROR) {
                throw XLevelRuntime(std::string("delayed callback '") + action.name +
                                    "' failed:\n" + lua::LastError(lua::LevelState()));
            }
        } else {
            SendMessage(Object::getObject(action.targetId),
                        Message(action.name, action.val, Object::getObject(action.senderId)));
        }
    }
    actionList.clear();
}

/* ---------- Puzzle scrambling -------------------- */

void World::add_scramble(GridPos p, Direction dir) {
    scrambles.push_back(Scramble(p, dir, scrambleIntensity));
}

void World::scramble_puzzles() {
    while (!scrambles.empty()) {
        for (auto i = scrambles.begin(); i != scrambles.end(); ++i) {
            Stone *puzz = GetStone(i->pos);
            if (puzz && i->intensity) {
                SendMessage(puzz, "_scramble", Value(double(i->dir)));
                --i->intensity;
            } else {
                fprintf(stderr, "no stone found for scramble at %i/%i\n", i->pos.x, i->pos.y);
                i->intensity = 0;
            }
        }

        scrambles.remove_if(mem_fun_ref(&Scramble::expired));
    }
}

//----------------------------------------------------------------------
// Physics simulation
//----------------------------------------------------------------------

/* -------------------- Force calculation -------------------- */

#ifndef M_PI
#define M_PI 3.1415926535
#endif

ecl::V2 World::drunkenMouseforce(Actor *a, V2 &mforce) {
    V2 f = mforce;
    if (a->get_controllers() != 0) {
        if (a->is_drunken()) {
            int t1 = (int)server::LevelTime;
            double base1 = fmod(t1 * M_PI, 2.0) - 1.0;
            int t2 = (int)(5.0 * server::LevelTime);
            double base2 = fmod(t2 * M_PI, 2.0) - 1.0;
            // rotate mouse force by random angle
            double angle = M_PI * (0.3 * base1 + 0.15 * base2);
            f = V2(f[0] * cos(angle) - f[1] * sin(angle), f[0] * sin(angle) + f[1] * cos(angle));
        }
    }
    return f;
}

/*! Calculate the total force on an actor A at time TIME.  The
  actor's current position X and velocity V are also passed.  [Note
  that the position and velocity entries in ActorInfo will be updated
  only after a *successful* time step, so they cannot be used
  here.] */
ecl::V2 World::get_local_force(Actor *a) {
    ecl::V2 f;
    ecl::V2 m;
    double friction = 0;

    if (a->is_on_floor()) {
        if (Floor *floor = a->m_actorinfo.field->floor) {
            // Mouse force
            if (a->get_controllers() != 0) {
                m = floor->process_mouseforce(a, m_mouseforce.get_force(a));
            }
            // Friction
            friction = floor->get_friction();

            // Floor force
            floor->add_force(a, f);
        }

        if (Item *item = a->m_actorinfo.field->item) {
            friction = item->getFriction(a->get_pos(), friction, a);
            if (a->get_controllers() != 0) {
                m = item->calcMouseforce(a, m_mouseforce.get_force(a), m);
            }
            item->add_force(a, f);
        }
    }

    a->m_actorinfo.friction = friction;
    f += drunkenMouseforce(a, m);

    return f;
}

/* Global forces are calculated less often than local ones, namely
   only once every four time steps, cf. move_actors().  They are used
   for forces that are more time consuming to calculate, i.e.,
   actor-actor interactions and external force fields. */
ecl::V2 World::get_global_force(Actor *a) {
    ecl::V2 f;

    // Constant force
    f += server::ConstantForce;

    // Electrostatic forces between actors.
    if (double q = get_charge(a)) {
        for (auto a2 : actorlist) {
            if (a2 == a)
                continue;
            if (double q2 = get_charge(a2)) {
                V2 distv = a->get_pos_force() - a2->get_pos_force();
                if (double dist = distv.normalize())
                    f += server::ElectricForce * q * q2 / (dist)*distv;
            }
        }
    }

    // All other force fields.
    for (auto force : forces)
        f += force->globalForce(a);

    return f;
}

/* -------------------- Collision handling -------------------- */

// Determine whether an actor is in contact with a stone at position `p'. The
// result is returned in `c'. Three situations can occur:
//
// 1) There is no stone at `p'. In this case, `is_contact' is set to false and
//    nothing else is done.
//
// 2) The stone and the actor are in contact. In this case, `c' is filled with
//    the contact information and `is_contact' is set to true.
//
// 3) The stone and the actor are _not_ in contact. In this case, `c' is
//    filled is filled with information about the closest feature on the stone
//    and `is_contact' is set to false.

/**
 * Examine a possible contact of an actor with stone at a given grid position. Joins
 * of outer corners have to be announced. All other cases are handeled.
 * @arg a  the actor that may cause a contact
 * @arg p  the grid position
 * @arg c  the contact info to be filled
 * @arg winFacesActorStone  the faces of a Window stone on the actors grid position
 * @arg isRounded  defaults to true and can be set to false for a join of the
 *         relevant outer edge
 * @arg st  stone at gridposition if already known
 */
void World::find_contact_with_stone(Actor *a, GridPos p, StoneContact &c,
                                    DirectionBits winFacesActorStone, bool isRounded, Stone *st) {

    c.is_contact = false;
    c.faces = NODIRBIT;
    c.outerCorner = false;
    bool isInnerContact = false;

    Stone *stone = (st != nullptr) ? st : GetStone(p);
    if (!stone)
        return;

    bool isWindow = stone->get_traits().id == st_window;
    DirectionBits wsides = NODIRBIT;
    if (isWindow)
        wsides = stone->getFaces(a->is_invisible());

    const ActorInfo &ai = *a->get_actorinfo();
    double r = ai.radius;

    int x = p.x, y = p.y;
    double ax = ai.pos[0], ay = ai.pos[1];

    //    const double contact_e = 0.02;
    const double erad_const = 2.0 / 32;         // edge radius
    const double erad_window_const = 1.5 / 32;  // edge radius for window - the windows glass
                                                // is 3/32 thick - needs to less than minimal
                                                // actor radius!
    double cdist = isWindow ? erad_window_const : erad_const;
    double erad = isRounded ? cdist : 0.0;

    // Inner bounce of a window stone
    if ((ax >= x) && (ax < x + 1) && (ay >= y) && (ay < y + 1)) {
        if (isWindow) {
            // preparations for inner corners of a window stone:
            // 8 inner corners can cause bounces if they are not joined with a
            // neighbour window face:
            // .1....2.
            // 8......3
            // ..    ..
            // ..    ..
            // ..    ..
            // ..    ..
            // 7......4
            // .6....5.
            // the actor can hit just one of the corners - get the candidate first:
            int xcorner = (ax >= x + 0.5);
            int ycorner = (ay >= y + 0.5);

            // get the neighbour window that might eliminate the corner by a join
            int xoff_neighbour =
                (ax < x + erad_window_const) ? -1 : (ax > x + 1 - erad_window_const);
            int yoff_neighbour =
                (ay < y + erad_window_const) ? -1 : (ay > y + 1 - erad_window_const);

            // get the candidate face of the window
            DirectionBits face = NODIRBIT;
            if (!xcorner && yoff_neighbour)
                face = WESTBIT;
            else if (xcorner && yoff_neighbour)
                face = EASTBIT;
            else if (!ycorner && xoff_neighbour)
                face = NORTHBIT;
            else if (ycorner && xoff_neighbour)
                face = SOUTHBIT;

            // the faces that the neighbour window owns
            Stone *neighbour = GetStone(GridPos(x + xoff_neighbour, y + yoff_neighbour));
            DirectionBits face_neighbour =
                (neighbour != nullptr && neighbour->get_traits().id == st_window)
                    ? neighbour->getFaces(a->is_invisible())
                    : NODIRBIT;

            if ((winFacesActorStone & face) && !(face_neighbour & face)) {
                // contact to an inner corner of a window stone
                // same code as external corner below
                double cx[2] = {cdist, -cdist};

                V2 corner(x + xcorner + cx[xcorner], y + ycorner + cx[ycorner]);
                V2 b = V2(ax, ay) - corner;

                // fix 45 degree collisions that may require precision
                if (abs(abs(b[0]) - abs(b[1])) < 1.0e-7) {
                    b[1] = (b[1] >= 0) ? abs(b[0]) : -abs(b[0]);
                }

                c.is_contact = (length(b) - r - cdist < contact_e);
                c.normal = normalize(b);
                c.faces = face;
                c.contact_point = corner + c.normal * cdist;
                isInnerContact = true;

                // all straight contacts to inner window faces including the corners that are joined
            } else if ((winFacesActorStone & SOUTHBIT) &&
                       (ay > y + 1 - 2 * erad_window_const - r - contact_e)) {
                c.contact_point = V2(ax, y + 1 - 2 * erad_window_const);
                c.normal = V2(0, -1);
                c.faces = SOUTHBIT;
                c.is_contact = true;
                isInnerContact = true;
            } else if ((winFacesActorStone & NORTHBIT) &&
                       (ay <= y + 2 * erad_window_const + r + contact_e)) {
                c.contact_point = V2(ax, y + 2 * erad_window_const);
                c.normal = V2(0, +1);
                c.faces = NORTHBIT;
                c.is_contact = true;
                isInnerContact = true;
            } else if ((winFacesActorStone & WESTBIT) &&
                       (ax <= x + 2 * erad_window_const + r + contact_e)) {
                c.contact_point = V2(x + 2 * erad_window_const, ay);
                c.normal = V2(+1, 0);
                c.faces = WESTBIT;
                c.is_contact = true;
                isInnerContact = true;
            } else if ((winFacesActorStone & EASTBIT) &&
                       (ax > x + 1 - 2 * erad_window_const - r - contact_e)) {
                c.contact_point = V2(x + 1 - 2 * erad_window_const, ay);
                c.normal = V2(-1, 0);
                c.faces = EASTBIT;
                c.is_contact = true;
                isInnerContact = true;
            }
        }
        // ignore all inner collisions of other stones
    }
    // Closest feature == north or south face of the stone?
    else if (ax >= x + erad && ax < x + 1 - erad &&
             (!isWindow || ((ay > y + 1) && (wsides & SOUTHBIT)) ||
              ((ay < y) && (wsides & NORTHBIT)) || (ax <= x + erad_window_const) ||
              (ax >= x + 1 - erad_window_const))) {
        // the last two terms are straight reflections on rectangular window sides due
        // to a join with the neighbour stone

        double dist = r + 5;

        // south
        if (ay > y + 1) {
            c.contact_point = V2(ax, y + 1);
            c.normal = V2(0, +1);
            c.faces = SOUTHBIT;
            dist = ay - (y + 1);
        }
        // north
        else if (ay < y) {
            c.contact_point = V2(ax, y);
            c.normal = V2(0, -1);
            c.faces = NORTHBIT;
            dist = y - ay;
        }
        c.is_contact = (dist - r < contact_e);

        if (isWindow &&
            (((ay > y + 1) && !(wsides & SOUTHBIT)) || ((ay < y) && !(wsides & NORTHBIT)))) {
            // actor did hit joined part of end face
            if (ax <= x + erad_window_const)
                c.faces = WESTBIT;
            else
                c.faces = EASTBIT;
        }
    }
    // Closest feature == west or east face of the stone?
    else if (ay >= y + erad && ay < y + 1 - erad &&
             (!isWindow || ((ax > x + 1) && (wsides & EASTBIT)) ||
              ((ax < x) && (wsides & WESTBIT)) || (ay <= y + erad_window_const) ||
              (ay >= y + 1 - erad_window_const))) {
        double dist = r + 5;
        if (ax > x + 1) {  // east
            c.contact_point = V2(x + 1, ay);
            c.normal = V2(+1, 0);
            c.faces = EASTBIT;
            dist = ax - (x + 1);
        } else if (ax < x) {  // west
            c.contact_point = V2(x, ay);
            c.normal = V2(-1, 0);
            c.faces = WESTBIT;
            dist = x - ax;
        }
        c.is_contact = (dist - r < contact_e);
        if (isWindow &&
            (((ax > x + 1) && !(wsides & EASTBIT)) || ((ax < x) && !(wsides & WESTBIT)))) {
            // actor did hit joined part of end face
            if (ay <= y + erad_window_const)
                c.faces = NORTHBIT;
            else
                c.faces = SOUTHBIT;
        }
    }
    // Closest feature == any of the four corners
    else if (!isWindow ||
             !(((ax > x + cdist) && (ax < x + 0.5) && (wsides & WESTBIT) &&
                (winFacesActorStone & WESTBIT)) ||
               ((ax >= x + 0.5) && (ax < x + 1 - cdist) && (wsides & EASTBIT) &&
                (winFacesActorStone & EASTBIT)) ||
               ((ay > y + cdist) && (ay < y + 0.5) && (wsides & NORTHBIT) &&
                (winFacesActorStone & NORTHBIT)) ||
               ((ay >= y + 0.5) && (ay < y + 1 - cdist) && (wsides & SOUTHBIT) &&
                (winFacesActorStone & SOUTHBIT)))) {
        // the 4 terms exclude collisions from inner corners of windows if they are joined
        // with a window stone face on the grid of the actor itself
        int xcorner = (ax >= x + 0.5);
        int ycorner = (ay >= y + 0.5);
        double cx[2] = {cdist, -cdist};

        V2 corner(x + xcorner + cx[xcorner], y + ycorner + cx[ycorner]);
        V2 b = V2(ax, ay) - corner;

        // fix 45 degree collisions that may require precision
        if (abs(abs(b[0]) - abs(b[1])) < 1.0e-7) {
            b[1] = (b[1] >= 0) ? abs(b[0]) : -abs(b[0]);
        }

        c.is_contact = (length(b) - r - cdist < contact_e);
        c.normal = normalize(b);
        c.contact_point = corner + c.normal * cdist;
        if (!isWindow) {
            if (abs(b[0]) >= abs(b[1])) {
                if (b[0] < 0)
                    c.faces = DirectionBits(c.faces | WESTBIT);
                else
                    c.faces = DirectionBits(c.faces | EASTBIT);
            } else if (abs(b[1]) >= abs(b[0])) {
                if (b[1] < 0)
                    c.faces = DirectionBits(c.faces | NORTHBIT);
                else
                    c.faces = DirectionBits(c.faces | SOUTHBIT);
            }
        } else {
            if (!xcorner && (b[0] > 0 || (!ycorner && !(wsides & NORTHBIT)) ||
                             (ycorner && !(wsides & SOUTHBIT)) || (abs(b[0]) >= abs(b[1]))))
                c.faces = DirectionBits(c.faces | WESTBIT);
            if (xcorner && (b[0] < 0 || (!ycorner && !(wsides & NORTHBIT)) ||
                            (ycorner && !(wsides & SOUTHBIT)) || (abs(b[0]) >= abs(b[1]))))
                c.faces = DirectionBits(c.faces | EASTBIT);
            if (ycorner && (b[1] < 0 || (!xcorner && !(wsides & WESTBIT)) ||
                            (xcorner && !(wsides & EASTBIT)) || (abs(b[1]) >= abs(b[0]))))
                c.faces = DirectionBits(c.faces | SOUTHBIT);
            if (!ycorner && (b[1] > 0 || (!xcorner && !(wsides & WESTBIT)) ||
                             (xcorner && !(wsides & EASTBIT)) || (abs(b[1]) >= abs(b[0]))))
                c.faces = DirectionBits(c.faces | NORTHBIT);
        }
        c.outerCorner = true;
    }

    if (c.is_contact) {
        // treat this as a collision only if actor not inside the stone
        // and velocity towards stone
        if (!isInnerContact && ax >= x && ax < x + 1 && ay >= y && ay < y + 1)
            c.is_collision = false;
        else
            c.is_collision = c.normal * ai.vel < 0;

        c.ignore = false;
        c.actor = a;
        c.stonepos = p;
        c.stoneid = stone->get_traits().id;
        c.response = stone->collision_response(c);
        c.sound = stone->collision_sound();
    }
}

/**
 * Examines all contacts of an actor that is the edge of a grid in a distance to touch
 * any stone on the three adjacent grid positions.
 * @arg a   the actor that causes contacts
 * @arg p0  the grid position that is diagonal to the actors grid at the give edge
 * @arg c0  the contact info for p0 initialized with a normal pointing to the actors grid
 * @arg p1  one of the grid positions that is a side neighbour to the actors grid at the give edge
 * @arg c1  the contact info for p1 initialized with a normal pointing to the actors grid
 * @arg p2  one of the grid positions that is a side neighbour to the actors grid at the give edge
 * @arg c2  the contact info for p0 initialized with a normal pointing to the actors grid
 * @arg winFacesActorStone  the faces of a Window stone on the actors grid position that
 *          could cause joins with others Window stones faces on the examined positions.
 *          Just inner corners of Window faces are affected.
 */
void World::find_contact_with_edge(Actor *a, GridPos p0, GridPos p1, GridPos p2, StoneContact &c0,
                                   StoneContact &c1, StoneContact &c2,
                                   DirectionBits winFacesActorStone) {
    Stone *s0 = GetStone(p0);
    Stone *s1 = GetStone(p1);
    Stone *s2 = GetStone(p2);
    if (s0 != nullptr)
        c0.response = s0->collision_response(c0);
    if (s1 != nullptr)
        c1.response = s1->collision_response(c1);
    if (s2 != nullptr)
        c2.response = s2->collision_response(c2);

    if (s1 && s2 && c1.response == STONE_REBOUND && c2.response == STONE_REBOUND) {
        // a real edge bounce - no rounded edges
        find_contact_with_stone(a, p1, c1, winFacesActorStone, false,
                                s1);  // collision with both straight neighbours
        find_contact_with_stone(a, p2, c2, winFacesActorStone, false,
                                s2);  // collision with both straight neighbours
    } else if (s0 && s1 && c0.response == STONE_REBOUND && c1.response == STONE_REBOUND) {
        // join stones to a block without rounded edges
        find_contact_with_stone(a, p1, c1, winFacesActorStone, false,
                                s1);  // collision with straight neighbour only
        if (c1.normal * (a->get_actorinfo()->vel) >=
            0 /* &&  s1 has restituion 1.0 */)  // leaving a oneway or door
            find_contact_with_stone(a, p0, c0, winFacesActorStone, true,
                                    s0);  // collide with diagonal stone
        find_contact_with_stone(a, p2, c2, winFacesActorStone, true,
                                s2);  // register contact without collision
    } else if (s0 && s2 && c0.response == STONE_REBOUND && c2.response == STONE_REBOUND) {
        // join stones to a block without rounded edges
        find_contact_with_stone(a, p2, c2, winFacesActorStone, false,
                                s2);  // contact with straight neighbour only
        if (c2.normal * (a->get_actorinfo()->vel) >=
            0 /* &&  s2 has restitution 1.0 */)  // leaving a oneway or door
            find_contact_with_stone(a, p0, c0, winFacesActorStone, true,
                                    s0);  // collide with diagonal stone
        find_contact_with_stone(a, p1, c1, winFacesActorStone, true,
                                s1);  // register contact without collision
    } else {
        // register single stone collisions and contacts
        if (s0)
            find_contact_with_stone(a, p0, c0, winFacesActorStone, true, s0);
        if (s1)
            find_contact_with_stone(a, p1, c1, winFacesActorStone, true, s1);
        if (s2)
            find_contact_with_stone(a, p2, c2, winFacesActorStone, true, s2);
    }
}

/**
 * Examines all contacts of an actor with a Window stone on the grid position of the
 * actor.
 * @arg a   the actor that causes contacts
 * @arg p   the grid position of the actor and the Window stone
 * @arg c0  a contact info for a east or west contact. The normal has no need of being intialized
 * @arg c1  a contact info for a noth or south contact. The normal has no need of being intialized
 * @arg windowFaces  the faces of a Window stone on the actors grid position
 */
void World::find_contact_with_window(Actor *a, GridPos p, StoneContact &c0, StoneContact &c1,
                                     DirectionBits windowFaces) {
    if (windowFaces == NODIRBIT)
        return;

    // as the actor cannot contact two opposite faces at the same time
    // we reuse the contact structure for optimization
    if (windowFaces & WESTBIT)
        find_contact_with_stone(a, p, c0, WESTBIT);
    if ((windowFaces & EASTBIT) && c0.is_contact == false)
        find_contact_with_stone(a, p, c0, EASTBIT);
    if (windowFaces & SOUTHBIT)
        find_contact_with_stone(a, p, c1, SOUTHBIT);
    if ((windowFaces & NORTHBIT) && c1.is_contact == false)
        find_contact_with_stone(a, p, c1, NORTHBIT);
}

/**
 * Examins all contacts of an actor with stones on and around it.
 * @arg a   the actor that causes contacts
 * @arg c0  an uninitialized contact info that will be filled with possible contact data
 * @arg c1  an uninitialized contact info that will be filled with possible contact data
 * @arg c2  an uninitialized contact info that will be filled with possible contact data
 */
void World::find_stone_contacts(Actor *a, StoneContact &c0, StoneContact &c1, StoneContact &c2) {
    // time critical routine that is performance optimized

    c0.is_contact = false;
    c1.is_contact = false;
    c2.is_contact = false;
    c0.actor = a;
    c1.actor = a;
    c2.actor = a;

    ActorInfo &ai = *a->get_actorinfo();
    double re = ai.radius + contact_e;
    GridPos g = GridPos(round_down<int>(ai.pos[0]), round_down<int>(ai.pos[1]));
    double x = ai.pos[0];
    double y = ai.pos[1];
    bool noCollisions = server::NoCollisions &&
           (a->get_traits().id_mask & (1 << ac_marble_white | 1 << ac_marble_black |
                                       1 << ac_marble_glass |
                                       1 << ac_pearl_white | 1 << ac_pearl_black));

    // info about a Window stone on the Gridpos of the actor that may cause
    // contacts within the grid
    Stone *actorWinStone = GetStone(g);
    DirectionBits winFacesActorStone =
        (actorWinStone != nullptr && actorWinStone->get_traits().id == st_window)
            ? actorWinStone->getFaces(a->is_invisible())
            : NODIRBIT;

    // distinguish 9 squares within gridpos that may cause contacts
    // low cost reduction of cases that need to be examined in detail:
    // - within the edges no inner contacts to a Window are possible
    // - middle parts can contact to a Window on the grid and one stone aside
    // - the center part can contact two faces of a Window on the grid
    // the contact info is prepared with normal vectors that allow quick checks
    // with stone faces
    if (y - g.y < re) {
        // upper grid part
        if (x - g.x < re) {
            // upper left edge
            if (!noCollisions)  // just Borderstone collisions are needed for no collisions!
                find_contact_with_window(a, GridPos(g.x, g.y), c1, c2, winFacesActorStone);
            if (c1.is_contact)
                // inner west window contact - just look for north adjacent stone contact
                find_contact_with_stone(a, GridPos(g.x, g.y - 1), c0, winFacesActorStone);
            else if (c2.is_contact)
                // inner north window contact - just look for west adjacent stone contact
                find_contact_with_stone(a, GridPos(g.x - 1, g.y), c0, winFacesActorStone);
            else {
                c0.normal = V2(+1, +1);  // no need of normalization - just direction
                c1.normal = V2(0, +1);
                c2.normal = V2(+1, 0);
                c0.contact_point = c1.contact_point = c2.contact_point = V2(g.x, g.y);
                find_contact_with_edge(a, GridPos(g.x - 1, g.y - 1), GridPos(g.x, g.y - 1),
                                       GridPos(g.x - 1, g.y), c0, c1, c2, winFacesActorStone);
            }
        } else if (-x + (g.x + 1) < re) {
            // upper right edge
            if (!noCollisions)
                find_contact_with_window(a, GridPos(g.x, g.y), c1, c2, winFacesActorStone);
            if (c1.is_contact)
                // inner east window contact - just look for north adjacent stone contact
                find_contact_with_stone(a, GridPos(g.x, g.y - 1), c0, winFacesActorStone);
            else if (c2.is_contact)
                // inner north window contact - just look for east adjacent stone contact
                find_contact_with_stone(a, GridPos(g.x + 1, g.y), c0, winFacesActorStone);
            else {
                c0.normal = V2(-1, +1);  // no need of normalization - just direction
                c1.normal = V2(0, +1);
                c2.normal = V2(-1, 0);
                c0.contact_point = c1.contact_point = c2.contact_point = V2(g.x + 1, g.y);
                find_contact_with_edge(a, GridPos(g.x + 1, g.y - 1), GridPos(g.x, g.y - 1),
                                       GridPos(g.x + 1, g.y), c0, c1, c2, winFacesActorStone);
            }
        } else {
            // upper middle part
            if (!noCollisions)
                find_contact_with_window(a, GridPos(g.x, g.y), c1, c2, winFacesActorStone);
            if (!c2.is_contact)
                // only hit adjacent stone if no inner window is in front
                find_contact_with_stone(a, GridPos(g.x, g.y - 1), c0, winFacesActorStone);
        }
    } else if (-y + (g.y + 1) < re) {
        // lower grid part
        if (x - g.x < re) {
            if (!noCollisions)
                find_contact_with_window(a, GridPos(g.x, g.y), c1, c2, winFacesActorStone);
            // lower left edge
            if (c1.is_contact)
                // inner west window contact - just look for south adjacent stone contact
                find_contact_with_stone(a, GridPos(g.x, g.y + 1), c0, winFacesActorStone);
            else if (c2.is_contact)
                // inner south window contact - just look for west adjacent stone contact
                find_contact_with_stone(a, GridPos(g.x - 1, g.y), c0, winFacesActorStone);
            else {
                c0.normal = V2(+1, -1);  // no need of normalization - just direction
                c1.normal = V2(0, -1);
                c2.normal = V2(+1, 0);
                c0.contact_point = c1.contact_point = c2.contact_point = V2(g.x, g.y + 1);
                find_contact_with_edge(a, GridPos(g.x - 1, g.y + 1), GridPos(g.x, g.y + 1),
                                       GridPos(g.x - 1, g.y), c0, c1, c2, winFacesActorStone);
            }
        } else if (-x + (g.x + 1) < re) {
            // lower right edge
            if (!noCollisions)
                find_contact_with_window(a, GridPos(g.x, g.y), c1, c2, winFacesActorStone);
            if (c1.is_contact)
                // inner east window contact - just look for south adjacent stone contact
                find_contact_with_stone(a, GridPos(g.x, g.y + 1), c0, winFacesActorStone);
            else if (c2.is_contact)
                // inner south window contact - just look for east adjacent stone contact
                find_contact_with_stone(a, GridPos(g.x + 1, g.y), c0, winFacesActorStone);
            else {
                c0.normal = V2(-1, -1);  // no need of normalization - just direction
                c1.normal = V2(0, -1);
                c2.normal = V2(-1, 0);
                c0.contact_point = c1.contact_point = c2.contact_point = V2(g.x + 1, g.y + 1);
                find_contact_with_edge(a, GridPos(g.x + 1, g.y + 1), GridPos(g.x, g.y + 1),
                                       GridPos(g.x + 1, g.y), c0, c1, c2, winFacesActorStone);
            }
        } else {
            // lower middle part
            if (!noCollisions)
                find_contact_with_window(a, GridPos(g.x, g.y), c1, c2, winFacesActorStone);
            if (!c2.is_contact)
                // only hit adjacent stone if no inner window is in front
                find_contact_with_stone(a, GridPos(g.x, g.y + 1), c0, winFacesActorStone);
        }
    } else {
        // middle grid part
        if (x - g.x < re) {
            // left middle part
            if (!noCollisions)
                find_contact_with_window(a, GridPos(g.x, g.y), c1, c2, winFacesActorStone);
            if (!c1.is_contact)
                // only hit adjacent stone if no inner window is in front
                find_contact_with_stone(a, GridPos(g.x - 1, g.y), c0, winFacesActorStone);
        } else if (-x + (g.x + 1) < re) {
            // right middle part
            if (!noCollisions)
                find_contact_with_window(a, GridPos(g.x, g.y), c1, c2, winFacesActorStone);
            if (!c1.is_contact)
                // only hit adjacent stone if no inner window is in front
                find_contact_with_stone(a, GridPos(g.x + 1, g.y), c0, winFacesActorStone);
        } else {
            // actor in center of grid - just inner window contacts
            if (!noCollisions)
                find_contact_with_window(a, GridPos(g.x, g.y), c0, c1, winFacesActorStone);
        }
    }
}

/*! This function is called for every contact between an actor and a
  stone.  It handles: (1) informing stones about contacts, (2)
  collision response, (3) updating the actor's contact list.  Note
  that a stone may kill itself during a call to actor_hit() or
  actor_contact() so we must not refer to it after having called these
  functions.  (Incidentally, this is why StoneContact only contains a
  'stonepos' and no 'stone' entry.) */
void World::handle_stone_contact(StoneContact &sc) {
    Actor *a = sc.actor;
    ActorInfo &ai = *a->get_actorinfo();
    double restitution = 1.0;  // 0.85;

    if (server::NoCollisions && (sc.stoneid != st_borderstone) &&
        a->get_traits().id_mask & (1 << ac_marble_white | 1 << ac_marble_black |
                                   1 << ac_marble_glass |
                                   1 << ac_pearl_white | 1 << ac_pearl_black))
        return;

    Contact contact(sc.contact_point, sc.normal);

    if (sc.is_contact && sc.response == STONE_REBOUND) {
        ASSERT(ai.contacts_count < MAX_CONTACTS, XLevelRuntime,
               ecl::strf("Enigma Error - insufficient contacts: %d", ai.contacts_count).c_str());
        ai.contacts[ai.contacts_count++] = contact;
    }

    if (sc.is_collision) {
        if (!sc.ignore && sc.response == STONE_REBOUND) {
            bool slow_collision = length(ai.vel) < 0.3;
            if (!has_nearby_contact(ai.last_contacts, ai.last_contacts_count, contact)) {
                if (Stone *stone = GetStone(sc.stonepos)) {
                    a->beforeStoneBounce(sc);
                    if (slow_collision)
                        stone->actor_touch(sc);
                    else
                        stone->actor_hit(sc);
                    a->afterStoneBounce(sc);
                    if (!slow_collision) {
                        client::Msg_Sparkle(sc.contact_point);
                        double volume = std::max(0.25, length(ai.vel) / 8);
                        volume = std::min(1.0, volume);
                        volume = GetVolume(sc.sound.c_str(), a, volume);
                        sound::EmitSoundEvent(sc.sound.c_str(), sc.contact_point, volume);
                    }
                }
            }

            // remove collision forces components from actor-actor collisions
            // in direction of stone
            double normal_component = sc.normal * ai.collforce;
            if (normal_component < 0) {
                ai.collforce -= normal_component * sc.normal;
            }

            double dt = ActorTimeStep;
            ai.collforce -= (1 + restitution) * (ai.vel * sc.normal) * sc.normal / dt * ai.mass;
        }
    } else if (sc.is_contact) {
        if (Stone *stone = GetStone(sc.stonepos)) {
            stone->actor_contact(sc.actor);

            if (a->get_gridpos() != sc.stonepos && !sc.ignore && sc.response == STONE_REBOUND) {
                // remove collision forces components from actor-actor collisions
                // in direction of stone
                double normal_component = sc.normal * ai.collforce;
                if (normal_component < 0) {
                    ai.collforce -= normal_component * sc.normal;
                }
            }
        }
    }
}

namespace {

struct ActorEntry {
    double pos;
    size_t idx;

    ActorEntry() {
        pos = 0;
        idx = 0;
    }
    ActorEntry(double pos_, size_t idx_) {
        pos = pos_;
        idx = idx_;
    }

    bool operator<(const ActorEntry &x) const { return pos < x.pos; }
};

}  // namespace

void World::handle_actor_contacts() {
    // For each actor, search for possible collisions with other actors.
    // If there is a good chance for a collision, call handle_actor_contact.
    Actor *a = leftmost_actor;
    while (a != nullptr) {
        Actor *candidate = a->right;
        double actingradius = a->m_actorinfo.radius + Actor::max_radius;
        double max_x = a->m_actorinfo.pos[0] + actingradius;
        while (candidate != nullptr && candidate->m_actorinfo.pos[0] <= max_x) {
            double ydist = candidate->m_actorinfo.pos[1] - a->m_actorinfo.pos[1];
            ydist = (ydist < 0) ? -ydist : ydist;
            if (ydist <= actingradius) {
                handle_actor_contact(a, candidate);
            }
            candidate = candidate->right;
        }
        a = a->right;
    }
}

void World::handle_actor_contact(Actor *actor1, Actor *actor2) {
    // Calculate if there is a collision between actor1 and actor2.

    ActorInfo &a1 = *actor1->get_actorinfo();
    ActorInfo &a2 = *actor2->get_actorinfo();

    if (a1.ignore_contacts || a2.ignore_contacts)
        return;

    V2 n = a1.pos - a2.pos;  // normal to contact surface
    // fix 45 degree collisions that may require precision
    //   ignore central overlapping marbles in this correction
    if (abs(abs(n[0]) - abs(n[1])) < 1.0e-7 && abs(n[1]) > 1.0e-5) {
        n[1] = (n[1] >= 0) ? abs(n[0]) : -abs(n[0]);
    }
    double dist = n.normalize();
    double overlap = a1.radius + a2.radius - dist;
    if (overlap > 0 && !a1.grabbed && !a2.grabbed) {
        double relspeed = (a2.vel - a1.vel) * n;

        if (relspeed < 0)  // not moving towards each other
            return;

        bool enforceCollision = actor1->on_collision(actor2);
        // Use eager evaluation of OR; e.g. if two glass marbles collide, both might shatter.
        enforceCollision = enforceCollision | actor2->on_collision(actor1);

        bool reboundp = (actor1->is_movable() && actor2->is_movable() &&
                         (enforceCollision || (actor1->is_on_floor() == actor2->is_on_floor())));

        if (reboundp) {
            Contact contact(a2.pos + n * a2.radius, -n);
            ASSERT(
                a2.contacts_count < MAX_CONTACTS, XLevelRuntime,
                ecl::strf("Enigma Error - insufficient contacts: %d", a2.contacts_count).c_str());
            a2.contacts[a2.contacts_count++] = contact;
            contact.normal = n;
            ASSERT(
                a1.contacts_count < MAX_CONTACTS, XLevelRuntime,
                ecl::strf("Enigma Error - insufficient contacts: %d", a1.contacts_count).c_str());
            a1.contacts[a1.contacts_count++] = contact;

            double restitution = 1.0;  // 0.95;

            // Calculate doubled reduced mass:
            double dmu = a1.mass;
            if (a1.mass != a2.mass)
                dmu = 2 * a1.mass * a2.mass / (a1.mass + a2.mass);

            V2 force = (restitution * dmu * relspeed / ActorTimeStep) * n;
            a1.collforce += force;
            a2.collforce -= force;
            if (!has_nearby_contact(a1.last_contacts, a1.last_contacts_count, contact)) {
                double volume = length(force) * ActorTimeStep;
                volume = std::min(1.0, volume);
                if (volume > 0.4) {
                    volume = GetVolume("ballcollision", nullptr, volume);
                    sound::EmitSoundEvent("ballcollision", contact.pos, volume);
                }
            }
        }
    }
}

void World::handle_stone_contacts(unsigned actoridx) {
    // Three contact structures are used to store info about contact candidates.
    // No more than two stone contacts per actor are possible, but it is more
    // efficient to provide three structures for candidates and to check afterwards.

    static StoneContact contacts[3];  // recycle structures for efficiency

    Actor *actor = actorlist[actoridx];

    if (actor->m_actorinfo.ignore_contacts)  // used by the cannonball for example
        return;

    // Find contacts without any sideeffects
    find_stone_contacts(actor, contacts[0], contacts[1], contacts[2]);

    // Handle contacts with stones - forces and stone hit, touch callback
    if (contacts[0].is_contact)
        handle_stone_contact(contacts[0]);
    if (contacts[1].is_contact)
        handle_stone_contact(contacts[1]);
    if (contacts[2].is_contact)
        handle_stone_contact(contacts[2]);
}

/* -------------------- Actor Motion -------------------- */

void World::move_actors(double dtime) {
    const double dt = ActorTimeStep;

    static double rest_time = 0;
    rest_time += dtime;

    size_t nactors = actorlist.size();
    for (unsigned i = 0; i < nactors; ++i) {
        Actor *a = actorlist[i];
        ActorInfo &ai = *a->get_actorinfo();
        // extrapolate actor position for better accuracy of forces
        if (!ai.grabbed)
            ai.pos_force = ai.pos + dtime * 0.4 * ai.vel;
        else
            ai.pos_force = ai.pos;
    }
    vector<V2> global_forces(nactors);
    for (unsigned i = 0; i < nactors; ++i) {
        Actor *a = actorlist[i];
        global_forces[i] = get_global_force(a);
    }

    while (rest_time > 0) {
        for (unsigned i = 0; i < nactors; ++i) {
            Actor *a = actorlist[i];
            ActorInfo &ai = *a->get_actorinfo();

            // the "6" is a historical accident, don't change it!
            ai.force = ai.forceacc * 6;
            ai.force += global_forces[i];
            ai.force += get_local_force(a);

            ai.forceacc = V2();
            ai.collforce = V2();

            // swap contacts and clear contacts
            ai.last_contacts = ai.contacts;
            ai.last_contacts_count = ai.contacts_count;
            ai.contacts = ai.contacts_a;
            ai.contacts_count = 0;
        }

        for (auto &rubberband : rubberbands) {
            rubberband->applyForces(dt);
        }

        handle_actor_contacts();
        collisionCriticalPositions.clear();
        registerCriticalPositions = true;
        for (unsigned i = 0; i < nactors; ++i) {
            Actor *a = actorlist[i];
            ActorInfo &ai = *a->get_actorinfo();
            if (!ai.grabbed)
                handle_stone_contacts(i);
        }

        doPerformPendingActions();

        for (unsigned i = 0; i < nactors; ++i) {
            Actor *a = actorlist[i];
            ActorInfo &ai = *a->get_actorinfo();
            double dtime = dt;

            if (!a->can_move()) {
                if (length(ai.force) > 30)
                    client::Msg_Sparkle(ai.pos);
                ai.vel = V2();
            } else if (!a->is_dead() && a->is_movable() && !ai.grabbed) {
                advance_actor(a, dtime);
            }
            a->move();  // 'move' nevertheless, to pick up items etc
            a->think(dtime);
        }
        registerCriticalPositions = false;

        rest_time -= dt;
    }
}

/* This function performs one step in the numerical integration of an
   actor's equation of motion.  TIME ist the current absolute time and
   H the size of the integration step. */
void World::advance_actor(Actor *a, double &dtime) {
    const double MAXVEL = 70;  // 70 grids/s  < min_actor_radius/timestep !

    ActorInfo &ai = *a->get_actorinfo();
    V2 oldPos = ai.pos;
    V2 force = ai.force;

    // If the actor is currently in contact with other objects, remove
    // the force components in the direction of these objects.
    for (unsigned i = 0; i < ai.contacts_count; ++i) {
        const V2 &normal = ai.contacts[i].normal;
        double normal_component = normal * force;
        if (normal_component < 0) {
            force -= normal_component * normal;
        }
    }
    force += ai.collforce;

    ai.vel += dtime * force / ai.mass;
    // Limit maximum velocity
    double q = length(ai.vel) / MAXVEL;
    if (q > 1)
        ai.vel /= q;

    // Friction influence
    double friction = server::FrictionFactor * ai.friction;
    if (a->has_spikes())
        friction += 7.0 * server::FrictionFactor;

    double vv = length(ai.vel);
    if (vv > 0) {
        V2 frictionf = ai.vel * friction;
        frictionf /= vv;
        frictionf *= pow(vv, 0.8);
        ai.vel -= dtime * frictionf / ai.mass;
        if (friction < 0) {  // inverse, accelerating friction
            // Limit to maximum velocity again
            double q = length(ai.vel) / MAXVEL;
            if (q > 1)
                ai.vel /= q;
        }
    }

    ai.pos += dtime * ai.vel;
    // avoid actors outside of world
    if (ai.pos[0] < 0)
        ai.pos[0] = 0.0;
    if (ai.pos[0] >= w)
        ai.pos[0] = w - 1e-12;
    if (ai.pos[1] < 0)
        ai.pos[1] = 0.0;
    if (ai.pos[1] >= h)
        ai.pos[1] = h - 1e-12;

    GridPos oldGridPos(oldPos);
    GridPos newGridPos(ai.pos);
    //
    for (auto &elem : collisionCriticalPositions) {
        if (elem == newGridPos) {
            ai.pos = oldPos;
            newGridPos = oldGridPos;
            break;
        }
    }

    // disallow direct diagonal grid moves
    if (oldGridPos.x != newGridPos.x && oldGridPos.y != newGridPos.y) {
        // split diagonal grid move in the middle of the path over the missed grid
        V2 newPos = ai.pos;
        V2 midPos;
        V2 corner(0.5 + (newGridPos.x + oldGridPos.x) / 2.0,
                  0.5 + (newGridPos.y + oldGridPos.y) / 2.0);
        double tx = (corner[1] - oldPos[1]) / ai.vel[1];
        double ty = (corner[0] - oldPos[0]) / ai.vel[0];
        double mid_t = (tx + ty) / 2;
        midPos[0] = (oldPos[0] + ai.vel[0] * tx + corner[0]) / 2.0;
        midPos[1] = (oldPos[1] + ai.vel[1] * ty + corner[1]) / 2.0;
        // detect moves directly on the first diagonal of a grid
        if (midPos == corner &&
            ((midPos[0] == oldGridPos.x && midPos[1] == oldGridPos.y) ||
             ((midPos[0] == oldGridPos.x + 1) && (midPos[1] == oldGridPos.y + 1)))) {
            // as the edge belongs to either old or new position we would never hit an
            // adjacent grid and thus would pass the diagonal unchecked.
            // we disturb the movement with a random minimal temporarily correcture
            midPos += (0.5 - IntegerRand(0, 1)) * V2(1e-10, -1e-10);
        }
        ai.pos = midPos;
        did_move_actor(a);
        a->move();        // 'move' nevertheless, to pick up items etc
        a->think(mid_t);  // partial time
        dtime -= mid_t;   // rest time
        if (!a->is_dead() && a->is_movable() && !ai.grabbed && ai.pos == midPos) {
            ai.pos = newPos;
        } else {
            // something happend - do not continue old move
            return;
        }
    }

    did_move_actor(a);
}

void World::did_move_actor(Actor *a) {
    a->m_actorinfo.gridpos = GridPos(a->m_actorinfo.pos);
    a->m_actorinfo.field = get_field(a->m_actorinfo.gridpos);

    double ax = a->m_actorinfo.pos[0];
    Actor *old_left = a->left;
    Actor *old_right = a->right;
    Actor *new_left = old_left;
    Actor *new_right = old_right;
    // find new position in actor list
    while (new_left != nullptr && new_left->m_actorinfo.pos[0] > ax) {
        new_left = new_left->left;
    };
    while (new_right != nullptr && new_right->m_actorinfo.pos[0] < ax) {
        new_right = new_right->right;
    };
    if (new_left != old_left || new_right != old_right) {
        // remove from old position
        if (old_left == nullptr) {
            leftmost_actor = old_right;
            old_right->left = nullptr;
        } else if (old_right == nullptr) {
            rightmost_actor = old_left;
            old_left->right = nullptr;
        } else {
            old_left->right = old_right;
            old_right->left = old_left;
        }
        // insert at new position
        if (new_left != old_left) {
            // did move to left side
            if (new_left == nullptr) {
                a->left = nullptr;
                a->right = leftmost_actor;
                leftmost_actor->left = a;
                leftmost_actor = a;
            } else {
                a->left = new_left;
                a->right = new_left->right;
                new_left->right = a;
                a->right->left = a;
            }
        } else if (new_right != old_right) {
            // did move to right side
            if (new_right == nullptr) {
                a->right = nullptr;
                a->left = rightmost_actor;
                rightmost_actor->right = a;
                rightmost_actor = a;
            } else {
                a->right = new_right;
                a->left = new_right->left;
                new_right->left = a;
                a->left->right = a;
            }
        }
    }
}

void World::tick_sound_dampings() {
    if (sound::IsSoundMute())
        return;
    // See SoundEffectManager.hh for details.
    static int counter = 0;
    ++counter;

    if (counter > 9) {
        counter = 0;
        auto i = level->sound_dampings.begin();
        auto end = level->sound_dampings.end();
        while (i != end) {
            if (i->tick())  // return true if damping entry should be deleted
                i = level->sound_dampings.erase(i);
            else
                ++i;
        }
    }
}

void World::stone_change(GridPos p) {
    if (const Field *f = GetField(p)) {
        Stone *st = f->stone;
        if (st) {
            st->on_floor_change();
            // Perhaps the stone was destroyed - get the new one.
            st = f->stone;
        }

        if (st) {
            st->freeze_check();
            st = f->stone;
        }

        if (Item *it = f->item)
            it->stone_change(st);

        if (Floor *fl = f->floor)
            fl->stone_change(st);

        // Additional freeze checks of the surrounding stones.
        // (Example: In a sokoball level, a stone has been pushed
        //           onto a goal, thus it should not freeze. But
        //           perhaps its neighbor is stuck!)
        if (Stone *stn = GetStone(move(p, NORTH)))
            stn->freeze_check();
        if (Stone *stn = GetStone(move(p, SOUTH)))
            stn->freeze_check();
        if (Stone *stn = GetStone(move(p, EAST)))
            stn->freeze_check();
        if (Stone *stn = GetStone(move(p, WEST)))
            stn->freeze_check();

        MaybeRecalcLight(p);
    }
}

/* -------------------- Functions -------------------- */

void Resize(int w, int h) {
    level.reset(new World(w, h));
    display::NewWorld(w, h);
    server::WorldSized = true;
    player::NewGame();
}

int Width() {
    return level->w;
}

int Height() {
    return level->h;
}

void WorldPrepareLevel() {
    GameTimer.clear();
    Resize(20, 13);              // delete old world with all its objects
    GridObject::prepareLevel();  // clear lists of no longer existing objects
    LaserBeam::prepareLevel();   // clear lists of no longer existing objects
    OxydStone::prepareLevel();   // clear lists of no longer existing objects
}

bool WorldInitLevel() {
    if (server::EnigmaCompatibility < 1.10) {
        server::CrackSpreading = server::Brittleness;
    }

    level->scramble_puzzles();

    RecalcLight();
    PerformRecalcLight(true);  // recalculate laser beams if necessary

    bool seen_player0 = false;

    for (auto a : level->actorlist) {
        a->on_creation(a->get_actorinfo()->pos);
        SendMessage(a, "_init", Value());

        if (Value v = a->getAttr("owner")) {
            int iplayer = v;
            player::AddActor(iplayer, a);
            if (iplayer == 0)
                seen_player0 = true;
        } else {
            player::AddUnassignedActor(a);
        }
    }

    level->changed_stones.clear();

    if (!seen_player0)
        throw XLevelLoading("Error: No player 0 defined!");

    BroadcastMessage("_init", Value(),
                     GridLayerBits(GRID_ITEMS_BIT | GRID_STONES_BIT | GRID_FLOOR_BIT));

    for (auto &elem : level->others) {
        SendMessage(elem, "_init", Value());
    }

    server::InitMoveCounter();
    display::StatusBar *status_bar = display::GetStatusBar();
    status_bar->show_move_counter(server::ShowMoves);
    status_bar->setCMode(server::NoCollisions);
    status_bar->setBasicModes(std::string() +
                              (server::GameCompatibility == GAMET_ENIGMA ? "" : "x") +
                              (server::IsDifficult ? "" : "e"));

    display::FocusReferencePoint();

    level->preparing_level = false;
    server::WorldInitialized = true;

    if (lua::IsFunc(lua::LevelState(), "postinit")) {
        // it is an existing callback function
        if (lua::CallFunc(lua::LevelState(), "postinit", Value(), nullptr) != lua::NO_LUAERROR) {
            throw XLevelRuntime(std::string("callback 'postinit' failed:\n") +
                                lua::LastError(lua::LevelState()));
        }
    }

    return true;
}

void SetMouseForce(V2 f) {
    level->m_mouseforce.add_force(f);
}

void NameObject(Object *obj, const std::string &name) {
    string oldname;
    if (Value v = obj->getAttr("name")) {
        oldname = v.to_string();
        if (oldname.size() > 0 && oldname[0] != '$' && name.size() > 0 && name[0] != '$')
            obj->warning("name '%s' overwritten by '%s'", oldname.c_str(), name.c_str());
        UnnameObject(obj);
    }
    level->name_object(obj, name);
}

void UnnameObject(Object *obj) {
    level->unname(obj);
}

Object *GetNamedObject(const std::string &name) {
    return level->get_named(name);
}

std::list<Object *> GetNamedGroup(const std::string &name, Object *reference) {
    return level->get_group(name, reference);
}

void NamePosition(Value po, const string &name) {
    level->namePosition(po, name);
}

Value GetNamedPosition(const string &name) {
    return level->getNamedPosition(name);
}

PositionList GetNamedPositionList(const string &tmpl, Object *reference) {
    return level->getPositionList(tmpl, reference);
}

bool IsLevelBorder(const GridPos &p) {
    return level->is_border(p);
}

bool IsInsideLevel(const GridPos &p) {
    return level->contains(p);
}

bool IsInsideLevel(const ecl::V2 &p) {
    return level->contains(p);
}

/* -------------------- Force fields -------------------- */

void AddForceField(ForceField *ff) {
    level->forces.push_back(ff);
}

void RemoveForceField(ForceField *ff) {
    level->remove(ff);
}

/* -------------------- Signals, Messages, Actions -------------------- */

void AddSignal(const GridLoc &srcloc, const GridLoc &dstloc, const string &msg) {
// this code is for Oxyd and Enigma < 1.1 compatibility only
#if defined(VERBOSE_MESSAGES)
    fprintf(stderr, "AddSignal src=%i/%i dest=%i/%i msg='%s'\n", srcloc.pos.x, srcloc.pos.y,
            dstloc.pos.x, dstloc.pos.y, msg.c_str());
#endif  // VERBOSE_MESSAGES

    Object *src = GetObject(srcloc);
    Object *dst = GetObject(dstloc);

    if (src == nullptr) {
        Log << ecl::strf("AddSignal: Invalid signal source: src=%i/%i-%d dest=%i/%i-%d msg='%s'\n",
                         srcloc.pos.x, srcloc.pos.y, srcloc.layer, dstloc.pos.x, dstloc.pos.y,
                         dstloc.layer, msg.c_str());
        return;  // ignore signal
    }

    // key: Oxyd uses signals to define the destination of wormholes
    if (src->getObjectType() == Object::ITEM) {
        ItemID src_id = get_id(dynamic_cast<Item *>(src));
        if (src_id >= it_wormhole_off && src_id <= it_wormhole_on) {
            //            Log << ecl::strf("AddSignal: Wormhole signal destination src=%i/%i-%d
            //            dest=%i/%i-%d msg='%s'\n",
            //                srcloc.pos.x, srcloc.pos.y, srcloc.layer, dstloc.pos.x, dstloc.pos.y,
            //                dstloc.layer, msg.c_str());
            src->setAttr("destination", GetFloor(dstloc.pos));  // use floor to guarantee existence
            return;
        }
    }
    if (src->getObjectType() == Object::ITEM) {
        ItemID src_id = get_id(dynamic_cast<Item *>(src));
        if (src_id >= it_vortex_open && src_id <= it_vortex_closed) {
            //            Log << ecl::strf("AddSignal: Vortex signal destination src=%i/%i-%d
            //            dest=%i/%i-%d msg='%s'\n",
            //                srcloc.pos.x, srcloc.pos.y, srcloc.layer, dstloc.pos.x, dstloc.pos.y,
            //                dstloc.layer, msg.c_str());
            TokenList tl = src->getAttr("destination");  // may be empty or may contain some tokens
            tl.push_back(GetFloor(dstloc.pos));          // use floor to guarantee existence);
            src->setAttr("destination", tl);
            return;
        }
    }

    if (dst == nullptr && dstloc.layer == GRID_ITEMS) {
        GridLoc altloc(GRID_STONES, dstloc.pos);
        dst = GetObject(altloc);
        if (dst && !(dst->isKind("st_blocker")))
            // just use blocker stone instead of blocker item as substitution
            dst = nullptr;
    }

    if (dst == nullptr) {
        Log << ecl::strf(
            "AddSignal: Invalid signal destination src=%i/%i-%d dest=%i/%i-%d msg='%s'\n",
            srcloc.pos.x, srcloc.pos.y, srcloc.layer, dstloc.pos.x, dstloc.pos.y, dstloc.layer,
            msg.c_str());
        return;  // ignore signal
    }
    //    Log << ecl::strf("AddSignal: Valid signal destination src=%i/%i-%d (%s) dest=%i/%i-%d (%s)
    //    msg='%s'\n",
    //        srcloc.pos.x, srcloc.pos.y, srcloc.layer, src->getKind(), dstloc.pos.x, dstloc.pos.y,
    //        dstloc.layer, dst->getKind(), msg.c_str());

    Value dstValue(dst);

    if (dst->isKind("st_blocker") || dst->isKind("st_blocker_new") || dst->isKind("it_blocker")) {
        if (!dst->getAttr("name")) {
            NameObject(dst, ecl::strf("$!oxyd!blocker%d", dst->getId()));
        }
        dstValue = dst->getAttr("name");
    }

    if (dst->isKind("it_meditation_hill") || dst->isKind("it_meditaion_bump") ||
        dst->isKind("it_meditation_hollow") || dst->isKind("it_meditation_dent")) {
        if (!dst->getAttr("name"))
            NameObject(dst, ecl::strf("$!oxyd!hillhollow%d", dst->getId()));
        dstValue = dst->getAttr("name");
    }

    if (src->isKind("st_actorimpulse")) {
        ObjectList ol = src->getDefaultedAttr("$!oxyd!destinations", Value(Value::GROUP));
        ol.push_back(dstValue);
        src->setAttr("$!oxyd!destinations", ol);
        return;
    }

    std::string target_key = "target";
    std::string action_key = "action";

    // fourswitch: 4 subsequent AddSignal calls for state specific signals
    if (src->getObjectType() == Object::STONE &&
        get_id(dynamic_cast<Stone *>(src)) == st_fourswitch) {
        for (int i = NORTH;; i--) {
            if (!src->getAttr(ecl::strf("target_%d", i))) {
                target_key = ecl::strf("target_%d", i);
                action_key = ecl::strf("action_%d", i);
                break;
            }
        }
    }

    // key: Oxyd uses signals from keys to key switches to determine which keys
    // activate which key hole
    if (src->getObjectType() == Object::ITEM) {
        ItemID src_id = get_id(dynamic_cast<Item *>(src));
        if (src_id == it_key && dst->isKind("st_key")) {
            dst->setAttr("code", src->getAttr("code"));
            return;
        }
    }

    // this function is supported for old API only - we can assume that target, action
    // are not set by other means than this function:
    TokenList targets = src->getDefaultedAttr(target_key, Value(Value::TOKENS));
    targets.push_back(dstValue);  // add this target to existing ones
    src->setAttr(target_key, Value(targets));
    TokenList actions = src->getDefaultedAttr(action_key, Value(Value::TOKENS));
    actions.push_back(Value("signal"));  // add this target to existing ones
    src->setAttr(action_key, actions);
}

Value SendMessage(Object *obj, const std::string &msg, const Value &value, Object *sender) {
    return SendMessage(obj, Message(msg, value, sender));
}

Value SendMessage(Object *obj, const Message &m) {
    if (obj) {
        if (TrackMessages)
            obj->warning("will be sent message '%s' (with Value)", m.message.c_str());
        return obj->message(m);
    } else if (TrackMessages) {
        fprintf(stderr, "Sending message '%s' to NULL-object\n", m.message.c_str());
        return Value();
    }
    return Value();
}

void BroadcastMessage(const std::string &msg, const Value &value, GridLayerBits grids, bool actors,
                      bool others) {
    bool to_floors = (grids & GRID_FLOOR_BIT) != 0;
    bool to_items = (grids & GRID_ITEMS_BIT) != 0;
    bool to_stones = (grids & GRID_STONES_BIT) != 0;

    //    Uint32 start_tick_time = SDL_GetTicks();   // meassure time for level loading

    if (grids != 0) {
        for (int y = 0; y < level->h; ++y) {
            for (int x = 0; x < level->w; ++x) {
                GridPos p(x, y);
                Field *f = level->get_field(p);
                if (to_floors && f->floor)
                    SendMessage(f->floor, msg, value);
                if (to_items && f->item)
                    SendMessage(f->item, msg, value);
                if (to_stones && f->stone)
                    SendMessage(f->stone, msg, value);
            }
        }
    }
    if (actors) {
        for (auto &actor : level->actorlist)
            SendMessage(actor, msg, value);
    }
    if (others) {
        for (auto &elem : level->others)
            SendMessage(elem, msg, value);
    }

    //    double exectime = (SDL_GetTicks() - start_tick_time)/1000.0;
    //    Log << ecl::strf("World init did take %g seconds for floors %d, items %d, stones %d,
    //    actors %d, others %d\n",
    //         exectime, to_floors, to_items, to_stones, actors, others);
}

void PerformSecureAction(int senderId, bool isCallback, int targetId, std::string name, Value val) {
    level->actionList.push_back(Action(senderId, isCallback, targetId, name, val));
}

namespace {
void explosion(GridPos source, GridPos dest, const char *explosion_item) {
    if (Stone *stone = GetStone(dest))
        SendMessage(stone, "_explosion", source);
    if (source == dest)  // item and floor handling on explosion center is specific
        return;
    if (Item *item = GetItem(dest)) {
        if (item->getClass() == "it_explosion") {
            Item *newExplosion = MakeItem(explosion_item);
            if ((int)(newExplosion->getAttr("state")) > (int)(item->getAttr("state")))
                SetItem(dest, newExplosion);
            else
                DisposeObject(newExplosion);
        } else if (has_flags(item, itf_indestructible))
            SendMessage(item, "_explosion", source);
        else
            SetItem(dest, MakeItem(explosion_item));
    } else
        SetItem(dest, MakeItem(explosion_item));
    if (Floor *floor = GetFloor(dest))
        SendMessage(floor, "_explosion");
}
}

void SendExplosionEffect(GridPos center, ExplosionType type) {
    const int AFFECTED_FIELDS = 9;

    for (int a = 0; a < AFFECTED_FIELDS; ++a) {
        GridPos dest = get_neighbour(center, a);
        bool direct_neighbor = a <= 4;

        switch (type) {
        case EXPLOSION_DYNAMITE:
            if (Stone *stone = GetStone(dest))
                SendMessage(stone, "ignite");
            if (Item *item = GetItem(dest))
                SendMessage(item, "ignite");
            if (Floor *floor = GetFloor(dest))
                SendMessage(floor, "ignite");
            break;

        case EXPLOSION_BLACKBOMB:
            if (direct_neighbor) {
                explosion(center, dest, "it_explosion_nil");
            } else if (server::GameCompatibility == GAMET_ENIGMA) {
                // Note: should not ignite in non-enigma-mode!
                if (Stone *stone = GetStone(dest))
                    SendMessage(stone, "ignite");
                if (Item *item = GetItem(dest))
                    SendMessage(item, "ignite");
                if (Floor *floor = GetFloor(dest))
                    SendMessage(floor, "ignite");
            }
            break;

        case EXPLOSION_WHITEBOMB:
            if (direct_neighbor || server::GameCompatibility == GAMET_ENIGMA) {
                // Note: at least in oxyd1 only direct neighbors
                // explode, and the others not even ignite
                explosion(center, dest, "it_explosion_debris");
            }
            break;

        case EXPLOSION_DISPENSER:
            if (direct_neighbor) {
                if (Stone *stone = GetStone(dest))
                    SendMessage(stone, "_dispenser");
                if (Item *item = GetItem(dest))
                    SendMessage(item, "_dispenser");
                if (Floor *floor = GetFloor(dest))
                    SendMessage(floor, "_dispenser");
            }
            break;

        case EXPLOSION_SPITTER:
            // TODO: spitter explosions
            break;
        }
    }
}

Object *GetObject(const GridLoc &l) {
    switch (l.layer) {
    case GRID_FLOOR: return GetFloor(l.pos);
    case GRID_ITEMS: return GetItem(l.pos);
    case GRID_STONES: return GetStone(l.pos);
    default: return nullptr;
    }
}

const Field *GetField(GridPos p) {
    return level->get_field(p);
}

/* -------------------- Other manipulation -------------------- */

void AddOther(Other *o) {
    level->others.push_back(o);
    Rubberband *rb = dynamic_cast<Rubberband *>(o);
    if (rb != nullptr)
        level->rubberbands.push_back(rb);
    o->postAddition();
}

void KillOther(Other *o) {
    if (o == nullptr)
        return;

    o->preRemoval();
    auto i = find(level->others.begin(), level->others.end(), o);
    if (i != level->others.end()) {
        level->others.erase(i);
        Rubberband *rb = dynamic_cast<Rubberband *>(o);
        if (rb != nullptr) {
            auto j = find(level->rubberbands.begin(), level->rubberbands.end(), rb);
            if (j != level->rubberbands.end()) {
                level->rubberbands.erase(j);
            }
        }
        DisposeObject(o);
    }
}

/* -------------------- Floor manipulation -------------------- */

void KillFloor(GridPos p) {
    level->fl_layer.kill(p);
    if (server::EnigmaCompatibility >= 1.10)
        lua::SetDefaultFloor(lua::LevelState(), p.x, p.y);
}

Floor *GetFloor(GridPos p) {
    return level->fl_layer.get(p);
}

void SetFloor(GridPos p, Floor *fl) {
    level->fl_layer.set(p, fl);
    if (!level->preparing_level)
        if (Stone *st = GetStone(p))
            st->on_floor_change();
}

/* -------------------- Stone manipulation -------------------- */

Stone *GetStone(GridPos p) {
    if (Field *f = level->get_field(p))
        return f->stone;
    else
        return level->st_layer.get(p);
}

void KillStone(GridPos p) {
    level->st_layer.kill(p);
    level->changed_stones.push_back(p);
}

Stone *YieldStone(GridPos p) {
    Stone *st = level->st_layer.yield(p);
    level->changed_stones.push_back(p);
    return st;
}

void SetStone(GridPos p, Stone *st) {
    level->st_layer.set(p, st);
    level->changed_stones.push_back(p);
    if (level->registerCriticalPositions)
        level->collisionCriticalPositions.push_back(p);
}

void ReplaceStone(GridPos p, Stone *st) {
    Stone *old = level->st_layer.get(p);
    if (old) {
        old->transferName(st);
        level->st_layer.kill(p);
    }
    SetStone(p, st);
}

void MoveStone(GridPos oldPos, GridPos newPos) {
    SetStone(newPos, YieldStone(oldPos));
}

void TouchStone(GridPos pos) {
    level->changed_stones.push_back(pos);
    if (level->registerCriticalPositions)
        level->collisionCriticalPositions.push_back(pos);
}

void SetScrambleIntensity(int intensity) {
    level->scrambleIntensity = intensity;
}

void AddScramble(GridPos p, Direction d) {
    level->add_scramble(p, d);
}

/* -------------------- Item manipulation -------------------- */

void KillItem(GridPos p) {
    MaybeRecalcLight(p);
    level->it_layer.kill(p);
}

Item *GetItem(GridPos p) {
    return level->it_layer.get(p);
}

Item *YieldItem(GridPos p) {
    MaybeRecalcLight(p);
    return level->it_layer.yield(p);
}

void SetItem(GridPos p, Item *it) {
    MaybeRecalcLight(p);
    level->it_layer.set(p, it);
}

/* -------------------- Actor manipulation -------------------- */

void AddActor(double x, double y, Actor *a) {
    level->add_actor(a, V2(x, y));
}

void AddActor(Actor *a) {
    level->add_actor(a);
}

void DidMoveActor(Actor *a) {
    level->did_move_actor(a);
}

Actor *YieldActor(Actor *a) {
    return level->yield_actor(a);
}

void KillActor(Actor *a) {
    delete YieldActor(a);
}

void WarpActor(Actor *a, double newx, double newy, bool keep_velocity) {
    V2 newpos = V2(newx, newy);
    ASSERT(IsInsideLevel(GridPos(newpos)), XLevelRuntime, "WarpActor: Tried to warp actor out of "
                                                          "level grid. (And hyperspace travel is "
                                                          "not implemented yet, sorry.)");
    if (!keep_velocity)
        a->get_actorinfo()->vel = V2();
    a->warp(newpos);
}

void FastRespawnActor(Actor *a, bool keep_velocity) {
    const V2 &p = a->get_respawnpos();
    WarpActor(a, p[0], p[1], keep_velocity);
}

void RespawnActor(Actor *a) {
    a->find_respawnpos();
    a->respawn();
}

Actor *FindActorByID(ActorID id) {
    for (auto &actor : level->actorlist) {
        if (get_id(actor) == id)
            return actor;
    }
    return nullptr;
}

unsigned CountActorsOfKind(ActorID id) {
    unsigned count = 0;
    for (auto &actor : level->actorlist) {
        if (get_id(actor) == id)
            ++count;
    }
    return count;
}

Actor *FindOtherMarble(Actor *thisMarble) {
    if (!thisMarble)
        return nullptr;

    switch (get_id(thisMarble)) {
    case ac_marble_black: return FindActorByID(ac_marble_white);
    case ac_marble_white: return FindActorByID(ac_marble_black);
    default: return nullptr;
    }
}

bool ExchangeMarbles(Actor *marble1) {
    if (Actor *marble2 = FindOtherMarble(marble1)) {
        level->exchange_actors(marble1, marble2);
        return true;
    }
    return false;
}

void GrabActor(Actor *a) {
    a->get_actorinfo()->grabbed = true;
}

void ReleaseActor(Actor *a) {
    a->get_actorinfo()->grabbed = false;
}

bool GetActorsInRange(ecl::V2 center, double range, vector<Actor *> &actors) {
    for (auto &a : level->actorlist) {
        if (length(a->get_pos() - center) < range)
            actors.push_back(a);
    }
    return !actors.empty();
}

bool GetActorsInsideField(const GridPos &pos, vector<Actor *> &actors) {
    for (auto &a : level->actorlist) {
        if (a->get_gridpos() == pos)
            actors.push_back(a);
    }
    return !actors.empty();
}

void ShatterActorsInsideField(const GridPos &p) {
    vector<Actor *> actors;
    GetActorsInsideField(p, actors);
    for (auto &actor : actors)
        SendMessage(actor, "_shatter");
}

/* -------------------- Functions -------------------- */

void ChangeMeditation(int diffMeditatists, int diffIndispensableHollows,
                      int diffEngagedIndispensableHollows, int diffEngagedDispensableHollows) {
    level->numMeditatists += diffMeditatists;
    level->indispensableHollows += diffIndispensableHollows;
    level->engagedIndispensableHollows += diffEngagedIndispensableHollows;
    level->engagedDispensableHollows += diffEngagedDispensableHollows;

    if (!level->preparing_level &&
        level->indispensableHollows == level->engagedIndispensableHollows &&
        (level->engagedIndispensableHollows + level->engagedDispensableHollows) ==
            level->numMeditatists) {
        server::FinishLevel();
    }
    //    Log << "chang meditation:  "<< level->numMeditatists << " " << level->indispensableHollows
    //    << " "<<level->engagedIndispensableHollows << " " << level->engagedDispensableHollows <<
    //    "\n";
}

/*! Determine whether the actor hitting a stone in a StoneContact
  has enough velocity to move the stone. Return either the direction
  the stone should move or NODIR. */
Direction GetPushDirection (const StoneContact &sc) {
    ActorInfo *ai  = sc.actor->get_actorinfo();
    Direction  dir = contact_face(sc);

    // Make sure the speed component towards the face of the stone is
    // large enough and pointing towards the stone.
    if (dir != enigma::NODIR && ai->vel * sc.normal < -4)
        return reverse(dir);
    return NODIR;
}

float GetVolume(const char *name, Object *obj, float def_volume) {
    if (def_volume == 0.0 || sound::IsSoundMute())
        return 0;
    // See SoundEffectManager.hh for details.
    for (auto &elem : level->sound_dampings) {
        if (elem.is_equal(name, obj))
            return elem.get_volume(def_volume);
    }
    // No entry found for this object. Create a new one.
    level->sound_dampings.push_back(sound::SoundDamping(name, obj));
    return def_volume;
}

void WorldTick(double dtime) {
    level->tick(dtime);
}

void TickFinished(double dtime) {
    for (auto &actor : level->actorlist)
        actor->move_screen();
    for (auto &other : level->others)
        other->tick(dtime);
}

void InitWorld() {
    Object::bootFinished();
    BootRegister(nullptr, nullptr, false);
    InitLasers();
    client::ClientInit();
}

void ShutdownWorld() {
    level.reset();
    player::PlayerShutdown();
    Repos_Shutdown();
    WorldProxy::shutdown();
    client::ClientShutdown();
}

/* -------------------- Object repository -------------------- */
namespace {

class ObjectRepos : public ecl::Nocopy {
public:
    ObjectRepos();
    ~ObjectRepos();
    void add_templ(const string &name, Object *o);
    bool has_templ(const string &name);
    Object *make(const string &name);
    Object *get_template(const string &name);

    void dump_info();

private:
    typedef std::map<string, Object *> ObjectMap;
    ObjectMap objmap;  // repository of object templates
    int stonecount, floorcount, itemcount;
};

}  // namespace

ObjectRepos::ObjectRepos() {
    stonecount = floorcount = itemcount = 0;
}

ObjectRepos::~ObjectRepos() {
    ecl::delete_map(objmap.begin(), objmap.end());
}

void ObjectRepos::add_templ(const string &kind, Object *o) {
    if (has_templ(kind))
        enigma::Log << "add_templ: redefinition of object `" << kind << "'.\n";
    else
        objmap[kind] = o;
}

bool ObjectRepos::has_templ(const string &name) {
    return objmap.find(name) != objmap.end();
}

Object *ObjectRepos::make(const string &name) {
    auto i = objmap.find(name);
    if (i == objmap.end())
        return nullptr;
    else
        return i->second->clone();
}

Object *ObjectRepos::get_template(const string &name) {
    if (objmap.find(name) != objmap.end())
        return objmap[name];
    else
        return nullptr;
}

/* Generate a list of all available objects and their attributes. */
void ObjectRepos::dump_info() {
    for (auto iter = objmap.begin(); iter != objmap.end(); ++iter) {
        cout << iter->first << "( ";
        Object *obj = iter->second;
        const Object::AttribMap &a = obj->get_attribs();
        for (const auto &elem : a) {
            if (elem.first != "kind")
                cout << elem.first << " ";
        }
        cout << ")\n";
    }
}

namespace {

struct BootKindObject {
    std::string kind;
    Object *object;
    BootKindObject(std::string name, Object *o) : kind(std::move(name)), object(o) {}
};

ObjectRepos *repos;

}  // namespace

void BootRegister(Object *obj, const char *name, bool isRegistration) {
    static std::list<BootKindObject *> templates;
    if (isRegistration) {
        std::string kind = (name != nullptr ? std::string(name) : std::string(""));
        templates.push_back(new BootKindObject(kind, obj));
    } else {
        int count = 0;
        for (auto &templ : templates) {
            Register(templ->kind, templ->object);
            delete templ;
            count++;
        }
        Log << count << " boot registered object\n";
    }
}

void Register(const std::string &kind, Object *obj) {
    if (!repos)
        repos = new ObjectRepos;
    ASSERT(!kind.empty(), XLevelRuntime, "Registration of object without kind");
    repos->add_templ(kind, obj);
}

void Repos_Shutdown() {
    delete repos;
}

Object *MakeObject(const char *kind) {
    static Object *last_templ = nullptr;
    static string last_kind;

    if (last_kind != kind) {
        last_kind = kind;
        last_templ = repos->get_template(kind);
    }

    Object *obj = nullptr;
    if (last_templ)
        obj = last_templ->clone();
    ASSERT(obj != nullptr, XLevelRuntime,
           ecl::strf("MakeObject: unknown object name `%s'\n", kind).c_str());
    return obj;
}

Object *GetObjectTemplate(const std::string &kind) {
    if (!repos->has_templ(kind)) {
        cerr << "GetObjectTemplate: unknown object name `" << kind << "'.\n";
        return nullptr;
    } else
        return repos->get_template(kind);
}

Floor *MakeFloor(const char *kind) {
    return dynamic_cast<Floor *>(MakeObject(kind));
}

Item *MakeItem(const char *kind) {
    return dynamic_cast<Item *>(MakeObject(kind));
}
Stone *MakeStone(const char *kind) {
    return dynamic_cast<Stone *>(MakeObject(kind));
}

Actor *MakeActor(const char *kind) {
    return dynamic_cast<Actor *>(MakeObject(kind));
}

void DisposeObject(Object *o) {
    level->dispose_object(o);
}

void DumpObjectInfo() {
    repos->dump_info();
}

}  // namespace enigma
