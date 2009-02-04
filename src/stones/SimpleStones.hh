/*
 * Copyright (C) 2008 Andreas Lochmann
 * Copyright (C) 2008,2009 Ronald Lamprecht
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
#ifndef SIMPLESTONES_HH
#define SIMPLESTONES_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * BlurStone (old YinYang)
     */
    class BlurStone : public Stone {
        CLONEOBJ(BlurStone);
    private:
        enum iState {
            STRAIGHT,   ///<  encoded flavor 
            CROSS,      ///< 
            MAGIC      ///< 
        };
    public:
        BlurStone(int type);
        
         // Object interface
        virtual std::string getClass() const;        
        virtual void setAttr(const string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        
        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        
        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
    };
    
    /** 
     * ChargeStone
     */
    class ChargeStone : public Stone {
        CLONEOBJ(ChargeStone);
        
    public:
        ChargeStone(double charge);

        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);

        // GridObject interface
        virtual void init_model();
        
        // ModelCallback interface  - Animation callback
        virtual void animcb();

        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
    };

    /** 
     * FlashStone
     */
    class FlashStone : public Stone {
        CLONEOBJ(FlashStone);

    public:
        FlashStone();

        // Object interface
        virtual std::string getClass() const;

        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
    };

    /** 
     * GrateStone
     */
    class GrateStone : public Stone {
        CLONEOBJ(GrateStone);

    private:
        enum iState {
            CROSS,      ///< 
            FRAMED      ///< 
        };
    public:
        GrateStone(int type);
        
         // Object interface
        virtual std::string getClass() const;        
        virtual void setAttr(const string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        
        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();

        // Stone interface
        virtual bool is_floating() const;
        virtual bool is_transparent(Direction d) const;
        virtual StoneResponse collision_response(const StoneContact &sc);
    };

    /** 
     * PlopStone
     */
    class PlopStone : public Stone {
        CLONEOBJ(PlopStone);
        DECL_TRAITS;
    public:
        PlopStone();
        
         // Object interface
        virtual std::string getClass() const;        
        
        // GridObject interface
        virtual void init_model();
        
        // Stone interface
        virtual void on_floor_change();
    };
   
    /** 
     * Yinyang 
     */
    class YinyangStone : public Stone {
        CLONEOBJ(YinyangStone);
        DECL_TRAITS;
    private:
        enum iState {
            IDLE,       ///< 
            ACTIVE      ///< 
        };
    public:
        YinyangStone(int initState);
        
         // Object interface
        virtual std::string getClass() const;        
        
        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        
        // ModelCallback interface  - Animation callback
        virtual void animcb();

        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
    };
} // namespace enigma

#endif /*SIMPLESTONES_HH*/
