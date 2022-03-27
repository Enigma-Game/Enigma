/*
 * Copyright (C) 2002,2003,2004,2005,2006 Daniel Heck
 * Copyright (C) 2006,2007,2008,2009,2010 Ronald Lamprecht
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
 
#include "gui/LevelMenu.hh"

#include "gui/HelpMenu.hh"
#include "gui/LevelPackMenu.hh"
#include "gui/LevelPackComposer.hh"

#include "game.hh"
#include "main.hh"
#include "nls.hh"
#include "options.hh"
#include "resource_cache.hh"
#include "server.hh"
#include "MusicManager.hh"
#include "StateManager.hh"
#include "lev/PersistentIndex.hh"
#include "lev/Index.hh"

using namespace std;
using namespace ecl;


namespace enigma { namespace gui {
/* -------------------- Level Menu -------------------- */
    
    LevelMenu::LevelMenu()
    : but_advancemode (new AdvanceModeButton),
      but_next       (new ImageButton("ic-next", "ic-next1", this)),
      but_back       (new StaticTextButton(N_("Main Menu"), this)),
      but_difficulty (new DifficultyButton),
      but_levelpack  (new StaticTextButton(N_("Level Pack"), this)),
      lbl_lpinfo     (new Label("")),
      lbl_statistics (new Label("")),
      lbl_levelname  (new Label("", HALIGN_LEFT)),
      lbl_levelinfo  (new Label("", HALIGN_LEFT)),
      shown_text_ttl(-1.0), main_quit (false)
    {
        static struct SpacingConfig {
            int hmargin_info, vmargin, vgap_info, vsize_info;
            int hmargin_prev, hgap_prev_nav, vgap_info_prev, vgap_prev_but;
            int hsize_nav, hmargin_nav, vsize_nav, vgap_info_nav, vgap_nav_nav, vgap_nav1_nav2;
            int hsize_button_small, hsize_button_large, hgap_button, vsize_button, vmargin_bottom;
        } param[] = {
            {  // VTS_16 (320x240)
                2, 5, 10, 14,
                5, 5, 5, 1,
                10, 5, 25, 10, 5, 45,
                45, 70, 5, 17, 7
            },
            {  // VTS_32 (640x480)
                5, 10, 20, 28,
                10, 10, 10, 2,
                20, 10, 50, 20, 10, 90,
                90, 140, 10, 35, 15
            },
            {  // VTS_40 (800x600)
                5, 10, 20, 28,
                10, 10, 10, 0,
                20, 10, 50, 20, 10, 120,
                90, 140, 10, 35, 15
            },
            {  // VTS_48 (960x720)  VM_1024x768
                5, 10, 20, 28,
                10, 10, 10, 0,
                20, 10, 50, 20, 10, 120,
                90, 140, 10, 35, 15
            },
            {  // VTS_64 (1280x960)
                15, 10, 25, 28,
                15, 25, 15, 0,
                20, 15, 50, 40, 10, 120,
                90, 140, 10, 35, 15
            }
        };

        const VMInfo &vminfo = *video_engine->GetInfo();
        const int vshrink = vminfo.width < 640 ? 1 : 0;
        VideoTileType vtt = vminfo.tt;

        int preview_y = param[vtt].vmargin + 2*param[vtt].vgap_info + param[vtt].vgap_info_prev;
        ecl::Rect previewarea(param[vtt].hmargin_prev, preview_y,
                vminfo.width - param[vtt].hmargin_nav - param[vtt].hsize_nav - param[vtt].hgap_prev_nav, 
                vminfo.height - preview_y - param[vtt].vgap_prev_but - param[vtt].vsize_button - param[vtt].vmargin_bottom);
        
        HList *hl, *hll, *hlr ;
    
        but_difficulty->set_listener (this);
    
        // Create buttons
        hll = new HList;
        hll->set_spacing (param[vtt].hgap_button);
        hll->set_alignment (HALIGN_CENTER, VALIGN_TOP);
        hll->set_default_size (param[vtt].hsize_button_small, param[vtt].vsize_button);
        hll->add_back (but_advancemode);
        hll->add_back (but_next);
        hll->add_back (but_difficulty);
        
        hlr = new HList;
        hlr->set_spacing (param[vtt].hgap_button);
        hlr->set_alignment (HALIGN_CENTER, VALIGN_TOP);
        hlr->set_default_size (param[vtt].hsize_button_large, param[vtt].vsize_button);
        hlr->add_back (but_levelpack);
        hlr->add_back (but_back);
        
        hl = new HList;
        hl->set_spacing (vshrink?5:10);
        hl->set_alignment (HALIGN_CENTER, VALIGN_TOP);
        hl->set_default_size (2*param[vtt].hsize_button_large + param[vtt].hgap_button, param[vtt].vsize_button);
        hl->add_back (hll);
        hl->add_back (hlr);
        this->add (hl, Rect(0, vminfo.height - param[vtt].vsize_button - param[vtt].vmargin_bottom,
                vminfo.width, param[vtt].vsize_button));
            
        // Add navigation buttons
        pgup     = new ImageButton("ic-up", "ic-up1", this);
        pgdown   = new ImageButton("ic-down", "ic-down1", this);
        start    = new ImageButton("ic-top", "ic-top1", this);
        end      = new ImageButton("ic-bottom", "ic-bottom1", this);
    
        Rect r(vminfo.width - param[vtt].hmargin_nav - param[vtt].hsize_nav,
                param[vtt].vmargin + 2*param[vtt].vgap_info + param[vtt].vgap_info_nav,
                param[vtt].hsize_nav, param[vtt].vsize_nav);
        add (pgup, r);
        r.y += param[vtt].vsize_nav + param[vtt].vgap_nav_nav;
        add (pgdown, r);
        r.y += param[vtt].vsize_nav + param[vtt].vgap_nav1_nav2;
        add (start, r);
        r.y += param[vtt].vsize_nav + param[vtt].vgap_nav_nav;
        add (end, r);
    
        // Information area
        hl = new HList;
        hl->add_back (lbl_levelname, List::EXPAND);
        hl->add_back (lbl_lpinfo, List::TIGHT);
        this->add (hl, Rect (param[vtt].hmargin_info, param[vtt].vmargin, 
                vminfo.width - 2*param[vtt].hmargin_info, param[vtt].vsize_info));
    
        hl_info_stat = new HList;
        hl_info_stat->add_back (lbl_levelinfo, List::EXPAND); //Rect (c.leftborder, Y2+20,305, 28));
        hl_info_stat->add_back (lbl_statistics, List::TIGHT);
        this->add (hl_info_stat, Rect (param[vtt].hmargin_info, param[vtt].vmargin + param[vtt].vgap_info, 
                vminfo.width - 2*param[vtt].hmargin_info, param[vtt].vsize_info));
    
        // Prepare level selection widget
        levelwidget = new LevelWidget();
        levelwidget->set_listener(this);
        levelwidget->realize (previewarea);
        levelwidget->set_area (previewarea);
    
        this->add (levelwidget);
        
        updateIndex();
    }
    
    void LevelMenu::tick(double dtime) 
    {
        levelwidget->tick(dtime);
        static double timeaccu = 0.0;
    
        // info texts disappear after some time
        if (shown_text_ttl>0.0) {
            shown_text_ttl -= dtime;
            if (shown_text_ttl <= 0.0)
                shown_text = "";
        }
        timeaccu += dtime;
        if (timeaccu > 0.1) {
            update_info();
            timeaccu = 0.0;
        }
    
    }
    
    static const char *helptext_levelmenu[] = {
        N_("Escape:"),              N_("Skip to main menu"),
        "F1:",                      N_("Show this help"),
        "F5:",                      0, // see below
        "F6:",                      N_("Add all to clipboard"),
        "F7:",                      N_("Update levelpack"),
        N_("Arrows:"),              N_("Select level"),
        N_("Return:"),              N_("Play selected level"),
        N_("Back/Space:"),          N_("Previous/next levelpack"),
        "u",                        N_("Mark current level as unsolved"),
    //    "s",                        N_("Mark current level as Solved"),
        N_("Alt+Return:"),          N_("Switch between fullscreen and window"),
        N_("Left click:"),               N_("Play selected level"),
        N_("Right or control click:"),   N_("Inspect selected level"),
        0
    };
    
    bool LevelMenu::on_event (const SDL_Event &e) 
    {
        // Pass all events to the level widget first
        bool handled=levelwidget->on_event(e);
    
        if (!handled) {
            if (e.type == SDL_KEYDOWN) {
                handled=true;
                switch (e.key.keysym.sym) {
                case SDLK_SPACE: next_levelpack(); break;
                case SDLK_BACKSPACE: previous_levelpack(); break;
                case SDLK_F1:
                    if (app.state->getInt("NextLevelMode") == lev::NEXT_LEVEL_NOT_BEST)
                        helptext_levelmenu[5] = N_("Select next level for world record hunt");
                    else
                        helptext_levelmenu[5] = N_("Select next unsolved level");
            
                    displayHelp(helptext_levelmenu, 200);
                    draw_all();
                    break;
                case SDLK_F5:
                    next_unsolved();
                    break;
                case SDLK_F6:
                    /*if (lev::Index::getCurrentIndex() != NULL) {
                        lev::Variation var;
                        for (int pos = 0; pos < lev::Index::getCurrentIndex()->size(); pos++) {
                            var = lev::Index::getCurrentIndex()->getVariation(pos);
                            clipboard->appendProxy(lev::Index::getCurrentIndex()->getProxy(pos),
                                    var.ctrl, var.unit, var.target, var.extensions);
                        }
                    }*/
                    gui::LevelPackComposer::addAllFromIndexToClipboard(lev::Index::getCurrentIndex());
                    break;
                case SDLK_F7:
                    lev::Index::getCurrentIndex()->updateFromFolder();
                    lev::Index::getCurrentIndex()->updateFromProxies();
                    draw_all();
                    break;
                case SDLK_u: {
                    lev::ScoreManager::instance()->markUnsolved(lev::Index::getCurrentProxy(), 
                            app.state->getInt("Difficulty"));
                    invalidate_all(); 
                    break;    
                }      
                case SDLK_s:
                    lev::ScoreManager::instance()->markSolved(lev::Index::getCurrentProxy(), 
                            app.state->getInt("Difficulty"));
                    invalidate_all();
                    break;
                default: handled=false; break;
                }
            }
            else
                handled = Menu::on_event (e);
        }
        return handled;
    }
    
    void LevelMenu::on_action(Widget *w) 
    {
        if (w==levelwidget) {
            lev::Index *ind = lev::Index::getCurrentIndex();
            int ilevel = ind->getCurrentPosition();
            if (w->lastModifierKeys() & KMOD_CTRL && w->lastModifierKeys() & KMOD_SHIFT) {
                // force a reload from file
                lev::Proxy::releaseCache();
            }
    
            if (ilevel < (int)ind->size()) {
                if (ind->mayPlayLevel(ilevel+1)) {
                    game::StartGame();
                    ilevel = ind->getCurrentPosition();
                    invalidate_all();
                    ind->setCurrentPosition(ilevel);
                    levelwidget->syncFromIndexMgr();
                }
                else
                    show_text(_("You are not allowed to play this level yet."));
            }
        } else if (w == but_back) {
            main_quit = true;
            Menu::quit();
        } else if (w == pgup) {
            levelwidget->page_up();
        } else if (w == pgdown) {
            levelwidget->page_down();
        } else if (w == start) {
            levelwidget->start();
        } else if (w == end) {
            levelwidget->end();
        } else if (w == but_next) {
            next_unsolved(); 
            // if we jump to first level the view is not updated to levelwidget->start()
        } else if (w == but_levelpack) {
            main_quit = false;
            Menu::quit();
        } else if (w == but_difficulty) {
            but_difficulty->on_action(w);
            invalidate_all();
        }
    }
    
    void LevelMenu::update_info() {
        // Note: all format strings have to be translated directly
        // as the formatted strings can no longer be translated.
        // The instant language change is guaranteed by the frequent
        // call of is method!
        
        lev::Index *ind = lev::Index::getCurrentIndex();
        int size = ind->size();
        lev::ScoreManager *scm = lev::ScoreManager::instance();
        lev::Proxy *curProxy = ind->getCurrent();
        int difficulty = app.state->getInt("Difficulty");

        lbl_lpinfo->set_text(ecl::strf(nls::ntranslate("%s: %d level", "%s: %d levels", size).c_str(),
                ind->getName().c_str(), size));
    
        if (size == 0) {
            // empty level pack
            lbl_statistics->set_text ("-");
            lbl_levelname->set_text ("-");
            lbl_levelinfo->set_text ("-");
        }
        else {
            // Display levelpack statistics (percentage of solved levels)
    
            if (app.state->getInt("NextLevelMode") == lev::NEXT_LEVEL_NOT_BEST) {
                int pct = 100* scm->countBestScore(ind, difficulty)/ size;
                lbl_statistics->set_text(ecl::strf(_("%d%% best"), pct));
            }
            else if (app.state->getInt("NextLevelMode") == lev::NEXT_LEVEL_OVER_PAR) {
                int pct = 100* scm->countParScore(ind, difficulty)/ size;
                double hcp = scm->calcHCP(ind, difficulty);
                lbl_statistics->set_text(ecl::strf(_("%d%% par, hcp %.1f"), pct, hcp));
            }
            else {
                int pct = 100* scm->countSolved(ind, difficulty) / size;
                lbl_statistics->set_text(ecl::strf(_("%d%% solved"), pct));
            }
    
            // Display level name
            if (enigma::WizardMode) {
                // add level path info - we just can display the normalized path
                // as we did not yet locate the absolute path - the user can
                // use the inspector to check the absolute path!
                lbl_levelname->set_text(ecl::strf("#%d: %s (%s)",
                          ind->getCurrentLevel(), curProxy->getTitle().c_str(), 
                          curProxy->getNormFilePath().c_str()));
            } else {
                lbl_levelname->set_text(ecl::strf("#%d: %s",
                          ind->getCurrentLevel(), curProxy->getTitle().c_str()));
            }
    
            // Display best time
            if (shown_text.length()) {
                lbl_levelinfo->set_text(shown_text);
            }
            else {
                // TODO prepare for scores that are not time based!
                // char txt[200];
                lev::RatingManager *ratingMgr = lev::RatingManager::instance();
                int wr_time    = ratingMgr->getBestScore(curProxy, difficulty);
                int par_time   = ratingMgr->getParScore(curProxy, difficulty);
                bool is_par    = scm->parScoreReached(curProxy, difficulty);
                int best_user_time = scm->getBestUserScore(curProxy, difficulty);
                string wr_name = ratingMgr->getBestScoreHolder(curProxy, difficulty);
                bool  wr_name_displayed = false;
    
                std::string your_time;
                std::string wr_text;
    
                if (best_user_time>0) {
                    your_time = _("Your time: ") + ecl::timeformat(best_user_time);
    
                    if (wr_time>0) {
                        int below = wr_time - best_user_time;
                        if (below == 0)
                            wr_text = _("That's world record.");
                        else if (below>0)
                            wr_text = ecl::strf(_("That's %s below world record."), ecl::timeformat(below).c_str());
                    }
                }
    
                if (wr_text.length() == 0 && wr_time>0) {
                    if (wr_name.length()) {
                        wr_name_displayed = true;
                    } else
                        if (is_par || par_time < 0)
                            wr_text = _("World record: ") + ecl::timeformat(wr_time);
                        else
                            wr_text = _("Par: ") + ecl::timeformat(par_time) + " " +  
                                    _("World record: ") + ecl::timeformat(wr_time);
                }
    
                if (!your_time.empty())
                    your_time += "  ";

                int wr_cut = 0;
                do {
                    if (wr_name_displayed) {
                        std::string tmp = ratingMgr->getBestScoreHolder(curProxy, difficulty, wr_cut++);
                        if (!tmp.empty())
                            wr_name = tmp;
                        if (is_par || par_time < 0)
                            wr_text = ecl::strf(_("World record by %s: "), wr_name.c_str()) +
                                    ecl::timeformat(wr_time);
                        else
                            wr_text = _("Par: ") + ecl::timeformat(par_time) + " " +
                                    ecl::strf(_("World record by %s: "), wr_name.c_str()) + 
                                    ecl::timeformat(wr_time);
                    }
                    lbl_levelinfo->set_text(your_time + wr_text);
                } while (!hl_info_stat->fits() && wr_name_displayed && (wr_cut < 20));
            }
        }
    }
    
    void LevelMenu::updateIndex() 
    {
        levelwidget->syncFromIndexMgr();
        
        update_info();
    }
    
    void LevelMenu::draw_background(ecl::GC &gc) 
    {
        const VMInfo *vminfo = video_engine->GetInfo();

        set_caption(_("Enigma - Level Menu"));
        sound::StartMenuMusic();
    
        blit(gc, vminfo->mbg_offsetx, vminfo->mbg_offsety, enigma::GetImage("menu_bg", ".jpg"));
    }

    void LevelMenu::next_unsolved() 
    {
        lev::Index *ind = lev::Index::getCurrentIndex();
        if (ind->advanceLevel(lev::ADVANCE_NEXT_MODE)) {
            levelwidget->syncFromIndexMgr();
        } else
            show_text(_("No further unsolved level available!"));
    }
    
    void LevelMenu::next_levelpack() 
    {
        lev::Index::setCurrentIndex(lev::Index::nextGroupIndex()->getName());
        updateIndex();
    }
    
    void LevelMenu::previous_levelpack() {
        lev::Index::setCurrentIndex(lev::Index::previousGroupIndex()->getName());
        updateIndex();
    }
        
    void LevelMenu::show_text(const string& text) {
        shown_text     = text;
        shown_text_ttl = 2.0; // show for two seconds
    }

    bool LevelMenu::isMainQuit() {
        return main_quit;
    }

    /* -------------------- DifficultyButton -------------------- */
    
    DifficultyButton::DifficultyButton() : ImageButton("ic-easymode","ic-easymode",this) {
        update();
    }
    
    void DifficultyButton::update() {
        if (app.state->getInt("Difficulty") == DIFFICULTY_EASY)
            ImageButton::set_images("ic-easymode","ic-normalmode");
        else
            ImageButton::set_images("ic-normalmode","ic-easymode");
    }
    
    void DifficultyButton::on_action(Widget *) 
    {
        int newdifficulty = (DIFFICULTY_EASY+DIFFICULTY_HARD) - app.state->getInt("Difficulty");
        app.state->setProperty("Difficulty", newdifficulty);        update();
        invalidate();
    }

    void DifficultyButton::draw(ecl::GC &gc, const ecl::Rect &r) {
        update();
        ImageButton::draw(gc, r);
    }
    
    /* -------------------- AdvanceModeButton -------------------- */
    
    AdvanceModeButton::AdvanceModeButton() : ImageButton("","",this) {
        update();
    }
        
    void AdvanceModeButton::update() {
        switch (app.state->getInt("NextLevelMode")) {
            case lev::NEXT_LEVEL_UNSOLVED :
                ImageButton::set_images("ic-unsolved", "par");
                break;
            case lev::NEXT_LEVEL_OVER_PAR :
                ImageButton::set_images("par", "ic-worldrecord");
                break;
            case lev::NEXT_LEVEL_NOT_BEST :
                ImageButton::set_images("ic-worldrecord", "ic-strictlynext");
                break;
            case lev::NEXT_LEVEL_STRICTLY : // use as default, too
            default:
                ImageButton::set_images("ic-strictlynext","ic-unsolved");
                break;        
        }
    }
    
    void AdvanceModeButton::on_action(Widget *) {
        switch (app.state->getInt("NextLevelMode")) {
            case lev::NEXT_LEVEL_STRICTLY :
                app.state->setProperty("NextLevelMode", lev::NEXT_LEVEL_UNSOLVED);
                break;        
            case lev::NEXT_LEVEL_UNSOLVED :
                app.state->setProperty("NextLevelMode", lev::NEXT_LEVEL_OVER_PAR);
                break;
            case lev::NEXT_LEVEL_OVER_PAR :
                app.state->setProperty("NextLevelMode", lev::NEXT_LEVEL_NOT_BEST);
                break;
            case lev::NEXT_LEVEL_NOT_BEST :
            default:
                app.state->setProperty("NextLevelMode", lev::NEXT_LEVEL_STRICTLY);
        }
        update();
        invalidate();
    }
    
    void AdvanceModeButton::draw(ecl::GC &gc, const ecl::Rect &r) {
        update();
        ImageButton::draw(gc, r);
    }
    
}} // namespace enigma::gui
