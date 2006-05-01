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
#include "oxyd_internal.hh"
#include "Utf8ToXML.hh"
#include "XMLtoUtf8.hh"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>
 
extern "C" {
#include "lua.h"
#include "lauxlib.h"
}


using namespace std;
using namespace enigma;
XERCES_CPP_NAMESPACE_USE 

namespace enigma { namespace lev {
    bool boolValue(const XMLCh * const string) {
        std::string boolString = XMLtoUtf8(string).c_str();
        if (boolString == "true" ||  boolString == "1")
            return true;
        else
            // we need no further investigation due to XML validation
            return false;
    }
     
    // http://enigma-game.org/schema/level/1
    const XMLCh Proxy::levelNS[] = {
            chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash,
            chForwardSlash, chLatin_e, chLatin_n, chLatin_i, chLatin_g, chLatin_m,
            chLatin_a, chDash, chLatin_g, chLatin_a, chLatin_m, chLatin_e,
            chPeriod, chLatin_o, chLatin_r, chLatin_g, chForwardSlash, 
            chLatin_s, chLatin_c, chLatin_h, chLatin_e, chLatin_m, chLatin_a,
            chForwardSlash, chLatin_l, chLatin_e, chLatin_v, chLatin_e,
            chLatin_l, chForwardSlash, chDigit_1, chNull
    };
    std::map<std::string, Proxy *> Proxy::cache;

    Proxy *Proxy::currentLevel = NULL;
    
    Proxy *Proxy::loadedLevel() {
        return currentLevel;
    }
    

    Proxy * Proxy::registerLevel(std::string levelPath, std::string indexPath,
            std::string levelId, std::string levelTitle, std::string levelAuthor,
            int levelScoreVersion, int levelRelease, bool levelHasEasymode) {
        Proxy *theProxy;
        pathType thePathType = pt_resource;
        std::string theNormLevelPath;
        
        // Normalize level path
        if (indexPath == "#commandline") {
            if (levelPath.find("://") != std::string::npos) {
                thePathType = pt_url; // http, ftp, https
            }
            else
                thePathType = pt_absolute;
            theNormLevelPath = levelPath;
        } else {
            if (levelPath.find("://") != std::string::npos) {
                thePathType = pt_url; // http, ftp, https
                theNormLevelPath = levelPath;
            } else if (!levelPath.empty() && levelPath[0] == '#'){
                thePathType = pt_oxyd;
                theNormLevelPath = levelPath;
            } else {
                thePathType = pt_resource;
                if (levelPath.find("./") == 0) {
                    assert(indexPath != "#history"); // no relative paths on history
                    // relative level path
                    if (indexPath.empty())
                        theNormLevelPath = levelPath.substr(2);
                    else
                        theNormLevelPath = indexPath + levelPath.substr(1);
                } else {
                    theNormLevelPath = levelPath;
                }
            }
        }
        
        // search for existing proxy
        // we use a combined key to support pack indices to reference
        // different releases of a level at the same path -- this is an
        // inconsistency in the users level collection as different releases
        // should have different filenames -- but we have to handle it:
        // we generate proxys for each registration and decide on level load
        // evaluating the level metadata which proxy was right.
        char txt[5];
        snprintf(txt, sizeof(txt), "%d", levelRelease);        
        std::string cacheKey = theNormLevelPath + levelId + txt;
        std::map<std::string, Proxy *>::iterator i = cache.find(cacheKey);
        if (i != cache.end()) {
            Proxy * candidate = i->second;
//            Log << "Proxy cache hit for " << theNormLevelPath << " found " << candidate->normLevelPath << " release " << candidate->releaseVersion << "\n";
            return candidate;
        }
        
        // create new proxy
        theProxy = new Proxy(thePathType, theNormLevelPath, levelId, levelTitle,
            levelAuthor, levelScoreVersion, levelRelease, levelHasEasymode);
        cache.insert(std::make_pair(cacheKey, theProxy));
        return theProxy;
    }
     

    Proxy::Proxy(pathType thePathType, std::string theNormLevelPath,
            std::string levelId, std::string levelTitle, std::string levelAuthor,
            int levelScoreVersion, int levelRelease, bool levelHasEasymode) :  
            normPathType(thePathType), normLevelPath(theNormLevelPath), 
            id(levelId), title(levelTitle), author(levelAuthor),
            scoreVersion(levelScoreVersion), releaseVersion(levelRelease),
            revisionNumber(0), hasEasymodeFlag(levelHasEasymode), 
            scoreUnit (duration), doc(NULL) {
    }
        
    Proxy::~Proxy() {
        if (doc != NULL)
            doc->release();
    }
    
    void Proxy::release() {
        if (doc != NULL) {
            doc->release();
            doc = NULL;
        }
        if (this == currentLevel)
            currentLevel = NULL;
    }
    
    std::string Proxy::getNormLevelPath() {
        return normLevelPath;
    }
    
    std::string Proxy::getLocalSubstitutionLevelPath() {
        std::string result = getNormLevelPath();
        std::string::size_type pos;
        // substitute url protocol "http://" by "http/"
        pos = result.find("://");
        if (pos != std::string::npos)
            result.replace(pos, 2,"");
        
        // substitute all filesystem uncommon chars to '_'
        const std::string validChars("\\_-/.~#0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
        pos = result.find_first_not_of(validChars, 0);
        while (pos != std::string::npos) {
            result.replace(pos, 1,"~");
            pos = result.find_first_not_of(validChars, pos+1);
        }
        return result;
    }
    
    Proxy::pathType Proxy::getNormPathType() {
        return normPathType;
    }

    std::string Proxy::getAbsLevelPath() {
        return absLevelPath;
    }
    
    void Proxy::loadLevel() {
        if (doc == NULL) {
            load(false);
        } else {
            loadLuaCode();
        }
    }
    
    void Proxy::loadMetadata() {
        load(true);
    }
    
    void Proxy::load(bool onlyMetadata) {
        bool useFileLoader = false;
        bool isXML = true;
        std::auto_ptr<std::istream> isptr;
        ByteVec levelCode;
        std::string errMessage;
        absLevelPath = "";
        
        // release current proxy
        if (currentLevel != NULL)
            currentLevel->release();
        currentLevel = this;
        
        // handle oxyd first
        if (normPathType == pt_oxyd) {
            if(onlyMetadata)
                return;
            // use oxyd loader
            std::string::size_type posSecondHash = normLevelPath.find('#',1);
            if (posSecondHash == string::npos)
                throw levels::XLevelLoading("Bad filename for oxyd level: " + normLevelPath );
            std::string packName = normLevelPath.substr(1, posSecondHash -1);
            oxyd::LevelPack_Oxyd *lp =  dynamic_cast<oxyd::LevelPack_Oxyd *>(levels::FindLevelPack(packName)); // preliminary
            if (lp != NULL) {
                std::string levelNumber = normLevelPath.substr(posSecondHash + 1);
                lp->load_oxyd_level(atoi(levelNumber.c_str()));
            }
            else {
                throw levels::XLevelLoading("Missing oxyd levelpack for: " + normLevelPath);
            }
            return;
            
        // resolve resource path to filepath
        } else if (normPathType == pt_absolute || normPathType == pt_url) { 
            absLevelPath = normLevelPath;
        } else if(normPathType == pt_resource) {
            if(!app.resourceFS->findFile ("levels/" + normLevelPath + ".xml", 
                        absLevelPath, isptr) &&
                    !app.resourceFS->findFile ("levels/" + normLevelPath + ".lua", 
                        absLevelPath, isptr))
                throw levels::XLevelLoading("Could not find level " + normLevelPath );
        } else
            // error unknown type
            return;
        
        
        // xml or lua
        size_t extbegin = absLevelPath.rfind ('.');
        if (extbegin != string::npos) {
            string ext = absLevelPath.substr (extbegin);
        
            if (normPathType != pt_url && (ext == ".lua" || ext == ".ell")) {
                useFileLoader = true;
                isXML = false;  // preliminary - may still be lua commented xml
            } else if (ext == ".xml" || ext == ".elx") {
                isXML = true;
                // use file loader only for zipped xml files
                useFileLoader = (isptr.get() != NULL) ? true : false;
            } else {
                throw levels::XLevelLoading ("Unknown file extension in " + absLevelPath);
            }
        } else {
                throw levels::XLevelLoading ("Unknown file extension in " + absLevelPath);
        }

        // load
        
        if (useFileLoader) {
            // preload plain Lua file or zipped level
            if (isptr.get() != NULL) {
                // zipped file
                Readfile (*isptr, levelCode);
            } else {
                // plain file
                basic_ifstream<char> ifs(absLevelPath.c_str(), ios::binary | ios::in);
                Readfile (ifs, levelCode);
            }
 
            if(!isXML) {
                if(levelCode.size() >= 8 && std::string("--xml-- ").compare(
                        0, 8, &(levelCode[0]), 8) == 0) {
                    isXML = true;
                    // delete lua "--xml-- " comments in level code
                    int s = levelCode.size();
                    int i = 0;
                    int j = 0;
                    for (i=8, j=0; i < s;) {
                        char c = levelCode[j++] = levelCode[i++];
                        if(c == '\n') {
                            if(s >= i+8 && std::string("--xml-- ").compare(
                                    0, 8, &(levelCode[i]), 8) == 0) {
                                i += 8;
                            }
                        }
                    }
                    levelCode.resize(j);
                } else if (!onlyMetadata){
                    // handle pure lua
                    // load plain lua file
                    doc = NULL;
                    lua_State *L = lua::LevelState();
                    if (lua::Dobuffer(L, levelCode) != 0) {
                        throw levels::XLevelLoading (lua::LastError(L));
                    }
                }
            }
        }
        if (isXML) {
            try {
                std::ostringstream errStream;
                app.domParserErrorHandler->resetErrors();
                app.domParserErrorHandler->reportToOstream(&errStream);
                app.domParserSchemaResolver->resetResolver();
                app.domParserSchemaResolver->addSchemaId("level.xsd","level.xsd");
                if (!useFileLoader) {
                    // local xml file or URL
                    doc = app.domParser->parseURI(absLevelPath.c_str());
                } else {
                    // preloaded lua-commented xml or zipped xml
                    std::auto_ptr<Wrapper4InputSource> domInputLevelSource ( new Wrapper4InputSource(
                            new MemBufInputSource(reinterpret_cast<const XMLByte *>(&(levelCode[0])),
                            levelCode.size(), absLevelPath.c_str(), false)));
                    doc = app.domParser->parse(*domInputLevelSource);
                }
                if (doc != NULL && !app.domParserErrorHandler->getSawErrors()) {
                    infoElem = dynamic_cast<DOMElement *>(doc->getElementsByTagNameNS(
                            levelNS, Utf8ToXML("info").x_str())->item(0));
                    stringList = doc->getElementsByTagNameNS(levelNS, 
                            Utf8ToXML("string").x_str());
                }
                if(app.domParserErrorHandler->getSawErrors()) {
                    errMessage = errStream.str();
                }
                app.domParserErrorHandler->reportToNull();  // do not report to errStream any more
            }
            catch (...) {
                errMessage = "Unexpected XML Exception on load of level\n";
            }
            if (!errMessage.empty()) {
                Log << errMessage;   // make long error messages readable
                throw levels::XLevelLoading (errMessage);
// todo: check metadata, handle shadowed levels
            } else if (!onlyMetadata){   
                loadLuaCode();
            }
        }
    }

    void Proxy::loadLuaCode() {
        lua_State *L = lua::LevelState();
        DOMNodeList * luamainList = doc->getElementsByTagNameNS(levelNS, Utf8ToXML("luamain").x_str());
        if (luamainList->getLength() == 1) {
            DOMElement *luamain  = dynamic_cast<DOMElement *>(luamainList->item(0));
            if (luaL_dostring(L, XMLtoUtf8(luamain->getTextContent()).c_str() ) != 0) {
	      lua_setglobal (L, "_LASTERROR");
                throw enigma_levels::XLevelLoading(lua::LastError(L));
            }
        } else {
            throw enigma_levels::XLevelLoading("not implemented");
        }
    }


    std::string Proxy::getLocalizedString(const std::string &key) {
        std::string english = key;
        std::string translation;
        std::string lang = ecl::GetLanguageCode (app.language);
        
        // add handling for calls besides level run: info from cache
        if (doc == NULL) {
            if (key == "title")
                return title;
            else
                return key;
        }
        
        if (key == "title" || key == "subtitle") {
            // get the english originals from the identity attributes
            DOMElement *identityElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("identity").x_str())->item(0));
            english = XMLtoUtf8(identityElem->getAttributeNS(levelNS, 
                    Utf8ToXML(&key).x_str())).c_str();
            if (key == "title")
                title = english;  // update cash
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
    
    std::string Proxy::getId() {
        return id;
    }
    
    int Proxy::getScoreVersion() {
        if (doc != NULL) {
            DOMElement *versionElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("version").x_str())->item(0));
            scoreVersion = XMLString::parseInt(versionElem->getAttributeNS(levelNS, 
                    Utf8ToXML("score").x_str()));
        }
        return scoreVersion;
    }
    
    int Proxy::getReleaseVersion() {
        if (doc != NULL) {
            DOMElement *versionElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("version").x_str())->item(0));
            releaseVersion = XMLString::parseInt(versionElem->getAttributeNS(levelNS, 
                    Utf8ToXML("release").x_str()));
        }
        return releaseVersion;
    }
    
    int Proxy::getRevisionNumber() {
        if (doc != NULL) {
            DOMElement *versionElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("version").x_str())->item(0));
            const XMLCh * revision = versionElem->getAttributeNS(levelNS, 
                    Utf8ToXML("revision").x_str());
            unsigned int len = XMLString::stringLen(revision);
            int start = -1;
            int end = -1;
            for (int i = 0; i<len; i++) {
                if(XMLString::isDigit(revision[i])) {
                    if (start == -1)
                        start = i;
                } else if (start != -1 && end == -1)
                    end = i;
            }
            if (start != -1) {
                if (end == -1)
                    end = len;
                XMLCh * revisionDigits = XMLString::replicate(revision);
                XMLString::subString(revisionDigits, revision, start, end);
                revisionNumber = XMLString::parseInt(revisionDigits);
                XMLString::release(&revisionDigits);
            } else {
                // "$Revision$" - level not stored in repository yet
                revisionNumber ==  0;
            }
        }
        return revisionNumber;
    }
    
    std::string Proxy::getAuthor() {
        if (doc != NULL) {
            DOMElement *authorElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("author").x_str())->item(0));
            author = XMLtoUtf8(authorElem->getAttributeNS(levelNS, 
                    Utf8ToXML("name").x_str())).c_str();
        }
        return author;
    }

    std::string Proxy::getTitle() {
        if (doc != NULL) {
            DOMElement *identityElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("identity").x_str())->item(0));
            title = XMLtoUtf8(identityElem->getAttributeNS(levelNS, 
                    Utf8ToXML("title").x_str())).c_str();
        }
        return title;
    }

    bool Proxy::hasEasymode() {
        if (doc != NULL) {
            DOMElement *modesElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("modes").x_str())->item(0));
            hasEasymodeFlag = boolValue(modesElem->getAttributeNS(levelNS, 
                        Utf8ToXML("easy").x_str()));
        }
        return hasEasymodeFlag;
    }

    std::string Proxy::getContact() {
        std::string contact;
        if (doc != NULL) {
            DOMElement *authorElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("author").x_str())->item(0));
            contact = XMLtoUtf8(authorElem->getAttributeNS(levelNS, 
                    Utf8ToXML("email").x_str())).c_str();
        }
        return contact;
    }
    
    std::string Proxy::getHomepage() {
        std::string homepage;
        if (doc != NULL) {
            DOMElement *authorElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("author").x_str())->item(0));
            homepage = XMLtoUtf8(authorElem->getAttributeNS(levelNS, 
                    Utf8ToXML("homepage").x_str())).c_str();
        }
        return homepage;
    }

    controlType Proxy::getControl() {
        controlType control = force;
        if (doc != NULL) {
            DOMElement *authorElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("modes").x_str())->item(0));
            std::string controlString = XMLtoUtf8(authorElem->getAttributeNS(levelNS, 
                    Utf8ToXML("control").x_str())).c_str();
            if (controlString == "balance")
                control = balance;
            else if  (controlString == "key")
                control = key;
            else if  (controlString == "other")
                control = other;
        }
        return control;
    }

    scoreUnitType Proxy::getScoreUnit() {
        if (doc != NULL) {
            DOMElement *modesElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("modes").x_str())->item(0));
            std::string txt = XMLtoUtf8(modesElem->getAttributeNS(levelNS, 
                        Utf8ToXML("scoreunit").x_str())).c_str();
            if (txt == "number")
                scoreUnit = number;
            else
                // default
                scoreUnit = duration;
        }
        return scoreUnit;
    }
    
    std::string Proxy::getScoreTarget() {
        std::string title = "time";
        if (doc != NULL) {
            DOMElement *modesElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("modes").x_str())->item(0));
            title = XMLtoUtf8(modesElem->getAttributeNS(levelNS, 
                        Utf8ToXML("scoretarget").x_str())).c_str();
        }
        return title;
    }
    
    std::string Proxy::getCredits(bool infoUsage) {
        std::string credits;
        std::string attribute = infoUsage ? "showinfo" : "showstart";
        if (doc != NULL) {
            DOMElement *creditsElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("credits").x_str())->item(0));
            if (creditsElem != NULL)  // element is optional
                if (boolValue(creditsElem->getAttributeNS(levelNS, 
                        Utf8ToXML(attribute.c_str()).x_str())))
                    credits = XMLtoUtf8(creditsElem->getTextContent()).c_str();
        }
        return credits;
    }

    std::string Proxy::getDedication(bool infoUsage) {
        std::string dedication;
        std::string attribute = infoUsage ? "showinfo" : "showstart";
        if (doc != NULL) {
            DOMElement *dedicationElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("dedication").x_str())->item(0));
            if (dedicationElem != NULL)  // element is optional
                if (boolValue(dedicationElem->getAttributeNS(levelNS, 
                        Utf8ToXML(attribute.c_str()).x_str())))
                    dedication = XMLtoUtf8(dedicationElem->getTextContent()).c_str();
        }
        return dedication;
    }
    int Proxy::getEasyScore() {
        int score = -1;
        if (doc != NULL) {
            DOMElement *versionElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("score").x_str())->item(0));
            score = scoreText2Int(XMLtoUtf8(versionElem->getAttributeNS(levelNS, 
                    Utf8ToXML("easy").x_str())).c_str());
        }
        return score;
    }
    
    int Proxy::getDifficultScore() {
        int score = -1;
        if (doc != NULL) {
            DOMElement *versionElem = 
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("score").x_str())->item(0));
            score = scoreText2Int(XMLtoUtf8(versionElem->getAttributeNS(levelNS, 
                    Utf8ToXML("difficult").x_str())).c_str());
        }
        return score;
    }

    int Proxy::scoreText2Int(std::string text) {
        if (text == "-")
            return -1;
        else {
            std::string::size_type colon = text.find(':');
            if (colon == std::string::npos)
                return atoi(text.c_str());
            else
                return atoi(text.substr(0,colon).c_str()) * 60 +
                        atoi(text.substr(colon+1).c_str());
        }
    }
}} // namespace enigma::lev
