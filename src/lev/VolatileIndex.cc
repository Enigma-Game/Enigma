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

#include "lev/VolatileIndex.hh"
#include "errors.hh"
#include "gui/ErrorMenu.hh"
#include "main.hh"
#include "nls.hh"


namespace enigma { namespace lev {

    int VolatileIndex::levelCount = 0;
    
    VolatileIndex::VolatileIndex(std::string anIndexName, std::string aGroupName,
            const std::vector<string> levelpaths, double defaultLocation) : 
            Index(anIndexName, aGroupName, defaultLocation) {
        for (unsigned i=0; i<levelpaths.size(); i++) {
            Proxy * aProxy = Proxy::registerLevel(levelpaths[i], "#commandline",
                    ecl::strf("_%d",levelCount), ecl::strf("Level %d", i), "unknown",
                    1, 0, false, GAMET_UNKNOWN, STATUS_UNKNOWN); 
            try {
                aProxy->loadMetadata(true);
                levelCount++;
                proxies.push_back(aProxy);
            }
            catch (XLevelLoading &err) {
                Log << "Level load error on '" << levelpaths[i] << "\n";
                std::string message = _("Error on auto registration of levelfile: ");
                message += levelpaths[i] + "\n\n";
                message += _("Note: the level will not show up in the \"Startup Folder\" levelpack!\n\n");
                message += err.what();
                gui::ErrorMenu m(message, N_("Continue"));
                m.manage();
            }
        }
    }
    
    VolatileIndex::~VolatileIndex() {}
    
    void VolatileIndex::clear() {
        proxies.clear();
        currentPosition = 0;
    }
    
    void VolatileIndex::updateFromFolder() {
    }
}} // namespace enigma::lev
