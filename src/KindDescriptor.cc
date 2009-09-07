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

#include "KindDescriptor.hh"

#include "errors.hh"
#include "main.hh"
#include "ObjectValidator.hh"

namespace enigma {
    KindDescriptor::KindDescriptor(std::string name, std::string superKind, bool abstract, 
            int states, bool initOnly) : kind (name), isAbstract (abstract), 
            isInitOnly (initOnly), numStates (states) {
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
            
        if (super != NULL)
            super->registerSubKind(this);
    }
    
    void KindDescriptor::addMessage(std::string msg) {
        ASSERT(isAbstract || isClass, XFrontend, 
                ecl::strf("Object description initialization error - message '%s' addition to subkind '%s'",
                msg.c_str(), kind.c_str()).c_str());
        if (messages[msg] == NULL)
            messages[msg] = ObjectValidator::instance()->getMessage(msg);
    }
    
    bool KindDescriptor::validateMessage(std::string msg) {
        std::map<std::string, MessageDescriptor *>::iterator it = messages.find(msg);
        return it != messages.end();
    }
    
    void KindDescriptor::addAttribute(std::string name) {
        ASSERT(isAbstract || isClass, XFrontend, 
                ecl::strf("Object description initialization error - attribute '%s' addition to subkind '%s'",
                name.c_str(), kind.c_str()).c_str());
        if (attributes[name] == NULL)
            attributes[name] = ObjectValidator::instance()->getDefaultAttributeDesc(name);
    }
    
    AttributeDescriptor * KindDescriptor::addModifiedAttribute(std::string name) {
        // make a clone and return it for modification
        AttributeDescriptor *inheritedAttribute = attributes[name];
        if (inheritedAttribute == NULL) {
            ASSERT(isAbstract || isClass, XFrontend, 
                    ecl::strf("Object description initialization error - attribute '%s' addition to subkind '%s'",
                    name.c_str(), kind.c_str()).c_str());
            inheritedAttribute = ObjectValidator::instance()->getDefaultAttributeDesc(name);
        }
        AttributeDescriptor *cloneAttribute = ObjectValidator::instance()->cloneAttribute(inheritedAttribute);
        if (!isAbstract && !isClass) {
            cloneAttribute->limitToKindValue();
            valuedAttributes[name] = cloneAttribute;
        }
        attributes[name] = cloneAttribute;
        return cloneAttribute;
    }
    
    ValidationResult KindDescriptor::validateAttributeWrite(std::string key, Value val) {
        std::map<std::string, AttributeDescriptor *>::iterator it = attributes.find(key);
        if (it == attributes.end())
            return VALID_UNKNOWN_KEY;
        else if (!it->second->isWritable())
            return VALID_ACCESS_DENIED;
        else if (val.getType() == Value::NIL)  // reset to default value
            return VALID_OK;
        else
            return it->second->checkValue(val);
    }
    
    bool KindDescriptor::validateAttributeRead(std::string key) {
        std::map<std::string, AttributeDescriptor *>::iterator it = attributes.find(key);
        if (it == attributes.end())
            return false;
        else
            return it->second->isReadable();
    }
    
    Value KindDescriptor::getDefaultValue(std::string key) {
        std::map<std::string, AttributeDescriptor *>::iterator it = attributes.find(key);
        if (it == attributes.end())
            return Value(Value::DEFAULT);
        else {
            Value v = it->second->getDefaultValue();
            if (v.getType() == Value::NIL)
                return Value(Value::DEFAULT);
            else
                return v;
        }
    }
    
    std::string KindDescriptor::getKind(const Object *obj) {
        for(std::list<KindDescriptor *>::iterator it = subKinds.begin(); it != subKinds.end(); ++it) {
            if (!(*it)->isInitOnly && (*it)->validateObject(obj))
                return (*it)->kind;
        }
        return kind;
    }
    
    bool KindDescriptor::isKind(const Object *obj, std::string match) {
        if (match == kind)
            return true;
        else {
            // is it a super kind name?
            for (KindDescriptor *s = super; s != NULL; s = s->super)
                if (match == s->kind)
                    return true;
                    
            // is it a special subkind?
            if (match == getKind(obj))
                return true;
        }
        return false;
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
    
    void KindDescriptor::registerSubKind(KindDescriptor *aKind) {
        subKinds.push_back(aKind);
    }
    
    bool KindDescriptor::validateObject(const Object *obj) {
        bool result = true;
        for (std::map<std::string, AttributeDescriptor *>::iterator it = valuedAttributes.begin();
                it != valuedAttributes.end(); ++it) {
            if ( it->second->getValue() != obj->getAttr(it->second->getName()))
                return false;
        }
        return result;
    }
} // namespace enigma
