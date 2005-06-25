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
 * $Id: floors.hh,v 1.2 2004/03/15 20:15:36 dheck Exp $
 */
#ifndef FLOORS_HH_INCLUDED
#define FLOORS_HH_INCLUDED

namespace world
{
    enum FloorFlags {
        flf_default        = 0,
        flf_indestructible = 0x01,
        flf_no_items       = 0x02,
        flf_norespawn      = 0x04,
    };

    enum FloorID {
        fl_abyss,
        fl_water,
        fl_swamp,
        fl_space
    };

    struct FloorTraits {
        // Variables
	string     name;
        double     friction;
        double     mousefactor;
        FloorFlags flags;

        // Constructor
        FloorTraits (const char *n, double f, double m, FloorFlags flags_)
        : name(n), friction(f), mousefactor(m), flags(flags_)
        {}
     };

    class Floor : public GridObject {
    public:
        Floor (const FloorTraits &tr);
        Floor (const char *kind, double friction, double mfactor, FloorFlags flags=flf_default);

        // Object interface
        Floor *clone();
        void dispose();
        void message(const string& msg, const Value &val);


        // Floor interface
        virtual ecl::V2 process_mouseforce (Actor *a, ecl::V2 force);
        virtual void add_force(Actor */*a*/, ecl::V2 &/*f*/) { }

        virtual void on_drop (Item *) {}
        virtual void on_pickup (Item *) {}

        virtual void stone_change(Stone *) {}
        virtual void actor_contact (Actor *) {}

        virtual double friction() const;
        virtual double mousefactor() const;

        virtual void get_sink_speed (double &sinkspeed, double &raisespeed) const;
	virtual bool is_destructible() const;
    protected:
        // GridObject interface
        void set_model (const std::string &mname);
        display::Model *get_model ();
        void kill_model (GridPos p);
    private:
        virtual void on_actorhit(Actor */*a*/) {}
        FloorTraits traits;
    };

    void InitFloors();
}

#endif
