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
#ifndef GRIDOBJECT_HH
#define GRIDOBJECT_HH

#include "StateObject.hh"

#include "display.hh"
#include "ecl_alist.hh"
#include <list>

namespace enigma { 

    using std::string;
    

/* -------------------- GridObject -------------------- */
    class Actor;

    /** 
     * GridObject is the base class for everything that can only be
     * placed on "The Grid", i.e., for floor tiles, items, and
     * stones. 
     */
    class GridObject : public StateObject, public display::ModelCallback {
    public:
        GridObject() : pos (GridPos(-1, -1)) {}
        GridObject(const char * kind) : StateObject(kind), pos (GridPos(-1, -1)) {}

        // Object interface
        virtual void setAttr(const string& key, const Value &val);
        virtual Value getAttr(const string &key) const;

        // StateObject interface
        virtual void setState(int extState);


        void creation(GridPos p) {
            pos = p;
            on_creation(p);
        }
        void removal(GridPos p) { 
            on_removal(p);
            pos.x = -1;
            pos.y = -1;
        }
        GridPos get_pos() const {return pos;}
        
        virtual void setOwner(int player);
        
        /**
         * Get the player number who is owning the object.
         * @return  integer value of player number or nil if not owned by a player
         */
        Value getOwner();
        
        /**
         * Set the location of the GridObject to a given world or mirrored owner
         * position.
         */
        virtual void setOwnerPos(GridPos po);
         
        /**
         * Get the position of object within the world. Only objects owned by
         * players will return positions outside of the world. 
         */
        GridPos getOwnerPos();
        
        /**
         * Verify that the GridObject is part of the world's grid and display 
         * models can be set and killed without restriction. Especially attribute
         * changes and messages need to check carefully if just the objects ivars
         * and attributes may be changed or if the display can and has to be 
         * updated, too.
         * @return   true if display access is possible, false otherwise
         */
        bool isDisplayable() const;

        // GridObject interface
        
        virtual void actor_enter(Actor *a) {}
        virtual void actor_leave(Actor *a) {}


        void warning(const char *format, ...) const;

        // Helper functions
        bool sound_event (const char *name, double vol = 1.0);

        DirectionBits getConnections() const;
        virtual DirectionBits getFaces(bool actorInvisible = false) const;
        
        virtual double squareDistance(const Object *other) const;
        virtual bool isSouthOrEastOf(const Object *other) const;

        virtual std::string getModelName() const;
        virtual display::Model *get_model () = 0;

    protected:
        // GridObject interface
        virtual void set_model (const std::string &mname) = 0;
        void set_anim (const std::string &mname);

        virtual void kill_model (GridPos p) = 0;

        virtual void init_model();

        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        

    private:
        // ModelCallback interface.
        void animcb() {}


        /**
         * The location of the GridObject. An object that is set on one of the world's
         * layers will store the x and y coordinates with values greater equal 0. An
         * object that is not a direct part of the world has a pos.x < 0. Objects that
         * are owned by other objects that are part of a world layer will store the
         * position of the owner mirrored at point -1, -1. This results in pos.x <= -2.
         * All other GridObjects will have pos.x = -1. In this case pos.y >= 0 identifies
         * the player that owns the object in his inventory. Objects that are neiter set
         * in the world nor owned by anyone have pos.x = pos.y = -1. 
         */
        GridPos pos;


    // GridObject laser light support
    public:
        static void preLaserRecalc();
        static void postLaserRecalc();
        static void prepareLevel();
    private:
        static std::list<GridObject *> photoSensorList;
        static std::list<GridObject *>::iterator postRecalcItr;
    
    public:
        virtual void processLight(Direction d);   // direction of laserbeam
        virtual DirectionBits emissionDirections() const;
        
    protected:
        /**
         * Hook of laser light recalculation that is called for photo active objects
         * every time the illumination from any side of the grid did change. You need
         * to activate a GridObject first by calling "activatePhoto()". On removal of
         * an activated GridObject "deactivatePhoto()" is automatically executed. All
         * reported directions are the directions the laser beams are bound to, not
         * the objects grid faces. Use the "revert" function to convert the light 
         * directions to object faces.<p>
         * To identify a side independent light switch on compare "oldDirs" to 0, 
         * to identify a side independent light switch off compare "newDirs" to 0.
         * Apply "added_dirs(oldDirs, newDirs)" to get the directions of new laser
         * light, apply "removed_dirs(oldDirs, newDirs)" to get the directions of
         * switched off light.<p>
         * Note that on level initialization this method will not be called. The
         * light directions are silently updated, but no actions should be caused
         * by the initial light beam calculation due to the snapshot principle.
         */
        virtual void lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs);
        
        void activatePhoto();
        void deactivatePhoto();
        DirectionBits updateCurrentLightDirs();
    };

} // namespace enigma

#endif
