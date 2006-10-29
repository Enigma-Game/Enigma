/*
 * Copyright (C) 2002,2003,2004,2005,2006 Daniel Heck
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

#include "ecl.hh"
#include "game.hh"
#include "main.hh"
#include "nls.hh"
#include "options.hh"
#include "server.hh"
#include "sound.hh"
#include "StateManager.hh"
#include "video.hh"
#include "lev/Index.hh"

using namespace std;
using namespace ecl;


namespace enigma { namespace gui {
/* -------------------- Level Menu -------------------- */
    
    struct LevelMenuConfig {
        int buttonw, ibuttonw, buttonh;
        int lbuttonw, lbuttonh;
        ecl::Rect previewarea;
        int thumbsy;                // y coordinate of thumbnail window
        int leftborder;
    
        LevelMenuConfig (const ecl::Rect &screen)
        : buttonw (140), ibuttonw (90), buttonh (35),
          lbuttonw (140), lbuttonh (100),
          previewarea (10, 60, screen.w-50, screen.h-130),
          thumbsy (60),
          leftborder (10)
        {}
    };
    
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
      shown_text_ttl(-1.0)
    {
        HList *hl, *hll, *hlr ;
    
        const video::VMInfo &vminfo = *video::GetInfo();
    
    
        // Levelmenu configuration
        const int Y2 = 10;          // y position for information area
        const int Y3 = vminfo.height-50; // y position for bottom button row
        LevelMenuConfig c (Rect (0, 0, vminfo.width, vminfo.height));
    
        but_difficulty->set_listener (this);
    
    
        // Create buttons
        hll = new HList;
        hll->set_spacing (10);
        hll->set_alignment (HALIGN_CENTER, VALIGN_TOP);
        hll->set_default_size (c.ibuttonw, c.buttonh);
        hll->add_back (but_advancemode);
        hll->add_back (but_next);
        hll->add_back (but_difficulty);
        
        hlr = new HList;
        hlr->set_spacing (10);
        hlr->set_alignment (HALIGN_CENTER, VALIGN_TOP);
        hlr->set_default_size (c.buttonw, c.buttonh);
        hlr->add_back (but_levelpack);
        hlr->add_back (but_back);
        
        hl = new HList;
        hl->set_spacing (10);
        hl->set_alignment (HALIGN_CENTER, VALIGN_TOP);
        hl->set_default_size (2*c.buttonw + 10, c.buttonh);
        hl->add_back (hll);
        hl->add_back (hlr);
        this->add (hl, Rect(c.leftborder, Y3, vminfo.width-20, c.buttonh));
            
        // Add navigation buttons
        pgup     = new ImageButton("ic-up", "ic-up1", this);
        pgdown   = new ImageButton("ic-down", "ic-down1", this);
        start    = new ImageButton("ic-top", "ic-top1", this);
        end      = new ImageButton("ic-bottom", "ic-bottom1", this);
    
        Rect r(vminfo.width-30, c.thumbsy, 20, 50);
        r.y = c.thumbsy;
        add (pgup, r);
        r.y += 60;
        add (pgdown, r);
        r.y = c.thumbsy + 240;
        add (start, r);
        r.y += 60;
        add (end, r);
    
        // Information area
        hl = new HList;
        hl->add_back (lbl_levelname, List::EXPAND);
        hl->add_back (lbl_lpinfo, List::TIGHT);
        this->add (hl, Rect (5, Y2, vminfo.width - 10, 28));
    
        hl = new HList;
        hl->add_back (lbl_levelinfo, List::EXPAND); //Rect (c.leftborder, Y2+20,305, 28));
        hl->add_back (lbl_statistics, List::TIGHT);
        this->add (hl, Rect (5, Y2+20, vminfo.width - 10, 28));
    
        // Prepare level selection widget
        levelwidget = new LevelWidget();
        levelwidget->set_listener(this);
        levelwidget->realize (c.previewarea);
        levelwidget->set_area (c.previewarea);
    
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
        N_("Arrows:"),              N_("Select level"),
        N_("Return:"),              N_("Play selected level"),
        N_("Back/Space:"),          N_("Previous/next levelpack"),
        "u",                        N_("Mark current level as Unsolved"),
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
                lev::Proxy * curProxy = lev::Proxy::loadedLevel();
                if (curProxy != NULL)
                    curProxy->release();
            }
    
            if ((unsigned)ilevel < ind->size()) {
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
        
        lbl_lpinfo->set_text(ecl::strf(_("%s: %d levels"),
                ind->getName().c_str(), size));
    
        if (size == 0) {
            // empty level pack
            lbl_statistics->set_text ("-");
            lbl_levelname->set_text ("-");
            lbl_levelinfo->set_text ("-");
        }
        else {
            int iselected = ind->getCurrentPosition();
    
            // Display levelpack statistics (percentage of solved levels)
    
            if (app.state->getInt("NextLevelMode") == lev::NEXT_LEVEL_NOT_BEST) {
                int pct = 100* scm->countBestScore(ind, difficulty)/ size;
                lbl_statistics->set_text(ecl::strf(_("%d%% best"), pct));
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
                          curProxy->getNormLevelPath().c_str()));
            } else {
                lbl_levelname->set_text(ecl::strf("#%d: %s",
                          ind->getCurrentLevel(), curProxy->getTitle().c_str()));
            }
    
            // Display best time
            if (shown_text.length()) {
                lbl_levelinfo->set_text(shown_text);
            }
            else {
                // TODO prepare vor scores that are not time based!
                char txt[200];
                lev::RatingManager *ratingMgr = lev::RatingManager::instance();
                int    par_time       = ratingMgr->getBestScore(curProxy, difficulty);
                int    best_user_time = scm->getBestUserScore(curProxy, difficulty);
                string par_name       = ratingMgr->getBestScoreHolder(curProxy, difficulty);
    
                string your_time;
                string par_text;
    
                if (best_user_time>0) {
                    your_time = strf(_("Your time: %d:%02d"),
                                     best_user_time/60, best_user_time%60);
    
                    if (par_time>0) {
                        int below = par_time-best_user_time;
                        if (below == 0)
                            par_text = _("That's world record.");
                        else if (below>0)
                            par_text = strf(_("That's %d:%02d below world record."),
                                            below/60, below%60);
                    }
                }
    
                if (par_text.length() == 0 && par_time>0) {
                    if (par_name.length())
                        par_text = strf(_("World record by %s: %d:%02d"), 
                                        par_name.c_str(), par_time/60, par_time%60);
                    else
                        par_text = strf(_("World record: %d:%02d"), par_time/60, par_time%60);
                }
    
                string time_text;
                if (your_time.length()>0)   time_text = your_time+"  "+par_text;
                else                        time_text = your_time+par_text;
    
                lbl_levelinfo->set_text(time_text.c_str());
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
        video::SetCaption(("Enigma - Level Menu"));
        sound::PlayMusic (options::GetString("MenuMusicFile"));
    
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
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
            ImageButton::set_images("ic-unsolved", "ic-worldrecord");
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
    
    void AdvanceModeButton::on_action(Widget *) 
    {
        switch (app.state->getInt("NextLevelMode")) {
        case lev::NEXT_LEVEL_STRICTLY :
            app.state->setProperty("NextLevelMode", lev::NEXT_LEVEL_UNSOLVED);
            break;        
        case lev::NEXT_LEVEL_UNSOLVED :
            app.state->setProperty("NextLevelMode", lev::NEXT_LEVEL_NOT_BEST);
            break;
        case lev::NEXT_LEVEL_NOT_BEST :
        default:
            app.state->setProperty("NextLevelMode", lev::NEXT_LEVEL_STRICTLY);
        }
        update();
        invalidate();
    }
    
}} // namespace enigma::gui
