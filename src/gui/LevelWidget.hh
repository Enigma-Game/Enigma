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
#ifndef GUI_LEVELWIDGET_HH_INCLUDED
#define GUI_LEVELWIDGET_HH_INCLUDED

#include "gui/widgets.hh"
#include "levels.hh"
#include <map>

namespace enigma { namespace gui {
    class LevelPreviewCache {
    public:
        LevelPreviewCache();
        ~LevelPreviewCache();

        void clear();
        void set_size(int xs, int ys);

        ecl::Surface *getPreview (const levels::Level &);
        ecl::Surface *makePreview (const levels::Level &);
        ecl::Surface *updatePreview (const levels::Level &);
    private:

        struct CacheElem {
            ecl::Surface  *surface;      // owned by ImageCache
            std::string  idx;          // level indexname
            
            CacheElem (ecl::Surface *s, std::string idx_)
            : surface (s), idx(idx_)
            {}
            
 //           bool operator<(const CacheElem& other) 
 //           { return idx<other.idx; }
        };
        
        typedef std::map<std::string, CacheElem*> PreviewMap;

        // ---------- Internal methods ----------

        ecl::Surface *newPreview (const Level &level);
        CacheElem *make_cache_elem (const levels::Level &level);
        void release();

        // ---------- Variables ----------

        PreviewMap         cache;
        enigma::ImageCache imgCache;
        int                xsize, ysize;
    };


/* -------------------- LevelWidget -------------------- */

    class LevelMenu;

    class LevelWidget : public Widget {
    public:
        LevelWidget(levels::LevelPack *lp,
                    LevelPreviewCache &cache);
        bool manage ();

        //---------- Widget interface ----------//
        void draw(ecl::GC &gc, const ecl::Rect &r);

        void set_listener(ActionListener *al) {
            listener = al;
        }
        void trigger_action();

        virtual void realize (const ecl::Rect &r);

        //---------- Cursor motion ----------//

        void page_up();
        void page_down();
        void start();
        void end();

        void set_current (size_t newsel);
        void next_unsolved();

        bool on_event(const SDL_Event &e);
        int get_position() const;
        void set_position(int pos);

        //---------- LevelMenu interaction ----------//

        int selected_level() const { return iselected; }
        LevelMenu *get_menu();
        void show_text(const string& text);
        void change_levelpack (levels::LevelPack *lp);

    private:
        //---------- Private functions ----------//
        void scroll_up(int lines);
        void scroll_down(int lines);
        void set_selected (size_t newfirst, size_t newsel);
        void recalc_available ();
        ecl::Surface *get_preview_image (const Level &);
        void draw_level_preview (ecl::GC &, const Level &, int x, int y);

        bool handle_keydown (const SDL_Event *e);
        bool handle_mousedown (const SDL_Event *e);

        //---------- Variables ----------//
        ImageCache         cache;
        LevelPreviewCache  &preview_cache;
        levels::LevelPack *level_pack; // The current level pack

        size_t             ifirst; // Index of "upper left" level
        size_t             iselected; // Index of selected level
        int                max_available; // Index of the last available level (one can choose out of x unsolved levels)
        int                width, height;
        std::vector<ecl::Rect>  m_areas; // Screen areas occupied by level previews
        ActionListener    *listener;
        int                buttonw, buttonh;
    };

}} // namespace enigma::gui
#endif
