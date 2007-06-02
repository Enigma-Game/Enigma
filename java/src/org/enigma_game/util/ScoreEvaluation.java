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

public class ScoreEvaluation {
    EnigmaUtil theApp = org.enigma_game.EnigmaUtil.theApp;
    
    public ScoreEvaluation(String ratingIn, String ratingOut, String userUrl,
            String[] scoreFiles, int firstIndex) {
        try {
            System.out.println("ScoreEvaluation");
            RatingManager ratingMgr = new RatingManager(ratingIn);
            ratingMgr.setUpLevelScores("");
            UserManager userMgr = new UserManager(userUrl);
            
            int numUsers = 0;
            for (int i = firstIndex; i < scoreFiles.length; i++) {
                ZipInputStream zin = new ZipInputStream(new BufferedInputStream(new FileInputStream(scoreFiles[i])));
                zin.getNextEntry();
                ScoreManager scm = new ScoreManager(zin);
                scm.checkUser(userMgr, false);
                scm.checkScores(ratingMgr, userMgr, false);
                scm.evaluateScores(ratingMgr);
                numUsers++;
            }
            
            ratingMgr.finishLevelScores(numUsers, userMgr);
            ratingMgr.setDate("");
            ratingMgr.save(ratingOut);
            userMgr.save(userUrl);
            
            System.out.println("ScoreEvaluation finished");
        } catch ( Exception ex ) {
            ex.printStackTrace();
        }
    }
}
