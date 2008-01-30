/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
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
#include "laser.hh"
#include "player.hh"
#include "server.hh"
#include "Inventory.hh"
#include "stones_internal.hh"

using namespace std;

namespace enigma {


/* -------------------- Laser / Time switches -------------------- */

namespace
{
    class LaserTimeSwitchBase : public PhotoStone, public TimeHandler {
    public:
        LaserTimeSwitchBase(const char *kind);
        virtual ~LaserTimeSwitchBase();

    private:
        // LaserTimeSwitchBase interface
        virtual const char *get_active_model() const   = 0;
        virtual const char *get_inactive_model() const = 0;
        virtual double timer_delay() const;

        // Stone interface
        void on_creation (GridPos p);
        void on_removal (GridPos p);
        const char *collision_sound() { return "metal"; }

        // PhotoStone interface.
        void notify_laseron();
        void notify_laseroff();

        // TimeHandler interface
        void alarm();

    protected:
        // variables :
        enum State { IDLE, LIGHTED, TOUCHED };
        State state;

        void change_state(State newstate);
    };


    class LaserSwitch : public LaserTimeSwitchBase {
        CLONEOBJ(LaserSwitch);
    public:
        LaserSwitch();
        virtual Value message(const Message &m);
    private:
        // LaserTimeSwitchBase interface
        const char *get_active_model() const;
        const char *get_inactive_model() const;
    };


    class LaserTimeSwitch : public LaserTimeSwitchBase {
        CLONEOBJ(LaserTimeSwitch);
    public:
        LaserTimeSwitch(const char *kind = "st-lasertimeswitch");
    private:
        // LaserTimeSwitchBase interface
        const char *get_active_model() const;
        const char *get_inactive_model() const;
        double timer_delay() const;

        // Stone interface
        void actor_hit(const StoneContact &sc);
    };

    class TimeSwitch : public LaserTimeSwitch {
        CLONEOBJ(TimeSwitch);
    public:
        TimeSwitch();
    private:
        // ignore laser:
        void notify_laseron();
        void notify_laseroff();
    };
}


LaserTimeSwitchBase::LaserTimeSwitchBase(const char *kind)
: PhotoStone(kind) , state(IDLE) 
{}

LaserTimeSwitchBase::~LaserTimeSwitchBase() {
    GameTimer.remove_alarm (this);
}

void LaserTimeSwitchBase::change_state(State newstate) {
    if (state == newstate)
        return;

    if (state == IDLE) {
//         sound_event ("st-switch");
        set_model(get_active_model());
        performAction(true);
        if (newstate == TOUCHED) {
            double delay = timer_delay();
            ASSERT(delay>0.0, XLevelRuntime, "LaserTimeSwitchBase: delay non-positive");
            GameTimer.set_alarm(this, delay, false);
        }
    }
    else if (newstate == IDLE) {
//         sound_event ("st-switch");
        set_model(get_inactive_model());
        performAction(false);
    }
    else {
        // it's not allowed to switch from LIGHTED to TOUCHED
       ASSERT(!(state == LIGHTED && newstate == TOUCHED), XLevelRuntime,
           "LaserTimeSwitchBase: trying to switch from lighted to touched");
    }
    state = newstate;
}

void LaserTimeSwitchBase::on_creation(GridPos) 
{
    set_model(get_inactive_model());
    photo_activate();
}

void LaserTimeSwitchBase::on_removal(GridPos)
{
    photo_deactivate();
}

void LaserTimeSwitchBase::notify_laseron()  {
    if (state != LIGHTED)
        change_state(LIGHTED);
}

void LaserTimeSwitchBase::notify_laseroff() {
    if (state == LIGHTED)
        change_state(IDLE);
}

void LaserTimeSwitchBase::alarm() {
    if (state == TOUCHED)
        change_state(IDLE);
}

double LaserTimeSwitchBase::timer_delay() const {
    return -1; // we have no timer delay
}

/* ---------- LaserSwitch ---------- */

LaserSwitch::LaserSwitch()
: LaserTimeSwitchBase("st-laserswitch")
{}

Value LaserSwitch::message(const Message &m) {
    if (m.message == "_model_reanimated") {
        RecalcLight(); // we may be swapped out of a laser beam
    }
    return LaserTimeSwitchBase::message(m);
}

const char *LaserSwitch::get_active_model() const { 
    return "st-laserswitch1"; 
}

const char *LaserSwitch::get_inactive_model() const { 
    return "st-laserswitch0"; 
}

/* ---------- LaserTimeSwitch ---------- */

LaserTimeSwitch::LaserTimeSwitch(const char *kind)
    : LaserTimeSwitchBase(kind)
{
    set_attrib("delay", 1.8);
}

const char *LaserTimeSwitch::get_active_model() const { 
    return "st-time1switch"; 
}

const char *LaserTimeSwitch::get_inactive_model() const { 
    return "st-timeswitch"; 
}

double LaserTimeSwitch::timer_delay() const {
    if (Value v = getAttr("delay"))
        return v;
    else
        ASSERT(0, XLevelRuntime, "LaserTimeSwitch: delay not properly defined");
}

void LaserTimeSwitch::actor_hit(const StoneContact &sc) {
    if (sc.actor && state == IDLE)
        change_state(TOUCHED);
}

/* ---------- TimeSwitch ---------- */

// Attributes:
//
// :on              1 or 0
// :target,action   as usual

TimeSwitch::TimeSwitch()
: LaserTimeSwitch("st-timeswitch")
{
}

void TimeSwitch::notify_laseron()  {} // ignore laser
void TimeSwitch::notify_laseroff() {}

/* -------------------- Functions -------------------- */

void InitSwitches()
{
    Register (new LaserSwitch);
    Register (new LaserTimeSwitch);
    Register (new TimeSwitch);
}

} // namespace enigma
