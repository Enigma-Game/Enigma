/*
 * Copyright (C) 2002,2004 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
 * Copyright (C) 2020-2022 Andreas Lochmann
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

#include "display.hh"
#include "ecl_system.hh"
#include "ecl_video.hh"
#include "gui/LevelPreviewCache.hh"
#include "main.hh"
#include "MouseCursor.hh"
#include "options.hh"
#include "resource_cache.hh"

#include <cassert>
#include <cstdio>
#include <memory>
#include <sstream>

using ecl::Rect;
using namespace enigma;

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
     "-3-4-2-0-"                   // fallback modes fullscreen
    },
    {
     VM_1152x864, 5, 1152, 864,    // id, preffilenr, w, h
     48, VTS_48,                   // tilesize, tiletype
     "1152x864", "XGA+", "4:3",    // name, fsname, fs only
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
     "-5-4-2-0-"                   // fallback modes fullscreen
    },
    {
     VM_1280x960, 6, 1280, 960,     // id, preffilenr, w, h
     64, VTS_64,                    // tilesize, tiletype
     "1280x960", "SXGA-", "4:3",    // name, fsname, fs only
     Rect(0, 0, 1280, 960),         // display area
     -400, -90,                     // menu background image offsets
     {160, 104, 5, "-160x104"},     // thumbnail size/extension
     Rect(0, 0, 1280, 832),         // game area
     Rect(0, 832, 1280, 128),       // statusbar area
     Rect(24, 857, 227, 80),        // time area
     Rect(204, 853, 40, 80),        // modes area
     Rect(204, 853, 40, 80),        // moves area
     Rect(376, 867, 947, 61),       // inventory area
     Rect(360, 884, 888, 53),       // text area
     1,                             // statusbar coffsety
     true,                          // available fullscreen
     "-6-4-2-0-"                    // fallback modes fullscreen
    },
    {
     VM_1400x1050, 7, 1400, 1050,   // id, preffilenr, w, h
     64, VTS_64,                    // tilesize, tiletype
     "1400x1050", "SXGA+", "4:3",   // name, fsname, fs only
     Rect(0, 0, 1280, 960),         // display area
     -400, -90,                     // menu background image offsets
     {160, 104, 5, "-160x104"},     // thumbnail size/extension
     Rect(0, 0, 1280, 832),         // game area
     Rect(0, 832, 1280, 128),       // statusbar area
     Rect(24, 857, 227, 80),        // time area
     Rect(204, 853, 40, 80),        // modes area
     Rect(204, 853, 40, 80),        // moves area
     Rect(376, 867, 947, 61),       // inventory area
     Rect(360, 884, 888, 53),       // text area
     1,                             // statusbar coffsety
     true,                          // available fullscreen
     "-7-6-4-2-0-"                  // fallback modes fullscreen
    },
    {
     VM_1440x1080, 8, 1440, 1080,   // id, preffilenr, w, h
     64, VTS_64,                    // tilesize, tiletype
     "1440x1080", "HDV", "4:3",     // name, fsname, fs only
     Rect(0, 0, 1280, 960),         // display area
     -400, -90,                     // menu background image offsets
     {160, 104, 5, "-160x104"},     // thumbnail size/extension
     Rect(0, 0, 1280, 832),         // game area
     Rect(0, 832, 1280, 128),       // statusbar area
     Rect(24, 857, 227, 80),        // time area
     Rect(204, 853, 40, 80),        // modes area
     Rect(204, 853, 40, 80),        // moves area
     Rect(376, 867, 947, 61),       // inventory area
     Rect(360, 884, 888, 53),       // text area
     1,                             // statusbar coffsety
     true,                          // available fullscreen
     "-8-6-4-2-0-"                  // fallback modes fullscreen
    },
    {
     VM_1600x1200, 9, 1600, 1200,   // id, preffilenr, w, h
     64, VTS_64,                    // tilesize, tiletype
     "1600x1200", "UXGA", "4:3",    // name, fsname, fs only
     Rect(0, 0, 1280, 960),         // display area
     -400, -90,                     // menu background image offsets
     {160, 104, 5, "-160x104"},     // thumbnail size/extension
     Rect(0, 0, 1280, 832),         // game area
     Rect(0, 832, 1280, 128),       // statusbar area
     Rect(24, 857, 227, 80),        // time area
     Rect(204, 853, 40, 80),        // modes area
     Rect(204, 853, 40, 80),        // moves area
     Rect(376, 867, 947, 61),       // inventory area
     Rect(360, 884, 888, 53),       // text area
     1,                             // statusbar coffsety
     true,                          // available fullscreen
     "-9-6-4-2-0-"                  // fallback modes fullscreen
    },
    {
     VM_1920x1440, 11, 1920, 1440,  // id, preffilenr, w, h
     64, VTS_64,                    // tilesize, tiletype
     "1920x1440", "1920x1440", "4:3", // name, fsname, fs only
     Rect(0, 0, 1280, 960),         // display area
     -400, -90,                     // menu background image offsets
     {160, 104, 5, "-160x104"},     // thumbnail size/extension
     Rect(0, 0, 1280, 832),         // game area
     Rect(0, 832, 1280, 128),       // statusbar area
     Rect(24, 857, 227, 80),        // time area
     Rect(204, 853, 40, 80),        // modes area
     Rect(204, 853, 40, 80),        // moves area
     Rect(376, 867, 947, 61),       // inventory area
     Rect(360, 884, 888, 53),       // text area
     1,                             // statusbar coffsety
     true,                          // available fullscreen
     "-11-6-4-2-0-"                 // fallback modes fullscreen
    },
    {
     VM_2048x1536, 12, 2048, 1536,  // id, preffilenr, w, h
     64, VTS_64,                    // tilesize, tiletype
     "2048x1536", "QXGA", "4:3",    // name, fsname, fs only
     Rect(0, 0, 1280, 960),         // display area
     -400, -90,                     // menu background image offsets
     {160, 104, 5, "-160x104"},     // thumbnail size/extension
     Rect(0, 0, 1280, 832),         // game area
     Rect(0, 832, 1280, 128),       // statusbar area
     Rect(24, 857, 227, 80),        // time area
     Rect(204, 853, 40, 80),        // modes area
     Rect(204, 853, 40, 80),        // moves area
     Rect(376, 867, 947, 61),       // inventory area
     Rect(360, 884, 888, 53),       // text area
     1,                             // statusbar coffsety
     true,                          // available fullscreen
     "-12-6-4-2-0-"                 // fallback modes fullscreen
    },
    {
     VM_2560x1920, 13, 2560, 1920,  // id, preffilenr, w, h
     64, VTS_64,                    // tilesize, tiletype
     "2560x1920", "2560x1920", "4:3", // name, fsname, fs only
     Rect(0, 0, 1280, 960),         // display area
     -400, -90,                     // menu background image offsets
     {160, 104, 5, "-160x104"},     // thumbnail size/extension
     Rect(0, 0, 1280, 832),         // game area
     Rect(0, 832, 1280, 128),       // statusbar area
     Rect(24, 857, 227, 80),        // time area
     Rect(204, 853, 40, 80),        // modes area
     Rect(204, 853, 40, 80),        // moves area
     Rect(376, 867, 947, 61),       // inventory area
     Rect(360, 884, 888, 53),       // text area
     1,                             // statusbar coffsety
     true,                          // available fullscreen
     "-13-6-4-2-0-"                 // fallback modes fullscreen
    },
    {
     VM_2880x2160, 14, 2880, 2160,  // id, preffilenr, w, h
     64, VTS_64,                    // tilesize, tiletype
     "2880x2160", "4K UHD", "4:3",  // name, fsname, fs only
     Rect(0, 0, 1280, 960),         // display area
     -400, -90,                     // menu background image offsets
     {160, 104, 5, "-160x104"},     // thumbnail size/extension
     Rect(0, 0, 1280, 832),         // game area
     Rect(0, 832, 1280, 128),       // statusbar area
     Rect(24, 857, 227, 80),        // time area
     Rect(204, 853, 40, 80),        // modes area
     Rect(204, 853, 40, 80),        // moves area
     Rect(376, 867, 947, 61),       // inventory area
     Rect(360, 884, 888, 53),       // text area
     1,                             // statusbar coffsety
     true,                          // available fullscreen
     "-14-6-4-2-0-"                 // fallback modes fullscreen
    },
    {
     VM_3200x2400, 15, 3200, 2400,  // id, preffilenr, w, h
     64, VTS_64,                    // tilesize, tiletype
     "3200x2400", "QUXGA", "4:3",   // name, fsname, fs only
     Rect(0, 0, 1280, 960),         // display area
     -400, -90,                     // menu background image offsets
     {160, 104, 5, "-160x104"},     // thumbnail size/extension
     Rect(0, 0, 1280, 832),         // game area
     Rect(0, 832, 1280, 128),       // statusbar area
     Rect(24, 857, 227, 80),        // time area
     Rect(204, 853, 40, 80),        // modes area
     Rect(204, 853, 40, 80),        // moves area
     Rect(376, 867, 947, 61),       // inventory area
     Rect(360, 884, 888, 53),       // text area
     1,                             // statusbar coffsety
     true,                          // available fullscreen
     "-15-6-4-2-0-"                 // fallback modes fullscreen
    },
    {
     VM_3840x2880, 16, 3840, 2880,  // id, preffilenr, w, h
     64, VTS_64,                    // tilesize, tiletype
     "3840x2880", "4K", "4:3",      // name, fsname, fs only
     Rect(0, 0, 1280, 960),         // display area
     -400, -90,                     // menu background image offsets
     {160, 104, 5, "-160x104"},     // thumbnail size/extension
     Rect(0, 0, 1280, 832),         // game area
     Rect(0, 832, 1280, 128),       // statusbar area
     Rect(24, 857, 227, 80),        // time area
     Rect(204, 853, 40, 80),        // modes area
     Rect(204, 853, 40, 80),        // moves area
     Rect(376, 867, 947, 61),       // inventory area
     Rect(360, 884, 888, 53),       // text area
     1,                             // statusbar coffsety
     true,                          // available fullscreen
     "-16-6-4-2-0-"                 // fallback modes fullscreen
    },
    {
     VM_4096x3072, 17, 4096, 3072,  // id, preffilenr, w, h
     64, VTS_64,                    // tilesize, tiletype
     "4096x3072", "4K DCI", "4:3",  // name, fsname, fs only
     Rect(0, 0, 1280, 960),         // display area
     -400, -90,                     // menu background image offsets
     {160, 104, 5, "-160x104"},     // thumbnail size/extension
     Rect(0, 0, 1280, 832),         // game area
     Rect(0, 832, 1280, 128),       // statusbar area
     Rect(24, 857, 227, 80),        // time area
     Rect(204, 853, 40, 80),        // modes area
     Rect(204, 853, 40, 80),        // moves area
     Rect(376, 867, 947, 61),       // inventory area
     Rect(360, 884, 888, 53),       // text area
     1,                             // statusbar coffsety
     true,                          // available fullscreen
     "-17-6-4-2-0-"                 // fallback modes fullscreen
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
    {VTS_32_050, "32x32 Enigma 0.50", VTS_32, 32,  VM_640x480, "models-32.lua", "gfx32-050/", VTS_32_130, false},
    {VTS_32_RETRO1, "32x32 Retro 1", VTS_32, 32,  VM_640x480, "models-32.lua", "gfx32-retro/", VTS_32_130, false},
    {VTS_32_RETRO2, "32x32 Retro 2", VTS_32, 32,  VM_640x480, "models-32.lua", "gfx32-retro2/", VTS_32_130, false},
};

}  // namespace

namespace enigma {

class VideoEngineImpl : public VideoEngine {
public:
    VideoEngineImpl();
    ~VideoEngineImpl() override;

    void Init() override;

    ecl::Screen *GetScreen() override;

    void UpdateBrightness() override;

    void SetCaption(const std::string &text) override;
    const std::string &GetCaption() override;

    std::vector<FullscreenMode> EnumerateFullscreenModes() override;
    std::vector<VideoTilesetId> EnumerateAllTilesets() override;
    std::vector<VideoTilesetId> EnumerateFittingTilesets(FullscreenMode fmode) override;
    WindowSize ActiveDisplayMode() override;
    WindowSize ActiveWindowSize() override;
    void SetVideoTileset(VideoTileset* vts) override;
    void SetDisplayMode(const WindowSize &display_mode, bool fullscreen, VideoTilesetId id) override;
    bool ApplySettings() override;
    void ResetSettings() override;
    void SaveWindowSizePreferences() override;
    void Resize(Sint32 width, Sint32 height) override;
    WindowSize SelectedWindowSize() override;
    int ActiveWindowSizeFactor() override;

    const VMInfo *GetInfo() override;
    const VMInfo *GetInfo(FullscreenMode mode) override { return &video_modes[mode]; }
    VideoTileset *GetTileset() override;
    VideoTilesetId GetTilesetId() override;

    FullscreenMode FindFullscreenMode(const WindowSize &display_mode) override;
    FullscreenMode FindClosestFullscreenMode(const WindowSize &display_mode) override;
    FullscreenMode FullscreenModeByPrefNr(int prefnr) override;
    FullscreenMode ParseVideomodesFallbackString(std::string modes, bool available_only, int seq = 1) override;

    bool SetFullscreen(bool enabled) override;
    bool ToggleFullscreen() override { return SetFullscreen(!IsFullscreen()); }
    bool IsFullscreen() override;

    ecl::Surface *BackBuffer() override;

    void Screenshot(const std::string &file_name, ecl::Surface *s = nullptr) override;

    void SetMouseCursor(std::unique_ptr<ecl::Surface> s, int hotx, int hoty) override;
    void HideMouse() override;
    void ShowMouse() override;
    int Mousex() override { return cursor->get_x(); }
    int Mousey() override { return cursor->get_y(); }

    bool GetInputGrab() override { return SDL_GetWindowMouseGrab(window); }
    bool SetInputGrab(bool enabled) override;

private:
    bool OpenWindow(int width, int height, bool fullscreen);
    bool ChangeWindowMode(int width, int height, bool fullscreen);
    void CloseWindow();

    ecl::Screen *screen;
    SDL_Window *window;
    std::unique_ptr<ecl::Surface> back_buffer;
    std::unique_ptr<MouseCursor> cursor;
    std::string window_caption;
    VideoTileset *video_tileset;
    VideoTilesetId video_tileset_id;

    bool nominal_inputgrab_status;
    std::vector<FullscreenMode> fullscreen_modes_cache;
};

VideoEngineImpl::VideoEngineImpl() :
    screen(nullptr),
    window(nullptr),
    video_tileset(nullptr),
    video_tileset_id(VTS_NONE),
    nominal_inputgrab_status(false)
{}

VideoEngineImpl::~VideoEngineImpl() {
    CloseWindow();
}

void VideoEngineImpl::Init() {
    bool isFullScreen = app.prefs->getBool("FullScreen");
    // Sanitize and save preferences for fullscreen mode.
    FullscreenMode fmode = ParseVideomodesFallbackString(app.prefs->getString("VideoModesFullscreen"), true);
    VideoTileset* fvts = VideoTilesetByName(app.prefs->getString("FullscreenTileset"));
    std::vector<FullscreenMode> available_modes = EnumerateFullscreenModes();
    auto modepos = std::find(available_modes.begin(), available_modes.end(), fmode);
    if ((fmode == VM_NONE) || (modepos == available_modes.end())) {
        const SDL_DisplayMode* dmode = SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay());
        if (dmode) {
            fmode = FindClosestFullscreenMode({dmode->w, dmode->h});
        }
        if (fmode == VM_NONE && !available_modes.empty()) {
            fmode = available_modes.back();
        }
        if (fmode != VM_NONE) {
            fvts = StandardTileset(video_modes[fmode].tt);
            app.selectedFullscreenMode = fmode;
            app.selectedFullscreenTilesetId = (fvts != nullptr) ? fvts->id : VTS_NONE;
        } else {
            app.selectedFullscreenMode = VM_NONE;
            app.selectedFullscreenTilesetId = VTS_NONE;
            isFullScreen = false;
        }
    } else {
        // Fullscreen mode is available
        if ((fvts == nullptr) || (fvts->tt != video_modes[fmode].tt))
            fvts = StandardTileset(video_modes[fmode].tt);
        app.selectedFullscreenMode = fmode;
        app.selectedFullscreenTilesetId = fvts->id;
    }
    // Sanitize and save preferences for window mode.
    int w = app.prefs->getInt("WindowWidth");
    int h = app.prefs->getInt("WindowHeight");
    int sizefactor = app.prefs->getInt("WindowSizeFactor");
    VideoTileset* wvts = VideoTilesetByName(app.prefs->getString("WindowTileset"));

    const SDL_DisplayMode *dm = SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay());
    int desktop_w = dm ? dm->w : 1920;
    int desktop_h = dm ? dm->h : 1080;
    if (w >= desktop_w || h >= desktop_h) {
        // Window dimensions were saved from a fullscreen session! Reset them.
        w = 0;
        h = 0;
    }

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
        if (!success) {
            Log << "Preferred video mode (" << w << "x" << h << " "
                << (isFullScreen ? "fullscreen" : "window") << " mode) failed.";
        }
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
        if (app.selectedFullscreenMode == VM_NONE) {
            FullscreenMode activeMode = FindFullscreenMode(ActiveDisplayMode());
            if (activeMode != VM_NONE)
                app.selectedFullscreenMode = activeMode;
        }
        app.selectedFullscreenTilesetId = GetTilesetId();
    } else {
        app.selectedWindowTilesetId = GetTilesetId();
    }
    // Set window brightness according to options.
    UpdateBrightness();
// Mac icon is set via Makefile
#ifndef MACOSX
    if (ecl::Surface *icon = enigma::GetImage("enigma_marble")) {
        SDL_SetWindowIcon(window, icon->get_surface());
    }
#endif
}

ecl::Screen *VideoEngineImpl::GetScreen() {
    return screen;
}

void VideoEngineImpl::UpdateBrightness() {
    float gamma = static_cast<float> (options::GetDouble("Gamma"));
    // gamma is a float between 0.2 and 2.0, with 1.0 meaning: normal.
    // We renormalize it to 0.6 to 1.5:
    gamma = gamma/2.0f + 0.5f;
    if (screen)
        screen->set_brightness_scale(gamma);
}

void VideoEngineImpl::SetCaption(const std::string &text) {
    if (window_caption == text)
        return;
    SDL_SetWindowTitle(window, text.c_str());
    window_caption = text;
}

const std::string &VideoEngineImpl::GetCaption() {
    return window_caption;
}

std::vector<FullscreenMode> VideoEngineImpl::EnumerateFullscreenModes() {
    if (!fullscreen_modes_cache.empty()) {
        return fullscreen_modes_cache;
    }
    std::vector<FullscreenMode> result;
    for (int i = VM_FIRST; i <= VM_LAST; ++i) {
        if (video_modes[i].f_available) {
            result.push_back(video_modes[i].mode);
        }
    }
    fullscreen_modes_cache = result;
    return fullscreen_modes_cache;
}

std::vector<VideoTilesetId> VideoEngineImpl::EnumerateAllTilesets() {
    std::vector<VideoTilesetId> ids;
    for (int i = VTSID_FIRST; i < VTSID_COUNT; i++) {
        ids.push_back((VideoTilesetId) i);
    }
    return ids;
}

std::vector<VideoTilesetId> VideoEngineImpl::EnumerateFittingTilesets(FullscreenMode fmode) {
    std::vector<VideoTilesetId> ids;
    if (fmode == VM_NONE || fmode < VM_FIRST || fmode > VM_LAST)
        return ids;
    const VMInfo* vminfo = GetInfo(fmode);
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
    VideoTileset* vts = VideoTilesetById(vtsid);
    if (!vts)
        return;
    SetVideoTileset(vts);
    ChangeWindowMode(display_mode.width, display_mode.height, fullscreen);
}

WindowSize VideoEngineImpl::SelectedWindowSize() {
    VideoTileset* vts = VideoTilesetById(app.selectedWindowTilesetId);
    assert(vts);
    int tilesize = vts->tilesize;
    if (app.selectedWindowSizeFactor == 0) {
        int w = app.prefs->getInt("WindowWidth");
        int h = app.prefs->getInt("WindowHeight");
        if (w > 0 && h > 0)
            return {w, h};
        return ActiveWindowSize();
    }
    return {app.selectedWindowSizeFactor * tilesize * 20, app.selectedWindowSizeFactor * tilesize * 15};
}

int VideoEngineImpl::ActiveWindowSizeFactor() {
    VideoTileset* vts = VideoTilesetById(app.selectedWindowTilesetId);
    assert(vts);
    return (int) (ActiveWindowSize().width / 20 / vts->tilesize);
}

// ApplySettings tries to apply the current settings set in app.prefs.
// Returns false, if nothing has been changed, true else.
bool VideoEngineImpl::ApplySettings() {
    bool wantFullscreen = app.prefs->getBool("FullScreen");
    if (wantFullscreen && !IsFullscreen())
        SaveWindowSizePreferences();
    // Do we have to change the display mode and/or active tileset?
    if (wantFullscreen && IsFullscreen()
        && (app.selectedFullscreenMode == FindFullscreenMode(ActiveDisplayMode()))
        && (app.selectedFullscreenTilesetId == GetTilesetId()))
        return false;
    if (!wantFullscreen && !IsFullscreen()
        && (SelectedWindowSize() == ActiveWindowSize())
        && (app.selectedWindowTilesetId == GetTilesetId()))
        return false;
    // Change display mode.
    if (wantFullscreen) {
        if (app.selectedFullscreenMode == VM_NONE || app.selectedFullscreenMode < VM_FIRST || app.selectedFullscreenMode > VM_LAST) {
            std::vector<FullscreenMode> modes = EnumerateFullscreenModes();
            if (!modes.empty())
                app.selectedFullscreenMode = modes.back();
            else
                app.selectedFullscreenMode = VM_640x480;
        }
        if (app.selectedFullscreenTilesetId == VTS_NONE) {
            VideoTileset *vts = StandardTileset(video_modes[app.selectedFullscreenMode].tt);
            if (vts)
                app.selectedFullscreenTilesetId = vts->id;
            else
                app.selectedFullscreenTilesetId = StandardTileset(VTS_32)->id;
        }
        int w = video_modes[app.selectedFullscreenMode].width;
        int h = video_modes[app.selectedFullscreenMode].height;
        SetDisplayMode({w,h}, true, app.selectedFullscreenTilesetId);
    } else {
        if (app.selectedWindowTilesetId == VTS_NONE)
            app.selectedWindowTilesetId = StandardTileset(VTS_32)->id;
        SetDisplayMode(SelectedWindowSize(), false, app.selectedWindowTilesetId);
    }
    // The display might have been set to different settings. Save these.
    app.prefs->setProperty("FullScreen", IsFullscreen());
    if (IsFullscreen()) {
        if (app.selectedFullscreenMode == VM_NONE) {
            FullscreenMode activeMode = FindFullscreenMode(ActiveDisplayMode());
            if (activeMode != VM_NONE)
                app.selectedFullscreenMode = activeMode;
        }
    } else {
        SaveWindowSizePreferences();
    }
    gui::LevelPreviewCache::instance()->clear();
    display::Shutdown();
    display::Init();
    return true;
}

void VideoEngineImpl::ResetSettings() {
    app.prefs->setProperty("FullScreen", false);
    app.prefs->setProperty("WindowWidth", 640);
    app.prefs->setProperty("WindowHeight", 480);
    app.prefs->setProperty("WindowTileset", StandardTileset(VTS_32)->id);
    app.prefs->setProperty("WindowSizeFactor", 1);
    app.selectedWindowSizeFactor = 1;
    app.selectedWindowTilesetId = StandardTileset(VTS_32)->id;
}

void VideoEngineImpl::SaveWindowSizePreferences() {
    if (IsFullscreen())
        return;
    WindowSize ws = ActiveWindowSize();
    if (ws.width > 0 && ws.height > 0) {
        const SDL_DisplayMode *dm = SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay());
        if (dm && (ws.width >= dm->w || ws.height >= dm->h))
            return;
        app.prefs->setProperty("WindowWidth", ws.width);
        app.prefs->setProperty("WindowHeight", ws.height);
    }
}

void VideoEngineImpl::Resize(Sint32 width, Sint32 height) {
    back_buffer.reset();
    if (screen) {
        screen->reinitScaler();
        screen->update_all();
    }
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

VideoTilesetId VideoEngineImpl::GetTilesetId() {
    return video_tileset_id;
}

bool VideoEngineImpl::SetFullscreen(bool enabled) {
    if (enabled == IsFullscreen())
        return IsFullscreen();

    if (!IsFullscreen())
        SaveWindowSizePreferences();

    SDL_SetWindowFullscreen(window, enabled);
    if (!enabled) {
        const SDL_DisplayMode *dm = SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay());
        int desktop_w = dm ? dm->w : 1920;
        int desktop_h = dm ? dm->h : 1080;

        VideoTileset* vts = VideoTilesetById(app.selectedWindowTilesetId);
        if (!vts) vts = StandardTileset(VTS_32);
        int tilesize = vts ? vts->tilesize : 32;

        int w = app.prefs->getInt("WindowWidth");
        int h = app.prefs->getInt("WindowHeight");
        int factor = app.selectedWindowSizeFactor;

        if (factor > 0) {
            w = factor * tilesize * 20;
            h = factor * tilesize * 15;
        } else if (w <= 0 || h <= 0 || w >= desktop_w || h >= desktop_h) {
            factor = 1;
            w = tilesize * 20;
            h = tilesize * 15;
            app.selectedWindowSizeFactor = 1;
            app.prefs->setProperty("WindowSizeFactor", 1);
        }
        app.prefs->setProperty("WindowWidth", w);
        app.prefs->setProperty("WindowHeight", h);

        SDL_SetWindowSize(window, w, h);
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }
    SDL_SyncWindow(window);

    back_buffer.reset();
    if (screen) {
        screen->reinitScaler();
        screen->update_all();
    }
    app.prefs->setProperty("FullScreen", IsFullscreen());
    return IsFullscreen();
}

bool VideoEngineImpl::IsFullscreen() {
    if (!window)
        return false;
    SDL_WindowFlags flags = SDL_GetWindowFlags(window);
    return (flags & SDL_WINDOW_FULLSCREEN) != 0;
}

ecl::Surface *VideoEngineImpl::BackBuffer() {
    if (!back_buffer) {
        back_buffer = Duplicate(screen->get_surface());
    }
    return back_buffer.get();
}

void VideoEngineImpl::Screenshot(const std::string &file_name, ecl::Surface *s) {
    // auto-create the directory if necessary
    std::string directory;
    if (ecl::split_path(file_name, &directory, nullptr) && !ecl::FolderExists(directory)) {
        ecl::FolderCreate(directory);
    }
    if (!s) {
        ecl::Rect rect = GetInfo()->area;
        std::unique_ptr<ecl::Surface> surface = ecl::Grab(screen->get_surface(), rect);
        ecl::SavePNG(surface.get(), file_name);
    } else {
        ecl::SavePNG(s, file_name);
    }
    Log << "Wrote screenshot to '" << file_name << "'\n";
}

void VideoEngineImpl::SetMouseCursor(std::unique_ptr<ecl::Surface> s, int hotx, int hoty) {
    cursor->set_image(std::move(s), hotx, hoty);
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
    // Try to initialize video mode, return error code on failure
    Log << "Opening in " << (fullscreen?"fullscreen":"window") << " mode, "
        << width << "x" << height << ".\n";
    window = SDL_CreateWindow(window_caption.c_str(), width, height,
                              (fullscreen ? SDL_WINDOW_FULLSCREEN : 0) | SDL_WINDOW_RESIZABLE);

    if (!window)
        return false;

    SDL_SyncWindow(window);

    assert(GetTileset());
    int tilesize = GetTileset()->tilesize;
    screen = new ecl::Screen(window, tilesize * 20, tilesize * 15);

    cursor = std::make_unique<MouseCursor>(screen);
    float fx = 0.0f, fy = 0.0f;
    SDL_GetMouseState(&fx, &fy);
    int x = static_cast<int>(fx);
    int y = static_cast<int>(fy);
    cursor->move(x, y);
    cursor->set_image(enigma::LoadImage("cur-magic"), 4, 4);
    video_engine->ShowMouse();
    SDL_HideCursor();
    UpdateBrightness();

#ifndef MACOSX
    if (ecl::Surface *icon = enigma::GetImage("enigma_marble")) {
        SDL_SetWindowIcon(window, icon->get_surface());
    }
#endif

    return true;
}

bool VideoEngineImpl::ChangeWindowMode(int width, int height, bool fullscreen) {
    if (!window)
        return OpenWindow(width, height, fullscreen);

    SetInputGrab(false);
    cursor = nullptr;
    back_buffer = nullptr;
    delete screen;
    screen = nullptr;

    if (fullscreen != IsFullscreen()) {
        SDL_SetWindowFullscreen(window, fullscreen);
    }
    if (!fullscreen) {
        SDL_SetWindowSize(window, width, height);
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }
    SDL_SyncWindow(window);

    assert(GetTileset());
    int tilesize = GetTileset()->tilesize;
    screen = new ecl::Screen(window, tilesize * 20, tilesize * 15);

    cursor = std::make_unique<MouseCursor>(screen);
    float fx = 0.0f, fy = 0.0f;
    SDL_GetMouseState(&fx, &fy);
    int x = static_cast<int>(fx);
    int y = static_cast<int>(fy);
    cursor->move(x, y);
    cursor->set_image(enigma::LoadImage("cur-magic"), 4, 4);
    video_engine->ShowMouse();
    SDL_HideCursor();
    UpdateBrightness();

    return true;
}

void VideoEngineImpl::CloseWindow() {
    if (window) {
        SetInputGrab(false);
        cursor = nullptr;
        back_buffer = nullptr;
        video_tileset = nullptr;
        delete screen;
        screen = nullptr;
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}


FullscreenMode VideoEngineImpl::FindFullscreenMode(const WindowSize &display_mode) {
    for (int i = VM_LAST; i >= VM_FIRST; --i) {
        const VMInfo &info = video_modes[i];
        if (info.width == display_mode.width && info.height == display_mode.height) {
            return info.mode;
        }
    }
    return VM_NONE;
}

FullscreenMode VideoEngineImpl::FindClosestFullscreenMode(const WindowSize &display_mode) {
    for (int i = VM_LAST; i >= VM_FIRST; --i) {
        const VMInfo &info = video_modes[i];
        if (info.width <= display_mode.width && info.height <= display_mode.height) {
            return info.mode;
        }
    }
    return VM_NONE;
}

FullscreenMode VideoEngineImpl::FullscreenModeByPrefNr(int prefnr) {
    for (int i = VM_LAST; i >= VM_FIRST; --i) {
        const VMInfo &info = video_modes[i];
        if (info.preffilenr == prefnr)
            return info.mode;
    }
    return VM_NONE;
}

/**
 * Calculate the best fullscreen mode out of the users preferences that is
 * available for the current configuration. As the user preference
 * state a sequence of fallback modes this function returns a useful
 * mode even if the user did run previously a future version of Enigma
 * and selected a mode that is not available in this Enigma version.
 * Note that the numbers in the sequence do not directly correspond
 * to the video_modes-sequence, but rather to their "preffilenr".
 * @arg  modes           string of modes to parse, typically from a pref file
 * @arg  available_only  if true, only choose modes available for fullscreen
 * @arg  seq             sequence number of best available mode, default to 1
 * @return               the preferable video mode
 */
FullscreenMode VideoEngineImpl::ParseVideomodesFallbackString(
        std::string modes, bool available_only, int seq) {
    // seq defaults to 1.
    // In Enigma 1.3, we actually use our own fallback system, so we only
    // ever use the very first known and activated mode we find and work
    // through our own fallbacks from there. Therefore, this function
    // is used with seq = 1 only.
    if (modes.length() > 1) {
        std::istringstream ms(modes);
        FullscreenMode mode = VM_NONE;
        int prefnr = -1;
        ms.ignore();  // leading '-'
        while (ms && seq > 0) {
            ms >> std::dec >> prefnr;
            ms.ignore();
            mode = FullscreenModeByPrefNr(prefnr);
            if (!available_only || video_modes[mode].f_available) {
                if (seq == 1)
                    return mode;
                --seq;
            }
        }
    }
    return VM_NONE;
}

bool VideoEngineImpl::SetInputGrab(bool enabled) {
    // From SDL 1 to SDL 2, the old grabbing commands were
    // split up into two separate pairs, one setting the mouse mode,
    // and one defining the window to be grabbed. As long as Enigma
    // uses only one window, we use both SDL_Set... commands in parallel,
    // but only SDL_GetWindowGrab to retrieve the current state. When
    // Enigma starts using several windows, this needs to be adapted.
    bool old_state = nominal_inputgrab_status;
    SDL_SetWindowMouseGrab(window, enabled);
    SDL_SetWindowRelativeMouseMode(window, enabled);
    nominal_inputgrab_status = enabled;
    // We are not returning the actual state (this would be GetInputGrab()),
    // but what we expected should have been set: A loss of window focus
    // would change GetInputGrab to false instead of true, but we might
    // want to reset to the state before losing window focus.
    return old_state;
}

// -------------------- Global variables & functions --------------------

std::unique_ptr<VideoEngine> video_engine;

void VideoInit() {
    video_engine = std::make_unique<VideoEngineImpl>();
    video_engine->Init();
    Log << "VideoInit done.\n";
}

void VideoShutdown() {
    video_engine.reset();
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
    font->render(gc, (screen->width() - font->get_width(text)) / 2, y, std::string(text));

    screen->update_all();
    screen->flush_updates();
}

VideoTileset* VideoTilesetById(VideoTilesetId id) {
    for(auto it = std::begin(video_tilesets); it != std::end(video_tilesets); ++it)
        if (it->id == id)
            return &(*it);
    return nullptr;
};

VideoTileset* VideoTilesetByName(const std::string& name) {
    if (name == "16x16 Standard")  return StandardTileset(VTS_16);
    if (name == "32x32 Standard")  return StandardTileset(VTS_32);
    if (name == "40x40 Standard")  return StandardTileset(VTS_40);
    if (name == "48x48 Standard")  return StandardTileset(VTS_48);
    if (name == "64x64 Standard")  return StandardTileset(VTS_64);
    for (auto it = std::begin(video_tilesets); it != std::end(video_tilesets); ++it) {
        if (it->name == name)
            return &(*it);
    }
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
