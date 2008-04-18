/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007,2008 Andreas Lochmann
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
#ifndef MUSICMGR_HH_INCLUDED
#define MUSICMGR_HH_INCLUDED

#include "SDL_mixer.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace sound
{

/* -------------------- Interface Functions -------------------- */

    void DefineMusicSingle(string title, string filename);

    bool StartMenuMusic();
    bool StartLevelMusic();
    void MusicTick(double dtime);
    void InitMusic();
    void SetInGameMusicActive(bool active);

    /*! Helper functions for options menu */
    int GetOptionMenuMusicCount();
    int GetOptionMenuMusic();
    void SetOptionMenuMusic(int value);
    string GetOptionMenuMusicText(int value);

/* -------------------- Music and MusicQueue -------------------- */

/*! The class "MusicSingle" holds filename and default playing information
    for a single music file. Several music files can be combined into
    a "MusicQueue" to play in a given or random sequence. MusicQueues are
    used for menu music. One MusicQueue corresponds to one choice on the
    option menu's button. */

    class MusicSingle {
    public:
        MusicSingle(string title_, string filename_, Uint32 length_,
                    Uint32 loop_start_, Uint32 loop_end_, bool allows_loop_)
        : title(title_), filename(filename_), length(length_),
          loop_start(loop_start_), loop_end(loop_end_),
          allows_loop(allows_loop_), start_time() {}

        MusicSingle(string title_, string filename_)
        : title(title_), filename(filename_), length(0),
          loop_start(0), loop_end(0), allows_loop(false), start_time() {}

        MusicSingle()
        : title(""), filename(""), length(0), loop_start(0), loop_end(0),
          allows_loop(false) {}

        bool start();
        bool maybeLoopBack();

    private:
        string title;
        string filename;
        Uint32 length;      // in milliseconds
        Uint32 loop_start;  // where the loop starts
        Uint32 loop_end;    // where the loop should end (but continues playing until next tick)
        Uint32 start_time;  // number of milliseconds since SDL init
        bool allows_loop;
    };

    class MusicQueue {
    public:
        MusicQueue(string title_, int button_position_)
        : title(title_), button_position(button_position_),
          current_position_in_queue(-1), single_title() {}

        MusicQueue()
        : title(""), button_position(-1),
          current_position_in_queue(-1), single_title() {}

        bool start();
        bool next();
        void leave();
        string getCurrentMusicTitle();
        int getButtonPosition() { return button_position; }
        void setButtonPosition(int pos) { button_position = pos; }
        void appendSingle(string title);

    private:
        int current_position_in_queue;
        string title;
        int button_position;
        vector<string> single_title;
    };

    typedef map<string, MusicSingle> MusicSingleRepository;
    typedef map<string, MusicQueue> MusicQueueRepository;

    /*! MusicContext is a nominal condition to change the
      music during the next tick.
      NONE: during initialisation (don't play music now),
      MENU/GAME: play music suitable for menu or during game. */
    enum MusicContext { MUSIC_NONE, MUSIC_MENU, MUSIC_GAME };

/* -------------------- MusicManager -------------------- */

    class MusicManager {
    public:
        static MusicManager* instance();
        ~MusicManager() {}

        void init();
        void tick(double dtime);

        // ---------- Music and music repository ----------

        void setMusicContext(MusicContext context) { music_context = context; }
        MusicContext getMusicContext() { return music_context; }

        bool defineMusicSingle(string title, string filename);
        bool playMusicSingle(string title);
        string getCurrentMusicTitle();
                
        bool setActiveMusicQueue(string music_queue_title);
        string getActiveMusicQueueTitle() { return active_music_queue; }
        string getMusicQueueByPosition(int button_position);
        int getMenuMusicQueueCount();
        int getMusicQueueButtonPosition(string music_queue_title) {
            return music_queues[music_queue_title].getButtonPosition();
        }

    protected:
        MusicManager();
        
    private:
        static MusicManager *theSingleton;
        MusicSingleRepository    music_singles;
        MusicQueueRepository     music_queues;
        string                   active_music_queue;
        MusicContext             music_context;
    };
    
}

#endif
