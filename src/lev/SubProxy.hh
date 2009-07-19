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
 
#ifndef LEV_SUBPROXY_HH_INCLUDED
#define LEV_SUBPROXY_HH_INCLUDED

#include "enigma.hh"
#include "lev/Proxy.hh"

namespace enigma { namespace lev {

    class SubProxy : public Proxy {
    public:
        SubProxy(Proxy * fileProxy, unsigned num, pathType thePathType, std::string theNormLevelPath,
                std::string levelId, std::string levelTitle);
                
        virtual void loadLevel();
        virtual void loadMetadata(bool expectLevel);
        virtual Proxy * copy(std::string newBasePath, std::string newPackPath, bool backup = true);
        virtual std::string getLocalizedString(const std::string &key);
        virtual int getQuantity();
        virtual int getScoreVersion();
        virtual int getReleaseVersion();
        virtual int getRevisionNumber();
        virtual levelStatusType getLevelStatus();
        virtual std::string getAuthor();
        virtual std::string getTitle(); // english title
        virtual bool hasEasyMode();
        virtual bool hasSingleMode();
        virtual bool hasNetworkMode();
        virtual std::string getContact();
        virtual std::string getHomepage();
        virtual controlType getControl();
        virtual scoreUnitType getScoreUnit();
        virtual std::string getScoreTarget();
        virtual std::string getCredits(bool infoUsage);
        virtual std::string getDedication(bool infoUsage);
        virtual int getEasyScore();
        virtual int getDifficultScore();
        virtual GameType getEngineCompatibility();
        virtual double getEnigmaCompatibility();
        virtual std::string getExternalData(std::string pathId);
        virtual std::string getNormLevelPath();
        virtual std::string getAbsLevelPath();
        
    private:
        Proxy * multiProxy;
        unsigned subNum;
    };
}} // namespace enigma::lev
#endif
