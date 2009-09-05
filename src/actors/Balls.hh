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
#ifndef BALLACTORS_HH
#define BALLACTORS_HH

#include "actors.hh"

namespace enigma {

    /** 
     * 
     */
    /*! The base class for all marbles. */
    class BasicBall : public Actor {
        
    protected:
        enum iState {
            NO_STATE,
            NORMAL,
            SHATTERING,
            BUBBLING,
            FALLING,            // falling into abyss
            JUMPING,
            DEAD,               // marble is dead
            RESURRECTED,        // has been resurrected; about to respawn
            APPEARING,          // appearing when level starts/after respawn
            DISAPPEARING,       // disappearing when level finished
            FALLING_VORTEX,     // falling into vortex
            RISING_VORTEX,      // appear in vortex
            JUMP_VORTEX,        // jump out of vortex (here player controls actor)
        };

        enum HaloState {
            NOHALO, HALOBLINK, HALONORMAL
        };

    public:
        BasicBall(const ActorTraits &tr);

        // Object interface.
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);

        // Actor interface
        virtual bool is_dead() const;
        virtual bool isMoribund() const;
        virtual bool is_movable() const;
        virtual bool is_flying() const;
        virtual bool is_on_floor() const;
        virtual bool is_drunken() const;
        virtual bool is_invisible() const;
        virtual bool can_drop_items() const;
        virtual bool can_pickup_items() const;
        virtual bool can_be_warped() const;
        virtual bool has_shield() const;
        
        virtual void on_creation(const ecl::V2 &pos);
        virtual void think (double dtime);
        virtual void on_respawn(const ecl::V2 &pos);

        virtual void move_screen();
        virtual void hide();
        
        // ModelCallback interface  - Animation callback
        virtual void animcb();

    protected:
        virtual std::string getModelBaseName() const;
        virtual void sink(double dtime);

    private:
        void disable_shield();
        void change_state_noshield(iState newstate);
        void change_state(iState newstate);

        // Model management
        void update_model();
        void set_sink_model(const string &m);
        void set_shine_model (bool shinep);
        void update_halo();

        static const int minSinkDepth = 0; // normal level
        static const int maxSinkDepth = 7; // actor dies at this depth
        double sinkDepth;       // how deep actor has sunk
        int    sinkModel;       // current model
        bool   lastshinep;

        double vortex_normal_time; // while jumping out of vortex: time at normal level

        // Variables
        static const double   SHIELD_TIME;
        display::SpriteHandle m_halosprite;
        double                m_shield_rest_time;
        HaloState             m_halostate;

        double m_drunk_rest_time;
        double m_invisible_rest_time;
    };
    
/* -------------------- Marble  -------------------- */
    class Marble : public BasicBall {
        CLONEACTOR(Marble);
        DECL_ACTORTRAITS_ARRAY(2, traitsIdx());
        
    public:
        Marble(int color);
        
        // Object interface.
        virtual std::string getClass() const;
        
    private:
        int traitsIdx() const;
    };
    
/* -------------------- Pearl  -------------------- */
    class Pearl : public BasicBall {
        CLONEACTOR(Pearl);
        DECL_ACTORTRAITS_ARRAY(2, traitsIdx());
        
    public:
        Pearl(int color);
        
        // Object interface.
        virtual std::string getClass() const;
        
    protected:
        virtual void sink(double dtime);
    private:
        int traitsIdx() const;
    };
    

} // namespace enigma

#endif /*BALLACTORS_HH*/
