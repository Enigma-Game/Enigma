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
 
#ifndef PREFMGR_HH_INCLUDED
#define PREFMGR_HH_INCLUDED

#include <string>
#include <xercesc/util/XercesVersion.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>

namespace enigma
{
    /**
     * A singelton manager for preferences stored in XML format. 
     * <p>The singleton can be accessed via standard static instance() method or
     * the application public ivar <code>app.prefs</code>.</p>
     * <p> During initialization old LUA stored options are convereted. New 
     * preferences with given default values are introduced to the user via the 
     * template in the systen data directory (data/enigmarc3.xml).</p>
     * <p>The storage location of the users preferences is determined by the
     * Application object.</p>
     * <p>The set and get methods for preferences take utf8 encoded preference
     * names and operate with utf8 encoded string values. Set methods create new
     * preference property elements if necessary. Get methods do not modify
     * reference values and return C++ default values if the named preference
     * does not exist.</p>
     */
    class PreferenceManager {
    public:
        static PreferenceManager *instance();
        ~PreferenceManager();
        bool save();
        void shutdown();
        
        void setPref(const char *prefName, const std::string &value);
        void getPref(const char *prefName, std::string &value);
        std::string getString(const char *prefName);
        void setPref(const char *prefName, const double &value);
        void getPref(const char *prefName, double &value);
        double getDouble(const char *prefName);
        void setPref(const char *prefName, const int &value);
        void getPref(const char *prefName, int &value);
        int  getInt(const char *prefName);
        void setPref(const char *prefName, const bool &value);
        void getPref(const char *prefName, bool &value);
        bool getBool(const char *prefName);
    protected:
        PreferenceManager();
    private:
        static PreferenceManager *theSingleton;
        XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc;
        
        /**
         * force a return of a property element with the given name as key.
         * @param prefName  the name of the searched or new property
         * @return  the property element with the given key name.
         */
        XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * getProperty(const char *prefName);

        /**
         * checks if a property exists.
         * @param prefName the preference name.
         * @param element  the searched property element or the last property 
         *                 element found in the preference list (for append usage).
         * @return validity of element.
         */
         bool hasProperty(const char *prefName, XERCES_CPP_NAMESPACE_QUALIFIER DOMElement ** element);
        /**
         * checks if a property exists.
         * @param key     the preference name.
         * @param element the searched property element or the last property 
         *                element found in the preference list (for append usage).
         * @return validity of element.
         */
        bool hasProperty(const XMLCh * key, XERCES_CPP_NAMESPACE_QUALIFIER DOMElement ** element);
    };
} // namespace enigma
#endif

