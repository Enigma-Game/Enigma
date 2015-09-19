/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
 * Copyright (C) 2008,2009 Ronald Lamprecht
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
#include "enigma.hh"
#include "player.hh"
#include "Inventory.hh"
#include "SoundEffectManager.hh"
#include "server.hh"
#include "world.hh"
#include "main.hh"

#include <iostream>
#include <set>

using namespace std;
using namespace enigma;
//using namespace world;
using ecl::V2;

//#include "actors_internal.hh"

namespace enigma {

const double Actor::max_radius = 24.0/64;


/* -------------------- ActorsInRangeIterator -------------------- */

ActorsInRangeIterator::ActorsInRangeIterator(Actor *center, double range,
        unsigned type_mask) : centerActor (center), previousActor (center),
        dir (WEST), rangeDist (range), typeMask (type_mask) {
    xCenter = center->m_actorinfo.pos[0];
}

Actor * ActorsInRangeIterator::next() {
    bool ready = false;
    while(!ready) {
        if (previousActor != NULL) {
            if (dir == WEST) {
                previousActor = previousActor->left;
            } else {
                previousActor = previousActor->right;
            }
        }
        if (dir == WEST && (previousActor == NULL
                || xCenter - previousActor->m_actorinfo.pos[0] > rangeDist)) {
            previousActor = centerActor->right;
            dir = EAST;
        }
        if (dir == EAST && (previousActor == NULL
                || previousActor->m_actorinfo.pos[0] - xCenter > rangeDist)) {
            ready = true;
            previousActor = NULL;
            continue;
        }
        unsigned id_mask = previousActor->get_traits().id_mask;
        if (id_mask & typeMask) {
            if (length(previousActor->m_actorinfo.pos - centerActor->m_actorinfo.pos)
                    < rangeDist) {
                ready = true;
            }
        }
    }
    return previousActor;
}


/* -------------------- Actor -------------------- */

Actor::Actor (const ActorTraits &tr)
: StateObject(tr.name),
  m_actorinfo(),
  m_sprite(),
  startingpos(),
  respawnpos(), flagRespawn(false), centerRespawn (true), inplaceRespawn (false),
  spikes(false), controllers (0), left (NULL), right (NULL)
{
    setAttr("adhesion", 0.0);

    // copy default properties to dynamic properties
    m_actorinfo.mass = tr.default_mass;
    m_actorinfo.radius = tr.radius;
    m_actorinfo.created = false;

    ASSERT(m_actorinfo.radius <= get_max_radius(), XLevelRuntime, "Actor: radius of actor too large");
}

    void Actor::setAttr(const string& key, const Value &val) {
        if (key == "controllers") {
            controllers = val;
        } else if (key == "adhesion") {
            adhesion = val;
        } else if (key == "charge") {
            m_actorinfo.charge = val;
        } else
            Object::setAttr(key, val);
    }

    Value Actor::getAttr(const std::string &key) const {
        if (key == "controllers") {
            return controllers;
        } else if (key == "adhesion") {
            return adhesion;
        } else if (key == "charge") {
            return m_actorinfo.charge;
        } else if (key == "mass") {
            return m_actorinfo.mass;
        } else
            return StateObject::getAttr(key);
    }

    Value Actor::message(const Message &m) {
        if (m.message == "_freeze") {
            m_actorinfo.frozen_vel = m_actorinfo.vel;
            m_actorinfo.vel = ecl::V2();
        } else if (m.message == "_revive") {
            m_actorinfo.vel = m_actorinfo.frozen_vel;
        } else if (m.message == "_update_mass") {
            if (getAttr("owner") == m.value) {
                m_actorinfo.mass = get_traits().default_mass +
                        (double)(player::GetInventory(this)->getAttr("mass"));
                ASSERT(m_actorinfo.mass > 0, XLevelRuntime, "Actor mass <= 0!");
                SendMessage(GetFloor(get_gridpos()), "_update_mass", true, this);
//                Log << "Actor new mass " << m_actorinfo.mass << "\n";
            }
        } else if (m.message == "_update_pin") {
            if (getAttr("owner") == m.value) {
                spikes = (player::GetInventory(this)->find("it_pin")) != -1;
//                Log << "Actor has spikes " << spikes << "\n";
            }
        }
        return StateObject::message(m);
    }

bool Actor::on_collision(Actor *a) {
    return false;
}

ActorInfo *Actor::get_actorinfo() {
    return &m_actorinfo;
}

const ActorInfo &Actor::get_actorinfo() const {
    return m_actorinfo;
}

const ecl::V2 &Actor::get_pos() const
{
    return m_actorinfo.pos;
}


const ecl::V2 &Actor::get_pos_force() const{
    return m_actorinfo.pos_force;
}


double Actor::get_max_radius() {
    return max_radius;
}

void Actor::think(double /*dtime*/) {
    if (m_actorinfo.field) {
        Floor *fl = m_actorinfo.field->floor;
        Item *it = m_actorinfo.field->item;
        bool item_covers_floor = (it && it->covers_floor(m_actorinfo.pos, this));
        if (!item_covers_floor && fl && this->is_on_floor())
            fl->actor_contact(this);
    }
}

void Actor::set_respawnpos(const V2& p)
{
    respawnpos     = p;
    flagRespawn = true;
}

void Actor::remove_respawnpos() {
    flagRespawn = false;
}

void Actor::find_respawnpos() {
//    V2& what_pos = flagRespawn ? respawnpos : startingpos;
//    FreeRespawnLocationFinder unblocked(what_pos, *this);
//    what_pos = unblocked.get_position();
}

const V2& Actor::get_respawnpos() const {
    return flagRespawn ? respawnpos : startingpos;
}

const V2 &Actor::get_startpos() const {
    return startingpos;
}

void Actor::respawn() {
    ecl::V2 p = startingpos; // default respawn on initial position
    if (flagRespawn || server::AutoRespawn) {
        if (inplaceRespawn)
            p = respawnpos;
        else if (centerRespawn)
            p = GridPos(respawnpos).center();
        else {  // respawn in nearest edge of grid (thus avoiding laser beams)
            p = respawnpos;
            GridPos gp(respawnpos);
            double dx = respawnpos[0] - gp.x;
            double dy = respawnpos[1] - gp.y;
            if (dx > 0.28 && dx < 0.5)
                p[0] = gp.x + 0.28;
            else if (dx < 0.72 && dx >= 0.5)
                p[0] = gp.x + 0.72;

            if (dy > 0.28 && dy < 0.5)
                p[1] = gp.y + 0.28;
            else if (dy < 0.72 && dy >= 0.5)
                p[1] = gp.y + 0.72;
       }
    }
    warp(p);
    on_respawn(p);
}

void Actor::add_force (const ecl::V2 &f) {
    m_actorinfo.forceacc += f;
}

void Actor::init() {
    m_sprite = display::AddSprite(get_pos());
}

void Actor::on_creation(const ecl::V2 &p)  {
    if (!m_actorinfo.created) { // avoid reinitialization on it_drop usage
        m_actorinfo.created = true;
        startingpos = get_pos();
        if (Value vx = getAttr("velocity_x")) {
            m_actorinfo.vel = V2(vx,  m_actorinfo.vel[1]);
        }
        if (Value vy = getAttr("velocity_y")) {
            m_actorinfo.vel = V2(m_actorinfo.vel[0], vy);
        }
    }
    set_model(getKind());
    m_sprite.move(p);
    move();
}

void Actor::on_respawn (const ecl::V2 &/*pos*/) {
    centerRespawn = true;
}

void Actor::warp(const ecl::V2 &newpos) {
    m_actorinfo.pos = newpos;
    DidMoveActor(this);
    m_actorinfo.vel = V2();
    m_sprite.move (newpos);
    move ();
    // notify rubberbands that may now exceed max/min limits
    ObjectList olist = getAttr("rubbers");   // a private deletion resistant copy
    for (ObjectList::iterator itr = olist.begin(); itr != olist.end(); ++itr)
        SendMessage(*itr, "_recheck");
}

void Actor::move()
{
    if (m_actorinfo.field) {
        if (m_actorinfo.gridpos != m_actorinfo.last_gridpos) {
            // Actor entered a new field -> notify floor and item objects
            // first leave old - avoid the possibility that an actor presses
            // two triggers at once.
            firstGridStep = true;
            if (const Field *of = GetField(m_actorinfo.last_gridpos)) {
                if (Floor *fl = of->floor)
                    fl->actor_leave(this);
                if (Item *it = of->item)
                    it->actor_leave(this);
            }
            // then enter new field
            if (Floor *fl = m_actorinfo.field->floor)
                fl->actor_enter(this);
            if (Item *it = m_actorinfo.field->item)
                it->actor_enter(this);

        }

        Item *it = m_actorinfo.field->item;
        if (it && it->actor_hit(this))
            player::PickupItem (this, m_actorinfo.gridpos);

        if (Stone *st = m_actorinfo.field->stone)
            st->actor_inside (this);

        if (firstGridStep && !is_flying()) {
            firstGridStep = false;
        } else if (!flagRespawn && !isMoribund() && !is_flying()) {
            Floor *fl = m_actorinfo.field->floor;
            if (fl != NULL) {
                if (fl->getAdhesion() != 0) {
                    respawnpos = m_actorinfo.pos;
                }
            } else {
                // Should never happen but occurs when there is no floor set
                // in the level. The (only?) way to get this is loading old
                // API levels which do not set a floor for all tiles in the world.
                // Note that in the new API we have a default floor in any case.
                Log << "Warning: no floor type set for current tile!\n";
            }
        }
    }
    m_actorinfo.last_gridpos = m_actorinfo.gridpos;
}

void Actor::move_screen() {
    m_sprite.move (m_actorinfo.pos);
}

void Actor::set_model(const string &name) {
    m_sprite.replace_model (display::MakeModel(name));
}

void Actor::animcb() {
}


void Actor::hide() {
    m_sprite.hide();
}

void Actor::show() {
    m_sprite.show();
}

void Actor::set_anim (const string &modelname) {
    set_model(modelname.c_str());
    get_sprite().set_callback (this);
}

bool Actor::can_move() const {
    if (Stone *st = GetStone (get_gridpos())) {
        if (!server::NoCollisions || !(get_traits().id_mask &
                        (1<<ac_marble_white | 1<<ac_marble_black | 1<<ac_pearl_white | 1<<ac_pearl_black)))
            return !st->is_sticky(this);
    }
    return true;
}


bool Actor::sound_event (const char *name, double vol) {
    return sound::EmitSoundEvent (name, get_pos(), GetVolume(name, this, vol));
}

    double Actor::squareDistance(const Object *other) const {
        const Actor *a = dynamic_cast<const Actor *>(other);
        if (a != NULL)
            return ecl::square(get_pos() - a->get_pos());
        else
            return other->squareDistance(this);
    }

    bool Actor::isSouthOrEastOf(const Object *other) const {
        const Actor *a = dynamic_cast<const Actor *>(other);
        if (a != NULL)
            return (get_pos()[1] > - a->get_pos()[1]) || ((get_pos()[1] ==  a->get_pos()[1]) && (get_pos()[0] > - a->get_pos()[0]));
        else
            return !(other->isSouthOrEastOf(this));
    }

} // namespace enigma
