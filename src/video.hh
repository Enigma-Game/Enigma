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

#include <memory>
#include "SDL.h"
#include "ecl_geom.hh"
#include "ecl_video.hh"

namespace enigma {

enum VideoMode {
    VM_None = -1,
    VM_320x240,
    VM_640x480,
    VM_800x600,
    VM_960x720,
    VM_1280x960,
    VM_COUNT
};

// Tile size in pixels
enum VideoTileType {
    VTS_16 = 0,
    VTS_32 = 1,
    VTS_40 = 2,
    VTS_48 = 3,
    VTS_64 = 4,
};

struct ThumbnailInfo {
    int width, height;          // Width and height of thumbnails
    int border_width;           // width of border around thumbnail
    std::string suffix;         // suffix added to thumbnails files and dirs
};    

struct VMInfo {
    VideoMode mode;
    int width, height;             // Screen width and height in pixels
    int tile_size;                 // Tile size in pixels
    VideoTileType tt;              // Tile type
    const char *name;              // Menu text resolution
    const char *std_name;          // Menu text svg standard
    const char *relation_name;     // Menu text relation width : height
    const char *initscript;        // Lua initialization script
    const char *gfxdir;            // Directory that contains the graphics
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
};

struct DisplayMode {
    int width;
    int height;

    bool operator==(const DisplayMode &other) {
        return width == other.width &&
            height == other.height;
    }
    bool operator!=(const DisplayMode &other) {
        return !(*this == other);
    }
};



class VideoEngine {
public:
    ~VideoEngine() = default;


    virtual void Init() = 0;
    virtual void Shutdown() = 0;

    // ---------- Main window ----------
    virtual ecl::Screen *GetScreen() = 0;

    virtual void SetCaption(const std::string &text) = 0;
    virtual const std::string &GetCaption() = 0;

    virtual std::vector<DisplayMode> EnumerateDisplayModes() = 0;
    virtual DisplayMode ActiveDisplayMode() = 0;
    virtual void SetDisplayMode(const DisplayMode &display_mode, bool fullscreen) = 0;

    /*! Return information about current video mode. */
    virtual const VMInfo *GetInfo() = 0;


    
    // Switch between windowed and fullscreen mode. Returns true if fullscreen
    // mode is active afterwards.
    virtual bool SetFullscreen(bool on) = 0;
    virtual bool ToggleFullscreen() = 0;
    virtual bool IsFullscreen() = 0;
    
    // The backbuffer is surface that has the same size and pixel format as
    // the screen.
    virtual ecl::Surface *BackBuffer() = 0;

    // Take a screenshot and save it as a PNG to the specified file.
    virtual void Screenshot(const std::string &file_name) = 0;

    // ---------- Mouse cursor ----------
    virtual void SetMouseCursor(ecl::Surface *s, int hotx, int hoty) = 0;
    virtual void HideMouse() = 0;
    virtual void ShowMouse() = 0;
    virtual int Mousex() = 0;
    virtual int Mousey() = 0;

    virtual bool GetInputGrab() = 0;
    virtual bool SetInputGrab(bool enabled) = 0;
};

extern VideoEngine *video_engine;

class ScopedInputGrab {
public:
    ScopedInputGrab(bool grab_input) {
        old_status = video_engine->SetInputGrab(grab_input);
    }

    ~ScopedInputGrab() {
        video_engine->SetInputGrab(old_status);
    }

private:
    bool old_status;
};


void VideoInit();
void ShowLoadingScreen(const char *text, int progress);

} // namespace enigma

#endif
