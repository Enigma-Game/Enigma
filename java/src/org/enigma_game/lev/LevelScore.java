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

import java.util.*; 
import org.w3c.dom.*;
import org.enigma_game.lev.UserManager;

public class LevelScore {
    Element elem;
    LevelScore parentScore = null;
    static String currentRel = "1.00";
    boolean isDiffSolved = false;
    boolean isEasySolved = false;
    int bsd = -1;
    String bsdh;
    Set<String> oldBsdh = new HashSet<String>();      // legacy old scores
    Set<String> oldConfBsdh = new HashSet<String>();  // confirmed old scores
    Set<String> newConfBsdh = new HashSet<String>();  // confirmed new scores
    int bse = -1;
    String bseh;
    Set<String> oldBseh = new HashSet<String>();
    Set<String> oldConfBseh = new HashSet<String>();
    Set<String> newConfBseh = new HashSet<String>();
    double pareSum;  // harm average: sum 1/score
    double pardSum;
    int numProfE;
    int numProfD;
    int solvne;   // num
    int solvpe;   // percentage * 100
    int solvnd;
    int solvpd;
    int avgurNum;
    int avgurSum;
    String title = "";
    String author = "";
    String relPath = "";
    boolean hasEasy = true;
    boolean partOfCurDist = false;
    int userScoreDiff;
    int userScoreEasy;
    boolean userDiffSolved = false;
    boolean userEasySolved = false;
    static Map<String, Integer> wrHolders = new HashMap<String, Integer>();
    static Map<String, Integer> wrSharers = new HashMap<String, Integer>();
    boolean fullEval = false;
    Map<Integer, String> scoresDiff = new TreeMap<Integer, String>();
    Map<Integer, String> scoresEasy = new TreeMap<Integer, String>();
    
    static void printWRStatistics(UserManager userMgr) {
        Map<String, Integer> wrHoldersTotal = new HashMap<String, Integer>(wrHolders);
        System.out.println("Unique Worldrecords:");
        for (Map.Entry<String, Integer> entry : wrHolders.entrySet()) {
            System.out.println("  " + entry.getValue() + " "+ entry.getKey());
        }
        System.out.println("Shared Worldrecords:");
        for (Map.Entry<String, Integer> entry : wrSharers.entrySet()) {
            System.out.println("  " + entry.getValue() + " "+ entry.getKey());
            Integer numUniqueWr = wrHoldersTotal.get(entry.getKey());
            wrHoldersTotal.put(entry.getKey(), (numUniqueWr == null 
                        ? entry.getValue()
                        : numUniqueWr + entry.getValue()));
        }
        
        Map<Integer, String> wrRanking = new TreeMap<Integer, String>();
        for (Map.Entry<String, Integer> entry : wrHoldersTotal.entrySet()) {
            String wrHolders = wrRanking.get(entry.getValue());
            wrRanking.put(entry.getValue(), (wrHolders == null ? entry.getKey()
                        : wrHolders + " + " + entry.getKey()));
        }
        System.out.println("Worldrecords Total Ranking:");
        for (Map.Entry<Integer, String> entry : wrRanking.entrySet()) {
            if (!entry.getValue().equals(""))
                System.out.println("  " + entry.getKey() + " "+ entry.getValue());
        }
        
        for (String userId : userMgr.getUserIds()) {
            String name = userMgr.getValue(userId, "name");
            Integer wrUnique = wrHolders.get(name);
            Integer wrShared = wrSharers.get(name);
            int sharedWRcount = ((wrShared != null) ? wrShared : 0);
            int totalWRrcount = ((wrUnique != null) ? wrUnique : 0) + sharedWRcount;
            
            Formatter formatterWRtotal = new Formatter(Locale.US);
            formatterWRtotal.format("%3d",  totalWRrcount);
            userMgr.setValue(userId, "wrtotal", formatterWRtotal.toString());
            
            Formatter formatterWRshared = new Formatter(Locale.US);
            formatterWRshared.format("%3d",  sharedWRcount);
            userMgr.setValue(userId, "wrshared", formatterWRshared.toString());
        }
    }
    
    public LevelScore(Element e, String bestScoreDiff, String bestScoreDiffHolder,
            String bestScoreEasy, String bestScoreEasyHolder) {
        elem = e;
        if (bestScoreDiff.length() > 0)
            bsd = Integer.parseInt(bestScoreDiff);
        bsdh = bestScoreDiffHolder;
        oldBsdh.addAll(Arrays.asList(bsdh.split("\\x2B"))); // "+"
        if (bestScoreEasy.length() > 0)
            bse = Integer.parseInt(bestScoreEasy);
        bseh = bestScoreEasyHolder;
        oldBseh.addAll(Arrays.asList(bseh.split("\\x2B"))); // "+"
    }
    
    public void setParentScore(LevelScore parent) {
        parentScore = parent;
    }
    
    public void renameUser(String oldname, String newname) {
        if (oldBsdh.remove(oldname)) {
            oldBsdh.add(newname);
            String holders = "";
            for(String name : oldBsdh) {
                holders += name + "+";
            }
            if (holders.endsWith("+"))
                holders = holders.substring(0, holders.length() -1);
            elem.setAttributeNS(null, "bsdh", holders);
        }
        if (oldBseh.remove(oldname)) {
            oldBseh.add(newname);
            String holders = "";
            for(String name : oldBseh) {
                holders += name + "+";
            }
            if (holders.endsWith("+"))
                holders = holders.substring(0, holders.length() -1);
            elem.setAttributeNS(null, "bseh", holders);
        }
    }
    
    public void addIndexInfo(String levelTitle, String levelAuthor, 
            String rPath,  String hasEasyMode) {
        partOfCurDist = true;
        title = levelTitle;
        author = levelAuthor;
        relPath = rPath;
        if (hasEasyMode.length() > 0)
            hasEasy = Boolean.parseBoolean(hasEasyMode);
    }
    
    public void setFullEvaluation(String pattern) {
        if (elem.getAttribute("id").contains(pattern) ||
                author.contains(pattern) || title.contains(pattern) ||
                relPath.contains(pattern)) {
            fullEval = true;
        }
    }
    
    public boolean isPartOfCurDist() {
        return partOfCurDist;
    }
    
    public String getTitle() {
        return title;
    }
    
    public boolean hasEasyMode() {
        return hasEasy;
    }
    
    public boolean isDiffSolved() {
        return isDiffSolved;
    }
    
    public boolean isEasySolved() {
        return isEasySolved;
    }
    
    public double getParDiff() {
        String parStr = elem.getAttribute("pard");
        double par = -1;
        if (parStr.length() > 0)
            par = Double.parseDouble(parStr);
        return par;
    }
    
    public double getParEasy() {
        String parStr = elem.getAttribute("pare");
        double par = -1;
        if (parStr.length() > 0)
            par = Double.parseDouble(parStr);
        return par;
    }
    
    public int getRatingNum() {
        return avgurNum;
    }
    
    public double getRatingAvg() {
        if (avgurNum >  0)
            return (double)avgurSum/avgurNum;
        else
            return -1;
    }
    
    public String getAuthor() {
        return author;
    }
    
    public String getId() {
        return elem.getAttribute("id");
    }
    
    public String getScoreVersion() {
        return elem.getAttribute("sv");
    }
    
    public boolean hasUserDiffSolved() {
        return userDiffSolved;
    }
    
    public boolean hasUserEasySolved() {
        return userEasySolved;
    }
    
    public int getUserScoreDiff() {
        return userScoreDiff;
    }
    
    public int getUserScoreEasy() {
        return userScoreEasy;
    }
    
    public boolean isUserWRHolderDiff() {
        return userScoreDiff >= 0 && userScoreDiff == bsd;
    }
    
    public boolean isUserWRHolderEasy() {
        return userScoreEasy >= 0 && userScoreEasy == bse;
    }
    
    public boolean isUniqueWRDiff() {
        return oldBsdh.size() + oldConfBsdh.size() + newConfBsdh.size() == 1;
    }
    
    public boolean isUniqueWREasy() {
        return oldBseh.size() + oldConfBseh.size() + newConfBseh.size() == 1;
    }
    
    public void resetUserSolved() {
        userScoreDiff = -1;
        userScoreEasy = -1;
        userDiffSolved = false;
        userEasySolved = false;
    }
    
    public void registerSolvage(String userName, String diffStr, String easyStr) {
        if (diffStr.length() > 0)
            userScoreDiff = Integer.parseInt(diffStr);
        if (easyStr.length() > 0)
            userScoreEasy = Integer.parseInt(easyStr);
        
        // user solved ?
        if (userScoreDiff >= 0) {
            userDiffSolved = true;
            if (fullEval) {
                String tieUsers = scoresDiff.get(userScoreDiff);
                scoresDiff.put(userScoreDiff, tieUsers == null ? userName :
                        tieUsers + "+" + userName); 
            }
        }
        if (userScoreEasy >= 0) {
            userEasySolved = true;
            if (fullEval) {
                String tieUsers = scoresEasy.get(userScoreEasy);
                scoresEasy.put(userScoreEasy, tieUsers == null ? userName :
                        tieUsers + "+" + userName); 
            }
        }
    }
    
    public void printLevelEvaluation() {
        if( fullEval) {
            System.out.println("");
            System.out.println("Level " + elem.getAttribute("id")
                    + "  '" + title + "' by " + author);
            for (Map.Entry<Integer, String> entry : scoresDiff.entrySet()) {
                System.out.println(entry.getKey() + "  " +  entry.getValue());
            }
            if (hasEasy) {
                System.out.println("");
                System.out.println("Easy mode:");
                for (Map.Entry<Integer, String> entry : scoresEasy.entrySet()) {
                    System.out.println(entry.getKey() + "  " +  entry.getValue());
                }
            }
        }
    }
    
    public void registerScore(String user, boolean isProfessional, String rating, 
            String diffStr, String diffRel, String easyStr, String easyRel,
            String diff2Str, String diff2Rel, String easy2Str, String easy2Rel) {
        int usd = -1;
        int use = -1;
        int usd2 = -1;
        int use2 = -1;
        int urat = -1;
        if (diffStr.length() > 0)
            usd = Integer.parseInt(diffStr);
        if (easyStr.length() > 0)
            use = Integer.parseInt(easyStr);
        if (diff2Str.length() > 0)
            usd2 = Integer.parseInt(diff2Str);
        if (easy2Str.length() > 0)
            use2 = Integer.parseInt(easy2Str);
        if (rating.length() > 0)
            urat = Integer.parseInt(rating);
        
        // solved with current release?
        if ((usd >= 0 && diffRel.equals(currentRel)) 
                || (usd2 >= 0 && diff2Rel.equals(currentRel))) {
            isDiffSolved = true;
        }
        if ((use >= 0 && easyRel.equals(currentRel))
                || (use2 >= 0 && easy2Rel.equals(currentRel))) {
            isEasySolved = true;
        }
        
        if (usd >= 0) {
            if (usd < bsd || bsd == -1) {
                // new world record
                bsd = usd;
                newConfBsdh.clear();
                newConfBsdh.add(user);
                oldBsdh.clear();
                oldConfBsdh.clear();
            } else if (usd == bsd) {
                // same as world record
                if (oldBsdh.contains(user)) {
                    oldBsdh.remove(user);
                    oldConfBsdh.add(user);
                } else {
                    newConfBsdh.add(user);
                }
            }
            solvnd++;
            if (usd == 0)
                usd = 1;
            if (isProfessional) {
                pardSum += 1.0/usd;
                numProfD++;
            }
        }
        if (use >= 0) {
            if (use < bse || bse == -1) {
                // new world record
                bse = use;
                newConfBseh.clear();
                newConfBseh.add(user);
                oldBseh.clear();
                oldConfBseh.clear();
            } else if (use == bse) {
                // same as world record
                if (oldBseh.contains(user)) {
                    oldBseh.remove(user);
                    oldConfBseh.add(user);
                } else {
                    newConfBseh.add(user);
                }
            }
            solvne++;
            if (use == 0)
                use = 1;
            if (isProfessional) {
                pareSum += 1.0/use;
                numProfE++;
            }
        }
        if (urat >= 0) {
            avgurNum++;
            avgurSum += urat;
        }
    }
    
    public void finish(int numUsers, int numProf) {
        elem.setAttributeNS(null, "bsd", Integer.toString(bsd));
        
        String holders = "";
        if (oldBsdh.isEmpty() && oldConfBsdh.isEmpty()) {
            for(String name : newConfBsdh)
                holders += name + "+";
        } else {
            for(String name : oldConfBsdh)
                holders += name + "+";
            for(String name : newConfBsdh)   // we may omit new record holders
                holders += name + "+";       // if there are too many 
            for(String name : oldBsdh)
                holders += name + "+";
        }
        if (holders.endsWith("+"))
            holders = holders.substring(0, holders.length() -1);
        elem.setAttributeNS(null, "bsdh", holders);
        
        elem.setAttributeNS(null, "bse", Integer.toString(bse));
        
        holders = "";
        if (oldBseh.isEmpty() && oldConfBseh.isEmpty()) {
            for(String name : newConfBseh)
                holders += name + "+";
        } else {
            for(String name : oldConfBseh)
                holders += name + "+";
            for(String name : newConfBseh)   // we may omit new record holders
                holders += name + "+";       // if there are too many 
            for(String name : oldBseh)
                holders += name + "+";
        }
        if (holders.endsWith("+"))
            holders = holders.substring(0, holders.length() -1);
        elem.setAttributeNS(null, "bseh", holders);
        
        if (solvne == 0) {
            pareSum = -1;
        } else {
            // rate profs that did not solve level as 10 * world record
            double subScore = 10.0 * Math.max(bse, 1.0);
            pareSum = (double)numProf / (pareSum + (numProf - numProfE)/subScore) + 0.5;
            if (pareSum >= (99*60+59.5))
                pareSum =  -1;
        }
        elem.setAttributeNS(null, "pare", Integer.toString((int)pareSum));

        if (solvnd == 0) {
            pardSum = -1;
        } else {
            // rate profs that did not solve level as 10 * world record
            double subScore = 10.0 * Math.max(bsd, 1.0);
            pardSum = (double)numProf / (pardSum + (numProf - numProfD)/subScore) + 0.5;
            if (pardSum >= (99*60+59.5))
                pardSum =  -1;
        }
        elem.setAttributeNS(null, "pard", Integer.toString((int)pardSum));

        elem.setAttributeNS(null, "solvne", Integer.toString(solvne));
        solvpe = solvne * 10000 / numUsers;
        elem.setAttributeNS(null, "solvpe", Integer.toString(solvpe));
        
        elem.setAttributeNS(null, "solvnd", Integer.toString(solvnd));
        solvpd = solvnd * 10000 / numUsers;
        elem.setAttributeNS(null, "solvpd", Integer.toString(solvpd));
        
        int avgur = -1;
        if (avgurNum >  0 || (parentScore != null && parentScore.getRatingNum() > 0)) { 
            int parentAddNum = 0;
            double parentAddSum = 0;
            if (parentScore != null && avgurNum < 10) {
                if (parentScore.getRatingNum() > 10 - avgurNum) {
                    parentAddNum = 10 - avgurNum;
                    parentAddSum = parentAddNum * parentScore.getRatingAvg();
                } else if (parentScore.getRatingNum() > 0) {
                    parentAddNum = parentScore.getRatingNum();
                    parentAddSum = parentAddNum * parentScore.getRatingAvg();
                }
            }
            parentAddSum = 10 * parentAddSum;
            avgur = (avgurSum * 10 + (int)parentAddSum) / (avgurNum + parentAddNum);
        }
        elem.setAttributeNS(null, "avgur", Integer.toString(avgur));
        
        // worldrecord holder statistics
        if (partOfCurDist) {
            int numWRHolder = oldBsdh.size() + oldConfBsdh.size() + newConfBsdh.size();
            if (numWRHolder > 1) {
                for(String name : newConfBsdh) {
                    Integer n = wrSharers.get(name);
                    wrSharers.put(name, (n == null ? 1 : n+1));
                }
                for(String name : oldConfBsdh) {
                    Integer n = wrSharers.get(name);
                    wrSharers.put(name, (n == null ? 1 : n+1));
                }
                for(String name : oldBsdh) {
                    Integer n = wrSharers.get(name);
                    wrSharers.put(name, (n == null ? 1 : n+1));
                }
            } else {
                for(String name : newConfBsdh) {
                    Integer n = wrHolders.get(name);
                    wrHolders.put(name, (n == null ? 1 : n+1));
                }
                for(String name : oldConfBsdh) {
                    Integer n = wrHolders.get(name);
                    wrHolders.put(name, (n == null ? 1 : n+1));
                }
                for(String name : oldBsdh) {
                    Integer n = wrHolders.get(name);
                    wrHolders.put(name, (n == null ? 1 : n+1));
                }
            }
            if (hasEasy) {
                numWRHolder = oldBseh.size() + oldConfBseh.size() + newConfBseh.size();
                if (numWRHolder > 1) {
                    for(String name : newConfBseh) {
                        Integer n = wrSharers.get(name);
                        wrSharers.put(name, (n == null ? 1 : n+1));
                    }
                    for(String name : oldConfBseh) {
                        Integer n = wrSharers.get(name);
                        wrSharers.put(name, (n == null ? 1 : n+1));
                    }
                    for(String name : oldBseh) {
                        Integer n = wrSharers.get(name);
                        wrSharers.put(name, (n == null ? 1 : n+1));
                    }
                } else {
                    for(String name : newConfBseh) {
                        Integer n = wrHolders.get(name);
                        wrHolders.put(name, (n == null ? 1 : n+1));
                    }
                    for(String name : oldConfBseh) {
                        Integer n = wrHolders.get(name);
                        wrHolders.put(name, (n == null ? 1 : n+1));
                    }
                    for(String name : oldBseh) {
                        Integer n = wrHolders.get(name);
                        wrHolders.put(name, (n == null ? 1 : n+1));
                    }
                }
            }
        }
    }
    
    public static class ComparatorRating implements Comparator<LevelScore> {
        public ComparatorRating() {
        }
        
        public int compare(LevelScore ls1, LevelScore ls2) {
            if (ls1.getRatingAvg() > ls2.getRatingAvg())
                return -1;
            else if (ls1.getRatingAvg() < ls2.getRatingAvg())
                return 1;
            else {
                if (ls1.getRatingNum() > ls2.getRatingNum())
                    return -1;
                else if (ls1.getRatingNum() < ls2.getRatingNum())
                    return 1;
                else
                    return 0;
            }
        }
    }
}
