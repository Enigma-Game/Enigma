/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007,2008,2009 Andreas Lochmann
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

namespace sound
{

/* -------------------- Interface Functions -------------------- */

    void DefineMusicSingle(std::string title, std::string filename);

    void StartMenuMusic();
    void StartLevelLoadMusic();
    void StartLevelMusic();
    void MusicTick(double dtime);
    void InitMusic();
    void SetInGameMusicActive(bool active);

    /*! Helper functions for options menu */
    int GetOptionMenuMusicCount();
    int GetOptionMenuMusic();
    void SetOptionMenuMusic(int value);
    std::string GetOptionMenuMusicText(int value);

/* -------------------- Music and MusicQueue -------------------- */

/*! The class "MusicSingle" holds filename and default playing information
    for a single music file. Several music files can be combined into
    a "MusicQueue" to play in a given or random sequence. MusicQueues are
    used for menu music. One MusicQueue corresponds to one choice on the
    option menu's button. */

    class MusicSingle {
    public:
        MusicSingle(std::string title_, std::string filename_, bool fadeout_ = false)
        : title(title_), filename(filename_) {}

        MusicSingle()
        : title(""), filename("") {}

        bool start();

    private:
        std::string title;
        std::string filename;
    };

    enum MusicQueueEntryType { MUSICQUEUE_SINGLE, MUSICQUEUE_WAIT };

    typedef struct {
        MusicQueueEntryType   type;
        std::string           title;
        bool                  fadeout_on_end;
        float                 wait_length;
        bool                  no_music;
        // no_music is a boolean to mark queue entries that do not play music, 
        // be it because e.g. they are waiting phases, or because their file is
        // missing. Queues without any music are eventually stopped this way.
    } MusicQueueEntry;

    class MusicQueue {
    public:
        MusicQueue(std::string title_, int button_position_)
        : title(title_), button_position(button_position_),
          current_position_in_queue(-1), queue_entry(), defunc(false) {}

        MusicQueue()
        : title(""), button_position(-1),
          current_position_in_queue(-1), queue_entry(), defunc(false) {}

        bool start();                // Start queue from the beginning.
        bool next(bool force_music = false);  // Execute next queue entry (e.g. play music).
        bool onMusicEnded(bool force_music);  // Called when no music is playing anymore.
        bool onWaitEnded();          // Called when waiting phase ended.
        void leave();                // Leave queue before switching to another queue.
        void assureQueueHasMusic();  // Assure that queue has music, deactivate otherwise.
        std::string getCurrentMusicTitle();
        int getButtonPosition() { return button_position; }
        void setButtonPosition(int pos) { button_position = pos; }
        void appendSingle(std::string title, bool fadeout_on_end);
        void appendSingleThenWait(std::string title, bool fadeout_on_end, float seconds);
        void appendWait(float seconds);

    private:
        int current_position_in_queue;
        std::string title;
        int button_position;
        std::vector<MusicQueueEntry> queue_entry;
        bool defunc;
        // defunc is true if the queue should be stopped, e.g. because it
        // doesn't contain any music. Does not apply to empty queues.
    };

    typedef std::map<std::string, MusicSingle> MusicSingleRepository;
    typedef std::map<std::string, MusicQueue> MusicQueueRepository;

    /*! MusicContext is a nominal condition to change the
      music during the next tick.
      NONE: during initialisation (don't play music now),
      MENU/GAME: play music suitable for menu or during game.
      LEVEL_LOADING: this phase is for fading out menu music during the level load. */
    enum MusicContext { MUSIC_NONE, MUSIC_MENU, MUSIC_LEVEL_LOADING, MUSIC_GAME };

/* -------------------- MusicManager -------------------- */

    class MusicManager {
    public:
        static MusicManager* instance();
        ~MusicManager() {}

        void init();
        void tick(double dtime);

        // ---------- Music and music repository ----------

        void setMusicContext(MusicContext new_context);
        MusicContext getMusicContext() { return music_context; }

        bool defineMusicSingle(std::string title, std::string filename);
        bool playMusicSingle(std::string title);
        std::string getCurrentMusicTitle();
        void setWaiting(float seconds);
        void stopWaiting();

        // --------- Interface to options menu -----------
        bool setActiveMusicQueue(std::string music_queue_title);
        std::string getMusicQueueByPosition(int button_position);
        std::string getActiveMusicQueueTitle() { return active_music_queue_title; }
        int getMenuMusicQueueCount();
        int getMusicQueueButtonPosition(std::string music_queue_title) {
            return music_queues[music_queue_title].getButtonPosition();
        }

    protected:
        MusicManager();
        
    private:
        static MusicManager *theSingleton;
        MusicSingleRepository    music_singles;
        MusicQueueRepository     music_queues;
        std::string              active_music_queue_title;
        float                    wait_length;   // in seconds
        MusicContext             music_context;
        bool                     is_waiting;    // true if a silent break is inserted
    };
    
}

#endif
