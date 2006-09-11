/*
 * Copyright (C) 2002,2003,2004,2005,2006 Daniel Heck, Ronald Lamprecht
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
#ifndef GUI_LEVELPACKMENU_HH_INCLUDED
#define GUI_LEVELPACKMENU_HH_INCLUDED

#include "gui/Menu.hh"
#include "video.hh"
#include "lev/Index.hh"

#include <map>
#include <string>

namespace enigma { namespace gui {
/* -------------------- LevelPackMenu -------------------- */

    class LevelPackMenu : public Menu {
    public:
        LevelPackMenu();
        bool manageLevelMenu();
        virtual bool manage();

        void on_action(Widget *w);
        void draw_background(ecl::GC &gc);

    private:
        static std::map<std::string, std::string> groupLastSelectedIndex;
        static std::map<std::string, int> groupLastSelectedColumn;
        static std::string lastGroupName;
        static int firstDisplayedGroup;
        
        video::VideoModes vm;
        std::vector<TextButton *> packButtons;
        std::vector<TextButton *> groupButtons;

        HList       *packsHList;
        ImageButton *scrollLeft;
        ImageButton *scrollRight;        
        ImageButton *scrollUp;        
        ImageButton *scrollDown;        
        VList       *groupsVList;
        HList       *commandHList;
        Widget      *but_search;
        Widget      *but_new;
        Widget      *but_level;
        Widget      *but_main;
        
        void setupMenu();
        void updateHighlight();
        int getGroupPosition(std::vector<std::string> * groups, std::string groupName);
        int getIndexPosition(std::vector<lev::Index *> * group, std::string indexName);
        int checkColumn(int rows, int columns, int size, int position, int oldColumn);
    };
}} // namespace enigma::gui
#endif
