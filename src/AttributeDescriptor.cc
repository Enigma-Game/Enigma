/*
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

#include "AttributeDescriptor.hh"

#include "errors.hh"
#include "ObjectValidator.hh"

namespace enigma {
    
    AttributeDescriptor::AttributeDescriptor(std::string attributeName, validationType valType, 
            Value aValue, bool allowRead, bool allowWrite, Value minVal, Value maxVal)
            : name (attributeName), type (valType), defaultValue (aValue), 
            readable (allowRead), writable(allowWrite), min (minVal), max (maxVal), isKindValue (false) {
    }
    
    std::string AttributeDescriptor::getName() {
        return name;
    }
    
    validationType AttributeDescriptor::getType() {
        return type;
    }
    
    bool AttributeDescriptor::isReadable() {
        return readable;
    }
    
    bool AttributeDescriptor::isWritable() {
        return writable;
    }
    
    Value AttributeDescriptor::getDefaultValue() {
        return defaultValue;
    }
    
    Value AttributeDescriptor::getValue() {
        return value;
    }
    
    ValidationResult AttributeDescriptor::checkValue(Value val) {
        switch (type) {
            case VAL_BOOL :
                return val.getType() == Value::BOOL ? VALID_OK : VALID_TYPE_MISMATCH;
                break;
            case VAL_INT :
                // TBD add int, min, max check
                if (name != "code")
                    return val.getType() == Value::DOUBLE  ? VALID_OK : VALID_TYPE_MISMATCH;
                else
                    return (val.getType() == Value::DOUBLE || val.getType() == Value::STRING)  ? 
                            VALID_OK : VALID_TYPE_MISMATCH;                    
                break;
            case VAL_DOUBLE :
                return val.getType() == Value::DOUBLE  ? VALID_OK : VALID_TYPE_MISMATCH;
                break;
            case VAL_STRING :
                return val.getType() == Value::STRING  ? VALID_OK : VALID_TYPE_MISMATCH;
                break;
            case VAL_TOKENS : {
                Value::Type vt = val.getType();
                bool result = (vt == Value::STRING || vt == Value::TOKENS || vt == Value::OBJECT || vt == Value::GROUP);
                if (name == "destination")
                    return (result || vt == Value::POSITION)  ? VALID_OK : VALID_TYPE_MISMATCH;
                else
                    return result  ? VALID_OK : VALID_TYPE_MISMATCH;
                break;
            }
            default :
                return VALID_OK;
        }
    }
    
    void AttributeDescriptor::setReadable(bool allowRead) {
        ASSERT(!isKindValue, XFrontend, 
                ecl::strf("Object description initialization error - attribute '%s' made readable for a subkind",
                name.c_str()).c_str());
        readable = allowRead;
    }
    
    void AttributeDescriptor::setWritable(bool allowWrite) {
        ASSERT(!isKindValue, XFrontend, 
                ecl::strf("Object description initialization error - attribute '%s' made writable for a subkind",
                name.c_str()).c_str());
        writable = allowWrite;
    }
    
    void AttributeDescriptor::setDefaultValue(const Value &newDefault) {
        ASSERT(!isKindValue, XFrontend, 
                ecl::strf("Object description initialization error - attribute '%s' new default for a subkind",
                name.c_str()).c_str());
        defaultValue = newDefault;
    }

    void AttributeDescriptor::setMinValue(const Value &newMin) {
        ASSERT(!isKindValue, XFrontend, 
                ecl::strf("Object description initialization error - attribute '%s' new min for a subkind",
                name.c_str()).c_str());
        min = newMin;
    }
    
    void AttributeDescriptor::setMaxValue(const Value &newMax) {
        ASSERT(!isKindValue, XFrontend, 
                ecl::strf("Object description initialization error - attribute '%s' new max for a subkind",
                name.c_str()).c_str());
        max = newMax;
    }
    
    void AttributeDescriptor::setValue(const Value &newValue) {
        ASSERT(isKindValue, XFrontend, 
                ecl::strf("Object description initialization error - attribute '%s' value for a class or abstract kind",
                name.c_str()).c_str());
        value = newValue;
    }
    
    void AttributeDescriptor::limitToKindValue() {
        isKindValue = true;
    }
    
} // namespace enigma
