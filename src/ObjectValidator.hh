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
 
#ifndef OBJECTVALIDATOR_HH_INCLUDED
#define OBJECTVALIDATOR_HH_INCLUDED

#include "KindDescriptor.hh"
#include "AttributeDescriptor.hh"
#include "MessageDescriptor.hh"
#include "Object.hh"
#include "Value.hh"
#include <list>
#include <map>
#include <string>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>

namespace enigma
{
    /**
     * 
     * - distinguish message vs. attribute names for lua
     * - ignore unknown messages - free classes from task to block unsupported messages
     * - rw access check for attributes on behalf of classes
     * - supply default attribute values if not set
     * - delete attribute or reset to default on nil setting
     * - getKind evaluation of special subKind conditions
     * - isKind checking against arbitrary kind names
     */
    class ObjectValidator {
    public:
        
        static ObjectValidator *instance();
        static void didInitXML();
        
        ~ObjectValidator();
        void shutdown();
        
        KindDescriptor *getKindDesc(std::string name);
        MessageDescriptor *getMessage(std::string name);
        bool validateMessage(Object *obj, std::string msg, Value val);
        AttributeDescriptor *getDefaultAttributeDesc(std::string name);
        AttributeDescriptor *cloneAttribute(AttributeDescriptor *masterAttr);
        bool validateAttributeWrite(const Object *obj, std::string key, Value val);
        bool validateAttributeRead(const Object *obj, std::string key);
        Value getDefaultValue(const Object *obj, std::string key);
        std::string getKind(const Object *obj);
        bool isKind(const Object *obj, std::string match);
        
    protected:
        ObjectValidator();
    private:
        static ObjectValidator *theSingleton;
        static bool mayInitialize;
        
        void init();
        void scanAttributeElement(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *attribElement);
        void scanMessageElement(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *msgElement);
        void scanObjectElement(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *objectElement);
        Value parseTypedValue(const XMLCh* xmlSource, const std::string &strSource, validationType type);
        
        std::map<std::string, KindDescriptor *> kinds;
        std::map<std::string, MessageDescriptor *> messages;
        std::map<std::string, AttributeDescriptor *> defaultAttributes;
        std::list<AttributeDescriptor *> otherAttributes;
        
        XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc;
    };
} // namespace enigma
#endif

