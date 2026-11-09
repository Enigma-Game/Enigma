/*
 * Copyright (C) 2006,2007,2008,2009,2010 Ronald Lamprecht
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

#include "lev/PersistentIndex.hh"

#include "ecl_font.hh"
#include "ecl_system.hh"
#include "errors.hh"
#include "file.hh"
#include "gui/ErrorMenu.hh"
#include "lev/Proxy.hh"
#include "lev/RatingManager.hh"
#include "LocalToXML.hh"
#include "main.hh"
#include "nls.hh"
#include "Utf8ToXML.hh"
#include "utilXML.hh"
#include "XMLtoUtf8.hh"

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>
#include <xercesc/util/XercesVersion.hpp>
#include <xercesc/util/XMLDouble.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

using namespace std;
XERCES_CPP_NAMESPACE_USE

namespace enigma::lev {

namespace {
    // Does 'str' end with 'suffix'?
    bool hasSuffix(const std::string& str, const std::string& suffix) {
        return str.size() >= suffix.size()
                && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }
} // namespace

Variation::Variation(controlType ctrlValue, scoreUnitType unitValue, const std::string& targetValue)
    : ctrl(ctrlValue), unit(unitValue), target(targetValue) {
}

bool Variation::operator==(const Variation& otherVar) const {
    return ctrl == otherVar.ctrl && unit == otherVar.unit && target == otherVar.target
            && extensions == otherVar.extensions;
}

PersistentIndex* PersistentIndex::historyIndex = nullptr;
std::vector<std::shared_ptr<PersistentIndex>> PersistentIndex::indexCandidates;
std::vector<std::unique_ptr<PersistentIndex>> PersistentIndex::persistentIndices;

void PersistentIndex::checkCandidate(const std::string& thePackPath, bool systemOnly,
        bool userOwned, bool isAuto, bool isSystemCross, bool isUserCross, double defaultLocation,
        const std::string& anIndexName, std::string theIndexFilename,
        const std::string& aDescription, const std::string& aGroupName) {
    int systemPackIndex = -1;
    if (!isAuto && !isUserCross) {
        for (unsigned i = 0; i < indexCandidates.size(); i++) {
            if (indexCandidates[i]->packPath == thePackPath) {
                if (!systemOnly && isSystemCross) { // a system cross-index update like enigma_cross
                    std::string::size_type p = theIndexFilename.find_last_of('_');
                    if (p == std::string::npos) // ignore wizard saves of system cross-indices
                        return;
                    std::string indexBase = theIndexFilename.substr(0, p);
                    if (indexBase + ".xml" == indexCandidates[i]->indexFilename) {
                        if (theIndexFilename
                                == indexBase + ecl::strf("_%d.xml", indexCandidates[i]->release)) {
                            // it is an update to an already loaded system pack
                            systemPackIndex = i;
                            break;
                        } else {
                            // wrong release - reject
                            return;
                        }
                    } else {
                        // update to another system index
                        continue;
                    }
                } else if (systemOnly && isSystemCross
                        && theIndexFilename == indexCandidates[i]->indexFilename) {
                    systemPackIndex = i;
                    break;
                } else if (!isSystemCross) {
                    // it is an update to an already loaded system pack
                    systemPackIndex = i;
                    userOwned = false;
                    if (!systemOnly) {
                        // no changes for multiple sys paths like ./data shadowing installed versions
                        // add release to index name to retrieve the matching version, e.g. "index_2.xml"
                        theIndexFilename = theIndexFilename.insert(theIndexFilename.size() - 4,
                                ecl::strf("_%d", indexCandidates[i]->release));
                    }
                    break;
                }
            }
        }
    }
    std::shared_ptr<PersistentIndex> candidate = std::make_shared<PersistentIndex>(thePackPath,
            systemOnly, userOwned, isAuto, defaultLocation, anIndexName, theIndexFilename,
            aDescription, aGroupName);
    if (candidate->getName().empty() || candidate->getCompatibility() > ENIGMACOMPATIBITLITY) {
        candidate.reset();
    } else {
        // check if new Index is an update of another
        if (systemPackIndex >= 0) {
            if (indexCandidates[systemPackIndex]->getRevision() >= candidate->getRevision()) {
                candidate.reset();
                return;
            } else {
                // it is an update
                indexCandidates[systemPackIndex] = candidate;
                return;
            }
        } else {
            // check for levelpacks that would be overridden by the button name
            for (const auto& indexCandidate : indexCandidates) {
                if (indexCandidate->getName() == candidate->getName()) {
                    // always prefer the first one, the one with more official background
                    candidate.reset();
                    return;
                }
            }
            indexCandidates.push_back(candidate);
        }
    }
}

void PersistentIndex::registerPersistentIndex(std::unique_ptr<PersistentIndex> index) {
    Index::registerIndex(index.get());
    persistentIndices.push_back(std::move(index));
}

void PersistentIndex::registerPersistentIndices(bool onlySystemIndices) {
    DirEntry dirEntry;

    // SystemPath: register directories and ZIP files with xml-indices
    std::vector<std::string> sysPaths = app.systemFS->getPaths();
    std::set<std::string> candidates;
    for (const string& sysPath : sysPaths) {
        std::unique_ptr<DirIter> dirIter = DirIter::create(sysPath + "/levels");
        while (dirIter->get_next(dirEntry)) {
            if (dirEntry.is_dir && dirEntry.name != "." && dirEntry.name != ".."
                    && dirEntry.name != ".svn" && dirEntry.name != "enigma_cross"
                    && dirEntry.name != "soko") {
                candidates.insert(dirEntry.name);
            } else {
                if (hasSuffix(dirEntry.name, ".zip"))
                    candidates.insert(dirEntry.name.substr(0, dirEntry.name.size() - 4));
            }
        }

        // check for sokoball levelpacks
        dirIter = DirIter::create(sysPath + "/levels/soko");
        while (dirIter->get_next(dirEntry)) {
            if (dirEntry.is_dir && dirEntry.name != "." && dirEntry.name != ".."
                    && dirEntry.name != ".svn") {
                candidates.insert("soko/" + dirEntry.name);
            } else {
                if (hasSuffix(dirEntry.name, ".zip")) {
                    candidates.insert("soko/" + dirEntry.name.substr(0, dirEntry.name.size() - 4));
                }
            }
        }
    }

    for (const std::string& candidate : candidates) {
        // register the index just on the system path even if the
        // user has an update on his path. We need to know the release to
        // decide if a user copy is an update
        checkCandidate(candidate, true, false);
    }

    // add system cross-indices
    for (const string& sysPath : sysPaths) {
        std::unique_ptr<DirIter> dirIter = DirIter::create(sysPath + "/levels/enigma_cross");
        while (dirIter->get_next(dirEntry)) {
            if (!dirEntry.is_dir && hasSuffix(dirEntry.name, ".xml")) {
                checkCandidate("enigma_cross", true, false, false, true, false,
                        INDEX_DEFAULT_PACK_LOCATION, "", dirEntry.name);
            }
        }
    }

    if (onlySystemIndices)
        return;

    // register index-free auto folder
    // this needs to be done prior history registration to avoid outdated proxies
    std::unique_ptr<PersistentIndex> autoIndex = std::make_unique<PersistentIndex>("auto", false, true, true,
            INDEX_AUTO_PACK_LOCATION, INDEX_AUTO_PACK_NAME, INDEX_STD_FILENAME,
            INDEX_AUTO_PACK_DESCRIPTION);
    autoIndex->isEditable = false;
    registerPersistentIndex(std::move(autoIndex));

    // register team auto not yet registered new files
    std::unique_ptr<PersistentIndex> teamautoIndex = std::make_unique<PersistentIndex>(
            "team_test_new_api", false, true, true, 75000, "test_new_api");
    if (teamautoIndex->size() > 0) {
        teamautoIndex->isEditable = false;
        registerPersistentIndex(std::move(teamautoIndex));
    }

    // UserPath: register directories and ZIP files with XML-indices, but
    // excluding the auto folder.
    std::set<std::string> candidates2;
    std::unique_ptr<DirIter> dirIter = DirIter::create(app.userPath + "/levels");
    while (dirIter->get_next(dirEntry)) {
        if (dirEntry.is_dir && dirEntry.name != "." && dirEntry.name != ".."
                && dirEntry.name != ".svn" && dirEntry.name != "auto" && dirEntry.name != "cross"
                && dirEntry.name != "enigma_cross" && dirEntry.name != "legacy_dat"
                && dirEntry.name != "soko") {
            candidates2.insert(dirEntry.name);
        } else {
            if (hasSuffix(dirEntry.name, ".zip")) {
                candidates2.insert(dirEntry.name.substr(0, dirEntry.name.size() - 4));
            }
        }
    }

    // User Path: register sokoballs
    dirIter = DirIter::create(app.userPath + "/levels/soko");
    while (dirIter->get_next(dirEntry)) {
        if (dirEntry.is_dir && dirEntry.name != "." && dirEntry.name != ".."
                && dirEntry.name != ".svn") {
            candidates2.insert("soko/" + dirEntry.name);
        } else {
            if (hasSuffix(dirEntry.name, ".zip")) {
                candidates2.insert("soko/" + dirEntry.name.substr(0, dirEntry.name.size() - 4));
            }
        }
    }

    candidates2.insert("");
#if defined(_WIN32) || defined(__MINGW32__)
    // eliminate logical duplicates as Windows does not distinguish
    // upper and lower case filenames, but we can have e.g. an uppercase zip
    // and a lower case dir
    candidates2 = ecl::UniqueFilenameSet(candidates2);
#endif
    for (auto i = candidates2.begin(); i != candidates2.end(); ++i) {
        checkCandidate(*i, false, true);
    }

    // add system cross-indices updates
    dirIter = DirIter::create(app.userPath + "/levels/enigma_cross");
    while (dirIter->get_next(dirEntry)) {
        if (!dirEntry.is_dir && hasSuffix(dirEntry.name, ".xml")) {
            checkCandidate("enigma_cross", false, false, false, true, false,
                    INDEX_DEFAULT_PACK_LOCATION, "", dirEntry.name);
        }
    }

    // add user cross-indices
    dirIter = DirIter::create(app.userPath + "/levels/cross");
    while (dirIter->get_next(dirEntry)) {
        if (!dirEntry.is_dir && hasSuffix(dirEntry.name, ".xml")) {
            checkCandidate("cross", false, true, false, false, true, INDEX_DEFAULT_PACK_LOCATION,
                    "", dirEntry.name);
        }
    }

    for (unsigned i = 0; i < indexCandidates.size(); i++) {
        registerIndex(indexCandidates[i].get());
    }

    // check if history is available - else generate a new index
    Index* foundHistory = findIndex("History");
    if (foundHistory != nullptr) {
        historyIndex = dynamic_cast<PersistentIndex*>(foundHistory);
    } else {
        historyIndex = new PersistentIndex("cross", false, true, false, INDEX_HISTORY_PACK_LOCATION,
                INDEX_HISTORY_PACK_NAME, "history.xml", INDEX_HISTORY_PACK_DESCRIPTION);
        registerIndex(historyIndex);
    }
    historyIndex->isEditable = false;
}

void PersistentIndex::addCurrentToHistory() {
    Proxy* curProxy = Index::getCurrentProxy();
    // remember all but commandline absolute and relative paths
    if (curProxy != nullptr && curProxy->getNormPathType() != Proxy::pt_absolute) {
        Variation var;
        PersistentIndex* curIndex = dynamic_cast<PersistentIndex*>(Index::getCurrentIndex());
        if (curIndex != nullptr)
            var = curIndex->getVariation(curIndex->getCurrentPosition());
        historyIndex->insertProxy(
                0, curProxy, false, var.ctrl, var.unit, var.target, var.extensions);
        if (historyIndex->size() > 100)
            historyIndex->erase(historyIndex->size() - 1);
        historyIndex->setCurrentPosition(0); // last played is always current in history
    }
}

PersistentIndex::PersistentIndex(const std::string& thePackPath, bool loadSystemFS, bool userOwned,
        bool autoLoading, double defaultLocation, const std::string& anIndexName,
        const std::string& theIndexFilename, const std::string& aDescription,
        const std::string& aGroupName)
    : Index(anIndexName, aDescription, aGroupName, defaultLocation), packPath(thePackPath),
      indexFilename(theIndexFilename), release(1), revision(1), compatibility(1.00),
      isModified(false), isUserOwned(userOwned), isEditable(true), isAuto(autoLoading) {
    load(loadSystemFS);
}

int autoIndexProxyCompare(Proxy* first, Proxy* second) {
    return first->getNormLevelPath() < second->getNormLevelPath();
}

void PersistentIndex::load(bool loadSystemFS, bool update) {
    if (doc != nullptr) {
        doc->release();
        doc = nullptr;
    }
    // new levelpacks are not loadable
    if (packPath == " ")
        return;

    if (isAuto) {
        std::unique_ptr<DirIter> dirIter;
        DirEntry dirEntry;
        dirIter = DirIter::create(app.userPath + "/levels/" + packPath);
        while (dirIter->get_next(dirEntry)) {
            if (dirEntry.is_dir)
                continue;
            if (hasSuffix(dirEntry.name, ".xml") || hasSuffix(dirEntry.name, ".lua")) {
                Proxy* newProxy = Proxy::autoRegisterLevel(
                        packPath, dirEntry.name.substr(0, dirEntry.name.size() - 4), 1);
                if (newProxy != nullptr) {
                    // first check that the proxy is not in the index
                    //  - may occur if the level is stored as .xml and .lua in the folder
                    if (!containsProxy(newProxy)) {
                        // it is new, add it
                        appendProxy(newProxy);
                    }
                    // do not delete Proxy if not used - we are not the owner!
                    // register additional sublevels
                    for (int i = 2; i <= newProxy->getQuantity(); i++) {
                        Proxy* newSubProxy = Proxy::autoRegisterLevel(
                                packPath, dirEntry.name.substr(0, dirEntry.name.size() - 4), i);
                        appendProxy(newSubProxy);
                    }
                }
            }
        }

        std::sort(proxies.begin(), proxies.end(), autoIndexProxyCompare);
        return;
    }

    ByteVec indexCode;
    std::stringstream indexCodeStream;
    absIndexPath = "";
    std::string relIndexPath = "levels/" + packPath + "/" + indexFilename;
    if ((!loadSystemFS && app.resourceFS->findFile(relIndexPath, absIndexPath, indexCodeStream))
            || (loadSystemFS
                    && app.systemFS->findFile(relIndexPath, absIndexPath, indexCodeStream))) {
        std::string errMessage;
        if (indexCodeStream.rdbuf()->in_avail()) {
            // zipped file
            Readfile(indexCodeStream, indexCode);
        } else {
            // plain file
            std::basic_ifstream<char> ifs(absIndexPath.c_str(), ios::binary | ios::in);
            Readfile(ifs, indexCode);
        }
        try {
            std::ostringstream errStream;
            app.domParserErrorHandler->resetErrors();
            app.domParserErrorHandler->reportToOstream(&errStream);
            app.domParserSchemaResolver->resetResolver();
            app.domParserSchemaResolver->addSchemaId("index.xsd", "index.xsd");
            if (update) {
                if (!Robinson)
                    // local XML file or URL
                    doc = app.domParser->parseURI(indexUrl.c_str());
            } else {
                // preloaded XML or zipped XML
                std::unique_ptr<DOMLSInput> domInputIndexSource(new Wrapper4InputSource(
                        new MemBufInputSource(reinterpret_cast<const XMLByte*>(&indexCode[0]),
                                indexCode.size(), absIndexPath.c_str(), false)));
                doc = app.domParser->parse(domInputIndexSource.get());
            }

            if (app.domParserSchemaResolver->didResolveSchema() && doc != nullptr
                    && !app.domParserErrorHandler->getSawErrors()) {
                infoElem = dynamic_cast<DOMElement*>(
                        doc->getElementsByTagName(Utf8ToXML("info").x_str())->item(0));
                updateElem = dynamic_cast<DOMElement*>(
                        doc->getElementsByTagName(Utf8ToXML("update").x_str())->item(0));
                levelsElem = dynamic_cast<DOMElement*>(
                        doc->getElementsByTagName(Utf8ToXML("levels").x_str())->item(0));
                DOMNodeList* descList = doc->getElementsByTagName(Utf8ToXML("description").x_str());
                if (descList->getLength() != 0) {
                    DOMElement* descElem = dynamic_cast<DOMElement*>(descList->item(0));
                    indexDescription = XMLtoUtf8(descElem->getTextContent()).c_str();
                    indexDescription = ecl::normalizeSpaces(indexDescription);
                }
            }

            if (update && Robinson) {
                errMessage = "Robinson rejects update.";
            } else if (app.domParserErrorHandler->getSawErrors()) {
                errMessage = errStream.str();
            } else if (!app.domParserSchemaResolver->didResolveSchema()) {
                errMessage = "Wrong XML document - expected a levelpack index, got something else!";
            }
            app.domParserErrorHandler->reportToNull(); // do not report to errStream anymore
        } catch (...) {
            errMessage = "Unexpected XML Exception on load of index\n";
        }
        if (!errMessage.empty()) {
            Log << errMessage; // make long error messages readable
            std::string message;
            if (update) {
                message = _("Error on update of levelpack index: \n");
                message += absIndexPath + "\n\n";
                message += _("Note: the current version will be reloaded!\n\n");
                message += errMessage;
            } else {
                if (doc != nullptr) {
                    doc->release(); // empty or erroneous doc
                    doc = nullptr;
                }
                message = _("Error on registration of levelpack index: \n");
                message += absIndexPath + "\n\n";
                message += _("Note: the levelpack will not show up!\n\n");
                message += errMessage;
            }
            gui::ErrorMenu m(message, N_("Continue"));
            m.manage();
            if (update) {
                load(loadSystemFS, false); // reload local version
            }
            return;
        } else if (doc != nullptr) {
            // TODO check if an updated index exists for system packs
            loadDoc();
        }
    }
}

void PersistentIndex::loadDoc() {
    if (doc != nullptr) {
        clear(); // allow a reload of an index
        indexName = XMLtoUtf8(infoElem->getAttribute(Utf8ToXML("title").x_str())).c_str();
        indexGroup = XMLtoUtf8(infoElem->getAttribute(Utf8ToXML("group").x_str())).c_str();
        defaultGroup = indexGroup;
        owner = XMLtoUtf8(infoElem->getAttribute(Utf8ToXML("owner").x_str())).c_str();
        release = XMLString::parseInt(infoElem->getAttribute(Utf8ToXML("release").x_str()));
        revision = XMLString::parseInt(infoElem->getAttribute(Utf8ToXML("revision").x_str()));
        XMLDouble* result = new XMLDouble(infoElem->getAttribute(Utf8ToXML("enigma").x_str()));
        compatibility = result->getValue();
        delete result;
        result = new XMLDouble(infoElem->getAttribute(Utf8ToXML("location").x_str()));
        indexDefaultLocation = result->getValue();
        indexLocation = indexDefaultLocation;
        delete result;

        if (updateElem != nullptr) {
            indexUrl = XMLtoUtf8(updateElem->getAttribute(Utf8ToXML("indexurl").x_str())).c_str();
        }
        DOMNodeList* levelList = levelsElem->getElementsByTagName(Utf8ToXML("level").x_str());
        std::set<std::string> knownAttributes;
        knownAttributes.insert("_seq");
        knownAttributes.insert("_title");
        knownAttributes.insert("_xpath");
        knownAttributes.insert("id");
        knownAttributes.insert("author");
        knownAttributes.insert("score");
        knownAttributes.insert("rel");
        knownAttributes.insert("rev");
        knownAttributes.insert("easy");
        knownAttributes.insert("ctrl");
        knownAttributes.insert("unit");
        knownAttributes.insert("target");
        for (int i = 0, l = levelList->getLength(); i < l; i++) {
            DOMElement* levelElem = dynamic_cast<DOMElement*>(levelList->item(i));
            std::string path = XMLtoUtf8(levelElem->getAttribute(Utf8ToXML("_xpath").x_str()))
                                       .c_str();
            std::string id = XMLtoUtf8(levelElem->getAttribute(Utf8ToXML("id").x_str())).c_str();
            std::string title = XMLtoUtf8(levelElem->getAttribute(Utf8ToXML("_title").x_str()))
                                        .c_str();
            std::string author = XMLtoUtf8(levelElem->getAttribute(Utf8ToXML("author").x_str()))
                                         .c_str();
            int scoreVersion = XMLString::parseInt(
                    levelElem->getAttribute(Utf8ToXML("score").x_str()));
            int releaseVersion = XMLString::parseInt(
                    levelElem->getAttribute(Utf8ToXML("rel").x_str()));
            int revisionVersion = XMLString::parseInt(
                    levelElem->getAttribute(Utf8ToXML("rev").x_str()));
            bool hasEasymodeFlag = boolValue(levelElem->getAttribute(Utf8ToXML("easy").x_str()));
            Proxy* newProxy = Proxy::registerLevel(path, packPath, id, title, author, scoreVersion,
                    releaseVersion, hasEasymodeFlag, GAMET_ENIGMA, STATUS_RELEASED,
                    revisionVersion);
            Variation var;
            std::string controlString = XMLtoUtf8(
                    levelElem->getAttribute(Utf8ToXML("ctrl").x_str()))
                                                .c_str();
            if (controlString == "balance")
                var.ctrl = balance;
            else if (controlString == "key")
                var.ctrl = key;
            else if (controlString == "other")
                var.ctrl = other;
            std::string txt = XMLtoUtf8(levelElem->getAttribute(Utf8ToXML("unit").x_str())).c_str();
            if (txt == "number")
                var.unit = number;
            else
                // default
                var.unit = duration;
            var.target = XMLtoUtf8(levelElem->getAttribute(Utf8ToXML("target").x_str())).c_str();
            DOMNamedNodeMap* attrMap = levelElem->getAttributes();
            for (int j = 0, k = attrMap->getLength(); j < k; j++) {
                DOMAttr* levelAttr = dynamic_cast<DOMAttr*>(attrMap->item(j));
                std::string attrName = XMLtoUtf8(levelAttr->getName()).c_str();
                if (knownAttributes.find(attrName) == knownAttributes.end()) {
                    Log << "PersistentIndex load unknown attribute: " << attrName << "\n";
                    var.extensions[attrName] = XMLtoUtf8(levelAttr->getValue()).c_str();
                }
            }
            appendProxy(newProxy, var.ctrl, var.unit, var.target, var.extensions);
        }
    }
}

PersistentIndex::~PersistentIndex() {
    if (doc != nullptr)
        doc->release();
}

// Must be called before XMLPlatformUtils::terminate
void PersistentIndex::shutdown() {
    indexCandidates.clear();
    persistentIndices.clear();
}

std::string PersistentIndex::getPackPath() {
    return packPath;
}

bool PersistentIndex::setName(std::string newName, bool isSokoball) {
    if (findIndex(newName) != nullptr)
        return false; // do not allow duplicate names

    // substitute spaces by underscores
    std::string fileName = newName;
    std::string::size_type pos = fileName.find_first_of(' ', 0);
    while (pos != std::string::npos) {
        fileName.replace(pos, 1, "_");
        pos = fileName.find_first_of(' ', pos + 1);
    }

    if (packPath == " " || (packPath == "cross" && indexFilename == INDEX_STD_FILENAME)) {
        // generate usable path name and check it is usable
        if (fileName == "cross" || fileName == "enigma_cross" || fileName == "legacy_dat"
                || fileName == "auto" || fileName == "soko" || fileName == "history") {
            return false;
        }
        // check if the name would conflict with existing files
        std::stringstream otherFileContent;
        absIndexPath = "";
        std::string relIndexPath1 = "levels/" + fileName + "/" + INDEX_STD_FILENAME;
        std::string relIndexPath2 = "levels/cross/" + fileName + ".xml";
        if (app.resourceFS->findFile(relIndexPath1, absIndexPath, otherFileContent)
                || app.resourceFS->findFile(relIndexPath2, absIndexPath, otherFileContent)) {
            return false;
        }
        if (packPath == " ") {
            packPath = (isSokoball ? "soko/" : "") + fileName;
            indexFilename = INDEX_STD_FILENAME;
        } else
            indexFilename = fileName + ".xml";
    }

    if (!getName().empty())
        renameIndex(newName); // Index maps
    else
        // a new unregistered levelpack is named the first time - just set the name
        indexName = newName;
    return true;
}

bool PersistentIndex::isUpdatable() const {
    return (updateElem != nullptr) && !indexUrl.empty();
}

bool PersistentIndex::isCross() const {
    return packPath == "cross" || packPath == "enigma_cross";
}

void PersistentIndex::markNewAsCross() {
    if (packPath == " ")
        packPath = "cross";
}

std::string PersistentIndex::getOwner() {
    return owner;
}

void PersistentIndex::setOwner(const std::string& newOwner) {
    owner = newOwner;
}

int PersistentIndex::getRelease() const {
    return release;
}

void PersistentIndex::setRelease(int newRelease) {
    release = newRelease;
}

int PersistentIndex::getRevision() const {
    return revision;
}

void PersistentIndex::setRevision(int newRevision) {
    revision = newRevision;
}

double PersistentIndex::getCompatibility() const {
    return compatibility;
}

void PersistentIndex::setCompatibility(double newCompatibility) {
    compatibility = newCompatibility;
}

bool PersistentIndex::isUserEditable() const {
    return isEditable && (isUserOwned || WizardMode);
}

void PersistentIndex::clear() {
    proxies.clear();
    variations.clear();
    currentPosition = 0;
}

void PersistentIndex::appendProxy(Proxy* newLevel, controlType varCtrl, scoreUnitType varUnit,
        std::string varTarget, std::map<std::string, std::string> varExtensions) {
    proxies.push_back(newLevel);

    Variation var(varCtrl, varUnit, varTarget);
    var.extensions = varExtensions;
    variations.push_back(var);
}

void PersistentIndex::insertProxy(int pos, Proxy* newLevel, bool allowDuplicates,
        controlType varCtrl, scoreUnitType varUnit, const std::string& varTarget,
        const std::map<std::string, std::string>& varExtensions) {
    // TODO Assert pos >= size

    Variation var(varCtrl, varUnit, varTarget);
    var.extensions = varExtensions;

    if (!allowDuplicates) {
        // delete duplicates
        auto itProxy = proxies.begin();
        auto itVar = variations.begin();
        while (itProxy != proxies.end()) {
            if (*itProxy == newLevel && *itVar == var) {
                itProxy = proxies.erase(itProxy);
                itVar = variations.erase(itVar);
            }
            if (itProxy != proxies.end()) {
                ++itProxy;
                ++itVar;
            }
        }
    }

    auto itProxy = proxies.begin();
    auto itVar = variations.begin();
    for (int i = 0; i < pos; i++) {
        ++itProxy;
        ++itVar;
    }
    proxies.insert(itProxy, newLevel);
    variations.insert(itVar, var);
}

Variation PersistentIndex::getVariation(int pos) {
    // TODO Assert pos
    return variations[pos];
}

void PersistentIndex::erase(int pos) {
    auto itProxy = proxies.begin();
    auto itVar = variations.begin();
    for (int i = 0; i < pos; i++) {
        ++itProxy;
        ++itVar;
    }
    proxies.erase(itProxy);
    variations.erase(itVar);
}

void PersistentIndex::exchange(int pos1, int pos2) {
    Proxy* proxy = proxies[pos1];
    proxies[pos1] = proxies[pos2];
    proxies[pos2] = proxy;
    Variation var = variations[pos1];
    variations[pos1] = variations[pos2];
    variations[pos2] = var;
    if (getCurrentPosition() == pos1)
        setCurrentPosition(pos2);
    else if (getCurrentPosition() == pos2)
        setCurrentPosition(pos1);
}

bool PersistentIndex::isSource(Proxy* aProxy) {
    std::string proxyPath = aProxy->getNormFilePath();
    if (proxyPath[0] == '#')
        // Oxyd reference
        return false;
    else if (packPath.empty()) {
        // old levelpack on levels directory
        if (proxyPath.find('/') == std::string::npos)
            return true;
        else
            return false;
    } else {
        if (proxyPath.find(packPath + "/") == 0)
            return true;
        else
            return false;
    }
}

void PersistentIndex::updateFromFolder() {
    if (!isAuto)
        return;

    // Force a reload of metadata to update existing proxies.
    // For multilevels, it is essential to do this prior to the reload.
    Index::updateFromProxies();
    clear();
    // now reload - existing proxies are reused, but we have updated them already
    load(false);
}

bool PersistentIndex::save(bool allowOverwrite) {
    bool result;

    if (doc == nullptr) {
        std::string indexTemplatePath;
        if (app.systemFS->findFile("schemas/index.xml", indexTemplatePath)) {
            std::string errMessage;
            try {
                std::ostringstream errStream;
                app.domParserErrorHandler->resetErrors();
                app.domParserErrorHandler->reportToOstream(&errStream);
                app.domParserSchemaResolver->resetResolver();
                app.domParserSchemaResolver->addSchemaId("index.xsd", "index.xsd");
                doc = app.domParser->parseURI(indexTemplatePath.c_str());
                if (app.domParserSchemaResolver->didResolveSchema() && doc != nullptr
                        && !app.domParserErrorHandler->getSawErrors()) {
                    infoElem = dynamic_cast<DOMElement*>(
                            doc->getElementsByTagName(Utf8ToXML("info").x_str())->item(0));
                    levelsElem = dynamic_cast<DOMElement*>(
                            doc->getElementsByTagName(Utf8ToXML("levels").x_str())->item(0));
                }
                if (app.domParserErrorHandler->getSawErrors()) {
                    errMessage = errStream.str();
                } else if (!app.domParserSchemaResolver->didResolveSchema()) {
                    errMessage = "Wrong XML document - expected a levelpack index, got something "
                                 "else!";
                }
                app.domParserErrorHandler->reportToNull(); // do not report to errStream anymore
            } catch (...) {
                errMessage = "Unexpected XML Exception on load of index\n";
            }
            if (!errMessage.empty()) {
                if (doc != nullptr) {
                    doc->release(); // empty or erroneous doc
                    doc = nullptr;
                }
                Log << errMessage; // make long error messages readable
                return false;
            }
        } else
            // TODO add error handling
            return false;
    }

    //

    infoElem->setAttribute(Utf8ToXML("title").x_str(), Utf8ToXML(&indexName).x_str());
    infoElem->setAttribute(Utf8ToXML("group").x_str(), Utf8ToXML(&defaultGroup).x_str());
    infoElem->setAttribute(Utf8ToXML("owner").x_str(), Utf8ToXML(&owner).x_str());
    infoElem->setAttribute(
            Utf8ToXML("release").x_str(), Utf8ToXML(ecl::strf("%d", release)).x_str());
    infoElem->setAttribute(
            Utf8ToXML("revision").x_str(), Utf8ToXML(ecl::strf("%d", revision)).x_str());
    infoElem->setAttribute(
            Utf8ToXML("enigma").x_str(), Utf8ToXML(ecl::strf("%.2f", compatibility)).x_str());
    infoElem->setAttribute(Utf8ToXML("location").x_str(),
            Utf8ToXML(ecl::strf("%g", indexDefaultLocation)).x_str());
    DOMNodeList* levelsChildList = levelsElem->getChildNodes();
    int levelsChildCount = levelsChildList->getLength();
    // delete no more used level elements
    for (int i = 0; i < levelsChildCount; i++) {
        // delete all elements of list, as the list is dynamically updated -
        // we cannot recycle level elements as they may be reordered and
        // their attributes are not identical
        levelsElem->removeChild(levelsChildList->item(0));
    }
    DOMElement* levelElem;
    // add level elements
    for (int i = 0; i < size(); i++) {
        // add a new element
        levelElem = doc->createElement(Utf8ToXML("level").x_str());
        levelsElem->appendChild(levelElem); // insert it at the end

        Proxy* level = getProxy(i);
        // convert Proxy normLevelPath to pack local path ./* if possible
        std::string xpath = level->getNormFilePath();
        if (xpath.find(packPath + "/") == 0)
            xpath = "." + xpath.substr(packPath.size());
        else if (packPath.empty() && xpath.find('/') == std::string::npos)
            xpath = "./" + xpath;
        levelElem->setAttribute(Utf8ToXML("_xpath").x_str(), Utf8ToXML(xpath).x_str());
        levelElem->setAttribute(Utf8ToXML("id").x_str(), Utf8ToXML(level->getId()).x_str());
        levelElem->setAttribute(Utf8ToXML("_title").x_str(), Utf8ToXML(level->getTitle()).x_str());
        levelElem->setAttribute(Utf8ToXML("author").x_str(), Utf8ToXML(level->getAuthor()).x_str());
        levelElem->setAttribute(Utf8ToXML("score").x_str(),
                Utf8ToXML(ecl::strf("%d", level->getScoreVersion())).x_str());
        levelElem->setAttribute(Utf8ToXML("rel").x_str(),
                Utf8ToXML(ecl::strf("%d", level->getReleaseVersion())).x_str());
        levelElem->setAttribute(Utf8ToXML("rev").x_str(),
                Utf8ToXML(ecl::strf("%d", level->getRevisionNumber())).x_str());
        levelElem->setAttribute(Utf8ToXML("easy").x_str(),
                Utf8ToXML(level->hasEasyMode() ? "true" : "false").x_str());
        std::string control;
        switch (variations[i].ctrl) {
            case lev::force:
                control = "force";
                break;
            case lev::balance:
                control = "balance";
                break;
            case lev::key:
                control = "key";
                break;
            default:
                control = "other";
                break;
        }
        levelElem->setAttribute(Utf8ToXML("ctrl").x_str(), Utf8ToXML(control).x_str());
        std::string unit;
        switch (variations[i].unit) {
            case lev::duration:
                unit = "duration";
                break;
            case lev::number:
                unit = "number";
                break;
        }
        levelElem->setAttribute(Utf8ToXML("unit").x_str(), Utf8ToXML(unit).x_str());
        levelElem->setAttribute(
                Utf8ToXML("target").x_str(), Utf8ToXML(variations[i].target).x_str());
        for (auto& extension : variations[i].extensions) {
            levelElem->setAttribute(
                    Utf8ToXML(extension.first).x_str(), Utf8ToXML(extension.second).x_str());
        }
    }

    // update the sequence number of the levels
    DOMNodeList* levList = levelsElem->getElementsByTagName(Utf8ToXML("level").x_str());
    for (int i = 0, l = levList->getLength(); i < l; i++) {
        DOMElement* levElem = dynamic_cast<DOMElement*>(levList->item(i));
        levElem->setAttribute(Utf8ToXML("_seq").x_str(), Utf8ToXML(ecl::strf("%d", i + 1)).x_str());
    }

    stripIgnorableWhitespace(doc->getDocumentElement());

    std::string path = app.userPath + "/levels/" + packPath + "/" + indexFilename;
    if (allowOverwrite || !ecl::FileExists(path)) {
        try {
            // auto-create the directory if necessary
            std::string directory;
            if (ecl::split_path(path, &directory, nullptr) && !ecl::FolderExists(directory)) {
                ecl::FolderCreate(directory);
            }

            result = app.domSer->writeToURI(doc, LocalToXML(&path).x_str());
        } catch (const XMLException& toCatch) {
            char* message = XMLString::transcode(toCatch.getMessage());
            cerr << "Exception on save of index: " << message << "\n";
            XMLString::release(&message);
            result = false;
        } catch (const DOMException& toCatch) {
            char* message = XMLString::transcode(toCatch.msg);
            cerr << "Exception on save of index: " << message << "\n";
            XMLString::release(&message);
            result = false;
        } catch (...) {
            cerr << "Unexpected exception on save of index\n";
            result = false;
        }
        if (!result)
            Log << "Index save fault on " << path << " \n";
        else
            Log << "Index save " << path << " o.k.\n";
    } else
        result = false;

    return result;
}

PersistentIndex::PersistentIndex(std::stringstream& legacyIndexStream,
        const std::string& thePackPath, bool isZip, const std::string& anIndexName,
        std::string theIndexFilename, const std::string& aDescription)
    : Index(anIndexName, aDescription, INDEX_DEFAULT_GROUP, Index::getNextUserLocation()),
      indexFilename(theIndexFilename), release(1), revision(1), compatibility(1.00),
      isModified(false), isUserOwned(true), isEditable(true), isAuto(false), doc(nullptr) {
    Log << "PersistentIndex convert 0.92 index " << thePackPath << " - " << anIndexName << "\n";
    lev::RatingManager* theRatingMgr = lev::RatingManager::instance();

    // prepare Proxy coding of pack path
    if (thePackPath == "levels")
        packPath = "";
    else {
        //  cut off leading "levels/"
        packPath = thePackPath.substr(7);
    }

    int linenumber = 0;
    try {
        std::string line;
        while (std::getline(legacyIndexStream, line)) {
            using namespace std;
            using namespace ecl;

            string filename;           //< Filename of the level (exl. extension)
            string indexname;          //< The name used in the options file,
                                       //   empty string -> use filename entry
            string name;               //< Complete name of the level
            string author;             //< Author of the level
            int revision = 1;          //< Revision # of this level
            bool has_easymode = false; //< whether level has an easymode
            int par_time_easy = -1;    //< Best time in seconds (for easy mode)
            int par_time_normal = -1;  //< Best time in seconds (for normal mode)
            string par_time_easy_by;   //< player name(s) for 'best_time_easy'
            string par_time_normal_by; //< same for 'best_time_normal'
            int par_moves = -1;        //< Minimum moves to solve level
            int intelligence = 0;
            int dexterity = 0;
            int patience = 0;
            int knowledge = 0;
            int speed = 0;

            ++linenumber;

            const char* wspace = " \t";
            size_t p = line.find_first_not_of(wspace);

            if (p == string::npos || line.at(p) != '{')
                continue;

            // found a level description

            int par_time = -1;
            string par_time_by;
            string par_moves_by;

            ++p;
            while (true) {
                string tag;
                string content;

                // ugly parser code starts here.
                // - it parses 'tag = content' or 'tag = "content"'
                // - '\"' is allowed in '"content"'
                // - whitespace is ignored
                // - sets message and breaks while loop in case of error
                size_t tag_start = line.find_first_not_of(wspace, p);
                Assert<XLevelPackInit>(tag_start != string::npos, "Expected tag or '}'");
                if (line.at(tag_start) == '}')
                    break; // line done

                size_t equal = line.find('=', tag_start);
                Assert<XLevelPackInit>(equal != string::npos, "'=' expected");
                Assert<XLevelPackInit>(equal != tag_start, "empty tag");

                size_t tag_end = line.find_last_not_of(wspace, equal - 1);
                tag = line.substr(tag_start, tag_end - tag_start + 1);

                size_t content_start = line.find_first_not_of(wspace, equal + 1);
                if (line.at(content_start) == '\"') { // content in ""
                    size_t oquote = line.find('\"', content_start + 1);
                    bool have_escapes = false;
                    while (oquote != string::npos && line.at(oquote - 1) == '\\') { // step over \"
                        oquote = line.find('\"', oquote + 1);
                        have_escapes = true;
                    }
                    Assert<XLevelPackInit>(oquote != string::npos, "unmatched quote");
                    content = line.substr(content_start + 1, oquote - content_start - 1);
                    if (have_escapes) {
                        size_t esc;
                        while ((esc = content.find("\\\"")) != string::npos)
                            content.replace(esc, 2, "\"");
                    }
                    p = oquote + 1;
                } else { // content w/o ""
                    size_t content_end = line.find_first_of(" \t}", content_start);
                    Assert<XLevelPackInit>(
                            content_end != string::npos, "expected space or } behind content");
                    content = line.substr(content_start, content_end - content_start);
                    p = content_end;
                }

                if (tag == "file")
                    filename = content;
                else if (tag == "indexname")
                    indexname = content;
                else if (tag == "name")
                    name = content;
                else if (tag == "author")
                    author = content;
                else if (tag == "revision")
                    revision = atoi(content.c_str());
                else if (tag == "easymode")
                    has_easymode = (content == "1");
                else if (tag == "int")
                    intelligence = atoi(content.c_str());
                else if (tag == "dex")
                    dexterity = atoi(content.c_str());
                else if (tag == "pat")
                    patience = atoi(content.c_str());
                else if (tag == "kno")
                    knowledge = atoi(content.c_str());
                else if (tag == "spe")
                    speed = atoi(content.c_str());
                else if (tag == "par_time")
                    parsePar(content, par_time, par_time_by);
                else if (tag == "par_time_easy")
                    parsePar(content, par_time_easy, par_time_easy_by);
                else if (tag == "par_time_normal")
                    parsePar(content, par_time_normal, par_time_normal_by);
                else if (tag == "par_moves")
                    parsePar(content, par_moves, par_moves_by);
                //                 else if (tag == "hint1") hint1           = content;
                //                 else if (tag == "hint2") hint2           = content;
                else
                    throw XLevelPackInit(strf("unknown tag '%s'", tag.c_str()));
            }

            Assert<XLevelPackInit>(filename.length() != 0, "mandatory tag 'file' missing");

            if (has_easymode) {
                Assert<XLevelPackInit>(par_time == -1,
                        "'par_time' not allowed when easymode=1 "
                        "(use 'par_time_easy' and 'par_time_normal')");
            } else {
                Assert<XLevelPackInit>(par_time_normal == -1 && par_time_easy == -1,
                        "'par_time_easy' and 'par_time_normal' are not allowed when easymode=0 "
                        "(use 'par_time')");
                par_time_easy = par_time_normal = par_time;
                par_time_easy_by = par_time_normal_by = par_time_by;
            }

            // correct filename for zip
            if (isZip) {
                Log << "Zip " << thePackPath << " - " << packPath << " - " << indexname << "\n";
                if (filename.find('/') == std::string::npos) {
                    // it is an old zip file with plain levelnames as filename
                    if (indexname.empty()) {
                        // keep id
                        indexname = filename;
                    }
                    // add the zip basename to get the complete path to the level
                    filename = packPath + "/" + filename;
                }
            } else if (linenumber == 1 && packPath.empty()) {
                // a std index.txt on levels dir - we need to guess location of pack
                // we take the dir of the first level
                std::string::size_type n = filename.rfind('/');
                if (n != std::string::npos) {
                    packPath = filename.substr(0, n);
                }
            }

            // register Proxy
            appendProxy(Proxy::registerLevel(filename, packPath,
                    (indexname.empty() ? filename : indexname), name, author, revision, revision,
                    has_easymode, GAMET_ENIGMA));
            theRatingMgr->registerRating((indexname.empty() ? filename : indexname), revision,
                    intelligence, dexterity, patience, knowledge, speed, par_time_easy,
                    par_time_easy_by, par_time_normal, par_time_normal_by);
        }
    } catch (const XLevelPackInit& e) {
        std::string xerror = ecl::strf("in line %i: %s", linenumber, e.what());
        throw XLevelPackInit(xerror);
    }

    // convert to XML

    updateFromProxies();

    // save but do not overwrite existing index.xml - would be a second conversion,
    // but the user may already have modified the index.xml
    save(false);
}

void PersistentIndex::parsePar(const string& par, int& par_value, std::string& par_text) {
    // 'par' is in format "value,text"
    // The value is stored in 'par_value' and the text in 'par_text'.
    using namespace std;
    using namespace ecl;

    size_t comma = par.find(',');
    Assert<XLevelPackInit>(comma != string::npos, "Comma expected in par");

    string value = par.substr(0, comma);
    par_text = par.substr(comma + 1);
    par_value = atoi(value.c_str());
}

void AddLevelPack(const char* init_file, const char* indexName) {
    if (Index::findIndex(indexName) == nullptr) {
        std::string absPath;
        if (app.resourceFS->findFile(init_file, absPath)) {
            try {
                std::string path = init_file;
                std::string dir;
                std::string filename;
                ecl::split_path(path, &dir, &filename);
                std::ifstream is(absPath.c_str());

                if (!is)
                    throw XLevelPackInit("Cannot open index file");
                std::string indexString;
                is >> indexString;
                std::stringstream indexStream(indexString);
                PersistentIndex::registerPersistentIndex(std::make_unique<PersistentIndex>(indexStream, dir, false, indexName));
            } catch (const XLevelPackInit& e) {
                Log << e.get_string() << "\n";
            }
        } else {
            Log << "Could not find level index file `" << init_file << "'\n";
        }
    } else {
        Log << "Ignored index file `" << init_file << "' - already converted to XML\n";
    }
}

void AddZippedLevelPack(const char* zipfile) {
    std::string absPath;
    if (app.resourceFS->findFile(zipfile, absPath)) {
        // the index file as it would be for an unpacked zip
        std::string zf = zipfile;
        std::string dir = zf.substr(0, zf.rfind('.'));
        std::string indexfile = dir + "/index.txt";
        try {
            std::stringstream inflatedContent;
            std::string dummy;
            if (!app.resourceFS->findFile(indexfile, dummy, inflatedContent))
                throw XLevelPackInit("No index in level pack: ");
            std::string line;
            std::string indexName;
            if (getline(inflatedContent, line)) {
                // we read the index in binary mode and have to strip of the \n for
                // windows
                if (line[line.size() - 1] == '\n') {
                    line.resize(line.size() - 1);
                }
                indexName = line;

                // check if already loaded
                PersistentIndex::registerPersistentIndex(std::make_unique<PersistentIndex>(
                        inflatedContent, dir, true, indexName));
            } else {
                throw XLevelPackInit("Invalid level pack: " + indexName);
            }
        } catch (std::exception&) {
            throw XLevelPackInit("Error reading from .zip file: " + indexfile);
        }
    } else {
        Log << "Could not find zip file `" << zipfile << "'\n";
    }
}

} // namespace enigma::lev
