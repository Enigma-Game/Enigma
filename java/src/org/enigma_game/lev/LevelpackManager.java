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

public class LevelpackManager {
    EnigmaUtil theApp = org.enigma_game.EnigmaUtil.theApp;
    Document doc;
    NodeList levelElems;
    String name;
    List<LevelScore> levelScores = new ArrayList<LevelScore>();
    
    public LevelpackManager(String levelpack) {
        name = levelpack;
        LSInput input = theApp.domImpl.createLSInput();
        InputStream iStream = LevelpackManager.class.getResourceAsStream(
                EnigmaUtil.RESOURCES +
                "levelpacks/" + levelpack + ".xml");
        BufferedInputStream bStream = new BufferedInputStream(iStream);
        input.setByteStream(bStream);
        doc = theApp.domParser.parse(input);
        
        DOMConfiguration config = doc.getDomConfig();
        config.setParameter("error-handler", theApp);
        
        levelElems = doc.getElementsByTagName("level");
    }
    
    public void registerLevels(RatingManager ratingMgr) {
        for (int i = 0; i < levelElems.getLength(); i++) {
            Element e = (Element) levelElems.item(i);
            LevelScore ls = ratingMgr.getLevelScore(e.getAttribute("id"),
                    e.getAttribute("score"));
            if (ls == null) {
                ls = ratingMgr.newLevelScore(e.getAttribute("id"),
                    e.getAttribute("score"));
            }
            ls.addIndexInfo(e.getAttribute("_title"), e.getAttribute("author"),
                    e.getAttribute("_xpath"), e.getAttribute("easy"));
            levelScores.add(ls);
        }
    }
    
    public String getName() {
        return name;
    }
    
    public List<LevelScore> getLevelScores() {
        return levelScores;
    }
    
    public int size() {
        return levelScores.size();
    }
}
