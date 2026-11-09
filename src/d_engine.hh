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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef D_ENGINE_HH
#define D_ENGINE_HH

#include "d_follower.hh"
#include "display_internal.hh"
#include "ecl_alist.hh"
#include "ecl_array2.hh"
#include "ecl_geom.hh"
#include "ecl_math.hh"
#include <SDL3/SDL.h>

namespace enigma::display {

class DisplayLayer;

/* -------------------- DisplayEngine -------------------- */

class DisplayEngine {
public:
    explicit DisplayEngine(int tilew = 32, int tileh = 32);
    ~DisplayEngine();

    /* ---------- Class configuration ---------- */
    void add_layer(DisplayLayer *layer);
    void set_screen_area(const ecl::Rect &r);
    void set_tilesize(int w, int h);

    int get_tilew() const { return m_tilew; }
    int get_tileh() const { return m_tileh; }
    int get_width() const { return width; }
    int get_height() const { return height; }
    const ecl::Rect &get_area() const { return area; }

    /* ---------- Scrolling / page flipping ---------- */
    void setOffset(const ecl::V2 &off);
    void move_offset(const ecl::V2 &off);
    ecl::V2 getOffset() const { return offset; }

    /* ---------- Game-related stuff ---------- */
    void new_world(int w, int h);
    void tick(double dtime);

    /* ---------- Coordinate conversion ---------- */
    void world_to_screen(const ecl::V2 &pos, int *x, int *y);
    WorldArea screen_to_world(const ScreenArea &a);
    ScreenArea world_to_screen(const WorldArea &a);

    /* "Video" coordinates are like screen coordinates, except the
       origin coincides with the world origin, not the current
       scrolling position. */
    void world_to_video(const ecl::V2 &pos, int *x, int *y) const;
    void video_to_screen(int x, int y, int *xx, int *yy);
    void videoToWorld(const ecl::Rect &r, ecl::Rect &s) const;

    ecl::V2 to_world(const ecl::V2 &pos);

    /* ---------- Screen upates ---------- */

    void mark_redraw_screen();
    void markRedrawArea(const WorldArea &wa, int delay = 0);

    void updateScreen();
    void drawAll(ecl::GC &gc);
    void updateOffset();

private:
    void update_layer(DisplayLayer *l, WorldArea wa);

    /* ---------- Variables ---------- */

    std::vector<DisplayLayer *> layers;
    int m_tilew, m_tileh;

    // Offset of screen
    ecl::V2 offset;       // Offset in world units
    ecl::V2 newOffset;   // New offset in world units
    int screenOffset[2] = {};  // Offset in screen units

    // Screen area occupied by level display
    ecl::Rect area;

    // Width and height of the world in tiles
    int width, height;

    ecl::Array2<uint8_t> mustRedraw;
};

/* -------------------- DisplayLayer -------------------- */

class DisplayLayer {
public:
    DisplayLayer() : engine(nullptr) {}
    virtual ~DisplayLayer() = default;

    /* ---------- Class configuration ---------- */
    void setEngine(DisplayEngine *e) { engine = e; }
    DisplayEngine *getEngine() const { return engine; }

    /* ---------- DisplayLayer interface ---------- */
    virtual void prepareDraw(const WorldArea &) {}
    virtual void draw(ecl::GC &gc, const WorldArea &a, int x, int y) = 0;
    virtual void drawSinglePass(ecl::GC & /*gc*/) {}
    virtual void tick(double /*dtime*/) {}
    virtual void newWorld(int /*w*/, int /*h*/) {}

    // Functions.
    void markRedrawArea(const ecl::Rect &r) { getEngine()->markRedrawArea(r); }

private:
    DisplayEngine *engine;
};

/* -------------------- ModelLayer -------------------- */

/// The base class for all layers that contains Models.
class ModelLayer : public DisplayLayer {
public:
    ModelLayer() = default;

    // DisplayLayer interface
    void tick(double dtime) override;
    void newWorld(int w, int h) override;

    // Member functions
    void activate(Model *m);
    void deactivate(Model *model);
    void maybeRedrawModel(Model *m, bool immediately = false);

    virtual int redrawSize() const { return 2; }

private:
    // Variables
    ModelList activeModels;
    ModelList newActiveModels;
};

/* -------------------- DL_Grid -------------------- */

/// Layer for grid-aligned models (stones, floor tiles, items).
class DL_Grid : public ModelLayer {
public:
    explicit DL_Grid(int redrawSize = 1);
    ~DL_Grid() override;

    void setModel(int x, int y, std::unique_ptr<Model> model);
    Model *getModel(int x, int y);
    std::unique_ptr<Model> yieldModel(int x, int y);

    // DisplayLayer interface.
    void newWorld(int w, int h) override;
    void draw(ecl::GC &gc, const WorldArea &a, int x, int y) override;

    // ModelLayer interface
    int redrawSize() const override { return m_redrawSize; }

private:
    // DL_Grid interface.
    void mark_redraw(int x, int y);

    // Variables.
    typedef ecl::Array2<std::unique_ptr<Model>> ModelArray;
    ModelArray modelArray;
    int m_redrawSize;
};

/* -------------------- Sprites -------------------- */

class Sprite : public ecl::Nocopy {
public:
    std::unique_ptr<Model> model;
    ecl::V2 pos;
    int screenPos[2] = {};
    SpriteLayer layer;
    bool visible;
    Sprite *above[3] = {};
    Sprite *beneath[3] = {};

    Sprite(const ecl::V2& pos, SpriteLayer layer, std::unique_ptr<Model> model)
        : model(std::move(model)), pos(pos), layer(layer), visible(true) {
        screenPos[0] = screenPos[1] = 0;
        above[0] = above[1] = above[2] = nullptr;
        beneath[0] = beneath[1] = beneath[2] = nullptr;
    }
    ~Sprite() = default;
};

typedef std::vector<Sprite *> SpriteList;

class DL_Sprites : public ModelLayer {
public:
    DL_Sprites();
    ~DL_Sprites() override;

    /* ---------- DisplayLayer interface ---------- */
    void draw(ecl::GC &gc, const WorldArea &a, int x, int y) override;
    void drawSinglePass(ecl::GC &gc) override;
    void newWorld(int, int) override;

    /* ---------- Member functions ---------- */
    SpriteId addSprite(Sprite *sprite, bool isDispensable = false);
    void killSprite(SpriteId id);
    void moveSprite(SpriteId, const ecl::V2 &newpos);
    void replaceSprite(SpriteId id, std::unique_ptr<Model> m);

    void redrawSpriteRegion(SpriteId id);
    void drawSprites(bool shades, ecl::GC &gc, const WorldArea &a);

    Model *getModel(SpriteId id) { return sprites[id]->model.get(); }

    void setMaxSprites(unsigned newMaxSprites, unsigned newDispensibleSprites) {
        maxSprites = newMaxSprites;
        dispensableSprites = newDispensibleSprites;
    }

    Sprite *getSprite(SpriteId id);

    static constexpr SpriteId MAGIC_SPRITE_ID = 1000000;
    SpriteList sprites;
    SpriteList bottomSprites;  // bottom sprite for each x

    // ModelLayer interface
    void tick(double /*dtime*/) override;

private:
    void updateSpriteRegion(Sprite *s, bool is_add, bool is_redraw_only = false);

    // Variables.
    unsigned numSprites;          // Current number of sprites
    unsigned maxSprites;          // Maximum number of sprites
    unsigned dispensableSprites;  // Threshold above which just critical sprites are accepted
};

/* -------------------- Shadows -------------------- */

class StoneShadowCache;

class DL_Shadows : public DisplayLayer {
public:
    DL_Shadows(DL_Grid *grid, DL_Sprites *sprites);
    ~DL_Shadows() override;

    void newWorld(int width, int height) override;
    void draw(ecl::GC &gc, int xpos, int ypos, int x, int y);

    // DisplayLayer interface
    void draw(ecl::GC &gc, const WorldArea &a, int x, int y) override;
    void prepareDraw(const WorldArea &) override;

private:
    /* ---------- Private functions ---------- */
    Model *getShadowModel(int x, int y);

    /* ---------- Variables ---------- */
    DL_Grid *grid;        // Stone models
    DL_Sprites *sprites;  // Sprite models

    std::unique_ptr<StoneShadowCache> stoneShadowCache;

    std::unique_ptr<ecl::Surface> buffer; // Shadows are composited on this surface

    ecl::Array2<char> hasActor;
};

/* -------------------- Lines -------------------- */

struct Line {
    ecl::V2 start, end;
    ecl::V2 oldStart, oldEnd;
    int r = 0, g = 0, b = 0;
    bool thick = false;

    Line() = default;
    Line(const ecl::V2& start, const ecl::V2& end, int red, int green, int blue, bool isThick)
        : start(start), end(end), r(red), g(green), b(blue), thick(isThick) {}
};

typedef ecl::AssocList<unsigned, Line> LineMap;

class DL_Lines : public DisplayLayer {
public:
    DL_Lines() : m_id(1) {}

    void draw(ecl::GC & /*gc*/, const WorldArea & /*a*/, int /*x*/, int /*y*/) override {}
    void drawSinglePass(ecl::GC &gc) override;

    LineHandle addLine(const ecl::V2& start, const ecl::V2& end, unsigned short red,
            unsigned short green, unsigned short blue, bool isThick);
    void setStart(unsigned id, const ecl::V2 &start);
    void setEnd(unsigned id, const ecl::V2 &end);
    void killLine(unsigned id);
    void newWorld(int w, int h) override;

private:
    // Private methods.
    void mark_redraw_line(const Line &r);

    // Variables.
    unsigned m_id;
    LineMap m_rubbers;
};

/* -------------------- CommonDisplay -------------------- */

/*! Parts of the display engine that are common to the game and
  the editor. */
class CommonDisplay {
public:
    CommonDisplay(const ScreenArea &a = ScreenArea(0, 0, 10, 10));
    ~CommonDisplay();

    Model *set_model(const GridLoc &l, std::unique_ptr<Model> m);
    Model *get_model(const GridLoc &l);
    std::unique_ptr<Model> yield_model(const GridLoc &l);

    void set_floor(int x, int y, std::unique_ptr<Model> m);
    void set_item(int x, int y, std::unique_ptr<Model> m);
    void set_stone(int x, int y, std::unique_ptr<Model> m);

    DisplayEngine *get_engine() const { return m_engine; }

    SpriteHandle add_effect(const ecl::V2 &pos, std::unique_ptr<Model> m, bool isDispensable = false);
    SpriteHandle add_sprite(const ecl::V2 &pos, std::unique_ptr<Model> m);

    LineHandle add_line(ecl::V2 p1, ecl::V2 p2, unsigned short rc, unsigned short gc,
                          unsigned short bc, bool isThick);

    void new_world(int w, int h);
    void redraw();

protected:
    DL_Grid *floor_layer;
    DL_Grid *item_layer;
    DL_Grid *stone_layer;

    DL_Sprites *effects_layer;

    DL_Lines *line_layer;
    DL_Sprites *sprite_layer;
    DL_Shadows *shadow_layer;

private:
    DisplayEngine *m_engine;
};

/* -------------------- GameDisplay -------------------- */

class GameDisplay : public CommonDisplay {
public:
    GameDisplay(const ScreenArea &gameArea, ScreenArea inventoryArea);
    ~GameDisplay();

    StatusBar *getStatusBar() const;

    void tick(double dtime);
    void newWorld(int width, int height);

    void resizeGameArea(int width, int height);

    /* ---------- Scrolling ---------- */
    void setFollowMode(FollowMode followMode);
    void updateFollowMode();

    // Move the screen so that the current reference point is centered.
    void followCenter();

    void setReferencePoint(const ecl::V2 &point);
    void setScrollBoundary(double d);

    // current screen coordinates of reference point
    void getReferencePointCoordinates(int *x, int *y);

    /* ---------- Screen updates ---------- */
    void redraw(ecl::Screen *scr);
    void redrawAll(ecl::Screen *scr);
    void drawAll(ecl::GC &gc);

private:
    void setFollower(std::unique_ptr<Follower> f);
    void drawBorders(ecl::GC &gc);

    /* ---------- Variables ---------- */
    Uint32 lastFrameTime;
    bool redrawEverything{false};
    std::unique_ptr<StatusBarImpl> statusBar;

    ecl::V2 referencePoint;
    std::unique_ptr<Follower> follower;

    ScreenArea inventoryArea;
};

} // namespace enigma::display

#endif
