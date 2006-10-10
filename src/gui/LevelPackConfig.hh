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
 
#ifndef GUI_LEVELPACKCONFIG_HH_INCLUDED
#define GUI_LEVELPACKCONFIG_HH_INCLUDED

#include "gui/Menu.hh"
#include "gui/TextField.hh"
#include "lev/Index.hh"

namespace enigma { namespace gui {

    class GroupButton : public ValueButton {
    public:
        GroupButton(std::vector<std::string> groups, int pos);
        int get_value() const;
        void set_value(int value);
        std::string get_text(int value) const;
    private:
        std::vector<std::string> groupNames;
        int position;
    };

    class LevelPackConfig : public gui::Menu {
    public:
        LevelPackConfig (std::string indexName, std::string groupName = "",
            bool forceGroupReasign = false);
        
        void on_action(Widget *w);
        void draw_background(ecl::GC &gc);
        bool isUndoQuit();
    private:
        lev::Index  *packIndex;
        TextField   *tf_packName;
        GroupButton *groupButton;
        int          initialPosition;
        Label       *errorLabel;
        Widget      *but_up;
        Widget      *but_down;
        Widget      *but_edit;
        Widget      *but_update;
        Widget      *but_ignore;
        Widget      *but_back;
        bool         isReasignOnly;
        bool         undo_quit;
    };

}} // namespace enigma::gui
#endif
