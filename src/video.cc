/*
 * Copyright (C) 2002,2004 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
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

#include "video.hh"

#include <cassert>
#include <cstdio>
#include <memory>
#include <sstream>

#include "ecl_system.hh"
#include "ecl_video.hh"
#include "enigma.hh"
#include "main.hh"
#include "options.hh"
#include "resource_cache.hh"
#include "display.hh"
#include "gui/LevelPreviewCache.hh"

using namespace ecl;
using namespace enigma;

/* -------------------- MouseCursor -------------------- */

namespace {

class MouseCursor {
public:
    MouseCursor(ecl::Screen *screen) : screen(screen) { SDL_AddEventWatch(event_filter, this); }
    ~MouseCursor() { SDL_DelEventWatch(event_filter, this); }

    void set_image(ecl::Surface *s, int hotx_, int hoty_);
    void move(int newx, int newy);
    void redraw();  // Redraw if position/image changed
    void draw();    // Draw cursor if visible
    void show();
    void hide();
    Rect get_rect() const;
    Rect get_oldrect() const;

    bool has_changed() { return changed; }
    int get_x() const { return x; }
    int get_y() const { return y; }

private:
    // This function is installed as an event watch by the constructor. It
    // tracks mouse events to keep the position of the mouse cursor updated.
    static int event_filter(void *data, SDL_Event *e) {
        MouseCursor *cursor = static_cast<MouseCursor *>(data);
        if (e->type == SDL_MOUSEMOTION) {
            cursor->move(e->motion.x, e->motion.y);
            cursor->redraw();
        }
        return 1;
    }

    // Private methods
    void grab_bg();
    void init_bg();
    void restore_bg();

    // Variables
    ecl::Screen *screen;
    std::unique_ptr<Surface> background;  // Copy of screen contents behind cursor
    std::unique_ptr<Surface> cursor;      // Pixmap of the cursor

    int x = 0, y = 0;
    int oldx = 0, oldy = 0;

    // Coordinates of hotspot inside cursor image
    int hotx = 0, hoty = 0;
    int visible = 0;
    bool changed = true;
};

void MouseCursor::set_image(ecl::Surface *s, int hx, int hy) {
    cursor.reset(s);
    hotx = hx;
    hoty = hy;

    if (visible > 0)
        init_bg();
}

void MouseCursor::draw() {
    if (visible > 0) {
        grab_bg();

        GC gc(screen->get_surface());
        blit(gc, x - hotx, y - hoty, cursor.get());
        screen->update_rect(get_rect());

        changed = false;
    }
}

void MouseCursor::redraw() {
    if (visible > 0 && changed) {
        restore_bg();
        draw();
    }
}

void MouseCursor::move(int newx, int newy) {
    x = (int)((double) (newx * screen->size().w) / screen->window_size().w + 0.5);
    y = (int)((double) (newy * screen->size().h) / screen->window_size().h + 0.5);
    changed = true;
}

void MouseCursor::show() {
    if (++visible == 1) {
        init_bg();
        changed = true;
    }
}

void MouseCursor::hide() {
    if (--visible == 0) {
        changed = true;
        restore_bg();
        background.reset(nullptr);
    }
}

Rect MouseCursor::get_rect() const {
    return Rect(x - hotx, y - hoty, cursor->width(), cursor->height());
}

Rect MouseCursor::get_oldrect() const {
    return Rect(oldx - hotx, oldy - hoty, cursor->width(), cursor->height());
}

void MouseCursor::init_bg() {
    assert(visible > 0);
    assert(cursor != 0);

    background.reset(ecl::MakeSurface(cursor->width(), cursor->height()));
    assert(background);
    grab_bg();
}

void MouseCursor::grab_bg() {
    assert(background != 0);

    GC gc(background.get());
    blit(gc, 0, 0, screen->get_surface(), get_rect());

    oldx = x;
    oldy = y;
}

void MouseCursor::restore_bg() {
    if (background) {
        GC gc(screen->get_surface());
        blit(gc, oldx - hotx, oldy - hoty, background.get());
        screen->update_rect(get_oldrect());
    }
}

}  // namespace

/* -------------------- Local Variables -------------------- */

namespace {

/*! List of available video modes. */
VMInfo video_modes[] = {
    {
     VM_320x240, 10, 320, 240,     // id, preffilenr, w, h
     16, VTS_16,                   // tilesize, tiletype
     "320x240", "CGA", "4:3",      // name, fsname, fs only
     Rect(0, 0, 320, 240),         // display area
     0, 0,                         // menu background image offsets
     {60, 39, 2, "-60x39"},        // thumbnail size/extension
     Rect(0, 0, 320, 208),         // game area
     Rect(0, 208, 320, 32),        // statusbar area
     Rect(3, 212, 65, 21),         // time area
     Rect(52, 210, 14, 21),        // modes area
     Rect(50, 220, 15, 21),        // moves area
     Rect(94, 216, 245, 26),       // inventory area
     Rect(90, 217, 222, 17),       // text area
     0,                            // statusbar coffsety
     true,                         // available fullscreen
     "-10-0-"                      // fallback modes fullscreen
    },
    {
     VM_640x480, 0, 640, 480,      // id, preffilenr, w, h
     32, VTS_32,                   // tilesize, tiletype
     "640x480", "VGA", "4:3",      // name, fsname, fs only
     Rect(0, 0, 640, 480),         // display area
     0, 0,                         // menu background image offsets
     {120, 78, 4, "-120x78"},      // thumbnail size/extension
     Rect(0, 0, 640, 416),         // game area
     Rect(0, 416, 640, 64),        // statusbar area
     Rect(10, 428, 117, 43),       // time area
     Rect(100, 422, 28, 43),       // modes area
     Rect(100, 425, 30, 43),       // moves area
     Rect(188, 433, 490, 52),      // inventory area
     Rect(180, 436, 444, 35),      // text area
     0,                            // statusbar coffsety
     true,                         // available fullscreen
     "-0-"                         // fallback modes fullscreen
    },
    {
     VM_640x512, 1, 640, 512,      // id, preffilenr, w, h
     32, VTS_32,                   // tilesize, tiletype
     "640x512", "640x512", "5:4",  // name, fsname, fs only
     Rect(0, 0, 640, 480),         // display area
     0, 0,                         // menu background image offsets
     {120, 78, 4, "-120x78"},      // thumbnail size/extension
     Rect(0, 0, 640, 416),         // game area
     Rect(0, 416, 640, 64),        // statusbar area
     Rect(15, 420, 110, 40),       // time area
     Rect(100, 420, 31, 40),       // modes area
     Rect(100, 420, 30, 40),       // moves area
     Rect(200, 433, 490, 52),      // inventory area
     Rect(150, 434, 475, 35),      // text area
     0,                            // statusbar coffsety
     false,                        // available fullscreen: 640x512 is deprecated!
     "-0-"                         // fallback modes fullscreen
    },
    {
     VM_800x600, 2, 800, 600,      // id, preffilenr, w, h
     40, VTS_40,                   // tilesize, tiletype
     "800x600", "SVGA", "4:3",     // name, fsname, fs only
     Rect(0, 0, 800, 600),         // display area
     0, 0,                         // menu background image offsets
     {120, 78, 4, "-120x78"},      // thumbnail size/extension
     Rect(0, 0, 800, 520),         // game area
     Rect(0, 520, 800, 80),        // statusbar area
     Rect(16, 540, 140, 40),       // time area
     Rect(140, 530, 31, 40),       // modes area
     Rect(140, 540, 30, 40),       // moves area
     Rect(235, 539, 610, 46),      // inventory area
     Rect(225, 547, 555, 39),      // text area
     0,                            // statusbar coffsety
     true,                         // available fullscreen
     "-2-0-"                       // fallback modes fullscreen
    },
    {
     VM_960x720, 4, 960, 720,      // id, preffilenr, w, h
     48, VTS_48,                   // tilesize, tiletype
     "960x720", "960x720", "4:3",  // name, fsname, fs only
     Rect(0, 0, 960, 720),         // display area
     -192, -144,                   // menu background image offsets
     {120, 78, 4, "-120x78"},      // thumbnail size/extension
     Rect(0, 0, 960, 624),         // game area
     Rect(0, 624, 960, 96),        // statusbar area
     Rect(18, 640, 170, 60),       // time area
     Rect(166, 636, 34, 60),       // modes area
     Rect(153, 640, 30, 60),       // moves area
     Rect(282, 650, 710, 46),      // inventory area
     Rect(270, 656, 666, 40),      // text area
     0,                            // statusbar coffsety
     true,                         // available fullscreen
     "-4-2-0-"                     // fallback modes fullscreen
    },
    {
     VM_1024x768, 3, 1024, 768,    // id, preffilenr, w, h
     48, VTS_48,                   // tilesize, tiletype
     "1024x768", "XGA", "4:3",     // name, fsname, fs only
     Rect(32, 0, 960, 720),        // display area
     -128, -96,                    // menu background image offsets
     {120, 78, 4, "-120x78"},      // thumbnail size/extension
     Rect(32, 0, 960, 624),        // game area
     Rect(32, 624, 960, 96),       // statusbar area
     Rect(50, 640, 170, 60),       // time area
     Rect(198, 634, 34, 60),       // modes area
     Rect(185, 640, 30, 60),       // moves area
     Rect(314, 650, 710, 46),      // inventory area
     Rect(302, 655, 666, 40),      // text area
     0,                            // statusbar coffsety
     false,                        // available fullscreen
     "-3-2-0-"                     // fallback modes fullscreen
    },
    {
     VM_1280x720, 5, 1280, 720,    // id, preffilenr, w, h
     48, VTS_48,                   // tilesize, tiletype
     "1280x720", "HD720", "16:10", // name, fsname, fs only
     Rect(160, 0, 960, 720),       // display area
     -192, -96,                    // menu background image offsets
     {120, 78, 4, "-120x78"},      // thumbnail size/extension
     Rect(160, 0, 960, 624),       // game area
     Rect(160, 624, 960, 96),      // statusbar area
     Rect(178, 640, 170, 60),      // time area
     Rect(313, 640, 30, 60),       // modes area
     Rect(313, 640, 30, 60),       // moves area
     Rect(400, 650, 710, 46),      // inventory area
     Rect(388, 654, 710, 40),      // text area
     0,                            // statusbar coffsety
     false,                        // available fullscreen
     "-5-0-"                       // fallback modes fullscreen
    },
    {
     VM_1280x960, 6, 1280, 960,     // id, preffilenr, w, h
     64, VTS_64,                    // tilesize, tiletype
     "1280x960", "1280x960", "4:3", // name, fsname, fs only
     Rect(0, 0, 1280, 960),         // display area
     -400, -90,                     // menu background image offsets
     {160, 104, 5, "-160x104"},     // thumbnail size/extension
     Rect(0, 0, 1280, 832),         // game area
     Rect(0, 832, 1280, 128),       // statusbar area
     Rect(24, 857, 227, 80),        // time area //TODO
     Rect(204, 853, 40, 80),        // modes area //TODO
     Rect(204, 853, 40, 80),        // moves area //TODO
     Rect(376, 867, 947, 61),       // inventory area //TODO
     Rect(360, 884, 888, 53),       // text area //TODO
     1,                             // statusbar coffsety
     true,                          // available fullscreen
     "-6-4-2-0-"                    // fallback modes fullscreen
    },
    {
     VM_1280x1024, 8, 1280, 1024,   // id, preffilenr, w, h
     64, VTS_64,                    // tilesize, tiletype
     "1280x1024", "SXGA", "5:4",    // name, fsname, fs only
     Rect(0, 0, 1280, 960),         // display area
     -400, -26,                     // menu background image offsets
     {160, 104, 5, "-160x104"},     // thumbnail size/extension
     Rect(0, 0, 1280, 832),         // game area
     Rect(0, 832, 1280, 128),       // statusbar area
     Rect(18, 640, 170, 60),        // time area //TODO
     Rect(153, 640, 30, 60),        // modes area //TODO
     Rect(153, 640, 30, 60),        // moves area //TODO
     Rect(324, 650, 710, 46),       // inventory area //TODO
     Rect(360, 655, 888, 40),       // text area //TODO
     0,                             // statusbar coffsety
     false,                         // available fullscreen
     "-8-0-"                        // fallback modes fullscreen
    },
    {
     VM_1440x960, 7, 1440, 960,     // id, preffilenr, w, h
     64, VTS_64,                    // tilesize, tiletype
     "1440x960", "1440x960", "3:2", // name, fsname, fs only
     Rect(80, 0, 1280, 960),        // display area
     -400, -90,                     // menu background image offsets
     {160, 104, 5, "-160x104"},     // thumbnail size/extension
     Rect(80, 0, 1280, 832),        // game area
     Rect(80, 832, 1280, 128),      // statusbar area
     Rect(104, 853, 227, 80),       // time area //TODO
     Rect(284, 853, 40, 80),        // modes area //TODO
     Rect(284, 853, 40, 80),        // moves area //TODO
     Rect(480, 867, 947, 61),       // inventory area //TODO
     Rect(384, 873, 947, 53),       // text area //TODO
     0,                             // statusbar coffsety
     false,                         // available fullscreen
     "-7-0-"                        // fallback modes fullscreen
    },
    {
     VM_1680x1050, 9, 1680, 1050,   // id, preffilenr, w, h
     64, VTS_64,                    // tilesize, tiletype
     "1680x1050", "WSXGA+", "16:10",// name, fsname, fs only
     Rect(200, 0, 1280, 960),       // display area
     -0, -0,                        // menu background image offsets
     {160, 104, 5, "-160x104"},     // thumbnail size/extension
     Rect(200, 0, 1280, 832),       // game area
     Rect(200, 832, 1280, 128),     // statusbar area
     Rect(224, 853, 227, 80),       // time area //TODO
     Rect(404, 853, 40, 80),        // modes area //TODO
     Rect(404, 853, 40, 80),        // moves area //TODO
     Rect(600, 867, 947, 61),       // inventory area //TODO
     Rect(504, 873, 947, 53),       // text area //TODO
     0,                             // statusbar coffsety
     false,                         // available fullscreen
     "-9-0-"                        // fallback modes fullscreen
    }
};

/*! List of available tilesets. */
std::vector<VideoTileset> video_tilesets {
    // Id, name, tiletype, tilesize, optimal fullscreen mode, initscript, dir, fallback
    {VTS_16_130, "16x16 Enigma 1.30", VTS_16, 16,  VM_320x240, "models-16.lua", "gfx16/", VTS_32_130, true},
    {VTS_32_130, "32x32 Enigma 1.30", VTS_32, 32,  VM_640x480, "models-32.lua", "gfx32/", VTS_NONE,   true},
    {VTS_40_130, "40x40 Enigma 1.30", VTS_40, 40,  VM_800x600, "models-40.lua", "gfx40/", VTS_NONE,   true},
    {VTS_48_130, "48x48 Enigma 1.30", VTS_48, 48,  VM_960x720, "models-48.lua", "gfx48/", VTS_NONE,   true},
    {VTS_64_130, "64x64 Enigma 1.30", VTS_64, 64, VM_1280x960, "models-64.lua", "gfx64/", VTS_48_130, true},
};

}  // namespace

namespace enigma {

class VideoEngineImpl : public VideoEngine {
public:
    VideoEngineImpl();
    ~VideoEngineImpl();

    void Init() override;

    void Shutdown() override;

    ecl::Screen *GetScreen() override;

    void SetCaption(const std::string &text) override;
    const std::string &GetCaption() override;

    std::vector<WindowSize> EnumerateFullscreenModes() override;
    std::vector<VideoTilesetId> EnumerateAllTilesets() override;
    std::vector<VideoTilesetId> EnumerateFittingTilesets(WindowSize &display_mode) override;
    WindowSize ActiveDisplayMode() override;
    WindowSize ActiveWindowSize() override;
    void SetVideoTileset(VideoTileset* vts);
    void SetDisplayMode(const WindowSize &display_mode, bool fullscreen, VideoTilesetId id) override;
    void ApplySettings() override;
    void SaveWindowSizePreferences() override;
    void Resize(Sint32 width, Sint32 height) override;
    WindowSize SelectedWindowSize() override;
    int ActiveWindowSizeFactor() override;

    const VMInfo *GetInfo() override;
    const VMInfo *GetInfo(FullscreenMode mode) { return &video_modes[mode]; }
    VideoTileset *GetTileset() override;
    const VideoTilesetId GetTilesetId() override;

    bool SetFullscreen(bool enabled) override;
    bool ToggleFullscreen() override { return SetFullscreen(!IsFullscreen()); }
    bool IsFullscreen() override;

    ecl::Surface *BackBuffer() override;

    void Screenshot(const std::string &file_name) override;

    void SetMouseCursor(ecl::Surface *s, int hotx, int hoty) override;
    void HideMouse() override;
    void ShowMouse() override;
    int Mousex() override { return cursor->get_x(); }
    int Mousey() override { return cursor->get_y(); }

    bool GetInputGrab() override { return SDL_GetWindowGrab(window) == SDL_TRUE; }

    bool SetInputGrab(bool enabled) override {
        // From SDL 1 to SDL 2, the old grabbing commands were
        // split up into two separate pairs, one setting the mouse mode,
        // and one defining the window to be grabbed. As long as Enigma
        // uses only one window, we use both SDL_Set... commands in parallel,
        // but only SDL_GetWindowGrab to retrieve the current state. When
        // Enigma starts using several windows, this needs to be adapted.
        bool old_state = GetInputGrab();
        SDL_SetWindowGrab(window, enabled ? SDL_TRUE : SDL_FALSE);
        SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE);
        return old_state;
    }

private:
    bool OpenWindow(int width, int height, bool fullscreen);
    void CloseWindow();

    ecl::Screen *screen;
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::unique_ptr<Surface> back_buffer;
    std::unique_ptr<MouseCursor> cursor;
    std::string window_caption;
    VideoTileset *video_tileset;
    VideoTilesetId video_tileset_id;
};

VideoEngineImpl::VideoEngineImpl() :
    screen(NULL),
    window(NULL),
    renderer(NULL),
    video_tileset(nullptr),
    video_tileset_id(VTS_NONE)
{}

VideoEngineImpl::~VideoEngineImpl() {
    CloseWindow();
}

void VideoEngineImpl::Init() {
    bool isFullScreen = app.prefs->getBool("FullScreen");
    // Sanitize and save preferences for fullscreen mode.
    FullscreenMode fmode = ParseVideomodesFallbackString(app.prefs->getString("VideoModesFullscreen"), true);
    VideoTileset* fvts = VideoTilesetByName(app.prefs->getString("FullscreenTileset"));
    if (fmode != VM_NONE) {
        if ((fvts == nullptr) || (fvts->tt != video_modes[fmode].tt))
            fvts = StandardTileset(video_modes[fmode].tt);
        app.selectedFullscreenMode = {video_modes[fmode].width, video_modes[fmode].height};
        app.selectedFullscreenTilesetId = fvts->id;
    } else {
        app.selectedFullscreenMode = {0, 0};
        app.selectedFullscreenTilesetId = VTS_NONE;
        isFullScreen = false;
    }
    // Sanitize and save preferences for window mode.
    int w = app.prefs->getInt("WindowWidth");
    int h = app.prefs->getInt("WindowHeight");
    int sizefactor = app.prefs->getInt("WindowSizeFactor");
    VideoTileset* wvts = VideoTilesetByName(app.prefs->getString("WindowTileset"));
    if ((sizefactor > 0) && (wvts != nullptr)) {
        w = sizefactor * wvts->tilesize * 20;
        h = sizefactor * wvts->tilesize * 15;
    }
    if ((w <= 0) || (h <= 0) || (wvts == nullptr)) {
        FullscreenMode wmode = ParseVideomodesFallbackString(app.prefs->getString("VideoModesWindow"), false);
        if (wmode != VM_NONE) {
            // Use older settings from 1.00..1.21.
            // Guess the correct tile type, then resize.
            // In most cases, this will result in the old window sizes,
            // except 1024x768, which will be replaced by 960x780.
            wvts = StandardTileset(video_modes[wmode].tt);
            wmode = wvts->OptimalFullscreenMode;
            w = video_modes[wmode].width;
            h = video_modes[wmode].height;
            sizefactor = 1;
        } else {
            // Nothing is set, or nothing set is working. Maybe first time?
            // Just choose something sensible and halfway representable.
            w = 800;
            h = 600;
            wvts = StandardTileset(VTS_40);
            sizefactor = 1;
        }
    }
    app.selectedWindowSizeFactor = sizefactor;
    app.selectedWindowTilesetId = wvts->id;
    // Try to activate the chosen mode.
    bool success = false;
    if (isFullScreen && (fmode != VM_NONE)) {
        // Try fullscreen mode.
        SetVideoTileset(fvts);
        success = OpenWindow(video_modes[fmode].width, video_modes[fmode].height, true);
        if (!success)
            Log << "Preferred video mode (" << w << "x" << h << " "
                << (isFullScreen?"fullscreen":"window") << " mode) failed.";
    }
    if (!success) {
        // Probably just window mode; or maybe fullscreen mode failed.
        SetVideoTileset(wvts);
        success = OpenWindow(w, h, false);
    }
    if (!success) {
        // Try two fallback solutions: 640x480, then 320x240, in windowed mode.
        Log << "Preferred video mode (" << w << "x" << h << " "
            << (isFullScreen?"fullscreen":"window") << " mode) failed.";
        SetVideoTileset(StandardTileset(VTS_32));
        if (!OpenWindow(640, 480, false)) {
            SetVideoTileset(StandardTileset(VTS_16));
            if (!OpenWindow(320, 240, false)) {
                // Give up.
                fprintf(stderr, "Could not find a working display mode during init, sorry.");
                exit(1);
            }
        }
    }
    // Save the de-facto modes (just in "app", not as preference).    
    if (video_engine->IsFullscreen()) {
        app.selectedFullscreenMode = ActiveWindowSize();
        app.selectedFullscreenTilesetId = GetTilesetId();
    } else {
        app.selectedWindowTilesetId == GetTilesetId();
    }
// Mac icon is set via Makefile
#ifndef MACOSX
    if (Surface *icon = enigma::GetImage("enigma_marble")) {
        SDL_SetWindowIcon(window, icon->get_surface());
    }
#endif
}

void VideoEngineImpl::Shutdown() {
    CloseWindow();
}

ecl::Screen *VideoEngineImpl::GetScreen() {
    return screen;
}

void VideoEngineImpl::SetCaption(const std::string &text) {
    SDL_SetWindowTitle(window, text.c_str());
    window_caption = text;
}

const std::string &VideoEngineImpl::GetCaption() {
    return window_caption;
}

std::vector<WindowSize> VideoEngineImpl::EnumerateFullscreenModes() {
    std::vector<WindowSize> modes;
    const int display = 0;
    const int num_modes = SDL_GetNumDisplayModes(display);
    for (int i = 0; i < num_modes; ++i) {
        SDL_DisplayMode mode;
        if (SDL_GetDisplayMode(display, i, &mode) == 0) {
            WindowSize new_mode = {mode.w, mode.h};
            FullscreenMode corresponding_mode = FindFullscreenMode(new_mode);
            if (   (corresponding_mode != VM_NONE)
                && (video_modes[corresponding_mode].f_available)
                && (std::find(modes.begin(), modes.end(), new_mode) == modes.end()))
            {
                Log << "Found fullscreen mode: " << new_mode.width
                    << "x" << new_mode.height << "\n";
                modes.push_back(new_mode);
            }
        }
    }
    return modes;
}

std::vector<VideoTilesetId> VideoEngineImpl::EnumerateAllTilesets() {
    std::vector<VideoTilesetId> ids;
    for (int i = VTSID_FIRST; i < VTSID_COUNT; i++) {
        ids.push_back((VideoTilesetId) i);
    }
    return ids;
}

std::vector<VideoTilesetId> VideoEngineImpl::EnumerateFittingTilesets(WindowSize &display_mode) {
    std::vector<VideoTilesetId> ids;
    const VMInfo* vminfo = GetInfo(FindFullscreenMode(display_mode));
    assert(vminfo);
    for (int i = VTSID_FIRST; i < VTSID_COUNT; i++) {
        VideoTileset *vts = VideoTilesetById((VideoTilesetId) i);
        assert(vts);
        if (vts->tt == vminfo->tt)
            ids.push_back((VideoTilesetId) i);
    }
    return ids;
}

WindowSize VideoEngineImpl::ActiveWindowSize() {
    return {screen->window_width(), screen->window_height()};
}

WindowSize VideoEngineImpl::ActiveDisplayMode() {
    return {screen->width(), screen->height()};
}

void VideoEngineImpl::SetVideoTileset(VideoTileset* vts) {
    if (vts == nullptr) {
        fprintf(stderr, "Error in video tileset selection, sorry.");
        exit(1);
    }
    Log << "Set video tile set " << vts->name << ".\n";
    video_tileset_id = vts->id;
    video_tileset = vts;
}

void VideoEngineImpl::SetDisplayMode(const WindowSize &display_mode, bool fullscreen, VideoTilesetId vtsid) {
    // Save old configuration in case the new one fails.
    bool old_fs = IsFullscreen();
    WindowSize old_mode = ActiveDisplayMode();
    VideoTileset* vts = VideoTilesetById(vtsid);
    VideoTileset* old_vts = GetTileset();
    // Close old window, then try new config.
    CloseWindow();
    SetVideoTileset(vts);
    if (OpenWindow(display_mode.width, display_mode.height, fullscreen))
        return;
    // Something went wrong with the new configuration -- try old config.
    SetVideoTileset(old_vts);
    if (OpenWindow(old_mode.width, old_mode.height, old_fs))
        return;
    // Try old config with window.
    SetVideoTileset(old_vts);
    if (OpenWindow(old_mode.width, old_mode.height, false))
        return;
    // Try two fallback solutions: 640x480, then 320x240, in windowed mode.
    SetVideoTileset(StandardTileset(VTS_32));
    if (OpenWindow(640, 480, false))
        return;
    SetVideoTileset(StandardTileset(VTS_16));
    if (OpenWindow(320, 240, false))
        return;
    // Give up.
    fprintf(stderr, "Could not find a working display mode, sorry.");
    exit(1);
}

WindowSize VideoEngineImpl::SelectedWindowSize() {
    VideoTileset* vts = VideoTilesetById(app.selectedWindowTilesetId);
    assert(vts);
    int tilesize = vts->tilesize;
    if (app.selectedWindowSizeFactor == 0) {
        return ActiveWindowSize();
    }
    return {app.selectedWindowSizeFactor * tilesize * 20, app.selectedWindowSizeFactor * tilesize * 15};
}

int VideoEngineImpl::ActiveWindowSizeFactor() {
    VideoTileset* vts = VideoTilesetById(app.selectedWindowTilesetId);
    assert(vts);
    return (int) (ActiveWindowSize().width / 20 / vts->tilesize);
}

void VideoEngineImpl::ApplySettings() {
    bool wantFullscreen = app.prefs->getBool("FullScreen");
    if (wantFullscreen && !IsFullscreen())
        SaveWindowSizePreferences();
    // Do we have to change the display mode and/or active tileset?
    if (wantFullscreen && IsFullscreen()
        && (app.selectedFullscreenMode == ActiveWindowSize())
        && (app.selectedFullscreenTilesetId == GetTilesetId()))
        return;
    if (!wantFullscreen && !IsFullscreen()
        && (SelectedWindowSize() == ActiveWindowSize())
        && (app.selectedWindowTilesetId == GetTilesetId()))
        return;
    // Change display mode.
    if (wantFullscreen) {
        SetDisplayMode(app.selectedFullscreenMode, true, app.selectedFullscreenTilesetId);
    } else {
        SetDisplayMode(SelectedWindowSize(), false, app.selectedWindowTilesetId);
    }
    // The display might have been set to a different setting. Save these.
    app.prefs->setProperty("FullScreen", IsFullscreen());
    if (IsFullscreen()) {
        app.selectedFullscreenMode = ActiveWindowSize();
    } else {
        SaveWindowSizePreferences();
    }
    gui::LevelPreviewCache::instance()->clear();
    display::Shutdown();
    display::Init();
}

void VideoEngineImpl::SaveWindowSizePreferences() {
    WindowSize ws = ActiveWindowSize();
    app.prefs->setProperty("WindowWidth", ws.width);
    app.prefs->setProperty("WindowHeight", ws.height);
}

void VideoEngineImpl::Resize(Sint32 width, Sint32 height) {
    SDL_SetWindowSize(window, width, height);
    SDL_RenderSetLogicalSize(renderer, width, height);
}

const VMInfo *VideoEngineImpl::GetInfo() {
    FullscreenMode mode = FindClosestFullscreenMode(ActiveDisplayMode());
    assert(mode != VM_NONE);
    return &video_modes[mode];
}

VideoTileset *VideoEngineImpl::GetTileset() {
    assert(video_tileset);
    return video_tileset;
}

const VideoTilesetId VideoEngineImpl::GetTilesetId() {
    return video_tileset_id;
}

bool VideoEngineImpl::SetFullscreen(bool enabled) {
    // TODO(sdl2): change display mode if necessary
    if (!IsFullscreen())
        SaveWindowSizePreferences();

    SDL_SetWindowFullscreen(window, enabled ? SDL_WINDOW_FULLSCREEN : 0);
    return IsFullscreen();
}

bool VideoEngineImpl::IsFullscreen() {
    Uint32 flags = SDL_GetWindowFlags(window);
    return (flags & SDL_WINDOW_FULLSCREEN) != 0;
}

ecl::Surface *VideoEngineImpl::BackBuffer() {
    if (!back_buffer) {
        back_buffer.reset(Duplicate(screen->get_surface()));
    }
    return back_buffer.get();
}

void VideoEngineImpl::Screenshot(const std::string &file_name) {
    // auto-create the directory if necessary
    std::string directory;
    if (ecl::split_path(file_name, &directory, 0) && !ecl::FolderExists(directory)) {
        ecl::FolderCreate(directory);
    }

    ecl::Rect rect = GetInfo()->area;
    ecl::SavePNG(ecl::Grab(screen->get_surface(), rect), file_name);
    enigma::Log << "Wrote screenshot to '" << file_name << "'\n";
}

void VideoEngineImpl::SetMouseCursor(ecl::Surface *s, int hotx, int hoty) {
    cursor->set_image(s, hotx, hoty);
    cursor->redraw();
}

void VideoEngineImpl::HideMouse() {
    cursor->hide();
    cursor->redraw();
}

void VideoEngineImpl::ShowMouse() {
    cursor->show();
    cursor->redraw();
}

bool VideoEngineImpl::OpenWindow(int width, int height, bool fullscreen) {
    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;

    // Try to initialize video mode, return error code on failure
    Log << "Opening in " << (fullscreen?"fullscreen":"window") << " mode, "
        << width << "x" << height << ".\n";
    window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
                              flags);
    if (!window)
        return false;

    assert(GetTileset());
    int tilesize = GetTileset()->tilesize;
    screen = new Screen(window, tilesize * 20, tilesize * 15);

    renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (!renderer)
        return false;

    FullscreenMode video_mode = FindClosestFullscreenMode({width, height});
    const VMInfo *vminfo = GetInfo(video_mode);
    SDL_RenderSetLogicalSize(renderer, vminfo->width, vminfo->height);

    cursor.reset(new MouseCursor(screen));
    int x, y;
    SDL_GetMouseState(&x, &y);
    cursor->move(x, y);
    cursor->set_image(enigma::LoadImage("cur-magic"), 4, 4);
    video_engine->ShowMouse();
    SDL_ShowCursor(0);

    return true;
}

void VideoEngineImpl::CloseWindow() {
    assert(window);
    assert(renderer);

    SetInputGrab(false);
    cursor = nullptr;
    back_buffer = nullptr;
    video_tileset = nullptr;
    delete screen;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

// -------------------- Global variables & functions --------------------

VideoEngine *video_engine = nullptr;

void VideoInit() {
    video_engine = new VideoEngineImpl;
    video_engine->Init();
    Log << "VideoInit done.\n";
}

void ShowLoadingScreen(const char *text, int /* progress */) {
    ecl::Screen *screen = video_engine->GetScreen();
    ecl::GC gc(screen->get_surface());

    blit(gc, 0, 0, GetImage("menu_bg", ".jpg"));

    ecl::Surface *logo = GetImage("enigma_logo3");
    const int x = (screen->width() - logo->width()) / 2;
    const int y = (screen->height() - logo->height()) / 2;
    blit(gc, x, y - logo->height(), logo);

    ecl::Font *font = GetFont("menufontsel");
    font->render(gc, (screen->width() - font->get_width(text)) / 2, y, text);

    screen->update_all();
    screen->flush_updates();
}

FullscreenMode FindClosestFullscreenMode(const WindowSize &display_mode) {
    for (int i = VM_LAST; i >= VM_FIRST; --i) {
        const VMInfo &info = video_modes[i];
        if (info.width <= display_mode.width && info.height <= display_mode.height) {
            return info.mode;
        }
    }
    return VM_NONE;
}

FullscreenMode FindFullscreenMode(const WindowSize &display_mode) {
    for (int i = VM_LAST; i >= VM_FIRST; --i) {
        const VMInfo &info = video_modes[i];
        if (info.width == display_mode.width && info.height == display_mode.height) {
            return info.mode;
        }
    }
    return VM_NONE;
}

FullscreenMode PrefFileNrToMode(int prefnr) {
    for (int i = VM_LAST; i >= VM_FIRST; --i) {
        const VMInfo &info = video_modes[i];
        if (info.preffilenr == prefnr)
            return info.mode;
    }
    return VM_NONE;
}

FullscreenMode ParseVideomodesFallbackString(std::string modes, bool available_only, int seq) {
    // seq defaults to 1.
    // In Enigma 1.3, we actually use our own fallback system, so we only
    // ever use the very first known and activated mode we find, and work
    // through our own fallbacks from there on. Therefore, this function
    // is used with seq = 1 only.
    if (modes.length() > 1) {
        std::istringstream ms(modes);
        FullscreenMode mode = VM_NONE;
        int prefnr = -1;
        ms.ignore();  // leading '-'
        while (ms && seq > 0) {
            ms >> std::dec >> prefnr;
            ms.ignore();
            mode = PrefFileNrToMode(prefnr);
            if ((!available_only) || video_modes[mode].f_available) {
                if (seq == 1)
                    return mode;
                --seq;
            }
        }
    }
    return VM_NONE;
}

VideoTileset* VideoTilesetById(VideoTilesetId id) {
    for(auto it = std::begin(video_tilesets); it != std::end(video_tilesets); ++it)
        if (it->id == id)
            return &(*it);
    return nullptr;
};

VideoTileset* VideoTilesetByName(std::string name) {
    if (name == "16x16 Standard")  return StandardTileset(VTS_16);
    if (name == "32x32 Standard")  return StandardTileset(VTS_32);
    if (name == "40x40 Standard")  return StandardTileset(VTS_40);
    if (name == "48x48 Standard")  return StandardTileset(VTS_48);
    if (name == "64x64 Standard")  return StandardTileset(VTS_64);
    for(auto it = std::begin(video_tilesets); it != std::end(video_tilesets); ++it)
        if (it->name == name)
            return &(*it);
    return nullptr;
}

VideoTileset* StandardTileset(VideoTileType tt) {
    for(auto it = std::begin(video_tilesets); it != std::end(video_tilesets); ++it)
        if ((it->tt == tt) && it->is_standard)
            return &(*it);
    fprintf(stderr, "Error in choosing standard tileset, sorry.");
    exit(1);
}

std::string VideoTilesetPrefName(VideoTilesetId id) {
    VideoTileset* vts = VideoTilesetById(id);
    if (vts == nullptr)
        return "Error while saving tileset preferences.";
    std::stringstream ss;
    if (vts->is_standard)
        ss << (vts->tilesize) << "x" << (vts->tilesize) << " Standard";
    else
        ss << (vts->name);
    return ss.str();
}

}  // namespace enigma
