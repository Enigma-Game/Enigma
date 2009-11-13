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
 *
 */

#ifndef VIDEO_HH_INCLUDED
#define VIDEO_HH_INCLUDED

#include <memory>
#include "SDL.h"
#include "ecl_fwd.hh"
#include "ecl_geom.hh"

namespace video
{

    enum VideoModes {
        VM_None      = -1,
        VM_640x480   = 0,   ///< 32 bit basic    -  4:3  - VGA
        VM_640x512   = 1,   ///< 32 bit embedded -  5:4  - none
        VM_800x600   = 2,   ///< 40 bit basic    -  4:3  - SVGA
        VM_1024x768  = 3,   ///< 48 bit embedded -  4:3  - XGA
        VM_960x720   = 4,   ///< 48 bit basic    -  4:3  - none
        VM_1280x720  = 5,   ///< 48 bit embedded - 16:9  - HD720
        VM_1280x960  = 6,   ///< 64 bit basic    -  4:3  - none
        VM_1440x960  = 7,   ///< 64 bit embedded -  3:2  - none
        VM_1280x1024 = 8,   ///< 64 bit embedded -  5:4  - SXGA
        VM_1680x1050 = 9,   ///< 64 bit embedded - 16:10 - WSXGA+
        VM_320x240   = 10,  ///< 16 bit basic    -  4:3  - CGA
        VM_COUNT
    };
    
    enum VideoTileType {    // Tile size in pixels
        VTS_16 = 0,
        VTS_32 = 1,
        VTS_40 = 2,
        VTS_48 = 3,
        VTS_64 = 4,
    };

    struct VMInfo {
        VideoModes     videomode;
        int            width, height;    // Screen width and height in pixels
        int            tile_size;        // Tile size in pixels
        VideoTileType  tt;               // Tile type
        const char    *name;             // Menu text resolution
        const char    *std_name;         // Menu text svg standard
        const char    *relation_name;    // Menu text relation width : height
        const char    *initscript;       // Lua initialization script
        const char    *gfxdir;           // Directory that contains the graphics
        ecl::Rect      area;             // Area that is used for display
        int            mbg_offsetx, mbg_offsety; // offsets for menu background image
        int            thumbw, thumbh;   // Width and height of thumbnails
        std::string    thumbsext;        // extension added to thumbnails files and dirs
        ecl::Rect      gamearea;
        ecl::Rect      statusbararea;
        ecl::Rect      sb_timearea;
        ecl::Rect      sb_modesarea;
        ecl::Rect      sb_movesarea;
        ecl::Rect      sb_itemarea;
        ecl::Rect      sb_textarea;
        bool           w_available;        // Is this video mode available?
        bool           f_available;        // Is this video mode available?
        std::string    fallback_window;    // hyphen seperated list of modes e.g. "-7-4-2-0-"
        std::string    fallback_fullscreen;
    };

    void Init();
    void Shutdown();

    void ChangeVideoMode();
    bool SetFullscreen(bool onoff);

    /*! Switch between windowed and fullscreen mode. Return true if
       fullscreen mode is active afterwards. */
    bool ToggleFullscreen();

    bool IsFullScreen();

    /*! Return information about arbitrary video mode. */
    const VMInfo *GetInfo (VideoModes vm);

    /*! Return information about current video mode. */
    const VMInfo *GetInfo();

    // just for main batch thumb generation in wrong videomode
    void SetThumbInfo(int width, int height, std::string extension);

    /**
     * Count number of available modes for the current configuration.
     * @arg  isFullScreen   video mode for fullscreen or window mode
     * @return   number of modes
     */
    int GetNumAvailableModes(bool isFullscreen);
    
    /**
     * Get the video mode by the sequence number of available modes for the
     * current configuration
     * @arg  number         desired sequence number within the list of available modes
     * @arg  isFullScreen   video mode for fullscreen or window mode
     * @return   the requested video mode
     */
    VideoModes GetVideoMode(int number, bool isFullscreen);
    
    /**
     * Calulate the sequence number for a given mode within the number of
     * available modes for the current configuration.
     * @arg  mode           the video mode to locate
     * @arg  isFullScreen   video mode for fullscreen or window mode
     * @return   the sequence number of the mode
     */
    int GetModeNumber(VideoModes mode, bool isFullScreen);
    
    /**
     * Calculate the best video mode out of the users preferences that is
     * available for the current configuration. As the user preference 
     * state a sequence of fallback modes this function returns a useful 
     * mode even if the user did run previously a future version of Enigma
     * and selected a mode that is not available in this Enigma version.
     * @arg  isFullScreen   video mode for fullscreen or window mode
     * @arg  seq            sequence number of best available mode, default to 1
     * @return   the preferable video mode 
     */
    VideoModes GetBestUserMode(bool isFullScreen, int seq = 1);
    
    bool ModeAvailable (VideoModes vm);

    //! Return the current video mode
    VideoModes GetVideoMode();

    /*! Return the number of bits per pixel in the current video
      mode. [currently always 16] */
    int GetColorDepth();

    ecl::Screen *GetScreen();

    /*! The backbuffer is surface that has the same size and pixel
      format as the screen.  This surface is used by ShowScreen() and
      FX_* functions below. */
    ecl::Surface *BackBuffer();
    
    /*! Update gamma correction using current options. */
    void UpdateGamma();

    void               SetCaption (const char *str);
    const std::string& GetCaption();

    /*! Take a screenshot and save it as a PNG to file FNAME. */
    void Screenshot (const std::string &fname);

/* -------------------- Input grabbing -------------------- */

    class TempInputGrab {
    public:
        TempInputGrab (bool onoff);
        ~TempInputGrab();
    private:
        bool old_onoff;
    };

    bool GetInputGrab();
    bool SetInputGrab (bool onoff);

/* -------------------- Mouse cursor -------------------- */

    void SetMouseCursor(ecl::Surface *s, int hotx, int hoty);
    void HideMouse();
    void ShowMouse();
    int Mousex();
    int Mousey();

/* -------------------- Visual effects -------------------- */

    class TransitionEffect {
    public:
        virtual ~TransitionEffect() {}
        virtual void tick (double dtime) = 0;
        virtual bool finished() const = 0;
    };

    enum FadeMode { FADEIN, FADEOUT };
    void FX_Fade (FadeMode mode);
    void FX_Fly (ecl::Surface *newscr, int originx, int originy);

    enum TransitionModes
    {
	TM_RANDOM,
	TM_FADEOUTIN,
	TM_SQUARES,
	TM_FLY_N, TM_FLY_S, TM_FLY_W, TM_FLY_E,
	TM_FLY_NW, TM_FLY_NE, TM_FLY_SE, TM_FLY_SW,
        TM_PUSH_RANDOM, TM_PUSH_N, TM_PUSH_S, TM_PUSH_W, TM_PUSH_E
    };
    void ShowScreen (TransitionModes tm, ecl::Surface *newscr);

    TransitionEffect *MakeEffect (TransitionModes tm, ecl::Surface *newscr);
}

#endif
