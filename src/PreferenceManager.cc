/*
 * Copyright (C) 2006 Ronald Lamprecht
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

#include "PreferenceManager.hh"
#include "main.hh"
#include "DOMErrorReporter.hh"
#include "DOMSchemaResolver.hh"
#include "LocalToXML.hh"
#include "Utf8ToXML.hh"
#include "options.hh"
#include "XMLtoLocal.hh"
#include "XMLtoUtf8.hh"
#include "ecl_system.hh"
#include <iostream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLDouble.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XercesVersion.hpp>
#if _XERCES_VERSION < 30000
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#endif


using namespace std;
using namespace enigma;
XERCES_CPP_NAMESPACE_USE 

namespace enigma {
    PreferenceManager *PreferenceManager::theSingleton = 0;
    
    PreferenceManager* PreferenceManager::instance() {
        if (theSingleton == 0) {
            theSingleton = new PreferenceManager();
        }
        return theSingleton;
    }
    
    PreferenceManager::PreferenceManager() {
        std::string prefTemplatePath;
        bool haveXMLProperties = (ecl::FileExists(app.prefPath)) ? true : false;
        
        if (!app.systemFS->findFile( PREFFILENAME , prefTemplatePath)) {
            cerr << "Preferences: no template found\n";
            exit(-1);
        }

        try {
            app.domParserErrorHandler->resetErrors();
            app.domParserErrorHandler->reportToErr();
            app.domParserSchemaResolver->resetResolver();
            app.domParserSchemaResolver->addSchemaId("preferences.xsd","preferences.xsd");

            if (haveXMLProperties) {
                // update existing XML prefs from possibly newer template:
                // use user prefs and copy new properties from template
                doc = app.domParser->parseURI(app.prefPath.c_str());
                // The following algorithm is not optimized - O(n^2)!
                DOMDocument * prefTemplate = app.domParser->parseURI(prefTemplatePath.c_str());
                DOMNodeList * tmplPropList = prefTemplate->getElementsByTagName(
                        Utf8ToXML("property").x_str());
                for (int i = 0, l = tmplPropList-> getLength(); i < l; i++) {
                    DOMElement *tmplProperty = dynamic_cast<DOMElement *>(tmplPropList->item(i));
                    const XMLCh * key = tmplProperty->getAttribute(Utf8ToXML("key").x_str());
                    DOMElement * lastUserProperty;
                    if (!hasProperty(key, &lastUserProperty)) {
                        // a new property in the template
                        Log << "Preferences: add new Property \"" << XMLtoLocal(key) << "\"\n";
                        // make a copy
                        DOMNode * newProperty =  doc->importNode(tmplProperty, false);
                        if (newProperty == NULL) {
                            Log << "Preferences: copy new Property failed!\n";
                        } else {
                            // insert it at the end of the existing user properties
                            lastUserProperty->getParentNode()->insertBefore(
                                     dynamic_cast<DOMElement *>(newProperty), 
                                     lastUserProperty->getNextSibling());
                        }
                    }
                }
                prefTemplate->release();
            } else {
                // update from LUA options to XML preferences:
                // use the template, copy LUA option values and save it later as prefs
                doc = app.domParser->parseURI(prefTemplatePath.c_str());
                DOMNodeList * propList = doc->getElementsByTagName(Utf8ToXML("property").x_str());
                for (int i = 0, l = propList-> getLength(); i < l; i++) {
                    DOMElement * property  = dynamic_cast<DOMElement *>(propList->item(i));
                    const XMLCh * key = property->getAttribute(Utf8ToXML("key").x_str());
                    std::string optionValue;
                    if (options::HasOption(XMLtoLocal(key).c_str(), optionValue)) {
                        Log << "Preferences: copy LUA option \"" << XMLtoLocal(key) << "\"\n";
                        property->setAttribute(Utf8ToXML("value").x_str(), 
                            LocalToXML(&optionValue).x_str());
                    } else {
                        Log << "Preferences: no LUA option \"" << XMLtoLocal(key) << "\"\n";
                    }
                }
            }
        }
        catch (const XMLException& toCatch) {
            char* message = XMLString::transcode(toCatch.getMessage());
            cerr << "Exception on load of preferences: "
                 << message << "\n";
            XMLString::release(&message);
            exit (-1);
        }
        catch (const DOMException& toCatch) {
            char* message = XMLString::transcode(toCatch.msg);
            cerr << "Exception on load of preferences: "
                 << message << "\n";
            XMLString::release(&message);
            exit (-1);
        }
        catch (...) {
            cerr << "Unexpected Exception on load of preferences\n" ;
            exit (-1);
        }        
    }
     
    PreferenceManager::~PreferenceManager() {
        if (doc != NULL)
            shutdown();
    }
    
    bool PreferenceManager::save() {
        bool result = true;
        
        if (doc == NULL)
            return true;
        
        try {
#if _XERCES_VERSION >= 30000
            result = app.domSer->writeToURI(doc, LocalToXML(& app.prefPath).x_str());
#else
            XMLFormatTarget *myFormTarget = new LocalFileFormatTarget(app.prefPath.c_str());
            result = app.domSer->writeNode(myFormTarget, *doc);            
#endif
        }
        catch (const XMLException& toCatch) {
            char* message = XMLString::transcode(toCatch.getMessage());
            cerr << "Exception on save of preferences: "
                 << message << "\n";
            XMLString::release(&message);
            result = false;
        }
        catch (const DOMException& toCatch) {
            char* message = XMLString::transcode(toCatch.msg);
            cerr << "Exception on save of preferences: "
                 << message << "\n";
            XMLString::release(&message);
            result = false;
        }
        catch (...) {
            cerr << "Unexpected exception on save of preferences\n" ;
            result = false;
        }
        if (!result)
            Log << "Preferences save fault\n";
        else
            Log << "Preferences save o.k.\n";
        
        return result;
    }

    void PreferenceManager::shutdown() {
        save();
        doc->release();
        doc = NULL;
    }
    
    void PreferenceManager::setPref(const char *prefName, const std::string &value) {
        DOMElement * property = getProperty(prefName);
        
        property->setAttribute(Utf8ToXML("value").x_str(),
                Utf8ToXML(&value).x_str());
    }
    
    void PreferenceManager::getPref(const char *prefName, std::string &value) {
        DOMElement * property;
        bool propFound = hasProperty(prefName, &property);
        if (propFound) {
            value = XMLtoUtf8(property->getAttribute(Utf8ToXML("value").x_str())).c_str();
        }
    }

    std::string PreferenceManager::getString(const char *prefName) {
        std::string value;
        getPref(prefName, value);
        return value;
    }
    
    void PreferenceManager::setPref(const char *prefName, const double &value) {
        char printedValue[20];
        sprintf(printedValue, "%.7g", value);
        DOMElement * property = getProperty(prefName);
        property->setAttribute(Utf8ToXML("value").x_str(), 
                Utf8ToXML(printedValue).x_str());
    }
    
    void PreferenceManager::getPref(const char *prefName, double &value) {
        DOMElement * property;
        bool propFound = hasProperty(prefName, &property);
        if (propFound) {
            XMLDouble * result = new XMLDouble(property->getAttribute(Utf8ToXML("value").x_str()));
            value = result->getValue();
            delete result;
        }
    }

    double PreferenceManager::getDouble(const char *prefName) {
        double value = 0;
        getPref (prefName, value);
        return value;
    }
    
    void PreferenceManager::setPref(const char *prefName, const int &value) {
        char printedValue[20];
        sprintf(printedValue, "%d", value);
        DOMElement * property = getProperty(prefName);

        property->setAttribute(Utf8ToXML("value").x_str(), 
                Utf8ToXML(printedValue).x_str());
    }
    
    void PreferenceManager::getPref(const char *prefName, int &value) {
        DOMElement * property;
        bool propFound = hasProperty(prefName, &property);
        if (propFound) {
            value = XMLString::parseInt(property->getAttribute(Utf8ToXML("value").x_str()));
        }
    }

    int PreferenceManager::getInt(const char *prefName) {
        int value = 0;
        getPref (prefName, value);
        return value;
    }
    
    void PreferenceManager::setPref(const char *prefName, const bool &value) {
        int i = (value ? 1: 0);
        setPref(prefName, i);
    }
    
    void PreferenceManager::getPref(const char *prefName, bool &value) {
        int result;
        getPref(prefName, result);
        value = ((result == 0) ? false: true);
    }

    bool PreferenceManager::getBool(const char *prefName) {
        bool value = 0;
        getPref (prefName, value);
        return value;
    }
    
    DOMElement * PreferenceManager::getProperty(const char *prefName) {
        XMLCh * key = XMLString::replicate(Utf8ToXML(prefName).x_str());
        DOMElement * property;
        bool propFound = hasProperty(prefName, &property);

        if (!propFound) {
            DOMElement * newProperty = doc->createElement (Utf8ToXML("property").x_str());
            newProperty->setAttribute(Utf8ToXML("key").x_str(), key);
            // insert it at the end of the existing properties
            property->getParentNode()->insertBefore(newProperty, property->getNextSibling());
            property = newProperty;
         }
        XMLString::release(&key);
        return property;
    }
    
    bool PreferenceManager::hasProperty(const char *prefName, DOMElement ** element) {
        return hasProperty(Utf8ToXML(prefName).x_str(), element);
    }
        
    bool PreferenceManager::hasProperty(const XMLCh * key, DOMElement ** element) {
        bool propFound = false;
        DOMElement * property;

// Xerces 3.0 has no full XPath support - otherwise the following simple
// statement would suffice and not be aborted with NOT_SUPPORTED_ERR.
//        doc->evaluate(Utf8ToXML("//property[@key='...']").x_str(), doc, NULL, 0, NULL);

        DOMNodeList * propList = doc->getElementsByTagName(Utf8ToXML("property").x_str());
        for (int i = 0, l = propList-> getLength(); i < l && !propFound; i++) {
            property  = dynamic_cast<DOMElement *>(propList->item(i));
            if (XMLString::equals(key, 
                    property->getAttribute(Utf8ToXML("key").x_str()))) {
                propFound = true;
            }
        }
        * element = property;
        return propFound;
    }
} // namespace enigma

