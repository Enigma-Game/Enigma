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

#include "Inventory.hh"
#include "errors.hh"
#include "player.hh"
#include "server.hh"
#include "world.hh"
#include "main.hh"
#include "items/GlassesItem.hh"

#include <iostream>

using namespace std;

namespace enigma {

Floor::Floor(const char *kind, double friction_, double mfactor, FloorFlags flags,
             FloorFireType flft, const char *firetransform_, const char *heattransform_)
: GridObject (kind),
  traits (kind, flags, flft, firetransform_, heattransform_),
  heating_animation(false),
  fire_countdown(1),
  var_floorforce(),
  adhesion(mfactor),
  friction(friction_)
{}

Floor::Floor (const FloorTraits &tr)
: GridObject (tr.name.c_str()),
  traits (tr),
  heating_animation(false),
  fire_countdown(1)
{}

const char * Floor::get_kind() const {
    return traits.name.c_str();
}

Floor *Floor::clone() {
    return new Floor(*this);
}

void Floor::dispose() {
    delete this;
}

Value Floor::message(const Message &m) {
    // "_init"     : Start burning, if "initfire" is set.
    // "heat"     : Heat the item, heat-transform floor
    //                 or maybe set fire to it (if burnable).
    // "setfire"  : Just try to make fire (if burnable).
    // "forcefire": Force fire, even on unburnable floor.
    // "stopfire" : Stop fire, put ash but don't transform floor.
    if (m.message == "_init" && has_firetype(flft_initfire))
        return force_fire();
    if (m.message == "heat")
        return try_heating(NODIR, flhf_message);
    if ((m.message == "ignite" || m.message == "_explosion") && has_firetype(flft_ignitable))
        return try_ignite(NODIR, flhf_message);
    if (m.message == "setfire")
        return try_ignite(NODIR, flhf_message);
    if (m.message == "forcefire")
        return force_fire();
    if (m.message == "stopfire")
        return stop_fire(true);
    return GridObject::message(m);
}

ecl::V2 Floor::process_mouseforce (Actor *a, ecl::V2 force) {
    if (a->controlled_by(player::CurrentPlayer()))
        return getAdhesion() * force;
    else
        return ecl::V2();
}

    void Floor::setAttr(const string& key, const Value &val) {
        if (key == "adhesion")
            adhesion = val;
        else if (key == "friction")
            friction = val;
        else if (key == "force_x")
            var_floorforce[0] = val;
        else if (key == "force_y")
            var_floorforce[1] = val;
        GridObject::setAttr(key, val);
    }

    Value Floor::getAttr(const std::string &key) const {
        if (key == "adhesion") {
            return adhesion;
        } else if (key == "friction") {
            return friction;
        }
        return GridObject::getAttr(key);
    }
    
    void Floor::on_creation(GridPos p) {
        if (Value v = GridObject::getAttr("adhesion"))
            adhesion = v;
        if (Value v = GridObject::getAttr("friction"))
            friction = v;
        GridObject::on_creation(p);
    }

void Floor::get_sink_speed (double &sinkspeed, double &raisespeed) const {
//     sinkspeed = raisespeed = 0.0;
}

double Floor::get_friction() const 
{ 
    return friction; 
}

    double Floor::getAdhesion() const { 
        return adhesion; 
    }

bool Floor::is_destructible() const 
{
    return true;
}

bool Floor::is_freeze_check() const 
{
    return to_bool(this->getAttr("freeze_check"));
}

void Floor::set_model (const std::string &mname) 
{
    display::SetModel (GridLoc(GRID_FLOOR, get_pos()), mname);
}

display::Model *Floor::get_model () 
{
    return display::GetModel (GridLoc(GRID_FLOOR, get_pos()));
}

void Floor::kill_model (GridPos p) 
{
    display::KillModel (GridLoc (GRID_FLOOR, p));
}

void Floor::add_force (Actor *, V2 &f)
{
    // Note that actor == 0 is possible from lightpassenger-calculation.
    f += var_floorforce;
}

    void Floor::stone_change(Stone *st) {
        if (getDefaultedAttr("floodable", false).to_bool()) {
            SendMessage(GetFloor(get_pos()), "_checkflood");
            for (Direction d = NORTH; d != NODIR; d = previous(d))
                SendMessage(GetFloor(move(get_pos(), d)), "_checkflood");
        }
    }


/* -------------- Fire Handling -------------- */

/*  The fire system is a confusing tangle of bools and flags.
 *
 *  A short overview:
 *  A floor is allowed to catch fire when it is:
 *    - burnable by default
 *    - the "burnable"-attribute is set and in Enigma-comp.mode
 *    - it-burnable or it-burnable_oil is on it
 *  Fire on a burnable floor can be ignited by:
 *    - "setfire"-message
 *    - "_explosion" or "ignite" message if the "ignitable"-attribute is set
 *        - thereby by it-dynamite or it-*bomb + "ignitable"
 *    - fire in the neighborhood (see below)
 *    - on initialisation ("_init"-message) when "initfire"-attribute is set
 *  On the floor itself it does
 *    - inform stones on it -> e.g. kill st-wood, st-hay
 *    - keep on burning by chance or if eternal-attribute set
 *    - when stop burning
 *        - replace floor by floor given in floor-transform,
 *            or defaults in non-Enigma-compatibility mode.
 *        - put ash, if noash-attribute of *new* floor is not set
 *  When a floor tile is burning, it does the following to its neighbor:
 *    - ignite items by chance or if secure-attribute is set and
 *        source fire stops next time
 *    - start heat-transformation on first fire call but not yet
 *        transforming and only in Enigma-mode
 *    - inform neighboring stones about fire
 *    - In Enigma-mode (resp. non-Enigma-mode), spread to other burnable
 *        floors (resp. floors of the same kind) by chance or message
 *        or last+secure, if none of the first two methods worked, but
 *        delayed by firecountdown to simulate the old system, but not
 *        if message or last+secure or fastfire-attribute set: Then spread
 *        without countdown. Don't do any of these when there is a movable
 *        stone on it.
 *    - And don't do any of the former three if there is a stone on it,
 *        which is not floating (in non-Enigma-mode: neither movable).
 *  Have I forgotten something?
 */

std::string Floor::get_firetransform() {
    if (server::GameCompatibility == GAMET_ENIGMA) {
        std::string classname = getAttr("$firetransform").to_string();
        return (classname.length() > 0) ? classname : traits.firetransform;
    } else {
        // In non-Enigma-compatibility-modes, only fl-wood and fl-stwood
        // transform. Note: This might have been a bug, as fl-stwood1/2
        // and fl-wood1/2 were not included (at least fl-wood1/2 were of
        // a newer date). However, we include them here.
        return isKind("fl_wood") ? "fl_abyss" : "";
    }
}

std::string Floor::get_heattransform(bool override_mode) {
    // The bool is needed to correctly exit the heating-animation
    // in case the level switched to non-Enigma-mode in between.
    if (server::GameCompatibility == GAMET_ENIGMA || override_mode) {
        std::string classname = getAttr("$heattransform").to_string();
        return (classname.length() > 0) ? classname : traits.heattransform;
    } else {
        // In non-Enigma-compatibility-modes, there is no heat-transformation.
        return "";
    }
}
int Floor::get_fire_countdown() {
    if (Item *it = GetItem(get_pos()))
        if (get_id(it) == it_burnable || get_id(it) == it_burnable_oil)
            return 0;
    return fire_countdown;
}

bool Floor::force_fire() {
    SetItem(get_pos(), MakeItem("it-burnable_ignited"));
    fire_countdown = 0;
    return true;
}

bool Floor::try_ignite(Direction sourcedir, FloorHeatFlags flhf) {
    GridPos p = get_pos();

    // Don't disturb heating-transformation
    if (heating_animation)
        return false;

    // No or floating stone -> Burn items and replicate.
    // Movable stone && enigma-mode -> Burn items and replicate.
    // Movable stone && non-enigma-mode -> Only burn items.
    // Else -> Don't do anything.
    // Special case: "st-flrock": No fire at all!
    bool no_closing_stone = true;
    if (Stone *st = GetStone(p)) {
        if (st->is_movable())
            no_closing_stone = false;
        else if(!st->allowsSpreading(sourcedir))
            return false;
    }

    if (server::GameCompatibility == GAMET_ENIGMA) {
        if (has_firetype(flft_burnable)) {
            // has_firetype also checks whether floor is already burning or ignited
            // (via it-burnables), but not which stone is above it.
            if (Item *it = GetItem(p)) {
                // The item didn't respond to the "heat"-message, so we
                // could assume it's burnable. However, as there might also
                // be a "setfire"-message from the user or a lazy "message"-
                // implementation, we still have to check the burnable-flag
                // of this item:
                if (!has_flags(it, itf_fireproof))
                    return force_fire();
            } else {
                // Just spread. Use the fire-countdown to delay fire without
                // it-burnable, but not in case of a message or a secure+last
                // call or a fastfire-floor.
                if (   (get_fire_countdown() == 0) || (flhf == flhf_message)
                    || (has_firetype(flft_fastfire))
                    || (((bool) (flhf & flhf_last)) && has_firetype(flft_secure)))
                    return force_fire();            
                fire_countdown = max(fire_countdown - 1, 0);
            }        
        }
    } else {  // non-Enigma-mode
        if (has_firetype(flft_burnable) || flhf == flhf_message) {
            // We only get here when there is a burnable item or the
            // burnable attribute is set (e.g. as part of replication).
            // A fireproof item doesn't allow to get here. (Note: We
            // don't check a second time for no_closing_stone.)
            return force_fire();
        } else {
            // This floor doesn't burn by default or by item. But, in
            // non-Enigma-compatibility-mode it should, when its neighbor
            // is of the same kind and burns also.
            if (Floor *fl = GetFloor(move(p, sourcedir))) {
                string sourcekind = fl->get_kind();
                string mykind = this->get_kind();
                if (no_closing_stone && sourcekind == mykind)
                    if (has_firetype(flft_fastfire))
                        return force_fire();
                    else
                        this->setAttr("burnable", true);
            }
        }
    }
    return false;  // meaning: no fire
}

bool Floor::try_heating(Direction sourcedir, FloorHeatFlags flhf) {
    // First of all: How are we allowed to react at all?
    // There are four branches of heating:
    //
    //  1) item-transformation (e.g. igniting it-dynamite, *not* burning!)
    //  2) floor-heat-transformation (e.g. melting ice)
    //  3) stone-heat-transformation (e.g. fireblocker st-flrock, itemfreeze)
    //  4) fire
    //
    //  a) Always do (1),(2),(3),(4) if try_heating is called from
    //     a "heat"-message by the user/level.
    //  b) Always do (2) if this is the first call of a regular fire
    //     (checked in on_heattransform).
    //  c) Always do (1), (3) and (4) if this is the last call of a regular
    //     fire and the secure-attribute is set (so ignition is safe).
    //  d) Else, do (1), (3) and (4) with probability 0.7, and always both.
    //  e) However, if (1), (2) or (3) led to success, then never do (4),
    //     this way we prevent fire to disturb any transformations.
    //
    bool secure = ((bool) (flhf & flhf_last)) && has_firetype(flft_secure);
    bool doItem = (flhf == flhf_message) || secure || DoubleRand(0, 1) > 0.3;
    bool doStone = doItem;
    bool doIgnite = doItem;
    bool reaction_happened = false;
    // Heat item -> destroy cracks, ignite bombs...
    if (doItem)
        if (Item *it = GetItem(get_pos()))
            if (to_int(SendMessage(it, "heat", Value(sourcedir))) != 0.0)
                reaction_happened = true;        
    // Maybe also transform floor?
    reaction_happened = on_heattransform(sourcedir, flhf) || reaction_happened;
    // Maybe transform stone, or stone blocks fire?
    if (doStone)
        if (Stone *st = GetStone(get_pos()))
            if (SendMessage(st, "heat", Value(sourcedir)).to_bool())
                reaction_happened = true;
    // Not item nor floor nor stone reacted? Then try to ignite the floor!
    // (Note: try_ignite also tests for the heating animation:
    //        No fire during transformation allowed!)
    if (doIgnite && !reaction_happened)
        return this->try_ignite(sourcedir, flhf);
    // Else: return reaction_happened from item or heat-transform
    return reaction_happened;
}

bool Floor::on_heattransform(Direction sourcedir, FloorHeatFlags flhf) {
    // return true to forbid fire (message caught)
    bool doHeatTransform = (flhf == flhf_message) || ((bool) (flhf & flhf_first));
    if (!doHeatTransform || get_heattransform(false) == "")
        return false;
    if (doHeatTransform && get_heattransform(false) != "" && !heating_animation) {
        set_anim(((string) this->get_kind()) + "_heating");
        heating_animation = true;
    }
    return true;
}

void Floor::heat_neighbor(Direction dir, FloorHeatFlags flhf) {
    if (Floor *fl = GetFloor(move(get_pos(), dir))) {
        fl->try_heating(reverse(dir), flhf);
    }
}

bool Floor::stop_fire(bool is_message) {
    // stop burning
    //   -> kill burnable-item
    //   -> transform floor?
    //   -> put ash?  (depends on the new floor!)
    //   -> reset fire-countdown to 1
    GridPos p = get_pos();

    // is_message indicates use of the stopfire-message,
    // so we have to check if there is fire at all.
    if (is_message)
        if (Item *it = GetItem(p)) {
            ItemID id = get_id(it);
            if (id != it_burnable_burning && id != it_burnable_ignited)
                return false;  // no fire
        } else
            return false; // no item == no fire

    KillItem(p);
    fire_countdown = 1;
    if (!is_message && get_firetransform() != "")
        SetFloor(p, MakeFloor(get_firetransform().c_str()));
    // Remember, at this point "this" may be destroyed.
    if (!GetFloor(p)->has_firetype(flft_noash))
        SetItem(p, MakeItem("it-burnable_ash"));
    return true; // fire extinguished  
}

void Floor::on_burnable_animcb(bool justIgnited) {
    GridPos p = get_pos();
    // 
    SendMessage(GetStone(p), "fire");

    // Will we stop this time with burning?
    bool cont_fire = justIgnited || has_firetype(flft_eternal) || DoubleRand(0,1) < 0.7;
    FloorHeatFlags flhf = (FloorHeatFlags) (flhf_fire
        | (justIgnited ? flhf_first : flhf_fire) | (cont_fire ? flhf_fire : flhf_last));

    Stone *st = GetStone(p);   // stone can be killed due to fire message
    if (st == NULL || st->allowsSpreading(NORTH))
        heat_neighbor(NORTH, flhf);
    if (st == NULL || st->allowsSpreading(EAST))
        heat_neighbor(EAST,  flhf);
    if (st == NULL || st->allowsSpreading(SOUTH))
        heat_neighbor(SOUTH, flhf);
    if (st == NULL || st->allowsSpreading(WEST))
        heat_neighbor(WEST,  flhf);

    if (cont_fire)
        // continue burning
        //   -> put animation
        SetItem(p, MakeItem("it-burnable_burning"));
    else
        stop_fire(false);
}

bool Floor::has_firetype(FloorFireType selector) {
    if (Item *it = GetItem(get_pos())) {
        ItemID id = get_id(it);
        if (selector == flft_burnable || selector == flft_ignitable) {
            if (  id == it_burnable || id == it_burnable_oil )
                return true;
            if (  id == it_burnable_ash     || id == it_burnable_fireproof
               || id == it_burnable_ignited || id == it_burnable_burning )
                return false;
            // In non-Enigma-compatibility-modes, the item decides about
            // burnability and only it_burnable[_oil] is ignitable:
            if (server::GameCompatibility != GAMET_ENIGMA)
                return (selector == flft_burnable) && !has_flags(it, itf_fireproof);
        }
    }
    bool dflt = (server::GameCompatibility == GAMET_ENIGMA) 
            && (traits.firetype & selector);
    // In non-Enigma-modes, without items on them, all floors behave the same:
    switch (selector) {
        case flft_burnable :
            return getDefaultedAttr("burnable", dflt).to_bool();
        case flft_ignitable :
            return getDefaultedAttr("ignitable", dflt).to_bool();
        case flft_secure :
            return getDefaultedAttr("secure", dflt).to_bool();
        case flft_eternal :
            return getDefaultedAttr("eternal", dflt).to_bool();
        case flft_noash :
            return getDefaultedAttr("noash", dflt).to_bool();
        case flft_fastfire :
            return getDefaultedAttr("fastfire", dflt).to_bool();
        case flft_initfire :
            return getDefaultedAttr("initfire", dflt).to_bool();
    }
    return dflt;
}

void Floor::animcb() {
    // Probably the heating-animation ended.
    if (heating_animation) {
        heating_animation = false;
        if (this->get_heattransform(true) != "")
            SetFloor(get_pos(), MakeFloor(get_heattransform(true).c_str()));
    }
}

/* =============== Special Floors ================ */

namespace
{

/* -------------------- DummyFloor -------------------- */

    class DummyFloor : public Floor {
        CLONEOBJ(DummyFloor);
    public:
        DummyFloor() : Floor("fl-dummy", 4.0, 2.5) {}
    private:
        void actor_enter(Actor *) {
            static int lastCode = -1;
            int        code     = getAttr("code");
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
        virtual void setAttr(const string& key, const Value &val);

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
    setAttr("type", Value(type_));
}

void Gradient::setAttr(const string& key, const Value &val)
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
    Floor::setAttr(key, val);
}


int Gradient::get_type() const
{
    return type;
}

void Gradient::init_model()
{
    set_model(ecl::strf("fl-gradient%d", get_type()));
}

void Gradient::add_force(Actor *a, V2 &f)
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
    Floor::add_force(a, f);
}



void InitFloors()
{
    // Floors (most floors are defined in init.lua)
    Register(new DummyFloor);

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

} // namespace enigma
