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
        intialGroupPosition = groups.size() - 1;   // INDEX_EVERY_GROUP as default
        // mark index's default group with square brackets and find current group
        bool defaultGroupFound = false;
        for (int i = 0; i < groups.size(); i++) {
            if (groups[i] == packIndex->getGroupName()) {
                intialGroupPosition = i;
            }
            if (groups[i] == packIndex->getDefaultGroupName()) {
                groups[i] = std::string("[") + groups[i] +"]";
                defaultGroupFound = true;
            }
        }
        if (!defaultGroupFound)  {
            groups.push_back(std::string("[") + packIndex->getDefaultGroupName() +"]");
        }
        groupButton = new GroupButton(groups, intialGroupPosition);

        // index location list setup
        std::vector<lev::Index *> * allIndices = lev::Index::getGroup(INDEX_ALL_PACKS);
        for (int i = 0; i < allIndices->size(); i++)
            locationList.push_back((*allIndices)[i]->getName());
        position = -1;
        for (int i = 0; i < locationList.size(); i++) {
            if (locationList[i] == indexName) {
                position = i;
                break;
            } 
        }
        oldPosition = position;
        if (position < 0) { // may be improved by last User Index + 100 ...
            locationList.push_back(indexName);
            position = locationList.size() - 1;
        }

        VList * titleLeftVList = new VList;
        titleLeftVList->set_spacing(11);
        titleLeftVList->set_alignment(HALIGN_LEFT, VALIGN_CENTER);
        titleLeftVList->set_default_size(140, 35);
        Label * titleLabel = new Label(N_("Levelpack:"), HALIGN_RIGHT);
        Label * ownerLabel = new Label(N_("Owner:"), HALIGN_RIGHT);
        Label * groupLabel = new Label(N_("Group:"), HALIGN_RIGHT);
        Label * loactionLabel1 = new Label(N_("Location"), HALIGN_LEFT);
        Label * loactionLabel2 = new Label(N_("in [All Packs]:"), HALIGN_RIGHT);
        titleLeftVList->add_back(titleLabel);
        if (!isReasignOnly) {
           titleLeftVList->add_back(ownerLabel);
        }
        titleLeftVList->add_back(groupLabel);
        if (!isReasignOnly) {        
            titleLeftVList->add_back(new Label());
            titleLeftVList->add_back(loactionLabel1);
            titleLeftVList->add_back(loactionLabel2);
            titleLeftVList->add_back(new Label());
            titleLeftVList->add_back(new Label());
        }
        
        VList * valueLeftVList = new VList;
        valueLeftVList->set_spacing(11);
        valueLeftVList->set_alignment(HALIGN_LEFT, VALIGN_CENTER);
        valueLeftVList->set_default_size(160, 35);

        Label * titleValue = new Label(indexName, HALIGN_CENTER);
        pre2Index = new Label();
        pre1Index = new Label();
        thisIndex = new Label();
        post1Index = new Label();
        post2Index = new Label();

        valueLeftVList->add_back(titleValue);
        if (!isReasignOnly) {        
            valueLeftVList->add_back(new Label());
        }
        valueLeftVList->add_back(groupButton);
        if (!isReasignOnly) {        
            valueLeftVList->add_back(pre2Index);
            valueLeftVList->add_back(pre1Index);
            valueLeftVList->add_back(thisIndex);
            valueLeftVList->add_back(post1Index);
            valueLeftVList->add_back(post2Index);
        }

        VList * scrollVList = new VList;
        scrollVList->set_spacing(12);
        scrollVList->set_alignment(HALIGN_LEFT, VALIGN_CENTER);
        scrollVList->set_default_size(30, 35);

        scrollUp = new ImageButton("ic-up", "ic-up1", this);
        scrollDown = new ImageButton("ic-down", "ic-down1", this);
        scrollVList->add_back(scrollUp);
        scrollVList->add_back(scrollDown);

        if (isReasignOnly) {        
            this->add(titleLeftVList, Rect(vminfo.width/2 - 270, 15, 140, vminfo.height-97));
            this->add(valueLeftVList, Rect(vminfo.width/2 - 80, 15, 160, vminfo.height-97));
        } else {
            this->add(titleLeftVList, Rect(vminfo.width/2 - 300, 15, 140, vminfo.height-97));
            this->add(valueLeftVList, Rect(vminfo.width/2 - 140, 15, 160, vminfo.height-97));
            this->add(scrollVList, Rect(vminfo.width/2 + 30, 15+3*(35+12), 30, 5*35+4*12));
        }
     
        errorLabel = new Label("", HALIGN_CENTER);
        this->add(errorLabel, Rect(10, vminfo.height-97, vminfo.width-20, 35));
        
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
        
        updateLocationList();
    }
    
    void LevelPackConfig::updateLocationList() {
        pre2Index->set_text((position > 1) ? locationList[position - 2] : "");
        pre1Index->set_text((position > 0) ? locationList[position - 1] : "");
        thisIndex->set_text(packIndex->getName());
        post1Index->set_text((position < locationList.size() - 1) ? locationList[position + 1] : "");
        post2Index->set_text((position < locationList.size() - 2) ? locationList[position + 2] : "");        
    }
    
    bool LevelPackConfig::isUndoQuit() {
        return undo_quit;
    }
    
    void LevelPackConfig::on_action(Widget *w) {
        if (w == but_back) {
            if (groupButton->get_value() != intialGroupPosition) {
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
            if (position != oldPosition)
                packIndex->locateBehind(position > 0 ? locationList[position - 1] : "");
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
        } else if (w == scrollUp) {
            if (position > 0) {
                std::string tmp = locationList[position];
                locationList[position] = locationList[position - 1];
                locationList[position - 1] = tmp;
                position--;
                updateLocationList();
                invalidate_all();
            }
        } else if (w == scrollDown) {
            if (position < locationList.size() - 1) {
                std::string tmp = locationList[position];
                locationList[position] = locationList[position + 1];
                locationList[position + 1] = tmp;
                position++;
                updateLocationList();
                invalidate_all();
            }
        }
    }
    
    void LevelPackConfig::draw_background(ecl::GC &gc) {
        video::SetCaption(("Enigma - Level Pack Configuration"));
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
    }
    

}} // namespace enigma::gui
