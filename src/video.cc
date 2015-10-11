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

// This file contains the code for initializing the video hardware and for
// various visual effects in the widest sense. This includes
//
// a) managing and rendering the mouse cursor,
// b) changing between windowed and fullscreen mode,
//
// The code in this file is independent of the game. For game-specific display
// code, refer to file display.cc

#include "video.hh"

#include <cassert>
#include <cstdio>
#include <sstream>

#include "ecl_system.hh"
#include "ecl_video.hh"
#include "enigma.hh"
#include "main.hh"
#include "options.hh"
#include "resource_cache.hh"

#define SCREEN ecl::Screen::get_instance()

using namespace std;
using namespace ecl;
using namespace video;
using namespace enigma;

/* -------------------- Video Engine -------------------- */

namespace {

class Video_SDL {
public:
    Video_SDL();
    ~Video_SDL();

    bool init(int w, int h, bool fullscreen);
    void toggle_fullscreen();
    void set_fullscreen(bool on_off);
    bool is_fullscreen() const;
    void set_caption(const char *str);
    const string &get_caption() const { return caption; }
    ecl::Screen *get_screen() { return screen; }

private:
    ecl::Screen *screen;
    SDL_Renderer *renderer;
    string caption;
    bool initialized;
};

Video_SDL::Video_SDL() : screen(NULL), renderer(NULL), initialized(false) {
}

Video_SDL::~Video_SDL() {
    SDL_SetWindowGrab(screen->window(), SDL_FALSE);
    delete screen;
}

void Video_SDL::set_caption(const char *str) {
    caption = str;
    if (initialized)
        SDL_SetWindowTitle(screen->window(), str);
}

bool Video_SDL::init(int w, int h, bool fullscreen) {
    Uint32 flags = SDL_WINDOW_SHOWN;
    if (fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;

    // TODO(SDL2): close existing windows
    // Try to initialize video mode, return error code on failure
    SDL_Window *window = SDL_CreateWindow(caption.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, w, h,
            flags);
    if (!window)
        return false;
    renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (!renderer)
        return false;
    screen = new Screen(window);
    initialized = true;
    return true;
}

bool Video_SDL::is_fullscreen() const {
    Uint32 flags = SDL_GetWindowFlags(screen->window());
    return (flags & SDL_WINDOW_FULLSCREEN) != 0;
}

void Video_SDL::set_fullscreen(bool on_off) {
    if (on_off != is_fullscreen())
        toggle_fullscreen();
}

void Video_SDL::toggle_fullscreen() {
    SDL_SetWindowFullscreen(screen->window(), is_fullscreen() ? 0 : SDL_WINDOW_FULLSCREEN);
}

}  // namespace

/* -------------------- MouseCursor -------------------- */

namespace {

class MouseCursor {
public:
    MouseCursor();
    ~MouseCursor();

    void set_image(ecl::Surface *s, int hotx_, int hoty_);
    void move(int newx, int newy);
    void redraw();  // Redraw if position/image changed
    void draw();    // Draw cursor if visible
    void show();
    void hide();
    Rect get_rect() const;
    Rect get_oldrect() const;

    bool has_changed() { return changedp; }
    int get_x() const { return x; }
    int get_y() const { return y; }

private:
    // Private methods
    void grab_bg();
    void init_bg();
    void restore_bg();

    // Variables
    Surface *background;  // Copy of screen contents behind cursor
    Surface *cursor;      // Pixmap of the cursor

    int x, y;
    int oldx, oldy;
    int hotx, hoty;  // Coordinates of hotspot inside cursor image
    int visible;
    bool changedp;
};

MouseCursor::MouseCursor() : background(NULL), cursor(NULL), x(0), y(0) {
    oldx = oldy = 0;
    hotx = hoty = 0;
    visible = 0;
    changedp = true;
}

MouseCursor::~MouseCursor() {
    delete background;
    delete cursor;
}

void MouseCursor::set_image(ecl::Surface *s, int hx, int hy) {
    delete cursor;
    cursor = s;
    hotx = hx;
    hoty = hy;

    if (visible > 0)
        init_bg();
}

void MouseCursor::draw() {
    if (visible > 0) {
        grab_bg();

        GC gc(SCREEN->get_surface());
        blit(gc, x - hotx, y - hoty, cursor);
        SCREEN->update_rect(get_rect());

        changedp = false;
    }
}

void MouseCursor::redraw() {
    if (visible > 0 && changedp) {
        restore_bg();
        draw();
    }
}

void MouseCursor::move(int newx, int newy) {
    x = newx;
    y = newy;
    changedp = true;
}

void MouseCursor::show() {
    if (++visible == 1) {
        init_bg();
        changedp = true;
    }
}

void MouseCursor::hide() {
    if (--visible == 0) {
        changedp = true;
        restore_bg();
        delete background;
        background = 0;
    }
}

Rect MouseCursor::get_rect() const {
    int scrx = x - hotx;
    int scry = y - hoty;
    return Rect(scrx, scry, cursor->width(), cursor->height());
}

Rect MouseCursor::get_oldrect() const {
    int scrx = oldx - hotx;
    int scry = oldy - hoty;
    return Rect(scrx, scry, cursor->width(), cursor->height());
}

void MouseCursor::init_bg() {
    assert(visible > 0);
    assert(cursor != 0);

    delete background;
    background = ecl::MakeSurfaceLike(cursor->width(), cursor->height(), SCREEN->get_surface());
    assert(background);
    grab_bg();
}

void MouseCursor::grab_bg() {
    assert(background != 0);

    GC gc(background);
    blit(gc, 0, 0, SCREEN->get_surface(), get_rect());

    oldx = x;
    oldy = y;
}

void MouseCursor::restore_bg() {
    if (background) {
        GC gc(SCREEN->get_surface());
        blit(gc, oldx - hotx, oldy - hoty, background);
        SCREEN->update_rect(get_oldrect());
    }
}

}  // namespace

/* -------------------- Local Variables -------------------- */

namespace {

Video_SDL *video_engine = 0;
MouseCursor *cursor = 0;
Surface *back_buffer = 0;

/*! List of available video modes. */
video::VMInfo video_modes[] = {{
                                VM_640x480, 640, 480,       // id, w, h
                                32, VTS_32,                 // tilesize, tiletype
                                "640x480", "VGA", "4:3",    // name, fsname, fs only
                                "models-32.lua", "gfx32/",  // initscript, dir
                                Rect(0, 0, 640, 480),       // display area
                                0, 0,                       // menu background image offsets
                                120, 78, 4, "-120x78",      // thumbnail size/extension
                                Rect(0, 0, 640, 416),       // game area
                                Rect(0, 416, 640, 64),      // statusbar area
                                Rect(10, 428, 117, 43),     // time area
                                Rect(100, 422, 28, 43),     // modes area
                                Rect(100, 425, 30, 43),     // moves area
                                Rect(188, 433, 490, 52),    // inventory area
                                Rect(180, 436, 444, 35),    // text area
                                0,                          // statusbar coffsety
                                true, true,                 // available window, fullscreen
                                "-0-", "-0-"                // fallback modes window, fullscreen
                               },
                               {
                                VM_640x512, 640, 512,         // id, w, h
                                32, VTS_32,                   // tilesize, tiletype
                                "640x512", "640x512", "5:4",  // name, fsname, fs only
                                "models-32.lua", "gfx32/",    // initscript, dir
                                Rect(0, 0, 640, 480),         // display area
                                0, 0,                         // menu background image offsets
                                120, 78, 4, "-120x78",        // thumbnail size/extension
                                Rect(0, 0, 640, 416),         // game area
                                Rect(0, 416, 640, 64),        // statusbar area
                                Rect(15, 420, 110, 40),       // time area
                                Rect(100, 420, 31, 40),       // modes area
                                Rect(100, 420, 30, 40),       // moves area
                                Rect(200, 433, 490, 52),      // inventory area
                                Rect(150, 434, 475, 35),      // text area
                                0,                            // statusbar coffsety
                                false, false,                 // 640x512 is deprecated!
                                "-0-", "-0-"                  // fallback modes window, fullscreen
                               },
                               {
                                VM_800x600, 800, 600,       // id, w, h
                                40, VTS_40,                 // tilesize, tiletype
                                "800x600", "SVGA", "4:3",   // name, fsname, fs only
                                "models-40.lua", "gfx40/",  // initscript, dir
                                Rect(0, 0, 800, 600),       // display area
                                0, 0,                       // menu background image offsets
                                120, 78, 4, "-120x78",      // thumbnail size/extension
                                Rect(0, 0, 800, 520),       // game area
                                Rect(0, 520, 800, 80),      // statusbar area
                                Rect(16, 540, 140, 40),     // time area
                                Rect(140, 530, 31, 40),     // modes area
                                Rect(140, 540, 30, 40),     // moves area
                                Rect(235, 539, 610, 46),    // inventory area
                                Rect(225, 547, 555, 39),    // text area
                                0,                          // statusbar coffsety
                                true, true,                 // available window, fullscreen
                                "-2-0-", "-2-0-"            // fallback modes window, fullscreen
                               },
                               {
                                VM_1024x768, 1024, 768,     // id, w, h
                                48, VTS_48,                 // tilesize, tiletype
                                "1024x768", "XGA", "4:3",   // name, fsname, fs only
                                "models-48.lua", "gfx48/",  // initscript, dir
                                Rect(32, 0, 960, 720),      // display area
                                -128, -96,                  // menu background image offsets
                                120, 78, 4, "-120x78",      // thumbnail size/extension
                                Rect(32, 0, 960, 624),      // game area
                                Rect(32, 624, 960, 96),     // statusbar area
                                Rect(50, 640, 170, 60),     // time area
                                Rect(198, 634, 34, 60),     // modes area
                                Rect(185, 640, 30, 60),     // moves area
                                Rect(314, 650, 710, 46),    // inventory area
                                Rect(302, 655, 666, 40),    // text area
                                0,                          // statusbar coffsety
                                true, true,                 // available window, fullscreen
                                "-3-2-0-", "-3-2-0-"        // fallback modes window, fullscreen
                               },
                               {
                                VM_960x720, 960, 720,         // id, w, h
                                48, VTS_48,                   // tilesize, tiletype
                                "960x720", "960x720", "4:3",  // name, fsname, fs only
                                "models-48.lua", "gfx48/",    // initscript, dir
                                Rect(0, 0, 960, 720),         // display area
                                -192, -144,                   // menu background image offsets
                                120, 78, 4, "-120x78",        // thumbnail size/extension
                                Rect(0, 0, 960, 624),         // game area
                                Rect(0, 624, 960, 96),        // statusbar area
                                Rect(18, 640, 170, 60),       // time area
                                Rect(166, 636, 34, 60),       // modes area
                                Rect(153, 640, 30, 60),       // moves area
                                Rect(282, 650, 710, 46),      // inventory area
                                Rect(270, 656, 666, 40),      // text area
                                0,                            // statusbar coffsety
                                true, true,                   // available window, fullscreen
                                "-4-2-0-", "-4-2-0-"          // fallback modes window, fullscreen
                               },
                               {
                                VM_1280x720, 1280, 720,        // id, w, h
                                48, VTS_48,                    // tilesize, tiletype
                                "1280x720", "HD720", "16:10",  // name, fsname, fs only
                                "models-48.lua", "gfx48/",     // initscript, dir
                                Rect(160, 0, 960, 720),        // display area
                                -192, -96,                     // menu background image offsets
                                120, 78, 4, "-120x78",         // thumbnail size/extension
                                Rect(160, 0, 960, 624),        // game area
                                Rect(160, 624, 960, 96),       // statusbar area
                                Rect(178, 640, 170, 60),       // time area
                                Rect(313, 640, 30, 60),        // modes area
                                Rect(313, 640, 30, 60),        // moves area
                                Rect(400, 650, 710, 46),       // inventory area
                                Rect(388, 654, 710, 40),       // text area
                                0,                             // statusbar coffsety
                                false, true,                   // available window, fullscreen
                                "-0-", "-5-0-"                 // fallback modes window, fullscreen
                               },
                               {
                                VM_1280x960, 1280, 960,         // id, w, h
                                64, VTS_64,                     // tilesize, tiletype
                                "1280x960", "1280x960", "4:3",  // name, fsname, fs only
                                "models-64.lua", "gfx64/",      // initscript, dir
                                Rect(0, 0, 1280, 960),          // display area
                                -400, -90,                      // menu background image offsets
                                160, 104, 5, "-160x104",        // thumbnail size/extension
                                Rect(0, 0, 1280, 832),          // game area
                                Rect(0, 832, 1280, 128),        // statusbar area
                                Rect(24, 857, 227, 80),         // time area //TODO
                                Rect(204, 853, 40, 80),         // modes area //TODO
                                Rect(204, 853, 40, 80),         // moves area //TODO
                                Rect(376, 867, 947, 61),        // inventory area //TODO
                                Rect(360, 884, 888, 53),        // text area //TODO
                                1,                              // statusbar coffsety
                                true, true,                     // available window, fullscreen
                                "-6-4-2-0-", "-6-4-2-0-"        // fallback modes window, fullscreen
                               },
                               {
                                VM_1440x960, 1440, 960,         // id, w, h
                                64, VTS_64,                     // tilesize, tiletype
                                "1440x960", "1440x960", "3:2",  // name, fsname, fs only
                                "models-64.lua", "gfx64/",      // initscript, dir
                                Rect(80, 0, 1280, 960),         // display area
                                -400, -90,                      // menu background image offsets
                                160, 104, 5, "-160x104",        // thumbnail size/extension
                                Rect(80, 0, 1280, 832),         // game area
                                Rect(80, 832, 1280, 128),       // statusbar area
                                Rect(104, 853, 227, 80),        // time area //TODO
                                Rect(284, 853, 40, 80),         // modes area //TODO
                                Rect(284, 853, 40, 80),         // moves area //TODO
                                Rect(480, 867, 947, 61),        // inventory area //TODO
                                Rect(384, 873, 947, 53),        // text area //TODO
                                0,                              // statusbar coffsety
                                false, true,                    // available window, fullscreen
                                "-0-", "-7-0-"                  // fallback modes window, fullscreen
                               },
                               {
                                VM_1280x1024, 1280, 1024,    // id, w, h
                                64, VTS_64,                  // tilesize, tiletype
                                "1280x1024", "SXGA", "5:4",  // name, fsname, fs only
                                "models-64.lua", "gfx64/",   // initscript, dir
                                Rect(0, 0, 1280, 960),       // display area
                                -400, -26,                   // menu background image offsets
                                160, 104, 5, "-160x104",     // thumbnail size/extension
                                Rect(0, 0, 1280, 832),       // game area
                                Rect(0, 832, 1280, 128),     // statusbar area
                                Rect(18, 640, 170, 60),      // time area //TODO
                                Rect(153, 640, 30, 60),      // modes area //TODO
                                Rect(153, 640, 30, 60),      // moves area //TODO
                                Rect(324, 650, 710, 46),     // inventory area //TODO
                                Rect(360, 655, 888, 40),     // text area //TODO
                                0,                           // statusbar coffsety
                                false, true,                 // available window, fullscreen
                                "-0-", "-8-0-"               // fallback modes window, fullscreen
                               },
                               {
                                VM_1680x1050, 1680, 1050,        // id, w, h
                                64, VTS_64,                      // tilesize, tiletype
                                "1680x1050", "WSXGA+", "16:10",  // name, fsname, fs only
                                "models-64.lua", "gfx64/",       // initscript, dir
                                Rect(200, 0, 1280, 960),         // display area
                                -0, -0,                          // menu background image offsets
                                160, 104, 5, "-160x104",         // thumbnail size/extension
                                Rect(200, 0, 1280, 832),         // game area
                                Rect(200, 832, 1280, 128),       // statusbar area
                                Rect(224, 853, 227, 80),         // time area //TODO
                                Rect(404, 853, 40, 80),          // modes area //TODO
                                Rect(404, 853, 40, 80),          // moves area //TODO
                                Rect(600, 867, 947, 61),         // inventory area //TODO
                                Rect(504, 873, 947, 53),         // text area //TODO
                                0,                               // statusbar coffsety
                                false, true,                     // available window, fullscreen
                                "-0-", "-9-0-"  // fallback modes window, fullscreen
                               },
                               {
                                VM_320x240, 320, 240,       // id, w, h
                                16, VTS_16,                 // tilesize, tiletype
                                "320x240", "CGA", "4:3",    // name, fsname, fs only
                                "models-16.lua", "gfx16/",  // initscript, dir
                                Rect(0, 0, 320, 240),       // display area
                                0, 0,                       // menu background image offsets
                                60, 39, 2, "-60x39",        // thumbnail size/extension
                                Rect(0, 0, 320, 208),       // game area
                                Rect(0, 208, 320, 32),      // statusbar area
                                Rect(3, 212, 65, 21),       // time area
                                Rect(52, 210, 14, 21),      // modes area
                                Rect(50, 220, 15, 21),      // moves area
                                Rect(94, 216, 245, 26),     // inventory area
                                Rect(90, 217, 222, 17),     // text area
                                0,                          // statusbar coffsety
                                true, true,                 // available window, fullscreen
                                "-10-0-", "-10-0-"          // fallback modes window, fullscreen
                               }};

VideoModes current_video_mode = VM_None;

}  // namespace

/* -------------------- Auxiliary functions -------------------- */

namespace {

bool vm_available(int w, int h, bool fullscreen) {
    Uint32 flags = 0;
    if (fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;
    return true;

    // TODO(SDL2): use something based on SDL_GetClosestDisplayMode instead.
    // int newbpp = SDL_VideoModeOK(w, h, bpp, flags);
    // if (newbpp != 0) {
    //     bpp = newbpp;
    //     return true;
    // }
    // return false;
}

// This function is installed as an event filter by video::Init. It intercepts
// mouse motions, which are used to update the position of the mouse cursor
// (but passed on to the event queue).
int event_filter(void *, SDL_Event *e) {
    if (e->type == SDL_MOUSEMOTION) {
        cursor->move(e->motion.x, e->motion.y);
        cursor->redraw();
    }
    return 1;
}

}  // namespace

/* -------------------- Functions -------------------- */

void video::SetMouseCursor(ecl::Surface *s, int hotx, int hoty) {
    cursor->set_image(s, hotx, hoty);
    cursor->redraw();
}

void video::HideMouse() {
    cursor->hide();
    cursor->redraw();
}

void video::ShowMouse() {
    cursor->show();
    cursor->redraw();
}

int video::Mousex() {
    return cursor->get_x();
}

int video::Mousey() {
    return cursor->get_y();
}

/* -------------------- Input grabbing -------------------- */

TempInputGrab::TempInputGrab(bool onoff) {
    old_onoff = SetInputGrab(onoff);
}

TempInputGrab::~TempInputGrab() {
    SetInputGrab(old_onoff);
}

bool video::GetInputGrab() {
    return SDL_GetWindowGrab(ecl::Screen::get_instance()->window());
}

bool video::SetInputGrab(bool enabled) {
    bool old_state = GetInputGrab();
    SDL_SetWindowGrab(GetScreen()->window(), enabled ? SDL_TRUE : SDL_FALSE);
    return old_state;
}

Surface *video::BackBuffer() {
    if (back_buffer == 0) {
        back_buffer = Duplicate(SCREEN->get_surface());
    }
    return back_buffer;
}

const video::VMInfo *video::GetInfo(VideoModes vm) {
    return &video_modes[vm];
}

const video::VMInfo *video::GetInfo() {
    return GetInfo(current_video_mode);
}

void video::SetThumbInfo(int width, int height, std::string extension) {
    video_modes[current_video_mode].thumbw = width;
    video_modes[current_video_mode].thumbh = height;
    video_modes[current_video_mode].thumbsext = extension;
}

int video::GetNumAvailableModes(bool isFullscreen) {
    int avail = 0;
    for (int i = VM_None + 1; i < VM_COUNT; i++)
        if (isFullscreen ? video_modes[i].f_available : video_modes[i].w_available)
            avail++;

    return avail;
}

VideoModes video::GetVideoMode(int number, bool isFullscreen) {
    int avail = 0;
    for (int i = VM_None + 1; i < VM_COUNT; i++)
        if (isFullscreen ? video_modes[i].f_available : video_modes[i].w_available) {
            if (avail == number)
                return static_cast<video::VideoModes>(i);
            else
                avail++;
        }
    return VM_None;
}

VideoModes video::GetBestUserMode(bool isFullscreen, int seq) {
    std::string modes =
        app.prefs->getString(isFullscreen ? "VideoModesFullscreen" : "VideoModesWindow");
    if (modes.length() > 1) {
        std::istringstream ms(modes);
        int m = VM_None;
        ms.ignore();  // leading '-'
        while (ms && seq > 0) {
            ms >> std::dec >> m;
            ms.ignore();
            if (m <= VM_COUNT && ((!isFullscreen && video_modes[m].w_available) ||
                                  (isFullscreen && video_modes[m].f_available))) {
                if (seq == 1) {
                    return static_cast<video::VideoModes>(m);
                }
                --seq;
            }
        }
    }
    return VM_None;
}

int video::GetModeNumber(VideoModes mode, bool isFullscreen) {
    int avail = 0;
    for (int i = VM_None + 1; i < mode; i++)
        if (isFullscreen ? video_modes[i].f_available : video_modes[i].w_available)
            avail++;
    return avail;
}

bool video::ModeAvailable(VideoModes vm) {
    const VMInfo *vminfo = GetInfo(vm);
    string fname;
    return (vminfo->w_available && app.systemFS->findFile(vminfo->initscript, fname));
}

void video::Init() {
    static bool isInit = false;

    if (!isInit) {
        assert(NUMENTRIES(video_modes) == VM_COUNT);
        isInit = true;
        for (int i = VM_None + 1; i < VM_COUNT; i++) {
            if (video_modes[i].w_available &&
                !vm_available(video_modes[i].width, video_modes[i].height, false)) {
                video_modes[i].w_available = false;
                Log << "Video mode " << video_modes[i].width << " x " << video_modes[i].height
                    << " window not available\n";
            }
            if (video_modes[i].f_available &&
                !vm_available(video_modes[i].width, video_modes[i].height, true)) {
                video_modes[i].f_available = false;
                Log << "Video mode " << video_modes[i].width << " x " << video_modes[i].height
                    << " fullscreen not available\n";
            }
        }
    }

    bool isFullScreen = app.prefs->getBool("FullScreen");
    int vidmode = -1;
    if (app.prefs->getString("VideoModesFullscreen").empty()) {
        // initialize from 1.0 mode if never set before
        vidmode = app.prefs->getInt("VideoMode");
        app.prefs->setProperty("VideoModesFullscreen", video_modes[vidmode].fallback_fullscreen);
        app.prefs->setProperty("VideoModesWindow", video_modes[vidmode].fallback_window);
    }
    vidmode = video::GetBestUserMode(isFullScreen);

    video_engine = new Video_SDL();

    int fallback_sequence = 1;
    while (true) {
        VMInfo *vminfo = &video_modes[vidmode];
        int w = vminfo->width;
        int h = vminfo->height;

        if (ModeAvailable(static_cast<VideoModes>(vidmode)) &&
            vm_available(w, h, isFullScreen) && video_engine->init(w, h, isFullScreen)) {
            // Success!
            break;
        }

        // Video mode not available? Try the next fallback video mode
        vidmode = video::GetBestUserMode(isFullScreen, ++fallback_sequence);
        if (vidmode == VM_None) {
            // Give up :-(
            fprintf(stderr, "Couldn't open screen: %s\n", SDL_GetError());
            exit(1);
        }
    }

    current_video_mode = static_cast<VideoModes>(vidmode);
    app.selectedVideoMode = current_video_mode;

// Mac icon is set via Makefile
#ifndef MACOSX
    if (Surface *icon = enigma::GetImage("enigma_marble")) {
        SDL_SetWindowIcon(GetScreen()->window(), icon->get_surface());
    }
#endif

    cursor = new MouseCursor;
    int x, y;
    SDL_GetMouseState(&x, &y);
    cursor->move(x, y);

    SDL_SetEventFilter(event_filter, NULL);
}

void video::Shutdown() {
    SDL_SetEventFilter(0, NULL);
    delete video_engine;
    delete cursor;
    delete back_buffer;
    video_engine = 0;
    cursor = 0;
    back_buffer = 0;
}

void video::ChangeVideoMode() {
    MouseCursor *oldcursor = cursor;
    cursor = 0;
    Shutdown();
    Init();
    delete cursor;
    cursor = oldcursor;
}

ecl::Screen *video::GetScreen() {
    return SCREEN;
}

VideoModes video::GetVideoMode() {
    return current_video_mode;
}

bool video::IsFullScreen() {
    return video_engine->is_fullscreen();
}

bool video::SetFullscreen(bool on) {
    if ((on && video_modes[current_video_mode].f_available) ||
        (!on && video_modes[current_video_mode].w_available)) {
        video_engine->set_fullscreen(on);
        bool is_fullscreen = video_engine->is_fullscreen();
        if (on == is_fullscreen) {
            app.prefs->setProperty("FullScreen", is_fullscreen);
        }
    }
    return video::IsFullScreen();
}

bool video::ToggleFullscreen() {
    return SetFullscreen(!video_engine->is_fullscreen());
}

void video::SetCaption(const char *str) {
    video_engine->set_caption(str);
}

const string &video::GetCaption() {
    return video_engine->get_caption();
}

void video::Screenshot(const std::string &fname) {
    // auto-create the directory if necessary
    string directory;
    if (ecl::split_path(fname, &directory, 0) && !ecl::FolderExists(directory)) {
        ecl::FolderCreate(directory);
    }

    ecl::SavePNG(ecl::Grab(SCREEN->get_surface(), video_modes[current_video_mode].area), fname);
    enigma::Log << "Wrote screenshot to '" << fname << "'\n";
}
