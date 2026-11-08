/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
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
#ifndef VIDEO_HH_INCLUDED
#define VIDEO_HH_INCLUDED

// This file contains code for managing the current window.

#include "ecl_geom.hh"
#include "ecl_video.hh"
#include <SDL3/SDL.h>

#include <memory>

namespace enigma {

enum FullscreenMode {
    VM_NONE = -1,
    VM_FIRST = 0,
    VM_320x240 = 0,    ///< 16x16 basic    -  4:3  - CGA / QVGA
    VM_640x480 = 1,    ///< 32x32 basic    -  4:3  - VGA
    VM_800x600 = 2,    ///< 40x40 basic    -  4:3  - SVGA
    VM_960x720 = 3,    ///< 48x48 basic    -  4:3  - 960x720
    VM_1024x768 = 4,   ///< 48x48 scaled   -  4:3  - XGA
    VM_1152x864 = 5,   ///< 48x48 scaled   -  4:3  - XGA+
    VM_1280x960 = 6,   ///< 64x64 basic    -  4:3  - SXGA- / Quad-VGA
    VM_1400x1050 = 7,  ///< 64x64 scaled   -  4:3  - SXGA+
    VM_1440x1080 = 8,  ///< 64x64 scaled   -  4:3  - HDV / 1080p
    VM_1600x1200 = 9,  ///< 64x64 scaled   -  4:3  - UXGA
    VM_1920x1440 = 10, ///< 64x64 scaled   -  4:3  - 1440p
    VM_2048x1536 = 11, ///< 64x64 scaled   -  4:3  - QXGA
    VM_2560x1920 = 12, ///< 64x64 scaled   -  4:3  - 1920p
    VM_2880x2160 = 13, ///< 64x64 scaled   -  4:3  - 4K UHD
    VM_3200x2400 = 14, ///< 64x64 scaled   -  4:3  - QUXGA
    VM_3840x2880 = 15, ///< 64x64 scaled   -  4:3  - 4K UHD
    VM_4096x3072 = 16, ///< 64x64 scaled   -  4:3  - 4K DCI
    VM_LAST = 16,
    VM_COUNT = 17
};

struct WindowSize {
    int width;
    int height;

    bool operator==(const WindowSize &other) const {
        return width == other.width && height == other.height;
    }
    bool operator!=(const WindowSize &other) const {
        return !(*this == other);
    }
};

// Tile size in pixels
enum VideoTileType {
    VTS_16 = 0,
    VTS_32 = 1,
    VTS_40 = 2,
    VTS_48 = 3,
    VTS_64 = 4,
};

enum VideoTilesetId {
    VTS_NONE = -1,
    VTSID_FIRST = 0,
    VTS_16_130 = 0,
    VTS_32_130 = 1,
    VTS_40_130 = 2,
    VTS_48_130 = 3,
    VTS_64_130 = 4,
    VTS_32_050 = 5,
    VTS_32_RETRO1 = 6,
    VTS_32_RETRO2 = 7,
    VTSID_COUNT
};

struct VideoTileset {
    VideoTilesetId id;             // Id of tileset
    const char *name;              // Name of tileset
    VideoTileType tt;              // Tile type (encoding tile width and height)
    int tilesize;                  // Tile size
    FullscreenMode OptimalFullscreenMode;  // Tile width and height times 20x13
    const char *initscript;        // Lua initialization script
    const char *gfxdir;            // Directory that contains the graphics
    VideoTilesetId fallback;       // Fallback tileset if images are missing
    bool is_standard;              // There should be one standard tileset per tile type.
};

struct ThumbnailInfo {
    int width, height;          // Width and height of thumbnails
    int border_width;           // width of border around thumbnail
    std::string suffix;         // suffix added to thumbnails files and dirs
};    

struct VMInfo {
    FullscreenMode mode;
    int preffilenr;                // Mode number in preference file, 1.0-compatible
    int width, height;             // Screen width and height in pixels
    int tile_size;                 // Tile size in pixels
    VideoTileType tt;              // Tile type
    const char *name;              // Menu text resolution
    const char *std_name;          // Menu text svg standard
    const char *relation_name;     // Menu text relation width : height
    ecl::Rect area;                // Area that is used for display
    int mbg_offsetx, mbg_offsety;  // offsets for menu background image
    ThumbnailInfo thumb;
    ecl::Rect gamearea;
    ecl::Rect statusbararea;
    ecl::Rect sb_timearea;
    ecl::Rect sb_modesarea;
    ecl::Rect sb_movesarea;
    ecl::Rect sb_itemarea;
    ecl::Rect sb_textarea;
    int sb_coffsety;              // center offset of statusbar due to top black lines
    bool f_available;             // Is this video mode in fullscreen available?
    std::string f_fallback;       // hyphen separated list of modes e.g. "-7-4-2-0-"
};



class VideoEngine {
public:
    virtual ~VideoEngine() = default;

    virtual void Init() = 0;

    // ---------- Main window ----------
    virtual ecl::Screen *GetScreen() = 0;

    /*! Update gamma correction using current options. */
    virtual void UpdateBrightness() = 0;

    virtual void SetCaption(const std::string &text) = 0;
    virtual const std::string &GetCaption() = 0;

    virtual std::vector<FullscreenMode> EnumerateFullscreenModes() = 0;
    virtual std::vector<VideoTilesetId> EnumerateAllTilesets() = 0;
    virtual std::vector<VideoTilesetId> EnumerateFittingTilesets(FullscreenMode fmode) = 0;
    virtual WindowSize ActiveDisplayMode() = 0;
    virtual WindowSize ActiveWindowSize() = 0;
    virtual void SetVideoTileset(VideoTileset* vts) = 0;
    virtual void SetDisplayMode(const WindowSize &display_mode, bool fullscreen, VideoTilesetId vtsid) = 0;
    virtual bool ApplySettings() = 0;
    virtual void ResetSettings() = 0;
    virtual void SaveWindowSizePreferences() = 0;
    virtual void Resize(Sint32 width, Sint32 height) = 0;
    virtual WindowSize SelectedWindowSize() = 0;
    virtual int ActiveWindowSizeFactor() = 0;

    /*! Return information about current or other video mode(s) and chosen tileset. */
    virtual const VMInfo *GetInfo() = 0;
    virtual const VMInfo *GetInfo(FullscreenMode mode) = 0;
    virtual VideoTileset *GetTileset() = 0;
    virtual VideoTilesetId GetTilesetId() = 0;

    /*! Identify fullscreen modes by window size or other properties. */
    virtual FullscreenMode FindFullscreenMode(const WindowSize &display_mode) = 0;
    virtual FullscreenMode FindClosestFullscreenMode(const WindowSize &display_mode) = 0;
    virtual FullscreenMode FullscreenModeByPrefNr(int prefnr) = 0;
    virtual FullscreenMode ParseVideomodesFallbackString(std::string modes, bool available_only, int seq = 1) = 0;

    // Switch between windowed and fullscreen mode. Returns true if fullscreen
    // mode is active afterwards.
    virtual bool SetFullscreen(bool on) = 0;
    virtual bool ToggleFullscreen() = 0;
    virtual bool IsFullscreen() = 0;
    
    // The backbuffer is surface that has the same size and pixel format as
    // the screen.
    virtual ecl::Surface *BackBuffer() = 0;

    // Take a screenshot and save it as a PNG to the specified file.
    virtual void Screenshot(const std::string &file_name, ecl::Surface *s = nullptr) = 0;

    // ---------- Mouse cursor ----------
    virtual void SetMouseCursor(std::unique_ptr<ecl::Surface> s, int hotx, int hoty) = 0;
    virtual void HideMouse() = 0;
    virtual void ShowMouse() = 0;
    virtual int Mousex() = 0;
    virtual int Mousey() = 0;

    virtual bool GetInputGrab() = 0;
    virtual bool SetInputGrab(bool enabled) = 0;
};

// ---------- Global variables ----------

extern std::unique_ptr<VideoEngine> video_engine;

// ---------- ScopedInputGrab ----------

class ScopedInputGrab {
public:
    explicit ScopedInputGrab(bool grab_input) {
        old_status = video_engine->SetInputGrab(grab_input);
    }

    ~ScopedInputGrab() {
        video_engine->SetInputGrab(old_status);
    }

private:
    bool old_status;
};

// ---------- Video API ----------

void VideoInit();
void VideoShutdown();
void ShowLoadingScreen(const char *text, int progress);

VideoTileset* VideoTilesetById(VideoTilesetId id);
VideoTileset* VideoTilesetByName(const std::string& name);
VideoTileset* StandardTileset(VideoTileType tt);
std::string VideoTilesetPrefName(VideoTilesetId id);

} // namespace enigma

#endif
