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
#ifndef TURNSTILE_HH
#define TURNSTILE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    class TurnstileArm;

    /**
     *
     */
    class TurnstilePivot : public Stone {
        CLONEOBJ(TurnstilePivot);
        DECL_TRAITS_ARRAY(2, traitsIdx());
    private:
        enum iState {
            IDLE,     ///<
            ROTATING  ///<
        };

    public:
        TurnstilePivot(std::string flavor);

        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const string& key, const Value &val);
        virtual Value message(const Message &m);

        // StateObject interface
        virtual int maxState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();

        // ModelCallback interface
        virtual void animcb();

        // Stone interface
//        virtual void actor_hit(const StoneContact &sc);

        bool rotate(bool clockwise, Object *impulse_sender, TurnstileArm *initiator);
        int traitsIdx() const;
    private:
        DirectionBits arms_present() const;
        bool no_stone(int xoff, int yoff) const;
        void rotate_arms(DirectionBits arms, bool clockwise);
        void setArm(Object * obj, Direction dir);
        bool calc_arm_seen(bool cw, DirectionBits arms, int field);
        void handleActorsAndItems(bool clockwise, Object *impulse_sender);
    };


    /**
     *
     */
    class TurnstileArm : public Stone {
        CLONEOBJ(TurnstileArm);
        DECL_TRAITS_ARRAY(4, state);

    public:
        TurnstileArm(Direction dir);

        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);

        // StateObject interface
        virtual int maxState() const;

        // GridObject interface
        virtual void init_model();

        // Stone interface
        virtual void on_impulse(const Impulse& impulse);

    private:
        TurnstilePivot *getPivot();
    };

} // namespace enigma

#endif
