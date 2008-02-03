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
 
#ifndef KINDDESCRIPTOR_HH_INCLUDED
#define KINDDESCRIPTOR_HH_INCLUDED

#include "AttributeDescriptor.hh"
#include "MessageDescriptor.hh"
#include <map>
#include <string>

namespace enigma
{
    class KindDescriptor {
    public:
        KindDescriptor(std::string name, std::string superKind, bool abstract, int states, bool initOnly);
        void addMessage(std::string msg);
        bool validateMessage(std::string msg);
        void addAttribute(std::string name);
        AttributeDescriptor * addModifiedAttribute(std::string name);
        bool validateAttributeWrite(std::string key, Value val);
        void log();
    private:
        std::string kind;
        KindDescriptor *super;
        bool isAbstract;  // object instantiation not possible
        bool isClass;     // 
        bool isInitOnly;  // isKind is never true 
        int numStates;
        std::map<std::string, MessageDescriptor *> messages;
        std::map<std::string, AttributeDescriptor *> attributes;
    };
} // namespace enigma
#endif
