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
#ifndef ACTORS_HH_INCLUDED
#define ACTORS_HH_INCLUDED

#include "objects.hh"

namespace enigma {

    enum ActorID {
        ac_INVALID    = -1,
        ac_FIRST      = 0,
        ac_blackball  = 0,
        ac_whiteball  = 1,
        ac_meditation = 2,
        ac_killerball = 3,
        ac_rotor      = 4,
        ac_top        = 5,
        ac_horse      = 6,
        ac_bug        = 7,
        ac_cannonball = 8,
        ac_spermbird  = 9,
        ac_LAST       = 9,
        ac_COUNT
    };

    struct ActorTraits {
        const char *name;
        ActorID     id;
        unsigned    id_mask;
        float       radius;
        float       default_mass;
    };

/* -------------------- ActorInfo -------------------- */

    struct Contact {
        ecl::V2 pos;
        ecl::V2 normal;

        // Constructor
        Contact() {
        }
        
        Contact (const ecl::V2 &pos_, const ecl::V2 &normal_)
        : pos(pos_), normal (normal_) {} 
    };
    
#define MAX_CONTACTS 7

    struct Field;
    /*!  
     * This class contains the information the physics engine
     * maintains about dynamic objects ("actors").
     */
    struct ActorInfo {
	// ---------- Variables ----------

        ecl::V2 pos;            // Absolute position
        GridPos gridpos;        // Grid position for pos
        const Field *field;     // Field of pos
        ecl::V2 vel;            // Velocity
        ecl::V2 pos_force;      // Extrapolated position for global force calc
        ecl::V2 forceacc;       // Force accumulator
        double charge;          // Electric charge
        double mass;            // Mass
        double radius;          // Radius of the ball
        bool   grabbed;         // Actor not controlled by the physics engine
        bool   ignore_contacts; // Do not perform collision handling

	// Variables used internally by the physics engine

//        ecl::V2 last_pos;        // Position before current tick
//        ecl::V2 oldpos;		// Backup position for enter/leave notification
        ecl::V2 force;		// Force used during tick
        ecl::V2 collforce;
        
        // 2 sets of contacts - one for the current tick, one for the last tick
        Contact contacts_a[MAX_CONTACTS];
        Contact contacts_b[MAX_CONTACTS];
        Contact *contacts;        // pointer to the durrent ticks contacts 
        Contact *last_contacts;   // pointer to the last ticks contacts
        int contacts_count;       // number of valid contacts for current tick
        int last_contacts_count;  // number of valid contacts for last tick

        // Constructor
        ActorInfo();
    };


    class Actor : public Object, public display::ModelCallback {
        friend class World;
        friend class ActorsInRangeIterator;
    public:
        static const double max_radius;
        
        // ModelCallback interface
        void animcb ();

        /* ---------- Object interface ---------- */
        Actor *clone() = 0;
        virtual Value   message (const string &m, const Value &);
        void   set_attrib (const string& key, const Value &val);

        /* ---------- Actor interface ---------- */
        virtual const ActorTraits &get_traits() const = 0;

        virtual void think (double dtime);

        virtual void on_collision(Actor *a);
        virtual void on_creation(const ecl::V2 &pos);
        virtual void on_respawn (const ecl::V2 &pos);

        virtual bool is_dead() const = 0;
        virtual bool is_movable() const { return true; }
        virtual bool is_flying() const { return false; }
        virtual bool is_on_floor() const { return true; }
        virtual bool is_drunken() const { return false; }
        virtual bool is_invisible() const { return false; }

        virtual bool can_drop_items() const { return false; }
        virtual bool can_move() const;
        virtual bool can_be_warped() const { return false; }
        virtual bool has_shield() const { return false; }

        virtual void init();

        /* ---------- Methods ---------- */
        void move ();
        virtual void move_screen ();
        void warp (const ecl::V2 &newpos);
        bool sound_event (const char *name, double vol = 1.0);

        void      respawn();
        void      set_respawnpos(const ecl::V2& p);
        void      remove_respawnpos();
        void      find_respawnpos();
        const ecl::V2 &get_respawnpos() const;
        const ecl::V2 &get_startpos() const;

        virtual void hide();
        void show();

        void add_force (const ecl::V2 &f);

        /* ---------- Accessors ---------- */
        ActorInfo *get_actorinfo();
        const ActorInfo &get_actorinfo() const;
        const ecl::V2 &get_pos() const;
        const ecl::V2 &get_pos_force() const;

        const ecl::V2 &get_vel() const {
            return m_actorinfo.vel; 
        }

        bool has_spikes() const { return spikes; }
        void set_spikes(bool has) { spikes = has; }

        static double get_max_radius(); // max. radius of all actors

        int get_controllers () const { return controllers; }
        double get_mouseforce () const { return mouseforce; }

        bool controlled_by(int player) const {
            return (get_controllers() & (1+player)) != 0;
        }

        const GridPos &get_gridpos() const { return m_actorinfo.gridpos; }

    protected:
        virtual Object::ObjectType getObjectType() const {return Object::ACTOR;}
        
        Actor(const ActorTraits &tr);
        void set_model (const string &modelname);
        void set_anim (const string &modelname);

        display::SpriteHandle &get_sprite() { return m_sprite; }

        /* ---------- Variables ---------- */
        ActorInfo             m_actorinfo;
    private:
        Actor       *left;   // x-coordinate sorted double linked list
        Actor       *right;
        display::SpriteHandle m_sprite;
        ecl::V2                startingpos;
        ecl::V2                respawnpos;
        bool                  use_respawnpos;
        bool                  spikes; // set by "it-pin"
        int                   controllers;
        double                mouseforce;
        GridPos               last_gridpos;   // last pos handled by actor move
    };

    inline ActorID get_id (Actor *a) {
        return a->get_traits().id;
    }

    inline double get_radius (const Actor *a) { 
        return a->get_actorinfo().radius; 
    }
    
    inline double get_mass (const Actor *a) { 
        return a->get_actorinfo().mass; 
    }
    
    inline double get_charge (const Actor *a) { 
        return a->get_actorinfo().charge; 
    }
    
    class ActorsInRangeIterator {
        public:
            ActorsInRangeIterator(Actor *center, double range, unsigned type_mask);
            Actor *next();
        private:
            Actor *centerActor;
            double xCenter;
            Actor *previousActor;
            Direction dir;
            double rangeDist;
            unsigned typeMask;
    };

/* -------------------- Global Functions -------------------- */

    void InitActors();
    
} // namespace enigma

#endif
