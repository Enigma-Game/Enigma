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
import java.util.*; 
import org.w3c.dom.*;
import org.w3c.dom.ls.*;

import org.enigma_game.EnigmaUtil;
import org.enigma_game.lev.RatingManager;
import org.enigma_game.lev.LevelScore;
import org.enigma_game.lev.UserManager;

public class ScoreManager {
    EnigmaUtil theApp = org.enigma_game.EnigmaUtil.theApp;
    Document doc;
    NodeList propertyElems;
    Element levelsElem;
    NodeList levelElems;
    boolean isProfessional = false;
    short[] ctab = new short[256];
    short pol = 0x1021;
    
    public ScoreManager(InputStream bStream) {
        for (int i = 0; i < 256; i++) {
            int r = i << 8;
            for (int j = 0; j < 8; j++) {
                boolean bit = (r & 0x8000) != 0;
                r <<= 1;
                if (bit)
                    r ^= pol;
            }
            ctab[i] = (short)(r & 0xFFFF);
        }
        
        LSInput input = theApp.domImpl.createLSInput();
        input.setByteStream(bStream);
        doc = theApp.domParser.parse(input);
        
        DOMConfiguration config = doc.getDomConfig();
        config.setParameter("error-handler", theApp);
        
        propertyElems = doc.getElementsByTagName("property");
        levelsElem = (Element) doc.getElementsByTagName("levels").item(0);
        levelElems = doc.getElementsByTagName("level");
    }
    
    public void save(OutputStream bStream) {
        LSOutput dOut = theApp.domImpl.createLSOutput();
        dOut.setByteStream(bStream);
        theApp.domWriter.write(doc, dOut);
    }
    
    public int getScoreElemsCount() {
        return levelElems.getLength();
    }

    public String getProperty(String key) {
        for (int i = 0; i < propertyElems.getLength(); i++) {
            Element e = (Element) propertyElems.item(i);
            if (e.getAttribute("key").equals(key)) {
                if (key.equals("UserName"))
                    return e.getAttribute("value").trim();
                else
                    return e.getAttribute("value");
            }
        }
        return "";
    }
    
    private String sec(String input) {
        byte[] target = new byte[0];
        try {
            target = input.getBytes("UTF-8");
        } catch (UnsupportedEncodingException e) {
        }
        int len = target.length;
        int r = 0;
        
        for (int i = 0; i < len; i++)
            r = (r<<8 & 0xFFFF) ^ ctab[((r >>> 8) ^ target[i]) & 0xFF];
        Formatter formatter = new Formatter(Locale.US);
        formatter.format("%04X", r & 0xFFFF);
        return formatter.toString();
    }
    
    public boolean checkUser(UserManager userMgr) {
        String id = getProperty("UserId");
        String id_1_00 = getProperty("UserId1.00");
        String userName = userMgr.getValue(id, "name");
        boolean result = false;
        
        if (getProperty("UserName").equals("") && userName.equals("")) {
            System.out.println("No user name given in score file!");
            return false;
        } else if (getProperty("UserName").contains("+")) {
            System.out.println("User name contains '+'!");
            return false;
        } else if (getProperty("UserName").length() > 20) {
            System.out.println("User name too long");
            return false;
        }
        
        if (!id_1_00.equals("")) {
            if (userMgr.getValue(id_1_00, "name").equals(getProperty("UserName"))) {
                // the user did reId since last score registration - update
                userMgr.setValue(id_1_00, "id", id);
                System.out.println("User did reId! Old Id = '" + id_1_00 + "'");
                userName = userMgr.getValue(id, "name");
            } else if (!userMgr.getValue(id_1_00, "id").equals("")) {
                // a user may have reId and renamed or it is another user
                // that used the same 1.00 id.
                System.out.println("User id 1.00 still in use by another user");
                return false;
            }
        }
        
        if (userName.equals(getProperty("UserName"))) {
            // user exists with same id and name
            if (Integer.parseInt(getProperty("Count")) >
                    Integer.parseInt(userMgr.getValue(id, "savecount"))) {
                userMgr.setValue(id, "savecount", getProperty("Count"));
                result = true;
            } else {
                System.out.println("Outdated scores not registered!");
            }
        } else if (userName.equals("")) {
            // no user registered with the id
            if (userMgr.isNameFree(getProperty("UserName"))) {
                userMgr.setValue(id, "name", getProperty("UserName"));
                userMgr.setValue(id, "savecount", getProperty("Count"));
                result = true;
            } else
                System.out.println("Error - new user with name in use '"
                        + getProperty("UserName") +"'");
        } else {
            // name mismatch - changed name or second user with same id
            if (Integer.parseInt(getProperty("Count")) >
                    Integer.parseInt(userMgr.getValue(id, "savecount"))) {
                // looks like a valid name change
                if (getProperty("UserName").equals("")) {
                    // do not rename to an empty name
                    userMgr.setValue(id, "savecount", getProperty("Count"));
                    return true;
                } else if (userMgr.isNameFree(getProperty("UserName")) ||
                    userMgr.getValue(id, "newname").equals(getProperty("UserName"))) {
                    System.out.println("Warning: user name change from '" 
                            + userName + "' to '" + getProperty("UserName") + "'");
                    userMgr.setValue(id, "newname", getProperty("UserName"));
                    userMgr.setValue(id, "savecount", getProperty("Count"));
                    result = true;
                } else {
                    // new name in use by another user
                    System.out.println("Error: user name change from '" 
                            + userName + "' to the already used name '"
                            + getProperty("UserName") +"'");
                }
            } else {
                // must be a second user with the same id
                System.out.println("Id mismatch - id already in use!");
            }
        }
        return result;
    }
    
    public void checkScores(RatingManager ratingMgr, UserManager userMgr, boolean checkSec) {
        ratingMgr.clearUserScores();
        String user = getProperty("UserName");
        String userId = getProperty("UserId");
        int numRatings = 0;
        int sumRatings = 0;
        for (int i = 0; i < levelElems.getLength(); i++) {
            Element e = (Element) levelElems.item(i);
            LevelScore ls = ratingMgr.getLevelScore(e.getAttribute("id"),
                    e.getAttribute("version"));
            if (ls != null) {
                String rating = e.getAttribute("rating");
                int urat = -1;
                if (rating.length() > 0)
                    urat = Integer.parseInt(rating);
                if (urat > -1) {
                    numRatings++;
                    sumRatings += urat;
                }
                ls.registerSolvage(user, e.getAttribute("diff1"), 
                        e.getAttribute("easy1"), urat);
            }
            if (checkSec) {
                NamedNodeMap attrXMap = e.getAttributes();
                Map<String, String> attrMap = new TreeMap<String, String>(); // sorted Map
                for (int j = 0, k = attrXMap.getLength(); j < k; j++) {
                    Attr levelAttr = (Attr)(attrXMap.item(j));
                    if (!levelAttr.getName().equals("sec") && levelAttr.getSpecified()) 
                        attrMap.put(levelAttr.getName(), levelAttr.getValue());
                }
                String target = "";
                for (Map.Entry<String, String> attrEntry : attrMap.entrySet())
                    target += attrEntry.getValue();
                target += userId;
                if (!sec(target).equals(e.getAttribute("sec")))
                    System.out.println("Faulty score entry!! " + target + " is " 
                            + sec(target) + "  should be " + e.getAttribute("sec"));
            }
        }
        double avgRating = -1.0;
        if (numRatings > 0)
            avgRating = ((float)sumRatings)/numRatings;
        Formatter formatter = new Formatter(Locale.US);
        formatter.format("Ratings count %d, avg %5.2f", numRatings, avgRating);
        System.out.println(formatter.toString());
        
        Formatter formatterRatcount = new Formatter(Locale.US);
        formatterRatcount.format("%3d", numRatings);
        userMgr.setValue(getProperty("UserId"), "ratcount", formatterRatcount.toString());
        
        Formatter formatterRatAvg = new Formatter(Locale.US);
        formatterRatAvg.format("%5.2f",  avgRating);
        userMgr.setValue(userId, "ratavg", formatterRatAvg.toString());
                
        isProfessional = ratingMgr.isUserProfessional(userMgr, getProperty("UserId"));
    }
    
    public void evaluateScores(RatingManager ratingMgr) {
        String user = getProperty("UserName");
        for (int i = 0; i < levelElems.getLength(); i++) {
            Element e = (Element) levelElems.item(i);
            LevelScore ls = ratingMgr.getLevelScore(e.getAttribute("id"),
                    e.getAttribute("version"));
            if (ls != null) {
                String rating = e.getAttribute("rating");
                int urat = -1;
                if (rating.length() > 0)
                    urat = Integer.parseInt(rating);
                ls.registerScore(user, isProfessional, rating,
                        e.getAttribute("diff1"),  e.getAttribute("diff1rel"), 
                        e.getAttribute("easy1"), e.getAttribute("easy1rel"),
                        e.getAttribute("diff2"),  e.getAttribute("diff2rel"), 
                        e.getAttribute("easy2"), e.getAttribute("easy2rel"));
            }
        }
    }
}
