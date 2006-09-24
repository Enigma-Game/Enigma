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
 
#include "gui/LevelWidget.hh"
#include "gui/LevelMenu.hh"
#include "gui/LevelInspector.hh"

#include "ecl.hh"
#include "main.hh"
#include "nls.hh"
#include "options.hh"
#include "sound.hh"
#include "StateManager.hh"
#include "video.hh"
#include "file.hh"
#include "lev/Proxy.hh"
#include <cassert>

using namespace ecl;
using namespace std;


namespace enigma { namespace gui {    
    /* -------------------- LevelWidget -------------------- */
    
    LevelWidget::LevelWidget()
    : width (0),
      height (0),
      m_areas(),
      listener(0)
    {
        const video::VMInfo &vminfo = *video::GetInfo();
    
        buttonw = vminfo.thumbw + 20;
        buttonh = vminfo.thumbh + 28;
        curIndex = lev::Index::getCurrentIndex();
        iselected = curIndex->getCurrentPosition();
        ifirst = curIndex->getScreenFirstPosition();
        preview_cache = LevelPreviewCache::instance();
        scoreMgr = lev::ScoreManager::instance();
        img_easy        = enigma::GetImage("completed-easy");
        img_hard        = enigma::GetImage("completed");
        img_changed     = enigma::GetImage("changed");
        img_unavailable = enigma::GetImage("unavailable");
//        img_unknown     = enigma::GetImage("unknown");
        img_par         = enigma::GetImage("par");
        img_border      = enigma::GetImage("thumbborder");
    }
    
    void LevelWidget::syncFromIndexMgr() {
        if (curIndex != lev::Index::getCurrentIndex()) {
            curIndex = lev::Index::getCurrentIndex();
            iselected = curIndex->getCurrentPosition();
            ifirst = curIndex->getScreenFirstPosition();
            invalidate();
            sound::SoundEvent ("menumove");
        } else if (iselected != curIndex->getCurrentPosition()) {
            sound::SoundEvent ("menumove");            
        }
        // repair ifirst on boot and screen resolution changes
        set_selected(curIndex->getScreenFirstPosition(), 
                curIndex->getCurrentPosition());
    }

    void LevelWidget::syncToIndexMgr() {
        curIndex->setCurrentPosition(iselected);
        curIndex->setScreenFirstPosition(ifirst);
    }
        
    void LevelWidget::realize (const ecl::Rect &area_)
    {
        Widget::realize (area_);
        width = area_.w / buttonw;
        height = area_.h / buttonh;
    }
    
    void LevelWidget::trigger_action() {
        if (listener) {
            listener->on_action(this);
        }
    }
        
    void LevelWidget::scroll_up (int nlines) 
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
        set_selected (newFirst, newSelected);
    }
    
    void LevelWidget::scroll_down(int nlines) 
    {
        int newFirst = ifirst;
        int newSelected = iselected;
        for (; nlines; --nlines) {
            if (newFirst < width)
                break;
            newFirst -= width;
            if (newSelected >= newFirst+width*height)
                newSelected -= width;
        }
        set_selected (newFirst, newSelected);
    }
    
    void LevelWidget::page_up() {
        set_selected ((ifirst >= width*height ? ifirst - width*height : 0), 
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
        set_selected (first, iselected + width*height);
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
    
    void LevelWidget::set_current (int newsel)
    {
        set_selected (ifirst, newsel);
    }
    
    void LevelWidget::set_selected (int newfirst, int newsel)
    {
        int numlevels = curIndex->size();
        newsel = Clamp<int> (newsel, 0, numlevels-1);
        if (newsel < 0) newsel = 0;
    
        if (newsel < newfirst)
            newfirst = (newsel/width)*width;
        if (newsel >= newfirst+width*height)
            newfirst = (newsel/width-height+1)*width;
    
        newfirst = Clamp<int> (newfirst, 0, numlevels-1);
        if (newfirst < 0) newfirst = 0;
    
        size_t oldsel = iselected;
        if (newfirst != ifirst) {
            ifirst    = newfirst;
            iselected = newsel;
    
            if (!m_areas.empty()) {
                sound::SoundEvent ("menumove");
                if (oldsel != newsel) 
                    sound::SoundEvent ("menuswitch");
                invalidate();
            }
        }
        else if (newsel != iselected) {
            iselected = newsel;
    
            if (!m_areas.empty()) {
                sound::SoundEvent ("menuswitch");
                invalidate_area(m_areas[oldsel-ifirst]); // old selection
                invalidate_area(m_areas[iselected-ifirst]); // new selection
            }
        }
    }
                
    void LevelWidget::draw_level_preview (ecl::GC &gc, int x, int y, 
            lev::Proxy *proxy, bool selected, bool showScore, bool locked) 
    {
        // Draw button with level preview
    
        Surface *img = preview_cache->getPreview(proxy);
   
        if (selected) {
            blit (gc, x-4, y-4, img_border);
            blit (gc, x, y, img);
        }
        else {
            img->set_alpha (127);
            blit (gc, x, y, img);
            img->set_alpha(255);
        }

        // Shade unavailable levels
        if (locked)
            blit (gc, x, y, img_unavailable);
    
        if (showScore) {
            // Draw solved/changed icons on top of level preview
            if (app.state->getInt("Difficulty") == DIFFICULTY_HARD) {
                // draw golden medal over silber medal
                if (scoreMgr->isSolved(proxy, DIFFICULTY_EASY))
                    blit (gc, x, y, img_easy);
                if (scoreMgr->isSolved(proxy, DIFFICULTY_HARD))
                    blit (gc, x+5, y, img_hard);
            }
            else {
                // draw silver medal over golden medal
                if (scoreMgr->isSolved(proxy, DIFFICULTY_HARD))
                    blit (gc, x+5, y, img_hard);
                if (scoreMgr->isSolved(proxy, DIFFICULTY_EASY))
                    blit (gc, x, y, img_easy);
            }
        
            // Add warning sign if level has been changed since player solved it
            if (scoreMgr->isOutdated(proxy, app.state->getInt("Difficulty")))
                blit (gc, x-3, y-3, img_changed);
        
            // Add time icon if par has been reached
            if (scoreMgr->bestScoreReached(proxy, app.state->getInt("Difficulty")))
                blit(gc, x+30, y+12, img_par);
        }
    
    }
    
    void LevelWidget::draw (ecl::GC &gc, const ecl::Rect &r)
    {
        const video::VMInfo &vminfo = *video::GetInfo();
        const int imgw = vminfo.thumbw;       // Size of the preview images
        const int imgh = vminfo.thumbh;
    
        const int hgap = Max(0, (get_w() - width*buttonw) / (width-1));
        const int vgap = Max(0, (get_h() - height*buttonh)/ (height-1));
    
        unsigned i=ifirst;          // level index
    
        for (int y=0; y<height; y++)
        {
            for (int x=0; x<width; x++, i++)
            {
                if (i >= curIndex->size())
                    goto done_painting;
    
                int xpos = get_x() + x*(buttonw + hgap);
                int ypos = get_y() + y*(buttonh + vgap);
    
                Rect buttonarea(xpos, ypos, buttonw, buttonh);
                if (!(r.overlaps(buttonarea) || r.w == 0))
                    continue;       // r.w==0 if repainting whole screen
    
                if( (i-ifirst) >= m_areas.size())
                    m_areas.push_back(buttonarea);
                else
                    m_areas[(i-ifirst)] = buttonarea;
    
                // Draw level preview
                lev::Proxy *levelProxy = curIndex->getProxy(i);
                int imgx = xpos+(buttonw-imgw)/2;
                int imgy = ypos + 4;
                if (levelProxy != NULL) {
                    draw_level_preview (gc, imgx, imgy, levelProxy, 
                            i == iselected, true, !curIndex->mayPlayLevel(i+1));
                }
                // Draw level name
                Font    *smallfnt = enigma::GetFont("levelmenu");
                const char *caption = levelProxy->getTitle().c_str();
                smallfnt->render (gc,
                                  xpos + (buttonw-smallfnt->get_width(caption))/2,
                                  imgy + imgh,
                                  caption);
            }
        }
      done_painting:
        m_areas.resize (i-ifirst); // Remove unused areas (if any) from the list
        return;
    }
    
    bool LevelWidget::on_event(const SDL_Event &e) 
    {
        bool handled = Widget::on_event(e);
    
        switch (e.type) {
        case SDL_MOUSEMOTION:
            if (get_area().contains(e.motion.x, e.motion.y)) {
                int newsel=iselected;
                for (unsigned i=0; i<m_areas.size(); ++i)
                    if (m_areas[i].contains(e.motion.x, e.motion.y))
                    {
                        newsel = ifirst+i;
                        break;
                    }
                set_current(newsel);
                handled = true;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (get_area().contains(e.button.x, e.button.y))
                handled = handle_mousedown (&e);
            break;
        case SDL_KEYDOWN:
            handled = handle_keydown (&e);
            break;
        }
        syncToIndexMgr();
        return handled;
    }
    
    bool LevelWidget::handle_mousedown (const SDL_Event *e) 
    {
        switch (e->button.button) {
        case SDL_BUTTON_LEFT:
            for (unsigned i=0; i<m_areas.size(); ++i)
                if (m_areas[i].contains(e->button.x, e->button.y))
                {
                    sound::SoundEvent ("menuok");
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
                if (m_areas[i].contains(e->button.x, e->button.y))
                {
                    sound::SoundEvent ("menuok");
                    iselected = ifirst+i;
                    syncToIndexMgr();
                    LevelInspector m(curIndex->getProxy(iselected));
                    m.manage();
                    get_parent()->draw_all();
                    return true;
                }
            break;
        case 4: scroll_down(1); return true;
        case 5: scroll_up(1); return true;
        }
        return false;
    }
    
    bool LevelWidget::handle_keydown (const SDL_Event *e)
    {
        switch (e->key.keysym.sym) {
    
        case SDLK_t:
            // Generate new level preview for current level
            preview_cache->updatePreview(curIndex->getProxy(iselected));
            invalidate();
            break;
        
        case SDLK_LEFT:  set_current (iselected>1 ? iselected-1 : 0); break;
        case SDLK_RIGHT: set_current (iselected+1); break;
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
