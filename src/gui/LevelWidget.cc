/*
 * Copyright (C) 2002,2003,2004,2005,2006 Daniel Heck
 * Copyright (C) 2006,2007                Ronald Lamprecht
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
 
#include "gui/LevelWidget.hh"
#include "gui/LevelMenu.hh"
#include "gui/LevelInspector.hh"

#include "ecl_font.hh"
#include "main.hh"
#include "nls.hh"
#include "options.hh"
#include "resource_cache.hh"
#include "SoundEffectManager.hh"
#include "StateManager.hh"
#include "video.hh"
#include "lev/Proxy.hh"
#include <cassert>

using namespace ecl;
using namespace std;

#define SCREEN ecl::Screen::get_instance()

namespace enigma { namespace gui {
    /* -------------------- LevelWidget -------------------- */
    
    LevelWidget::LevelWidget(bool withScoreIcons, bool withEditBorder) : 
            displayScoreIcons (withScoreIcons), displayEditBorder (withEditBorder),
            listener(0), width (0), height (0), m_areas(), 
            isInvalidateUptodate (true), lastUpdate (0)
    {
        const VMInfo &vminfo = *video_engine->GetInfo();
        const int vshrink = vminfo.width < 640 ? 1 : 0;

        buttonw = vminfo.thumb.width +
                  (vshrink ? 13 : 27);  // min should be +30 for all modes but 640x480
        buttonh = vminfo.thumb.height + (vshrink ? 22 : 44);
        curIndex = lev::Index::getCurrentIndex();
        iselected = curIndex->getCurrentPosition();
        ifirst = curIndex->getScreenFirstPosition();
        preview_cache = LevelPreviewCache::instance();
        scoreMgr = lev::ScoreManager::instance();
        img_link        = enigma::GetImage("ic-link");
        img_copy        = enigma::GetImage("ic-copy");
        img_feather     = enigma::GetImage("ic-feather");
        img_easy        = enigma::GetImage("completed-easy");
        img_hard        = enigma::GetImage("completed");
        img_obsolete = enigma::GetImage(("ic-obsolete" + vminfo.thumb.suffix).c_str());
        img_outdated = enigma::GetImage(("ic-outdated" + vminfo.thumb.suffix).c_str());
        img_unavailable = enigma::GetImage("unavailable");
        img_par         = enigma::GetImage("par");
        img_wrEasy      = enigma::GetImage("ic-wr-easy");
        img_wrDifficult = enigma::GetImage("ic-wr-difficult");
        img_border = enigma::GetImage(("thumbborder" + vminfo.thumb.suffix).c_str());
        img_editborder = enigma::GetImage(("editborder" + vminfo.thumb.suffix).c_str());
        thumbmode = (vminfo.thumb.width == 160) ? 2 : ((vminfo.thumb.width == 120) ? 1 : 0);
    }
    
    void LevelWidget::syncFromIndexMgr() {
        if (curIndex != lev::Index::getCurrentIndex()) {
            curIndex = lev::Index::getCurrentIndex();
            iselected = curIndex->getCurrentPosition();
            ifirst = curIndex->getScreenFirstPosition();
            invalidate();
            sound::EmitSoundEvent ("menumove");
        } else if (iselected != curIndex->getCurrentPosition()) {
            sound::EmitSoundEvent ("menumove");            
        }
        // repair ifirst on boot and screen resolution changes
        set_selected(curIndex->getScreenFirstPosition(), 
                curIndex->getCurrentPosition());
    }

    void LevelWidget::syncToIndexMgr() {
        curIndex->setCurrentPosition(iselected);
        curIndex->setScreenFirstPosition(ifirst);
    }
        
    void LevelWidget::realize(const ecl::Rect &area_) {
        Widget::realize(area_);
        width = area_.w / buttonw;
        height = area_.h / buttonh;
    }
    
    void LevelWidget::trigger_action() {
        if (listener) {
            listener->on_action(this);
        }
    }
        
    void LevelWidget::scroll_up(int nlines) 
    {
        int newFirst = ifirst;
        int newSelected = iselected;
        for (; nlines; --nlines) {
            if (newFirst+width*height >= curIndex->size())
                break;
            newFirst += width;
            if (newSelected < newFirst)
                newSelected += width;
        }
        set_selected(newFirst, newSelected);
    }
    
    void LevelWidget::scroll_down(int nlines) 
    {
        int newFirst = ifirst;
        int newSelected = iselected;
        for (; nlines; --nlines) {
            if (newFirst == 0) { 
                break;
            } else if (newFirst < width) {
                newFirst = 0;
                if (newSelected >= width*height)
                    newSelected = width*height - 1;                
            } else {
                newFirst -= width;
                if (newSelected >= newFirst+width*height)
                    newSelected -= width;
            }
        }
        set_selected(newFirst, newSelected);
    }
    
    void LevelWidget::page_up() {
        set_selected((ifirst >= width*height ? ifirst - width*height : 0), 
                (iselected >= width*height ? iselected - width*height : 0));
        syncToIndexMgr();
    }
    
    void LevelWidget::page_down() 
    {
        int s = lev::Index::getCurrentIndex()->size() - 1;  // last position
        int lastPageFirst = (s >= width*height ? (s / width - height + 1) * width : 0);
    
        // make sure last page is shown as a whole
        int first = std::min<int> (lastPageFirst, ifirst + width*height);
        //    set_selected (first, s-1);
        set_selected(first, iselected + width*height);
        syncToIndexMgr();
    }
    
    void LevelWidget::start() { 
        set_selected(0,0); 
        syncToIndexMgr();
    }
    
    void LevelWidget::end() {
        int s = lev::Index::getCurrentIndex()->size() - 1;  // last position
        int lastPageFirst = (s >= width*height ? (s / width - height + 1) * width : 0);
        set_selected(lastPageFirst, s);
        syncToIndexMgr();
    }
    
    void LevelWidget::set_current(int newsel)
    {
        set_selected(ifirst, newsel);
    }
    
    void LevelWidget::set_selected(int newfirst, int newsel)
    {
        int numlevels = curIndex->size();
        newsel = Clamp<int> (newsel, 0, numlevels-1);
        if (newsel < 0) newsel = 0;
    
        if (newsel < newfirst)
            newfirst = (newsel/width)*width;
        if (newsel >= newfirst+width*height)
            newfirst = (newsel/width-height+1)*width;
    
        newfirst = ecl::Clamp<int>(newfirst, 0, numlevels-1);
        if (newfirst < 0) newfirst = 0;
    
        size_t oldsel = iselected;
        if (newfirst != ifirst) {
            ifirst    = newfirst;
            iselected = newsel;
    
            if (!m_areas.empty()) {
                sound::EmitSoundEvent ("menumove");
                if ((int)oldsel != newsel) 
                    sound::EmitSoundEvent ("menuswitch");
                invalidate();
            }
        }
        else if (newsel != iselected) {
            iselected = newsel;
    
            if (!m_areas.empty()) {
                sound::EmitSoundEvent ("menuswitch");
                invalidate_area(m_areas[oldsel-ifirst]); // old selection
                invalidate_area(m_areas[iselected-ifirst]); // new selection
            }
        }
    }
                
    int LevelWidget::thumb_off(int small, int medium, int large) {
    	return (thumbmode == 2) ? large : ((thumbmode == 1) ? medium : small);
    }

    bool LevelWidget::draw_level_preview(ecl::GC &gc, int x, int y, int borderWidth,
            lev::Proxy *proxy, bool selected, bool isCross, bool locked,
            bool allowGeneration, bool &didGenerate) { 
        // Draw button with level preview

        const VMInfo *vminfo = video_engine->GetInfo();
        Surface *img =
            preview_cache->getPreview(proxy, vminfo->thumb, allowGeneration, didGenerate);
        if (img == NULL)
            return false;
   
        if (selected) {
            blit (gc, x - borderWidth, y - borderWidth, displayEditBorder ? img_editborder : img_border);
            blit (gc, x, y, img);
        } else {
            // With SDL2, set_alpha doesn't work properly anymore. We are therefore
            // using set_brightness instead.
            // TODO: Revert to set_alpha, once it's repaired.
            img->set_brightness(170);
            blit (gc, x, y, img);
            img->set_brightness(255);
        }

        // Shade unavailable levels
        if (locked)
            blit (gc, x, y, img_unavailable);
    
        if (displayScoreIcons) {
            // Draw solved/changed icons on top of level preview
            // Easy/difficult mode and solved status:
            //   None: Level not beaten - no easy mode available
            //   Feather: Level not beaten - easy mode available
            //   Feather + Gold: Level beaten in normal mode - easy available
            //   Silver: Level beaten in easy mode (normal mode available)
            //   Gold: Level beaten in normal mode - easy not availabe
            //   Silver + Gold: Level beaten in all modes - easy available
            Surface *useAsEasy = NULL;
            Surface *useAsDifficult = NULL;
            if (proxy->hasEasyMode()) {
                useAsEasy = img_feather;
                if (scoreMgr->isSolved(proxy, DIFFICULTY_EASY))
                    useAsEasy = img_easy;
            }
            if (scoreMgr->isSolved(proxy, DIFFICULTY_HARD))
                useAsDifficult = img_hard;
            
            if (app.state->getInt("Difficulty") == DIFFICULTY_HARD) {
                // draw golden medal over silber medal
                if (useAsEasy != NULL)
                    blit (gc, x+thumb_off(3,3,24), y, useAsEasy);
                if (useAsDifficult != NULL)
                    blit (gc, x+thumb_off(8,8,29), y, useAsDifficult);
            }
            else {
                // draw silver medal over golden medal
                if (useAsDifficult != NULL)
                    blit (gc, x+thumb_off(8,8,29), y, useAsDifficult);
                if (useAsEasy != NULL)
                    blit (gc, x+thumb_off(3,3,24), y, useAsEasy);
            }
        
            // Add warning sign if level has been changed since player solved it
            if (scoreMgr->isObsolete(proxy, app.state->getInt("Difficulty")))
                blit(gc, x-2, y-2, img_obsolete);
            else if (scoreMgr->isOutdated(proxy, app.state->getInt("Difficulty")))
                blit(gc, x-2, y-2, img_outdated);
        
            // Add icon if worldrecord or par
            if (scoreMgr->bestScoreReached(proxy, app.state->getInt("Difficulty"))) {
                blit(gc, x+thumb_off(5,35,59), y+thumb_off(2,5,20),
                        (app.state->getInt("Difficulty") != DIFFICULTY_HARD &&
                        proxy->hasEasyMode()) ? img_wrEasy : img_wrDifficult);
            } else if (scoreMgr->parScoreReached(proxy, app.state->getInt("Difficulty"))){
                blit(gc, x+thumb_off(33,33,55), y+thumb_off(12,12,12), img_par);
            }
        } else {
            // Draw solved/changed icons on top of level preview
            if (isCross) 
                blit (gc, x+4, y+4, img_link);
            else
                blit (gc, x+4, y+4, img_copy);
        }
        return true;
    }
    
    void LevelWidget::draw(ecl::GC &gc, const ecl::Rect &r) {
        const VMInfo &vminfo = *video_engine->GetInfo();
        const int imgw = vminfo.thumb.width;  // Size of the preview images
        const int imgh = vminfo.thumb.height;
        const int bwidth = vminfo.thumb.border_width;

        const int hgap = Max(0, (get_w() - width*buttonw) / (width));
        const int vgap = Max(0, (get_h() - height*buttonh)/ (height-1));
    
        unsigned i=ifirst;          // level index
        bool allowGeneration = true;
    
        for (int y=0; y<height; y++) {
            for (int x=0; x<width; x++, i++) {
                if ((int)i >= curIndex->size())
                    goto done_painting;
    
                int xpos = get_x() + hgap/2 + x*(buttonw + hgap);
                int ypos = get_y() + y*(buttonh + vgap);
    
                Rect buttonarea(xpos, ypos, buttonw, buttonh);
                if (!(r.overlaps(buttonarea) || r.w == 0))
                    continue;       // r.w==0 if repainting whole screen
    
                if( (i-ifirst) >= m_areas.size()) {
                    m_areas.push_back(buttonarea);
                    pending_redraws.push_back(false);
                } else {
                    m_areas[(i-ifirst)] = buttonarea;
                }
                // Draw level preview
                lev::Proxy *levelProxy = curIndex->getProxy(i);
                int imgx = xpos + (buttonw-imgw)/2;
                int imgy = ypos + bwidth;
                if (levelProxy != NULL) {
                    bool didGenerate;
                    bool didDraw = draw_level_preview(gc, imgx, imgy, bwidth, levelProxy, 
                            (int) i == iselected, !curIndex->isSource(levelProxy), 
                            !curIndex->mayPlayLevel(i+1),
                            allowGeneration, didGenerate);
                    if (didGenerate) {
                        // do not generate more than 1 preview from level source
                        // per draw call
                        allowGeneration = false;
                    }
                    if (didDraw) {
                        pending_redraws[(i-ifirst)] = false;
                    } else {
                        // the button is not drawn - mark it to be drawn on
                        // a future tick
                        pending_redraws[(i-ifirst)] = true;
                        isInvalidateUptodate = false;
                    }
                    // Draw level name
                    Font *smallfnt = enigma::GetFont("levelmenu");
                    Font *altsmallfnt = enigma::GetFont("smallalternative");;
                    int maxwidth = buttonw + hgap;
                    std::string caption = levelProxy->getTitle();
                    std::vector<std::string> lines = ecl::breakToLines(smallfnt, caption, " ", maxwidth);
                    if ((lines.size() > 0) && !lines[0].empty())
                        smallfnt->render (gc,
                              xpos + buttonw/2 - ecl::Min(smallfnt->get_width(lines[0].c_str(), altsmallfnt)/2, maxwidth/2),
                              imgy + imgh + ((lines.size() == 1) ? 2 : 0),
                              lines[0], altsmallfnt, maxwidth);
                    if ((lines.size() > 1) && !lines[1].empty())
                        smallfnt->render (gc,
                              xpos + buttonw/2 - ecl::Min(smallfnt->get_width(lines[1].c_str(), altsmallfnt)/2, maxwidth/2),
                              imgy + imgh + smallfnt->get_height() - 3,
                              lines[1], altsmallfnt, maxwidth);
                }
            }
        }
        done_painting:
        m_areas.resize(i-ifirst); // Remove unused areas (if any) from the list
        return;
    }
    
    void LevelWidget::tick(double time) {
        if (!isInvalidateUptodate) {
            // invalidate just 1 button for redraw
            bool isFirst = true;
            for (unsigned i = 0; i < pending_redraws.size(); i++) {
                if (pending_redraws[i] == true) {
                    if (isFirst) {
                        invalidate_area(m_areas[i]);
                        isInvalidateUptodate = true;
                        isFirst = false;
                    } else {
                        isInvalidateUptodate = false;
                        return;
                    }
                }
            }
        }
    }
    
    bool LevelWidget::on_event(const SDL_Event &e) {
        bool handled = Widget::on_event(e);
        int mouse_x = 0;
        int mouse_y = 0;
    
        switch (e.type) {
            case SDL_MOUSEMOTION:
                mouse_x = (int)((double) (e.motion.x * SCREEN->size().w) / SCREEN->window_size().w + 0.5);
                mouse_y = (int)((double) (e.motion.y * SCREEN->size().h) / SCREEN->window_size().h + 0.5);
                if (get_area().contains(mouse_x, mouse_y)) {
                    int newsel=iselected;
                    for (unsigned i=0; i<m_areas.size(); ++i)
                        if (m_areas[i].contains(mouse_x, mouse_y))
                        {
                            newsel = ifirst+i;
                            break;
                        }
                    set_current(newsel);
                    handled = true;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_x = (int)((double) (e.button.x * SCREEN->size().w) / SCREEN->window_size().w + 0.5);
                mouse_y = (int)((double) (e.button.y * SCREEN->size().h) / SCREEN->window_size().h + 0.5);
                if (get_area().contains(mouse_x, mouse_y))
                    handled = handle_mousedown (&e);
                break;
            case SDL_MOUSEWHEEL:
                if (e.wheel.y < 0) {
                    scroll_up(1);
                    handled = true;
                }
                if (e.wheel.y > 0) {
                    scroll_down(1);
                    handled = true;
                }
                break;
            case SDL_KEYDOWN:
                handled = handle_keydown (&e);
                break;
        }
        syncToIndexMgr();
        return handled;
    }
    
    bool LevelWidget::handle_mousedown(const SDL_Event *e) {
        int mouse_x = (int)((double) (e->button.x * SCREEN->size().w) / SCREEN->window_size().w + 0.5);
        int mouse_y = (int)((double) (e->button.y * SCREEN->size().h) / SCREEN->window_size().h + 0.5);
        switch (e->button.button) {
            case SDL_BUTTON_LEFT:
                for (unsigned i=0; i<m_areas.size(); ++i)
                    if (m_areas[i].contains(mouse_x, mouse_y))
                    {
                        sound::EmitSoundEvent ("menuok");
                        iselected = ifirst+i;
                        syncToIndexMgr();
                        if (SDL_GetModState() & KMOD_CTRL && !(SDL_GetModState() & KMOD_SHIFT)) {
                            // control key pressed - level inspector
                            LevelInspector m(curIndex->getProxy(iselected));
                            m.manage();
                            get_parent()->draw_all();
                        } else {
                            // no control key - start level
                            trigger_action();
                        }
                        return true;
                    }
                break;
            case SDL_BUTTON_RIGHT: 
                for (unsigned i=0; i<m_areas.size(); ++i)
                    if (m_areas[i].contains(mouse_x, mouse_y))
                    {
                        sound::EmitSoundEvent ("menuok");
                        iselected = ifirst+i;
                        syncToIndexMgr();
                        LevelInspector m(curIndex->getProxy(iselected));
                        m.manage();
                        get_parent()->draw_all();
                        return true;
                    }
                break;
        }
        return false;
    }
    
    bool LevelWidget::handle_keydown(const SDL_Event *e) {
        switch (e->key.keysym.sym) {
            case SDLK_t:
                // Generate new level preview for current level
                preview_cache->updatePreview(curIndex->getProxy(iselected),
                                             video_engine->GetInfo()->thumb);
                invalidate();
                break;
            
            case SDLK_LEFT:  
                if (!(SDL_GetModState() & KMOD_ALT)) {
                    set_current (iselected>1 ? iselected-1 : 0); 
                    break;
                } else
                    return false;
            case SDLK_RIGHT:
                if (!(SDL_GetModState() & KMOD_ALT)) {
                     set_current (iselected+1);
                     break;
                } else
                    return false;
            case SDLK_DOWN:  set_current (iselected+width); break;
            case SDLK_UP:    set_current (iselected>width ? iselected-width : 0); break;
            case SDLK_PAGEDOWN: page_down(); break;
            case SDLK_PAGEUP: page_up(); break;
            case SDLK_HOME: start(); break;
            case SDLK_END: end(); break;
        
            case SDLK_RETURN:
                trigger_action();
                break;
        
            default:
                return false;           // key not handled
        }
        return true;
    }
}} // namespace enigma::gui
