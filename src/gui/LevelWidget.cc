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
#include "gui/HelpMenu.hh"
#include "ecl.hh"
#include "main.hh"
#include "nls.hh"
#include "options.hh"
#include "oxyd.hh"
#include "sound.hh"
#include "video.hh"

#include <cassert>

using namespace ecl;
using namespace std;

using enigma::levels::LevelStatus;
using enigma::levels::LevelInfo;
using enigma::levels::LevelPack;
using enigma::levels::LevelPacks;

namespace enigma { namespace gui {
    void SavePreview (std::string preview_name, ecl::Surface *s)
    {
        string preview_path;
        if (char *home = getenv ("HOME")) {
            preview_path = home;
            preview_path += "/.enigma/thumbs";
            if (!ecl::FolderExists (preview_path))
                ecl::FolderCreate (preview_path);
        }
        else {
        // to be eliminated! Do not write on system path
            string preview_dir_path = app.systemFS->findFile("thumbs/README");
            ecl::split_path(preview_dir_path, &preview_path, 0);
        }
        
        string parent_dir;
        if (ecl::split_path(preview_path, &parent_dir, 0)) {
            preview_name = ecl::concat_paths(parent_dir, preview_name);
            enigma::Log << "Caching auto-preview as '" << preview_name << '\'' << endl;
            ecl::SavePNG (s, preview_name);
        }
    }

    /** Remove non-alphanumeric characters from `s'. */
    string remove_bad_chars(const string& s) 
    {
        string s2 = s;
        for (string::iterator c = s2.begin(); c != s2.end(); ++c) {
            if (!isalnum(*c)) *c = '_';
        }
        return s2;
    }

    string gen_preview_name(int revision, int preview_version, const string& name) 
    {
        return ecl::strf("thumbs/%i_%i_%s.png", revision, preview_version, name.c_str());
    }


    string MakePreviewName (const levels::Level &l)
    {
        levels::LevelPack *lp = l.get_levelpack();
        
        string cleanedName     = remove_bad_chars (l.get_info().filename);
        int    rev_number      = l.get_revision();
        int    preview_version = lp->get_preview_version();
        
        return gen_preview_name(rev_number, preview_version, cleanedName);
    }

    void remove_old_previews (const levels::Level &level)
    {
        levels::LevelPack *lp = level.get_levelpack();
        string name           = remove_bad_chars (level.get_info().filename);
        int    revision       = level.get_revision();
        int    preview_version = lp->get_preview_version();

        for (int r = revision; r >= 1; --r) {
            for (int v = preview_version; v >= 1; --v) {
                string preview_name = gen_preview_name(r, v, name);
                string found_name;
                if (app.resourceFS->findFile(preview_name, found_name)) {
                    if (remove(found_name.c_str()) == -1) {
                        enigma::Log << "Error deleting outdated preview '" << found_name << '\'' << endl;
                    }
                    else {
                        enigma::Log << "Deleting outdated preview '" << found_name << '\'' << endl;
                    }
                }
            }
        }
    }

    
    /* -------------------- LevelPreviewCache -------------------- */
    
    LevelPreviewCache::LevelPreviewCache() 
    : xsize(0), ysize(0) 
    {}
    
    LevelPreviewCache::~LevelPreviewCache() 
    { 
        release(); 
    }
    
    void LevelPreviewCache::release() 
    {
        delete_map (cache.begin(), cache.end());
    }
    
    void LevelPreviewCache::clear() 
    { 
        release(); 
        cache.clear(); 
        imgCache.clear();
    }
    
    void LevelPreviewCache::set_size(int xs, int ys)
    {
        xsize = xs; 
        ysize = ys; 
    }
    
    Surface *LevelPreviewCache::getPreview(const levels::Level &level)
    {
        PreviewMap::iterator i = cache.find(level.uniqueName());
        return  (i != cache.end()) ? i->second->surface : 0;
    }
    
    Surface *LevelPreviewCache::makePreview(const levels::Level &level)
    {
        if (Surface *s = getPreview (level))
            return s;
    
        assert(xsize != 0 && ysize != 0); // forgot to call set_size() ?
    
        CacheElem *ce = make_cache_elem (level);
        if (ce)
            cache[level.uniqueName()] = ce;
        return ce->surface;
    }
    
    ecl::Surface *LevelPreviewCache::newPreview (const Level &level)
    {
        Surface *surface = 0;
        ecl::GC gc(video::BackBuffer());
        if (game::DrawLevelPreview (gc, level)) { 
            surface = Resample (video::BackBuffer(), 
                                video::GetInfo()->gamearea, 
                                xsize, ysize); 
        }
        return surface;
    }
    
    Surface *LevelPreviewCache::updatePreview (const levels::Level &level)
    {
        if (Surface *surface = newPreview (level)) 
        {
            std::string idx = level.uniqueName();
            PreviewMap::iterator i = cache.find (idx);
            if (i != cache.end())
                delete i->second;
            cache[idx] = new CacheElem (surface, idx);
    
            string preview_name = MakePreviewName (level);
            imgCache.store (preview_name, surface);
            remove_old_previews (level);
            SavePreview (preview_name, surface);
            return surface;
        }
        return 0;
    }
    
    
    LevelPreviewCache::CacheElem *
    LevelPreviewCache::make_cache_elem (const levels::Level &level)
    {
        string preview_name = MakePreviewName (level);
        Surface *surface = 0;
    
        string preview_path;
        if (app.resourceFS->findFile(preview_name, preview_path))
            surface = imgCache.get(preview_path);
    
        if (!surface) {
            surface = newPreview (level);
            if (surface) {
                imgCache.store (preview_name, surface);
                remove_old_previews (level);
                SavePreview (preview_name, surface);
            }
            else 
                surface = enigma::GetImage ("error");
        }
    
        if (surface)
            return new CacheElem (surface, level.uniqueName());
        return 0;
    }
    
    
    /* -------------------- LevelWidget -------------------- */
    
    LevelWidget::LevelWidget(LevelPack *lp, LevelPreviewCache &cache_)
    : preview_cache (cache_),
      level_pack(lp),
      ifirst(0), iselected(0),
      width (0),
      height (0),
      m_areas(),
      listener(0)
    {
        const video::VMInfo &vminfo = *video::GetInfo();
    
        buttonw = vminfo.thumbw + 20;
        buttonh = vminfo.thumbh + 28;
    }
    
    void LevelWidget::realize (const ecl::Rect &area_)
    {
        Widget::realize (area_);
        width = area_.w / buttonw;
        height = area_.h / buttonh;
    }
    
    void LevelWidget::trigger_action() {
        cache.clear();
        if (listener) {
            listener->on_action(this);
            recalc_available();
        }
    }
    
    LevelMenu * LevelWidget::get_menu() {
        if (Widget *m = Widget::get_parent())
            return dynamic_cast<LevelMenu*>(m);
        else {
            assert(0);
            return 0;
        }
    }
    
    void LevelWidget::show_text(const string& text) {
        get_menu()->show_text(text);
    }
    
    void LevelWidget::scroll_up (int nlines) 
    {
        for (; nlines; --nlines) {
            if (ifirst+width*height >= level_pack->size())
                break;
            ifirst += width;
            if (iselected < ifirst)
                iselected += width;
        }
        invalidate();
    }
    
    void LevelWidget::scroll_down(int nlines) 
    {
        for (; nlines; --nlines) {
            if (ifirst < width)
                break;
            ifirst -= width;
            if (iselected >= ifirst+width*height)
                iselected -= width;
        }
        invalidate();
    }
    
    void LevelWidget::page_up() {
        set_selected ((ifirst >= width*height ? ifirst - width*height : 0), 
                (iselected >= width*height ? iselected - width*height : 0));
    }
    
    void LevelWidget::page_down() 
    {
        unsigned s = level_pack->size();
        size_t lastPage = (s >= width*height ? (s / width - height + 1) * width : 0);
    
        // make sure last page is shown as a whole
        size_t first = std::min<size_t> (lastPage, ifirst + width*height);
        //    set_selected (first, s-1);
        set_selected (first, iselected + width*height);
    }
    
    void LevelWidget::start() { 
        set_selected (0,0); 
    }
    
    void LevelWidget::end() 
    {
        unsigned s = level_pack->size();
        size_t lastPage = (s >= width*height ? (s / width - height + 1) * width : 0);
        set_selected (lastPage, s-1);
    }
    
    void LevelWidget::recalc_available() {
        max_available = HighestAvailableLevel(level_pack);
    }
    
    void LevelWidget::change_levelpack (LevelPack *lp) {
        iselected  = 0;
        ifirst     = 0;
        level_pack = lp;
        oxyd::ChangeSoundset(options::GetInt("SoundSet"), level_pack->get_default_SoundSet());
        cache.clear();
        preview_cache.clear();
        recalc_available();
        invalidate();
        sound::SoundEvent ("menumove");
    }
    
    void LevelWidget::next_unsolved() 
    {
        Level level (level_pack, iselected);
    
        if (levels::advance_level (level, levels::advance_unsolved))
            set_current(level.get_index());
        else
            show_text(_("No further unsolved level available!"));
    }
    
    Surface *LevelWidget::get_preview_image (const Level &level)
    {
        Surface *img_error       = enigma::GetImage("error");
    
        const LevelInfo &levelinfo = level.get_info();
    
        Surface *img = 0;
        if (level_pack->may_have_previews()) {
            string fname = string("levels/") + levelinfo.filename + ".png";
            if (app.resourceFS->findFile (fname, fname))
                img = cache.get(fname);
        }
        if (!img) {
            img = preview_cache.makePreview (level);
            if (img == 0)
                img = img_error;
        }
        return img;
    }
    
    void LevelWidget::draw_level_preview (ecl::GC &gc, const Level &level, int x, int y)
    {
        Surface *img_easy        = enigma::GetImage("completed-easy");
        Surface *img_hard        = enigma::GetImage("completed");
        Surface *img_changed     = enigma::GetImage("changed");
        Surface *img_unavailable = enigma::GetImage("unavailable");
    //    Surface *img_unknown     = enigma::GetImage("unknown");
        Surface *img_par         = enigma::GetImage("par");
        Surface *img_border      = enigma::GetImage("thumbborder");
    
        const LevelInfo &levelinfo = level.get_info();
    
        // Draw button with level preview
    
        Surface *img = get_preview_image(level);
    //        img = img_unknown;
    
        if (level.get_index() == (unsigned) iselected) {
            blit (gc, x-4, y-4, img_border);
            blit (gc, x, y, img);
        }
        else {
            img->set_alpha (127);
            blit (gc, x, y, img);
            img->set_alpha(255);
        }
    
        // Draw additional icons
    
        int  finished      = 0;
        bool level_changed = false;
        bool par_reached   = false;
    
        LevelStatus levelstat;
        if (level.get_status(levelstat)) {
            finished = levelstat.finished;
            if (finished) {
                level_changed = level.get_revision() > levelstat.solved_revision;
    
                int par_time = level.get_par_time (options::GetDifficulty());
                int best_user_time = level.get_best_user_time (options::GetDifficulty());
                par_reached = best_user_time>0 && (par_time<0 || best_user_time <= par_time);
            }
        }
    
        // Shade unavailable levels
        if (LevelIsLocked(level))
            blit (gc, x, y, img_unavailable);
    
        // Draw solved/changed icons on top of level preview
        if (finished) {
            if (!levelinfo.has_easymode)
                finished = DIFFICULTY_EASY | DIFFICULTY_HARD;
            if (options::GetDifficulty () == DIFFICULTY_HARD) {
                if (finished & DIFFICULTY_EASY)
                    blit (gc, x, y, img_easy);
                if (finished & DIFFICULTY_HARD)
                    blit (gc, x+5, y, img_hard);
            }
            else {
                if (finished & DIFFICULTY_HARD)
                    blit (gc, x+5, y, img_hard);
                if (finished & DIFFICULTY_EASY)
                    blit (gc, x, y, img_easy);
            }
        }
    
        // Add warning sign if level has been changed since player solved it
        if (level_changed)
            blit (gc, x-3, y-3, img_changed);
    
        // Add time icon if par has been reached
        if (par_reached)
            blit(gc, x+30, y+12, img_par);
    
    }
    
    struct LevelWidgetConfig {
        int imgw, imgh;             // size of preview images
        int imgpadx, imgpady;       // padding of image buttons
        const char *fontname;
    };
    
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
                if (i >= level_pack->size())
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
                Level level (level_pack, i);
                int imgx = xpos+(buttonw-imgw)/2;
                int imgy = ypos + 4;
                draw_level_preview (gc, level, imgx, imgy);
                                    
                // Draw level name
                Font    *smallfnt = enigma::GetFont("levelmenu");
                const char *caption = level.get_info().name.c_str();
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
    
    void LevelWidget::set_current (size_t newsel)
    {
        set_selected (ifirst, newsel);
    }
    
    void LevelWidget::set_selected (size_t newfirst, size_t newsel)
    {
        // check for implicit argument convertions of negativ integers to unsigned
        assert(newfirst < 1000000);
        assert(newsel < 1000000);
        
        size_t numlevels = level_pack->size();
        newsel = Clamp<size_t> (newsel, 0, numlevels-1);
    
        if (newsel < newfirst)
            newfirst = (newsel/width)*width;
        if (newsel >= newfirst+width*height)
            newfirst = (newsel/width-height+1)*width;
    
        newfirst = Clamp<size_t> (newfirst, 0, numlevels-1);
    
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
    
    bool LevelWidget::on_event(const SDL_Event &e) 
    {
        bool handled = false;
    
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
        return handled;
    }
    
    bool LevelWidget::handle_mousedown (const SDL_Event *e) 
    {
        switch (e->button.button) {
        case SDL_BUTTON_LEFT:
            for (unsigned i=0; i<m_areas.size(); ++i)
                if (m_areas[i].contains(e->button.x, e->button.y))
                {
                    if (SDL_GetModState() & KMOD_CTRL) {
                        // control key pressed - level inspector
                        Level level (level_pack,ifirst+i);
                        lev::Proxy *levelProxy = level.get_info().proxy;
                        LevelInspector m(levelProxy, get_preview_image(level));
                        m.manage();
                        get_menu()->draw_all();
                    } else {
                        // no control key - start level
                        sound::SoundEvent ("menuok");
                        iselected = ifirst+i;
                        trigger_action();
                    }
                    return true;
                }
            break;
        case SDL_BUTTON_RIGHT: 
            for (unsigned i=0; i<m_areas.size(); ++i)
                if (m_areas[i].contains(e->button.x, e->button.y))
                {
                    Level level (level_pack,ifirst+i);
                    lev::Proxy *levelProxy = level.get_info().proxy;
                    LevelInspector m(levelProxy, get_preview_image(level));
                    m.manage();
                    get_menu()->draw_all();
                    return true;
                }
            break;
        case 4: scroll_down(1); return true;
        case 5: scroll_up(1); return true;
        }
        return false;
    }
    
    static const char *helptext_levelmenu[] = {
        N_("Escape:"),              N_("Skip to main menu"),
        "F1:",                      N_("Show this help"),
        "F5:",                      0, // see below
        N_("Arrows:"),              N_("Select level"),
        N_("Return:"),              N_("Play selected level"),
        N_("Back/Space:"),          N_("Previous/next levelpack"),
        "u",                        N_("Mark current level as Unsolved"),
    //    "s",                        N_("Mark current level as Solved"),
        N_("Alt+Return:"),          N_("Switch between fullscreen and window"),
        0
    };
    
    bool LevelWidget::handle_keydown (const SDL_Event *e)
    {
        Level level(level_pack, iselected);
        switch (e->key.keysym.sym) {
        case SDLK_u:
            level.mark_unsolved(); 
            invalidate(); 
            break;
    
        case SDLK_s:
            if (enigma::WizardMode) {
                level.mark_solved();
                invalidate();
            }
            break;
    
        case SDLK_t:
            // Generate new level preview for current level
            preview_cache.updatePreview (level);
            invalidate();
            break;
    
        case SDLK_LESS:
        case SDLK_MINUS:
            if  (level_pack->swap (iselected, iselected-1)) {
                set_current (iselected-1);
                invalidate();
            }
            break;
    
        case SDLK_GREATER:
        case SDLK_PLUS:
            if (level_pack->swap (iselected, iselected+1)) {
                set_current (iselected+1);
                invalidate();
            }
            break;
    
        case SDLK_LEFT:  set_current (iselected>1 ? iselected-1 : 0); break;
        case SDLK_RIGHT: set_current (iselected+1); break;
        case SDLK_DOWN:  set_current (iselected+width); break;
        case SDLK_UP:    set_current (iselected>width ? iselected-width : 0); break;
        case SDLK_PAGEDOWN: page_down(); break;
        case SDLK_PAGEUP: page_up(); break;
        case SDLK_HOME: start(); break;
        case SDLK_END: end(); break;
    
        case SDLK_F1: {
            if (options::GetBool("TimeHunting"))
                helptext_levelmenu[5] = N_("Select next non-par level");
            else
                helptext_levelmenu[5] = N_("Select next unsolved level");
    
            displayHelp(helptext_levelmenu, 200);
            get_menu()->draw_all();
            break;
        }
        case SDLK_F5:
            next_unsolved();
            break;
    
        case SDLK_RETURN:
            trigger_action();
            break;
    
        default:
            return false;           // key not handled
        }
        return true;
    }
    
    int LevelWidget::get_position() const 
    {
        return (ifirst << 8) | (iselected-ifirst);
    }
    
    void LevelWidget::set_position(int pos) 
    {
        int new_first = pos >> 8;
        int off       = pos & 0xff;
        int new_sel   = new_first+off;
        if (new_sel > width*height)
            set_current (new_sel);
        else
            set_selected(new_first, new_sel);
    }

}} // namespace enigma::gui
