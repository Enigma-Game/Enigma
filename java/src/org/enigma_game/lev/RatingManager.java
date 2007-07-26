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
import javax.xml.datatype.*;
import org.w3c.dom.*;
import org.w3c.dom.ls.*;

import org.enigma_game.EnigmaUtil;
import org.enigma_game.lev.LevelScore;
import org.enigma_game.lev.LevelpackManager;
import org.enigma_game.lev.UserManager;

public class RatingManager {
    EnigmaUtil theApp = org.enigma_game.EnigmaUtil.theApp;
    Document doc;
    Element updateElem;
    Element levelsElem;
    NodeList levelElems;
    Map<String, LevelScore> levelScores = new HashMap<String, LevelScore>();
    List<LevelpackManager> levelPacks = new ArrayList<LevelpackManager>();
    int numDifficult;
    int numEasy;
    int numProf = 0;
    static double log_2 = Math.log(2.0);

    public RatingManager(String url) {
        doc = theApp.domParser.parseURI(url);

        DOMConfiguration config = doc.getDomConfig();
        config.setParameter("error-handler", theApp);
        // set validation feature
        config.setParameter("validate", Boolean.FALSE);
        config.setParameter("schema-type", "http://www.w3.org/2001/XMLSchema");
        //config.setParameter("schema-location","data/personal.xsd");

        updateElem = (Element) doc.getElementsByTagName("update").item(0);
        levelsElem = (Element) doc.getElementsByTagName("levels").item(0);
        levelElems = doc.getElementsByTagName("level");
    }

    public void setUpLevelScores(String levelPattern) {
        for (int i = 0; i < levelElems.getLength(); i++) {
            Element e = (Element) levelElems.item(i);
            String key = e.getAttribute("id") + "_" + e.getAttribute("sv");
            LevelScore ls = new LevelScore(e, e.getAttribute("bsd"),
                    e.getAttribute("bsdh"), e.getAttribute("bse"), e.getAttribute("bseh"));
            levelScores.put(key, ls);
        }
        LevelpackManager lpmgr;
        lpmgr = new LevelpackManager("enigma0_92_1");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma0_92_2");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma0_92_3");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("newlevels");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma_tutorial");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma_i");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma_ii");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma_iii");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma_iv");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma_v");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma_vi");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma_vii");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma_esprit");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma_oxyd");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma_peroxyd");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma_oxydextra");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma_oxydmagnum");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma_sokoban");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma_microban");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);
        lpmgr = new LevelpackManager("enigma_mas_microban");
        lpmgr.registerLevels(this);
        levelPacks.add(lpmgr);

        for(Map.Entry<String, LevelScore> entry : levelScores.entrySet()) {
            LevelScore lev = entry.getValue();
            int sv = Integer.parseInt(lev.getScoreVersion());
            if (sv > 1) {
                LevelScore parentScore = getLevelScore(lev.getId(), Integer.toString(sv - 1));
                if (parentScore != null) {
                    lev.setParentScore(parentScore);
                }
            }
            if (lev.isPartOfCurDist()) {
                numDifficult++;
                if (lev.hasEasyMode()) {
                    numEasy++;
                }
            }
            if (!levelPattern.equals("")) {
                lev.setFullEvaluation(levelPattern);
            }
        }
    }

    public LevelScore getLevelScore(String id, String scoreVersion) {
        return levelScores.get(id + "_" + scoreVersion);
    }

    public LevelScore newLevelScore(String id, String scoreVersion) {
        Element level = doc.createElementNS(null, "level");
        level.setAttributeNS(null, "id", id);
        level.setAttributeNS(null, "sv", scoreVersion);
        levelsElem.appendChild(level);

        LevelScore ls = new LevelScore(level, "", "", "", "");
        String key = id + "_" + scoreVersion;
        levelScores.put(key, ls);

        System.out.println("Missing Rating: id '" + id + "' score version "
                + scoreVersion);
        return ls;
    }

    public void clearUserScores() {
        for(Map.Entry<String, LevelScore> entry : levelScores.entrySet()) {
            if (entry.getValue().isPartOfCurDist()) {
                entry.getValue().resetUserSolved();
            }
        }
    }

    public boolean isUserProfessional(UserManager userMgr, String userId, boolean print) {
        boolean isProfessional = false;
        int userDiffCount = 0;
        int userEasyCount = 0;
        for(Map.Entry<String, LevelScore> entry : levelScores.entrySet()) {
            LevelScore ls = entry.getValue();
            if (ls.isPartOfCurDist()) {
                if (ls.hasUserDiffSolved())
                    userDiffCount++;
//              else
//                  System.out.println("Diff " + ls.getTitle() + " by " + ls.getAuthor() + " id " +  ls.getId());
                if (ls.hasEasyMode()) {
                    if (ls.hasUserEasySolved())
                        userEasyCount++;
//                  else
//                      System.out.println("Easy " + ls.getTitle() + " by " + ls.getAuthor() + " id " +  ls.getId());
                }
            }
        }
        double solvedPercent = 100.0 * (userDiffCount + userEasyCount)/(numDifficult + numEasy);
        Formatter formatter = new Formatter(Locale.US);
        formatter.format("Solved diff %3d/%3d - easy %3d/%3d = %6.2f%%", userDiffCount,
                numDifficult, userEasyCount, numEasy, solvedPercent);
        if (print) {
            System.out.println(formatter.toString());
        }
        if (solvedPercent > 30.0) {
            isProfessional = true;
            numProf++;
        }
        Formatter formatterTotal = new Formatter(Locale.US);
        formatterTotal.format("%6.2f",  solvedPercent);
        userMgr.setValue(userId, "solvedtotal", formatterTotal.toString());

        Formatter formatterEasy = new Formatter(Locale.US);
        formatterEasy.format("%3d/%3d", userEasyCount, numEasy);
        userMgr.setValue(userId, "solvede", formatterEasy.toString());

        Formatter formatterDiff = new Formatter(Locale.US);
        formatterDiff.format("%4d/%4d", userDiffCount, numDifficult);
        userMgr.setValue(userId, "solvedd", formatterDiff.toString());

        return isProfessional;
    }

    public void evaluateUser(UserManager userMgr, String userId) {
//        System.out.println("Par Evalutation");
        StringBuilder sb = new StringBuilder();
        Formatter formatter = new Formatter(sb, Locale.US);

        for (LevelpackManager lpm : levelPacks) {
            double hcpe = 0;
            double hcpd = 0;
            for (LevelScore ls : lpm.getLevelScores()) {
                double dhcpd = levelHcp(ls.getUserScoreDiff(), ls.getParDiff());
                hcpd += dhcpd;
                if (ls.hasEasyMode())
                    hcpe += levelHcp(ls.getUserScoreEasy(), ls.getParEasy());
                else
                    hcpe += dhcpd;
            }
            hcpd = hcpd * 100.0 / lpm.size();
            hcpe = hcpe * 100.0 / lpm.size();
            formatter.format("%25s  Diff = %6.1f  Easy = %6.1f%n",
                    lpm.getName(), hcpd, hcpe);
        }
        double hcpsum = 0;
        int levelsum = 0;
        double hcpsumsolved = 0;
        int levelsumsolved = 0;
        int totalWRrcount = 0;
        int sharedWRcount = 0;
        for(Map.Entry<String, LevelScore> entry : levelScores.entrySet()) {
            LevelScore ls = entry.getValue();
            if (ls.isPartOfCurDist()) {
                hcpsum += levelHcp(ls.getUserScoreDiff(), ls.getParDiff());
                levelsum++;
                if (ls.hasUserDiffSolved()) {
                    hcpsumsolved += levelHcp(ls.getUserScoreDiff(), ls.getParDiff());
                    levelsumsolved++;
                }
                if (ls.isUserWRHolderDiff()) {
                    totalWRrcount++;
                    if (!ls.isUniqueWRDiff())
                        sharedWRcount++;
                }
                if (ls.hasEasyMode()) {
                    hcpsum += levelHcp(ls.getUserScoreEasy(), ls.getParEasy());
                    levelsum++;
                    if (ls.hasUserEasySolved()) {
                        hcpsumsolved += levelHcp(ls.getUserScoreEasy(), ls.getParEasy());
                        levelsumsolved++;
                    }
                    if (ls.isUserWRHolderEasy()) {
                        totalWRrcount++;
                        if (!ls.isUniqueWREasy())
                            sharedWRcount++;
                    }
                }
            }
        }
        hcpsum = hcpsum * 100.0 / levelsum;
        int unsolved = numDifficult + numEasy - levelsumsolved;
        hcpsumsolved += unsolved / 10.0;  // add 10% of unsolved levels as unsuccessful attempts
        levelsumsolved += unsolved / 10;
        hcpsumsolved = hcpsumsolved  * 100.0 / levelsumsolved;
        formatter.format("Enigma Total Hcp = %6.1f, Solved Hcp = %6.1f at %d levels",
                hcpsum, hcpsumsolved, levelsum);
//        System.out.println(sb.toString());

        Formatter formatterHcpTotal = new Formatter(Locale.US);
        formatterHcpTotal.format("%6.1f",  hcpsum);
        userMgr.setValue(userId, "hcptotal", formatterHcpTotal.toString());

        Formatter formatterHcpSolved = new Formatter(Locale.US);
        formatterHcpSolved.format("%6.1f",  hcpsumsolved);
        userMgr.setValue(userId, "hcpsolved", formatterHcpSolved.toString());

        // future direct output of WR based only on registerd scores
//         Formatter formatterWRtotal = new Formatter(Locale.US);
//         formatterWRtotal.format("%3d",  totalWRrcount);
//         userMgr.setValue(userId, "wrtotal", formatterWRtotal.toString());
//
//         Formatter formatterWRshared = new Formatter(Locale.US);
//         formatterWRshared.format("%3d",  sharedWRcount);
//         userMgr.setValue(userId, "wrshared", formatterWRshared.toString());
    }

    private double levelHcp(int score, double par) {
        double dhcp = 0;
        if (score == -1) {
            dhcp = 1;
        } else if (score == -2) {
            dhcp = 0.7;
        } else if (score >= par && par > 0) {
            dhcp = Math.log10(score/par);
            if (dhcp > 0.7)
                dhcp = 0.7;
        } else if (score < par && par > 0) {
            dhcp = Math.log(score/par) / log_2;
            if (dhcp < -3)
                dhcp = -3;
        } else { // par <= 0 no par
            dhcp = -3;
        }
        return dhcp;
    }

    public void finishLevelScores(int numUsers, UserManager userMgr) {
        int diffSolved = 0;
        int diffUnsolved = 0;
        int easySolved = 0;
        int easyUnsolved = 0;
        for (Map.Entry<String, LevelScore> entry : levelScores.entrySet()) {
            LevelScore ls = entry.getValue();
            ls.finish(numUsers, numProf);
            if (ls.isPartOfCurDist()) {
                if (ls.isDiffSolved()) {
                    diffSolved++;
                } else {
                    diffUnsolved++;
                    System.out.println("Diff unsolved: " + entry.getKey()
                            + " - " + ls.getTitle());
                }
                if (ls.hasEasyMode()) {
                    if (ls.isEasySolved()) {
                        easySolved++;
                    } else {
                        easyUnsolved++;
                        System.out.println("Easy unsolved: " + entry.getKey()
                                + " - " + ls.getTitle());
                    }
                }
            }
        }
        System.out.println("Diff Levels " + numDifficult + " Easy Levels " + numEasy);
        System.out.println("Diff Solved " + diffSolved + " / Unsolved " + diffUnsolved);
        System.out.println("Easy Solved " + easySolved + " / Unsolved " + easyUnsolved);
        System.out.println("Total Solved " + LevelScore.solvedTotal);
        LevelScore.printWRStatistics(userMgr);


        try {
            PrintWriter output = new PrintWriter(new FileWriter(new File("top_rated_levels.txt")), true);
            output.println("Top Rated Levels (with more than one rating)");
            List<LevelScore> topRatedLevel = new ArrayList<LevelScore>(levelScores.values());
            LevelScore.ComparatorRating compRating = new LevelScore.ComparatorRating();
            Collections.sort(topRatedLevel, compRating);
            int topPosition = 1;
            for (LevelScore ls : topRatedLevel) {
                if (ls.isPartOfCurDist() && (ls.getRatingNum() > 1 || ls.getInheritedRatingNum() > 2)) {
                    Formatter formatterTopRatedLevel = new Formatter(Locale.US);
                    formatterTopRatedLevel.format("%5.2f  %2d (%2d)  %-23s by %-18s (%s)",  ls.getRatingAvg(),
                            ls.getRatingNum(), ls.getInheritedRatingNum(), ls.getTitle(), ls.getAuthor(), ls.getId());
                    output.println(formatterTopRatedLevel.toString());
                    topPosition++;
                    if (topPosition > 60)
                        break;
                }
            }

            output = new PrintWriter(new FileWriter(new File("stat-other.html")), true);
            StringBuilder sb = new StringBuilder();
            Formatter formatter = new Formatter(sb, Locale.US);
            formatter.format("  <div class=\"stat-help\">\n");
            formatter.format("    <h3>Other Statistics</h3>\n");
            formatter.format("    <h4>Scores</h4>\n");
            formatter.format("      %d single level scores have been registered.\n", LevelScore.solvedTotal);
            formatter.format("    <h4>Ratings</h4>\n");
            formatter.format("      %s single level ratings have been registered with an average of "
                    + "%4.2f and the following distribution: \n", LevelScore.ratingsTotal,
                    ((double)LevelScore.ratingsTotalSum)/LevelScore.ratingsTotal);
            formatter.format("      <table>\n");
            formatter.format("        <colgroup><col width=\"80\"><col width=\"80\"></colgroup>\n");
            formatter.format("        <tr><th>rating</th><th>count</th></tr>\n");
            for (int i=0; i<=10; i++) {
                formatter.format("        <tr><td class=\"num\">%d</td><td class=\"num\">%d</td></tr>\n", i, LevelScore.ratingsDist[i]);
            }
            formatter.format("      </table>\n");
            formatter.format("  </div>\n");
            output.print(sb.toString());
            output.flush();
        } catch (Exception e) {
        }

        System.out.println("Total Ratings " + LevelScore.ratingsTotal + "  Average " +
                ((double)LevelScore.ratingsTotalSum)/LevelScore.ratingsTotal);
        for (int i=0; i<=10; i++) {
            System.out.println("   " + i + " - " + LevelScore.ratingsDist[i]);
        }
    }

    public void evaluateLevels() {
        for (Map.Entry<String, LevelScore> entry : levelScores.entrySet()) {
            LevelScore ls = entry.getValue();
            ls.printLevelEvaluation();
        }
    }

    public void renameUser(String oldname, String newname) {
        for(Map.Entry<String, LevelScore> entry : levelScores.entrySet()) {
            entry.getValue().renameUser(oldname, newname);
        }
    }

    public void save(String url) {
        //doc.normalizeDocument();
        System.out.println("save " + url + " - "+ levelElems.getLength());
        theApp.domWriter.writeToURI(doc, url);
    }

    public String getDate() {
        return updateElem.getAttribute("date");
    }

    public void setDate(String date) {
        if (date.equals("")) {
            GregorianCalendar calendar = new GregorianCalendar(TimeZone.getTimeZone("GMT"));
            Date now = new Date();
            calendar.setTime(now);
            XMLGregorianCalendar xmlCal;
            try {
                xmlCal = DatatypeFactory.newInstance().newXMLGregorianCalendar(calendar);
                date = xmlCal.toXMLFormat();
            } catch( Exception ex ) {
                System.out.println("Ratings cannot set current update time");
                ex.printStackTrace();
            }
        }
        updateElem.setAttribute("date", date);
    }

    public String getUrlFull() {
        return updateElem.getAttribute("urlFull");
    }

    public void setUrlFull(String url) {
        updateElem.setAttribute("urlFull", url);
    }

    public String getUrlIncremental() {
        return updateElem.getAttribute("urlIncremental");
    }

    public void setUrlIncremental(String url) {
        updateElem.setAttribute("urlIncremental", url);
    }

    public Set<Map<String,String>> getRatingData() {
        Map<String, String> level;
        Set<Map<String,String>> allData = new HashSet<Map<String,String>>();
        for (int i = 0; i < levelElems.getLength(); i++) {
            level = new HashMap<String, String>();
            NamedNodeMap attributeNodes = levelElems.item(i).getAttributes();
            for (int j = 0; j < attributeNodes.getLength(); j++) {
                Attr attr = (Attr)attributeNodes.item(j);
                level.put(attr.getName(), attr.getValue());
            }
            allData.add(level);
        }
        return allData;
    }

    public void addRatingData(Set<Map<String,String>> additions) {
        for (Map<String,String> rating : additions) {
            Element level = doc.createElementNS(null, "level");
            for (Map.Entry<String, String> attr : rating.entrySet()) {
                level.setAttributeNS(null, attr.getKey(), attr.getValue());
            }
            levelsElem.appendChild(level);
        }
    }
}
