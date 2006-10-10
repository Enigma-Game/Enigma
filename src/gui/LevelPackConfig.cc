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
 
#include "gui/LevelPackConfig.hh"
#include "ecl.hh"
#include "errors.hh"
#include "nls.hh"
#include "video.hh"
#include "lev/Index.hh"

#include "main.hh"

using namespace ecl;
using namespace std;

namespace enigma { namespace gui {
    GroupButton::GroupButton(std::vector<std::string> groups, int pos) : 
            ValueButton(0, groups.size() - 1), position (pos), groupNames (groups) {
        init();
    }
    
    int GroupButton::get_value() const  {
        return position; 
    }
    
    void GroupButton::set_value(int value) {
        position = value;
    }
    
    std::string GroupButton::get_text(int value) const {
        return groupNames[value];
    }
    
    
    
    LevelPackConfig::LevelPackConfig(std::string indexName, std::string groupName,
            bool forceGroupReasign) : isReasignOnly (forceGroupReasign), undo_quit (false) {
        const video::VMInfo &vminfo = *video::GetInfo();
        
        // TODO handle empty indexName as new Index
        
        packIndex = lev::Index::findIndex(indexName);
        ASSERT (packIndex != NULL, XFrontend, "not existing index Name");
        
        // build a list of allowed group
        std::vector<std::string> groups = lev::Index::getGroupNames();
        // eliminate pseudo group "All Packs"
        std::vector<std::string>::iterator itg = groups.begin();
        while (itg != groups.end()) {
            if (*itg == INDEX_ALL_PACKS) {
                itg = groups.erase(itg);
                break;
            }
            if (itg != groups.end())
                itg++;
        }
        // add pseudo group "[Every Group]"
        groups.push_back(std::string("[") + INDEX_EVERY_GROUP +"]");
        initialPosition = groups.size() - 1;   // INDEX_EVERY_GROUP as default
        // mark index's default group with square brackets and find current group
        bool defaultGroupFound = false;
        for (int i = 0; i < groups.size(); i++) {
            if (groups[i] == packIndex->getGroupName()) {
                initialPosition = i;
            }
            if (groups[i] == packIndex->getDefaultGroupName()) {
                groups[i] = std::string("[") + groups[i] +"]";
                defaultGroupFound = true;
            }
        }
        if (!defaultGroupFound)  {
            groups.push_back(std::string("[") + packIndex->getDefaultGroupName() +"]");
        }
        groupButton = new GroupButton(groups, initialPosition);

        VList * titleLeftVList = new VList;
        titleLeftVList->set_spacing(12);
        titleLeftVList->set_alignment(HALIGN_LEFT, VALIGN_CENTER);
        titleLeftVList->set_default_size(160, 35);
        Label * titleLabel = new Label(N_("Levelpack:"), HALIGN_RIGHT);
        Label * groupLabel = new Label(N_("Group:"), HALIGN_RIGHT);
        titleLeftVList->add_back(titleLabel);
        titleLeftVList->add_back(groupLabel);
        
        VList * valueLeftVList = new VList;
        valueLeftVList->set_spacing(12);
        valueLeftVList->set_alignment(HALIGN_LEFT, VALIGN_CENTER);
        valueLeftVList->set_default_size(160, 35);

        Label * titleValue = new Label(indexName, HALIGN_CENTER);
        valueLeftVList->add_back(titleValue);
        valueLeftVList->add_back(groupButton);

        this->add(titleLeftVList, Rect(vminfo.width/2 - 290, 0, 160, vminfo.height-100));
        this->add(valueLeftVList, Rect(vminfo.width/2 - 80, 0, 160, vminfo.height-100));
     
        errorLabel = new Label("", HALIGN_CENTER);
        this->add(errorLabel, Rect(10, vminfo.height-100, vminfo.width-20, 35));
        
        if (isReasignOnly) 
            errorLabel->set_text(N_("Please reasign levelpack to another group for group deletion"));
       
        // Create buttons - positioning identical to Levelmenu
        but_edit = new StaticTextButton(N_("Compose Pack"), this);
        but_update = new StaticTextButton(N_("Update Pack"), this);
        but_ignore = new StaticTextButton(N_("Undo"), this);
        but_back = new StaticTextButton(N_("Ok"), this);
        
        HList * commandHList = new HList;
        commandHList->set_spacing(10);
        commandHList->set_alignment(HALIGN_CENTER, VALIGN_TOP);
        commandHList->set_default_size(140, 35);
        if (isReasignOnly) {
            commandHList->add_back(new Label());
            commandHList->add_back(new Label());
        } else {
            commandHList->add_back(but_edit);
            commandHList->add_back(but_update);
        }
        commandHList->add_back(but_ignore);
        commandHList->add_back(but_back);
        this->add(commandHList, Rect(10, vminfo.height-50, vminfo.width-20, 35));
    }
    
    bool LevelPackConfig::isUndoQuit() {
        return undo_quit;
    }
    
    void LevelPackConfig::on_action(Widget *w) {
        if (w == but_back) {
            if (groupButton->get_value() != initialPosition) {
                std::string newGroupName = groupButton->get_text(groupButton->get_value());
                // strip off square brackets used to mark default and pseudo groups
                if (newGroupName.size() > 2 && newGroupName[0] == '[' && 
                        newGroupName[newGroupName.size() -1] == ']') {
                    newGroupName = newGroupName.substr(1, newGroupName.size() - 2);
                }
                packIndex->moveToGroup(newGroupName);
            } else if (isReasignOnly) {
                // the user did not reasign - take as an undo request
                undo_quit = true;
            }
            Menu::quit();
        } else if (w == but_ignore) {
            undo_quit = true;
            Menu::quit();
        } else if (w == but_update) {
            errorLabel->set_text(("Sorry - update not yet implemented."));
            invalidate_all();
        } else if (w == but_edit) {
            errorLabel->set_text(("Sorry - compose not yet implemented."));
            invalidate_all();
        }
    }
    
    void LevelPackConfig::draw_background(ecl::GC &gc) {
        video::SetCaption(("Enigma - Level Pack Configuration"));
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
    }
    

}} // namespace enigma::gui
