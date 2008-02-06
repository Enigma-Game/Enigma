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
 */
 
#ifndef ATTRIBUTEDESCRIPTOR_HH_INCLUDED
#define ATTRIBUTEDESCRIPTOR_HH_INCLUDED

#include "Value.hh"
#include <string>

namespace enigma
{
    enum validationType {
        VAL_BOOL,
        VAL_INT,
        VAL_DOUBLE,
        VAL_NIL,
        VAL_DIR,
//        VAL_POS,
        VAL_STRING,
        VAL_ENUM,
        VAL_TOKENS
    };    

    class AttributeDescriptor {
    public:
        AttributeDescriptor(std::string name, validationType valType, Value defaultValue,
                bool allowRead, bool allowWrite, Value minVal, Value maxVal);
        std::string getName();
        validationType getType();
        bool isReadable();
        bool isWritable();
        Value getDefaultValue();
        Value getValue();
        void setReadable(bool allowRead);
        void setWritable(bool allowWrite);
        void setDefaultValue(const Value &newDefault);
        void setMinValue(const Value &newMin);
        void setMaxValue(const Value &newMax);
        void setValue(const Value &newValue);
        void limitToKindValue();

    private:
        std::string name;
        validationType type;
        Value defaultValue;
        bool readable;
        bool writable;
        Value  min;
        Value  max;
        Value value;
        bool isKindValue;
    };
} // namespace enigma
#endif
