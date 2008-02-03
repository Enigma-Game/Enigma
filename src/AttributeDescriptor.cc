/*
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

#include "AttributeDescriptor.hh"

#include "ObjectValidator.hh"

namespace enigma {
    
    AttributeDescriptor::AttributeDescriptor(std::string attributeName, validationType valType, 
            Value aValue, bool allowRead, bool allowWrite, Value minVal, Value maxVal)
            : name (attributeName), type (valType), defaultValue (aValue), 
            readable (allowRead), writable(allowWrite), min (minVal), max (maxVal) {
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
    
    Value AttributeDescriptor::getValue() {
        return value;
    }
    
    void AttributeDescriptor::setReadable(bool allowRead) {
        readable = allowRead;
    }
    
    void AttributeDescriptor::setWritable(bool allowWrite) {
        writable = allowWrite;
    }
    
    void AttributeDescriptor::setDefaultValue(const Value &newDefault) {
        defaultValue = newDefault;
    }

    void AttributeDescriptor::setMinValue(const Value &newMin) {
        min = newMin;
    }
    
    void AttributeDescriptor::setMaxValue(const Value &newMax) {
        max = newMax;
    }
    
    void AttributeDescriptor::setValue(const Value &newValue) {
        value = newValue;
    }
    
} // namespace enigma
