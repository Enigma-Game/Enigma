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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id: floors.cc,v 1.12 2004/05/22 13:04:31 dheck Exp $
 */

#include "player.hh"
#include "server.hh"
#include "world.hh"
#include "main.hh"

#include <iostream>
#include <cassert>

using namespace std;
using namespace enigma;
using namespace world;

Floor::Floor(const char *kind, double friction_, double mfactor, FloorFlags flags)
: GridObject (kind),
  traits (kind, friction_, mfactor, flags)
{}

Floor::Floor (const FloorTraits &tr)
: GridObject (tr.name.c_str()),
  traits (tr)
{}

Floor *Floor::clone() {
    return this;
}

void Floor::dispose() {
    ; // do nothing
}

void Floor::message(const string& /*msg*/, const Value &/*val*/) {
}

ecl::V2 Floor::process_mouseforce (Actor *a, ecl::V2 force) {
    if (a->controlled_by(player::CurrentPlayer()))
        return mousefactor() * force;
    else
        return ecl::V2();
}

void Floor::get_sink_speed (double &sinkspeed, double &raisespeed) const {
//     sinkspeed = raisespeed = 0.0;
}


 double Floor::friction() const { return traits.friction; }
 double Floor::mousefactor() const { return traits.mousefactor; }

 bool Floor::is_destructible() const {return true;}

 void Floor::set_model (const std::string &mname) {
     display::SetModel (GridLoc(GRID_FLOOR, get_pos()), mname);
 }

 display::Model *Floor::get_model () {
     return display::GetModel (GridLoc(GRID_FLOOR, get_pos()));
 }

 void Floor::kill_model (GridPos p) {
     display::KillModel (GridLoc (GRID_FLOOR, p));
 }

namespace
{

/* -------------------- Abyss -------------------- */

    class Abyss : public Floor {
        CLONEOBJ(Abyss);
    public:
        Abyss() : Floor("fl-abyss", 2.0, 1, flf_indestructible) {}
    private:
//         void actor_enter(Actor* a) {SendMessage(a, "fall");}
        void actor_contact (Actor* a) {SendMessage(a, "fall");}
	bool is_destructible() const {return false;}
    };

/* -------------------- Ice -------------------- */

    class Ice : public Floor {
        CLONEOBJ (Ice);
    public:
        Ice() : Floor ("fl-ice", 0.1, 0.1) {
        }

        virtual double friction() const {
            return 0.1 * server::IceFriction;
        }
    };

/* -------------------- Water -------------------- */

    class Water : public Floor {
        CLONEOBJ(Water);
    public:
        Water() : Floor("fl-water", 5, 1, flf_indestructible) {}
    private:

	bool is_destructible() const {return false;}

        void get_sink_speed (double &sink_speed, double &raise_speed) const { 
            sink_speed = server::WaterSinkSpeed;
            raise_speed = 1000; // always sink in water
        }

    };

/* -------------------- Swamp -------------------- */

    class Swamp : public Floor {
        CLONEOBJ(Swamp);
    public:
        Swamp() : Floor("fl-swamp", 13, 1.0, flf_indestructible) {}
    private:
	bool is_destructible() const {return false;}
        
        void get_sink_speed (double &sink_speed, double &raise_speed) const { 
            sink_speed = server::SwampSinkSpeed;
            raise_speed = 6.0;
        }
    };

/* -------------------- Space with force -------------------- */

    class SpaceForce : public Floor {
        CLONEOBJ (SpaceForce);

        // Floor interface
        void add_force (Actor *, V2 &f) {
            f[1] -= server::FlatForce;
        }

    public:
        SpaceForce() : Floor("fl-space-force", 0, 0) {
        }
    };

/* -------------------- FallenBox -------------------- */

    class FallenBox : public Floor {
        CLONEOBJ(FallenBox);
    public:
        FallenBox(const char *kind)
        :  Floor(kind, 6.4, 2.0) // uses same traits as fl-wood
        {}

    private:
        const char *modify_kind(const char *kind) {
            if (0 == strcmp(kind, "fl-stwood")) {
                return enigma::IntegerRand(0, 1) ? "fl-stwood1" :  "fl-stwood2";
            }
            return kind;
        }
    };

/* -------------------- DummyFloor -------------------- */

    class DummyFloor : public Floor {
        CLONEOBJ(DummyFloor);
    public:
        DummyFloor() : Floor("fl-dummy", 4.0, 2.5) {}
    private:
        void actor_enter(Actor *) {
            static int lastCode = -1;
            int        code     = int_attrib("code");
            if (lastCode != code) {
                fprintf(stderr, "Entering floor 0x%x\n", code);
                lastCode = code;
            }
        }
    };
}


/* -------------------- Gradient -------------------- */

/** \page fl-gradient Gradient Floor

This is a sloped floor that accelerates in a particular direction.

\subsection gradienta Attributes

- \b type: number between 0 and 15 (see below)

\subsection gradiente Examples

*/
namespace
{
    class Gradient : public Floor {
        CLONEOBJ(Gradient);
        int type;
        bool use_forcefac;
        double forcefac;
    public:
        Gradient(int type=MINTYPE);
    private:
        int get_type() const;
        enum { MINTYPE=1, MAXTYPE=24 };

        // Object interface
        virtual void set_attrib (const string& key, const Value &val);

        // GridObject interface.
        virtual void init_model();

        // Floor interface
        virtual void add_force(Actor *a, V2 &f);
    };
}

Gradient::Gradient(int type_)
: Floor("fl-gradient", 4, 2),
  type (type_),
  use_forcefac (false),
  forcefac (1.0)
{
    set_attrib ("type", Value(type_));
//    set_attrib ("force", Value(1.0));
}

void Gradient::set_attrib (const string& key, const Value &val)
{
    if (key == "type") {
        int t=to_int (val);
        if (t < MINTYPE || t>MAXTYPE) {
            enigma::Log << "Gradient: Illegal type="<< int(t) << std::endl;
            t=MINTYPE;
        }
        type = t;
    }
    else if (key == "force") {
        use_forcefac = true;
        forcefac = to_double (val);
    }
    Object::set_attrib (key, val);
}


int Gradient::get_type() const
{
    return type;
}

void Gradient::init_model()
{
    set_model(ecl::strf("fl-gradient%d", get_type()));
}

void Gradient::add_force(Actor */*a*/, V2 &f)
{
    ecl::V2 force;
    int t = get_type();

    static int xforce[MAXTYPE-MINTYPE+1] = {
         0,  0,  1, -1,
         1, -1,  1, -1,
         1,  1, -1, -1,
        -1, -1,  1,  1,
         1, -1,  1, -1,
         0,  0,  1, -1
    };
    static int yforce[MAXTYPE-MINTYPE+1] = {
         1, -1,  0,  0,
         1,  1, -1, -1,
         1, -1,  1, -1,
         1, -1,  1, -1,
        -1, -1,  1,  1,
         1, -1,  0,  0
    };
    force = ecl::V2(xforce[t-MINTYPE], yforce[t-MINTYPE]);
    force.normalize();

    // use enigma::SlopeForce if no "force" attribute is set
    double factor = server::SlopeForce;
    if (use_forcefac)
        factor = forcefac;
    force *= factor;
    f += force;
}


/* -------------------- Bridge -------------------- */

/** \page fl-bridge Bridge Floor

This Floor can be opened and closed much like a bridge.
The actor can move over this floor if the bridge is closed,
and will fall into abyss when the bridge is open.

\subsection bridgea Attributes

- \b type	The type of the bridge, currently only 'a' is possible

\subsection bridgem Messages
- \b open	open the bridge so actors cannot pass it
- \b close	close the bridge so actors can pass it
- \b openclose	toggle the state of the bridge
- \b signal     same as \b openclose

*/
namespace
{
    class Bridge : public Floor {
        CLONEOBJ(Bridge);
    public:
        Bridge(bool open=true);
    private:
        enum State {
            OPEN, CLOSED, OPENING, CLOSING, // normal states
            CLOSING_BYSTONE, CLOSED_BYSTONE // when stones are moved onto the bridge
        } state;
        // the BYSTONE-states look like closed, but act like open

        char get_type() const {
            string type = "a";
            string_attrib("type", &type);
            return type[0];
        }

//         void actor_enter(Actor *);
        void actor_contact (Actor* a) {if (state!=CLOSED) SendMessage(a, "fall");}
        void message(const string &m, const Value &);
        void init_model();
        void stone_change(Stone *st);

        void change_state( State newstate);
        void animcb();
    };
}

Bridge::Bridge(bool open) : Floor("fl-bridge", 5, 1)
{
    set_attrib("type", "a");
    state=open ? OPEN : CLOSED;
}

void Bridge::stone_change(Stone *st) {
    if (st && !st->is_floating()) {
        if (state == OPEN || state == OPENING) {
            change_state(CLOSING_BYSTONE);
        }
    }
    else {
        if (state == CLOSED_BYSTONE || state == CLOSING_BYSTONE) {
            change_state(OPENING);
        }
    }
}

void Bridge::message(const string &m, const Value &)
{
    if (m == "open" && (state==CLOSED || state==CLOSING))
        change_state(OPENING);
    else if (m=="close")
        switch (state) {
        case OPEN:
        case OPENING:
        case CLOSING_BYSTONE:
            change_state(CLOSING);
            break;
        case CLOSED_BYSTONE:
            change_state(CLOSED);
            break;
        case CLOSED:
        case CLOSING:
            break; // already closed

        }
    else if (m=="openclose" || m=="signal")
        switch (state) {
        case OPEN:
        case OPENING:
        case CLOSING_BYSTONE:
            change_state(CLOSING);
            break;
        case CLOSED_BYSTONE:
            change_state(CLOSED);
            break;
        case CLOSED:
        case CLOSING:
            change_state(OPENING);
            break;
        }
}

void Bridge::init_model()
{
    set_model(ecl::strf("fl-bridge%c-%s", get_type(),
                       (state==OPEN) ? "open" : "closed"));
}

void Bridge::change_state( State newstate)
{
    if (state != newstate) {
        string mname = string("fl-bridge")+get_type();

        switch( newstate) {
        case OPENING: {
            Stone *st = GetStone(get_pos());
            if (st && !st->is_floating()) {
                if (state == CLOSED || state == CLOSED_BYSTONE)
                    newstate = CLOSED_BYSTONE;
                else if (state == CLOSING || state == CLOSING_BYSTONE)
                    newstate = CLOSING_BYSTONE;
                // here the model is already correct!
            }
            else { // no stone or floating stone :
                if( state == CLOSING || state == CLOSING_BYSTONE)
                    get_model()->reverse();
                else
                    set_anim(mname+"-opening");
            }
            break;
        }
        case CLOSING:
        case CLOSING_BYSTONE:
            if( state == OPENING)
                get_model()->reverse();
            else if (state != CLOSING && state != CLOSING_BYSTONE)
                set_anim(mname+"-closing");
            break;
        case OPEN:
        case CLOSED:
        case CLOSED_BYSTONE:
            state = newstate;
            init_model();
            break;
        }
        state = newstate;
    }
}

void Bridge::animcb()
{
    switch (state) {
        case OPENING: change_state(OPEN); break;
        case CLOSING: change_state(CLOSED); break;
        case CLOSING_BYSTONE: change_state(CLOSED_BYSTONE); break;
        default : assert(0); break;
    }
}


//----------------------------------------
// Black and white tiles
//----------------------------------------
namespace
{
    class BlackTile : public Floor {
    public:
        BlackTile() : Floor ("fl-acblack", 5.2, 2.0) {}

        ecl::V2 process_mouseforce (Actor */*a*/, ecl::V2 force) {
            if (player::CurrentPlayer() == 0)
                return mousefactor() * force;
            else
                return ecl::V2();
        }
    };

    class WhiteTile : public Floor {
    public:
        WhiteTile() : Floor ("fl-acwhite", 5.2, 2.0) {}

        ecl::V2 process_mouseforce (Actor */*a*/, ecl::V2 force) {
            if (player::CurrentPlayer() == 1)
                return mousefactor() * force;
            else
                return ecl::V2();
        }
    };
}


void world::InitFloors()
{
    // Floors (most floors are defined in init.lua)
    Register(new Abyss);
    Register(new Ice);
    Register(new Water);
    Register(new Swamp);
    Register(new DummyFloor);
    Register(new FallenBox("fl-stwood"));
    Register(new FallenBox("fl-stwood1"));
    Register(new FallenBox("fl-stwood2"));
    Register(new Bridge);
    Register("fl-bridge-open", new Bridge(true));
    Register("fl-bridge-closed", new Bridge(false));
    Register(new WhiteTile);
    Register(new BlackTile);
    Register(new SpaceForce);

    Register(new Gradient);
    Register("fl-gradient1", new Gradient(1));
    Register("fl-gradient2", new Gradient(2));
    Register("fl-gradient3", new Gradient(3));
    Register("fl-gradient4", new Gradient(4));
    Register("fl-gradient5", new Gradient(5));
    Register("fl-gradient6", new Gradient(6));
    Register("fl-gradient7", new Gradient(7));
    Register("fl-gradient8", new Gradient(8));
    Register("fl-gradient9", new Gradient(9));
    Register("fl-gradient10", new Gradient(10));
    Register("fl-gradient11", new Gradient(11));
    Register("fl-gradient12", new Gradient(12));
    Register("fl-gradient13", new Gradient(22));
    Register("fl-gradient14", new Gradient(21));
    Register("fl-gradient15", new Gradient(24));
    Register("fl-gradient16", new Gradient(23));
}
