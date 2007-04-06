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

public class ScoreRegistration {
    EnigmaUtil theApp = org.enigma_game.EnigmaUtil.theApp;
    
    public ScoreRegistration(String url, String ratingUrl, String userUrl) {
        try {
            System.out.println("ScoreRegistration");
            
            RatingManager ratingMgr = new RatingManager(ratingUrl);
            ratingMgr.setUpLevelScores("");

            BufferedInputStream bin = new BufferedInputStream(new FileInputStream(url));
            ByteArrayOutputStream baout = new ByteArrayOutputStream();
            int c;
            while ((c = bin.read()) != -1)
                baout.write(c ^ 0xE5);
            byte[] ba = baout.toByteArray();
            int basize = baout.size();
            
            if ((ba[0x06] & 0x08) == 0) {
                // zipios++ patch needed for 1.00 beta scores:
                //   1.00 beta did not patch the zips produced by zipios++.
                //   The zips are malformed and unreadable by most zip 
                //   programs including java. This is a just a minimum fix
                //   to get the zips java readable. A more complete fix
                //   is performed in the 1.00 Enigma sources.
                ba[0x0E] = ba[basize - 61];  // copy CRC from end to start
                ba[0x0F] = ba[basize - 60];
                ba[0x10] = ba[basize - 59];
                ba[0x11] = ba[basize - 58];
                int compsize = basize - 77 - 39;
                ba[0x12] = (byte)((char)(compsize & 0xFF)) ;  // copy compressed size
                ba[0x13] = (byte)((char)(compsize & 0xFF00) >>> 8);
                ba[0x14] = (byte)((char)(compsize & 0xFF0000) >>> 16);
                ba[0x15] = (byte)((char)(compsize & 0xFF000000) >>> 24);
                ba[0x16] = ba[basize - 53];  // copy orig size
                ba[0x17] = ba[basize - 52];
                ba[0x18] = ba[basize - 51];
                ba[0x19] = ba[basize - 50];
            }
            
//             BufferedOutputStream out = new BufferedOutputStream(new FileOutputStream("score.zip"));
//             for (int i = 0; i < basize; i++)
//                 out.write(ba[i]);
//             out.flush();
            
            ZipInputStream zin = new ZipInputStream(new ByteArrayInputStream(ba));
            zin.getNextEntry();

//             BufferedOutputStream out = new BufferedOutputStream(new FileOutputStream("score.xml"));
//             
//             int co;
//             while ((co = zin.read()) != -1)
//                out.write(co);
//             out.flush();

            UserManager userMgr = new UserManager(userUrl);
            
            ScoreManager scm = new ScoreManager(zin);
            String userId = scm.getProperty("UserId");
            System.out.println("User '" + scm.getProperty("UserName") +
                    "' - Id '" + userId + "'");
            System.out.println("Score version = " + scm.getProperty("Count")
                    + ", elements = " + scm.getScoreElemsCount());
            
            if (scm.checkUser(userMgr)) {
                scm.checkScores(ratingMgr, userMgr, true);
    
                ZipOutputStream zout = new ZipOutputStream (new BufferedOutputStream(
                        new FileOutputStream(userId +".zip")));
                ZipEntry ze = new ZipEntry("score.xml");
                zout.putNextEntry(ze);
                scm.save(zout);
                zout.close();
                userMgr.save(userUrl);
            }
        } catch ( Exception ex ) {
            ex.printStackTrace();
        }
    }
}
