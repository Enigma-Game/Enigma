/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2008,2009,2010 Ronald Lamprecht
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

#include "actors/Balls.hh"
#include "errors.hh"
#include "world.hh"
#include "main.hh"

namespace enigma {

/* -------------------- BasicBall  -------------------- */

    const double BasicBall::SHIELD_TIME = 10.0;

    BasicBall::BasicBall(const ActorTraits &tr) : Actor (tr),
            sinkDepth (minSinkDepth), sinkModel (-1),
            lastshinep (false), vortex_normal_time (0), m_halosprite (),
            m_shield_rest_time (0), m_halostate (NOHALO),
            m_drunk_rest_time (0), m_invisible_rest_time (0) {
        state = NO_STATE;
    }

    Value BasicBall::message(const Message &m) {   // shatter, suicide, fall, jump, booze, shield are candidates for public usage
        bool handled = false;
        switch (state) {
            case NORMAL:
                if (m.message == "_shatter") {
                    change_state_noshield(SHATTERING);
                    handled = true;
                } else if (m.message == "_suicide") {
                    inplaceRespawn = true;
                    change_state(SHATTERING);
                    handled = true;
                } else if (m.message == "_laserhit") {
                    if(getAttr("color") != GLASS) {
                        centerRespawn = false;
                        change_state_noshield(SHATTERING);
                    }
                    handled = true;
                } else if (m.message == "_fall") {
                    change_state_noshield(FALLING);
                    handled = true;
                } else if (m.message == "_fallvortex") {
                    change_state(FALLING_VORTEX);
                    handled = true;
                } else if (m.message == "_jump") {
                    change_state(JUMPING);
                    handled = true;
                } else if (m.message == "_appear") {
                    change_state(APPEARING);
                    handled = true;
                } else if (m.message == "_disappear") {
                    change_state(DISAPPEARING);
                    handled = true;
                } else if (m.message == "_setcolor") {
                    if (   ((get_id (this) == ac_pearl_black) && (m.value == GLASS))
                        || ((get_id (this) == ac_pearl_white) && (m.value == GLASS))) 
                        throw XLevelRuntime("ac_pearl_glass not yet defined (will come soon).");
                    setAttr("color", m.value);
                    update_model();
                    return true;
                }
                break;
            case JUMPING:
                if (m.message == "_shatter") {
                    change_state_noshield(SHATTERING);
                    handled = true;
                } else if (m.message == "_disappear") {
                    change_state(DISAPPEARING);
                    handled = true;
                }
                break;
            case SHATTERING:
                if (m.message == "_levelfinish") {
                    change_state(DEAD);
                    handled = true;
                }
                break;
            case DEAD:
                if (m.message == "_resurrect") {
                    change_state(RESURRECTED);
                    handled = true;
                }
                break;
            case FALLING_VORTEX:
                if (m.message == "_rise") {
                    change_state(RISING_VORTEX); // vortex->vortex teleportation
                } else if (m.message == "_appear") {
                    change_state(APPEARING); // vortex->non-vortex teleportation
                    handled = true;
                }
                break;
            case JUMP_VORTEX:
                if (m.message == "_laserhit") {
                    change_state(SHATTERING);
                    handled = true;
                }
                break;
            case APPEARING:
                // ugly hack
                if (m.message == "_init") {
                    Actor::message (m);
                    handled = true;
                } else if (m.message == "_shatter") {
                    change_state (SHATTERING);
                    handled = true;
                }
                break;
            default:
                break;
        }

        // Shield, booze and invisibility can be activated in all states except DEAD

        if (state != DEAD) {
            if (m.message == "_shield" && getAttr("owner").getType() != Value::DEFAULT &&  getAttr("owner") ==  (int)m.value) {
                m_shield_rest_time += SHIELD_TIME;
                update_halo();
                return true;
            }
            else if (m.message == "_invisibility") {
                m_invisible_rest_time += 8.0;
                return true;
            }
            else if (m.message == "_booze") {
                m_drunk_rest_time += (int)m.value; // Drunken for 5 more seconds
                return true;
            }
        }

        return handled ? Value() : Actor::message(m);
    }


    int BasicBall::externalState() const {
        return state == DEAD ? 1 : 0;
    }

    void BasicBall::setState(int extState) {
        // no change possible
    }

    bool BasicBall::is_dead() const {
        return state == DEAD;
    }

    bool BasicBall::isMoribund() const {
        return state == DEAD || state == SHATTERING || state==BUBBLING || state==FALLING || state==RESURRECTED;
    }

    bool BasicBall::is_movable() const {
        return (state!=DEAD && state!=RESURRECTED && state!=APPEARING && state!=DISAPPEARING);
    }

    bool BasicBall::is_flying() const {
        return state == JUMPING;
    }

    bool BasicBall::is_on_floor() const {
        return state == NORMAL || state == JUMP_VORTEX || state==APPEARING;
    }

    bool BasicBall::is_drunken() const {
        return m_drunk_rest_time>0;
    }

    bool BasicBall::is_invisible() const {
        return m_invisible_rest_time>0;
    }

    bool BasicBall::can_drop_items() const {
        return state == NORMAL || state == JUMP_VORTEX || state==JUMPING;
    }

    bool BasicBall::can_pickup_items() const {
        return state == NORMAL || state == JUMP_VORTEX;
    }

    bool BasicBall::can_be_warped() const {
        return state==NORMAL;
    }

    bool BasicBall::has_shield() const {
        return m_shield_rest_time > 0;
    }

    bool BasicBall::on_collision(Actor *a) {
        if (getAttr("color") == GLASS && !is_flying() && !a->is_flying()) {
            ecl::V2 dv = get_vel() - a->get_vel();
            Log << dv*dv;
            Log << "\n";
            if (dv*dv > 16)
                change_state_noshield(SHATTERING);
        }
        return Actor::on_collision(a);
    }

    void BasicBall::on_creation(const ecl::V2 &p) {
        bool recreation = m_actorinfo.created;
        Actor::on_creation(p);
        if (server::CreatingPreview) {
            change_state(NORMAL);
        } else if (recreation) {   // end of it_drop
            change_state(NORMAL);  // marble may be in any state
            ReleaseActor(this);    // in case it has been already NORMAL
        } else {
            change_state(APPEARING);
        }
    }

    void BasicBall::think(double dtime)  {
        if (m_invisible_rest_time > 0)
            m_invisible_rest_time -= dtime;

        // Update protection shield
        if (m_shield_rest_time > 0)
            m_shield_rest_time -= dtime;

        switch (state) {
            case NORMAL:
                if (m_drunk_rest_time > 0)
                    m_drunk_rest_time -= dtime;
                sink (dtime);
                break;
            case JUMP_VORTEX:
                vortex_normal_time += dtime;
                if (vortex_normal_time > 0.025) // same time as appear animation
                    if (vortex_normal_time > dtime) // ensure min. one tick in state JUMP_VORTEX!
                        change_state(JUMPING); // end of short control over actor
                break;
            default:
                break;
        }

        Actor::think(dtime);
    }

    void BasicBall::on_respawn (const ecl::V2 &pos) {
            Actor::on_respawn(pos);
            change_state(APPEARING);
            // notify objects on grid about the death - used by it_trigger, sensor, meditation
            SendMessage(GetFloor(get_gridpos()), "_dying", false, this);
            SendMessage(GetItem(get_gridpos()), "_dying", false, this);
            SendMessage(GetStone(get_gridpos()), "_dying", false, this);
    }

    void BasicBall::move_screen() {
        update_model();
        update_halo();
        Actor::move_screen();
    }

    void BasicBall::hide() {
        Actor::hide();
        disable_shield();
    }

    void BasicBall::animcb() {
        std::string kind = getModelBaseName();

        switch (state) {
            case SHATTERING:
                set_model(kind+"-shattered");
                change_state(DEAD);
                break;
            case BUBBLING:
                set_model("invisible");
                change_state(DEAD);
                break;
            case FALLING:
                set_model(kind+"-fallen"); // invisible
                if (get_id (this) == ac_pearl_white)
                    sound_event("shattersmall");
                else
                    sound_event("shatter");
                change_state(DEAD);
                break;
            case JUMPING:
                set_model(kind);
                change_state(NORMAL);
                break;
            case APPEARING:
                set_model(kind);
                change_state(NORMAL);
                break;
            case DISAPPEARING:
                set_model("ring-anim");
                break;
            case FALLING_VORTEX: {
                set_model(kind+"-fallen"); // invisible
                break;
            }
            case RISING_VORTEX: {
                set_model(kind);
                if (Item *it = GetItem(get_gridpos())) {
                    ItemID theid = get_id(it);
                    if (theid == it_vortex_open || theid == it_vortex_closed)
                        SendMessage(it, "_passed"); // closes some vortex
                }
                change_state(JUMP_VORTEX);
                break;
            }
            default:
                break;
        }
    }

    std::string BasicBall::getModelBaseName() const {
        return getKind();
    }

    void BasicBall::sink(double dtime) {
        double sink_speed  = 0.0;
        double raise_speed = 0.0;   // at this velocity don't sink; above: raise

        Floor *fl = m_actorinfo.field->floor;
        Item *it = m_actorinfo.field->item;
        if (!(it != NULL && it->covers_floor(get_pos(), this)) && fl != NULL)
            fl->get_sink_speed (sink_speed, raise_speed);

        if (sink_speed == 0.0 || has_shield() || getAttr("color") == GLASS) {
            sinkDepth = minSinkDepth;
            sinkModel = -1;
        }
        else {
            ActorInfo *ai = get_actorinfo();
            double sinkSpeed = sink_speed * (1 - length(ai->vel) / raise_speed);
            sinkDepth += sinkSpeed*dtime;

            if (sinkDepth >= maxSinkDepth) {
                set_model(getModelBaseName() + "-sunk");
                ai->vel = ecl::V2();     // stop!
                sound_event ("swamp");
                change_state(BUBBLING);
            }
            else {
                if (sinkDepth < minSinkDepth)
                    sinkDepth = minSinkDepth;
            }
        }
    }

    void BasicBall::disable_shield() {
        if (has_shield()) {
            m_shield_rest_time = 0;
            update_halo();
        }
    }

    void BasicBall::change_state_noshield(iState newstate) {
        if (!has_shield())
            change_state(newstate);
    }

    void BasicBall::change_state(iState newstate) {
        if (newstate == state)
            return;

        std::string kind = getModelBaseName();
        iState oldstate = (iState)state;
        state = newstate;

        if (oldstate == JUMPING) {
            // notify objects on grid about the landing - used by it_trigger
            SendMessage(GetStone(get_gridpos()), "_jumping", false);
            SendMessage(GetItem(get_gridpos()), "_jumping", false);
            SendMessage(GetFloor(get_gridpos()), "_jumping", false);
        }

        // Whatever happened to the ball, the sink depth
        // should be returned to zero.
        sinkDepth = minSinkDepth;
        sinkModel = -1;

        switch (newstate) {
            case NORMAL:
                if (oldstate == APPEARING) {
                    ActorInfo *ai = get_actorinfo();
                    ai->forceacc = ecl::V2();
                }
                ReleaseActor(this);
                break;

            case SHATTERING:
                if (get_id (this) == ac_marble_white)
                    sound_event("shattersmall");
                else
                    sound_event("shatter");
                GrabActor(this);
                set_anim(kind+"-shatter");
                break;

            case BUBBLING:
                GrabActor(this);
        //         sound::PlaySound("drown");
                m_invisible_rest_time = 0;
                set_anim("ac-drowned");
                break;
            case FALLING:
            case FALLING_VORTEX:
                GrabActor(this);
                set_anim(kind+"-fall");
                break;
            case DEAD:
                disable_shield();
                m_drunk_rest_time = 0;
                m_invisible_rest_time = 0;
                // notify objects on grid about the death - used by it_trigger, sensor, meditation
                SendMessage(GetFloor(get_gridpos()), "_dying", true, this);
                SendMessage(GetItem(get_gridpos()), "_dying", true, this);
                SendMessage(GetStone(get_gridpos()), "_dying", true, this);
                break;
            case JUMPING:
                sound_event("jump");
                set_anim(kind+"-jump");
                // notify objects on grid about the jumping - used by it_trigger
                SendMessage(GetFloor(get_gridpos()), "_jumping", true);
                SendMessage(GetItem(get_gridpos()), "_jumping", true);
                SendMessage(GetStone(get_gridpos()), "_jumping", true);
                break;
            case APPEARING:
            case RISING_VORTEX:
                set_anim(kind+"-appear");
                GrabActor(this);
                break;
            case JUMP_VORTEX:
                ASSERT(oldstate == RISING_VORTEX, XLevelRuntime,
                    "BasicBall: change to state JUMP_VORTEX but not RISING_VORTEX");
                vortex_normal_time = 0;
                set_model(kind);
                ReleaseActor(this);
                break;
            case DISAPPEARING:
                GrabActor(this);
                disable_shield();
                set_anim(kind+"-disappear");
                break;
            case RESURRECTED:
                disable_shield();
                sinkDepth = minSinkDepth;
            break;
            default:
                break;
        }
    }

    void BasicBall::update_model() {
        if (m_invisible_rest_time > 0)
            get_sprite().hide();
        else
            get_sprite().show();

        switch (state) {
            case NORMAL:
                if (sinkDepth > minSinkDepth && sinkDepth < maxSinkDepth) {
                    set_sink_model(getModelBaseName());
                }
                else {
                    ActorInfo *ai = get_actorinfo();
                    int xpos = ecl::round_nearest<int> (ai->pos[0] * 32.0);
                    int ypos = ecl::round_nearest<int> (ai->pos[1] * 32.0);

                    bool shinep = ((xpos + ypos) % 2) != 0;
                    set_shine_model (shinep);
                }
                break;
            default:
                break;
        }
    }

    void BasicBall::set_sink_model(const std::string &m) {
        int modelnum = ecl::round_down<int>(sinkDepth);

        if (!has_shield() && modelnum != sinkModel) {
            ASSERT(modelnum >= minSinkDepth && modelnum < maxSinkDepth, XLevelRuntime,
                "BasicBall: set_sink_model called though modelnum incorrect");

            std::string img = m+"-sink";
            img.append(1, static_cast<char>('0'+modelnum));
            set_model(img);

            sinkModel = modelnum;
        }
    }

    void BasicBall::set_shine_model (bool shinep)
    {
        if (shinep != lastshinep) {
            std::string modelname = getModelBaseName();
            if (shinep)
                set_model (modelname + "-shine");
            else
                set_model (modelname);
            lastshinep = shinep;
        }
    }

    void BasicBall::update_halo() {
        HaloState newstate;

        if (m_shield_rest_time <= 0)
            newstate = NOHALO;
        else if (m_shield_rest_time <= 3.0)
            newstate = HALOBLINK;
        else
            newstate = HALONORMAL;

        if (newstate != m_halostate) {
            double radius = get_actorinfo()->radius;
            std::string halokind;

            // Determine which halomodel has to be used:
            if (radius == 19.0/64) { // Halo for normal balls
                halokind = "halo";
            } else if (radius == 13.0f/64) { // Halo for small balls
                halokind = "halo-small";
            }

            if (m_halostate == NOHALO){
                m_halosprite = display::AddSprite (get_pos(), halokind.c_str());
            }
            switch (newstate) {
            case NOHALO:
                // remove halo
                m_halosprite.kill();
                m_halosprite = display::SpriteHandle();
                break;
            case HALOBLINK:
                // blink for the last 3 seconds
                m_halosprite.replace_model (display::MakeModel (halokind+"-blink"));
                break;
            case HALONORMAL:
                m_halosprite.replace_model (display::MakeModel (halokind));
                break;
            }
            m_halostate = newstate;
        } else if (m_halostate != NOHALO) {
            m_halosprite.move (get_pos());
        }
    }

    void BasicBall::beforeStoneBounce(const StoneContact &sc) {
        if (getAttr("color") == GLASS) {
            if (GetPushDirection(sc) != NODIR) {
                change_state_noshield(SHATTERING);
                return;
            }
            if (sc.outerCorner && sc.is_contact) {
                ActorInfo *ai  = sc.actor->get_actorinfo();
                if (ai->vel * ai->vel > 3)
                    change_state_noshield(SHATTERING);
            }
        }
    }


/* -------------------- Marble  -------------------- */
    Marble::Marble(int color) : BasicBall(traits[color]) {
        setAttr("adhesion", 1.0);
        setAttr("color", color);
        setAttr("owner", color == WHITE ? YANG : YIN);
        setAttr("controllers", color == WHITE ? 2 : 1);
    }

    std::string Marble::getClass() const {
        return "ac_marble";
    }

    int Marble::traitsIdx() const {
        return getAttr("color");
    }

    ActorTraits Marble::traits[3] = {
        {"ac_marble_black", ac_marble_black, 1<<ac_marble_black, 19.0/64, 1.0},
        {"ac_marble_white", ac_marble_white, 1<<ac_marble_white, 19.0/64, 1.0},
        {"ac_marble_glass", ac_marble_glass, 1<<ac_marble_glass, 19.0/64, 0.5},
    };

/* -------------------- Pearl  -------------------- */
    Pearl::Pearl(int color) : BasicBall(traits[color]) {
        setAttr("adhesion", 1.0);
        setAttr("color", color);
        setAttr("owner", YIN);
        setAttr("controllers", 3);
    }

    std::string Pearl::getClass() const {
        return "ac_pearl";
    }

    void Pearl::sink(double dtime) {
        if (server::GameCompatibility != GAMET_ENIGMA) {
            if (m_actorinfo.field->floor->isKind("fl_swamp"))
                return;    // do not sink pearls in swamp in oxyd modes
        }
        BasicBall::sink(dtime);
    }

    int Pearl::traitsIdx() const {
        return getAttr("color");
    }

    ActorTraits Pearl::traits[2] = {
        {"ac_pearl_black", ac_pearl_black, 1<<ac_pearl_black, 13.0/64, 0.7},
        {"ac_pearl_white", ac_pearl_white, 1<<ac_pearl_white, 13.0/64, 0.7},
    };

    BOOT_REGISTER_START
        BootRegister(new Marble(BLACK), "ac_marble");
        BootRegister(new Marble(BLACK), "ac_marble_black");
        BootRegister(new Marble(WHITE), "ac_marble_white");
        BootRegister(new Marble(GLASS), "ac_marble_glass");
        BootRegister(new Pearl(WHITE), "ac_pearl");
        BootRegister(new Pearl(WHITE), "ac_pearl_white");
        BootRegister(new Pearl(BLACK), "ac_pearl_black");
    BOOT_REGISTER_END

} // namespace enigma
