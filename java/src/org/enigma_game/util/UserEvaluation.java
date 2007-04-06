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

public class UserEvaluation {
    EnigmaUtil theApp = org.enigma_game.EnigmaUtil.theApp;
    
    public UserEvaluation(String rating, String userUrl,
            String[] scoreFiles, int firstIndex) {
        try {
            System.out.println("UserEvaluation");
            RatingManager ratingMgr = new RatingManager(rating);
            ratingMgr.setUpLevelScores("");
            UserManager userMgr = new UserManager(userUrl);
            PrintWriter htmlOut;
            
            GregorianCalendar calendar = new GregorianCalendar(TimeZone.getTimeZone("GMT"));
            Date now = new Date();
            calendar.setTime(now);
            calendar.add(Calendar.DATE, -15);  // monthly evaluation within first 2 weeks of next month
            Formatter formatter = new Formatter(Locale.US);
            formatter.format("%1$tB %1$tY",calendar);

            int numUsers = 0;
            for (int i = firstIndex; i < scoreFiles.length; i++) {
                ZipInputStream zin = new ZipInputStream(new BufferedInputStream(new FileInputStream(scoreFiles[i])));
                zin.getNextEntry();
                ScoreManager scm = new ScoreManager(zin);
                System.out.println("User '" + scm.getProperty("UserName") +
                        "' - Id '" + scm.getProperty("UserId") + "'");
                scm.checkScores(ratingMgr, userMgr, false);
                ratingMgr.evaluateUser(userMgr, scm.getProperty("UserId"));
                System.out.println("");
                numUsers++;
            }
            userMgr.save(userUrl);
            
            System.out.println("");
            System.out.println("User Names:");
            htmlOut = new PrintWriter(new FileWriter(new File("userlist.html")), true);
            htmlOut.println("<div class=\"usernames\">");
            htmlOut.println("Names in use:");
            htmlOut.println("  <ul>");
            List<String> userName = new ArrayList<String>(userMgr.getUserIds());
            UserManager.ComparatorName compName = userMgr.new ComparatorName();
            Collections.sort(userName, compName);
            for (String id : userName) {
                System.out.println(userMgr.getValue(id, "name"));
                htmlOut.println("    <li>" + userMgr.getValue(id, "name") + "</li>");
            }
            htmlOut.println("  </ul>");
            htmlOut.println("</div>");
            htmlOut.flush();
            
            htmlOut = new PrintWriter(new FileWriter(new File("table-wr.html")), true);
            System.out.println("");
            System.out.println("Worldrecord Statistics:");
            System.out.print("total (shared) - users");
            htmlOut.println("  <table>");
            htmlOut.println("    <caption>Worldrecord Statistics of " + formatter.toString() + "</caption>");
            htmlOut.println("    <colgroup><col width=\"80\"><col width=\"80\"><col width=\"470\"></colgroup>");
            htmlOut.print("    <tr><th>total</th><th>shared</th><th class=\"left\">users</th></tr>");
            List<String> userByWR = new ArrayList<String>(userMgr.getUserIds());
            UserManager.ComparatorWRTotal compWRTotal = userMgr.new ComparatorWRTotal();
            Collections.sort(userByWR, compWRTotal);
            String lastId = "";
            for (String id : userByWR) {
                if (compWRTotal.compare(lastId, id) != 0) {
                    System.out.println("");
                    System.out.print("  " + userMgr.getValue(id, "wrtotal") + " ("
                            + userMgr.getValue(id, "wrshared") + ") - '"
                            + userMgr.getValue(id, "name") + "'");
                    if (lastId.equals(""))
                        htmlOut.println("");
                    else
                        htmlOut.println("</td></tr>");
                    htmlOut.print("    <tr><td class=\"num\">" + userMgr.getValue(id, "wrtotal")
                            + "</td><td class=\"num\">" + userMgr.getValue(id, "wrshared")
                            + "</td><td class=\"left\">'" + userMgr.getValue(id, "name") + "'");
                } else {
                    System.out.print("+'" + userMgr.getValue(id, "name") + "'");
                    htmlOut.print(" + '" + userMgr.getValue(id, "name") + "'");
                }
                lastId = id;
            }
            System.out.println("");
            htmlOut.println("</td></tr>");
            htmlOut.println("  </table>");
            htmlOut.flush();

            
            htmlOut = new PrintWriter(new FileWriter(new File("table-solved.html")), true);
            System.out.println("");
            System.out.println("Solved Statistics:");
            System.out.println("solved difficult - solved easy = total solved - user");
            htmlOut.println("  <table>");
            htmlOut.println("    <caption>Solved Level Statistics of " + formatter.toString() + "</caption>");
            htmlOut.println("    <colgroup><col width=\"130\"><col width=\"130\"><col width=\"130\"><col width=\"240\"></colgroup>");
            htmlOut.println("    <tr><th>difficult</th><th>easy</th><th>total</th><th class=\"left\">user</th></tr>");
            List<String> userBySolved = new ArrayList<String>(userMgr.getUserIds());
            UserManager.ComparatorSolved compSolved = userMgr.new ComparatorSolved();
            Collections.sort(userBySolved, compSolved);
            for (String id : userBySolved) {
                String solvedtotal = userMgr.getValue(id, "solvedtotal");
                double solvedTotal = (!solvedtotal.equals("")) ? Double.parseDouble(solvedtotal.trim()) : 0;
                if (solvedTotal > 0) {
                    System.out.println("  " + userMgr.getValue(id, "solvedd")
                            + " - " + userMgr.getValue(id, "solvede")        
                            + " = " + userMgr.getValue(id, "solvedtotal")        
                            + "%  - '" + userMgr.getValue(id, "name") + "'");
                    htmlOut.println("    <tr><td class=\"num\">" + userMgr.getValue(id, "solvedd")
                            + "</td><td class=\"num\">" + userMgr.getValue(id, "solvede")
                            + "</td><td class=\"num\">" + userMgr.getValue(id, "solvedtotal")
                            + "%</td><td class=\"left\">'" + userMgr.getValue(id, "name") + "'</td></tr>");
                }
            }
            System.out.println("");
            htmlOut.println("  </table>");
            htmlOut.flush();
           
            htmlOut = new PrintWriter(new FileWriter(new File("table-hcp.html")), true);
            System.out.println("");
            System.out.println("Handicap Statistics:");
            System.out.println("total - solved hcp - user");
            htmlOut.println("  <table>");
            htmlOut.println("    <caption>Handicap Statistics of " + formatter.toString() + "</caption>");
            htmlOut.println("    <colgroup><col width=\"130\"><col width=\"220\"></colgroup>");
            htmlOut.println("    <tr><th>solved hcp</th><th class=\"left\">user</th></tr>");
            List<String> userByHCP = new ArrayList<String>(userMgr.getUserIds());
            UserManager.ComparatorHcpSolved compHCPSolved = userMgr.new ComparatorHcpSolved();
            Collections.sort(userByHCP, compHCPSolved);
            for (String id : userByHCP) {
                String hcptotal = userMgr.getValue(id, "hcptotal");
                double hcpTotal = (!hcptotal.equals("")) ? Double.parseDouble(hcptotal.trim()) : 100;
                if (hcpTotal < 100) {
                    System.out.println("  " + userMgr.getValue(id, "hcptotal")
                            + "   " + userMgr.getValue(id, "hcpsolved")
                            + " - '" + userMgr.getValue(id, "name") + "'");
                    htmlOut.println("     <tr><td class=\"num\">" + userMgr.getValue(id, "hcpsolved")
                            + "</td><td class=\"left\">'" + userMgr.getValue(id, "name") + "'</td></tr>");
                }
            }
            System.out.println("");
            htmlOut.println("  </table>");
            htmlOut.flush();
            
            System.out.println("UserEvaluation finished");
        } catch ( Exception ex ) {
            ex.printStackTrace();
        }
    }
}
