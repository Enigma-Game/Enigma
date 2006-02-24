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
 */

#include "lev/Proxy.hh"

#include "ecl_system.hh"
#include "levels.hh"
#include "lua.hh"
#include "main.hh"
#include "nls.hh"
#include "Utf8ToXML.hh"
#include "XMLtoUtf8.hh"
#include <iostream>
#include <sstream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

extern "C" {
#include "lua.h"
}


using namespace std;
using namespace enigma;
XERCES_CPP_NAMESPACE_USE 

namespace enigma { namespace lev {
    // http://enigma/level
    const XMLCh Proxy::levelNS[] =
    {
            chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash
        ,   chForwardSlash, chLatin_e, chLatin_n, chLatin_i, chLatin_g, chLatin_m
        ,   chLatin_a, chForwardSlash, chLatin_l, chLatin_e, chLatin_v, chLatin_e
        ,   chLatin_l, chNull
    };

    Proxy *Proxy::currentLevel = NULL;
    
    Proxy *Proxy::loadedLevel() {
        return currentLevel;
    }
    
    void Proxy::releaseCurrentLevel() {
        if (currentLevel != NULL)
            currentLevel->release();
    }
        
    void Proxy::loadLevel(std::string filename) {
        if (currentLevel != NULL)
            currentLevel->release();
        currentLevel = new Proxy(filename);
        currentLevel->loadLua();
    }

    
    Proxy::Proxy(std::string filename) {
        loadXML(filename);
    }
        
    Proxy::~Proxy() {
        if (doc != NULL)
            doc->release();
    }
    
    void Proxy::release() {
        if (doc != NULL)
            doc->release();
        if (this == currentLevel)
            currentLevel = NULL;
    }
    

    void Proxy::loadXML(std::string filename) {
        std::string errMessage;
        try {
            std::ostringstream errStream;
            app.domParserErrorHandler->resetErrors();
            app.domParserErrorHandler->reportToOstream(&errStream);
            app.domParserSchemaResolver->resetResolver();
            app.domParserSchemaResolver->addSchemaId("level.xsd","level.xsd");
            doc = app.domParser->parseURI(filename.c_str());
            infoElem = dynamic_cast<DOMElement *>(doc->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("info").x_str())->item(0));
            stringList = doc->getElementsByTagNameNS(levelNS, 
                    Utf8ToXML("string").x_str());
            if(app.domParserErrorHandler->getSawErrors()) {
                errMessage = std::string(errStream.str(), errStream.str().size());
            }
            app.domParserErrorHandler->reportToNull();  // do not report to errStream any more
        }
        catch (...) {
            errMessage = "Unexpected XML Exception on load of level\n";
        }
        if (!errMessage.empty()) {
            Log << errMessage;   // make long error messages readable
            throw levels::XLevelLoading (errMessage);
        }
            
    }
    std::string Proxy::getLocalizedString(const std::string &key) {
        std::string english = key;
        std::string translation;
        std::string lang = ecl::GetLanguageCode (app.language);
        
        // add handling for calls besides level run: info from cache 
        
        if (key == "titel" || key == "subtitel") {
            // get the english originals from the identity attributes
            DOMElement *identityElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("identity").x_str())->item(0));
            english = XMLtoUtf8(identityElem->getAttributeNS(levelNS, 
                    Utf8ToXML(&key).x_str())).c_str();
        }
        
        bool translFound = false;
        bool protectedString = true;
        for (int i = 0, l = stringList-> getLength();  i < l && !translFound; i++) {
            DOMElement *stringElem = dynamic_cast<DOMElement *>(stringList->item(i));
            if (key == XMLtoUtf8(stringElem->getAttributeNS(levelNS, 
                    Utf8ToXML("key").x_str())).c_str()) {
                // 2 strings with matching key may be found: 
                // first the protected, then the public one
                if (protectedString) {
                    // resolve english text from string->english subelement
                    DOMElement *englishElem = 
                         dynamic_cast<DOMElement *>(stringElem->getElementsByTagNameNS(
                         levelNS, Utf8ToXML("english").x_str())->item(0));
                    std::string tmp = XMLtoUtf8(englishElem->getTextContent()).c_str();
                    if (!tmp.empty()) {
                        // overwrite the english key only if text is provided
                        english = tmp;
                    }
                    // we got the final english string 
                    if (lang == "en") {
                        translation = english;
                        translFound = true;
                    }
                }
                if (!translFound) {
                    // protected and public translations
                    DOMNodeList *translList = stringElem->getElementsByTagNameNS(
                            levelNS, Utf8ToXML("translation").x_str());
                    for (int i = 0, l = translList-> getLength();  i < l && !translFound; i++) {
                        DOMElement *translElem = dynamic_cast<DOMElement *>(translList->item(i));
                        if (lang == XMLtoUtf8(translElem->getAttributeNS(levelNS, 
                                Utf8ToXML("lang").x_str())).c_str()) {
                            translation = XMLtoUtf8(translElem->getTextContent()).c_str();
                            translFound = true;
                        }
                    }
                }
                if  (!translFound && protectedString) {
                    // gettext
                    std::string tmp = gettext(english.c_str());
                    if (tmp != english) {
                        translation = tmp;
                        translFound = true;
                    }
                }
                protectedString = false;    // the next matching string is public
            }
        }
        return (translFound ? translation : english);
    }
    
    std::string Proxy::getAuthor() {
        // todo - get author from cache if possible
        DOMElement *identityElem = 
                dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                levelNS, Utf8ToXML("author").x_str())->item(0));
        return XMLtoUtf8(identityElem->getAttributeNS(levelNS, 
                Utf8ToXML("name").x_str())).c_str();
    }

    void Proxy::loadLua() {
        lua_State *L = lua::LevelState();
        DOMNodeList * luamainList = doc->getElementsByTagNameNS(levelNS, Utf8ToXML("luamain").x_str());
        if (luamainList->getLength() == 1) {
            DOMElement *luamain  = dynamic_cast<DOMElement *>(luamainList->item(0));
            if (lua_dostring(L, XMLtoUtf8(luamain->getTextContent ()).c_str() ) != 0) {
                throw enigma_levels::XLevelLoading (lua::LastError(L));
            }
        } else {
            throw enigma_levels::XLevelLoading ("not implemented");
        }
    }
}} // namespace enigma::lev
