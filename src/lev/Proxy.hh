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
 
#ifndef LEV_PROXY_HH_INCLUDED
#define LEV_PROXY_HH_INCLUDED

#include <string>
#include <xercesc/dom/DOMDocument.hpp>

namespace enigma { namespace lev {
    /**
     * This class is still experimental !
     */
    class Proxy {
    public:
        static const XMLCh levelNS[];
        static Proxy *loadedLevel();
        static void releaseCurrentLevel(); // temp - as long as levelpack exists
        static void loadLevel(std::string filename);
        Proxy(std::string filename);
        ~Proxy();
        void loadLua();
        void release();
        
        /**
         * Retrieve and translate a level string. The key may be "titel",
         * "subtitel" or any level specific string key. The priorities for
         * translation are as follows: protected translation - gettext 
         * translation - public translation - protected english - key
         * @arg key     the key for the search string
         * @return      the translation of the string
         */
        std::string getLocalizedString(const std::string &key);
        
        std::string getAuthor();
    private:
        static Proxy *currentLevel;
        XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc;
        XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *infoElem;
        XERCES_CPP_NAMESPACE_QUALIFIER DOMNodeList *stringList;
        void loadXML(std::string filename);
    };
}} // namespace enigma::lev
#endif

