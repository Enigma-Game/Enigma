/*
 * Copyright (C) 2002,2003 Daniel Heck
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
#ifndef WIDGETS_HH_INCLUDED
#define WIDGETS_HH_INCLUDED

#include "ecl_font.hh"
#include "ecl_geom.hh"
#include "SDL.h"

namespace enigma { namespace gui {


/* -------------------- Alignment -------------------- */

    enum HAlignment {
        HALIGN_LEFT,
        HALIGN_CENTER,
        HALIGN_RIGHT
    };

    enum VAlignment {
        VALIGN_TOP,
        VALIGN_CENTER,
        VALIGN_BOTTOM
    };

/* -------------------- Events & Event Handlers -------------------- */
    class Widget;

    class ActionListener {
    public:
        virtual ~ActionListener() {}
        virtual void on_action(Widget *) {};
    };

/* -------------------- GUI Widgets -------------------- */

    class Container;

    class Widget : public ActionListener {
    public:

        /* ---------- Widget interface ---------- */
        virtual void draw (ecl::GC &gc, const ecl::Rect &r) = 0;
        virtual void activate() {}
        virtual void deactivate() {}
        
        virtual void realize (const ecl::Rect &r) {
            set_area (r);
        }

        virtual bool on_event(const SDL_Event &/*e*/);
        Uint8 lastMouseButton() {return mouseButton;}
        Uint16 lastModifierKeys() { return modifierKeys; }
        

        virtual void move (int x, int y);
        virtual void resize (int w, int h);

        virtual void naturalsize (int &w, int &h) const {
            w = h = 5;
        }

        /* ---------- Accessors ---------- */
        void set_size(int w, int h) {area.w = w; area.h = h;}

        ecl::Rect get_area() const { return area; }
        void set_area(const ecl::Rect &r) { area = r; }
        int get_x() const { return area.x; }
        int get_y() const { return area.y; }
        int get_w() const { return area.w; }
        int get_h() const { return area.h; }

        void set_parent(Container *parent) { m_parent = parent; }
        Container *get_parent() const { return m_parent; }

        void set_listener(ActionListener *al) {
            m_listener = al;
        }

        void invalidate();
        virtual void tick (double /*dtime*/) {}

    protected:
        Widget(Container *parent=nullptr);

        /* ---------- Functions ---------- */
        void reconfigure();
        void invalidate_area(const ecl::Rect &r);
        void invoke_listener();
        
    private:
        ecl::Rect        area;
        Container      *m_parent;
        ActionListener *m_listener;
        Uint16  modifierKeys;
        Uint8   mouseButton;
    };

/* -------------------- AreaManager -------------------- */

    class AreaManaged {
    public:
        virtual ~AreaManaged() {}

        virtual void invalidate_area(const ecl::Rect &r) = 0;
        virtual void invalidate_all() = 0;
        virtual void refresh() = 0;
    };

    // The AreaManager perform refreshes of invalidated regions.
    // It gets attached to the top-level Container during the first invalidation
    // or refresh request.
    class AreaManager : public AreaManaged {
    public:
        AreaManager(Container *managed);

        void invalidate_area(const ecl::Rect &r) override;
        void invalidate_all() override;
        void refresh() override;

    private:
        ecl::RectList  dirtyrects;
        Container    *top_container;
    };

/* -------------------- Container -------------------- */

    class Container : public Widget, public AreaManaged {
    public:
        Container();
        ~Container();

        void add_child (Widget *w);
        virtual void remove_child (Widget *w);
        virtual void exchange_child (Widget *oldChild, Widget *newChild);
        virtual void reconfigure_child (Widget *w);

        Widget *find_widget(int x, int y);
        Widget *find_adjacent_widget(Widget *from, int x, int y);

        void clear();
        void draw_all();

        virtual void set_key_focus(Widget *newfocus);
        virtual bool is_key_focus(Widget *focus);

        // Widget interface.
        void draw (ecl::GC& gc, const ecl::Rect &r) override;
        void move (int x, int y) override;

        // AreaManaged interface.
        void invalidate_area(const ecl::Rect &r) override;
        void invalidate_all() override;
        void refresh() override;
        
    protected:
        typedef std::vector<Widget *> WidgetList;
        typedef WidgetList::iterator iterator;

        iterator begin() { return m_widgets.begin(); }
        iterator end() { return m_widgets.end(); }
        WidgetList m_widgets;

    private:
        ecl::Rect boundingbox();

        AreaManager *getAreaManager();
        AreaManager *managed_by;
    };


/* -------------------- List, HList, VList -------------------- */

    class List : public Container {
    public:
        void set_spacing (int pixels);

        enum ExpansionMode {
            EXPAND,
            TIGHT
        };

        void add_back (Widget *w, ExpansionMode m = List::TIGHT);
        virtual void remove_child (Widget *w) override;
        virtual void exchange_child (Widget *oldChild, Widget *newChild) override;

        void set_default_size (int w, int h);
        void set_alignment (HAlignment halign, VAlignment valign);
        virtual bool fits() = 0;

    protected:
        List(int spacing=5);

        int calc_minimum_height () const;
        int calc_minimum_width () const;

        int get_spacing () const;

        void get_size (const Widget *widget, int &w, int &h) const;

        // ---------- Widget interface ----------
        virtual void move (int x, int y) override;
        virtual void resize(int w, int h) override;

        // ---------- List interface ---------- 
        virtual void recalc() = 0;

        // ---------- Container interface ----------
        virtual void reconfigure_child (Widget *w) override;


    protected:
        std::vector<ExpansionMode> m_expansionmodes;

    private:
        int m_spacing;          // # of pixels between container items
        bool has_default_size;
        int defaultw, defaulth;
    protected:
        HAlignment m_halign;
        VAlignment m_valign;

    };


    class HList : public List {
    public:
        HList() : List() {}
        virtual bool fits() override;

    private:
        // List interface
        virtual void recalc() override;
    };

    class VList : public List {
    public:
        VList() : List() {}
        virtual bool fits() override;

    private:
        // List interface
        virtual void recalc() override;

    };

/* -------------------- Image -------------------- */

    class Image : public Widget {
    public:
        Image (std::string iname) : imgname(std::move(iname)) {}
        void draw (ecl::GC &gc, const ecl::Rect &r) override;
    private:
        std::string imgname;
    };

/* -------------------- Label -------------------- */

    class Label : public Widget {
    public:
        Label (const std::string &text="",
               HAlignment halign=HALIGN_CENTER, 
               VAlignment valign=VALIGN_CENTER);

        // Widget interface
        virtual void draw (ecl::GC &gc, const ecl::Rect &r) override;
        virtual void naturalsize (int &w, int &h) const override;

        // Methods
        void set_text (const std::string &text);
        virtual std::string get_text() const;  // translated
        std::string getText() const;
        void set_font (ecl::Font *font);
        void set_alignment (HAlignment halign, VAlignment valign=VALIGN_CENTER);
        bool text_fits(double area_fraction = 1.0);
    protected:
        // Variables.
        std::string m_text;
        ecl::Font   *m_font;
        HAlignment  m_halign;
        VAlignment  m_valign;
    };

/* -------------------- UntranslatedLabel -------------------- */

    class UntranslatedLabel : public Label {
    public:
        UntranslatedLabel(const std::string &text="",
               HAlignment halign=HALIGN_CENTER, 
               VAlignment valign=VALIGN_CENTER);

        // TextButton interface.
        virtual std::string get_text() const override;
    };

/* -------------------- Button -------------------- */

    class Button : public Widget {
    public:
        void setHighlight(bool shouldHighlight);
        bool isHighlight();
    protected:
        Button();
        
        // Widget interface.
        void draw(ecl::GC &gc, const ecl::Rect &r) override;
        void activate() override;
        void deactivate() override;
        bool m_activep;
        bool highlight;
    };


/* -------------------- PushButton -------------------- */

    class PushButton : public Button {
    public:
        PushButton();

        bool is_pressed() { return m_pressedp; }
        
    protected:
        bool on_event(const SDL_Event &e) override;
        void deactivate() override;
        SDL_Keycode getLastUpSym();
        Uint8 getLastUpButton();
        virtual bool soundOk(); 
    private:
        bool m_pressedp;
        SDL_Keycode lastUpSym;
        Uint8 lastUpButton;
    };

/* -------------------- TextButton -------------------- */

    class TextButton : public PushButton {
    public:
        TextButton(ActionListener *al=nullptr);
        virtual std::string get_text() const = 0;

    private:
        // Widget interface.
        void draw(ecl::GC &gc, const ecl::Rect &r) override;

        // Variables.
        ecl::Font *menufont;
        ecl::Font *menufont_pressed;
    };

/* -------------------- StaticTextButton -------------------- */

    class StaticTextButton : public TextButton {
    public:
        StaticTextButton(const std::string &t, ActionListener *al=nullptr);
        virtual void set_text(const std::string &t);

        // TextButton interface.
        std::string get_text() const override;

    protected:
        // Variables.
        std::string text;
    };

/* -------------------- UntranslatedStaticTextButton -------------------- */

    class UntranslatedStaticTextButton : public StaticTextButton {
    public:
        UntranslatedStaticTextButton(const std::string &t, ActionListener *al=nullptr);

        // TextButton interface.
        std::string get_text() const override;
    };

/* -------------------- BoolOptionButton -------------------- */
    class BoolOptionButton : public TextButton {
    public:
        BoolOptionButton(const char         *option_name,
                         const std::string&  true_text,
                         const std::string&  false_text,
                         ActionListener     *al = nullptr);

        bool toggle(); // returns new value
        virtual void on_action(Widget *) override;

        // TextButton interface.
        std::string get_text() const override;

    private:
        const char *optionName;
        std::string trueText;
        std::string falseText;
    };

/* -------------------- ValueButton -------------------- */
    class ValueButton: public TextButton {
    public:
        ValueButton(int min_value_, int max_value_, ActionListener *al = NULL);

        virtual int get_value() const     = 0;
        virtual void set_value(int value) = 0;
        void setMaxValue(int max);

        bool inc_value(int offset);

        // TextButton interface.
        virtual std::string get_text() const override;

        // Widget interface.
        virtual void on_action(Widget *w) override;

        // ValueButton uses listener in another way, so for event
        // handling we have to introduce a second ActionListener here.
        ActionListener *secondaryListener;

    protected:
        void init(); // called in ctor of derived
        virtual bool soundOk() override; 
    private:
        int min_value;
        int max_value;

        bool update_value(int old_value, int new_value);
        virtual std::string get_text(int value) const = 0;
    };

    /* -------------------- ImageButton -------------------- */

    class ImageButton : public PushButton {
    public:
        ImageButton(const std::string &unselected,
                    const std::string &selected,
                    ActionListener    *al = nullptr);
        void set_images(const std::string &unselected, const std::string &selected);
        // Widget interface.
        virtual void draw(ecl::GC &gc, const ecl::Rect &r) override;
    private:
        std::string fname_sel, fname_unsel;
    };


    /* -------------------- BorderlessImageButton -------------------- */

    class BorderlessImageButton : public PushButton {
    public:
        BorderlessImageButton(const std::string &unselected,
                              const std::string &selected,
                              const std::string &mouseover,
                              bool isSelected =false,
                              ActionListener    *al = nullptr);
        void set_images(const std::string &unselected, const std::string &selected,
                        const std::string &mouseover);
        void setState(bool isSelected);
        bool getState() const;
        // Widget interface.
        virtual void draw(ecl::GC &gc, const ecl::Rect &r) override;
    private:
        std::string fname_sel, fname_unsel, fname_mouse;
        bool state;
    };

}} // namespace enigma::gui
#endif
