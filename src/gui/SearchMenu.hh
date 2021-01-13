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
 
#ifndef GUI_SEARCHMENU_HH_INCLUDED
#define GUI_SEARCHMENU_HH_INCLUDED

#include "gui/Menu.hh"
#include "gui/TextField.hh"
#include "lev/Proxy.hh"
#include "lev/SearchCombination.hh"

namespace enigma { namespace gui {

    class SearchMenu : public gui::Menu {
    public:
        SearchMenu ();
        
        void on_action(Widget *w);
        void draw_background(ecl::GC &gc);
        bool isSearchQuit();
        
    private:
        TextField   *tf_search_text;
        Label       *lbInt;
        Label       *lbDex;
        Label       *lbPat;
        Label       *lbKno;
        Label       *lbSpe;
        Label       *lbDif;
        Label       *lbAvr;
        Label       *lbUnsolvEasy;
        Label       *lbUnsolvHard;
        Label       *lbSortMethod;
        Label       *lbLevelCount;
        Widget      *but_reset;
        Widget      *but_ignore;
        Widget      *but_search;
        Widget      *but_int_min;
        Widget      *but_int_max;
        Widget      *but_dex_min;
        Widget      *but_dex_max;
        Widget      *but_pat_min;
        Widget      *but_pat_max;
        Widget      *but_kno_min;
        Widget      *but_kno_max;
        Widget      *but_spe_min;
        Widget      *but_spe_max;
        Widget      *but_dif_min;
        Widget      *but_dif_max;
        Widget      *but_avr_min;
        Widget      *but_avr_max;
        Widget      *but_only_ue;
        Widget      *but_only_uh;
        Widget      *but_sortmethod;
        bool         didSearch;
        lev::SearchCombination *sc;
    };

}} // namespace enigma::gui
#endif
