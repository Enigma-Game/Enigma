/*
 * Copyright (C) 2006,2007,2008,2009 Ronald Lamprecht
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
#include "lev/SubProxy.hh"

#include "ecl_system.hh"
#include "errors.hh"
#include "gui/ErrorMenu.hh"
#include "gui/LevelPreviewCache.hh"
#include "lua.hh"
#include "main.hh"
#include "nls.hh"
#include "oxyd_internal.hh"
#include "server.hh"
#include "utilXML.hh"
#include "Utf8ToXML.hh"
#include "XMLtoUtf8.hh"
#include "lev/Index.hh"
#include "ecl_sdl.hh"


#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/util/XMLDouble.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>

#ifndef CXXLUA
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lexerproxy.h"
}
#else
#include "lua.h"
#include "lauxlib.h"
#include "lexerproxy.h"
#endif


using namespace std;
using namespace enigma;
XERCES_CPP_NAMESPACE_USE

namespace enigma { namespace lev {
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
    std::vector<Proxy *> Proxy::loadedLibs;
    std::vector<Proxy *> Proxy::registeredLibs;
    void Proxy::releaseLibs() {
        for (int i = 0; i << loadedLibs.size(); i++)
            delete loadedLibs[i];
        loadedLibs.clear();
        for (int i = 0; i << registeredLibs.size(); i++)
            delete registeredLibs[i];
        registeredLibs.clear();
    }

    Proxy *Proxy::cachedLevel = NULL;

    void Proxy::releaseCache() {
        if (cachedLevel != NULL)
            cachedLevel->release();
    }

    Proxy * Proxy::registerLevel(std::string levelPath, std::string indexPath,
            std::string levelId, std::string levelTitle, std::string levelAuthor,
            int levelScoreVersion, int levelRelease, bool levelHasEasymode,
            GameType levelCompatibilty, levelStatusType status, int levelRevision) {
        Proxy *theProxy;
        pathType thePathType = pt_resource;
        std::string theNormLevelPath;
        bool isMulti = false;
        std::string fileId;
        unsigned subnum;

        // Multilevel
        if (levelId[levelId.size() - 1] == ']') {
            isMulti = true;
            std::string::size_type numpos = levelId.find_last_of("[");
            ASSERT(numpos != std::string::npos, XLevelPackInit, "Proxy - invalid multilevel id");
            ASSERT(levelId.find_first_not_of("0123456789", numpos+1) == levelId.size() -1,
                    XLevelPackInit, "Proxy - invalid multilevel id");
            std::istringstream subnumstr(levelId.substr(numpos + 1, levelId.size() - numpos - 2));
            subnumstr >> std::dec >> subnum;
            fileId = levelId.substr(0, numpos);
//            Log << "Multilevel Id '" << levelId << "' sublevel " << subnum << " fileId '" << fileId << "'\n";
        }

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
                        // levelpack on data/levels directory - depreceated
                        theNormLevelPath = levelPath.substr(2);
                    else
                        // a subdirectory or zip
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
        std::string cacheKey = theNormLevelPath + levelId + ecl::strf("%d", levelRelease);
        std::map<std::string, Proxy *>::iterator i = cache.find(cacheKey);
        if (i != cache.end()) {
            Proxy * candidate = i->second;
            return candidate;
        }

        if (!isMulti) {
            // create new proxy
            theProxy = new Proxy(false, thePathType, theNormLevelPath, levelId, levelTitle,
                levelAuthor, levelScoreVersion, levelRelease, levelHasEasymode,
                levelCompatibilty, status, levelRevision);
            cache.insert(std::make_pair(cacheKey, theProxy));
            return theProxy;
        } else {
            // search for existing proxy for multilevel file
            Proxy * fileProxy;
            std::string multiCacheKey = theNormLevelPath + fileId + ecl::strf("%d", levelRelease);
            std::map<std::string, Proxy *>::iterator mi = cache.find(multiCacheKey);
            if (i != cache.end()) {
                fileProxy = mi->second;
            } else {
                // create new multilevel file proxy
                fileProxy = new Proxy(false, thePathType, theNormLevelPath, fileId, levelTitle,
                    levelAuthor, levelScoreVersion, levelRelease, levelHasEasymode,
                    levelCompatibilty, status, levelRevision);
                cache.insert(std::make_pair(multiCacheKey, fileProxy));
            }
            // create sublevel proxy
            theProxy = new SubProxy(fileProxy, subnum, thePathType, theNormLevelPath, levelId, levelTitle);
            cache.insert(std::make_pair(cacheKey, theProxy));
            return theProxy;
        }

    }

    Proxy * Proxy::autoRegisterLevel(std::string indexPath, std::string filename, int subNum) {
        Proxy *theProxy = new Proxy(false, pt_resource, indexPath + "/" + filename , "", "",
            "unknown", 1, 0, false, GAMET_UNKNOWN, STATUS_UNKNOWN);
        try {
            theProxy->loadMetadata(true);
        }
        catch (XLevelLoading &err) {
            Log << "autoRegisterLevel load error on '" << indexPath << "/"<< filename << "\n";
            std::string message = _("Error on auto registration of levelfile: ");
            message += indexPath + "/" + filename + ".[xml/lua]\n\n";
            message += _("Note: the level will not show up in the \"Auto Folder\" levelpack!\n\n");
            message += err.what();
            gui::ErrorMenu m(message, N_("Continue"));
            m.manage();
        }
        if (theProxy->getId().empty() || theProxy->isLibraryFlag) {
            delete theProxy;
            theProxy = NULL;
        } else {
            // eliminate duplicates and register
//            Log << "autoRegisterLevel register '" << indexPath << "/"<< filename << " Title: " << theProxy->getTitle() <<"\n";
            std::string cacheKey = theProxy->getNormFilePath() + theProxy->getId() +
                    ecl::strf("%d", theProxy->getReleaseVersion());
            std::map<std::string, Proxy *>::iterator i = cache.find(cacheKey);
            if (i != cache.end()) {
                delete theProxy;
//                Log << "duplicate Proxy - " << cacheKey <<"\n";
                theProxy = i->second;
            } else {
                cache.insert(std::make_pair(cacheKey, theProxy));
            }
        }
        // multilevel
        if ((theProxy != NULL) &&
                (theProxy->isMultiFlag)) {
            std::string cacheKey = theProxy->getNormFilePath() + theProxy->getId() + ecl::strf("[%d]", subNum) +
                    ecl::strf("%d", theProxy->getReleaseVersion());
            std::map<std::string, Proxy *>::iterator i = cache.find(cacheKey);
            if (i != cache.end()) {
                theProxy = i->second;
            } else {
                // create sublevel proxy
                std::string fileTitle = theProxy->getTitle();
                bool isLuaTitle = (fileTitle[fileTitle.size() - 1] != '#');
                theProxy = new SubProxy(theProxy, subNum, pt_resource, indexPath + "/" + filename,
                    theProxy->getId() + ecl::strf("[%d]", subNum), "");
                cache.insert(std::make_pair(cacheKey, theProxy));
                if (isLuaTitle) {  // update Preview to get lua title
                    gui::LevelPreviewCache::instance()->updatePreview(theProxy);
                }
            }
        }
        return theProxy;
    }

    struct LowerCaseString {
        std::string low;
        LowerCaseString(const std::string& s) : low(s) {
            for (std::string::iterator i = low.begin(); i != low.end(); ++i)
                *i = tolower(*i);
        }
        bool containedBy(LowerCaseString other) const {
            return other.low.find(low) != string::npos;
        }
    };

    Index * searchIndex;
    LowerCaseString searchText("");
    void do_search(const std::map<std::string, Proxy *>::value_type pair) {
        Proxy * candidate = pair.second;
        if (searchText.containedBy(candidate->getNormFilePath()) ||
                searchText.containedBy(candidate->getTitle()) ||
                searchText.containedBy(candidate->getId()) ||
                searchText.containedBy(candidate->getAuthor())) {
            searchIndex->appendProxy(candidate);
//            Log << "Search result: " << pair.first << " - is - " << candidate->getTitle() << "\n";
        }
    }

    std::string Proxy::search(std::string text) {
        searchIndex = Index::findIndex("Search Result");
        // assert searchIndex
        searchIndex->clear();
        searchText = LowerCaseString(text);
        std::for_each(cache.begin(), cache.end(), do_search);
        return (searchIndex->size() > 0) ? searchIndex->getName() : "";
    }

    void Proxy::countLevels() {
        int countProxy = 0;
        int countLegacy = 0;
        int countStable = 0;
        int countReleased = 0;
        map<std::string, short> ids;
        for (std::map<std::string, Proxy *>::iterator i= cache.begin(); i != cache.end(); i++) {
            Proxy * candidate = (*i).second;
            countProxy++;
            if (candidate->getNormPathType() == pt_oxyd)
                countLegacy++;
            else if (candidate->getLevelStatus() == STATUS_STABLE)
                ids[candidate->getId()];
            else if (candidate->getLevelStatus() == STATUS_RELEASED)
                ++ids[candidate->getId()];
        }
        for (std::map<std::string, short>::iterator i= ids.begin(); i != ids.end(); i++) {
            if ((*i).second == 0)
                countStable++;
            else
                countReleased++;
        }

        Log << "Proxy count: " << countReleased <<" released, "
            << countStable << " stable, " << countLegacy << " legacy, "
            << countProxy << " total\n";
    }

    std::set<std::string> Proxy::getLevelIds(bool withEasy) {
        std::set<std::string> result;
        for (std::map<std::string, Proxy *>::iterator i= cache.begin(); i != cache.end(); i++) {
            if ((*i).second->hasEasyMode() == withEasy)
                result.insert((*i).second->getId());
        }
        return result;
    }

    std::set<Proxy *> Proxy::getProxies() {
        std::set<Proxy *> result;
        for (std::map<std::string, Proxy *>::iterator i= cache.begin(); i != cache.end(); i++) {
            result.insert((*i).second);
        }
        return result;
    }

    Proxy::Proxy(bool proxyIsLibrary, pathType thePathType, std::string theNormLevelPath,
            std::string levelId, std::string levelTitle, std::string levelAuthor,
            int levelScoreVersion, int levelRelease, bool levelHasEasymode,
            GameType levelCompatibilty,levelStatusType status, int levelRevision) :
            isLibraryFlag (proxyIsLibrary), isMultiFlag (false), normPathType(thePathType), normFilePath(theNormLevelPath),
            quantity (1), id(levelId), title(levelTitle), author(levelAuthor),
            scoreVersion(levelScoreVersion), releaseVersion(levelRelease),
            revisionNumber(levelRevision), hasEasyModeFlag(levelHasEasymode),
            engineCompatibility(levelCompatibilty), levelStatus (status),
            scoreUnit (duration), doc(NULL), loadtime (0) {
    }

    Proxy::~Proxy() {
        this->release();
    }

    void Proxy::release() {
        if (doc != NULL) {
            doc->release();
            doc = NULL;
        }
        if (this == cachedLevel) {
            cachedLevel = NULL;
            releaseLibs();
        }
    }

    std::string Proxy::getNormFilePath() {
        return normFilePath;
    }

    std::string Proxy::getNormLevelPath() {
        return normFilePath;
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
        load(false, true);
    }

    Proxy * Proxy::copy(std::string newBasePath, std::string newPackPath, bool backup) {
        bool useFileLoader = false;
        bool isXML = true;
        std::unique_ptr<std::istream> isptr;
        ByteVec levelCode;
        std::string absLevelPath = "";
        std::string filename;
        std::string filenameBase; // without extension

        if (normPathType == pt_oxyd) {
            return NULL;

        // resolve resource path to filepath
        } else if (normPathType == pt_absolute || normPathType == pt_url) {
            absLevelPath = normFilePath;
        } else if(normPathType == pt_resource) {
            if(!app.resourceFS->findFile("levels/" + normFilePath + ".xml",
                        absLevelPath, isptr) &&
                    !app.resourceFS->findFile("levels/" + normFilePath + ".lua",
                        absLevelPath, isptr)) {
                return NULL;
            }
        } else
            // error unknown type
            return NULL;

        size_t lastSlash = absLevelPath.rfind ('/');
        if (lastSlash == std::string::npos) {
            filename = absLevelPath;
        } else {
            filename = absLevelPath.substr(lastSlash + 1);
        }

        // xml or lua
        size_t extbegin = filename.rfind('.');
        if (extbegin != string::npos) {
            std::string ext = filename.substr(extbegin);

            if ( ext != ".lua" && ext!= ".ell" && ext != ".xml" && ext != ".elx") {
                return NULL;
            } else {
                filenameBase = filename.substr(0, extbegin);
            }
        } else {
                return NULL;
        }

        // load
        if (normPathType != pt_url) {
            // preload plain Lua file or zipped level
            if (isptr.get() != NULL) {
                // zipped file
                Readfile(*isptr, levelCode);
            } else {
                // plain file
                basic_ifstream<char> ifs(absLevelPath.c_str(), ios::binary | ios::in);
                Readfile(ifs, levelCode);
            }
            // auto-create the destination directory if necessary
            std::string directory = newBasePath + "/" + newPackPath;
            if (!ecl::FolderExists(directory))
                ecl::FolderCreate (directory);
            std::string oPath =  directory + "/" + filename;
            if (backup) {
                std::remove((oPath + "~").c_str());
                std::rename(oPath.c_str(), (oPath + "~").c_str());
            }
            basic_ofstream<char> ofs(oPath.c_str(), ios::binary | ios::out);
            for (int i = 0; i < levelCode.size(); i++) {
                ofs << levelCode[i];
            }
            ofs.close();
        } else {
            // load XML via Xerces
            return NULL;
        }

        // create new proxy
        return registerLevel(std::string("./")+filenameBase, newPackPath, id, title,
                        author, scoreVersion, releaseVersion, hasEasyModeFlag,
                        engineCompatibility, levelStatus, revisionNumber);
    }

    void Proxy::loadMetadata(bool expectLevel) {
        load(true, expectLevel);
    }

    void Proxy::load(bool onlyMetadata, bool expectLevel) {
        if (doc != NULL) {
            if (onlyMetadata)
                // doc exists - metadata are loaded
                return;
            if (!isLibraryFlag != expectLevel)
                    throw XLevelLoading(ecl::strf("Level - Library mismatch on %s", normFilePath.c_str()));
            // doc exists - we can directly load
            loadDoc();
            return;
        }

        Uint32 start_tick_time = SDL_GetTicks();   // meassure time for level loading
        bool useFileLoader = false;
        bool isXML = true;
        std::unique_ptr<std::istream> isptr;
        ByteVec levelCode;
        std::string errMessage;
        absLevelPath = "";

        // release current proxy
        if (!isLibraryFlag) {
            if (cachedLevel != NULL)
                cachedLevel->release();
            cachedLevel = this;
        }

        // handle oxyd first
        if (normPathType == pt_oxyd) {
            if(onlyMetadata)
                return;
            // set server flags
            server::SetCompatibility(this);
            server::EnigmaCompatibility = getEnigmaCompatibility();
            server::LevelStatus = getLevelStatus();
            if (server::EnigmaCompatibility < 1.10)
                server::AllowSingleOxyds = true;
            server::PrepareLua();
            // use oxyd loader
            std::string::size_type posSecondHash = normFilePath.find('#',1);
            if (posSecondHash == string::npos)
                throw XLevelLoading("Bad filename for oxyd level: " + normFilePath );
            std::string packName = normFilePath.substr(1, posSecondHash -1);
            std::string levelNumber = normFilePath.substr(posSecondHash + 1);
            if (Index * oxydIndex = Index::findIndex(packName)) {
                dynamic_cast<oxyd::LevelPack_Oxyd *>(oxydIndex)->load_oxyd_level(atoi(levelNumber.c_str()));
            } else {
                throw XLevelLoading("Missing oxyd levelpack for: " + normFilePath);
            }
            return;

        // resolve resource path to filepath
        } else if (normPathType == pt_absolute || normPathType == pt_url) {
            absLevelPath = normFilePath;
        } else if(normPathType == pt_resource) {
            if(!app.resourceFS->findFile ("levels/" + normFilePath + ".xml",
                        absLevelPath, isptr) &&
                    !app.resourceFS->findFile ("levels/" + normFilePath + ".lua",
                        absLevelPath, isptr)) {
                std::string type = isLibraryFlag ? "library " : "level ";
                throw XLevelLoading("Could not find " + type + normFilePath );
            }
        } else
            // error unknown type
            return;


        // xml or lua
        size_t extbegin = absLevelPath.rfind ('.');
        if (extbegin != string::npos) {
            std::string ext = absLevelPath.substr(extbegin);

            if (normPathType != pt_url && (ext == ".lua" || ext == ".ell")) {
                isXML = false;  // preliminary - may still be lua commented xml
            } else if (ext == ".xml" || ext == ".elx") {
                isXML = true;
            } else {
                throw XLevelLoading("Unknown file extension in " + absLevelPath);
            }
        } else {
            throw XLevelLoading("Unknown file extension in " + absLevelPath);
        }

        // preload file into buffer
        if (normPathType == pt_url) {
            // url address via curl
            Downloadfile(normFilePath, levelCode);
        } else if (isptr.get() != NULL) {
            // zipped file
            Readfile (*isptr, levelCode);
        } else {
            // plain file
            basic_ifstream<char> ifs(absLevelPath.c_str(), ios::binary | ios::in);
            Readfile (ifs, levelCode);
        }

        if(!isXML) {
            if (levelCode.size() >= 8 && std::string("--xml-- ").compare(
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
                const char *buffer = reinterpret_cast<const char *>(&levelCode[0]);
                // add debugging info to lua code
                std::string luaCode = "--@" + absLevelPath + "\n" +
                            buffer;
                lua_State *L = lua::LevelState();
                if (luaL_dostring(L, luaCode.c_str() ) != 0) {
                    lua_setglobal (L, "_LASTERROR");
                    throw XLevelLoading(lua::LastError(L));
                }
            } else {
                // ensure that metadata are consistent - called for all
                // new commanline lua levels
                if (releaseVersion == 0)
                    releaseVersion = 1;
            }
        }

        if (isXML) {
            // count XML header lines for proper Lua line count messages
            static const char *luaId = "<el:luamain><![CDATA[";
            int codeSize = levelCode.size();
            headerLines = 0;
            int idPtr = 0;
            bool pendingCR = false;
            for (int i = 0; i < codeSize; i++) {
                char c = levelCode[i];
                if (pendingCR && c == '\n') {
                    pendingCR = false;
                } else if (c == '\r') {
                    headerLines++;
                    pendingCR = true;
                    idPtr = 0;
                } else if (c == '\n') {
                    headerLines++;
                    pendingCR = false;
                    idPtr = 0;
                } else if (c == luaId[idPtr]) {
                    idPtr++;
                    if (idPtr == 20) {
                        break;
                    }
                }
            }

            // load XML
            try {
                std::ostringstream errStream;
                app.domParserErrorHandler->resetErrors();
                app.domParserErrorHandler->reportToOstream(&errStream);
                app.domParserSchemaResolver->resetResolver();
                app.domParserSchemaResolver->addSchemaId("level.xsd","level.xsd");

                // preloaded lua-commented xml or zipped xml
#if _XERCES_VERSION >= 30000
                std::unique_ptr<DOMLSInput> domInputLevelSource(new Wrapper4InputSource(
                    new MemBufInputSource(reinterpret_cast<const XMLByte *>(&levelCode[0]),
                                          levelCode.size(), absLevelPath.c_str(), false)));
                doc = app.domParser->parse(domInputLevelSource.get());
#else
                std::unique_ptr<Wrapper4InputSource> domInputLevelSource(new Wrapper4InputSource(
                    new MemBufInputSource(reinterpret_cast<const XMLByte *>(&levelCode[0]),
                                          levelCode.size(), absLevelPath.c_str(), false)));
                doc = app.domParser->parse(*domInputLevelSource);
#endif
                if (app.domParserSchemaResolver->didResolveSchema() && doc != NULL
                        && !app.domParserErrorHandler->getSawErrors()) {
                    infoElem = dynamic_cast<DOMElement *>(doc->getElementsByTagNameNS(
                            levelNS, Utf8ToXML("info").x_str())->item(0));
                    stringList = doc->getElementsByTagNameNS(levelNS,
                            Utf8ToXML("string").x_str());
                }
                if (app.domParserErrorHandler->getSawErrors()) {
                    errMessage = errStream.str();
                } else if (!app.domParserSchemaResolver->didResolveSchema()) {
                    errMessage = "Wrong XML document - expected a level, got something else!";
                }
                app.domParserErrorHandler->reportToNull();  // do not report to errStream any more
            }
            catch (...) {
                errMessage = "Unexpected XML Exception on load of level\n";
            }
            if (!errMessage.empty()) {
                release();           // empty or errornous doc
                Log << errMessage;   // make long error messages readable
                throw XLevelLoading (errMessage);
// todo: check metadata, handle shadowed levels
            } else {
                // check metadata - currently just overwrite
                isLibraryFlag = (getType() == "library") ? true : false;
                isMultiFlag = (getType() == "multilevel") ? true : false;
                if (!updateReleaseVersion()) {
                    release();   // avoid load success on a second read attempt
                    throw XLevelLoading(ecl::strf("Release version mismatch on %s: requested %d\n", normFilePath.c_str(), releaseVersion));
                }
                if (!updateId()) {
                    release();   // avoid load success on a second read attempt
                    throw XLevelLoading(ecl::strf("Id mismatch on %s: requested %s\n", normFilePath.c_str(), id.c_str()));
                }
                getQuantity();
                getTitle();
                getScoreVersion();
                getRevisionNumber();
                getLevelStatus();
                getAuthor();
                hasEasyMode();
                hasSingleMode();
                hasNetworkMode();
                getScoreUnit();
                getEngineCompatibility();
                if (!onlyMetadata){
                    if (!isLibraryFlag != expectLevel)
                        throw XLevelLoading(ecl::strf("Level - Library mismatch on %s", normFilePath.c_str()));
                    loadDoc();
                }
            }
        }
        loadtime = (SDL_GetTicks() - start_tick_time)/1000.0;
    }

    void Proxy::loadDoc() {
        if (getEnigmaCompatibility() > ENIGMACOMPATIBITLITY)
            throw XLevelLoading(ecl::strf("Level is incompatible: %s requires Enigma %.2f or above",
                    absLevelPath.c_str(), getEnigmaCompatibility()));
        if (this == cachedLevel) {    // just level - no libs
            server::SetCompatibility(this);
            server::EnigmaCompatibility = getEnigmaCompatibility();
            server::TwoPlayerGame = hasNetworkMode();
            server::LevelStatus = getLevelStatus();
            lua_evaluate_assert = (server::LevelStatus == STATUS_RELEASED || server::LevelStatus == STATUS_STABLE)
                    ? false || !noAssert : true;
            if (server::EnigmaCompatibility < 1.10) {
                server::AllowSingleOxyds = true;
                server::SurviveFinish = false;
            }
            server::PrepareLua();
        }
        processDependencies();
        processExternaldata();
        loadLuaCode();
    }

    void Proxy::processDependencies() {
        // cleanup on level but not on libs
        if (this == cachedLevel) {
            // cleanup all lib proxies loaded by previous load
            releaseLibs();
        }
        if (doc != NULL) {
            DOMNodeList *depList = infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("dependency").x_str());
            for (int i = 0, l = depList->getLength();  i < l; i++) {
                DOMElement *depElem = dynamic_cast<DOMElement *>(depList->item(i));
                std::string depPath;
                std::string depId;
                int         depRelease;
                bool        depPreload;
                std::string depUrl;
                depPath = XMLtoUtf8(depElem->getAttributeNS(levelNS,
                        Utf8ToXML("path").x_str())).c_str();
                depId = XMLtoUtf8(depElem->getAttributeNS(levelNS,
                        Utf8ToXML("id").x_str())).c_str();
                depRelease = XMLString::parseInt(depElem->getAttributeNS(levelNS,
                        Utf8ToXML("release").x_str()));
                depPreload = boolValue(depElem->getAttributeNS(levelNS,
                            Utf8ToXML("preload").x_str()));
                depUrl = XMLtoUtf8(depElem->getAttributeNS(levelNS,
                        Utf8ToXML("url").x_str())).c_str();
//                Log << "Deps: Path="<<depPath<<" Id="<< depId <<" Rel="<< depRelease <<" Prel="<< depPreload<< " Url=" << depUrl<<"\n";
                // load every dependency just once and break circular dependencies
                // by central load via Level
                cachedLevel->registerPreloadDependency(depPath, depId, depRelease,
                        depPreload, depUrl);
            }
        }
    }

    void Proxy::registerPreloadDependency(std::string depPath, std::string depId,
            int depRelease, bool depPreload, std::string depUrl) {
        // check if lib is already registered
        for (int i=0 ; i<loadedLibs.size(); i++) {
            if (loadedLibs[i]->getId() == depId) {
                if (loadedLibs[i]->getReleaseVersion() !=  depRelease)
                    throw XLevelLoading(ecl::strf("declaration of incompatible library version: '%s' release %d - release %d already loaded",
                            depId.c_str(), depRelease, loadedLibs[i]->getReleaseVersion()));
                else
                    return;
	    }
        }
        for (int i=0 ; i<registeredLibs.size(); i++) {
            if (registeredLibs[i]->getId() == depId) {
                if (registeredLibs[i]->getReleaseVersion() !=  depRelease) {
                    throw XLevelLoading(ecl::strf("declaration of incompatible library version: '%s' release %d - release %d already registered",
                            depId.c_str(), depRelease, registeredLibs[i]->getReleaseVersion()));
                } else if (depPreload == true) {
                    // same lib but now load it directly - delete the no preload entry
                    registeredLibs[i] = registeredLibs[registeredLibs.size() - 1];
                    registeredLibs.pop_back();
                    break;  // end search and do load lib
                } else {
                    // same lib and again no preload
                    return;
                }
	    }
        }

        // resolve relative lib paths
        if (depPath.find("./") == 0) {
            // relative lib path
            std::string levelDir;
            std::string levelFilename;
            if (ecl::split_path(normFilePath, &levelDir, &levelFilename))
                // the level is on subdirectory or in a zip
                depPath = levelDir + "/" +  depPath.substr(2);
            else
                // level on data/levels directory - depreceated
                depPath = depPath.substr(2);
        }

        // find lib in requested release version without or with release number
        // in filename
        Proxy * depProxy = new Proxy(true, depPath.empty() ? pt_url : pt_resource,
                (depPath.empty() ? depUrl : depPath) + ecl::strf("_%d",depRelease),
                depId, "", "", 0, depRelease, false, GAMET_ENIGMA, STATUS_UNKNOWN);
        try {
            depProxy->loadMetadata(false);
        } catch (XLevelLoading &err) {
            delete depProxy;
            depProxy = new Proxy(true, depPath.empty() ? pt_url : pt_resource,
                    depPath.empty() ? depUrl : depPath, depId, "", "", 0, depRelease,
                    false, GAMET_ENIGMA, STATUS_UNKNOWN);
            try {
                depProxy->loadMetadata(false);
            } catch (XLevelLoading &err) {
                delete depProxy;
                throw err;
            }
        }

        // load and register lib
        if (depPreload) {
            loadedLibs.push_back(depProxy);
            depProxy->load(false, false);
        } else {
            registeredLibs.push_back(depProxy);
        }

    }

    void Proxy::loadDependency(std::string depId) {
        // check if lib is already loaded
        for (int i=0 ; i<loadedLibs.size(); i++) {
            if (loadedLibs[i]->getId() == depId)
                // library is already loaded
                return;
        }

        for (int i=0 ; i<registeredLibs.size(); i++) {
            if (registeredLibs[i]->getId() == depId) {
                // library is registered to be loaded - do it
                loadedLibs.push_back(registeredLibs[i]);
                registeredLibs[i]->load(false, false);
                // delete form not yet loaded list
                registeredLibs[i] = registeredLibs[registeredLibs.size() - 1];
                registeredLibs.pop_back();
                return;
            }
        }
        if (doc == NULL) {
//        if (true) {
//            Log << "loadDependency " << depId << "\n";
            // handling for legacy Lua levels that did not register dependencies
            Proxy * depProxy = new Proxy(true, pt_resource, depId, depId, "", "", 0, 1,
                false, GAMET_ENIGMA, STATUS_UNKNOWN);
            loadedLibs.push_back(depProxy);
            depProxy->load(false, false);
            return;
        } else
            // xml levels have to register used libraries as dependencies
            throw XLevelLoading("load attempt of undeclared library");
    }

    void Proxy::processExternaldata() {
        if (doc != NULL) {
            DOMNodeList *extList = infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("externaldata").x_str());
            for (int i = 0, l = extList->getLength();  i < l; i++) {
                DOMElement *extElem = dynamic_cast<DOMElement *>(extList->item(i));
                std::string extNormPath;
                std::string extUrl;
                extNormPath = XMLtoUtf8(extElem->getAttributeNS(levelNS,
                        Utf8ToXML("path").x_str())).c_str();
                extUrl = XMLtoUtf8(extElem->getAttributeNS(levelNS,
                        Utf8ToXML("url").x_str())).c_str();
//                Log << "EData: Path="<<extNormPath<< " Url=" << extUrl<<"\n";
                // load every external data resource just once even if multiple urls are given
                if (externalData.find(extNormPath) == externalData.end()) {
                    std::unique_ptr<std::istream> isptr;
                    ByteVec extCode;

                    std::string extFilename;
                    std::string extPath;
                    bool isLocal = true;
                    if (extNormPath.find("./") == 0) {
                        extFilename = extNormPath.substr(2);
                        if (normPathType == pt_resource) {
                            extPath = normFilePath.substr(0, normFilePath.rfind('/') + 1) + extFilename + ".txt";
                        } else if (normPathType == pt_absolute) {
                            extPath = absLevelPath.substr(0, absLevelPath.rfind('/') + 1) + extFilename + ".txt";
                        } else
                            ASSERT(false, XLevelLoading, ("Invalid external data path '" + extNormPath + "'").c_str());
                    } else if  (extNormPath.find("externaldata/") == 0) {
                        extFilename = extNormPath.substr(13);
                        isLocal = false;
                        extPath = extNormPath + ".txt";
                    } else {
                        ASSERT(false, XLevelLoading, ("Invalid external data path '" + extNormPath + "'").c_str());
                    }

                    // TODO check extFilename no further /, no suffix, legal chars

                    // resolve resource path to filepath
                    std::string absExtPath;
//                    Log << "External data extPath " << extPath << "\n";
                    bool haveLocalCopy = true;
                    if (normPathType == pt_absolute) {
                        absExtPath = extPath;
                    } else {
                        haveLocalCopy = app.resourceFS->findFile("levels/" + extPath, absExtPath, isptr);
                    }

                    // preload plain file or zipped file
                    if (haveLocalCopy && isptr.get() != NULL) {
                        // zipped file
                        Readfile(*isptr, extCode);
                    } else if (haveLocalCopy) {
                        // plain file
                        basic_ifstream<char> ifs(absExtPath.c_str(), ios::binary | ios::in);
                        if (ifs)
                            Readfile(ifs, extCode);
                        else
                            haveLocalCopy = false;
                    }

                    if (!haveLocalCopy) {
                        // need to download via curl
                        absExtPath = app.userPath + "/levels/" + extPath;
//                        Log << "External data download via curl '" << extUrl << "' - '" << absExtPath <<"'\n";

                        // TODO try catch assertions with proper handling, checksums
                        Downloadfile(extUrl, extCode);

                        // save a local copy
                        std::string directory;
                        if (ecl::split_path(absExtPath, &directory, 0) && !ecl::FolderExists(directory)) {
                            ecl::FolderCreate (directory);
                        }
                        ofstream ofs(absExtPath.c_str(), ios::binary | ios::out);
                        ofs.write(&extCode[0], extCode.size());
                        ofs.close();
                        if (ofs.fail())
                            Log << "External data failed to write local copy to '" << absExtPath << "'\n";

                        // terminate the yet unterminated code string
                        extCode.push_back(0);
                    }

                    std::string extString = reinterpret_cast<const char *>(&extCode[0]);
                    Log << "External data loaded:\n" << extNormPath << "\n";
//                    Log << "External data loaded:\n" << extString << "\n";
                    externalData[extNormPath] = extString;
                }
            }
        }
    }

    std::string Proxy::getExternalData(std::string pathId) {
        std::map<std::string, std::string>::iterator itr = externalData.find(pathId); // [] access would create map entries!
        ASSERT(itr != externalData.end(), XLevelRuntime, ("Unknown external data access '" + pathId + "'\n").c_str());
        return itr->second;
    }

    void Proxy::loadLuaCode() {
        lua_State *L = lua::LevelState();
        DOMNodeList * luamainList = doc->getElementsByTagNameNS(levelNS, Utf8ToXML("luamain").x_str());
        if (luamainList->getLength() == 1) {
            DOMElement *luamain  = dynamic_cast<DOMElement *>(luamainList->item(0));
            // add debugging info to lua code
            std::string luaCode = "--@" + absLevelPath + std::string(headerLines, '\n') +
                        XMLtoUtf8(luamain->getTextContent()).c_str();
            if (luaL_dostring(L, luaCode.c_str() ) != 0) {
                lua_setglobal (L, "_LASTERROR");
                throw XLevelLoading(lua::LastError(L));
            }
        } else {
            throw XLevelLoading("not implemented");
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
        bool keyFound = false;
        bool protectedString = true;
        for (int i = 0, l = stringList-> getLength();  i < l && !translFound; i++) {
            DOMElement *stringElem = dynamic_cast<DOMElement *>(stringList->item(i));
            if (key == XMLtoUtf8(stringElem->getAttributeNS(levelNS,
                    Utf8ToXML("key").x_str())).c_str()) {
                keyFound = true;
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
                    bool translate = boolValue(englishElem->getAttributeNS(levelNS,
                                Utf8ToXML("translate").x_str()));
                    // we got the final english string
                    if ((lang == "en") || !translate) {
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
                    std::string tmp = _(english.c_str());
                    if (tmp != english) {
                        translation = tmp;
                        translFound = true;
                    }
                }
                protectedString = false;    // the next matching string is public
            }
        }
        if (!keyFound && !english.empty()) {
            // string may originate from a lib - still try gettext
            std::string tmp = _(english.c_str());
            if (tmp != english) {
                translation = tmp;
                translFound = true;
            }
         }
        return (translFound ? translation : english);
    }

    std::string Proxy::getType() {
        if (doc != NULL) {
            return XMLtoUtf8(infoElem->getAttributeNS(levelNS,
                    Utf8ToXML("type").x_str())).c_str();
        } else
            return "";
    }

    int Proxy::getQuantity() {
        if (doc != NULL) {
            quantity = XMLString::parseInt(infoElem->getAttributeNS(levelNS,
                    Utf8ToXML("quantity").x_str()));
        }
        return quantity;
    }

    bool Proxy::updateId() {
        if (doc != NULL) {
            DOMElement *identityElem =
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("identity").x_str())->item(0));
            std::string docId = XMLtoUtf8(identityElem->getAttributeNS(levelNS,
                    Utf8ToXML("id").x_str())).c_str();
            if (id.empty() || id[0] == '_') {
                // set yet undetermined id (auto folder, command line,...)
                id = docId;
                return true;
            } else if (id != docId)
                return false;
        }
        return true;
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

    bool Proxy::updateReleaseVersion() {
        if (doc != NULL) {
            DOMElement *versionElem =
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("version").x_str())->item(0));
            int docRelease = XMLString::parseInt(versionElem->getAttributeNS(levelNS,
                    Utf8ToXML("release").x_str()));
            if (releaseVersion == 0) {
                // set yet undetermined version
                releaseVersion = docRelease;
                return true;
            } else if (releaseVersion != docRelease) {
                return false;
            }
        }
        return true;
    }

    int Proxy::getReleaseVersion() {
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
                revisionNumber = 0;
            }
        }
        return revisionNumber;
    }

    levelStatusType Proxy::getLevelStatus() {
        if (doc != NULL) {
            DOMElement *versionElem =
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("version").x_str())->item(0));
            std::string status = XMLtoUtf8(versionElem->getAttributeNS(levelNS,
                    Utf8ToXML("status").x_str())).c_str();
            if (status == "released")
                levelStatus = STATUS_RELEASED;
            else if (status == "stable")
                levelStatus = STATUS_STABLE;
            else if (status == "test")
                levelStatus = STATUS_TEST;
            else if (status == "experimental")
                levelStatus = STATUS_EXPERIMENTAL;
        }
        return levelStatus;
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

    bool Proxy::hasEasyMode() {
        if (doc != NULL) {
            DOMElement *modesElem =
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("modes").x_str())->item(0));
            hasEasyModeFlag = boolValue(modesElem->getAttributeNS(levelNS,
                        Utf8ToXML("easy").x_str()));
        }
        return hasEasyModeFlag;
    }

    bool Proxy::hasSingleMode() {
        if (doc != NULL) {
            DOMElement *modesElem =
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("modes").x_str())->item(0));
            hasSingleModeFlag = boolValue(modesElem->getAttributeNS(levelNS,
                        Utf8ToXML("single").x_str()));
        }
        return hasSingleModeFlag;
    }

    bool Proxy::hasNetworkMode() {
        if (doc != NULL) {
            DOMElement *modesElem =
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("modes").x_str())->item(0));
            hasNetworkModeFlag = boolValue(modesElem->getAttributeNS(levelNS,
                        Utf8ToXML("network").x_str()));
        }
        return hasNetworkModeFlag;
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

    double Proxy::getEnigmaCompatibility() {
        double value = 0.92;
        if (doc != NULL) {
            DOMElement *compatibilityElem =
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("compatibility").x_str())->item(0));
            XMLDouble * result = new XMLDouble(compatibilityElem->getAttributeNS(levelNS,
                    Utf8ToXML("enigma").x_str()));
            value = result->getValue();
            delete result;
        }
        return value;
    }

    GameType Proxy::getEngineCompatibility() {
        if (doc != NULL) {
            DOMElement *authorElem =
                    dynamic_cast<DOMElement *>(infoElem->getElementsByTagNameNS(
                    levelNS, Utf8ToXML("compatibility").x_str())->item(0));
            engineCompatibility = GetGameType(XMLtoUtf8(authorElem->getAttributeNS(levelNS,
                    Utf8ToXML("engine").x_str())).c_str());
        }
        return engineCompatibility;
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

    double Proxy::getLoadtime() {
        return loadtime;
    }
}} // namespace enigma::lev
