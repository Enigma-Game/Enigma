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

#include "KindDescriptor.hh"

#include "errors.hh"
#include "main.hh"
#include "ObjectValidator.hh"

namespace enigma {
    KindDescriptor::KindDescriptor(std::string name, std::string superKind, bool abstract, 
            int states, bool initOnly) : kind (name), isAbstract (abstract), 
            isInitOnly (isInitOnly), numStates (states) {
        // find super kind
        if (superKind != "-") {
            // an explicit super kind is given
            super = ObjectValidator::instance()->getKindDesc(superKind);
        } else {
            // implicit super kind by suffix cut off
            std::string::size_type pos = name.rfind('_');
            if (pos != std::string::npos)
                super = ObjectValidator::instance()->getKindDesc( name.substr(0, pos));
            else
                super = ObjectValidator::instance()->getKindDesc("");
        }
        ASSERT(super != NULL || name == "", XFrontend, "Object description initialization error - unknown super kind");
        // isClass - if all super are abstract
        isClass = !abstract;
        for (KindDescriptor *s = super; s != NULL; s = s->super)
            if (!s->isAbstract)
                isClass = false;

        // inherit messages from super
        if (super != NULL)
            messages = super->messages;

        // inherit attributes from super
        if (super != NULL)
            attributes = super->attributes;
    }
    
    void KindDescriptor::addMessage(std::string msg) {
        if (messages[msg] == NULL)
            messages[msg] = ObjectValidator::instance()->getMessage(msg);
    }
    
    bool KindDescriptor::validateMessage(std::string msg) {
        std::map<std::string, MessageDescriptor *>::iterator it = messages.find(msg);
        return it != messages.end();
    }
    
    void KindDescriptor::addAttribute(std::string name) {
        if (attributes[name] == NULL)
            attributes[name] = ObjectValidator::instance()->getDefaultAttribute(name);
    }
    
    AttributeDescriptor * KindDescriptor::addModifiedAttribute(std::string name) {
        // make a clone and return it for modification
        AttributeDescriptor *inheritedAttribute = attributes[name];
        if (inheritedAttribute == NULL)
            inheritedAttribute = ObjectValidator::instance()->getDefaultAttribute(name);
        AttributeDescriptor *cloneAttribute = ObjectValidator::instance()->cloneAttribute(inheritedAttribute);
        attributes[name] = cloneAttribute;
        return cloneAttribute;
    }
    
    bool KindDescriptor::validateAttributeWrite(std::string key, Value val) {
        std::map<std::string, AttributeDescriptor *>::iterator it = attributes.find(key);
        if (it == attributes.end())
            return false;
        else
            return it->second->isWritable();
    }
    
    void KindDescriptor::log() {
        if (super != NULL)
            Log << "Kind " << kind << " of super " << super->kind << " isClass " << isClass << "\n";
        else
            Log << "Kind " << kind << " is base - isClass " << isClass << "\n";
        for (std::map<std::string, MessageDescriptor *>::iterator it = messages.begin(); it != messages.end(); ++it)
            Log << "    Message: " << it->first << "\n";
        for (std::map<std::string, AttributeDescriptor *>::iterator it = attributes.begin(); it != attributes.end(); ++it) {
            Log << "    Attribute: " << it->first << " - ";
            switch (it->second->getType()) {
                case VAL_BOOL :
                    Log << "bool - ";
                    break;
                case VAL_NIL :
                    Log << "nil - ";
                    break;
                case VAL_STRING :
                    Log << "string - ";
                    break;
                case VAL_INT :
                    Log << "int - ";
                    break;
                case VAL_DIR :
                    Log << "dir - ";
                    break;
                case VAL_ENUM :
                    Log << "enum - ";
                    break;
                case VAL_DOUBLE :
                    Log << "double - ";
                    break;
                case VAL_TOKENS :
                    Log << "tokens - ";
                    break;
            }
            if (it->second->isReadable()) Log << "r";
            if (it->second->isWritable()) Log << "w";
            if (it->second->getType() == VAL_INT && it->second->getValue() != Value())
                Log << " : " << (int)(it->second->getValue());
            Log << "\n";
        }
    }
} // namespace enigma
