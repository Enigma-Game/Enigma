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
#ifndef OBJECTS_DECL_HH
#define OBJECTS_DECL_HH

#include "display.hh"
#include "ecl_alist.hh"
#include <map>

namespace world
{
    using std::string;
    using namespace enigma;

/* -------------------- Objects -------------------- */

    /*! Object is the base class for all ``objects'' in the world.
      The most important facilities this class provides are:
     
      (1) A way to clone() and dispose() objects.  This is mainly used
          in function MakeObject() to create new objects of a given
          type.
     
      (2) A way to pass messages between unrelated objects via message().
          This allows us to send messages to objects from Lua and to
          decouple objects types as much as possible.
     
      (3) A way to get and set attributes.  These attributes are quite
          similar to instance variables, but they can be easily modified
          from Lua.  This makes it possible to modify certain object
          parameters (such as the text on a piece of paper or the color
          of an oxyd stone) in level descriptions.
     */
    class Object {
    public:
        enum ObjectType { 
            OTHER,
            STONE,
            FLOOR,
            ITEM,
            ACTOR
            };
            
        Object();
        Object(const char *kind);
        Object(const Object &src_obj); 
        virtual ~Object();

        static Object * getObject(int id);
        int getId() const;
        
        /* ---------- Attributes ---------- */

        typedef ecl::AssocList<std::string, Value> AttribMap;

        bool is_kind(const char *kind) const;
        bool is_kind(const string& kind) const;

        const AttribMap &get_attribs() const { return attribs; }
        
        /**
         * Get an attribute or a special given default value. This method
         * gets attributes like the simple argumented getAttr method but
         * returns the given default value instead of a DEFAULT value if
         * no explicit attribute exists.
         */
        Value getAttr(const string &key, Value defaultValue) const;


        /* ---------- Helper routines ---------- */

        void send_impulse(const GridPos& dest, Direction dir);
        void send_impulse(const GridPos& dest, Direction dir, double delay);

        /* ---------- Object interface ---------- */

        virtual const char *get_kind() const;

        virtual Value on_message (const Message &m);
        virtual Value message(const string& msg, const Value &val);
        virtual void set_attrib(const string& key, const Value &val);
        
        /**
         * Get an attribute that has been set or that stands as a proxy for a
         * trait or ivar. Object itself will just return attribute values
         * that are stored in its attribute map. For not existing attributes
         * a value of type DEFAULT is returned.
         * 
         * Subclasses may override this method to supply values of traits or
         * ivars. This way levels can gain read access to attributes that can
         * not to be stored in the attribute map due to performance reasons.
         */
        virtual Value getAttr(const string &key) const;
        
        /**
         * Get the attribute, traits, ivar or default value for a given key.
         * This is the main access method for object values that returns
         * the best available value known for a key. It resolves attributes,
         * uses proxy values for ivars and traits, uses class defaults,
         * world defaults or system defaults.
         * 
         * This is a template method. Subclass have to override the method
         * <code>getDefaultValue()</code> to supply proper class defaults.
         * All other defaults are resolved by this method
         */
        Value getValue(const string &key) const;

        virtual Object *clone()=0;
        virtual void dispose()=0;

        virtual void on_levelinit();

        virtual void warning(const char *format, ...) const;
        virtual ObjectType getObjectType() const;

    protected:
        virtual Value getDefaultValue(const string &key) const;
    private:
        static int next_id;
        static std::map<int, Object *> objects;
        int id;
        AttribMap attribs;
        static int getNextId(Object *obj);
        static void freeId(int id);
        const Value* get_attrib(const string& key) const;
    };

/* -------------------- GridObject -------------------- */

    /*! GridObject is the base class for everything that can only be
      placed on "The Grid", i.e., for floor tiles, items, and
      stones. */

    class GridObject : public Object, public display::ModelCallback {
    public:
        GridObject() {}
        GridObject(const char * kind) : Object(kind) {}

        void creation(GridPos p) {
            pos = p;
            on_creation (p);
        }
        void removal(GridPos p) { on_removal(p); }
        GridPos get_pos() const {return pos;}

        // GridObject interface
        virtual void on_laserhit (Direction) {}
        virtual void actor_enter (Actor *) {}
        virtual void actor_leave (Actor *) {}


        void warning(const char *format, ...) const;

        // Helper functions
        bool sound_event (const char *name, double vol = 1.0);
        display::Model *set_anim (const std::string &mname);

    protected:
        // GridObject interface
        virtual void set_model (const std::string &mname) = 0;
        virtual display::Model *get_model () = 0;
        virtual void kill_model (GridPos p) = 0;

        virtual void init_model() 
        { set_model(get_kind()); }

        virtual void on_creation (GridPos) 
        { init_model(); }

        virtual void on_removal (GridPos p) 
        { kill_model (p); }

    private:
        // ModelCallback interface.
        void animcb() {}


        // Variables
        GridPos pos;
    };
}


#define CLONEOBJ(TYPE)                                  \
        TYPE* clone() { return new TYPE(*this); }       \
        void dispose() { delete this; }

#define CLONEACTOR(TYPE)                                                \
        TYPE* clone() { TYPE *o=new TYPE(*this); o->init(); return o; } \
        void dispose() { delete this; }


// #define SINGLETONOBJ(TYPE)                    \
//         TYPE* clone() { return this; }        \
//         void dispose() {}


#define INSTANCELISTOBJ(TYPE)                                                   \
        typedef std::vector<TYPE*> InstanceList;                                \
        static InstanceList instances;                                          \
        TYPE *clone() { TYPE *o = new TYPE(*this); instances.push_back(o); return o;}  \
        void dispose() {                                                        \
            instances.erase(find(instances.begin(), instances.end(), this));    \
            delete this;                                                        \
        }

#endif
