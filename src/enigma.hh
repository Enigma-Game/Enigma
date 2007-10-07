/*
 * Copyright (C) 2002,2004 Daniel Heck
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
#ifndef ENIGMA_HH
#define ENIGMA_HH

/*
 * This file contains declarations for facilities used by several
 * different parts of the program, like common types and constants,
 * and routines for resource management.
 */

#include "fwd.hh"
#include "file.hh"
#include "ecl_fwd.hh"
#include "ecl_math.hh"
#include "ecl_util.hh"
#include "ecl_cache.hh"

#define NUMENTRIES(array) (sizeof(array)/sizeof(*array))

namespace enigma
{
    using namespace world;
    class world::Object;
    
/* -------------------- Various types -------------------- */

    enum Difficulty {
        DIFFICULTY_EASY = 1,
        DIFFICULTY_HARD = 2,
        DIFFICULTY_ANY  = 3
    };


    /*! Enigma can run its own levels but also emulate various
      versions of Oxyd.  All these games behave similarly, but there
      are a few differences in object behaviour, visual appearance,
      etc.  */
    enum GameType {
        GAMET_FIRST,

        GAMET_ENIGMA = GAMET_FIRST,
        GAMET_OXYD1,
        GAMET_PEROXYD,
        GAMET_OXYDEXTRA,
        GAMET_OXYDMAGNUM,

        GAMET_LAST = GAMET_OXYDMAGNUM,
        GAMET_COUNT,
        GAMET_UNKNOWN
    };

    GameType GetGameType(std::string name);
    std::string GetGameTypeName(GameType type);

/* -------------------- Resource Management -------------------- */

    class ImageCache : public ecl::PtrCache<ecl::Surface> {
    public:
        using ecl::PtrCache<ecl::Surface>::store;

        ecl::Surface *acquire(const std::string &name);
    };


/* The `Get...' functions return a pointer to a cached copy of the
   font or image.  The `Load...' functions load a new copy which
   you must deallocate yourself.  */
   

    void DefineFont (const char *name, 
                     const char *ttf_name, 
                     int ttf_size,
                     const char *bmf_name,
                     int r, int g, int b);
    ecl::Font *GetFont (const char *name);

    ecl::Surface *LoadImage (const char *name);
    ecl::Surface *GetImage (const char *name, const char *ext = ".png");
    ecl::Surface *RegisterImage (const char *name, ecl::Surface *s);
    void ClearImageCache();

/* -------------------- Direction, DirectionBits -------------------- */

    enum Direction {
        NODIR = -1,
        WEST  = 0,
        SOUTH = 1,
        EAST  = 2,
        NORTH = 3,
    };

    Direction reverse (Direction d);
    Direction rotate_cw (Direction d);
    Direction rotate_ccw (Direction d);

    std::string to_suffix(Direction d);

    enum DirectionBits {
        NODIRBIT       = 0,
        WESTBIT        = 1 << WEST,
        SOUTHBIT       = 1 << SOUTH,
        EASTBIT        = 1 << EAST,
        NORTHBIT       = 1 << NORTH,
        ALL_DIRECTIONS = 15
    };

    DirectionBits rotate (DirectionBits d, bool clockwise);
    DirectionBits to_bits (Direction d);
    bool has_dir (DirectionBits db, Direction dir);


/* -------------------- Value -------------------- */

    /**
     * A flexible container for various types of data. This class is one
     * of the central Enigma concepts. It is used for Object attributes,
     * for passing arguments and return values in messages and for exchanging
     * values with LUA code.
     * 
     * Object attributes besides the common floor, item, stone, actor traits
     * are usually very specific and only few objects will be aware of
     * them. The Value provides a common container, that allows all objects
     * and the world to pass these attributes without any further knowledge.
     * 
     * Instances of Value can be passed to and from the type free LUA language.
     * Objects can be configured in their attributes and questioned about states
     * via attributes.
     * 
     * Value are converted to required types as far as possible and in a
     * LUA compatible manner.
     * 
     * Due to the history of Enigma and LUA the bool values are not a native
     * type. The value "false" is coded as type "NIL". Every Value besides
     * type "NIL" and "DEFAULT" is interpreted as "true". All attributes 
     * should be declared with a default value of "false". Bool Values can
     * not be assigned directly to a bool variable. Use the to_bool() method
     * for retrieving proper bool values.
     * 
     * A special Value type called "DEFAULT" exists within the C++ engine. It
     * describes a not existing Value. The receiver of this Value should use
     * or request the default behaviour. "DEFAULT" Values are the only Values
     * that are converted to bool "false". All other Values convert to bool 
     * "true". Thus a value can be checked for existance by evaluating it as a
     * bool. This remains true even for Values that contain bool content! A
     * common pattern for handling Values is:
     * 
     * <pre>
     * if (Value v = getAttr("key")) {   // proceed only if attribute exists
     *     int i = v;                    // assign value to an int
     *     std::string s(v);             // create a string out of a value
     *     x = (double)v * 2;            // cast v to a desired type
     *     double d = getAttr("x", 2);   // get a value with a default of 2.0
     * }
     * </pre>
     */
    class Value {
    public:
        /**
         * Specifier for the current type of the value content.
         */
        enum Type { 
            DEFAULT,  ///< Pseudotype for a not existing attribute that should
                      ///< cause the default behaviour of the object
            NIL,      ///< Value that is equivalent to Lua "nil". It represents
                      ///< an invalid value
            BOOL,     ///< Value that represents bool values "true" and "false".
            DOUBLE,   ///< Value is numerical and can take a "double". It is used
                      ///< for other numericals values like "int", too.
            STRING,   ///< Value is a string. Such a string may encode another
                      ///< type that has no native representation in Value
            OBJECT,    ///< Value is an object id. The id is a persistent object
                      ///< identifier.
            GROUP     ///< Value is a group of objects.
        };

        Value();                       ///< Constructor for NIL value that 
                                       ///< represents an invalid value
        Value(double d);               ///< Constructor for DOUBLE value
        Value(const char* str);        ///< Constructor for STRING value. The
                                       ///< given string is duplicated
        Value(const std::string& str); ///< Constructor for STRING value. The
                                       ///< given string is duplicated
        Value(int i);                  ///< Constructor for DOUBLE value
        Value(bool b);                 ///< Constructor for BOOL value
        Value(Object *obj);            ///< Constructor for OBJECT value that properly
                                       ///< represents a persistent reference to an object
        Value(Type t);                 ///< Constructor for a given type. The
                                       ///< value defaults to 0.0 or ""
        ~Value();

        Value(const Value& v);                 ///< Explicit copy constructor that
                                               ///< performs a deep copy
        Value& operator=(const Value& v);      ///< Explicit copy assignment that
                                               ///< performs a deep copy
        /**
         * Compare values in type and value. This compare is a LUA like
         * compare of values. Note that a DOUBLE value of 1.0 does not equal
         * a STRING value of "1.0" even though both values are seamless casted
         * to the same double and result in the same numerical calculation results.
         * 
         * If you want to compare two values that express numerical data independent
         * of their value type use the pattern:
         * <pre>
         *     (double) value1 == (double) value2
         * </pre>
         */
        bool operator==(const Value& v) const;
        bool operator!=(const Value& v) const; ///< Compare values in type and value
        bool operator==(int i) const;          ///< Compare value with int without casting.
                                               ///< f.e <code>if (v == 2)</code>
        bool operator!=(int i) const;          ///< Compare value with int without casting
        
        /**
         * Test of a value for explicit existence besides a default.
         * This conversion returns "false" if the value is of type DEFAULT.
         * This happens when a "getAttr()" call did not find a concrete value.
         * All other value types that represent explicit given values return "true".
         * The main usage is the common pattern <code>if (Value v = getAttr("key"))</code>.
         * 
         * Note: this conversion does not return the result of a boolean stored
         * in the value. If a concrete value contains a boolean this test
         * returns always "true", even if the boolean is "false" and the value
         * is of type NIL.
         */
        operator bool() const;
        
        /**
         * Conversion of a value to a double. String values are interpreted as
         * a double like it is done by LUA. All other values default to a double
         * value of 0.0.
         */
        operator double() const;
         
        /**
         * Conversion of a value to a int. String values are interpreted as
         * a int like it is done by LUA. All other values default to a int
         * value of 0.
         */
        operator int() const;
        
        /**
         * Conversion of a value to an object reference. All values besides valid
         * object values default to a NULL reference.
         */
        operator Object *() const;
        
        /**
         * Conversion of a value to a <code>char *</code> just for initialization
         * of a std::string. Numerical values are converted to a string like it
         * id done by LUA. All other values default to an empty string.
         * 
         * Note that the returned pointer may be volatile and
         * cannot be used for any further usage besides immediate initialization of
         * a string object. For all other purposes use the <code>to_string()</code> method to
         * receive a non-volatile copy of the string.
         */
        operator const char*() const;

        void assign(double d);       ///< Reset value to a DOUBLE
        void assign(const char* s);  ///< Reset value to a STRING with a copy of
                                     ///< the given string

        Type    getType() const;               ///< Returns the current value type
        double  get_double() const throw();     ///< Returns the current double value without
                                                ///< any conversion if it is a DOUBLE.
                                                ///< On type mismatch a XLevelRuntime is thrown
        const char* get_string() const throw(); ///< Returns the current string value without
                                                ///< any conversion if it is a STRING.
                                                ///< On type mismatch a XLevelRuntime is thrown
        bool isDefault() const;                 ///< Returns true if type is DEFAULT
        
        /**
         * Returns a std::string with convertion of numerical values. All other
         * values default to an empty string.
         */
        std::string to_string() const;
        
        /**
         * Returns the LUA compatible boolean representation of the value.
         * <code>false</code> is returned for a NIL value and <code>true</code>
         * for any other value.
         */
        bool to_bool() const;
    private:
        void clear();      ///< Release resources and assign type NIL

        // Variables
        Type type;
        union {
            double dval;
            char* str;
        } val;
    };


    bool        to_bool(const Value &v);        ///< Synonym for v.to_bool()
    int         to_int(const Value &v);         ///< Synonym for (int)v
    double      to_double(const Value &v);      ///< Synonym for (double)v
    std::string to_string(const Value &v);      ///< Synonym for v.to_string()
    Direction   to_direction (const Value &v);  ///< Casting of value to Direction

/* -------------------- Timers -------------------- */

    /* Interface for time event handlers. */
    class TimeHandler {
    public:
        virtual ~TimeHandler() {}
        virtual void tick (double /*dtime*/) {}
        virtual void alarm() {}
    };

/* -------------------- GridPos -------------------- */

    struct GridPos {
        // Variables
        int x, y;

        // Methods
        explicit GridPos(int xx=0, int yy=0);
        explicit GridPos(const ecl::V2& p);
        void move(Direction dir);
        ecl::V2 center() const;
    };

    GridPos move(GridPos p, Direction dir);
    GridPos move(GridPos p, Direction dir, Direction dir2);
    bool operator== (GridPos a, GridPos b);
    bool operator != (GridPos a, GridPos b);
    bool operator< (GridPos a, GridPos b);

    /* 516
       203
       748 */
    GridPos get_neighbour (GridPos p, int i);

    // source and target have to be adjacent
    Direction direction_fromto(GridPos source, GridPos target); 

    std::ostream& operator<<(std::ostream& os, const GridPos& val);


    // ---------- GridLayer ----------

    enum GridLayer {
        GRID_FLOOR, 
        GRID_ITEMS, 
        GRID_STONES, 
        GRID_COUNT
    };

    enum GridLayerBits {
        GRID_FLOOR_BIT  = 1,
        GRID_ITEMS_BIT  = 2,
        GRID_STONES_BIT = 4
    };

    // ---------- GridLoc ----------

    struct GridLoc {
        // Variables
        GridPos pos;
        GridLayer layer;

        // Constructor
        GridLoc(GridLayer l = GRID_FLOOR, GridPos p = GridPos());
    };

    /*! Converts strings like "it(10 10)", "st(5 2)" to GridLoc
      structures. */
    bool to_gridloc (const char *str, GridLoc &loc);


/* -------------------- Random Numbers -------------------- */

    void   Randomize ();
    void   Randomize (unsigned seed);
    int    IntegerRand (int min, int max);
    double DoubleRand (double min, double max);

/* -------------------- Time & Date -------------------- */

    const char *date(const char *format); // format see 'man strftime'


    /* ==================== Inline definitions ==================== */

    inline DirectionBits to_bits(Direction d) {
        if (d==NODIR)
            return NODIRBIT;
        return DirectionBits(1 << d);
    }

    inline bool has_dir(DirectionBits db, Direction dir) {
        return (db & to_bits(dir)) != 0;
    }


    // ---------- GridPos ----------

    inline GridPos::GridPos(int xx, int yy) 
    : x(xx), y(yy) 
    {}


    inline void GridPos::move(Direction dir) {
        switch(dir) {
        case NORTH: y--; break; 
        case SOUTH: y++; break;
        case EAST: x++; break; 
        case WEST: x--; break;
        case NODIR: break;
        }
    }

    inline ecl::V2 GridPos::center() const { 
        return ecl::V2(x+.5, y+.5); 
    }


    inline GridPos move(GridPos p, Direction dir) {
        GridPos tmp = p;
        tmp.move(dir);
        return tmp;
    }

    inline GridPos move(GridPos p, Direction dir, Direction dir2) {
        GridPos tmp = p;
        tmp.move(dir);
        tmp.move(dir2);
        return tmp;
    }

    inline bool operator == (GridPos a, GridPos b) { 
        return (a.x==b.x && a.y==b.y); 
    }
  
    inline bool operator != (GridPos a, GridPos b) { 
        return (a.x!=b.x || a.y!=b.y); 
    }

    inline bool operator< (GridPos a, GridPos b) {
        return ((a.y<<16) + a.x) < ((b.y<<16) + b.x);
    }


    // ---------- GridLoc ---------- 

    inline GridLoc::GridLoc(GridLayer l, GridPos p) 
    : pos(p), layer(l) 
    {}

}

#endif
