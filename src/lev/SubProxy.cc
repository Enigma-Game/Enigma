/*
 * Copyright (C) 2009 Ronald Lamprecht
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

#include "lev/SubProxy.hh"

#include "main.hh"
#include "server.hh"

namespace enigma { namespace lev {
    
    SubProxy::SubProxy(Proxy * fileProxy, unsigned num, Proxy::pathType thePathType, std::string theNormLevelPath,
            std::string levelId, std::string levelTitle) :
            Proxy(false, thePathType, theNormLevelPath, levelId, levelTitle, "unknown",
            1, 0, false, GAMET_UNKNOWN, STATUS_UNKNOWN) {
        multiProxy = fileProxy;
        subNum = num;
    }
    
    void SubProxy::loadLevel() {
        server::SublevelNumber = subNum;
        multiProxy->loadLevel();
        if (server::SublevelTitle.length() > 0)
            title = server::SublevelTitle;
    }
    
    void SubProxy::loadMetadata(bool expectLevel) {
        multiProxy->loadMetadata(expectLevel);
    }
    
    std::string SubProxy::getLocalizedString(const std::string &key) {
        if (key == "title")
            return getTitle();   // no localization possible
        else
            return multiProxy->getLocalizedString(key);
    }

    Proxy * SubProxy::copy(std::string newBasePath, std::string newPackPath, bool backup) {
        Proxy * newMulti = multiProxy->copy(newBasePath, newPackPath, backup);
        return registerLevel(newMulti->getNormFilePath(), newPackPath, getId(), getTitle(),
                        getAuthor(), getScoreVersion(), getReleaseVersion(), hasEasyMode(), 
                        getEngineCompatibility(), getLevelStatus(), getRevisionNumber());
        
    }
    
    int SubProxy::getQuantity() {
        return multiProxy->getQuantity();
    }

    int SubProxy::getScoreVersion() {
        return multiProxy->getScoreVersion();
    }
    int SubProxy::getReleaseVersion() {
        return multiProxy->getReleaseVersion();
    }
    int SubProxy::getRevisionNumber() {
        return multiProxy->getRevisionNumber();
    }
    
    levelStatusType SubProxy::getLevelStatus() {
        return multiProxy->getLevelStatus();
    }

    std::string SubProxy::getAuthor() {
        return multiProxy->getAuthor();
    }

    std::string SubProxy::getTitle() {
        std::string text = title;
        if (text.size() == 0) {
            text = multiProxy->getTitle();
            if (text[text.size() - 1] == '#') {
                // numbered titles for sublevels are intended
                text += ecl::strf(" %d", subNum);
                title = text;  // store generated title for index usage
            } else {
                // Lua part will supply title on load
                text += ecl::strf(" # %d", subNum);  // just a temporary title
            } 
        }
        return text;
    }

    bool SubProxy::hasEasyMode() {
        return multiProxy->hasEasyMode();
    }

    bool SubProxy::hasSingleMode() {
        return multiProxy->hasSingleMode();
    }

    bool SubProxy::hasNetworkMode() {
        return multiProxy->hasNetworkMode();
    }

    std::string SubProxy::getContact() {
        return multiProxy->getContact();
    }

    std::string SubProxy::getHomepage() {
        return multiProxy->getHomepage();
    }

    controlType SubProxy::getControl() {
        return multiProxy->getControl();
    }

    scoreUnitType SubProxy::getScoreUnit() {
        return multiProxy->getScoreUnit();
    }

    std::string SubProxy::getScoreTarget() {
        return multiProxy->getScoreTarget();
    }

    std::string SubProxy::getCredits(bool infoUsage) {
        return multiProxy->getCredits(infoUsage);
    }

    std::string SubProxy::getDedication(bool infoUsage) {
        return multiProxy->getDedication(infoUsage);
    }

    int SubProxy::getEasyScore() {
        return multiProxy->getEasyScore();
    }

    int SubProxy::getDifficultScore() {
        return multiProxy->getDifficultScore();
    }

    GameType SubProxy::getEngineCompatibility() {
        return multiProxy->getEngineCompatibility();
    }

    double SubProxy::getEnigmaCompatibility() {
        return multiProxy->getEnigmaCompatibility();
    }
    
    std::string SubProxy::getExternalData(std::string pathId) {
        return multiProxy->getExternalData(pathId);
    }
    
    std::string SubProxy::getNormLevelPath() {
        return getNormFilePath() + ecl::strf("_%d", subNum);
    }
    
    std::string SubProxy::getAbsLevelPath() {
        return multiProxy->getAbsLevelPath();
    }

}} // namespace enigma::lev
