/*
 * Copyright (C) 2003 Daniel Heck
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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id: menus_internal.hh,v 1.12 2004/05/22 13:04:30 dheck Exp $
 */

#include <png.h>
#include <iostream>

/* -------------------- Helper classes -------------------- */
namespace
{
    class BuildVList {
        Rect r;
        Menu *container;
        int skip;
    public:
        BuildVList(Menu *cc, const px::Rect &rr, int s)
            : r(rr), container(cc), skip(s)
        {}

        Widget *add(Widget *w) {
            container->add(w, r);
            r.y += r.h+skip;
            return w;
        }

        Rect pos() const { return r; }
    };

    class VTableBuilder {
        Menu     *m_menu;
        px::Rect  m_targetarea;
        px::Rect  m_widgetsize;
        int       m_vspacing, m_hspacing;
        
    public:
        VTableBuilder (Menu *menu, const px::Rect &targetarea, const px::Rect &widgetsize,
                       int   vspacing, int hspacing)
            : m_menu(menu),
              m_targetarea(targetarea),
              m_widgetsize(widgetsize),
              m_vspacing(vspacing),
              m_hspacing(hspacing)
        {}

        bool finish(vector<Widget*> widgets) {
            // if finish returns false, no widgets have been added to the menu
            // because the layout failed.

            if (widgets.empty())
                return true;

            int count       = widgets.size();
            int max_columns = (m_targetarea.w+m_vspacing) / (m_widgetsize.w+m_vspacing);
            int max_rows    = (m_targetarea.h+m_hspacing) / (m_widgetsize.h+m_hspacing);
            int min_columns = ((count-1) / max_rows)+1;
            int min_rows    = ((count-1) / max_columns)+1;
            
            if (min_columns>max_columns || min_rows>max_rows)
                return false;   // layout not possible

            double aspect_ratio = fabs(max_columns/static_cast<double>(max_rows));
            double best_diff    = 1000; // really bad
            int    best_free    = 20; // really bad
            int    best_cols    = -1;
            int    best_rows    = -1;

            for (int cols = min_columns; cols <= max_columns; ++cols)
                for (int rows = min_rows; rows <= max_rows; ++rows) {
                    int elements = rows*cols;
                    int Free     = elements-count;

                    if (Free >= 0) {
                        double curr_aspect_ratio = fabs(cols/static_cast<double>(rows));
                        double curr_diff         = fabs(aspect_ratio-curr_aspect_ratio);
                        
                        if (Free<best_free || (Free == best_free && curr_diff<best_diff)) {
                            best_diff = curr_diff;
                            best_free = Free;
                            best_cols = cols;
                            best_rows = rows;
                        }
                    }
                }

            if (best_cols == -1)
                return false;

            // layout succeeded -> add widgets to Menu

            int i=0;
            int x=0;
            for (int col = 0; col < best_cols && i<count; col++) {
                int y=0;
                for (int row=0; row < best_rows && i<count; row++, i++) {
                    px::Rect rr(x, y, m_widgetsize.w, m_widgetsize.h);
                    m_menu->add(widgets[i], rr);
                    // widgets[i]->move (x, y);
                    widgets[i]  = 0; // widget now owned by m_menu
                    y            += m_widgetsize.h + m_vspacing;
                }
                x += m_widgetsize.w + m_hspacing;
            }
            return true;
        }
    };

    class BuildHList {
        Rect r;
        Menu *container;
        int skip;
    public:
        BuildHList(Menu *cc, const px::Rect &rr, int s)
        : r(rr), container(cc), skip(s)
        {}

        Widget * add(Widget *w) {
            container->add(w, r);
            r.x += r.w+skip;
            return w;
        }
        Widget *add (Widget *w, int width) {
            px::Rect rr(r.x, r.y, width, r.h);
            container->add(w, rr);
            r.x += width + skip;
            return w;
        }

        Rect pos() const { return r; }
    };
}

namespace
{
    class LevelPreviewCache {
    public:
        LevelPreviewCache();
        ~LevelPreviewCache();

        void clear();
        void set_size(int xs, int ys);

        Surface *getPreview (const levels::Level &);
        Surface *makePreview (const levels::Level &);
        Surface *updatePreview (const levels::Level &);
    private:

        struct CacheElem {
            Surface  *surface;      // owned by ImageCache
            unsigned  idx;          // level index
            
            CacheElem (px::Surface *s, unsigned idx_)
            : surface (s), idx(idx_)
            {}
            
            bool operator<(const CacheElem& other) 
            { return idx<other.idx; }
        };
        
        typedef std::map<unsigned, CacheElem*> PreviewMap;

        // ---------- Internal methods ----------

        px::Surface *newPreview (const Level &level);
        CacheElem *make_cache_elem (const levels::Level &level);
        void release();

        // ---------- Variables ----------

        PreviewMap         cache;
        enigma::ImageCache imgCache;
        int                xsize, ysize;
    };

/* -------------------- LevelPackMenu -------------------- */

    class LevelPackMenu : public Menu {
    public:
        LevelPackMenu();

        void on_action(Widget *w);
        void draw_background(px::GC &gc);
        int get_selection() const { return m_selection; }

    private:
        vector<Widget *> buttons;
        int m_selection;
    };


    class LevelMenu;

/* -------------------- LevelWidget -------------------- */

    class LevelWidget : public Widget {
    public:
        LevelWidget(levels::LevelPack *lp,
                    LevelPreviewCache &cache);
        bool manage ();

        //---------- Widget interface ----------//
        void draw(px::GC &gc, const px::Rect &r);

        void set_listener(ActionListener *al) {
            listener = al;
        }
        void trigger_action();

        virtual void realize (const px::Rect &r);

        //---------- Cursor motion ----------//

        void page_up();
        void page_down();
        void start();
        void end();

        void set_current(int newsel) { set_selected(ifirst, newsel); }
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
        void set_selected (int newfirst, int newsel);
        void recalc_available ();
        Surface *get_preview_image (const Level &);
        void draw_level_preview (px::GC &, const Level &, int x, int y);

        bool handle_keydown (const SDL_Event *e);
        bool handle_mousedown (const SDL_Event *e);

        //---------- Variables ----------//
        ImageCache         cache;
        LevelPreviewCache  &preview_cache;
        levels::LevelPack *level_pack; // The current level pack

        int               ifirst; // Index of "upper left" level
        int               iselected; // Index of selected level
        int               max_available; // Index of the last available level (one can choose out of x unsolved levels)
        int               width, height;
        vector<px::Rect>  m_areas; // Screen areas occupied by level previews
        ActionListener   *listener;
        int buttonw, buttonh;
    };

/* -------------------- LevelMenu -------------------- */

    class LevelMenu : public Menu {
    public:
        LevelMenu(levels::LevelPack *lp, unsigned long pos);

        // Rotate through levelpacks
        void next_levelpack();
        void previous_levelpack();
        int get_position() const;
        void set_position(int pos);
        void show_text(const string& text);

    private:
        void update_info();

        void set_levelpack (unsigned index);

        // Menu interface.
        void tick (double time);
        void draw_background(px::GC &gc);

        // Widget interface.
        bool on_event (const SDL_Event &e);

        // ActionListener interface.
        void on_action(Widget *w);

        // Variables.

        LevelPreviewCache preview_cache;

        Widget *pgup, *pgdown, *start, *end;
	Widget	    *but_unsolved;	    // Next unsolved level button
        Widget      *but_tournament;
        Widget      *but_back;		    // "Back" button
        Widget	    *but_difficulty;	    // "Difficulty" button
        TextButton  *but_levelpack;	    // "Levelpack" button
        Label       *lbl_lpinfo;	    // Levelpack information
	Label	    *lbl_statistics;	    // percentage solved
        Label       *lbl_levelname;
	Label	    *lbl_levelinfo;
        LevelWidget *levelwidget;
        levels::LevelPack   *level_pack;
        unsigned     m_ilevelpack;
        string       shown_text; // info text (disappears automatically)
        double       shown_text_ttl; // rest duration for shown_text
    };

/* -------------------- MainMenu -------------------- */

    class MainMenu : public Menu {
    public:
        MainMenu();
    private:
        // Menu interface
        void draw_background(px::GC &gc);
        void tick(double dtime);

        // ActionListener interface.
        void on_action(Widget *w);

        // Private methods.
        void show_credits();
        void show_help();
        void show_text( const char *text[]);
        void build_menu();

        // Variables.
        Widget *m_startgame;
        Widget *leveled;
        Widget *manual;
        Widget *options;
        Widget *credits;
        Widget *quit;
        Widget *lpack;
    };
}

/* -------------------- OptionsMenu -------------------- */
namespace
{
    class OptionsMenu : public gui::Menu {
    public:
        OptionsMenu(px::Surface *background_);
        ~OptionsMenu();
    private:
        void update_info();

        // ActionListener interface.
        bool on_event (const SDL_Event &e);
        void on_action(gui::Widget *w);

        // Menu interface.
        void draw_background(px::GC &gc);
        void tick(double dtime);

        // Variables.
        gui::Widget *back, *fullscreen;
        gui::Label  *m_restartinfo;
        px::Surface *background;
        std::string  previous_caption;
    };
}

/* -------------------- InGameOptionsMenu -------------------- */
namespace
{
    class InGameOptionsMenu : public gui::Menu {
    public:
        InGameOptionsMenu();
        ~InGameOptionsMenu();
    private:
        void update_info();

        // ActionListener interface.
        bool on_event (const SDL_Event &e);
        void on_action(gui::Widget *w);

        // Menu interface.
        void draw_background(px::GC &gc);
        void tick(double dtime);

        // Variables.
        gui::Widget *back, *fullscreen;
        gui::Label  *m_restartinfo;
        px::Surface *background;
        std::string  previous_caption;
    };
}

/* -------------------- Options Buttons -------------------- */
namespace
{
    class DifficultyButton : public TextButton {
        // TextButton interface.
        std::string get_text() const;
        // ActionListener interface.
        void on_action(Widget *);
    public:
        DifficultyButton();
    };

    struct FullscreenButton : public BoolOptionButton {
        FullscreenButton();
    };

    struct TournamentButton : public BoolOptionButton {
        TournamentButton();
    };


    class StereoButton : public ValueButton {
        int get_value() const;
        void set_value(int value);
        string get_text(int value) const;
    public:
        StereoButton();
    };


    class SoundSetButton : public ValueButton {
        int get_value() const;
        void set_value(int value);
        string get_text(int value) const;

        bool hasSoundSet(int value);

    public:
        SoundSetButton();
    };

    class LanguageButton : public ValueButton {
        int get_value() const;
        void set_value(int value);
        string get_text(int value) const;
    public:
        LanguageButton ();
        
    };

    class GammaButton : public ValueButton {
        int get_value() const;
        void set_value(int value);
        string get_text(int value) const;
    public:
        GammaButton ();
        
    };

}
