/*
 * Copyright (C) 2006 Ronald Lamprecht
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
 */

#include "gui/LevelInspector.hh"
#include "enigma.hh"
#include "nls.hh"
#include "video.hh"
#include "ecl_util.hh"
#include "gui/menus_internal.hh"
#include "lev/RatingManager.hh"

#include <vector>

using namespace ecl;
using namespace enigma;

namespace enigma { namespace gui {

    
LevelInspector::LevelInspector(lev::Proxy *aLevel, ecl::Surface *preview):
        previewImage(preview), levelProxy(aLevel),
        annotation (new TextField()),
        back (new StaticTextButton(N_("Ok"), this))
    {
        const video::VMInfo *vminfo = video::GetInfo();
        vspacing = vminfo->height < 500 ? 2 :(vminfo->height < 650 ? 3 : 4);
        vmargin = vminfo->height < 500 ? 10 :(vminfo->height < 650 ?  20 : 30);
        hmargin = vminfo->width < 660 ? 10 : (vminfo->width < 900 ? 20 : 30);    
    
        add(back, Rect(vminfo->width-100-2*hmargin,vminfo->height-50,100,35));
    
        try {
            aLevel->loadMetadata();
        }
        catch (levels::XLevelLoading &err) {
            std::vector<string> lines;
            std::string errmsg = _("Server Error: could not load level '")
                                   + aLevel->getNormLevelPath() + "'\n"
                                   + err.what();
            ecl::split_copy (errmsg, '\n', back_inserter(lines));
            int x     = 60;
            int y     = 60;
            int yskip = 25;
            for (unsigned i=0; i<lines.size(); ++i) {
                add(new Label(lines[i], HALIGN_LEFT), Rect(x, y, vminfo->width-80,yskip));
                y += yskip;
            }
            return;
        }
    
        std::string tmp, tmp2;
        lev::RatingManager *theRatingMgr = lev::RatingManager::instance();
    
        BuildVList authorT(this, Rect(hmargin,vmargin,110,25), vspacing);
        authorT.add(new Label(N_("Level: "), HALIGN_RIGHT));
        authorT.add(new Label((""), HALIGN_RIGHT)); // subtitel
        authorT.add(new Label(N_("Author: "), HALIGN_RIGHT));
        authorT.add(new Label(N_("Contact: "), HALIGN_RIGHT));
        authorT.add(new Label(N_("Homepage: "), HALIGN_RIGHT));
        
        BuildVList author(this, Rect(hmargin+110+10,vmargin,
                vminfo->width-(hmargin+110+10)-10-(vminfo->thumbw+10)-hmargin,25), vspacing);
        tmp = levelProxy->getTitel();
        tmp2 = levelProxy->getLocalizedString("titel");
        if (tmp != tmp2)
            tmp = tmp + "  --  " + tmp2;
        author.add(new Label( tmp, HALIGN_LEFT));
        tmp = levelProxy->getLocalizedString("subtitel");
        if (tmp == "subtitel")
            tmp = "";
        author.add(new Label(tmp, HALIGN_LEFT)); // subtitel
        author.add(new Label(levelProxy->getAuthor(), HALIGN_LEFT));
        
        BuildVList address(this, Rect(hmargin+110+10,vmargin+3*(25+vspacing),
                vminfo->width-(hmargin+110+10)-hmargin,25), vspacing);
        address.add(new Label(levelProxy->getContact(), HALIGN_LEFT));
        address.add(new Label(levelProxy->getHomepage(), HALIGN_LEFT));
        
        BuildVList ratingPubT(this, Rect(hmargin+65,vmargin+5*25+4*vspacing+16, 130,25), 2);
        ratingPubT.add(new Label(N_("Public Ratings"), HALIGN_CENTER));
        BuildVList ratingPubST(this, Rect(hmargin,vmargin+6*25+5*vspacing+16, 130,25), 2);
        ratingPubST.add(new Label(N_("Intelligence: "), HALIGN_RIGHT));
        ratingPubST.add(new Label(N_("Dexterity: "), HALIGN_RIGHT));
        ratingPubST.add(new Label(N_("Patience: "), HALIGN_RIGHT));
        ratingPubST.add(new Label(N_("Knowledge: "), HALIGN_RIGHT));
        ratingPubST.add(new Label(N_("Speed: "), HALIGN_RIGHT));
        
        BuildVList ratingPub(this, Rect(hmargin+130+15,vmargin+6*25+5*vspacing+16, 10,25), 2);
        ratingPub.add(new Label(ratingToString(theRatingMgr->getIntelligence(aLevel)).c_str(), HALIGN_CENTER));
        ratingPub.add(new Label(ratingToString(theRatingMgr->getDexterity(aLevel)).c_str(), HALIGN_CENTER));
        ratingPub.add(new Label(ratingToString(theRatingMgr->getPatience(aLevel)).c_str(), HALIGN_CENTER));
        ratingPub.add(new Label(ratingToString(theRatingMgr->getKnowledge(aLevel)).c_str(), HALIGN_CENTER));
        ratingPub.add(new Label(ratingToString(theRatingMgr->getSpeed(aLevel)).c_str(), HALIGN_CENTER));
        
        BuildVList scoresT(this, Rect(vminfo->width/2-100/2-20,vmargin+5*25+4*vspacing+16,100,25), 2);
        scoresT.add(new Label(N_("Scores"), HALIGN_CENTER));
        BuildVList scoresST(this, Rect(vminfo->width/2-100-20,vmargin+6*25+5*vspacing+16,100,25), 2);
        scoresST.add(new Label(N_("You: "), HALIGN_RIGHT));
        scoresST.add(new Label(N_("Best: "), HALIGN_RIGHT));
        scoresST.add(new Label(N_("PAR: "), HALIGN_RIGHT));
        scoresST.add(new Label(N_("Author: "), HALIGN_RIGHT));
        scoresST.add(new Label(N_("# Players: "), HALIGN_RIGHT));
    
        BuildVList scores(this, Rect(vminfo->width/2-15,vmargin+6*25+5*vspacing+16,95,25), 2);
        scores.add(new Label("", HALIGN_CENTER));
        scores.add(new Label(scoreToString(theRatingMgr->getBestScoreEasy(aLevel),
            theRatingMgr->getBestScoreDifficult(aLevel),aLevel).c_str(), HALIGN_CENTER));
        
        BuildVList versionT(this, Rect(vminfo->width-100/2-90-3*hmargin,vmargin+5*25+4*vspacing+16,100,25), 2);
        versionT.add(new Label(N_("Version"), HALIGN_CENTER));
        BuildVList versionST(this, Rect(vminfo->width-110-90-3*hmargin,vmargin+6*25+5*vspacing+16,110,25), 2);
        versionST.add(new Label(N_("Score: "), HALIGN_RIGHT));
        versionST.add(new Label(N_("Release: "), HALIGN_RIGHT));
        versionST.add(new Label(N_("Revision: "), HALIGN_RIGHT));
        versionST.add(new Label(N_("Easy/Diff: "), HALIGN_RIGHT));
        versionST.add(new Label(N_("Control: "), HALIGN_RIGHT));
        
        BuildVList ratingPubT1(this, Rect(hmargin,vmargin+11*25+9*vspacing+2*16,110,25), 2);
        ratingPubT1.add(new Label(N_("Credit: "), HALIGN_RIGHT));
        ratingPubT1.add(new Label(N_("Dedication: "), HALIGN_RIGHT));
        ratingPubT1.add(new Label(N_("Annotation: "), HALIGN_RIGHT));
        ratingPubT1.add(new Label(N_("Rating: "), HALIGN_RIGHT));
    
        BuildVList comments(this, Rect(hmargin+110+10,vmargin+11*25+9*vspacing+2*16, vminfo->width-3*hmargin-110-10,25), 2);
        comments.add(new Label(levelProxy->getCredit(true), HALIGN_LEFT));
        comments.add(new Label(levelProxy->getDedication(true), HALIGN_LEFT));
    //comments.add(new TextField());
    
        add(annotation, Rect(hmargin+110+10, vmargin+13*25+11*vspacing+2*16, vminfo->width-3*hmargin-110-10, 25));
        
    }
    
    LevelInspector::~LevelInspector () {
    }
    
    bool LevelInspector::on_event (const SDL_Event &e) {
        return false;
    }
    
    void LevelInspector::on_action(gui::Widget *w) {
        if (w == back)
            Menu::quit();
    }
    
    void LevelInspector::draw_background(ecl::GC &gc) {
        const video::VMInfo *vminfo = video::GetInfo();
        video::SetCaption (("Enigma - Level Inspector"));
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
        blit(gc, vminfo->width-vminfo->thumbw-10-hmargin, vmargin, previewImage);
    }
    
    void LevelInspector::tick(double dtime) {
    }

    std::string LevelInspector::ratingToString(int value) {
        if (value == 0) {
            // no rating available
            return "";
        } else {
            return ecl::strf("%1d", value);
        }
    }
    
    std::string LevelInspector::scoreToString(int easy, int difficult, lev::Proxy *aLevel) {
        if (easy >= 0 && difficult >=0)
            return ecl::strf( "%2d:%02d / %2d:%02d", easy/60, easy%60, difficult/60,
                    difficult%60);
        else if (easy < 0 && difficult >= 0)
            return ecl::strf( "  -   / %2d:%02d", difficult/60, difficult%60);
        else if (easy >= 0 && difficult < 0)
            return ecl::strf( "%2d:%02d /   -  ", easy/60, easy%60);
        else
            return "  -   /   -  ";
    }

}} // namespace enigma::lev
