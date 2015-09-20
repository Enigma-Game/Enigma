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
#ifndef FLOORS_HH_INCLUDED
#define FLOORS_HH_INCLUDED

#include "items.hh"

namespace enigma {

class Actor;
class Stone;

enum FloorFlags {
    flf_default = 0,
    flf_indestructible = 0x01,
    flf_no_items = 0x02,
    flf_norespawn = 0x04,
};

enum FloorID { fl_abyss, fl_water, fl_swamp, fl_space, fl_thief };

enum FloorFireType {
    flft_default = 0,
    flft_burnable = 0x01,   // Floor behaves as if it-burnable lies on it.
    flft_ignitable = 0x02,  // Ignite on bomb explosions. Not used.
    flft_noash = 0x04,      // Don't leave ash behind; floor might burn again.
    flft_burnagain = 0x08,  // Not used yet, nor implemented; see flft_noash.
    flft_eternal = 0x10,    // Fire doesn't stop burning by itself.
    flft_secure = 0x20,     // Secures fire and heat-effects when neighbors burn.
    flft_fastfire = 0x40,   // Suppress use of fire_countdown, resulting in faster fire.
    flft_initfire = 0x80    // Start burning (forcefire on init-message).
    // Note that only flft_burnable and flft_noash are really used as traits.
    // The others are 0 by default for all floors, but used as selectors
    // for has_firetype. (Future use for special floors not excluded.)
};

struct FloorTraits {
    // Variables
    std::string name;
    FloorFlags flags;
    FloorFireType firetype;
    std::string firetransform;  // fire on the same tile
    std::string heattransform;  // fire on neighboring tile

    // Constructor
    FloorTraits(const char *n, FloorFlags flags_, FloorFireType flft = flft_default,
                const char *ft = "", const char *ht = "")
    : name(n), flags(flags_), firetype(flft), firetransform(ft), heattransform(ht) {}
};

enum FloorHeatFlags {
    // These are used as second argument to try_heating and try_ignite,
    // they contain the context of a call.
    flhf_message = 0,   // Source is a user-message.
    flhf_fire = 0x01,   // Source is fire.
    flhf_first = 0x02,  // First heat message from a burning site.
    flhf_last = 0x04    // Last heat message from a burning site.
};

class Floor : public GridObject {
public:
    Floor(const FloorTraits &tr);
    Floor(const char *kind, double friction_ = 0.0, double adhesion = 0.0,
          FloorFlags flags = flf_default, FloorFireType flft = flft_default,
          const char *firetransform_ = "", const char *heattransform_ = "");

    // Object interface
    virtual Value message(const Message &m);
    virtual void setAttr(const std::string &key, const Value &val);
    virtual Value getAttr(const std::string &key) const;

    // Floor interface
    virtual ecl::V2 process_mouseforce(Actor *a, ecl::V2 force);
    virtual void add_force(Actor *, ecl::V2 &);  // Note: actor = 0 must be allowed!

    virtual void on_drop(Item *) {}
    virtual void on_pickup(Item *) {}

    virtual void stone_change(Stone *);
    virtual void actor_contact(Actor *) {}

    virtual double get_friction() const;
    virtual double getAdhesion() const;

    virtual void get_sink_speed(double &sinkspeed, double &raisespeed) const;
    virtual bool is_destructible() const;
    virtual bool is_freeze_check() const;

    virtual void animcb();
    void on_burnable_animcb(bool justIgnited);  // Called by burnable-items on it.

protected:
    virtual Object::ObjectType getObjectType() const { return Object::FLOOR; }

    // GridObject interface
    virtual void on_creation(GridPos p);
    virtual void set_model(const std::string &mname);
    virtual display::Model *get_model();
    virtual void kill_model(GridPos p);

    // Fire interface
    virtual bool has_firetype(FloorFireType selector);
    virtual std::string get_firetransform();
    virtual std::string get_heattransform(bool override_mode);
    void heat_neighbor(Direction dir, FloorHeatFlags flhf);
    int get_fire_countdown();
    virtual bool on_heattransform(Direction sourcedir, FloorHeatFlags flhf);

    ecl::V2 var_floorforce;

private:
    virtual void on_actorhit(Actor * /*a*/) {}
    // Fire logic
    bool try_heating(Direction sourcedir, FloorHeatFlags flhf);
    bool try_ignite(Direction sourcedir, FloorHeatFlags flhf);
    bool force_fire();
    bool stop_fire(bool is_message);

    // Traits and variables
    FloorTraits traits;
    bool heating_animation;
    int fire_countdown;  // used to delay ignition, default is 1.
    double friction;
    double adhesion;
};

}  // namespace enigma

#endif
