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
    x = newx;
    y = newy;
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
     VM_320x240, 320, 240,       // id, w, h
     16, VTS_16,                 // tilesize, tiletype
     "320x240", "CGA", "4:3",    // name, fsname, fs only
     "models-16.lua", "gfx16/",  // initscript, dir
     Rect(0, 0, 320, 240),       // display area
     0, 0,                       // menu background image offsets
     {60, 39, 2, "-60x39"},        // thumbnail size/extension
     Rect(0, 0, 320, 208),       // game area
     Rect(0, 208, 320, 32),      // statusbar area
     Rect(3, 212, 65, 21),       // time area
     Rect(52, 210, 14, 21),      // modes area
     Rect(50, 220, 15, 21),      // moves area
     Rect(94, 216, 245, 26),     // inventory area
     Rect(90, 217, 222, 17),     // text area
     0,                          // statusbar coffsety
    },
    {
     VM_640x480, 640, 480,       // id, w, h
     32, VTS_32,                 // tilesize, tiletype
     "640x480", "VGA", "4:3",    // name, fsname, fs only
     "models-32.lua", "gfx32/",  // initscript, dir
     Rect(0, 0, 640, 480),       // display area
     0, 0,                       // menu background image offsets
     {120, 78, 4, "-120x78"},      // thumbnail size/extension
     Rect(0, 0, 640, 416),       // game area
     Rect(0, 416, 640, 64),      // statusbar area
     Rect(10, 428, 117, 43),     // time area
     Rect(100, 422, 28, 43),     // modes area
     Rect(100, 425, 30, 43),     // moves area
     Rect(188, 433, 490, 52),    // inventory area
     Rect(180, 436, 444, 35),    // text area
     0,                          // statusbar coffsety
    },
    {
     VM_800x600, 800, 600,       // id, w, h
     40, VTS_40,                 // tilesize, tiletype
     "800x600", "SVGA", "4:3",   // name, fsname, fs only
     "models-40.lua", "gfx40/",  // initscript, dir
     Rect(0, 0, 800, 600),       // display area
     0, 0,                       // menu background image offsets
     {120, 78, 4, "-120x78"},      // thumbnail size/extension
     Rect(0, 0, 800, 520),       // game area
     Rect(0, 520, 800, 80),      // statusbar area
     Rect(16, 540, 140, 40),     // time area
     Rect(140, 530, 31, 40),     // modes area
     Rect(140, 540, 30, 40),     // moves area
     Rect(235, 539, 610, 46),    // inventory area
     Rect(225, 547, 555, 39),    // text area
     0,                          // statusbar coffsety
    },
    {
     VM_960x720, 960, 720,         // id, w, h
     48, VTS_48,                   // tilesize, tiletype
     "960x720", "960x720", "4:3",  // name, fsname, fs only
     "models-48.lua", "gfx48/",    // initscript, dir
     Rect(0, 0, 960, 720),         // display area
     0, 0,                         // menu background image offsets
     {120, 78, 4, "-120x78"},        // thumbnail size/extension
     Rect(0, 0, 960, 624),         // game area
     Rect(0, 624, 960, 96),        // statusbar area
     Rect(18, 640, 170, 60),       // time area
     Rect(166, 636, 34, 60),       // modes area
     Rect(153, 640, 30, 60),       // moves area
     Rect(282, 650, 710, 46),      // inventory area
     Rect(270, 656, 666, 40),      // text area
     0,                            // statusbar coffsety
    },
    {
     VM_1280x960, 1280, 960,         // id, w, h
     64, VTS_64,                     // tilesize, tiletype
     "1280x960", "1280x960", "4:3",  // name, fsname, fs only
     "models-64.lua", "gfx64/",      // initscript, dir
     Rect(0, 0, 1280, 960),          // display area
     0, 0,                           // menu background image offsets
     {160, 104, 5, "-160x104"},        // thumbnail size/extension
     Rect(0, 0, 1280, 832),          // game area
     Rect(0, 832, 1280, 128),        // statusbar area
     Rect(24, 857, 227, 80),         // time area //TODO
     Rect(204, 853, 40, 80),         // modes area //TODO
     Rect(204, 853, 40, 80),         // moves area //TODO
     Rect(376, 867, 947, 61),        // inventory area //TODO
     Rect(360, 884, 888, 53),        // text area //TODO
     1,                              // statusbar coffsety
    },
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

    std::vector<DisplayMode> EnumerateDisplayModes() override;
    DisplayMode ActiveDisplayMode() override;
    void SetDisplayMode(const DisplayMode &display_mode, bool fullscreen) override;
    void Resize(Sint32 width, Sint32 height) override;

    const VMInfo *GetInfo() override;
    const VMInfo *GetInfo(VideoMode mode) { return &video_modes[mode]; }

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
    VideoMode FindClosestVideoMode(const DisplayMode &display_mode);
    void CloseWindow();

    ecl::Screen *screen;
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::unique_ptr<Surface> back_buffer;
    std::unique_ptr<MouseCursor> cursor;
    std::string window_caption;
};

VideoEngineImpl::VideoEngineImpl() : screen(NULL), window(NULL), renderer(NULL) {
}

VideoEngineImpl::~VideoEngineImpl() {
    CloseWindow();
}

void VideoEngineImpl::Init() {
    bool isFullScreen = false;  // app.prefs->getBool("FullScreen");
    int w = 1280;
    int h = 960;
    OpenWindow(w, h, isFullScreen);

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

std::vector<DisplayMode> VideoEngineImpl::EnumerateDisplayModes() {
    std::vector<DisplayMode> modes;
    const int display = 0;
    const int num_modes = SDL_GetNumDisplayModes(display);
    for (int i = 0; i < num_modes; ++i) {
        SDL_DisplayMode mode;
        if (SDL_GetDisplayMode(display, i, &mode) == 0) {
            Log << "  display mode " << mode.w << "x" << mode.h << "\n";
            DisplayMode new_mode = {mode.w, mode.h};
            if (std::find(modes.begin(), modes.end(), new_mode) == modes.end())
                modes.push_back(new_mode);
        }
    }
    return modes;
}

DisplayMode VideoEngineImpl::ActiveDisplayMode() {
    return {screen->width(), screen->height()};
}

void VideoEngineImpl::SetDisplayMode(const DisplayMode &display_mode, bool fullscreen) {
    DisplayMode old_mode = ActiveDisplayMode();
    CloseWindow();
    if (!OpenWindow(display_mode.width, display_mode.height, fullscreen)) {
        bool success = OpenWindow(old_mode.width, old_mode.height, false);
        // TODO(sdl2): proper error handling when old mode cannot be re-enabled.
        assert(success);
    }
}

void VideoEngineImpl::Resize(Sint32 width, Sint32 height) {
    SDL_SetWindowSize(window, width, height);
    SDL_RenderSetLogicalSize(renderer, width, height);
}

const VMInfo *VideoEngineImpl::GetInfo() {
    auto mode = FindClosestVideoMode(ActiveDisplayMode());
    assert(mode != VM_None);
    return &video_modes[mode];
}

bool VideoEngineImpl::SetFullscreen(bool enabled) {
    // TODO(sdl2): change display mode if necessary
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
    Log << "Opening window " << width << "x" << height << "\n";
    window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
                              flags);
    if (!window)
        return false;
    screen = new Screen(window);

    renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (!renderer)
        return false;

    VideoMode video_mode = FindClosestVideoMode({width, height});
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

VideoMode VideoEngineImpl::FindClosestVideoMode(const DisplayMode &display_mode) {
    for (int i = VM_1280x960; i >= VM_320x240; --i) {
        const VMInfo &info = video_modes[i];

        if (info.width <= display_mode.width && info.height <= display_mode.height) {
            return info.mode;
        }
    }
    return VM_None;
}

void VideoEngineImpl::CloseWindow() {
    assert(window);
    assert(renderer);

    SetInputGrab(false);
    cursor = nullptr;
    back_buffer = nullptr;
    delete screen;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

// -------------------- Global variables & functions --------------------

VideoEngine *video_engine = nullptr;

void VideoInit() {
    video_engine = new VideoEngineImpl;
    video_engine->Init();
    Log << "VideoInit done\n";
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

}  // namespace enigma
