/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007,2008 Ronald Lamprecht
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
#ifndef OBJECT_HH
#define OBJECT_HH

#include "display.hh"
#include "ecl_alist.hh"
#include "ecl_math.hh"
#include "Value.hh"
#include <map>

namespace enigma { 

    using std::string;
    
    struct Message;
    class Value;

    /**
     *  Object is the base class for all ``objects'' in the world.
     * The most important facilities this class provides are:
     *
     * (1) A way to clone() and dispose() objects.  This is mainly used
     *     in function MakeObject() to create new objects of a given
     *     type.
     * 
     * (2) A way to pass messages between unrelated objects via message().
     *     This allows us to send messages to objects from Lua and to
     *     decouple objects types as much as possible.
     * 
     * (3) A way to get and set attributes.  These attributes are quite
     *     similar to instance variables, but they can be easily modified
     *     from Lua.  This makes it possible to modify certain object
     *     parameters (such as the text on a piece of paper or the color
     *     of an oxyd stone) in level descriptions.
     * 
     * The various Object subclasses instances need to register a template
     * instance for each object name. To avoid the inclusion of every
     * subclass declaration file into the registry for registry driven forward
     * initialization we make use of the static file based initialization
     * that occurs prior to the main application startup. To be independent of
     * the undefined sequence in which the files are initialized we store the
     * template instances, object names and id's in function local static caches.
     * These caches are copied to the final runtime data structures on the main
     * application startup. We call this feature "boot"-initialization. The macros
     * BOOT_REGISTER_START and BOOT_REGISTER_END will be used once at the end of
     * every subclass file. They embrace the BootRegister() function calls that
     * register the templates.
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
        Value getDefaultedAttr(const string &key, Value defaultValue) const;


        /* ---------- Helper routines ---------- */

        void send_impulse(const GridPos& dest, Direction dir);
        void send_impulse(const GridPos& dest, Direction dir, double delay);

        /* ---------- Object interface ---------- */

        virtual const char *get_kind() const;

        /**
         * 
         */
        virtual Value message(const Message &m);
        
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
        void transferName(Object *target);
        virtual void transferIdentity(Object *target);

    protected:
        virtual Value getDefaultValue(const string &key) const;
        /** This function is used by all triggers, switches etc. that
         * perform some particular action when activated (like opening
         * doors or switching lasers on and off). It interprets the
         * "action" and "target" attributes of `o'. 
         */
        void performAction(const Value &val);
        
        virtual Value invertActionValue(const Value &val) const;
        
        /**
         * Evaluate multiple destinations described by tokenized destination
         * attribute where every token may be a group by itself. All valid
         * destinations are indexed in the sequence of tokens and group positions.
         * The position at the given index is returned as the destination position.
         * If the caller did index beyond the last valid position this convenience
         * method returns false, otherwise true to mark a valid position. 
         * Objects with destinations like Vortex and Horse make use of this method.
         * @arg idx      requested index of destination starting with 0
         * @arg destpos  return value for position that is centered to a grid
         * @return       validity of position at given index
         */
        bool getDestinationByIndex(int idx, ecl::V2 &dstpos) const;
        
    private:
        friend void InitWorld();   // for bootFinished() access
        
        static int next_id;
        static std::map<int, Object *> objects;
        int id;
        AttribMap attribs;
        static int getNextId(Object *obj, bool bootFinished);
        static void bootFinished();
        static void freeId(int id);
        const Value* get_attrib(const string& key) const;
    };


} // namespace enigma


#define BOOT_REGISTER_START        \
        namespace {                \
            bool do_boot_register() {

#define BOOT_REGISTER_END          \
                return true;       \
            }                      \
            static bool boot_registered = do_boot_register(); \
        } 

#define CLONEOBJ(TYPE)                                  \
        TYPE* clone() { return new TYPE(*this); }       \
        void dispose() { delete this; }

#define CLONEACTOR(TYPE)                                                \
        TYPE* clone() { TYPE *o=new TYPE(*this); o->init(); return o; } \
        void dispose() { delete this; }

#define INSTANCELISTOBJ(TYPE)                                                   \
        typedef std::vector<TYPE*> InstanceList;                                \
        static InstanceList instances;                                          \
        TYPE *clone() { TYPE *o = new TYPE(*this); instances.push_back(o); return o;}  \
        void dispose() {                                                        \
            instances.erase(find(instances.begin(), instances.end(), this));    \
            delete this;                                                        \
        }

#endif
