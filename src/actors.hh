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

#include "StateObject.hh"

namespace enigma {

    enum ActorID {
        ac_INVALID    = -1,
        ac_FIRST      = 0,
        ac_marble_black  = 0,
        ac_marble_white  = 1,
        ac_pearl_black   = 2,
        ac_pearl_white   = 3,
        ac_killer_black  = 4,
        ac_killer_white  = 5,
        ac_rotor      = 6,
        ac_top        = 7,
        ac_horse      = 8,
        ac_bug        = 9,
        ac_cannonball = 10,
        ac_spermbird  = 11,
        ac_LAST       = 11,
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
        ecl::V2 frozen_vel;     // Velocity backup for forzen actors
        ecl::V2 pos_force;      // Extrapolated position for global force calc
        ecl::V2 forceacc;       // Force accumulator
        double charge;          // Electric charge
        double mass;            // Mass
        double radius;          // Radius of the ball
        bool   grabbed;         // Actor not controlled by the physics engine
        bool   created;         // Actor has already been created
        bool   ignore_contacts; // Do not perform collision handling

	// Variables used internally by the physics engine

//        ecl::V2 last_pos;        // Position before current tick
//        ecl::V2 oldpos;		// Backup position for enter/leave notification
        ecl::V2 force;		// Force used during tick
        ecl::V2 collforce;
        double  friction;       // friction on the current position
        
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


    class Actor : public StateObject, public display::ModelCallback {
        friend class World;
        friend class ActorsInRangeIterator;
    public:
        static const double max_radius;
        
        // ModelCallback interface
        void animcb ();

        /* ---------- Object interface ---------- */
        Actor *clone() = 0;
        virtual void setAttr(const string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);
 
        /* ---------- Actor interface ---------- */
        virtual const ActorTraits &get_traits() const = 0;

        virtual void think (double dtime);

        virtual bool on_collision(Actor *a);
        virtual void on_creation(const ecl::V2 &pos);
        virtual void on_respawn(const ecl::V2 &pos);

        virtual bool is_dead() const = 0;
        virtual bool isMoribund() const {return is_dead(); }
        virtual bool is_movable() const { return true; }
        virtual bool is_flying() const { return false; }
        virtual bool is_on_floor() const { return true; }
        virtual bool is_drunken() const { return false; }
        virtual bool is_invisible() const { return false; }

        virtual bool can_drop_items() const { return true; }
        virtual bool can_move() const;
        virtual bool can_be_warped() const { return false; }
        virtual bool has_shield() const { return true; }

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
//        void set_spikes(bool has) { spikes = has; }

        static double get_max_radius(); // max. radius of all actors

        int get_controllers() const { return controllers; }
        bool isSteerable() const { return adhesion != 0.0; }
        double get_mouseforce() const { return adhesion; }

        bool controlled_by(int player) const {
            return (get_controllers() & (1+player)) != 0;
        }

        const GridPos &get_gridpos() const { return m_actorinfo.gridpos; }
        virtual double squareDistance(const Object *other) const;
        virtual bool isSouthOrEastOf(const Object *other) const;

    protected:
        virtual Object::ObjectType getObjectType() const {return Object::ACTOR;}
        
        Actor(const ActorTraits &tr);
        void set_model (const string &modelname);
        void set_anim (const string &modelname);

        display::SpriteHandle &get_sprite() { return m_sprite; }

        /* ---------- Variables ---------- */
        ActorInfo             m_actorinfo;
        bool         centerRespawn;   // default, like on flag drop or sink, fall, shatter
        bool         inplaceRespawn;  // respawn on exactly last valid position, use on suicide to prevent shortcuts
    private:
        Actor       *left;   // x-coordinate sorted double linked list
        Actor       *right;
        display::SpriteHandle m_sprite;
        ecl::V2      startingpos;
        ecl::V2      respawnpos;
        bool         flagRespawn;
        bool         firstGridStep;
        bool         spikes; // set by "it-pin"
        int          controllers;
        double       adhesion;
        GridPos      last_gridpos;   // last pos handled by actor move
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
    
/* -------------------- Actor Macros -------------------- */


#define DECL_ACTORTRAITS \
        static ActorTraits traits; \
        const ActorTraits &get_traits() const { return traits; } \

#define DECL_ACTORTRAITS_ARRAY(n, subtype_expr)                                  \
        static ActorTraits traits[n];                                            \
        const ActorTraits &get_traits() const { return traits[subtype_expr]; }

    
} // namespace enigma

#endif
