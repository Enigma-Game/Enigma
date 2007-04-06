/*
 * Copyright (C) 2006, 2007 Ronald Lamprecht
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
package org.enigma_game.lev;

import java.io.*;
import java.text.*;
import java.util.*; 
import org.w3c.dom.*;
import org.w3c.dom.ls.*;

import org.enigma_game.EnigmaUtil;

public class UserManager {
    EnigmaUtil theApp = org.enigma_game.EnigmaUtil.theApp;
    Document doc;
    Element usersElem;
    NodeList userElems;
    Map<String, Element> idElementMap = new HashMap<String, Element>();
    Set<String> nameSet = new HashSet<String>();
    
    public UserManager(String url) {
        doc = theApp.domParser.parseURI(url);
        
        DOMConfiguration config = doc.getDomConfig();
        config.setParameter("error-handler", theApp);
        
        usersElem = (Element) doc.getElementsByTagName("users").item(0);
        userElems = doc.getElementsByTagName("user");
        
        for (int i = 0; i < userElems.getLength(); i++) {
            Element e = (Element) userElems.item(i);
            idElementMap.put(e.getAttribute("id"), e);
            nameSet.add(e.getAttribute("name"));
            String newname = e.getAttribute("newname");
            if (!newname.equals("")) {
                nameSet.add(newname);
            }
        }
    }
    
    public Set<String> getUserIds() {
        return idElementMap.keySet();
    }
    
    public boolean isNameFree(String name) {
        return !nameSet.contains(name);
    }
    
    public String getValue(String id, String key) {
        Element e = idElementMap.get(id);
        return e == null ? "" : e.getAttribute(key);
    }
    
    public void setValue(String id, String key, String value) {
        Element e = idElementMap.get(id);
        if (e == null && key.equals("name")) {
            if (isNameFree(value)) {
                nameSet.add(value);
                // create element first
                Element user = doc.createElementNS(null, "user");
                user.setAttributeNS(null, "id", id);
                user.setAttributeNS(null, "name", value);
                usersElem.appendChild(user);
                idElementMap.put(id, user);
            }
        } else if (e != null) {
            if (key.equals("newname") && !value.equals("")) {
                nameSet.add(value);
            } else if (key.equals("id") && !value.equals("")) {
                idElementMap.put(value, e);  // add element with new id
                idElementMap.remove(id);     // remove element with old id
            }
            e.setAttributeNS(null, key, value);
        }
    }
    
    public void save(String url) {
        //doc.normalizeDocument();
        System.out.println("save " + url + " - "+ userElems.getLength());
        theApp.domWriter.writeToURI(doc, url);
    }
    
    public class ComparatorName implements Comparator<String> {
        public ComparatorName() {
        }
        
        public int compare(String id1, String id2) {
            final Collator usCollator = Collator.getInstance(Locale.US);
            return usCollator.compare(getValue(id1, "name"),getValue(id2, "name"));
        }
    }
    
    public class ComparatorWRTotal implements Comparator<String> {
        public ComparatorWRTotal() {
        }
        
        public int compare(String id1, String id2) {
            String wrtotal1 = getValue(id1, "wrtotal");
            int wrTotal1 = (!wrtotal1.equals("")) ? Integer.parseInt(wrtotal1.trim()) : 0;
            String wrtotal2 = getValue(id2, "wrtotal");
            int wrTotal2 = (!wrtotal2.equals("")) ? Integer.parseInt(wrtotal2.trim()) : 0;
            String wrshared1 = getValue(id1, "wrshared");
            int wrShared1 = (!wrshared1.equals("")) ? Integer.parseInt(wrshared1.trim()) : 0;
            String wrshared2 = getValue(id2, "wrshared");
            int wrShared2 = (!wrshared2.equals("")) ? Integer.parseInt(wrshared2.trim()) : 0;
            int result = wrTotal2 - wrTotal1;
            if (result == 0)
                result = wrShared1 - wrShared2;
            return result;
        }
    }
    
    public class ComparatorSolved implements Comparator<String> {
        public ComparatorSolved() {
        }
        
        public int compare(String id1, String id2) {
            String solvedtotal1 = getValue(id1, "solvedtotal");
            double solvedTotal1 = (!solvedtotal1.equals("")) ? Double.parseDouble(solvedtotal1.trim()) : 0;
            String solvedtotal2 = getValue(id2, "solvedtotal");
            double solvedTotal2 = (!solvedtotal2.equals("")) ? Double.parseDouble(solvedtotal2.trim()) : 0;
            int result = ( solvedTotal1 < solvedTotal2) ? 1 : ((solvedTotal1 == solvedTotal2) ? 0 : -1);
            return result;
        }
    }
    
    public class ComparatorHcpTotal implements Comparator<String> {
        public ComparatorHcpTotal() {
        }
        
        public int compare(String id1, String id2) {
            String hcptotal1 = getValue(id1, "hcptotal");
            double hcpTotal1 = (!hcptotal1.equals("")) ? Double.parseDouble(hcptotal1.trim()) : 100;
            String hcptotal2 = getValue(id2, "hcptotal");
            double hcpTotal2 = (!hcptotal2.equals("")) ? Double.parseDouble(hcptotal2.trim()) : 100;
            int result = ( hcpTotal1 < hcpTotal2) ? -1 : ((hcpTotal1 == hcpTotal2) ? 0 : 1);
            return result;
        }
    }
    
    public class ComparatorHcpSolved implements Comparator<String> {
        public ComparatorHcpSolved() {
        }
        
        public int compare(String id1, String id2) {
            String hcpsolved1 = getValue(id1, "hcpsolved");
            double hcpSolved1 = (!hcpsolved1.equals("")) ? Double.parseDouble(hcpsolved1.trim()) : 100;
            String hcpsolved2 = getValue(id2, "hcpsolved");
            double hcpSolved2 = (!hcpsolved2.equals("")) ? Double.parseDouble(hcpsolved2.trim()) : 100;
            int result = ( hcpSolved1 < hcpSolved2) ? -1 : ((hcpSolved1 == hcpSolved2) ? 0 : 1);
            return result;
        }
    }
    
}
