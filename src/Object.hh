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
#include <stdint.h>

namespace enigma { 

    using std::string;
    
    struct Message;
    
    enum ObjectFlagsBits {
        OBJBIT_LIGHTNEWDIRS  =  15,   ///< GridObject 4 direction bits with current/new light
        OBJBIT_LIGHTOLDDIRS  =  240,  ///< GridObject 4 direction bits with previous light
        OBJBIT_LIGHTALLDIRS  =  255,  ///< GridObject all 8 direction bits with old an new light
        OBJBIT_PHOTOACTIV    =  1<<8  ///< GridObject registered as photo activ
    };
    
    class Value;

    /**
     * Object is the base class for all "objects" in the world.
     * The most important facilities this class provides are:
     *
     * - A way to clone() and dispose() objects.  This is mainly used
     *     in function MakeObject() to create new objects of a given
     *     type.
     * 
     * - A way to pass messages between unrelated objects via message().
     *     This allows us to send messages to objects from Lua and to
     *     decouple objects types as much as possible.
     * 
     * - A way to get and set attributes.  These attributes are quite
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
            
        typedef ecl::AssocList<std::string, Value> AttribMap;

        Object();
        Object(const char *kind);
        Object(const Object &src_obj); 
        virtual ~Object();

        static Object * getObject(int id);
        int getId() const;
        
        /* ---------- depreceated methods ---------- */

        bool is_kind(const char *kind) const;
        bool is_kind(const string& kind) const;
        virtual const char *get_kind() const;
        const AttribMap &get_attribs() const { return attribs; }  // just used by ObjectRepos::dump_info()        

        /* ---------- Helper routines ---------- */

        void send_impulse(const GridPos& dest, Direction dir);
        void send_impulse(const GridPos& dest, Direction dir, double delay);

        /* ---------- Object interface ---------- */
        
        /**
         * The main object category name of the object that describes its gaming class.
         * All objects of a gaming class support the same attributes and messages. But
         * they may still be members of different kind subcategories. E.g. "st_panel"
         * is a class with "st_panel_n", "st_panel_s",... as kinds. A gaming object
         * class is often implemented as a separate C++ class. But sometimes several 
         * gaming classes share a single C++ class. E.g. "st_panel", "st_bluesand" are
         * both impleneted by the class "ClusterStone". In this case we talk about a 
         * common "familiy". 
         */
        virtual std::string getClass() const;
        
        /**
         * The most specific object category name. Many objects change their kind during
         * their lifecycle. All attributes are evaluated to determine the current kind
         * of a gaming object. 
         */
        std::string getKind() const;
        
        /**
         * Check the current kind of the object versus the given name.
         */
        bool isKind(const std::string &kind) const;
        
        
        bool validateMessage(std::string msg, Value arg);

        /**
         * 
         */
        virtual Value message(const Message &m);
                
        /**
         * Store the value information for the given key with prior checking of
         * write allowance. All level code attribute settings should pass this
         * central call. If a set is granted the virtual unchecked setAttr is 
         * executed.
         */
        void setAttrChecked(const std::string &key, const Value &val);
        
        /**
         * Store the value information for the given key. If it is a XML declared
         * system attribute or a user attribute it will be stored in the attribute
         * map. Otherwise an ivar may be set. Nothing happens if the attribute is
         * declared as read only. If the value is "nil" the attribute will be reset
         * to its declared default. If the default is "nil", too, or if it is a user
         * attribute, it will be deleted in the attribute map. In these cases a read
         * will return "nil" either way.
         */
        virtual void setAttr(const std::string &key, const Value &val);
        
        /**
         * Get an attribute or value for the given key with prior checking of
         * read allowance. All level code attribute reads should pass this central
         * call. If a read is granted the virtual unchecked getAttr is executed.
         */
        Value getAttrChecked(const std::string &key) const;
        
        /**
         * Get an attribute that has been set or that stands as a proxy for a
         * trait or ivar. Object itself will just return attribute values
         * that are stored in its attribute map. For not existing attributes
         * a XML declared standard default value will be returned. If the default
         * is "nil" type DEFAULT will be returned. 
         * 
         * Subclasses may override this method to supply values of traits or
         * ivars. This way levels can gain read access to attributes that can
         * not to be stored in the attribute map due to performance reasons.
         */
        virtual Value getAttr(const std::string &key) const;
        

        /**
         * Get an attribute or a special given default value. This method
         * gets attributes like the simple argumented getAttr method but
         * returns the given default value instead of a DEFAULT value if
         * no explicit attribute exists.
         */
        Value getDefaultedAttr(const string &key, Value defaultValue) const;
        
        
        virtual Object *clone()=0;
        virtual void dispose()=0;

        virtual void warning(const char *format, ...) const;
        virtual ObjectType getObjectType() const;
        void transferName(Object *target);
        virtual void transferIdentity(Object *target);

    protected:
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
        
        /**
         * A central managed container for 32 single bit flags to be used by 
         * subclasses. Object is reponsible of efficiently cloning, saving and
         * restoring these flags on demand. Subclasses use the bits as follows:
         * 
         * - Bit 0-15 are reserved for classes Object to GridObject
         * 
         * - Bit 16-23 are reserved for classes Item, Stone, Floor, Actor
         * 
         * - Bit 24-31 are reserved for the final subclass
         * 
         * See enumeration ObjectFlagsBits for Bitmasks of Bit 0-15. Note that
         * Bitmasks would not have been an alternative as the subclasses may
         * and will use the upper bits in a quite different mannor. 
         */
        uint32_t  objFlags;
        
    private:
        friend void InitWorld();   // for bootFinished() access
        
        static int next_id;
        static std::map<int, Object *> objects;
        
        int       id;
        AttribMap attribs;
        
        static int getNextId(Object *obj, bool bootFinished);
        static void bootFinished();
        static void freeId(int id);
        
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
