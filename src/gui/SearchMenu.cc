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
 *
 */
 
#include "gui/SearchMenu.hh"
#include "ecl_font.hh"
#include "ecl_video.hh"
#include "errors.hh"
#include "lev/Index.hh"
#include "lev/Proxy.hh"
#include "lev/SearchCombination.hh"
#include "nls.hh"
#include "resource_cache.hh"
#include "video.hh"
#include "gui/InfoMenu.hh"

#include "main.hh"

using namespace ecl;
using namespace std;
using namespace enigma::lev;

namespace enigma { namespace gui {

    static const char *helptext_search[] = {
        N_("Searches for levels with given criteria."),
        N_("Use 'Text Search' for a case independent search in levels' titles, author names, file names, and level identification names."),
        N_("Levels are rated in their difficulty in five categories: intelligence, dexterity, patience, knowledge about Enigma, and speed control. Each rating ranges from 1 (easy) to 5 (difficult). Levels with special mechanisms may also be a 6 in knowledge. With the corresponding buttons, you can search for levels with certain difficulty ratings."),
        N_("The five single ratings are combined into an overall difficulty rating, ranging from 1 to 100. You can also search for levels within a given overall difficulty range."),
        N_("'Average rating' is the mean of player's ratings of the levels. You can rate a level on its level inspector page with a rating from 0 (worst) to 10 (best). You can search for levels which average rating is within a specified interval, too."),
        N_("If you choose 'Only levels that are unsolved in easy mode', you filter for all levels which feature an easy mode, and that have not been solved by you in easy mode yet. Accordingly, you can choose 'Only levels that are unsolved in normal mode' to filter for those."),
        0,
        N_("Finally, you can sort the results randomly, by average rating, by overall difficulty, or by their sequence on load time, which mostly is the sequence in which they appear in the level packs (sort by 'None')."),
        N_("The number of levels satisfying all chosen criteria is shown on the line above the search button. Please note that some levels have no ratings, or no average ratings, and are therefore automatically excluded as soon as you choose to search for ratings or average ratings or sort by them."),
        0
    };

    class RatingSearchButton : public ValueButton {
    public:
        RatingSearchButton(ActionListener *al,SearchCombination *sc_, SCValueKey key_,
                SCValueMinMax mm_, short min_, short max_, short divisor_ = 1)
                 : ValueButton(min_, max_, al) {
            sc = sc_;
            key = key_;
            mm = mm_;
            divisor = divisor_;
            init();
        }

        int get_value() const {  return sc->getValue(key, mm);  }

        void set_value(int value) {  sc->setValue(key, mm, value);  }

        string get_text(int value) const {
            if (divisor == 10) {
                std::string s;
                s = ecl::strf("%3.2d", value);
                s.insert(2,1,'.');
                return s;
            }
            if (divisor != 1)
                Log << "Warning: divisor should be 1 or 10 only.\n";
            return strf("%d", value);
        }

    private:
        SearchCombination *sc;
        SCValueKey key;
        SCValueMinMax mm;
        short divisor;
    };

    class UnsolvedSearchButton : public TextButton {
    public:
        UnsolvedSearchButton(ActionListener *al, SearchCombination *sc_, Difficulty diff_)
                : TextButton(this) {
            sc = sc_;
            diff = diff_;
            secondaryListener = al;
        }

        void on_action(Widget *) {
            sc->toggleOnlyUnsolved(diff);
            invalidate();
            if (secondaryListener != NULL)
                secondaryListener->on_action(this);
        }

        string get_text() const {
            return sc->getOnlyUnsolved(diff) ? _("Yes") : _("No");
        }

    private:
        SearchCombination *sc;
        Difficulty diff;
        ActionListener *secondaryListener;
    };

    class SortMethodButton : public ValueButton {
    public:
        SortMethodButton(ActionListener *al, SearchCombination *sc_)
                : ValueButton(SC_SORT_NONE, SC_SORT_LAST-1, this) {
            sc = sc_;
            secondaryListener = al;
        }

        int get_value() const {
            return sc->getSortMethod();
        }

        void set_value(int value) {
            int s = ecl::Clamp(value, (int)SC_SORT_NONE, (int)SC_SORT_LAST-1);
            sc->setSortMethod((SCSortMethod) (s));
        }

        std::string get_text(int value) const {
            switch(value) {
            case SC_SORT_NONE: { return _("None"); break; }
// TRANSLATORS: Would be shown as "Sort by: Random".
            case SC_SORT_RANDOM: { return _("Random"); break; }
// TRANSLATORS: Would be shown as "Sort by: Difficulty".
            case SC_SORT_DIF: { return _("Difficulty"); break; }
// TRANSLATORS: Would be shown as "Sort by: Average rating".
            case SC_SORT_AVR: { return _("Average rating"); break; }
            }
// TRANSLATORS: Would be shown as "Sort by: Unknown [method]".
            Log << "Unknown sort method number " << value << ".\n";
            return _("Unknown");
        }

    private:
        SearchCombination *sc;
    };


    /* -------------------- Search Menu -------------------- */

    SearchMenu::SearchMenu() : didSearch(false) {
        sc = SearchCombination::getSingleton();
        bool sc_is_fresh = sc->isFresh();

        const VMInfo &vminfo = *video_engine->GetInfo();
        const int vshrink = vminfo.width < 640 ? 1 : 0;
        ecl::Font *menufont = enigma::GetFont("menufont");

        Label * lbTitle = new Label(N_("Text Search:"), HALIGN_LEFT);
// TRANSLATORS: the translation can have double size of the english text 
        std::string helpText = _("case independent search in title, author, filename");
        std::string workString = helpText;
        std::string::size_type breakPos = breakString(menufont, workString, " ", vshrink?190:380);
        Label * lbInfo1 = new UntranslatedLabel(workString.substr(0,breakPos), HALIGN_LEFT);
        Label * lbInfo2 = new UntranslatedLabel(workString.substr(breakPos), HALIGN_LEFT);
        tf_search_text = new TextField(sc->getSearchText(), this);
        lbLevelCount = new Label("");

        if(!vshrink) {
            lbInt = new Label(N_("Intelligence: "), HALIGN_RIGHT);
            lbDex = new Label(N_("Dexterity: "), HALIGN_RIGHT);
            lbPat = new Label(N_("Patience: "), HALIGN_RIGHT);
            lbKno = new Label(N_("Knowledge: "), HALIGN_RIGHT);
            lbSpe = new Label(N_("Speed: "), HALIGN_RIGHT);
            lbDif = new Label(N_("Difficulty: "), HALIGN_RIGHT);
            lbAvr = new Label(N_("Average Rating: "), HALIGN_RIGHT);
            lbUnsolvEasy = new Label(N_("Only levels that are unsolved in easy mode:"), HALIGN_RIGHT);
            lbUnsolvHard = new Label(N_("Only levels that are unsolved in normal mode:"), HALIGN_RIGHT);
            // new Label(N_("Only levels of level pack group:"), HALIGN_RIGHT);
            lbSortMethod = new Label(N_("Sort by: "), HALIGN_RIGHT);
            but_int_min = new RatingSearchButton(this, sc, SC_INT, SC_MIN, 1, 5);
            but_int_max = new RatingSearchButton(this, sc, SC_INT, SC_MAX, 1, 5);
            but_dex_min = new RatingSearchButton(this, sc, SC_DEX, SC_MIN, 1, 5);
            but_dex_max = new RatingSearchButton(this, sc, SC_DEX, SC_MAX, 1, 5);
            but_pat_min = new RatingSearchButton(this, sc, SC_PAT, SC_MIN, 1, 5);
            but_pat_max = new RatingSearchButton(this, sc, SC_PAT, SC_MAX, 1, 5);
            but_kno_min = new RatingSearchButton(this, sc, SC_KNO, SC_MIN, 1, 6);
            but_kno_max = new RatingSearchButton(this, sc, SC_KNO, SC_MAX, 1, 6);
            but_spe_min = new RatingSearchButton(this, sc, SC_SPE, SC_MIN, 1, 5);
            but_spe_max = new RatingSearchButton(this, sc, SC_SPE, SC_MAX, 1, 5);
            but_dif_min = new RatingSearchButton(this, sc, SC_DIF, SC_MIN, 0, 100);
            but_dif_max = new RatingSearchButton(this, sc, SC_DIF, SC_MAX, 0, 100);
            but_avr_min = new RatingSearchButton(this, sc, SC_AVR, SC_MIN, 0, 100, 10);
            but_avr_max = new RatingSearchButton(this, sc, SC_AVR, SC_MAX, 0, 100, 10);

            but_only_ue = new UnsolvedSearchButton(this, sc, DIFFICULTY_EASY);
            but_only_uh = new UnsolvedSearchButton(this, sc, DIFFICULTY_HARD);
            but_sortmethod = new SortMethodButton(this, sc);
        }

        int x = vminfo.width/2;
        int y = vminfo.height/2;
        this->add(lbTitle, Rect(0, vshrink?0:0, vshrink?190:380, vshrink?17:35));
        this->add(lbInfo1, Rect(0, vshrink?30:60, vshrink?190:380, vshrink?12:25));
        this->add(lbInfo2, Rect(0, vshrink?45:90, vshrink?190:380, vshrink?12:25));
        this->add(tf_search_text, Rect(0, vshrink?77:155, vshrink?190:380, vshrink?17:35));
        this->add(lbSortMethod, Rect(0, vshrink?110:210, vshrink?80:180, vshrink?17:35));
        this->add(but_sortmethod, Rect(vshrink?90:200, vshrink?110:210, vshrink?100:180, vshrink?17:35));
        if(!vshrink) {
            this->add(lbInt,       Rect(400,   0, 100, 35));
            this->add(but_int_min, Rect(520,   0,  50, 35));
            this->add(but_int_max, Rect(610,   0,  50, 35));
            this->add(lbDex,       Rect(400,  50, 100, 35));
            this->add(but_dex_min, Rect(520,  50,  50, 35));
            this->add(but_dex_max, Rect(610,  50,  50, 35));
            this->add(lbPat,       Rect(400, 100, 100, 35));
            this->add(but_pat_min, Rect(520, 100,  50, 35));
            this->add(but_pat_max, Rect(610, 100,  50, 35));
            this->add(lbKno,       Rect(400, 150, 100, 35));
            this->add(but_kno_min, Rect(520, 150,  50, 35));
            this->add(but_kno_max, Rect(610, 150,  50, 35));
            this->add(lbSpe,       Rect(400, 200, 100, 35));
            this->add(but_spe_min, Rect(520, 200,  50, 35));
            this->add(but_spe_max, Rect(610, 200,  50, 35));
            this->add(lbDif,       Rect(400, 250, 100, 35));
            this->add(but_dif_min, Rect(520, 250,  50, 35));
            this->add(but_dif_max, Rect(610, 250,  50, 35));
            this->add(lbAvr,       Rect(350, 300, 150, 35));
            this->add(but_avr_min, Rect(520, 300,  50, 35));
            this->add(but_avr_max, Rect(610, 300,  50, 35));
            this->add(lbUnsolvEasy, Rect(  0, 350, 590, 35));
            this->add(but_only_ue,  Rect(610, 350,  50, 35));
            this->add(lbUnsolvHard, Rect(  0, 400, 590, 35));
            this->add(but_only_uh,  Rect(610, 400,  50, 35));
            this->add(lbLevelCount, Rect(  0, 445, 590, 35));
        }
        center();

        // Create buttons on the bottom - positioning identical to Levelmenu
        Label * dummy = new Label();
        but_reset = new StaticTextButton(N_("Reset"), this);
        but_ignore = new StaticTextButton(N_("Undo"), this);
        but_search = new StaticTextButton(N_("Search"), this);
        
        HList * commandHList = new HList;
        commandHList->set_spacing(10);
        commandHList->set_alignment(HALIGN_CENTER, VALIGN_TOP);
        commandHList->set_default_size(vshrink?70:140, vshrink?17:35);
        commandHList->add_back(but_reset);
        commandHList->add_back(dummy);
        commandHList->add_back(but_ignore);
        commandHList->add_back(but_search);
        this->add(commandHList, Rect(vshrink?5:10, vminfo.height-(vshrink?25:50),
                                     vminfo.width-(vshrink?10:20), vshrink?17:35));

        set_key_focus(tf_search_text);

        if (!sc_is_fresh)
            on_action(NULL); // this will show/update the level count
    }

    bool SearchMenu::isSearchQuit() {
        return didSearch;
    }

    bool SearchMenu::on_event(const SDL_Event &e) {
        bool handled = false;
        if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_F1) {
                displayInfo(helptext_search, 2);
                draw_all();
                handled = true;
            }
        }
        return handled;
    }

    void SearchMenu::on_action(Widget *w) {
        if (w == but_search || (w == tf_search_text && tf_search_text->wasLastActionReturn())) {
            sc->setSearchText(tf_search_text->getText());
            lev::Index::setCurrentIndex(lev::Proxy::search(sc));
            didSearch = true;
            Menu::quit();
            return; // do not show new level count
        } else if (w == but_ignore) {
            Menu::quit();
            return; // do not show new level count
        } else if (w == but_reset) {
            sc->reset();
            tf_search_text->set_text("");
            invalidate_all();
        }
        // update level count
        if (lbLevelCount != NULL) {
            sc->setSearchText(tf_search_text->getText());
            int count = Proxy::countSearchResults(sc);
            std::string text = ecl::strf(_("Levels fitting these criteria: %d"), count);
            lbLevelCount->set_text(text);
        }
    }

    void SearchMenu::draw_background(ecl::GC &gc) {
        set_caption(_("Enigma - Search Menu"));
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
    }
}} // namespace enigma::gui
