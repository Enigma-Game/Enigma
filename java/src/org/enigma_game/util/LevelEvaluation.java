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
package org.enigma_game.util;

import java.io.*;
import java.util.*; 
import java.util.zip.*;
import org.enigma_game.EnigmaUtil;
import org.enigma_game.lev.RatingManager;
import org.enigma_game.lev.ScoreManager;
import org.enigma_game.lev.UserManager;

public class LevelEvaluation {
    EnigmaUtil theApp = org.enigma_game.EnigmaUtil.theApp;
    
    public LevelEvaluation(String rating, String userUrl, String levelPattern,
            String[] scoreFiles, int firstIndex) {
        try {
            System.out.println("LevelEvaluation");
            RatingManager ratingMgr = new RatingManager(rating);
            ratingMgr.setUpLevelScores(levelPattern);
            UserManager userMgr = new UserManager(userUrl);
            
            for (int i = firstIndex; i < scoreFiles.length; i++) {
                ZipInputStream zin = new ZipInputStream(new BufferedInputStream(new FileInputStream(scoreFiles[i])));
                zin.getNextEntry();
                ScoreManager scm = new ScoreManager(zin);
                scm.checkScores(ratingMgr, userMgr, false);
            }
            ratingMgr.evaluateLevels();
            
            System.out.println("LevelEvaluation finished");
        } catch ( Exception ex ) {
            ex.printStackTrace();
        }
    }
}
