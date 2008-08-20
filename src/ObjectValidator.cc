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

#include "ObjectValidator.hh"

#include "errors.hh"
#include "main.hh"
#include "DOMErrorReporter.hh"
#include "DOMSchemaResolver.hh"
#include "utilXML.hh"
#include "Utf8ToXML.hh"
#include "XMLtoUtf8.hh"
#include "gui/ErrorMenu.hh"
#include <iostream>
#include <sstream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLDouble.hpp>

XERCES_CPP_NAMESPACE_USE 

namespace enigma {
    ObjectValidator *ObjectValidator::theSingleton = NULL;
    bool ObjectValidator::mayInitialize = false;
    
    void ObjectValidator::didInitXML() {
        mayInitialize = true;
    }
    
    ObjectValidator* ObjectValidator::instance() {
        // autoinitialize on first access, but block any access during 
        // bootstraping as XML cannot be loaded at this stage. This may
        // and shall cause NULL pointer exceptions on inproper usage 
        // in basic object initialization!
        if (theSingleton == NULL && mayInitialize) {
            theSingleton = new ObjectValidator();
            theSingleton->init();
        }
        return theSingleton;
    }
    
    ObjectValidator::ObjectValidator() {
        std::string objectsPath;
        std::string errMessage;
        
        if (!app.systemFS->findFile( "schemas/objects.xml" , objectsPath)) {
                throw XFrontend("Cannot load object declarations!");
        }

        try {
            std::ostringstream errStream;
            app.domParserErrorHandler->resetErrors();
            app.domParserErrorHandler->reportToOstream(&errStream);
            app.domParserSchemaResolver->resetResolver();
            app.domParserSchemaResolver->addSchemaId("objects.xsd","objects.xsd");

            doc = app.domParser->parseURI(objectsPath.c_str());
            if (doc != NULL && !app.domParserErrorHandler->getSawErrors()) {
                
            }
            if(app.domParserErrorHandler->getSawErrors()) {
                errMessage = errStream.str();
            }
            app.domParserErrorHandler->reportToNull();  // do not report to errStream any more
        }
        catch (...) {
            errMessage = "Unexpected XML Exception on load of object declarations\n";
        }
        if (!errMessage.empty()) {
            throw XFrontend("Cannot load object declarations file: " + objectsPath +
                    "\nError: " + errMessage);
        }
    }
     
    ObjectValidator::~ObjectValidator() {
        if (doc != NULL)
            shutdown();
        for (std::map<std::string, KindDescriptor *>::iterator it = kinds.begin(); it != kinds.end(); ++it)
            delete it->second;
        for (std::map<std::string, MessageDescriptor *>::iterator it = messages.begin(); it != messages.end(); ++it)
            delete it->second;
        for (std::map<std::string, AttributeDescriptor *>::iterator it = defaultAttributes.begin(); it != defaultAttributes.end(); ++it)
            delete it->second;
        while (!otherAttributes.empty()) {
            delete otherAttributes.back();
            otherAttributes.pop_back();
        }
    }
    
    void ObjectValidator::init() {
        ASSERT(doc != NULL, XFrontend, "ObjectValidator try to init without given document");
        
        std::string errMessage;
        DOMNodeList *elemList;
        DOMElement *baseElem;
        
        try{
            baseElem = dynamic_cast<DOMElement *>(doc->getElementsByTagName(Utf8ToXML("attributes").x_str())->item(0));
            elemList = baseElem->getElementsByTagName(Utf8ToXML("attr").x_str());
            for (int i = 0, l = elemList->getLength(); i < l; i++) {
                scanAttributeElement(dynamic_cast<DOMElement *>(elemList->item(i)));
            }
            
            baseElem = dynamic_cast<DOMElement *>(doc->getElementsByTagName(Utf8ToXML("messages").x_str())->item(0));
            elemList = baseElem->getElementsByTagName(Utf8ToXML("msg").x_str());
            for (int i = 0, l = elemList->getLength(); i < l; i++) {
                scanMessageElement(dynamic_cast<DOMElement *>(elemList->item(i)));
            }
            
            elemList = doc->getElementsByTagName(Utf8ToXML("object").x_str());
            for (int i = 0, l = elemList->getLength(); i < l; i++) {
                scanObjectElement(dynamic_cast<DOMElement *>(elemList->item(i)));
            }
        } catch (...) {
            errMessage = "Exception on parse of object declarations\n";
        }
        if (!errMessage.empty()) {
            throw XFrontend("Cannot init object declarations file: \nError: " + errMessage);
        }
    }
    
    void ObjectValidator::shutdown() {
        if (doc != NULL)
            doc->release();
        doc = NULL;
    }
    
    KindDescriptor *ObjectValidator::getKindDesc(std::string name) {
        std::map<std::string, KindDescriptor *>::iterator it = kinds.find(name);
        if (it != kinds.end())
            return it->second;
        else
            return NULL;
    }
    
    MessageDescriptor *ObjectValidator::getMessage(std::string name) {
        std::map<std::string, MessageDescriptor *>::iterator it = messages.find(name);
        if (it != messages.end())
            return it->second;

        ASSERT(false, XFrontend, ("ObjectValidator missing message declaration for: " + name).c_str());
        return NULL;
    }
    
    bool ObjectValidator::validateMessage(Object *obj, std::string msg, Value val) {
        KindDescriptor *kind = getKindDesc(obj->getClass());
        if (kind == NULL) {
            // object is not under validator control
            if (obj->getAttr(msg))
                return false;       // default attribute value - try message
            else
                return true;        // existing attribute value - deny message
        }
        return kind->validateMessage(msg);
    }

    AttributeDescriptor *ObjectValidator::getDefaultAttributeDesc(std::string name) {
        std::map<std::string, AttributeDescriptor *>::iterator it = defaultAttributes.find(name);
        if (it != defaultAttributes.end())
            return it->second;

        ASSERT(false, XFrontend, ("ObjectValidator missing default attribute declaration for: " + name).c_str());
        return NULL;
    }
    
    AttributeDescriptor *ObjectValidator::cloneAttribute(AttributeDescriptor *masterAttr) {
        AttributeDescriptor *clone = new AttributeDescriptor(*masterAttr);
        otherAttributes.push_back(clone);
        return clone;
    }

    bool ObjectValidator::validateAttributeWrite(const Object *obj, std::string key, Value val) {
        KindDescriptor *kind = getKindDesc(obj->getClass());
        if (kind == NULL) {
            return true;    // object is not under validator control - allow write
        }
        // state dependent target/action support
        if (key.find("target_") == 0)
            key = "target";
        else if (key.find("action_") == 0)
            key = "action";

        return kind->validateAttributeWrite(key, val);
    }
    
    bool ObjectValidator::validateAttributeRead(const Object *obj, std::string key) {
        KindDescriptor *kind = getKindDesc(obj->getClass());
        if (kind == NULL) {
            return true;    // object is not under validator control
        }
        // state dependent target/action support
        if (key.find("target_") == 0)
            key = "target";
        else if (key.find("action_") == 0)
            key = "action";

        return kind->validateAttributeRead(key);
    }
    
    Value ObjectValidator::getDefaultValue(const Object *obj, std::string key) {
        static bool recursive = false;
                
        // block potential recursions
        if (recursive)
            return Value(Value::DEFAULT);
            
        recursive = true;
        KindDescriptor *kind = getKindDesc(obj->getClass());  // getClass is critical if it uses attributes
        recursive = false;
        
        if (kind == NULL) {
            return Value(Value::DEFAULT);    // object is not under validator control
        }
        
        // state dependent target/action support
        if (key.find("target_") == 0)
            key = "target";
        else if (key.find("action_") == 0)
            key = "action";

        return kind->getDefaultValue(key);
    }
    
    std::string ObjectValidator::getKind(const Object *obj) {
        std::string className = obj->getClass();
        KindDescriptor *kind = getKindDesc(className);
        if (kind == NULL) {
            return className;    // object is not under validator control
        }
        return kind->getKind(obj);
    }
    
    bool ObjectValidator::isKind(const Object *obj, std::string match) {
        KindDescriptor *kind = getKindDesc(obj->getClass());
        if (kind == NULL) {
            return false;    // object is not under validator control
        }
        return kind->isKind(obj, match);
    }

    void ObjectValidator::scanAttributeElement(DOMElement *attribElement) {
        std::string name = XMLtoUtf8(attribElement->getAttribute(Utf8ToXML("name").x_str())).c_str();
        std::string typeName = XMLtoUtf8(attribElement->getAttribute(Utf8ToXML("type").x_str())).c_str();
        validationType valType;
        if (typeName == "bool")
            valType = VAL_BOOL;
        else if (typeName == "int")
            valType = VAL_INT;
        else if (typeName == "double")
            valType = VAL_DOUBLE;
        else if (typeName == "nil")
            valType = VAL_NIL;
        else if (typeName == "dir")
            valType = VAL_DIR;
//        else if (typeName == "pos")
//            valType = VAL_POS;
        else if (typeName == "string")
            valType = VAL_STRING;
        else if (typeName == "enum")
            valType = VAL_ENUM;
        else if (typeName == "tokens")
            valType = VAL_TOKENS;
        
        const XMLCh* defaultAttribute = attribElement->getAttribute(Utf8ToXML("default").x_str());
        std::string defaultString = XMLtoUtf8(defaultAttribute).c_str();
        Value defaultValue = parseTypedValue(defaultAttribute, defaultString, valType);

        std::string rwString = XMLtoUtf8(attribElement->getAttribute(Utf8ToXML("rw").x_str())).c_str();
        bool allowRead = (rwString.find('r') != std::string::npos) ? true : false;
        bool allowWrite = (rwString.find('w') != std::string::npos) ? true : false;
        
        const XMLCh* minAttribute = attribElement->getAttribute(Utf8ToXML("min").x_str());
        std::string minString = XMLtoUtf8(minAttribute).c_str();
        Value minVal = parseTypedValue(minAttribute, minString, valType);

        const XMLCh* maxAttribute = attribElement->getAttribute(Utf8ToXML("max").x_str());
        std::string maxString = XMLtoUtf8(maxAttribute).c_str();
        Value maxVal = parseTypedValue(maxAttribute, maxString, valType);
        
        AttributeDescriptor *attr = new AttributeDescriptor(name, valType, defaultValue, allowRead, allowWrite, minVal, maxVal);
        defaultAttributes[name] = attr;
//        Log << "Default Attribute " << name << "\n";
    }
    
    void ObjectValidator::scanMessageElement(DOMElement *msgElement) {
        std::string name = XMLtoUtf8(msgElement->getAttribute(Utf8ToXML("name").x_str())).c_str();
        MessageDescriptor *msg = new MessageDescriptor(name);
        messages[name] = msg;
//        Log << "Default Message " << name << "\n";
    }
    
    void ObjectValidator::scanObjectElement(DOMElement *objectElement) {
        std::string name = XMLtoUtf8(objectElement->getAttribute(Utf8ToXML("name").x_str())).c_str();
        std::string super = XMLtoUtf8(objectElement->getAttribute(Utf8ToXML("super").x_str())).c_str();
        int states = XMLString::parseInt(objectElement->getAttribute(Utf8ToXML("states").x_str()));
        bool isAbstract = boolValue(objectElement->getAttribute(Utf8ToXML("abstract").x_str()));
        bool isInit = boolValue(objectElement->getAttribute(Utf8ToXML("init").x_str()));
        
        KindDescriptor *kd = new KindDescriptor(name, super, isAbstract, states, isInit);
        kinds[name] = kd;
        
        DOMNodeList *elemList;
        
        elemList = objectElement->getElementsByTagName(Utf8ToXML("msg").x_str());
        for (int i = 0, l = elemList->getLength(); i < l; i++) {
            DOMElement *msgElem = dynamic_cast<DOMElement *>(elemList->item(i));
            kd->addMessage(XMLtoUtf8(msgElem->getAttribute(Utf8ToXML("name").x_str())).c_str());
        }
        
        elemList = objectElement->getElementsByTagName(Utf8ToXML("attr").x_str());
        for (int i = 0, l = elemList->getLength(); i < l; i++) {
            DOMElement *attrElem = dynamic_cast<DOMElement *>(elemList->item(i));
                        
            std::string name = XMLtoUtf8(attrElem->getAttribute(Utf8ToXML("name").x_str())).c_str();
            std::string typeName = XMLtoUtf8(attrElem->getAttribute(Utf8ToXML("type").x_str())).c_str();
            const XMLCh* defaultAttribute = attrElem->getAttribute(Utf8ToXML("default").x_str());
            std::string defaultString = XMLtoUtf8(defaultAttribute).c_str();
            std::string rwString = XMLtoUtf8(attrElem->getAttribute(Utf8ToXML("rw").x_str())).c_str();
            const XMLCh* minAttribute = attrElem->getAttribute(Utf8ToXML("min").x_str());
            std::string minString = XMLtoUtf8(minAttribute).c_str();
            const XMLCh* maxAttribute = attrElem->getAttribute(Utf8ToXML("max").x_str());
            std::string maxString = XMLtoUtf8(maxAttribute).c_str();
            const XMLCh* valAttribute = attrElem->getAttribute(Utf8ToXML("value").x_str());
            std::string valString = XMLtoUtf8(valAttribute).c_str();

            if (typeName == "" && defaultString == "" && rwString == "" && minString == "" 
                    && maxString == "" && valString == "")
                kd->addAttribute(name);
            else {
                ASSERT(typeName == "", XFrontend, ("ObjectValidator unsupported retyping of: " + name).c_str());
                AttributeDescriptor *cloneAttr = kd->addModifiedAttribute(name);
                if (rwString != "") {
                    cloneAttr->setReadable((rwString.find('r') != std::string::npos) ? true : false);
                    cloneAttr->setWritable((rwString.find('w') != std::string::npos) ? true : false);
                }
                validationType valType = cloneAttr->getType();
                if (defaultString != "") {
                    Value defaultValue = parseTypedValue(defaultAttribute, defaultString, valType);
                    cloneAttr->setDefaultValue(defaultValue);
                }
                if (minString != "") {
                    Value minValue = parseTypedValue(minAttribute, minString, valType);
                    cloneAttr->setDefaultValue(minValue);
                }
                if (maxString != "") {
                    Value maxValue = parseTypedValue(maxAttribute, maxString, valType);
                    cloneAttr->setMaxValue(maxValue);
                }
                if (valString != "") {
                    Value value = parseTypedValue(valAttribute, valString, valType);
                    cloneAttr->setValue(value);
                }
            }
        }
        
//        kd->log();
    }
    
    Value ObjectValidator::parseTypedValue(const XMLCh* xmlSource, const std::string &strSource, validationType type) {
        if (strSource != "nil") {
            switch (type) {
                case VAL_BOOL :
                    return Value(boolValue(xmlSource));
                    break;
                case VAL_STRING :
                    return Value(XMLtoUtf8(xmlSource).c_str());
                case VAL_INT :
                case VAL_DIR :
                case VAL_ENUM :
                    return Value(XMLString::parseInt(xmlSource));
                    break;
                case VAL_DOUBLE : {
                    XMLDouble * result = new XMLDouble(xmlSource);
                    Value v = result->getValue();
                    delete result;
                    return v;
                    break;
                }
                default :    // nil, tokens default to nil
                    return Value();
                    break;
            }
        } else
            return Value();
    }
    
} // namespace enigma

