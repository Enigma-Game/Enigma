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
import org.enigma_game.EnigmaUtil;
import org.enigma_game.lev.RatingManager;
import org.enigma_game.lev.UserManager;

public class UserRename {
    EnigmaUtil theApp = org.enigma_game.EnigmaUtil.theApp;
    
    public UserRename(String ratingIn, String ratingOut, String userUrl) {
        try {
            System.out.println("User renaming");
            RatingManager ratingMgr = new RatingManager(ratingIn);
            ratingMgr.setUpLevelScores("");
            UserManager userMgr = new UserManager(userUrl);
            for (String id : userMgr.getUserIds()) {
                String newname = userMgr.getValue(id, "newname");
                if (!newname.equals("")) {
                    String oldname = userMgr.getValue(id, "name");
                    System.out.println("Rename '" + oldname + "' to '" + newname + "'");
                    ratingMgr.renameUser(oldname, newname);
                    userMgr.setValue(id, "name", newname);
                    userMgr.setValue(id, "newname", "");
                }
            }
            ratingMgr.setDate("");
            ratingMgr.save(ratingOut);
            userMgr.save(userUrl);
            
            System.out.println("User renaming finished");
        } catch ( Exception ex ) {
            ex.printStackTrace();
        }
    }
}
