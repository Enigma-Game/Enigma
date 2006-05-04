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

#include "gui/LevelPackMenu.hh"

#include "ecl.hh"
#include "levels.hh"
#include "main.hh"
#include "nls.hh"
#include "options.hh"
#include "server.hh"
#include "sound.hh"
#include "video.hh"

using namespace std;
using namespace ecl;

using enigma::levels::LevelStatus;
using enigma::levels::LevelInfo;
using enigma::levels::LevelPack;
using enigma::levels::LevelPacks;

namespace enigma { namespace gui {
/* -------------------- Level Menu -------------------- */
    
    struct LevelMenuConfig {
        int buttonw, buttonh;
        int lbuttonw, lbuttonh;
        ecl::Rect previewarea;
        int thumbsy;                // y coordinate of thumbnail window
        int leftborder;
    
        LevelMenuConfig (const ecl::Rect &screen)
        : buttonw (140), buttonh (35),
          lbuttonw (140), lbuttonh (100),
          previewarea (10, 60, screen.w-50, screen.h-130),
          thumbsy (60),
          leftborder (10)
        {}
    };
    
    LevelMenu::LevelMenu(LevelPack *lp, unsigned long pos)
    : but_unsolved   (new ImageButton("ic-unsolved", "ic-unsolved1", this)), 
    //  but_tournament (new TournamentButton),
      but_back       (new StaticTextButton(N_("Back"), this)),
      but_difficulty (new DifficultyButton),
      but_levelpack  (new StaticTextButton(N_("Level Pack"), this)),
      lbl_lpinfo     (new Label("")),
      lbl_statistics (new Label("")),
      lbl_levelname  (new Label("", HALIGN_LEFT)),
      lbl_levelinfo  (new Label("", HALIGN_LEFT)),
      level_pack  (lp),
      m_ilevelpack (),
      shown_text_ttl(-1.0)
    {
        HList *hl;
    
        const video::VMInfo &vminfo = *video::GetInfo();
    
        // Set size of preview images
        preview_cache.set_size(vminfo.thumbw, vminfo.thumbh);
    
        // Levelmenu configuration
        const int Y2 = 10;          // y position for information area
        const int Y3 = vminfo.height-50; // y position for bottom button row
        LevelMenuConfig c (Rect (0, 0, vminfo.width, vminfo.height));
    
        but_difficulty->set_listener (this);
    
    
        // Create buttons
        hl = new HList;
        hl->set_spacing (10);
        hl->set_alignment (HALIGN_CENTER, VALIGN_TOP);
        hl->set_default_size (c.buttonw, c.buttonh);
        hl->add_back (but_levelpack);
        hl->add_back (but_unsolved);
        hl->add_back (but_difficulty);
        hl->add_back (but_back);
        this->add (hl, Rect(c.leftborder, Y3, vminfo.width-10, c.buttonh));
        
    //     BuildHList hlist1(this, Rect(c.leftborder, Y3, c.buttonw, c.buttonh), 10);
    //     hlist1.add (but_levelpack);
    // //    hlist1.add (but_tournament);
    //     hlist1.add (but_unsolved);
    //     hlist1.add (but_difficulty);
    //     but_difficulty->set_listener(this);
    // //        hlist1.add (newgame);
    //     hlist1.add (but_back);
    
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
        levelwidget = new LevelWidget(lp, preview_cache);
        levelwidget->set_listener(this);
        levelwidget->realize (c.previewarea);
        levelwidget->set_area (c.previewarea);
    
        this->add (levelwidget);
    
        set_position(pos);
    }
    
    void LevelMenu::tick(double dtime) 
    {
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
    
        // automatically create thumbnails in the background
    //     if (int idx = levelwidget->missing_thumbnail()) {
    
    //         invalidate_all();
    //     }
    
    }
    
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
                default: handled=false; break;
                }
            }
    // right mouse button used with priority for level inspector
    //         else if (e.type == SDL_MOUSEBUTTONDOWN
    //                  && e.button.button == SDL_BUTTON_RIGHT)
    //         {
    //             Menu::quit();
    //             handled=true;
    //         }
            else
                handled = Menu::on_event (e);
        }
        return handled;
    }
    
    void LevelMenu::on_action(Widget *w) 
    {
        if (w==levelwidget) {
            int ilevel = levelwidget->selected_level();
            LevelPack *lp = LevelPacks[m_ilevelpack];
    
            if ((unsigned)ilevel < lp->size()) {
                Level level (lp, ilevel);
                if (!LevelIsLocked (level)) {
                    game::StartGame(lp, ilevel);
                    if (levels::IsHistory(lp))
                        // the played level is now the first in history
                        ilevel = 0;
                    else
                        ilevel = server::CurrentLevel;
                    if (lp != server::CurrentLevelPack) {
                        set_levelpack(IndexOfLevelPack(server::CurrentLevelPack));
                    }
                    invalidate_all();
                    levelwidget->set_current(ilevel);
                }
                else
                    show_text(_("You are not allowed to play this level yet."));
            }
        } else if (w == but_back) {
            Menu::quit();
        } else if (w == pgup) {
            levelwidget->page_up();
        } else if (w == pgdown) {
            levelwidget->page_down();
        } else if (w == start) {
            levelwidget->start();
        } else if (w == end) {
            levelwidget->end();
        } else if (w == but_unsolved) {
            levelwidget->next_unsolved();
        } else if (w == but_levelpack) {
            LevelPackMenu lpm;
            lpm.center();
            if (lpm.manage()) {
                set_levelpack(lpm.get_selection());
            }
            invalidate_all();
        } else if (w == but_difficulty) {
            but_difficulty->on_action(w);
            invalidate_all();
        }
    }
    
    void LevelMenu::update_info()
    {
        char txt[200];
        LevelPack *lp  = LevelPacks[m_ilevelpack];
        Level level (lp, levelwidget->selected_level());
        int difficulty = options::GetDifficulty();
    
        if (lp->size() == 0) {
            // empty level pack
            lbl_statistics->set_text ("-");
            lbl_levelname->set_text ("-");
            lbl_levelinfo->set_text ("-");
        }
        else {
            int iselected = levelwidget->selected_level();
    
            // Display levelpack statistics (percentage of solved levels)
            unsigned numparhold;
            int      numsolved = CountSolvedLevels (lp, difficulty, &numparhold);
    
            if (options::GetBool("TimeHunting")) {
                int pct = 100*numparhold / lp->size();
                snprintf (txt, sizeof(txt), _("%d%% par"), pct);
            }
            else {
                int pct = 100*numsolved / int (lp->size());
                snprintf (txt, sizeof(txt), _("%d%% solved"), pct);
            }
            lbl_statistics->set_text(txt);
    
            const LevelInfo &li = level.get_info();
    
            // Display level name
            if (enigma::WizardMode) {
                snprintf (txt, sizeof(txt), "#%d: %s (%s)",
                          iselected+1, li.name.c_str(), li.filename.c_str());
            } else {
                snprintf (txt, sizeof(txt), "#%d: %s",
                          iselected+1, li.name.c_str());
            }
            lbl_levelname->set_text(txt);
    
            // Display best time
            if (shown_text.length()) {
                lbl_levelinfo->set_text(shown_text);
            }
            else {
                int    par_time       = level.get_par_time (difficulty);
                int    best_user_time = level.get_best_user_time (difficulty);
                string par_name       = level.get_par_holder (difficulty);
    
                string your_time;
                string par_text;
    
                if (best_user_time>0) {
                    your_time = strf(_("Your time: %d:%02d"),
                                     best_user_time/60, best_user_time%60);
    
                    if (par_time>0) {
                        int below = par_time-best_user_time;
                        if (below == 0)
                            par_text = _("That's par.");
                        else if (below>0)
                            par_text = strf(_("That's %d:%02d below par."),
                                            below/60, below%60);
                    }
                }
    
                if (par_text.length() == 0 && par_time>0) {
                    if (par_name.length())
                        par_text = strf(_("Par by %s: %d:%02d"), 
                                        par_name.c_str(), par_time/60, par_time%60);
                    else
                        par_text = strf(_("Par: %d:%02d"), par_time/60, par_time%60);
                }
    
                string time_text;
                if (your_time.length()>0)   time_text = your_time+"  "+par_text;
                else                        time_text = your_time+par_text;
    
                lbl_levelinfo->set_text(time_text.c_str());
            }
        }
    }
    
    void LevelMenu::set_levelpack (unsigned index) 
    {
        if (index < LevelPacks.size()) {
            m_ilevelpack = index;
    
            LevelPack *lp = LevelPacks[m_ilevelpack];
            levelwidget->change_levelpack (lp);
    
            char txt[100];
            snprintf (txt, sizeof(txt), _("%s: %d levels"),
                      lp->get_name().c_str(), lp->size());
            lbl_lpinfo->set_text(txt);
    
            update_info();
        }
    }
    
    void LevelMenu::draw_background(ecl::GC &gc) 
    {
        video::SetCaption(("Enigma - Level Menu"));
        sound::PlayMusic (options::GetString("MenuMusicFile"));
    
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
    }
    
    void LevelMenu::next_levelpack() 
    {
        unsigned next_pack = m_ilevelpack+1;
        if (next_pack == LevelPacks.size()) next_pack = 0;
        set_levelpack(next_pack);
    }
    
    void LevelMenu::previous_levelpack() {
        size_t prev_pack = m_ilevelpack;
        if (prev_pack == 0) prev_pack = LevelPacks.size()-1;
        else --prev_pack;
        set_levelpack(prev_pack);
    }
    
    int LevelMenu::get_position() const {
        return (m_ilevelpack << 16) | (levelwidget->get_position() & 0xffff);
    }
    
    void LevelMenu::set_position(int pos) {
        set_levelpack(pos >> 16);
        levelwidget->set_position(pos & 0xffff);
    }
    
    void LevelMenu::show_text(const string& text) {
        shown_text     = text;
        shown_text_ttl = 2.0; // show for two seconds
    }

    /* -------------------- DifficultyButton -------------------- */
    
    DifficultyButton::DifficultyButton() : TextButton(this) {
    }
    
    string DifficultyButton::get_text() const 
    {
        if (options::GetDifficulty() == DIFFICULTY_EASY)
            return _("Difficulty: Easy");
        else
            return _("Difficulty: Normal");
    }
    
    void DifficultyButton::on_action(Widget *) 
    {
        int newdifficulty = (DIFFICULTY_EASY+DIFFICULTY_HARD) - options::GetDifficulty();
        options::SetOption("Difficulty", newdifficulty);
        options::MustRestartLevel = true;
        invalidate();
    }
    
}} // namespace enigma::gui
