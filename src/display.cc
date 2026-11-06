/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
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

// This file contains the code that renders the game graphics. This
// includes displaying the current landscape with all its objects and the
// inventory at the bottom of the screen.

#include "display_internal.hh"
#include "client.hh"
#include "errors.hh"
#include "main.hh"
#include "resource_cache.hh"
#include "server.hh"
#include "video.hh"

#include "ecl_font.hh"
#include "ecl_sdl.hh"
#include "ecl_video.hh"

#include <algorithm>
#include <functional>
#include <iostream>
#include "d_engine.hh"
#include "d_models.hh"

using namespace ecl;

namespace enigma::display {

class dRect {
public:
    dRect(double x_, double y_, double w_, double h_) {
        x = x_;
        y = y_;
        w = w_;
        h = h_;
    }
    double x, y, w, h;
};

Rect round_grid(const dRect &r, double w, double h) {
    double x = r.x / w;
    double y = r.y / h;
    double x2 = (r.x + r.w - 1) / w;
    double y2 = (r.y + r.h - 1) / h;

    Rect s(round_down<int>(x), round_down<int>(y), round_down<int>(x2), round_down<int>(y2));
    s.w -= s.x - 1;
    s.h -= s.y - 1;
    return s;
}

/* -------------------- Local variables -------------------- */

namespace {

    const int NTILESH = 20;  // Default game screen width in tiles
    const int NTILESV = 13;  // Default game screen height in tiles

    DisplayFlags display_flags = SHOW_ALL;
    GameDisplay *gamedpy = nullptr;
    bool ShowFPS = false;

}  // namespace

//======================================================================
// STATUS BAR
//======================================================================

StatusBarImpl::StatusBarImpl(const ScreenArea& area)
    : Window(area), textDisplay(*GetFont("statusbarfont")) {
    const VMInfo* vminfo = video_engine->GetInfo();
    itemArea = vminfo->sb_itemarea;
    timeFont = GetFont("timefont");
    movesFont = GetFont("smallfont");
    modesFont = GetFont("modesfont");

    maxWidthDigit = 0;
    for (int i = 0; i < 10; i++) {
        widthDigit[i] = timeFont->get_width('0' + i);
        maxWidthDigit = ecl::Max(maxWidthDigit, widthDigit[i]);
    }
    widthColon = timeFont->get_width(':');
    widthApos = timeFont->get_width('\'');
    widthQuote = timeFont->get_width('\"');
}

StatusBarImpl::~StatusBarImpl() = default;

void StatusBarImpl::setTime(double time) {
    double oldtime = levelTime;
    levelTime = time;
    if (showTime && floor(levelTime) - floor(oldtime) >= 1)
        changed = true;  // update clock
}

void StatusBarImpl::hideText() {
    if (textActive) {
        textActive = false;
        changed = true;
    }
}

void StatusBarImpl::setSpeed(double /*speed*/) {
}

void StatusBarImpl::setTravelledDistance(double /*distance*/) {
}

void StatusBarImpl::setCounter(int newCounter) {
    if (showMoves && newCounter != moveCounter) {
        changed = true;
        moveCounter = newCounter;
    }
}

void StatusBarImpl::showMoveCounter(bool active) {
    if (active != showMoves) {
        showMoves = active;
        changed = true;
    }
}
void StatusBarImpl::setCMode(bool flag) {
    cMode = flag;
    changed = true;
}

void StatusBarImpl::setBasicModes(std::string flags) {
    basicModes = flags;
    changed = true;
}

void StatusBarImpl::redraw(ecl::GC &gc, const ScreenArea &r) {
    const VMInfo *vminfo = video_engine->GetInfo();
    ScreenArea a = getArea();
    clip(gc, intersect(a, r));

    blit(gc, a.x, a.y,
         GetImage(player == YIN ? "inventory_yin" : "inventory_yang", ".png"));

    // draw player indicator
    int ts = vminfo->tile_size;
    int xoff = 35 * ts / 8;
    int yoff = 4 * ts / 8 + vminfo->sb_coffsety;
    blit(gc, a.x + xoff, a.y + yoff, GetImage("player_switch_anim", ".png"),
         Rect(0, playerImage * ts, ts, ts));

    //     set_color (gc, 255, 0, 0);
    //     frame (gc, vminfo->sb_timearea);
    //     frame (gc, vminfo->sb_textarea);
    //     set_color (gc, 0, 255, 0);
    //     frame (gc, vminfo->sb_movesarea);
    //     frame (gc, vminfo->sb_itemarea);

    ScreenArea timeArea = vminfo->sb_timearea;
    ScreenArea modesArea = vminfo->sb_modesarea;
    ScreenArea movesArea = vminfo->sb_movesarea;

    // draw modes indicators
    std::unique_ptr<Surface> surfaceModes = modesFont->render(((cMode ? "c" : "") + basicModes));
    {
        int x = modesArea.x + modesArea.w - surfaceModes->width();
        int y = modesArea.y;
        blit(gc, x, y, surfaceModes.get());
    }

    if (showTime || showMoves) {
        int abstime = ecl::round_nearest<int>(fabs(levelTime));
        // abstime += 10*63*60;  //for testing purposes
        // Clamp time to a maximum of 9 hours, 59 minutes, and 59 seconds
        abstime = std::min(abstime, 9 * 3600 + 59 * 60 + 59);
        int hours = abstime / 3600;
        int minutes = (abstime - 3600 * hours) / 60;
        int seconds = abstime % 60;
        bool showHours = hours != 0;
        bool showMinutes = hours != 0 || minutes >= 10;
        bool showSeconds = hours == 0 || vminfo->tile_size >= 40;

        int timeWidth = 0;
        if (showTime) {
            timeWidth = (showHours ? maxWidthDigit + widthColon : 0)
                    + (showMinutes ? maxWidthDigit : 0) + maxWidthDigit + widthApos
                    + (showSeconds ? 2 * maxWidthDigit + widthQuote : 0);
        }

        std::unique_ptr<Surface> surfaceMoves;
        if (showMoves)
            surfaceMoves = movesFont->render(ecl::strf("%d", moveCounter));

        if (showTime) {
            if (showMoves) {  // time + moves
                int x = movesArea.x + (movesArea.w - surfaceMoves->width()) / 2;
                int y = movesArea.y + (movesArea.h + timeFont->get_lineskip()) / 2
                        - movesFont->get_lineskip() - 4;
                blit(gc, x, y, surfaceMoves.get());
            }

            // draw time in pixel-stable positions
            int x = showMoves ? timeArea.x + (movesArea.x - timeArea.x - timeWidth) / 2
                : timeArea.x + (timeArea.w - timeWidth) / 2;
            int y = timeArea.y + (timeArea.h - timeFont->get_lineskip()) / 2;
            if (showHours) {
                blit(gc, x + maxWidthDigit - widthDigit[hours], y,
                        timeFont->render(ecl::strf("%d:", hours)).get());
                x += maxWidthDigit + widthColon;
            }
            if (showMinutes) {
                blit(gc, x + maxWidthDigit - widthDigit[minutes / 10], y,
                        timeFont->render(ecl::strf("%02d", minutes)).get());
                x += 2 * maxWidthDigit;
            } else {
                blit(gc, x + maxWidthDigit - widthDigit[minutes % 10], y,
                        timeFont->render(ecl::strf("%d", minutes)).get());
                x += maxWidthDigit;
            }
            blit(gc, x, y, timeFont->render(std::string("'")).get());
            x += widthApos;
            if (showSeconds) {
                blit(gc, x + maxWidthDigit - widthDigit[seconds / 10], y,
                        timeFont->render(ecl::strf("%02d", seconds)).get());
                x += 2 * maxWidthDigit;
                blit(gc, x, y, timeFont->render(std::string("\"")).get());
            }
        } else {  // only moves
            int x = timeArea.x + (timeArea.w - surfaceMoves->width()) / 2;
            int y = timeArea.y + (timeArea.h - movesFont->get_lineskip()) / 2;
            blit(gc, x, y, surfaceMoves.get());
        }
    }

    if (textActive) {
        textDisplay.draw(gc, r);
    } else {
        client::Msg_FinishedText();
        int itemSize = static_cast<int>(vminfo->tile_size * 1.125);
        int x = itemArea.x;
        for (auto &model : itemModels) {
            model->draw(gc, x, itemArea.y);
            x += itemSize;
        }
    }
    changed = false;
}

void StatusBarImpl::setInventory(Player activePlayer, const std::vector<std::string>& modelNames) {
    player = activePlayer;
    if (textActive && interruptible) {
        hideText();
    }

    itemModels.clear();
    for (auto &modelName : modelNames) {
        itemModels.push_back(MakeModel(modelName));
    }
    changed = true;
}

void StatusBarImpl::showText(const std::string &str, bool scrolling, double duration) {
    textDisplay.setText(str, scrolling, duration);
    interruptible = false;
    textActive = true;
    changed = true;
}

void StatusBarImpl::tick(double dtime) {
    // Animation of player indicator
    playerImageDuration += dtime;
    if ((player * 12 != playerImage) && (playerImageDuration > 0.1)) {
        playerImage = (playerImage + 1) % 24;
        playerImageDuration = 0;
        changed = true;
    }

    // Update text display
    if (textActive) {
        textDisplay.tick(dtime);
        changed = changed || textDisplay.hasChanged();
        if (textDisplay.hasFinished()) {
            textActive = false;
            changed = true;
        }
    }
}

void StatusBarImpl::newWorld() {
    itemModels.clear();
    levelTime = 0;
    textActive = false;
    changed = true;
    player = YIN;
    playerImage = 0;
    playerImageDuration = 0;
}

/* -------------------- TextDisplay implementation -------------------- */

TextDisplay::TextDisplay(Font& font)
    : scrollSpeed(DEFAULT_TextSpeed * FACTOR_TextSpeed), font(font) {
    const VMInfo *vminfo = video_engine->GetInfo();
    area = vminfo->sb_textarea;
    // Note: "scrollSpeed" is not yet initialized with
    //   GetTextSpeed() * FACTOR_TextSpeed but a
    //   default value because Application State
    //   Manager has not been initialized at this point
    //   yet: This would crash.
}

void TextDisplay::setText(const std::string &newText, bool scrolling, double duration) {
    text = newText;
    textSurface = font.render(text);
    pingpong = false;
    time = 0;

    if (scrolling) {
        if (duration <= 0) {
            xoff = -area.w;
            scrollSpeed = GetTextSpeed() * FACTOR_TextSpeed;
        } else {
            // Showscroll mode: first show the string, then scroll it out
            showscroll = true;
            scrollSpeed = 0;
            if (area.w < textSurface->width()) {
                // start left adjusted for long strings
                xoff = 0;
            } else {
                // start centered for short strings
                xoff = -(area.w - textSurface->width()) / 2;
            }
        }
    }

    if (duration > 0)
        maxtime = duration;
    else
        maxtime = 1e20;  // "infinite" for all practical purposes

    if (!scrolling) {  // centered text string
        if (area.w < textSurface->width()) {
            pingpong = true;
            scrollSpeed = (textSurface->width() - area.w) / duration;
            xoff = 0;
        } else {
            xoff = -(area.w - textSurface->width()) / 2;
            scrollSpeed = 0;
        }
    }

    finished = false;
    changed = true;
}

void TextDisplay::tick(double dtime) {
    time += dtime;
    if (time > maxtime) {
        if (showscroll) {
            showscroll = false;
            scrollSpeed = GetTextSpeed() * FACTOR_TextSpeed;
            maxtime = 1e20;  // "infinite" for all practical purposes
        } else {
            finished = true;
            changed = true;
        }
    } else {
        int oldXOff = round_nearest<int>(xoff);
        xoff += dtime * scrollSpeed;
        int newXOff = round_nearest<int>(xoff);
        changed = newXOff != oldXOff;
        if (pingpong) {
            if ((scrollSpeed > 0 && area.w + newXOff >= textSurface->width())
                    || (scrollSpeed < 0 && newXOff <= 0)) {
                scrollSpeed = -scrollSpeed;
            }
        } else if (xoff >= textSurface->width()) {
            finished = true;
            changed = true;
        }
    }
}

void TextDisplay::draw(ecl::GC &gc, const ScreenArea &r) {
    clip(gc, intersect(area, r));
    set_color(gc, 0, 0, 0);
    box(gc, area);
    if (Surface *s = textSurface.get())
        blit(gc, area.x - round_nearest<int>(xoff), area.y, s);
}

//======================================================================
// DISPLAY ENGINE
//======================================================================

DisplayEngine::DisplayEngine(int tilew, int tileh)
: m_tilew(tilew),
  m_tileh(tileh),
  width(0),
  height(0),
  mustRedraw(0, 0) {
    area = video_engine->GetInfo()->area;
    screenOffset[0] = screenOffset[1] = 0;
}

DisplayEngine::~DisplayEngine() {
    delete_sequence(layers.begin(), layers.end());
}

void DisplayEngine::set_tilesize(int w, int h) {
    m_tilew = w;
    m_tileh = h;
}

void DisplayEngine::add_layer(DisplayLayer *layer) {
    layer->setEngine(this);
    layers.push_back(layer);
}

void DisplayEngine::setOffset(const V2 &off) {
    offset = newOffset = off;
    world_to_video(off, &screenOffset[0], &screenOffset[1]);
}

void DisplayEngine::move_offset(const ecl::V2 &off) {
    newOffset = off;
}

/*! Scroll the screen contents and mark the newly exposed regions for
  redrawing.  This method assumes that the screen contents were not
  modified externally since the last call to update_offset(). */
void DisplayEngine::updateOffset() {
    ecl::Screen *screen = video_engine->GetScreen();

    int oldx = screenOffset[0];
    int oldy = screenOffset[1];
    int newx, newy;
    world_to_video(newOffset, &newx, &newy);

    if (newx != oldx || newy != oldy) {
        // TODO: Up to Enigma 1.21, we used the following code:
        /*
        const Rect &a = get_area();
        Rect oldarea(a.x + oldx, a.y + oldy, a.w, a.h);
        Rect newarea(a.x + newx, a.y + newy, a.w, a.h);
        Rect common = intersect(newarea, oldarea);

        // Blit overlapping screen area from old to new position
        GC screengc(screen->get_surface());
        Rect blitrect(common.x - oldx, common.y - oldy, common.w, common.h);
        blit(screengc, common.x - newx, common.y - newy, screen->get_surface(), blitrect);
        blitrect.x = common.x - newx;
        blitrect.y = common.y - newy;
        screen->update_rect(blitrect);

        // Update offset
        set_offset(V2(newx / double(m_tilew), newy / double(m_tileh)));

        // Mark areas that could not be copied from old screen for redraw
        RectList rl;
        rl.push_back(get_area());
        rl.sub(blitrect);
        for (auto &rect : rl)
            mark_redraw_area(screen_to_world(rect));
        */
        // Unfortunately, switching to SDL 2 created problems with blitting
        // from one surface to the same. For the time being, we simply
        // redraw the whole surface:
        setOffset(V2(newx / double(m_tilew), newy / double(m_tileh)));
        mark_redraw_screen();
        // This is very resource-hungry, but only marginally slower than
        // blitting first to a temporary surface, and then back to the
        // video screen. The best solution would be to have two surfaces to
        // blit to in class Screen, and smooth scrolling would alternate
        // between these; or to keep the whole level in one surface and
        // blit from there to the screen.
    }
}

void DisplayEngine::set_screen_area(const ecl::Rect &r) {
    area = r;
}

void DisplayEngine::new_world(int w, int h) {
    width = w;
    height = h;
    offset = newOffset = V2();
    screenOffset[0] = screenOffset[1] = 0;
    mustRedraw.resize(w, h);
    mustRedraw.fill(1);

    for (auto &layer : layers)
        layer->newWorld(w, h);
}

void DisplayEngine::tick(double dtime) {
    for (auto &layer : layers)
        layer->tick(dtime);
}

void DisplayEngine::world_to_screen(const V2 &pos, int *x, int *y) {
    *x = round_nearest<int>(pos[0] * m_tilew) - screenOffset[0] + get_area().x;
    *y = round_nearest<int>(pos[1] * m_tileh) - screenOffset[1] + get_area().y;
}

void DisplayEngine::world_to_video(const ecl::V2 &pos, int *x, int *y) const {
    *x = round_nearest<int>(pos[0] * m_tilew);
    *y = round_nearest<int>(pos[1] * m_tileh);
}

void DisplayEngine::video_to_screen(int x, int y, int *xx, int *yy) {
    *xx = x - screenOffset[0] + get_area().x;
    *yy = y - screenOffset[1] + get_area().y;
}

/* Calculate the smallest rectangle 's' in world space aligned to
   tiles that contains a certain rectangle 'r' in video space.  This
   function is used for calculating the region that needs to be
   updated when a sprite with extension 'r' is moved on the screen. */
void DisplayEngine::videoToWorld(const ecl::Rect &r, Rect &s) const {
    dRect dr(r.x, r.y, r.w, r.h);
    s = round_grid(dr, get_tilew(), get_tileh());
}

ScreenArea DisplayEngine::world_to_screen(const WorldArea &a) {
    int x, y;
    world_to_screen(V2(a.x, a.y), &x, &y);
    return ScreenArea(x, y, a.w * m_tilew, a.h * m_tileh);
}

WorldArea DisplayEngine::screen_to_world(const ScreenArea &a) {
    int sx = screenOffset[0] + a.x - get_area().x;
    int sy = screenOffset[1] + a.y - get_area().y;

    int x1 = Max(0, sx / m_tilew);
    int y1 = Max(0, sy / m_tileh);
    int x2 = Min(width, (sx + a.w + m_tilew - 1) / m_tilew);
    int y2 = Min(height, (sy + +a.h + m_tileh - 1) / m_tileh);

    return WorldArea(x1, y1, x2 - x1, y2 - y1);
}

V2 DisplayEngine::to_world(const V2 &pos) {
    return offset + V2((pos[0] - get_area().x) / m_tilew, (pos[1] - get_area().y) / m_tileh);
}

void DisplayEngine::markRedrawArea(const WorldArea &wa, int delay) {
    int x2 = Min(width, wa.x + wa.w);
    int y2 = Min(height, wa.y + wa.h);
    for (int x = Max(0, wa.x); x < x2; x++)
        for (int y = Max(0, wa.y); y < y2; y++) {
            int d = mustRedraw(x, y);
            if (d == 0 || 1 + delay < d)
                mustRedraw(x, y) = 1 + delay;
        }
}

void DisplayEngine::mark_redraw_screen() {
    markRedrawArea(screen_to_world(area));
}

void DisplayEngine::drawAll(ecl::GC &gc) {
    WorldArea wa = screen_to_world(get_area());

    // Fill screen the area not covered by world
    {
        RectList rl;
        rl.push_back(get_area());
        rl.sub(world_to_screen(WorldArea(0, 0, width, height)));
        set_color(gc, 200, 0, 200);
        for (auto &rect : rl)
            box(gc, rect);
    }

    int xpos, ypos;
    world_to_screen(V2(wa.x, wa.y), &xpos, &ypos);
    for (auto &layer : layers) {
        clip(gc, get_area());
        layer->prepareDraw(wa);
        layer->draw(gc, wa, xpos, ypos);
        layer->drawSinglePass(gc);
    }
}

void DisplayEngine::update_layer(DisplayLayer *l, WorldArea wa) {
    GC gc(video_engine->GetScreen()->get_surface());

    int x2 = wa.x + wa.w;
    int y2 = wa.y + wa.h;
    int y2m1 = y2 - 1;

    clip(gc, get_area());
    int xpos, ypos0;
    world_to_screen(V2(wa.x, wa.y), &xpos, &ypos0);

    l->prepareDraw(wa);
    for (int x = wa.x; x < x2; x++, xpos += m_tilew) {
        int ypos = ypos0;
        for (int y = wa.y; y < y2; y++, ypos += m_tileh) {
            if (mustRedraw(x, y) == 1) {
                if (y < y2m1 && mustRedraw(x, y + 1) == 1) {
                    l->draw(gc, WorldArea(x, y, 1, 2), xpos, ypos);
                    y++;
                    ypos += m_tileh;
                } else
                    l->draw(gc, WorldArea(x, y, 1, 1), xpos, ypos);
            }
        }
    }
    l->drawSinglePass(gc);
}

void DisplayEngine::updateScreen() {
    ecl::Screen *screen = video_engine->GetScreen();
    GC gc(screen->get_surface());

    if (newOffset != offset) {
        updateOffset();
        newOffset = offset;
    }

    Rect area = get_area();
    clip(gc, area);

    WorldArea wa = screen_to_world(area);
    for (auto &layer : layers) {
        update_layer(layer, wa);
    }
    int x2 = wa.x + wa.w;
    int y2 = wa.y + wa.h;
    for (int x = wa.x; x < x2; x++) {
        for (int y = wa.y; y < y2; y++) {
            if (mustRedraw(x, y) >= 1) {
                if ((mustRedraw(x, y) -= 1) == 0)
                    screen->update_rect(world_to_screen(WorldArea(x, y, 1, 1)));
            }
        }
    }
}

/* -------------------- ModelLayer -------------------- */

void ModelLayer::maybeRedrawModel(Model *m, bool immediately) {
    Rect videoarea;
    if (m->hasChanged(videoarea)) {
        int delay = immediately ? 0 : IntegerRand(0, 2, false);
        WorldArea worldArea;
        getEngine()->videoToWorld(videoarea, worldArea);
        getEngine()->markRedrawArea(worldArea, delay);
    }
}

void ModelLayer::activate(Model *m) {
    newActiveModels.push_back(m);
}

void ModelLayer::deactivate(Model *model) {
    std::list<Model *> &am = activeModels;
    auto it = find(am.begin(), am.end(), model);
    if (it == am.end()) {
        newActiveModels.remove(model);
    } else {
        *it = nullptr;
    }
}

void ModelLayer::newWorld(int, int) {
    activeModels.clear();
    newActiveModels.clear();
}

void ModelLayer::tick(double dtime) {
    activeModels.remove(nullptr);
    activeModels.remove_if(std::mem_fn(&Model::hasFinished));

    // Append new active models to list
    activeModels.splice(activeModels.end(), newActiveModels);

    // We cannot use a foreach loop here because animations can remove themselves
    // during a tick. This may happen, for example, when a model callback decides
    // to replace the old model with another one.
    for (auto it = activeModels.begin(); it != activeModels.end(); ++it) {
        if (Model *model = *it) {
            model->tick(dtime);

            // We have to check (*it) again because the list of active
            // models may have changed!
            if ((model = *it))
                maybeRedrawModel(model);
        }
    }
}

/* -------------------- GridLayer -------------------- */

DL_Grid::DL_Grid(int redrawSize) : modelArray(0, 0), m_redrawSize(redrawSize) {
}

DL_Grid::~DL_Grid() = default;

void DL_Grid::newWorld(int w, int h) {
    ModelLayer::newWorld(w, h);
    modelArray.resize(w, h);
}

void DL_Grid::mark_redraw(int x, int y) {
    getEngine()->markRedrawArea(WorldArea(x, y, m_redrawSize, m_redrawSize));
}

void DL_Grid::setModel(int x, int y, std::unique_ptr<Model> model) {
    if (!(x >= 0 && y >= 0 && x < modelArray.width() && y < modelArray.height())) {
        return;
    }

    if (modelArray(x, y) != model) {
        if (Model* oldModel = modelArray(x, y).get()) {
            oldModel->removeFromLayer(this);
        }
        mark_redraw(x, y);
        Model* newModel = model.get();
        modelArray(x, y) = std::move(model);
        if (newModel) {
            int vx, vy;
            getEngine()->world_to_video(V2(x, y), &vx, &vy);
            newModel->expose(this, vx, vy);
        }
    }
}

Model *DL_Grid::getModel(int x, int y) {
    return modelArray(x, y).get();
}

std::unique_ptr<Model> DL_Grid::yieldModel(int x, int y) {
    if (Model *model = getModel(x, y))
        model->removeFromLayer(this);
    mark_redraw(x, y);
    return std::move(modelArray(x, y));
}

void DL_Grid::draw(ecl::GC &gc, const WorldArea &a, int destx, int desty) {
    int x2 = a.x + a.w;
    int y2 = a.y + a.h;
    int tilew = getEngine()->get_tilew();
    int tileh = getEngine()->get_tileh();
    int xpos = destx;
    for (int x = a.x; x < x2; ++x) {
        int ypos = desty;
        for (int y = a.y; y < y2; ++y) {
            if (Model *m = modelArray(x, y).get())
                m->draw(gc, xpos, ypos);
            ypos += tileh;
        }
        xpos += tilew;
    }
}

/* -------------------- Sprites -------------------- */

SpriteHandle::SpriteHandle(DL_Sprites *l, unsigned spriteid) : layer(l), id(spriteid) {
}

SpriteHandle::SpriteHandle() : layer(nullptr) {
    id = DL_Sprites::MAGIC_SPRITE_ID;
}

void SpriteHandle::kill() {
    if (layer) {
        layer->killSprite(id);
        layer = nullptr;
        id = DL_Sprites::MAGIC_SPRITE_ID;
    }
}

void SpriteHandle::move(const ecl::V2 &newpos) const {
    if (layer)
        layer->moveSprite(id, newpos);
}

void SpriteHandle::replace_model(std::unique_ptr<Model> m) const {
    if (layer)
        layer->replaceSprite(id, std::move(m));
}

Model *SpriteHandle::get_model() const {
    return layer ? layer->getModel(id) : nullptr;
}

void SpriteHandle::set_callback(ModelCallback *cb) const {
    if (Model *m = get_model())
        m->setCallback(cb);
}

void SpriteHandle::hide() const {
    if (layer) {
        Sprite *s = layer->getSprite(id);
        if (s->visible) {
            s->visible = false;
            layer->redrawSpriteRegion(id);
        }
    }
}

void SpriteHandle::show() const {
    if (layer) {
        Sprite *s = layer->getSprite(id);
        if (!s->visible) {
            s->visible = true;
            layer->redrawSpriteRegion(id);
        }
    }
}

/* -------------------- Sprite layer -------------------- */

DL_Sprites::DL_Sprites() : numSprites(0), maxSprites(1000), dispensableSprites(1000) {
}

DL_Sprites::~DL_Sprites() {
    delete_sequence(sprites.begin(), sprites.end());
}

Sprite *DL_Sprites::getSprite(SpriteId id) {
    ASSERT(id != MAGIC_SPRITE_ID, XLevelRuntime,
           "Sprite layer fatal error: request of not existing sprite");
    return sprites[id];
}

void DL_Sprites::newWorld(int w, int h) {
    ModelLayer::newWorld(w, h);
    delete_sequence(sprites.begin(), sprites.end());
    sprites.clear();
    Sprite *dummy = nullptr;
    bottomSprites.assign(w, dummy);
    numSprites = 0;
}

void DL_Sprites::moveSprite(SpriteId id, const ecl::V2 &newpos) {
    Sprite *sprite = sprites[id];

    int newx, newy;
    getEngine()->world_to_video(newpos, &newx, &newy);

    if (newx != sprite->screenPos[0] || newy != sprite->screenPos[1]) {
        updateSpriteRegion(sprite, false);  // make sure old sprite is removed
        sprite->pos = newpos;
        sprite->screenPos[0] = newx;
        sprite->screenPos[1] = newy;
        if (Anim2d *anim = dynamic_cast<Anim2d *>(sprite->model.get()))
            anim->move(newx, newy);
        updateSpriteRegion(sprite, true);  // draw new sprite
    }
}

SpriteId DL_Sprites::addSprite(Sprite *sprite, bool isDispensable) {
    if (numSprites >= maxSprites || (isDispensable && numSprites >= dispensableSprites)) {
        delete sprite;
        return MAGIC_SPRITE_ID;
    }

    SpriteList &sl = sprites;
    SpriteId id = 0;

    // Find the first empty slot
    auto i = find(sl.begin(), sl.end(), nullptr);
    if (i == sl.end()) {
        id = sl.size();
        sl.push_back(sprite);
    } else {
        id = distance(sl.begin(), i);
        *i = sprite;
    }
    getEngine()->world_to_video(sprite->pos, &sprite->screenPos[0], &sprite->screenPos[1]);
    if (Model *m = sprite->model.get())
        m->expose(this, sprite->screenPos[0], sprite->screenPos[1]);
    updateSpriteRegion(sprite, true);
    numSprites += 1;
    return id;
}

void DL_Sprites::replaceSprite(SpriteId id, std::unique_ptr<Model> m) {
    Sprite *sprite = sprites[id];
    if (Model *old = sprite->model.get()) {
        updateSpriteRegion(sprite, false);
        old->removeFromLayer(this);
    }
    sprite->model = std::move(m);
    if (sprite->model) {
        sprite->model->expose(this, sprite->screenPos[0], sprite->screenPos[1]);
        updateSpriteRegion(sprite, true);
    }
}

void DL_Sprites::killSprite(SpriteId id) {
    if (Sprite *sprite = sprites[id]) {
        updateSpriteRegion(sprite, false);
        if (Model *m = sprite->model.get()) {
            m->removeFromLayer(this);
        }
        sprites[id] = nullptr;
        numSprites -= 1;
        delete sprite;
    }
}

void DL_Sprites::draw(ecl::GC &gc, const WorldArea &a, int /*x*/, int /*y*/) {
    DisplayEngine *engine = getEngine();
    clip(gc, intersect(engine->get_area(), engine->world_to_screen(a)));
    drawSprites(false, gc, a);
}

void DL_Sprites::drawSprites(bool drawshadowp, GC &gc, const WorldArea &a) {
    int gx = a.x;
    for (int i = 0; i < a.w; i++, gx++) {
        int m = gx % 3;
        Sprite *s = bottomSprites[gx];
        for (; s != nullptr; s = s->above[m]) {
            if (s->model && s->visible) {
                int sx, sy;
                getEngine()->world_to_screen(s->pos, &sx, &sy);
                if (drawshadowp)
                    s->model->drawShadow(gc, sx, sy);
                else
                    s->model->draw(gc, sx, sy);
            }
        }
    }
}

void DL_Sprites::drawSinglePass(ecl::GC & /*gc*/) {
    //     draw_sprites (false, gc);
}

void DL_Sprites::redrawSpriteRegion(SpriteId id) {
    Sprite *s = sprites[id];
    updateSpriteRegion(s, true, true);
}

void DL_Sprites::updateSpriteRegion(Sprite *s, bool is_add, bool is_redraw_only) {
    if (s && s->model) {
        Rect redrawr;
        Rect r = s->model->boundingBox();
        r.x += s->screenPos[0];
        r.y += s->screenPos[1];
        DisplayEngine *e = getEngine();
        e->videoToWorld(r, redrawr);
        e->markRedrawArea(redrawr);
        if (is_redraw_only)
            return;

        int x = redrawr.x;
        for (int i = 0; i < redrawr.w; i++, x++) {
            if (x >= 0 && x < e->get_width()) {
                int m = x % 3;
                if (is_add) {
                    if (bottomSprites[x] != nullptr)
                        bottomSprites[x]->beneath[m] = s;
                    s->above[m] = bottomSprites[x];
                    s->beneath[m] = nullptr;
                    bottomSprites[x] = s;
                } else {  // remove
                    if (bottomSprites[x] == s) {
                        bottomSprites[x] = s->above[m];
                        if (s->above[m] != nullptr)
                            s->above[m]->beneath[m] = nullptr;
                    } else {
                        if (s->above[m] != nullptr) {
                            s->above[m]->beneath[m] = s->beneath[m];
                        }
                        if (s->beneath[m] != nullptr) {
                            s->beneath[m]->above[m] = s->above[m];
                        }
                    }
                }
            }
        }
    }
}

void DL_Sprites::tick(double dtime) {
    SpriteList &sl = sprites;
    for (unsigned i = 0; i < sl.size(); ++i) {
        Sprite *s = sl[i];
        if (!s || !s->model)
            continue;

        if (s->model->hasFinished() && s->layer == SPRITE_EFFECT) {
            // Only remove effect sprites -- actor sprites remain in
            // the world all the time
            killSprite(i);
        }
    }
    ModelLayer::tick(dtime);
}

//----------------------------------------------------------------------
// RUBBER BANDS
//----------------------------------------------------------------------

void DL_Lines::drawSinglePass(ecl::GC &gc) {
    DisplayEngine *engine = getEngine();

    //    set_color (gc, 240, 140, 20, 255);
    set_flags(gc.flags, GS_ANTIALIAS);

    for (auto &elem : m_rubbers) {
        int x1, y1, x2, y2;
        engine->world_to_screen(elem.second.start, &x1, &y1);
        engine->world_to_screen(elem.second.end, &x2, &y2);

        set_color(gc, elem.second.r, elem.second.g, elem.second.b, 255);
        line(gc, x1, y1, x2, y2);
        if (elem.second.thick) {
            line(gc, x1 - 1, y1, x2 - 1, y2);
            line(gc, x1, y1 - 1, x2, y2 - 1);
            line(gc, x1 - 1, y1 - 1, x2 - 1, y2 - 1);
        }
    }
}

/* Mark the screen region occupied by a rubber band for redrawing.
   The problem is: what region is that exactly?  What pixels on the screen
   will the line rasterizer touch?  Hard to tell, especially when
   antialiasing is used.

   This function constructs a list of rectangles that completely
   enclose the line by subdividing the line into n segments and
   constructing the bounding box for each of these segments.  To
   account for the (effective) finite width of the line, these boxes
   need to be enlarged by a small amount to make them overlap a bit.

   The number n of subdivision depends on the length of the line.  n=1
   would, of course, do, but we want to redraw as little of the screen
   as possible.  `n' is therefore chosen in such a way that the line
   is covered with boxes of size not larger than 'maxboxsize'.
*/
void DL_Lines::mark_redraw_line(const Line &r) {
    const double maxboxsize = 0.5;

    double w0 = r.start[0] - r.end[0];
    double h0 = r.start[1] - r.end[1];
    int n = int(std::max(abs(w0), abs(h0)) / maxboxsize) + 1;

    double w = w0 / n;
    double h = h0 / n;

    double overlap = 0.1;

    double x = r.end[0];
    double y = r.end[1];

    double xoverlap = w < 0 ? -overlap : overlap;
    double yoverlap = h < 0 ? -overlap : overlap;

    for (int i = 0; i < n; ++i) {
        dRect dr(x - xoverlap, y - yoverlap, w + 2 * xoverlap, h + 2 * yoverlap);
        WorldArea wa = round_grid(dr, 1, 1);

        if (wa.w < 0) {
            wa.x += wa.w;
            wa.w = -wa.w;
        }
        if (wa.h < 0) {
            wa.y += wa.h;
            wa.h = -wa.h;
        }
        wa.w++;
        wa.h++;

        getEngine()->markRedrawArea(wa);

        x += w;
        y += h;
    }
}

LineHandle DL_Lines::addLine(const V2 &start, const V2 &end, unsigned short red, unsigned short green,
                                unsigned short blue, bool isThick) {
    m_rubbers[m_id] = Line(start, end, red, green, blue, isThick);
    mark_redraw_line(m_rubbers[m_id]);
    return LineHandle(this, m_id++);
}

void DL_Lines::setStart(unsigned id, const V2 &start) {
    mark_redraw_line(m_rubbers[id]);
    m_rubbers[id].start = start;
    mark_redraw_line(m_rubbers[id]);
}

void DL_Lines::setEnd(unsigned id, const V2 &end) {
    mark_redraw_line(m_rubbers[id]);
    m_rubbers[id].end = end;
    mark_redraw_line(m_rubbers[id]);
}

void DL_Lines::killLine(unsigned id) {
    mark_redraw_line(m_rubbers[id]);
    auto i = m_rubbers.find(id);
    if (i != m_rubbers.end())
        m_rubbers.erase(i);
}

void DL_Lines::newWorld(int /*w*/, int /*h*/) {
    m_rubbers.clear();
    m_id = 1;
}

LineHandle::LineHandle(DL_Lines* lineLayer, unsigned id) : lineLayer(lineLayer), id(id) {
}

void LineHandle::setStartPoint(const V2 &start) {
    lineLayer->setStart(id, start);
}

void LineHandle::setEndPoint(const V2 &end) {
    lineLayer->setEnd(id, end);
}

void LineHandle::kill() {
    lineLayer->killLine(id);
}

//----------------------------------------------------------------------
// SHADOWS
//----------------------------------------------------------------------

/*
** Drawing the shadows is more difficult than drawing any of the
** other layers. There are a couple of reasons for this:
**
** 1. Both Stones and actors cast a shadow.  Not a real problem, but
**    it makes the implementation more complex.
**
** 2. Shadows can overlap.  Not only can the shadows of stones and
**    actors overlap, but also the shadows of two adjacent stones can.
**    Since we are using alpha blending for the shadows, this means
**    that we cannot blit the invidual shadows to the screen, but we
**    have to use an intermediate buffer.
**
** 3. Performance is critical.  Drawing the shadows is time-consuming,
**    firstly because alpha blending is costly and secondly because of
**    the intermediate buffer.  So we should try to cache shadows *and*
**    avoid the buffer if possible.
**
** So, how do we approach these problems? We handle stone and actor
** shadows separately: The stone shadows do not change very often, so
** it's easy to cache them, one tile at a time.  If there is no actor
** on this tile, we can blit the cached image directly to the screen.
** Otherwise, we have no choice but to use the buffer.
**
** The remaining problem is the shadow cache.  The easiest solution
** would be to use one huge image for the whole level and keep it in
** memory all the time.  This would consume roughly 20mb for a 100x100
** landscape, which is, of course, excessive, considering that there are
** rarely more than 40 different shadow tiles in each landscape.
**
** Instead, Enigma caches the most recently calculated shadow tiles in
** a linked list.  (If this should one day turn out to be too slow,
** it's still possible to resort to a hash table or something
** similar.)
*/

namespace {

    struct ImageQuad {
        Image* images[4];

        ImageQuad() : images{} {}

        ImageQuad(Image* i1, Image* i2, Image* i3, Image* i4) : images{i1, i2, i3, i4} {}

        bool operator==(const ImageQuad& q) const {
            return images[0] == q.images[0] && images[1] == q.images[1] && images[2] == q.images[2]
                    && images[3] == q.images[3];
        }
        Image* operator[](int idx) const { return images[idx]; }
    };

    // Returns true if all four models are static ImageModels; fills ImageQuad
    // with the corresponding images in this case.
    bool onlyStaticShadows(Model *models[4], ImageQuad &quad) {
        int num_static_shadows = 4;

        for (int i = 0; i < 4; ++i) {
            if (models[i] == nullptr) {
                // No model at all? -> static
                quad.images[i] = nullptr;
            } else if (Model *shadow = models[i]->get_shadow()) {
                if (ImageModel *im = dynamic_cast<ImageModel *>(shadow)) {
                    // We have a model with a static image shadow
                    quad.images[i] = im->get_image();
                } else {
                    quad.images[i] = nullptr;
                    num_static_shadows--;
                }
            } else
                quad.images[i] = nullptr;
        }
        return num_static_shadows == 4;
    }

    // Returns a new RGBA surface suitable for drawing shadows.
    SDL_Surface *CreateShadowSurface(int width, int height) {
        SDL_Surface *ss = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_ARGB8888);
        SDL_SetSurfaceAlphaMod(ss, 128);
        return ss;
    }

    struct StoneShadow {
        ImageQuad images;
        std::unique_ptr<Surface> surface;
        bool inCache;

        StoneShadow(const ImageQuad& images, bool cached)
        : images(images), inCache(cached) {}
    };

}  // namespace

class StoneShadowCache : public ecl::Nocopy {
public:
    StoneShadowCache(int tilew, int tileh);
    ~StoneShadowCache();

    StoneShadow* retrieve(Model* models[4]);
    void release(StoneShadow* s);
    void clear();

private:
    // Private methods.
    std::unique_ptr<Surface> newSurface();
    StoneShadow* findInCache(const ImageQuad& images);

    void fillImage(StoneShadow* s);
    void fillImage(StoneShadow* sh, Model* models[4]);

    // Variables
    // Use std::list to maintain LRU cache.
    std::list<std::unique_ptr<StoneShadow>> lruCache;
    int tileWidth, tileHeight;
    std::vector<std::unique_ptr<Surface>> surfaceAvail;
};

StoneShadowCache::StoneShadowCache(int tilew, int tileh) {
    tileWidth = tilew;
    tileHeight = tileh;
}

StoneShadowCache::~StoneShadowCache() {
    clear();
}

void StoneShadowCache::clear() {
    lruCache.clear();
    surfaceAvail.clear();
}

void StoneShadowCache::fillImage(StoneShadow *sh) {
    // Special case: no shadows at all:
    if (!sh->images[0] && !sh->images[1] && !sh->images[2] && !sh->images[3]) {
        sh->surface = nullptr;
        return;
    }

    std::unique_ptr<Surface> surface = newSurface();
    GC gc(surface.get());
    set_color(gc, 255, 255, 255, 0);
    box(gc, surface->size());

    if (Image *i = sh->images[0])
        draw_image(i, gc, -tileWidth, -tileHeight);
    if (Image *i = sh->images[1])
        draw_image(i, gc, 0, -tileHeight);
    if (Image *i = sh->images[2])
        draw_image(i, gc, -tileWidth, 0);
    if (Image *i = sh->images[3])
        draw_image(i, gc, 0, 0);
    sh->surface = std::move(surface);
}

void StoneShadowCache::fillImage(StoneShadow *sh, Model *models[4]) {
    std::unique_ptr<Surface> s = newSurface();
    GC gc(s.get());
    set_color(gc, 255, 255, 255, 0);
    box(gc, s->size());
    if (models[0])
        models[0]->drawShadow(gc, -tileWidth, -tileHeight);
    if (models[1])
        models[1]->drawShadow(gc, 0, -tileHeight);
    if (models[2])
        models[2]->drawShadow(gc, -tileWidth, 0);
    if (models[3])
        models[3]->drawShadow(gc, 0, 0);
    sh->surface = std::move(s);
}

StoneShadow *StoneShadowCache::findInCache(const ImageQuad &images) {
    for (auto it = lruCache.begin(); it != lruCache.end(); ++it) {
        if ((*it)->images == images) {
            StoneShadow *sh = it->get();
            // Move entry to front of list
            lruCache.splice(lruCache.begin(), lruCache, it);
            return sh;
        }
    }
    return nullptr;
}

/* Try to look up the shadow created by the four models in 'models[]'
   in the shadow cache. */
StoneShadow *StoneShadowCache::retrieve(Model *models[4]) {
    StoneShadow *shadow = nullptr;
    ImageQuad images;

    // Only cache static stone shadows, i.e., those consisting
    // only of Image models.
    if (onlyStaticShadows(models, images)) {
        shadow = findInCache(images);
        if (!shadow) {
            shadow = new StoneShadow(images, true);
            fillImage(shadow);
            lruCache.push_front(std::unique_ptr<StoneShadow>(shadow));
        }
    } else {
        shadow = new StoneShadow(images, false);
        fillImage(shadow, models);
    }
    return shadow;
}

void StoneShadowCache::release(StoneShadow *s) {
    if (s->inCache) {
        // Image is in cache, no need to free anything
    } else {
        surfaceAvail.push_back(std::move(s->surface));
        delete s;
    }
}

std::unique_ptr<Surface> StoneShadowCache::newSurface() {
    std::unique_ptr<Surface> surface;
    if (surfaceAvail.empty()) {
        SDL_Surface *ss = CreateShadowSurface(tileWidth, tileHeight);
        surface = Surface::make_surface(ss);
    } else {
        surface = std::move(surfaceAvail.back());
        surfaceAvail.pop_back();
    }
    return std::move(surface);
}

/* -------------------- Shadow layer -------------------- */

DL_Shadows::DL_Shadows(DL_Grid *grid, DL_Sprites *sprites)
    : grid(grid), sprites(sprites), stoneShadowCache(nullptr), hasActor(0, 0) {
}

DL_Shadows::~DL_Shadows() = default;

void DL_Shadows::newWorld(int width, int height) {
    hasActor.resize(width, height);
    hasActor.fill(false);

    DisplayEngine *engine = getEngine();
    int tileWidth = engine->get_tilew();
    int tileHeight = engine->get_tileh();

    stoneShadowCache = std::make_unique<StoneShadowCache>(tileWidth, tileHeight);
    buffer = Surface::make_surface(CreateShadowSurface(tileWidth, tileHeight));
}

void DL_Shadows::draw(ecl::GC &gc, const WorldArea &a, int destx, int desty) {
    int x2 = a.x + a.w;
    int y2 = a.y + a.h;
    int tilew = getEngine()->get_tilew();
    int tileh = getEngine()->get_tileh();
    int xpos = destx;
    for (int x = a.x; x < x2; ++x) {
        int ypos = desty;
        for (int y = a.y; y < y2; ++y) {
            draw(gc, xpos, ypos, x, y);
            ypos += tileh;
        }
        xpos += tilew;
    }
}

// Prepare the shadow layer for redrawing. This routine pre-calculates the
// tiles that currently are partially covered by an actor.
void DL_Shadows::prepareDraw(const WorldArea &wa) {
    for (int i = 0; i < wa.w; ++i)
        for (int j = 0; j < wa.h; ++j)
            hasActor(wa.x + i, wa.y + j) = false;

    for (auto s : sprites->sprites) {
        if (s && s->layer == SPRITE_ACTOR && s->model) {
            Rect r, redrawr;
            r = s->model->boundingBox();
            r.x += s->screenPos[0];
            r.y += s->screenPos[1];
            DisplayEngine *e = getEngine();
            e->videoToWorld(r, redrawr);
            redrawr.intersect(wa);

            for (int i = 0; i < redrawr.w; ++i)
                for (int j = 0; j < redrawr.h; ++j)
                    hasActor(redrawr.x + i, redrawr.y + j) = true;
        }
    }
}

Model *DL_Shadows::getShadowModel(int x, int y) {
    if (x >= 0 && y >= 0) {
        if (Model *m = grid->getModel(x, y))
            return m;  // return m->get_shadow();
    }
    return nullptr;
}

void DL_Shadows::draw(GC &gc, int xpos, int ypos, int x, int y) {
    Model *models[4];
    models[0] = getShadowModel(x - 1, y - 1);
    models[1] = getShadowModel(x, y - 1);
    models[2] = getShadowModel(x - 1, y);
    models[3] = getShadowModel(x, y);

    StoneShadow *sh = stoneShadowCache->retrieve(models);

    int tilew = getEngine()->get_tilew();
    int tileh = getEngine()->get_tileh();

    bool hasActor = this->hasActor(x, y);
    if (hasActor || sh->surface) {
        Surface *shadowSurface = sh->surface.get();
        if (hasActor) {
            GC gc2(buffer.get());
            if (shadowSurface) {
                shadowSurface->lock();
                buffer->lock();
                SDL_Surface *ss = shadowSurface->get_surface();
                SDL_Surface *bs = buffer->get_surface();
                memcpy(bs->pixels, ss->pixels, ss->w * ss->h * ss->format->BytesPerPixel);
                buffer->unlock();
                shadowSurface->unlock();
            } else {
                set_color(gc2, 255, 255, 255, 0);
                box(gc2, buffer->size());
            }

            int m = x % 3;
            Sprite *sp = sprites->bottomSprites[x];
            for (; sp != nullptr; sp = sp->above[m]) {
                if (sp->visible && sp->model) {
                    int sx = round_nearest<int>(sp->pos[0] * tilew) - x * tilew;
                    int sy = round_nearest<int>(sp->pos[1] * tileh) - y * tileh;
                    sp->model->drawShadow(gc2, sx, sy);
                }
            }
            blit(gc, xpos, ypos, buffer.get());
        } else {
            blit(gc, xpos, ypos, shadowSurface);
        }
    }

    stoneShadowCache->release(sh);
}

//----------------------------------------------------------------------
// Editor / game display engine
//----------------------------------------------------------------------

CommonDisplay::CommonDisplay(const ScreenArea &a) {
    m_engine = new DisplayEngine;
    m_engine->set_screen_area(a);

    VideoTileset *vts = video_engine->GetTileset();
    m_engine->set_tilesize(vts->tilesize, vts->tilesize);

    // Create and configure display layers
    floor_layer = new DL_Grid;
    item_layer = new DL_Grid;
    sprite_layer = new DL_Sprites;
    stone_layer = new DL_Grid(2);
    shadow_layer = new DL_Shadows(stone_layer, sprite_layer);
    line_layer = new DL_Lines;
    effects_layer = new DL_Sprites;
    effects_layer->setMaxSprites(70, 50);

    // Register display layers
    m_engine->add_layer(floor_layer);
    m_engine->add_layer(item_layer);
    m_engine->add_layer(shadow_layer);
    m_engine->add_layer(sprite_layer);
    m_engine->add_layer(stone_layer);
    m_engine->add_layer(line_layer);
    m_engine->add_layer(effects_layer);
}

CommonDisplay::~CommonDisplay() {
    delete m_engine;
}

Model *CommonDisplay::set_model(const GridLoc &l, std::unique_ptr<Model> m) {
    int x = l.pos.x, y = l.pos.y;

    Model *result = m.get();
    switch (l.layer) {
        case GRID_FLOOR:
            floor_layer->setModel(x, y, std::move(m));
            break;
        case GRID_ITEMS:
            item_layer->setModel(x, y, std::move(m));
            break;
        case GRID_STONES:
            stone_layer->setModel(x, y, std::move(m));
            break;
        case GRID_COUNT: break;
    }
    return result;
}

Model *CommonDisplay::get_model(const GridLoc &l) {
    int x = l.pos.x, y = l.pos.y;
    switch (l.layer) {
        case GRID_FLOOR: return floor_layer->getModel(x, y);
        case GRID_ITEMS: return item_layer->getModel(x, y);
        case GRID_STONES: return stone_layer->getModel(x, y);
        case GRID_COUNT: return nullptr;
    }
    return nullptr;
}

std::unique_ptr<Model> CommonDisplay::yield_model(const GridLoc &l) {
    int x = l.pos.x, y = l.pos.y;
    switch (l.layer) {
        case GRID_FLOOR: return floor_layer->yieldModel(x, y);
        case GRID_ITEMS: return item_layer->yieldModel(x, y);
        case GRID_STONES: return stone_layer->yieldModel(x, y);
        case GRID_COUNT: return nullptr;
    }
    return nullptr;
}

LineHandle CommonDisplay::add_line(V2 p1, V2 p2, unsigned short rc, unsigned short gc,
                                     unsigned short bc, bool isThick) {
    return line_layer->addLine(p1, p2, rc, gc, bc, isThick);
}

SpriteHandle CommonDisplay::add_effect(const V2 &pos, std::unique_ptr<Model> m, bool isDispensable) {
    auto spr = new Sprite(pos, SPRITE_EFFECT, std::move(m));
    return SpriteHandle(effects_layer, effects_layer->addSprite(spr, isDispensable));
}

SpriteHandle CommonDisplay::add_sprite(const V2 &pos, std::unique_ptr<Model> m) {
    auto spr = new Sprite(pos, SPRITE_ACTOR, std::move(m));
    return SpriteHandle(sprite_layer, sprite_layer->addSprite(spr));
}

void CommonDisplay::new_world(int w, int h) {
    get_engine()->new_world(w, h);
}

void CommonDisplay::redraw() {
    get_engine()->updateScreen();
}

void CommonDisplay::set_floor(int x, int y, std::unique_ptr<Model> m) {
    floor_layer->setModel(x, y, std::move(m));
}

void CommonDisplay::set_item(int x, int y, std::unique_ptr<Model> m) {
    item_layer->setModel(x, y, std::move(m));
}

void CommonDisplay::set_stone(int x, int y, std::unique_ptr<Model> m) {
    stone_layer->setModel(x, y, std::move(m));
}

//----------------------------------------------------------------------
// Game Display Engine
//----------------------------------------------------------------------

GameDisplay::GameDisplay(const ScreenArea& gameArea, ScreenArea inventoryArea)
    : CommonDisplay(gameArea), lastFrameTime(0), redrawEverything(false), follower(nullptr),
      inventoryArea(inventoryArea) {
    statusBar = std::make_unique<StatusBarImpl>(inventoryArea);
}

GameDisplay::~GameDisplay() = default;

void GameDisplay::tick(double dtime) {
    get_engine()->tick(dtime);
    statusBar->tick(dtime);

    if (follower)
        follower->tick(dtime, referencePoint);
}

void GameDisplay::newWorld(int width, int height) {
    CommonDisplay::new_world(width, height);
    statusBar->newWorld();
    resizeGameArea(NTILESH, NTILESV);
    updateFollowMode();
    referencePoint = V2();
}

StatusBar *GameDisplay::getStatusBar() const {
    return statusBar.get();
}

/* -------------------- Scrolling -------------------- */

void GameDisplay::setFollowMode(FollowMode followMode) {
    switch (followMode) {
        case FOLLOW_NONEOLD:
            setFollower(nullptr);
            break;
        case FOLLOW_SCROLLING:
            setFollower(std::make_unique<Follower_Scrolling>(get_engine(), false));
            break;
        case FOLLOW_SCREEN:
            setFollower(std::make_unique<Follower_Screen>(get_engine()));
            break;
        case FOLLOW_SCREENSCROLLING:
            setFollower(std::make_unique<Follower_Scrolling>(get_engine(), true, 0.5, 0.5));
            break;
        case FOLLOW_SMOOTH:
            setFollower(std::make_unique<Follower_Smooth>(get_engine()));
    }
    get_engine()->mark_redraw_screen();
}

void GameDisplay::updateFollowMode() {
    if (!server::FollowGrid) {
        setFollower(std::make_unique<Follower_Smooth>(get_engine()));
    } else if (server::FollowMethod == FOLLOW_NONE) {
        setFollower(nullptr);
    } else if (server::FollowMethod == FOLLOW_FLIP) {
        if (server::FollowThreshold.getType() == Value::DOUBLE) {
            setFollower(std::make_unique<Follower_Screen>(get_engine(),
                    server::FollowThreshold.toDouble(), server::FollowThreshold.toDouble()));
        } else {
            setFollower(std::make_unique<Follower_Screen>(get_engine(),
                    server::FollowThreshold.toVec()[0], server::FollowThreshold.toVec()[1]));
        }
    } else if (server::FollowThreshold.getType() == Value::DOUBLE
            && server::FollowThreshold.toDouble() == 0.5
            && server::FollowAction == Value(ecl::V2(9.5, 6))) {
        setFollower(std::make_unique<Follower_Scrolling>(get_engine(), false));
    } else if (server::FollowThreshold.getType() == Value::DOUBLE) {
        setFollower(std::make_unique<Follower_Scrolling>(get_engine(), true,
                server::FollowThreshold.toDouble(), server::FollowThreshold.toDouble()));
    } else {
        setFollower(std::make_unique<Follower_Scrolling>(get_engine(), true,
                ecl::V2(server::FollowThreshold.toVec())[0],
                ecl::V2(server::FollowThreshold.toVec())[1]));
    }
    get_engine()->mark_redraw_screen();
}

void GameDisplay::setFollower(std::unique_ptr<Follower> f) {
    follower = std::move(f);
    if (follower)
        follower->center(referencePoint);
}

void GameDisplay::followCenter() {
    if (follower)
        follower->center(referencePoint);
}

void GameDisplay::setReferencePoint(const V2 &point) {
    referencePoint = point;
}

void GameDisplay::getReferencePointCoordinates(int *x, int *y) {
    get_engine()->world_to_screen(referencePoint, x, y);
}

void GameDisplay::setScrollBoundary(double boundary) {
    if (follower)
        follower->setBorder(boundary, boundary);
}

/* ---------- Screen updates ---------- */

void GameDisplay::redrawAll(Screen *scr) {
    get_engine()->mark_redraw_screen();
    redrawEverything = true;
    scr->update_all();
    redraw(scr);
}

void GameDisplay::redraw(ecl::Screen *screen) {
    GC gc(screen->get_surface());
    if (SDL_GetTicks() - lastFrameTime > 10) {
        CommonDisplay::redraw();

        if (ShowFPS) {
            char fps[20];
            sprintf(fps, "fps: %d\n", int(1000.0 / (SDL_GetTicks() - lastFrameTime)));
            Font *f = GetFont("levelmenu");

            clip(gc);
            Rect area(0, 0, 80, 20);
            set_color(gc, 0, 0, 0);
            box(gc, area);
            f->render(gc, 0, 0, std::string(fps));

            screen->update_rect(area);
        }
        lastFrameTime = SDL_GetTicks();
    }
    if (statusBar->hasChanged() || redrawEverything) {
        statusBar->redraw(gc, inventoryArea);
        screen->update_rect(inventoryArea);
    }
    if (redrawEverything)
        drawBorders(gc);
    screen->flush_updates();
    redrawEverything = false;
}

void GameDisplay::drawAll(GC &gc) {
    get_engine()->drawAll(gc);
    statusBar->redraw(gc, inventoryArea);
    drawBorders(gc);
}

void GameDisplay::drawBorders(GC &gc) {
    RectList rl;
    rl.push_back(gc.drawable->size());
    rl.sub(get_engine()->get_area());
    rl.sub(inventoryArea);
    clip(gc);
    set_color(gc, 0, 0, 0);
    for (auto &rect : rl)
        box(gc, rect);
}

void GameDisplay::resizeGameArea(int width, int height) {
    DisplayEngine *e = get_engine();
    int neww = width * e->get_tilew();
    int newh = height * e->get_tileh();

    VideoTileset *vts = video_engine->GetTileset();

    int screenw = NTILESH * vts->tilesize;
    int screenh = NTILESV * vts->tilesize;
    if (neww > screenw || newh > screenh) {
        Log << "Illegal screen size (" << neww << "," << newh
            << "): larger than physical display\n";
        return;
    }
    Rect r((screenw - neww) / 2, (screenh - newh) / 2, neww, newh);
    e->set_screen_area(r);
    followCenter();
}

/* -------------------- Global functions -------------------- */

void Init(bool showFps) {
    if (showFps)  // keep ShowFPS on false for screen resolution changes
        ShowFPS = true;
    InitModels();

    const VMInfo *vminfo = video_engine->GetInfo();
    gamedpy = new GameDisplay(vminfo->gamearea, vminfo->statusbararea);
}

void Shutdown() {
    delete gamedpy;
    ShutdownModels();
}

void Tick(double dtime) {
    gamedpy->tick(dtime);
}

StatusBar *GetStatusBar() {
    return gamedpy->getStatusBar();
}

void NewWorld(int w, int h) {
    gamedpy->newWorld(w, h);
}

void FocusReferencePoint() {
    gamedpy->followCenter();
}

void SetReferencePoint(const ecl::V2 &point) {
    gamedpy->setReferencePoint(point);
}

void SetFollowMode(FollowMode m) {
    gamedpy->setFollowMode(m);
}

void UpdateFollowMode() {
    gamedpy->updateFollowMode();
}

void SetScrollBoundary(double boundary) {
    gamedpy->setScrollBoundary(boundary);
}

void GetReferencePointCoordinates(int *x, int *y) {
    gamedpy->getReferencePointCoordinates(x, y);
}

Model* SetModel(const GridLoc &l, std::unique_ptr<Model> m) {
    return gamedpy->set_model(l, std::move(m));
}

Model* SetModel(const GridLoc &l, const std::string &modelname) {
    return SetModel(l, MakeModel(modelname));
}

void KillModel(const GridLoc &l) {
    (void)YieldModel(l); // discard result
}

Model *GetModel(const GridLoc &l) {
    return gamedpy->get_model(l);
}

std::unique_ptr<Model> YieldModel(const GridLoc &l) {
    return gamedpy->yield_model(l);
}

SpriteHandle AddEffect(const V2 &pos, const char *modelname, bool isDispensible) {
    return gamedpy->add_effect(pos, MakeModel(modelname), isDispensible);
}

SpriteHandle AddSprite(const V2 &pos, const char *modelname) {
    std::unique_ptr<Model> m = modelname ? MakeModel(modelname) : nullptr;
    return gamedpy->add_sprite(pos, std::move(m));
}

void ToggleFlag(DisplayFlags flag) {
    toggle_flags(display_flags, flag);
}

void DrawAll(GC &gc) {
    gamedpy->drawAll(gc);
}

void RedrawAll(Screen *screen) {
    gamedpy->redrawAll(screen);
}

void Redraw(Screen *screen) {
    gamedpy->redraw(screen);
}

void ResizeGameArea(int w, int h) {
    gamedpy->resizeGameArea(w, h);
}
const Rect &GetGameArea() {
    return gamedpy->get_engine()->get_area();
}

LineHandle AddRubber(const V2& start, const V2& end, unsigned short red,
        unsigned short green, unsigned short blue, bool isThick) {
    return gamedpy->add_line(start, end, red, green, blue, isThick);
}

void SetTextSpeed(int newSpeed) {
    int speed = ecl::Clamp<int>(newSpeed, MIN_TextSpeed, MAX_TextSpeed);
    app.state->setProperty("TextSpeed", speed);
}

int GetTextSpeed() {
    int speed = app.state->getInt("TextSpeed");
    if (speed == 0) {
        // Text Speed has not been set yet. Use default.
        SetTextSpeed(DEFAULT_TextSpeed);
        return DEFAULT_TextSpeed;
    }
    return speed;
}

} // namespace enigma::display
