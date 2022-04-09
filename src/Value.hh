/*
 * Copyright (C) 2002,2004 Daniel Heck
 * Copyright (C) 2007,2008,2009 Ronald Lamprecht
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
#ifndef VALUE_HH
#define VALUE_HH

// This file contains declarations for facilities used by several different
// parts of the program, like common types and constants, and routines for
// resource management.

#include "enigma.hh"
#include <list>

namespace enigma { 

    class Object;
    class Value;
   
    typedef std::list<Object *> ObjectList;
    typedef std::list<Value> TokenList;
    typedef std::list<Value> PositionList;

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
     * Bool Values can not be assigned directly to a bool variable. Use the
     * to_bool() method for retrieving proper bool values.
     * 
     * A special Value type called "DEFAULT" exists within the C++ engine. It
     * describes a not explicitly set Value. The receiver of this Value should use
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
                      ///< for other numerical values like "int", too.
            STRING,   ///< Value is a string. Such a string may encode another
                      ///< type that has no native representation in Value
            OBJECT,   ///< Value is an object id. The id is a persistent object
                      ///< identifier.
            GROUP,    ///< Value is a group of objects.
            POSITION, ///< Value is a position
            GRIDPOS,  ///< Value is a grid position
            TOKENS,   ///< Value is a list of token values
            NAMEDOBJECT  ///< internal Value type for named objects - public type is OBJECT
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
        Value(ObjectList aList);       ///< Constructor for OBJECT value that properly
                                       ///< represents a set of objects
        Value(TokenList aList);        ///< Constructor for TOKENS value that properly
                                       ///< represents a list of tokens
        Value(ecl::V2 pos);            ///< Constructor for POSITION value
        Value(GridPos gpos);           ///< Constructor for POSITION value
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
        bool operator<=(int i) const;          ///< Compare value with int without casting
        bool operator>=(int i) const;          ///< Compare value with int without casting
        
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
         * Conversion of a value to an object set.
         */
        operator ObjectList() const;
        
        /**
         * Conversion of a value to a list of tokens.
         */
        operator TokenList() const;
        
        /**
         * Conversion of a value to a position vector.
         */
        operator ecl::V2() const;
        
        /**
         * Conversion of a value to a grid position.
         */
        operator GridPos() const;
        
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

        /**
         * Returns <code>true</code> if and only if the value is a string and
         * starts with a <code>%</code> character. In that case, the string
         * might origin from autonumbering.
         */
        bool maybe_autonumbered() const;

        /**
         * Returns the value converted to a vector position with a centering of all
         * grid position type values. GRIDPOS type values as well as all positions of
         * GridObject's will be return as grid centered vector positions, while true
         * vector position values and positions of Actor's will be returned as is.
         * Other values will return a position of (-1, -1) indicating a false position 
         */
        ecl::V2 centeredPos() const;
        
        /**
         * 
         */
        ObjectList getObjectList(Object *reference = nullptr) const;
        PositionList getPositionList(Object *reference =nullptr) const;
        bool finalizeNearestObjectReference(Object *reference = nullptr);
        bool maybeNearestObjectReference() const;
    private:
        void clear();      ///< Release resources and assign type NIL

        // Variables
        Type type;
        union {
            double dval[2];
            char* str;
        } val;
    };


    bool        to_bool(const Value &v);        ///< Synonym for v.to_bool()
    int         to_int(const Value &v);         ///< Synonym for (int)v
    double      to_double(const Value &v);      ///< Synonym for (double)v
    std::string to_string(const Value &v);      ///< Synonym for v.to_string()
    Direction   to_direction (const Value &v);  ///< Casting of value to Direction

} // namespace enigma

#endif
