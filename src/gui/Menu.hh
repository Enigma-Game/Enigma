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
#ifndef GUI_MENU_HH_INCLUDED
#define GUI_MENU_HH_INCLUDED

#include "gui/widgets.hh"
#include "gui/Menu.hh"
#include "ecl_fwd.hh"
#include "ecl_geom.hh"
#include "SDL.h"
#include <cmath>
#include <vector>

namespace enigma { namespace gui {
/* -------------------- Menu -------------------- */

    class Menu : public Container {
    public:
        Menu();

        //! true: ok, false: menu aborted by user
        bool manage();

        void add(Widget *w);
        void add(Widget *w, ecl::Rect r);
        void center();

        void draw_all();
        void draw (ecl::GC &gc, const ecl::Rect &r);

        void quit();
        void abort();

    protected:
        void reset_active_widget()
        { active_widget = 0; }

        // Menu interface.
        virtual void draw_background(ecl::GC &/*gc*/) {}

    private:
        void handle_event(const SDL_Event &e);

        void switch_active_widget(Widget *to_activate);
        void track_active_widget( int x, int y ); // used by mouse
        void goto_adjacent_widget(int xdir, int ydir); // used by keyboard

        // Variables.
        Widget *active_widget;
        bool quitp, abortp;
    };

    class BuildVList {
        ecl::Rect r;
        Menu *container;
        int skip;
    public:
        BuildVList(Menu *cc, const ecl::Rect &rr, int s)
        : r(rr), container(cc), skip(s)
        {}

        Widget *add(Widget *w) {
            container->add(w, r);
            r.y += r.h+skip;
            return w;
        }

        ecl::Rect pos() const { return r; }
    };

    class VTableBuilder {
        Menu     *m_menu;
        ecl::Rect  m_targetarea;
        ecl::Rect  m_widgetsize;
        int       m_vspacing, m_hspacing;
        
    public:
        VTableBuilder (Menu *menu, const ecl::Rect &targetarea, const ecl::Rect &widgetsize,
                       int   vspacing, int hspacing)
        : m_menu(menu),
          m_targetarea(targetarea),
          m_widgetsize(widgetsize),
          m_vspacing(vspacing),
          m_hspacing(hspacing)
        {}

        bool finish(std::vector<Widget*> widgets) {
            // if finish returns false, no widgets have been added to the menu
            // because the layout failed.

            if (widgets.empty())
                return true;

            int count       = (int) widgets.size();
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
                    ecl::Rect rr(x, y, m_widgetsize.w, m_widgetsize.h);
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
        ecl::Rect r;
        Menu *container;
        int skip;
    public:
        BuildHList(Menu *cc, const ecl::Rect &rr, int s)
        : r(rr), container(cc), skip(s)
        {}

        Widget * add(Widget *w) {
            container->add(w, r);
            r.x += r.w+skip;
            return w;
        }
        Widget *add (Widget *w, int width) {
            ecl::Rect rr(r.x, r.y, width, r.h);
            container->add(w, rr);
            r.x += width + skip;
            return w;
        }

        ecl::Rect pos() const { return r; }
    };


    class BuildTable {
        Container *container;
        int columns;
        int current_column;
        int rowheight;

    public:
        BuildTable (Container *container_, int columns_, int rowheight_)
        : container(container_) , 
          columns (columns_),
          current_column (0),
          rowheight (rowheight_)
        {}
   
        void add (Widget *w) {

        }
    };
}} // namespace enigma::gui
#endif
