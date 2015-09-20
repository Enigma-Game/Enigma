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
 
#include "gui/LevelPackMenu.hh"
#include "gui/LevelMenu.hh"
#include "gui/LPGroupConfig.hh"
#include "gui/LevelPackConfig.hh"
#include "gui/SearchMenu.hh"
#include "gui/HelpMenu.hh"
#include "errors.hh"
#include "nls.hh"
#include "resource_cache.hh"
#include "video.hh"
#include "lev/Index.hh"
#include "lev/PersistentIndex.hh"

#include "main.hh"

using namespace ecl;
using namespace std;

namespace enigma { namespace gui {
/* -------------------- Level Pack Menu -------------------- */
    std::map<std::string, std::string> LevelPackMenu::groupLastSelectedIndex;
    std::map<std::string, int> LevelPackMenu::groupLastSelectedColumn;
    std::string LevelPackMenu::lastGroupName;
    int LevelPackMenu::firstDisplayedGroup = 0;

    static const char *helptext[] = {
        N_("Left column:"),              N_("Levelpack groups"),
        N_("Right columns:"),            N_("Levelpacks of selected group"),
        N_("Left click:"),               N_("Select group or levelpack"),
        N_("Right or control click:"),   N_("Configure group or levelpack"),
        0
    };
    
    LevelPackMenu::LevelPackMenu() : packsHList (NULL), scrollLeft (NULL), 
            scrollRight (NULL), scrollUp (NULL), scrollDown (NULL), 
            groupsVList (NULL), isLevelMenuSubmenu (false) {
        const video::VMInfo &vminfo = *video::GetInfo();
        const int vshrink = vminfo.width < 640 ? 1 : 0;
        vm = vminfo.videomode;
        vtt = vminfo.tt;
        vh = vminfo.area.x;
        vv = (vminfo.height - vminfo.area.h)/2;
       
        // Create buttons - positioning identical to Levelmenu
        but_new = new StaticTextButton(N_("New Group"), this);
        but_search = new StaticTextButton(N_("Search"), this);
        but_level = new StaticTextButton(N_("Start Game"), this);
        but_main = new StaticTextButton(N_("Main Menu"), this);
        
        commandHList = new HList;
        commandHList->set_spacing(vshrink ? 5 : 10);
        commandHList->set_alignment(HALIGN_CENTER, VALIGN_TOP);
        commandHList->set_default_size(vshrink ? 70 : 140, vshrink ? 17 : 35);
        commandHList->add_back(but_new);
        commandHList->add_back(but_search);
        commandHList->add_back(but_level);
        commandHList->add_back(but_main);
        this->add(commandHList, Rect(vshrink ? 5 : 10, vminfo.height-(vshrink ? 25 : 50), vminfo.width-(vshrink ? 10 : 20), vminfo.height < 320 ? 17 : 35));
        
    }
    
    void LevelPackMenu::setupMenu() {
        static struct SpacingConfig {
            int packcolumns, rows;
            int vmargin, vrow_row;
            int hmargin, hgroup_pack, hscrollbutton, hscroll_pack, hpack_pack;
        } param[] = {
            {  // VTS_16 (320x240)
                2, 9,
                7, 5,
                10, 18, 11, 5, 10
            },
            {  // VTS_32 (640x480)
                2, 9,
                15, 10,
                20, 36, 22, 10, 20
            },
            {  // VTS_40 (800x600)
                3, 11,
                15, 13,
                15, 36, 22, 10, 15
            },
            {  // VTS_48 (960x720)  VM_1024x768
                3, 14,
                15, 10,
                70, 56, 22, 20, 20
            },
            {  // VTS_64 (1280x960)
                5, 17,
                25, 14,
                60, 40, 22, 18, 20
            }
        };
        
        if (groupsVList != NULL) {
            groupsVList->clear();
            remove_child(groupsVList);
            delete groupsVList;
            groupsVList = NULL;
            scrollUp = NULL;   // deleted with groupsVList
            scrollDown = NULL; // deleted with groupsVList
        }
        
        if (packsHList != NULL) {
            packsHList->clear();
            remove_child(packsHList);
            delete packsHList;
            packsHList = NULL;
        }
        
        if (scrollLeft != NULL) {
            remove_child(scrollLeft);
            delete scrollLeft;
            scrollLeft = NULL;
        }
        if (scrollRight != NULL) {
            remove_child(scrollRight);
            delete scrollRight;
            scrollRight =  NULL;
        }
        
        packButtons.clear();
        groupButtons.clear();
        
        std::vector<std::string> groupNames = lev::Index::getGroupNames();
        int groupCount = groupNames.size();
        std::string curGroupName = lev::Index::getCurrentGroup();
        bool needUpScroll = false;
        bool needDownScroll = false;
        int numDisplayGroups = param[vtt].rows;
        int usedGroupRows = (groupCount > numDisplayGroups) ? numDisplayGroups : groupCount;
        
        // correct scroll attempts and screen resolution changes
        firstDisplayedGroup = ecl::Clamp<int>(firstDisplayedGroup, 0, 
            (groupCount > numDisplayGroups) ? groupCount - numDisplayGroups : 0);
        needUpScroll = firstDisplayedGroup > 0;
        needDownScroll = firstDisplayedGroup < groupCount - numDisplayGroups;
        if (curGroupName != lastGroupName) {
            // group changed by indirect user action - ensure it is visible
            int curGroupPos = getGroupPosition(&groupNames, curGroupName);
            if (curGroupPos != -1) {
                if (curGroupPos <= firstDisplayedGroup ) {
                    if (curGroupPos <= 1) {
                        needUpScroll = false;
                        firstDisplayedGroup = 0;
                    } else {
                        needUpScroll = true;
                        firstDisplayedGroup = curGroupPos -1;
                    }
                    needDownScroll = firstDisplayedGroup < groupCount - numDisplayGroups;
                } else if (curGroupPos >= firstDisplayedGroup + numDisplayGroups - 1) {
                    if (curGroupPos >= groupCount - 2) {
                        needDownScroll = false;
                        firstDisplayedGroup = groupCount - numDisplayGroups;
                    } else {
                        needDownScroll = true;
                        firstDisplayedGroup = curGroupPos - numDisplayGroups + 2;
                    }
                    if (firstDisplayedGroup < 0)
                        firstDisplayedGroup = 0;
                    needUpScroll = firstDisplayedGroup > 0;
                }
            }
        }
        
        
        groupsVList = new VList; 
        groupsVList->set_spacing(param[vtt].vrow_row);
        groupsVList->set_alignment(HALIGN_LEFT, VALIGN_CENTER);
        groupsVList->set_default_size(vtt==0 ? 80 : 160, vtt==0 ? 17 : 35);
        
        for (int i = 0; i < usedGroupRows; i++) {
            if (i == 0 && needUpScroll) {
                scrollUp = new ImageButton("ic-up", "ic-up1", this);
                groupsVList->add_back(scrollUp);
            } else if (i == usedGroupRows -1 && needDownScroll) {
                scrollDown = new ImageButton("ic-down", "ic-down1", this);
                groupsVList->add_back(scrollDown);
            } else {
                TextButton * button = new UntranslatedStaticTextButton(
                        groupNames[firstDisplayedGroup + i], this);;
                groupButtons.push_back(button);
                groupsVList->add_back(button);
            }
        }

        this->add(groupsVList, Rect(param[vtt].hmargin + vh, param[vtt].vmargin + vv, 
                vtt==0 ? 80 : 160, param[vtt].rows * (vtt==0 ? 17 : 35) + 
                (param[vtt].rows - 1) * param[vtt].vrow_row));
        
        lastGroupName = curGroupName;
        std::vector<lev::Index *> * group = lev::Index::getGroup(curGroupName);
        ASSERT(group != NULL, XFrontend,"");
        unsigned packCount = group->size();
        
        int posCurrentIndex = getIndexPosition(group, lev::Index::getCurrentIndex()->getName());
        int selectedColumn = lev::Index::getGroupSelectedColumn(curGroupName);
        int colCurrentIndex = 0;
        int nextPack = 0;        // pack displayed at top of first display column
        
        if (selectedColumn != INDEX_GROUP_COLUMN_UNKNOWN || 
                groupLastSelectedIndex.find(curGroupName) == groupLastSelectedIndex.end()) {
            colCurrentIndex = checkColumn(param[vtt].rows, param[vtt].packcolumns,
                    packCount, posCurrentIndex, selectedColumn);
            nextPack = (posCurrentIndex / param[vtt].rows - colCurrentIndex) * param[vtt].rows;
        } else {
            // the user selected a new level pack and the column was not yet
            // calculated: we try to keep the display unchanged in respect of
            // of the last selected pack and if necessary scroll minimum amount
            // of columns 
            int posLastIndex = getIndexPosition(group,groupLastSelectedIndex[curGroupName]);
            int colLastIndex = checkColumn(param[vtt].rows, param[vtt].packcolumns,
                    packCount, posLastIndex, groupLastSelectedColumn[curGroupName]);
            nextPack = (posLastIndex / param[vtt].rows - colLastIndex) * param[vtt].rows;
            if (posCurrentIndex < nextPack) {
                // current index would be left of display - we need to scroll left
                nextPack -= (((nextPack - posCurrentIndex - 1)/param[vtt].rows) + 1) *
                        param[vtt].rows;
                colCurrentIndex = 0;
            } else if (posCurrentIndex < nextPack + param[vtt].rows * param[vtt].packcolumns) {
                // current index is still visible - keep nextPack
                colCurrentIndex = (posCurrentIndex - nextPack) / param[vtt].rows;
            } else {
                // current index would be right of display - we need to scroll right
                nextPack += (((posCurrentIndex - nextPack)/param[vtt].rows) - 
                        (param[vtt].packcolumns - 1)) * param[vtt].rows;
                colCurrentIndex = param[vtt].packcolumns - 1;                
            }
        }
        
        bool needRightScroll = ((int)packCount > nextPack + param[vtt].rows * param[vtt].packcolumns);
        bool needLeftScroll = (nextPack > 0);

        lev::Index::setGroupSelectedColumn(curGroupName, colCurrentIndex);
        groupLastSelectedIndex[curGroupName] = lev::Index::getCurrentIndex()->getName();
        groupLastSelectedColumn[curGroupName] = colCurrentIndex;
        
        packsHList = new HList; 
        packsHList->set_spacing(param[vtt].hpack_pack);
        packsHList->set_alignment(HALIGN_CENTER, VALIGN_TOP);
        packsHList->set_default_size(vtt==0 ? 80 : 160, param[vtt].rows*(vtt==0 ? 17 : 35) + 
                (param[vtt].rows - 1) * param[vtt].vrow_row);
        
        for (int col = 0; col < param[vtt].packcolumns; col++) {
            if (packCount - nextPack > 0) {
                VList * pl = new VList;
                pl->set_spacing (param[vtt].vrow_row);
                // first column is centered - if it is full it is like top alignment:
                pl->set_alignment (HALIGN_LEFT, col == 0 ? VALIGN_CENTER : VALIGN_TOP);
                pl->set_default_size (vtt==0 ? 80 : 160, vtt==0 ? 17 : 35);
                for (int row = 0; row < param[vtt].rows; row++) {
                    if (nextPack < packCount) {
                        lev::Index *ind = (*group)[nextPack];
                        TextButton * button = new UntranslatedStaticTextButton(ind->getName(), this);
                        packButtons.push_back(button);
                        pl->add_back(button);
                        nextPack++;
                    } else
                        break;
                }
                packsHList->add_back(pl);               
            } else
                break;
        }

        this->add(packsHList, Rect(param[vtt].hmargin + vh + (vtt==0 ? 80 : 160) + param[vtt].hgroup_pack +
                param[vtt].hscrollbutton + param[vtt].hscroll_pack, 
                param[vtt].vmargin + vv, 
                param[vtt].packcolumns * (vtt==0 ? 80 : 160) + (param[vtt].packcolumns - 1) * 
                param[vtt].hpack_pack, 
                param[vtt].rows * (vtt==0 ? 17 : 35) + 
                (param[vtt].rows - 1) * param[vtt].vrow_row));
        
        if (needLeftScroll) {
            scrollLeft = new ImageButton("ic-left", "ic-left1", this);
            this->add(scrollLeft, Rect(param[vtt].hmargin + vh + (vtt==0 ? 80 : 160) + param[vtt].hgroup_pack,
                    param[vtt].vmargin + vv + param[vtt].rows / 2 * ((vtt==0 ? 17 : 35) + param[vtt].vrow_row),
                    param[vtt].hscrollbutton, (vtt==0 ? 17 : 35)));
        }
                
        if (needRightScroll) {
            scrollRight = new ImageButton("ic-right", "ic-right1", this);
            this->add(scrollRight, Rect(param[vtt].hmargin + vh + (vtt==0 ? 80 : 160) + param[vtt].hgroup_pack +
                    param[vtt].hscrollbutton + 2 * param[vtt].hscroll_pack +
                    param[vtt].packcolumns * (vtt==0 ? 80 : 160) + (param[vtt].packcolumns - 1) * 
                    param[vtt].hpack_pack,
                    param[vtt].vmargin + vv+ param[vtt].rows / 2 * ((vtt==0 ? 17 : 35) + param[vtt].vrow_row),
                    param[vtt].hscrollbutton, (vtt==0 ? 17 : 35)));
        }
    }
    
    void LevelPackMenu::manageLevelMenu() {
        bool finished = false;
        while (!finished) {
            {        
                LevelMenu m;
                if (!m.manage() || m.isMainQuit()) {
                    // ESC or Main button has been pressed in LevelMenu -
                    // the user wants to return
                    finished = true;
                }
            }
            if (!finished) {
                // the user left LevelMenu via LevelPack button 
                this->isLevelMenuSubmenu = true;
                if (this->manage()) {
                    // not ESC - the user pressed Main button
                    finished = true;
                } else {
                    // the user pressed ESC - return to LevelMenu
                }
            }
        }       
    }

    bool LevelPackMenu::manage() {
        setupMenu();
        updateHighlight();
        return Menu::manage();
    }
    
    bool LevelPackMenu::on_event (const SDL_Event &e) {
        switch (e.type) {
            case SDL_KEYDOWN:
                SDLKey keysym = e.key.keysym.sym;
                switch (keysym) {
                case SDLK_F1:     
                    displayHelp(helptext, 200);
                    invalidate_all();
                    return true;
                default:
                    break;
                }
                break;
        }
        return false;
    }
    
    void LevelPackMenu::on_action(Widget *w) {
        if (w == but_main) {
            Menu::quit();
        } else if (w == but_new) {
            LPGroupConfig m("");
            m.manage();
            setupMenu();
            updateHighlight();
            invalidate_all();
        } else if (w == but_level) {
            LevelMenu m;
            if ((!m.manage() && isLevelMenuSubmenu) || m.isMainQuit()) {
                // ESC in LevelMenu in case we are a submenu of LevelMenu or
                // Main button has been pressed in LevelMenu 
                Menu::quit();
            }
            setupMenu();
            updateHighlight();
            invalidate_all();            
        } else if (w == but_search) {
            SearchMenu ml;
            ml.manage();
            if (ml.isSearchQuit()) {
                // show search result levelpack
                LevelMenu ml;
                if ((!ml.manage() && isLevelMenuSubmenu) || ml.isMainQuit()) {
                    // ESC in LevelMenu in cade we are a submenu of LevelMenu or
                    // Main button has been pressed in LevelMenu
                    Menu::quit();
                }
            }
            setupMenu();
            updateHighlight();
            invalidate_all();            
        } else if (w == scrollUp) {
            firstDisplayedGroup--;
            reset_active_widget();  // we will delete it with setup
            setupMenu();
            updateHighlight();
            invalidate_all();            
        } else if (w == scrollDown) {
            firstDisplayedGroup++;
            reset_active_widget();  // we will delete it with setup
            setupMenu();
            updateHighlight();
            invalidate_all();            
        } else if (w->get_parent() == groupsVList){
            lev::Index::setCurrentGroup(dynamic_cast<TextButton *>(w)->get_text());
            if ((w->lastMouseButton() == SDL_BUTTON_RIGHT ||
                     w->lastModifierKeys() & KMOD_CTRL) &&
                     dynamic_cast<TextButton *>(w)->get_text() != INDEX_ALL_PACKS) {
                // configure group
                // INDEX_ALL_PACKS cannot be renamed, deleted, no packs can be created
                LPGroupConfig m(dynamic_cast<TextButton *>(w)->get_text());
                m.manage();
                lastGroupName = "";  // the group may have moved, force a recalc
            }
            reset_active_widget();  // we will delete it with setup
            setupMenu();
            updateHighlight();
            invalidate_all();
        } else if (w->get_parent()->get_parent() == packsHList){
            lev::Index::setCurrentIndex(dynamic_cast<TextButton *>(w)->get_text());
            if (w->lastMouseButton() == SDL_BUTTON_RIGHT ||
                     w->lastModifierKeys() & KMOD_CTRL) {
                // configure levelpack index
                LevelPackConfig m(dynamic_cast<TextButton *>(w)->get_text());
                m.manage();
            } else {
                LevelMenu m;
                if ((!m.manage() && isLevelMenuSubmenu) || m.isMainQuit()) {
                    // ESC in LevelMenu in case we are a submenu of LevelMenu or
                    // Main button has been pressed in LevelMenu
                    Menu::quit();
                    return;
                }
            }
            reset_active_widget();  // we will delete it with setup
            setupMenu();
            updateHighlight();
            invalidate_all();
        } else if (w == scrollLeft) {
            std::string curGroupName = lev::Index::getCurrentGroup();
            lev::Index::setGroupSelectedColumn(curGroupName,
                    lev::Index::getGroupSelectedColumn(curGroupName) + 1);
            reset_active_widget();  // we will delete it with setup
            setupMenu();
            updateHighlight();
            invalidate_all();
        } else if (w == scrollRight) {
            std::string curGroupName = lev::Index::getCurrentGroup();
            lev::Index::setGroupSelectedColumn(curGroupName,
                    lev::Index::getGroupSelectedColumn(curGroupName) - 1);
            reset_active_widget();  // we will delete it with setup
            setupMenu();
            updateHighlight();
            invalidate_all();
        }
    }
    
    void LevelPackMenu::updateHighlight() {
        for (unsigned i = 0; i < packButtons.size(); i++) {
            TextButton * button = packButtons[i];
            if (button->get_text() == lev::Index::getCurrentIndex()->getName())
                button->setHighlight(true);
            else
                button->setHighlight(false);      
        }
        for (unsigned i = 0; i < groupButtons.size(); i++) {
            TextButton * button = groupButtons[i];
            if (button->get_text() == lev::Index::getCurrentGroup())
                button->setHighlight(true);
            else
                button->setHighlight(false);      
        }
    }
    
    void LevelPackMenu::draw_background(ecl::GC &gc) {
        const video::VMInfo *vminfo = video::GetInfo();
        video::SetCaption(("Enigma - Level Pack Menu"));
        blit(gc, vminfo->mbg_offsetx, vminfo->mbg_offsety, enigma::GetImage("menu_bg", ".jpg"));
    }
    
    int LevelPackMenu::getGroupPosition(std::vector<std::string> * groups, std::string groupName) {
        std::vector<std::string>::iterator it;
        int i = 0;
        for (it = groups->begin(); it != groups->end(); it++, i++) {
            if ((*it) == groupName) {
                return i;
            }
        }
        return -1;
    }
    
    int LevelPackMenu::getIndexPosition(std::vector<lev::Index *> * group, std::string indexName) {
        std::vector<lev::Index *>::iterator it;
        int i = 0;
        for (it = group->begin(); it != group->end(); it++, i++) {
            if ((*it)->getName() == indexName) {
                return i;
            }
        }
        return -1;
    }
    
    int LevelPackMenu::checkColumn(int rows, int columns, int size, 
            int position, int oldColumn) {
        int naturalColumn = position / rows;
        int numColumns = (size - 1) / rows + 1;
        if (oldColumn == INDEX_GROUP_COLUMN_UNKNOWN)
            return (naturalColumn > columns) ? columns - 1 : naturalColumn;
        else
            return ecl::Clamp<int>(oldColumn, naturalColumn - 
                    ((numColumns > columns) ? (numColumns - columns) : 0), 
                    naturalColumn);
    }
    

}} // namespace enigma::gui
