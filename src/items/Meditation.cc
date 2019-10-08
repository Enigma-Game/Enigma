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

#include "items/Meditation.hh"

#include "main.hh"
#include "world.hh"

namespace enigma {

    Meditation::Meditation(int initState) {
        state = initState;
        whiteball = NULL;
    }

    std::string Meditation::getClass() const {
        return "it_meditation";
    }

    void Meditation::setAttr(const std::string& key, const Value &val) {
        if (key == "essential") {
            bool essential = (val == 1);
            if (essential != (bool)(objFlags & OBJBIT_INDISPENSIBLE)) {
                if (isDisplayable()) {
                    if (whiteball != NULL && enter_time == -1) {   // meditatist is registered
                        bool indispensable = objFlags & OBJBIT_INDISPENSIBLE;
                        ChangeMeditation(0, 0, indispensable ? -1 : +1, indispensable ? +1 : -1);
                    }
                    ChangeMeditation(0, essential ? +1 : -1 , 0, 0);
                }
                objFlags ^= OBJBIT_INDISPENSIBLE;
            }
            return;
        }
        Item::setAttr(key, val);
    }

    Value Meditation::getAttr(const std::string &key) const {
        if (key == "essential") {
            return (objFlags & OBJBIT_INDISPENSIBLE) ? 1 : 0;
        }
        return Item::getAttr(key);
    }

    Value Meditation::message(const Message &m) {
        if (m.message == "flip") {
            setState(-state);
            return Value();
        } else if (m.message == "signal") {
            setState(m.value != 0 ? std::abs(state) : -std::abs(state));
            return Value();
        } else if (m.message == "shovel") {
            shovel();
            return Value();
        } else if (m.message == "_init") {
            checkActors();
            return Value();
        } else if (m.message == "_dying" ) {
            if (whiteball == m.sender && m.value.to_bool() == true)
                // meditatist left hollow (warp, ...)
                deregisterWhiteball();
            return Value();
        } else
            return Item::message(m);
    }

    int Meditation::minState() const {
        return CALDERA;
    }
    int Meditation::maxState() const {
        return VOLCANO;
    }

    void Meditation::toggleState() {
        int newState;
        switch (state) {
            case CALDERA: newState = HILL; break;
            case HOLLOW:  newState = VOLCANO; break;
            case DENT:    newState = BUMP; break;
            case BUMP:    newState = DENT; break;
            case HILL:    newState = CALDERA; break;
            case VOLCANO: newState = HOLLOW; break;
        }
        setState(newState);
    }

    void Meditation::setState(int extState) {
        if (extState != state && extState != 0) {
            state = extState;
            if (isDisplayable()) {
                init_model();
            }
        }
    }

    void Meditation::on_creation(GridPos p) {
        if (objFlags & OBJBIT_INDISPENSIBLE)
            ChangeMeditation(0, +1, 0, 0);
        if (server::WorldInitialized)
            checkActors();
        Item::on_creation(p);
    }

    void Meditation::on_removal(GridPos p) {
        if (objFlags & OBJBIT_INDISPENSIBLE)
            ChangeMeditation(0, -1, 0, 0);
        if (whiteball != NULL)
            deregisterWhiteball();
        Item::on_removal(p);
    }

    void Meditation::actor_leave(Actor *a) {
        if (whiteball == a)
            // meditatist left hollow (warp, ...)
            deregisterWhiteball();
    }

    void Meditation::on_stonehit(Stone *) {
        shovel();
    }

    bool Meditation::actor_hit(Actor *a) {
        static const double MINTIME = 1.0;
        ItemID theid = get_id(this);

        if (whiteball == NULL && !a->is_flying() && !a->is_dead()
                && (get_id(a) == ac_pearl_white || get_id(a) == ac_pearl_black) && isMeditating(a)) {
            // meditatist entered a free hollow
            whiteball  = a;
            enter_time = server::LevelTime;
        } else if (whiteball == a) {
            if (a->is_flying() || a->is_dead() || !isMeditating(a)) {
                // meditatist left hollow
                whiteball = NULL;
                if (enter_time == -1) {   // meditatist is registered
                    bool indispensable = objFlags & OBJBIT_INDISPENSIBLE;
                    ChangeMeditation(0, 0, indispensable ? -1 : 0, indispensable ? 0 : -1);
                }
            } else  if (enter_time != -1 && (server::LevelTime - enter_time) >= MINTIME) {
                    // just meditated enough to mark hollow as engaged
                    bool indispensable = objFlags & OBJBIT_INDISPENSIBLE;
                    ChangeMeditation(0, 0, indispensable ? +1 : 0, indispensable ? 0 : +1);
                    enter_time = -1;  // mark as registered
            }
        }

        return false;  // do not pick up
    }

    void Meditation::add_force(Actor *a, ecl::V2 &f) {
        ecl::V2 v = a->get_pos() - get_pos().center();
        double dist = ecl::length(v);

        if (dist > (std::abs(state) > 1 ? 0.5 : 0.3))
            return;

        if (dist <= 0) { // exactly on hill-top
            ActorInfo *ai = a->get_actorinfo();
            if (length(ai->vel) <= 0) { // no velocity
                // we are never "exactly" on the top!
                double x = DoubleRand(-0.03, 0.05);
                double y = DoubleRand(-0.03, 0.05);
                x = (x < 0.01) ? x - 0.02 : x;
                y = (y < 0.01) ? y - 0.02 : y;
                v = ecl::V2(x, y);
            }
        }

        f += (state < 0 ? -1 : 1) * ((std::abs(state) == VOLCANO && dist < 0.3) ? -1 : 1)
                * 90 * server::HoleForce * v; // get the force
    }

    double Meditation::getFriction(ecl::V2 position, double defaultFriction, Actor *a) {
        Value v = getAttr("friction");
        if (v && covers_floor(position, a))
            return v;
        else
            return defaultFriction;
    }

    ecl::V2 Meditation::calcMouseforce(Actor *a, ecl::V2 mouseForce, ecl::V2 floorForce) {
        Value v = getAttr("adhesion");
        if (v && covers_floor(a->get_pos(), a))
            return mouseForce * (double)v ;
        else
            return floorForce;
    }

    bool Meditation::isMeditating(Actor *a) {
        double dist = ecl::length(a->get_pos() - get_pos().center());
        return dist < 0.24 || ((state <= HOLLOW || state >= HILL) && dist < 0.4) ;
    }

    void Meditation::shovel() {
        int newState = (state > 0) ? state - 1 : state + 1;
        if (newState == 0) {
//            // double register indispensible holes as the kill will subtract one essentialness
//            if (/* state != HILL && state != BUMP && */ (objFlags & OBJBIT_INDISPENSIBLE))
//                ChangeMeditation(0, +1, 0, 0);
            kill();
        } else {
            setState(newState);
        }
    }

    void Meditation::checkActors() {
        ItemID theid = get_id(this);
        std::vector<Actor*> actors;
        GetActorsInsideField(get_pos(), actors);
        for (std::vector<Actor*>::iterator itr = actors.begin(); itr != actors.end(); ++itr) {
            if (!(*itr)->is_flying() &&  whiteball==NULL
                    && (get_id(*itr)==ac_pearl_white || get_id(*itr)==ac_pearl_black) && isMeditating(*itr)) {
                 // meditatist entered a free hollow
                whiteball  = *itr;
                enter_time = server::LevelTime;
                break;
            }
        }
    }

    void Meditation::deregisterWhiteball() {
        if (whiteball != NULL && enter_time == -1) {   // meditatist is registered
            bool indispensable = objFlags & OBJBIT_INDISPENSIBLE;
            ChangeMeditation(0, 0, indispensable ? -1 : 0, indispensable ? 0 : -1);
        }
        whiteball = NULL;
    }

    int Meditation::traitsIdx() const {
        return state < 0 ? state + 3 : state + 2;
    }

    ItemTraits Meditation::traits[6] = {
        {"it_meditation_caldera",  it_meditation_caldera,  itf_static | itf_fireproof | itf_freezable},
        {"it_meditation_hollow",  it_meditation_hollow,  itf_static | itf_fireproof | itf_freezable},
        {"it_meditation_dent",  it_meditation_dent,  itf_static | itf_fireproof | itf_freezable},
        {"it_meditation_bump",  it_meditation_bump,  itf_static | itf_fireproof | itf_freezable},
        {"it_meditation_hill",  it_meditation_hill,  itf_static | itf_fireproof | itf_freezable},
        {"it_meditation_volcano",  it_meditation_volcano,  itf_static | itf_fireproof | itf_freezable},
    };

    BOOT_REGISTER_START
        BootRegister(new Meditation(-2), "it_meditation");
        BootRegister(new Meditation(-3), "it_meditation_caldera");
        BootRegister(new Meditation(-2), "it_meditation_hollow");
        BootRegister(new Meditation(-1), "it_meditation_dent");
        BootRegister(new Meditation(1), "it_meditation_bump");
        BootRegister(new Meditation(2), "it_meditation_hill");
        BootRegister(new Meditation(3), "it_meditation_volcano");
    BOOT_REGISTER_END

} // namespace enigma
