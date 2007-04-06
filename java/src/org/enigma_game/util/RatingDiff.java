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

import java.util.*; 
import org.enigma_game.EnigmaUtil;
import org.enigma_game.lev.RatingManager;

public class RatingDiff {
    EnigmaUtil theApp = org.enigma_game.EnigmaUtil.theApp;
    
    public RatingDiff(String oldRatingUrl, String newRatingUrl) {
        try {
            System.out.println("Start building ratings diff.");
            System.out.println("parse " + oldRatingUrl);
            RatingManager oldRating = new RatingManager(oldRatingUrl);
            System.out.println("parse " + newRatingUrl);
            RatingManager newRating = new RatingManager(newRatingUrl);

            System.out.println("parse template for incremental ratings");
            RatingManager incRating = new RatingManager(
                    RatingDiff.class.getResource(theApp.RESOURCES + "schemas/ratings.xml").toString());
            System.out.println("parse template for dummy incremental ratings");
            RatingManager incDummyRating = new RatingManager(
                    RatingDiff.class.getResource(theApp.RESOURCES + "schemas/ratings.xml").toString());
            
            incRating.setDate(newRating.getDate());
            incDummyRating.setDate(newRating.getDate());
            
            newRating.setUrlFull(oldRating.getUrlFull());
            incRating.setUrlFull(oldRating.getUrlFull());
            incDummyRating.setUrlFull(oldRating.getUrlFull());
            
            String oldUrlIncr = oldRating.getUrlIncremental();
            String incVersionString = oldUrlIncr.substring(oldUrlIncr.length() - 7,
                    oldUrlIncr.length() - 4);
            int incVersion = Integer.parseInt(incVersionString);
            
            int incNewVersion = incVersion + 1;
            String incNewVersionString = Integer.toString(incNewVersion);
            if (incNewVersionString.length() == 1)
                incNewVersionString = "00" + incNewVersionString;
            else if (incVersionString.length() == 2)
                incNewVersionString = "0" + incNewVersionString;
            
            String newUrlIncr = oldUrlIncr.substring(0, oldUrlIncr.length() - 7)
                    + incNewVersionString + ".xml";
            
            newRating.setUrlIncremental(newUrlIncr);
            incRating.setUrlIncremental(newUrlIncr);
            incDummyRating.setUrlIncremental(newUrlIncr);
            
            Set<Map<String,String>> oldLevels = oldRating.getRatingData();
            Set<Map<String,String>> newLevels = newRating.getRatingData();
            System.out.println("Level count old " + oldLevels.size() + " - new " + newLevels.size());
            
            newLevels.removeAll(oldLevels);
            System.out.println("Level count diff " + newLevels.size());
            incRating.addRatingData(newLevels);
            
            newRating.save("ratings.xml");
            incRating.save("ratings_i" + incVersionString + ".xml");
            incDummyRating.save("ratings_i" + incNewVersionString + ".xml");
            System.out.println("Finished building ratings diff.");
        } catch ( Exception ex ) {
            ex.printStackTrace();
        }
    }
}
