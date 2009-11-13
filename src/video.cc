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

/*
 * This file contains the code for initializing the video hardware and
 * for various visual effects in the widest sense.  This includes
 *
 * a) managing and rendering the mouse cursor,
 * b) changing between windowed and fullscreen mode,
 * c) fading between different screen using one of a number of
 *    predefined effects.
 *
 * The code in this file is independent of the game.  For game-specific
 * display code, refer to file display.cc
 */

#include "enigma.hh"
#include "video.hh"
#include "lua.hh"
#include "options.hh"
#include "main.hh"
#include "ecl.hh"
#include "SDL.h"
#include <cassert>
#include <cstdio>
#include <fstream>
#include <sstream>
#include "config.h"

#define SCREEN ecl::Screen::get_instance()

using namespace std;
using namespace ecl;
using namespace video;
using namespace enigma;

namespace
{
    class Video_SDL {
        SDL_Surface*    sdlScreen;
        string          caption;
        ecl::Screen*     screen;
        bool            initialized;
    public:
        Video_SDL();
        ~Video_SDL();

        bool init(int w, int h, int bpp, bool fullscreen);
        void toggle_fullscreen();
        void set_fullscreen(bool on_off);
        bool is_fullscreen() const;
        void set_caption(const char *str);
        const string& get_caption() const { return caption; }
        ecl::Screen *get_screen() { return screen; }
    };

    class MouseCursor {
    public:
        MouseCursor ();
        ~MouseCursor();

        void set_image (ecl::Surface *s, int hotx_, int hoty_);
        void move (int newx, int newy);
        void redraw ();         // Redraw if position/image changed
        void draw();            // Draw cursor if visible
        void show ();
        void hide ();
        Rect get_rect() const;
        Rect get_oldrect() const;

	bool has_changed() { return changedp; }
        int get_x() const { return x; }
        int get_y() const { return y; }

    private:
        // Private methods
        void grab_bg ();
        void init_bg();
        void restore_bg();

        // Variables
        Surface *background;    // Copy of screen contents behind cursor
        Surface *cursor;        // Pixmap of the cursor

        int      x, y;
        int      oldx, oldy;
        int      hotx, hoty;    // Coordinates of hotspot inside cursor image
        int      visible;
        bool     changedp;
    };
}

/* -------------------- Video Engine -------------------- */

Video_SDL::Video_SDL()
: sdlScreen(0), 
  screen(0),
  initialized(false)
{}

Video_SDL::~Video_SDL() 
{
    SDL_WM_GrabInput(SDL_GRAB_OFF);
//     if (sdlScreen != 0 && fullScreen)
//         SDL_WM_ToggleFullScreen(sdlScreen);
    delete screen;
}

void Video_SDL::set_caption(const char *str) {
    caption = str;
    if (initialized)
        SDL_WM_SetCaption(str, 0);
}


bool Video_SDL::init(int w, int h, int bpp, bool fullscreen)
{
    static bool firstInit = true;
    
#ifndef MACOSX
    if (firstInit) {
        // Set the caption icon -- due to SDL doc it has to be set before first SDL_SetVideoMode() !!
        // In praxis this SetIcon does not work for Linux, nor is it necessary for OSX.
        // Just XP with selected "WindowsXP Design" needs this SetIcon.
        // See video::Init() for icon set for Linux and other Windows versions
        // Mac icon is set via Makefile 
        firstInit = false;
        std::string iconpath;
        ecl::Surface *es = NULL;
        if (app.resourceFS->findFile("gfx/enigma_marble.png", iconpath)) { // cannot use ecl::findImageFile !
            es = ecl::LoadImage(iconpath.c_str());
            if (es) {
                SDL_WM_SetIcon(es->get_surface(), NULL);
            }
        }
    }
#endif

    SDL_WM_SetCaption(caption.c_str(), 0);

    Uint32 flags = SDL_SWSURFACE;
    if (fullscreen)
        flags |= SDL_FULLSCREEN;

    // Try to initialize vide mode, return error code on failure
    sdlScreen = 0;
    bpp = SDL_VideoModeOK (w, h, bpp, flags);
    if (bpp == 0)
        return false;
    sdlScreen = SDL_SetVideoMode(w, h, bpp, flags);
    if (sdlScreen == 0) 
        return false;

    // Video mode could be set
    screen = new Screen(sdlScreen);
    initialized = true;

#if 0
    // the Mac SDL port seems to ignore the following ShowCursor,
    // so we just set the Cursor to be invisible.
    SDL_Cursor *hiddenCursor=SDL_CreateCursor(NULL, NULL, 0, 0, 0, 0);
    SDL_SetCursor(hiddenCursor);
    SDL_FreeCursor(hiddenCursor);
#endif
    
    // Hack to hide the cursor after switching between
    // window/fullscreen mode.
    SDL_ShowCursor (SDL_ENABLE);
    SDL_ShowCursor (SDL_DISABLE);

    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY / 2,
                        SDL_DEFAULT_REPEAT_INTERVAL / 2);

    return true;
}

bool Video_SDL::is_fullscreen() const 
{ 
    if (sdlScreen)
        return (sdlScreen->flags & SDL_FULLSCREEN) != 0; 
    return false;
}


void Video_SDL::set_fullscreen(bool on_off) {
    if (on_off != is_fullscreen())
        toggle_fullscreen();
}

void Video_SDL::toggle_fullscreen() 
{
    SDL_WM_ToggleFullScreen (sdlScreen);
}


/* -------------------- MouseCursor -------------------- */

MouseCursor::MouseCursor ()
: background(0), cursor(0)
{
    oldx = oldy = 0;
    hotx = hoty = 0;
    visible = 0;
    changedp = true;
}

MouseCursor::~MouseCursor() {
    delete background;
    delete cursor;
}

void MouseCursor::set_image (ecl::Surface *s, int hx, int hy) {
    delete cursor;
    cursor = s;
    hotx   = hx;
    hoty   = hy;

    if (visible > 0) {
        init_bg();
    }
}

void MouseCursor::draw () {
    if (visible > 0) {
        grab_bg();

        GC gc(SCREEN->get_surface());
        blit (gc, x-hotx, y-hoty, cursor);
        SCREEN->update_rect (get_rect());

        changedp = false;
    }
}

void MouseCursor::redraw () {
    if (visible > 0 && changedp) {
        restore_bg ();
        draw();
    }
}

void MouseCursor::move(int newx, int newy) {
    x        = newx;
    y        = newy;
    changedp = true;
}

void MouseCursor::show () {
    if (++visible == 1) {
        init_bg();
        changedp=true;
    }
}

void MouseCursor::hide () {
    if (--visible == 0) {
        changedp = true;
        restore_bg();
        delete background;
        background=0;
    }
}

Rect MouseCursor::get_rect() const {
    int scrx=x-hotx;
    int scry=y-hoty;
    return Rect(scrx,scry,cursor->width(), cursor->height());
}

Rect MouseCursor::get_oldrect() const {
    int scrx=oldx-hotx;
    int scry=oldy-hoty;
    return Rect(scrx,scry,cursor->width(), cursor->height());
}

void MouseCursor::init_bg() {
    assert (visible > 0);
    assert (cursor != 0);

    if (background != 0) {
        delete background;
    }

    background = ecl::MakeSurfaceLike (cursor->width(),
                                      cursor->height(),
                                      SCREEN->get_surface());
    grab_bg();
}

void MouseCursor::grab_bg () {
    assert (background != 0);

    GC gc(background);
    blit (gc, 0,0, SCREEN->get_surface(), get_rect());

    oldx=x;
    oldy=y;
}

void MouseCursor::restore_bg () {
    if (background) {
        GC gc(SCREEN->get_surface());
        blit (gc, oldx-hotx, oldy-hoty, background);
        SCREEN->update_rect (get_oldrect());
    }
}

/* -------------------- Local Variables -------------------- */
namespace
{
    Video_SDL   *video_engine = 0;
    MouseCursor *cursor       = 0;
    Surface     *back_buffer  = 0;

    /*! List of available video modes. */
    video::VMInfo video_modes[] = {
        { 
            VM_640x480, 640, 480,            // id, w, h
            32, VTS_32,                      // tilesize, tiletype
            "640x480", "VGA", "4:3",         // name, fsname, fs only
            "models-32.lua", "gfx32/",       // initscript, dir
            Rect (0, 0, 640, 480),           // display area
            0, 0,                            // menu background image offsets 
            120, 78, "",                     // thumbnail size/extension
            Rect (0, 0, 640, 416),           // game area
            Rect (0, 416, 640, 64),          // statusbar area
            Rect (10, 425, 117, 43),         // time area
            Rect (100, 419, 28, 43),         // modes area
            Rect (100, 425, 30, 43),         // moves area
            Rect (152, 433, 490, 52),        // inventory area
            Rect (150, 434, 475, 35),        // text area
            true, true,                      // available window, fullscreen
            "-0-","-0-"                      // fallback modes window, fullscreen
        },
        { 
            VM_640x512, 640, 512,            // id, w, h
            32, VTS_32,                      // tilesize, tiletype
            "640x512", "640x512", "5:4",     // name, fsname, fs only
            "models-32.lua", "gfx32/",       // initscript, dir
            Rect (0, 0, 640, 480),           // display area
            0, 0,                            // menu background image offsets 
            120, 78, "",                     // thumbnail size/extension
            Rect (0, 0, 640, 416),           // game area
            Rect (0, 416, 640, 64),          // statusbar area
            Rect (15, 420, 110, 40),         // time area
            Rect (100, 420, 31, 40),         // modes area
            Rect (100, 420, 30, 40),         // moves area
            Rect (152, 433, 490, 52),        // inventory area
            Rect (150, 434, 475, 35),        // text area
            false, false,                    // 640x512 is deprecated!
            "-0-","-0-"                      // fallback modes window, fullscreen
        },
        { 
            VM_800x600, 800, 600,            // id, w, h
            40, VTS_40,                      // tilesize, tiletype
            "800x600", "SVGA", "4:3",        // name, fsname, fs only
            "models-40.lua", "gfx40/",       // initscript, dir
            Rect (0, 0, 800, 600),           // display area
            0, 0,                            // menu background image offsets 
            120, 78, "",                     // thumbnail size/extension
            Rect (0, 0, 800, 520),           // game area
            Rect (0, 520, 800, 80),          // statusbar area
            Rect (15, 540, 140, 40),         // time area
            Rect (125, 529, 31, 40),         // modes area
            Rect (125, 540, 30, 40),         // moves area
            Rect (192, 539, 610, 46),        // inventory area
            Rect (185, 545, 600, 39),        // text area
            true, true,                      // available window, fullscreen
            "-2-0-","-2-0-"                  // fallback modes window, fullscreen
        },
        { 
            VM_1024x768, 1024, 768,          // id, w, h
            48, VTS_48,                      // tilesize, tiletype
            "1024x768", "XGA", "4:3",        // name, fsname, fs only
            "models-48.lua", "gfx48/",       // initscript, dir
            Rect (32, 0, 960, 720),          // display area
            -128, -96,                       // menu background image offsets 
            120, 78, "",                     // thumbnail size/extension
            Rect (32, 0, 960, 624),          // game area
            Rect (32, 624, 960, 96),         // statusbar area
            Rect (50, 640, 170, 60),         // time area
            Rect (185, 634, 34, 60),         // modes area
            Rect (185, 640, 30, 60),         // moves area
            Rect (260, 650, 710, 46),        // inventory area
            Rect (260, 655, 710, 40),        // text area
            true, true,                      // available window, fullscreen
            "-3-2-0-","-3-2-0-"              // fallback modes window, fullscreen
        },
        { 
            VM_960x720, 960, 720,            // id, w, h
            48, VTS_48,                      // tilesize, tiletype
            "960x720", "960x720", "4:3",     // name, fsname, fs only
            "models-48.lua", "gfx48/",       // initscript, dir
            Rect (0, 0, 960, 720),           // display area
            -192, -144,                      // menu background image offsets 
            120, 78, "",                     // thumbnail size/extension
            Rect (0, 0, 960, 624),           // game area
            Rect (0, 624, 960, 96),          // statusbar area
            Rect (18, 640, 170, 60),         // time area
            Rect (153, 634, 34, 60),         // modes area
            Rect (153, 640, 30, 60),         // moves area
            Rect (228, 650, 710, 46),        // inventory area
            Rect (228, 655, 710, 40),        // text area
            true, true,                      // available window, fullscreen
            "-4-2-0-","-4-2-0-"              // fallback modes window, fullscreen
        },
        { 
            VM_1280x720, 1280, 720,          // id, w, h
            48, VTS_48,                      // tilesize, tiletype
            "1280x720", "HD720", "16:10",    // name, fsname, fs only
            "models-48.lua", "gfx48/",       // initscript, dir
            Rect (160, 0, 960, 720),         // display area
            -192, -96,                       // menu background image offsets 
            120, 78, "",                     // thumbnail size/extension
            Rect (160, 0, 960, 624),         // game area
            Rect (160, 624, 960, 96),        // statusbar area
            Rect (178, 640, 170, 60),        // time area
            Rect (313, 640, 30, 60),         // modes area
            Rect (313, 640, 30, 60),         // moves area
            Rect (388, 650, 710, 46),        // inventory area
            Rect (388, 655, 710, 40),        // text area
            false, true,                     // available window, fullscreen
            "-0-","-5-0-"                    // fallback modes window, fullscreen
        },
        { 
            VM_1280x960, 1280, 960,          // id, w, h
            64, VTS_64,                      // tilesize, tiletype
            "1280x960", "1280x960", "4:3",   // name, fsname, fs only
            "models-64.lua", "gfx64/",       // initscript, dir
            Rect (0, 0, 1280, 960),          // display area
            -400, -90,                       // menu background image offsets 
            160, 104, "-160x104",            // thumbnail size/extension
            Rect (0, 0, 1280, 832),          // game area
            Rect (0, 832, 1280, 128),        // statusbar area
            Rect (24, 853, 227, 80),         // time area //TODO
            Rect (204, 853, 40, 80),         // modes area //TODO
            Rect (204, 853, 40, 80),         // moves area //TODO
            Rect (304, 867, 947, 61),        // inventory area //TODO
            Rect (304, 873, 947, 53),        // text area //TODO
            true, true,                      // available window, fullscreen
            "-6-4-2-0-","-6-4-2-0-"          // fallback modes window, fullscreen
        },
        { 
            VM_1440x960, 1440, 960,          // id, w, h
            64, VTS_64,                      // tilesize, tiletype
            "1440x960", "1440x960", "3:2",   // name, fsname, fs only
            "models-64.lua", "gfx64/",       // initscript, dir
            Rect (80, 0, 1280, 960),         // display area
            -400, -90,                       // menu background image offsets 
            160, 104, "-160x104",            // thumbnail size/extension
            Rect (80, 0, 1280, 832),         // game area
            Rect (80, 832, 1280, 128),       // statusbar area
            Rect (104, 853, 227, 80),        // time area //TODO
            Rect (284, 853, 40, 80),         // modes area //TODO
            Rect (284, 853, 40, 80),         // moves area //TODO
            Rect (384, 867, 947, 61),        // inventory area //TODO
            Rect (384, 873, 947, 53),        // text area //TODO
            false, true,                     // available window, fullscreen
            "-0-","-7-0-"                    // fallback modes window, fullscreen
        },
        { 
            VM_1280x1024, 1280, 1024,        // id, w, h
            64, VTS_64,                      // tilesize, tiletype
            "1280x1024", "SXGA", "5:4",      // name, fsname, fs only
            "models-64.lua", "gfx64/",       // initscript, dir
            Rect (0, 0, 1280, 960),          // display area
            -400, -26,                       // menu background image offsets 
            160, 104, "-160x104",            // thumbnail size/extension
            Rect (0, 0, 1280, 832),          // game area
            Rect (0, 832, 1280, 128),        // statusbar area
            Rect (18, 640, 170, 60),         // time area //TODO
            Rect (153, 640, 30, 60),         // modes area //TODO
            Rect (153, 640, 30, 60),         // moves area //TODO
            Rect (228, 650, 710, 46),        // inventory area //TODO
            Rect (228, 655, 710, 40),        // text area //TODO
            false, true,                     // available window, fullscreen
            "-0-","-8-0-"                    // fallback modes window, fullscreen
        },
        { 
            VM_1680x1050, 1680, 1050,        // id, w, h
            64, VTS_64,                      // tilesize, tiletype
            "1680x1050", "WSXGA+", "16:10",  // name, fsname, fs only
            "models-64.lua", "gfx64/",       // initscript, dir
            Rect (200, 0, 1280, 960),        // display area
            -0, -0,                          // menu background image offsets 
            160, 104, "-160x104",            // thumbnail size/extension
            Rect (200, 0, 1280, 832),        // game area
            Rect (200, 832, 1280, 128),      // statusbar area
            Rect (224, 853, 227, 80),        // time area //TODO
            Rect (404, 853, 40, 80),         // modes area //TODO
            Rect (404, 853, 40, 80),         // moves area //TODO
            Rect (504, 867, 947, 61),        // inventory area //TODO
            Rect (504, 873, 947, 53),        // text area //TODO
            false, true,                     // available window, fullscreen
            "-0-","-9-0-"                    // fallback modes window, fullscreen
        },
        { 
            VM_320x240, 320, 240,            // id, w, h
            16, VTS_16,                      // tilesize, tiletype
            "320x240", "CGA", "4:3",         // name, fsname, fs only
            "models-16.lua", "gfx16/",       // initscript, dir
            Rect (0, 0, 320, 240),           // display area
            0, 0,                            // menu background image offsets 
            60, 39, "-60x39",                // thumbnail size/extension
            Rect (0, 0, 320, 208),           // game area
            Rect (0, 208, 320, 32),          // statusbar area
            Rect (5, 212, 65, 21),           // time area
            Rect (50, 210, 14, 21),          // modes area
            Rect (50, 220, 15, 21),          // moves area
            Rect (76, 216, 245, 26),         // inventory area
            Rect (75, 217, 237, 17),         // text area
            true, true,                      // available window, fullscreen
            "-10-0-","-10-0-"                // fallback modes window, fullscreen
        }
    };

    VideoModes current_video_mode = VM_None;
}

/* -------------------- Auxiliary functions -------------------- */

namespace
{
    bool vm_available (int w, int h, int &bpp, bool fullscreen)
    {
        Uint32 flags = SDL_HWSURFACE;
        if (fullscreen)
            flags |= SDL_FULLSCREEN;

        int newbpp = SDL_VideoModeOK (w, h, bpp, flags);
        if (newbpp != 0) {
            bpp = newbpp;
            return true;
        }
        return false;
    }

/*! This function is installed as an event filter by video::Init.  It
  intercepts mouse motions, which are used to update the position of
  the mouse cursor (but passed on to the event queue) */
    int event_filter(const SDL_Event *e)
    {
        if (e->type == SDL_MOUSEMOTION) {
            cursor->move(e->motion.x, e->motion.y);
            cursor->redraw();
        }
        return 1;
    }

}


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


TempInputGrab::TempInputGrab (bool onoff)
{
    old_onoff = SetInputGrab (onoff);
}

TempInputGrab::~TempInputGrab()
{
    SetInputGrab (old_onoff);
}

bool video::GetInputGrab()
{
    return SDL_WM_GrabInput (SDL_GRAB_QUERY) == SDL_GRAB_ON;
}

bool video::SetInputGrab (bool onoff)
{
    bool old_onoff = GetInputGrab (); 
    if (onoff) {
        Screen *screen = GetScreen();
        SDL_WarpMouse (screen->width()/2, screen->height()/2);
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ;                   // swallow mouse motion event
        }
        SDL_WM_GrabInput (SDL_GRAB_ON);
    }
    else
        SDL_WM_GrabInput (SDL_GRAB_OFF);
    return old_onoff;
}


Surface* video::BackBuffer() {
    if (back_buffer==0) {
        back_buffer= Duplicate(SCREEN->get_surface());
    }
    return back_buffer;
}

const video::VMInfo *video::GetInfo (VideoModes vm)
{
    return &video_modes[vm];
}

const video::VMInfo *video::GetInfo ()
{
    return GetInfo (current_video_mode);
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
    std::string modes = app.prefs->getString(isFullscreen ? "VideoModesFullscreen" : "VideoModesWindow");
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


bool video::ModeAvailable (VideoModes vm)
{
    const VMInfo *vminfo = GetInfo (vm);
    string fname;
    return (vminfo->w_available && app.systemFS->findFile (vminfo->initscript, fname));
}


void video::Init() 
{
    static bool isInit = false;
    static int bpp_default = 16;
    int bpp = bpp_default; //options::BitsPerPixel;

    
    if (!isInit) {
        assert (NUMENTRIES(video_modes) == VM_COUNT);
        isInit = true;
        for (int i = VM_None + 1; i < VM_COUNT; i++) {
            bpp = bpp_default;
            if (video_modes[i].w_available && !vm_available(video_modes[i].width, video_modes[i].height, bpp, false)) {
                video_modes[i].w_available = false;
                Log << "Video mode " << video_modes[i].width << " x " << video_modes[i].height << " window not available\n";
            }
            if (bpp != bpp_default)
                Log << "Video mode " << video_modes[i].width << " x " << video_modes[i].height << " window available with bpp " << bpp << "\n";
            bpp = bpp_default;
            if (video_modes[i].f_available && !vm_available(video_modes[i].width, video_modes[i].height, bpp, true)) {
                video_modes[i].f_available = false;
                Log << "Video mode " << video_modes[i].width << " x " << video_modes[i].height << " fullscreen not available\n";
            }
            if (bpp != bpp_default)
                Log << "Video mode " << video_modes[i].width << " x " << video_modes[i].height << " fullscreen available with bpp " << bpp << "\n";
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
    
    int oldvidmode = vidmode;

    video_engine = new Video_SDL();


    assert(bpp==16 || bpp==32);
    int fallback_sequence = 1;
    while (true) {
        VMInfo *vminfo     = &video_modes[vidmode];
        int     w          = vminfo->width;
        int     h          = vminfo->height;

        if (ModeAvailable (static_cast<VideoModes> (vidmode))
            && vm_available (w, h, bpp, isFullScreen)
            && video_engine->init (w, h, bpp, isFullScreen)) 
        {
            // Success!
            break;
        }

        // Video mode not available? Try the next fallback video mode
        vidmode =  video::GetBestUserMode(isFullScreen, ++fallback_sequence); 
        if (vidmode == VM_None) {
            // Give up :-(
            fprintf(stderr, "Couldn't open screen: %s\n", SDL_GetError());
            exit(1);
        }
    }

    current_video_mode = static_cast<VideoModes>(vidmode);
    app.selectedVideoMode = current_video_mode;

    // Set the caption icon -- this position after SDL_SetVideoMode() contradicts SDL doc!!
    // But it proves to be the only working position for Linux.
    // It works for Windows besides XP with selected "WindowsXP Design", too.
    // Mac icon is set via Makefile 
#ifndef MACOSX
    Surface *icn = enigma::GetImage("enigma_marble");
    if(icn)
        SDL_WM_SetIcon(icn->get_surface(), NULL);
#endif

    cursor = new MouseCursor;
    int x, y;
    SDL_GetMouseState(&x, &y);
    cursor->move(x,y);

    SDL_SetEventFilter(event_filter);


    UpdateGamma();
}

void video::Shutdown() 
{
    SDL_SetEventFilter(0);
    delete video_engine;
    delete cursor;
    delete back_buffer;
    video_engine = 0;
    cursor = 0;
    back_buffer = 0;
}

void video::ChangeVideoMode() 
{
    MouseCursor *oldcursor = cursor;
    cursor = 0;
    Shutdown();
    Init();
    delete cursor;
    cursor = oldcursor;
}

ecl::Screen * video::GetScreen() {
    return SCREEN;
}

VideoModes video::GetVideoMode() {
    return current_video_mode;
}

bool video::IsFullScreen()
{
    return video_engine->is_fullscreen();
}

int video::GetColorDepth() {
    return SCREEN->get_surface()->bipp();
}

bool video::SetFullscreen(bool on) 
{
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

bool video::ToggleFullscreen() 
{
    return SetFullscreen (!video_engine->is_fullscreen());
}

void video::SetCaption(const char *str) 
{
    video_engine->set_caption(str);
}

const string& video::GetCaption() 
{
    return video_engine->get_caption();
}

void video::UpdateGamma()
{
    float gamma = static_cast<float> (app.prefs->getDouble ("Gamma"));
    if (gamma < 0.25)  
        gamma = 0.25;  // Windows does not set gamma for values < 0.2271
    int result = SDL_SetGamma (gamma, gamma, gamma);
}

void video::Screenshot (const std::string &fname) 
{
    // auto-create the directory if necessary
    string directory;
    if (ecl::split_path (fname, &directory, 0) && !ecl::FolderExists(directory)) {
        ecl::FolderCreate (directory);
    }

    ecl::SavePNG (ecl::Grab(SCREEN->get_surface(), video_modes[current_video_mode].area), fname);
    enigma::Log << "Wrote screenshot to '" << fname << "\n";
}

/* -------------------- Special Effects -------------------- */

void video::FX_Fade(FadeMode mode) 
{
    ecl::Screen *screen = ecl::Screen::get_instance();
    Surface *d = screen->get_surface();
    const double fadesec = 0.6;
    double v = 255/fadesec;

    ecl::Surface *buffer = Duplicate(d);
    double dt;

    double a = mode==FADEIN ? 0 : 255;

    GC gc (d);
    
    while (true) {
        Uint32 otime = SDL_GetTicks();

        box(gc, d->size());
        buffer->set_alpha(int(a));
        blit(gc, 0,0,buffer);
        screen->update_all();
        screen->flush_updates();

        dt = (SDL_GetTicks()-otime)/1000.0;
        if (mode==FADEIN && (a+=v*dt) > 255)
            break;
        else if (mode==FADEOUT && (a-=v*dt) < 0)
            break;
    }

    if (mode==FADEIN) {
        buffer->set_alpha(255);
        blit(gc, 0,0,buffer);
    } else
        box (gc, d->size());
    screen->update_all();
    screen->flush_updates();
    delete buffer;
}

void video::FX_Fly (Surface *newscr, int originx, int originy) 
{
    double rest_time = 0.5;

    double velx = -originx / rest_time;
    double vely = -originy / rest_time;

    double origx = originx;
    double origy = originy;

    Screen *scr = SCREEN;
    GC scrgc(scr->get_surface());

    while (rest_time > 0)
    {
        Uint32 otime = SDL_GetTicks();

        Rect r(static_cast<int>(origx),
               static_cast<int>(origy),
               scr->width(), scr->height());
        blit (scrgc, r.x, r.y, newscr);

        scr->update_rect(r);
        scr->flush_updates();

        double dt = (SDL_GetTicks()-otime)/1000.0;
        if (dt > rest_time)
            dt = rest_time;
        rest_time -= dt;
        origx += velx * dt;
        origy += vely * dt;
    }
}

namespace
{
    class Effect_Push : public TransitionEffect {
    public:
        Effect_Push(ecl::Surface *newscr, int originx, int originy);
        void tick (double dtime);
        bool finished() const;
    private:
        double rest_time;
        ecl::Surface *newscr;
        std::auto_ptr<ecl::Surface > oldscr;
        int originx, originy;
        double velx, vely;
        double accx, accy;
        double x, y;
        double t;
    };
}

Effect_Push::Effect_Push(ecl::Surface *newscr_, int originx_, int originy_)
: rest_time (0.7),
  newscr (newscr_),
  oldscr (Duplicate(SCREEN->get_surface())),
  originx (originx_),
  originy (originy_),
  velx (-2 * originx / rest_time),
  vely (-2 * originy / rest_time),
  accx (-0.5*velx/rest_time),
  accy (-0.5*vely/rest_time),
  x (originx),
  y (originy),
  t (0)
{
}

void Effect_Push::tick (double dtime)
{
    Screen *scr = SCREEN;
    GC scrgc(scr->get_surface());

    if (rest_time > 0) {
        if (dtime > rest_time)
            dtime = rest_time;
        rest_time -= dtime;
        t+=dtime;

        x = (accx*t + velx)*t + originx;
        y = (accy*t + vely)*t + originy;

        blit (scrgc, (int)x-originx, (int)y, oldscr.get());
        blit (scrgc, (int)x, (int)y-originy, oldscr.get());
        blit (scrgc, (int)x-originx, (int)y-originy, oldscr.get());

        blit (scrgc, (int)x, (int) y, newscr);

        scr->update_all();
        scr->flush_updates();
    }
    else {
        blit(scrgc, 0,0, newscr);
        scr->update_all();
        scr->flush_updates();
    }
}

bool Effect_Push::finished() const
{
    return rest_time <= 0;
}




TransitionEffect *
video::MakeEffect (TransitionModes tm, ecl::Surface *newscr)
{
    int scrw = SCREEN->width();
    int scrh = SCREEN->height();

    switch (tm) {
    case TM_PUSH_RANDOM: {
        int xo=0, yo=0;
        while (xo==0 && yo==0) {
            xo = enigma::IntegerRand(-1, 1, false)*scrw;
            yo = enigma::IntegerRand(-1, 1, false)*scrh;
        }
        return new Effect_Push(newscr, xo, yo);
    }
    case TM_PUSH_N: return new Effect_Push (newscr, 0, -scrh);
    case TM_PUSH_S: return new Effect_Push (newscr, 0, +scrh);
    case TM_PUSH_E: return new Effect_Push (newscr, +scrw, 0);
    case TM_PUSH_W: return new Effect_Push (newscr, -scrw, 0);
    default:
        return 0;
    };
}


void video::ShowScreen (TransitionModes tm, Surface *newscr) {
    int scrw = SCREEN->width();
    int scrh = SCREEN->height();

    switch (tm) {
    case TM_RANDOM:
	break;
    case TM_FADEOUTIN:
	break;
    case TM_SQUARES:
	break;
    case TM_FLY_N: FX_Fly (newscr, 0, -scrh); break;
    case TM_FLY_S: FX_Fly (newscr, 0, +scrh); break;
    case TM_FLY_E: FX_Fly (newscr, +scrw, 0); break;
    case TM_FLY_W: FX_Fly (newscr, -scrw, 0); break;

    default:
        break;
    }
}

