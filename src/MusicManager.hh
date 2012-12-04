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

    void DefineMusicSingle(std::string title, std::string filename,
        float affinity_intelligence = 0, float affinity_dexterity = 0,
        float affinity_patience = 0, float affinity_knowledge = 0,
        float affinity_speed = 0);

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
    // TODO: Add Get/SetOptionInGameMusic*. Currently we have no need
    // for them, however. There is no option button either.

/* -------------------- Music and MusicQueue -------------------- */

/*! The class "MusicSingle" holds filename and default playing information
    for a single music file. Several music files can be combined into
    a "MusicQueue" to play in a given or random sequence. MusicQueues are
    used for menu music. One MusicQueue corresponds to one choice on the
    option menu's button. */

    class MusicSingle {
    public:
        MusicSingle(std::string title_, std::string filename_,
            float aff_int = 0, float aff_dex = 0, float aff_pat = 0,
            float aff_kno = 0, float aff_spe = 0)
        : title(title_), filename(filename_), affinity_intelligence(aff_int),
        affinity_dexterity(aff_dex), affinity_patience(aff_pat),
        affinity_knowledge(aff_kno), affinity_speed(aff_spe) {}

        MusicSingle()
        : title(""), filename(""), affinity_intelligence(0),
        affinity_dexterity(0), affinity_patience(0),
        affinity_knowledge(0), affinity_speed(0) {}

        bool start();
        float affinity(float lev_int, float lev_dex, float lev_pat,
                       float lev_kno, float lev_spe);

    private:
        std::string    title;
        std::string    filename;
        float          affinity_intelligence;
        float          affinity_dexterity;
        float          affinity_patience;
        float          affinity_knowledge;
        float          affinity_speed;
        // The affinity-floats measure how suitable the single is
        // for a level, based on its public ratings.
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
        float                 points_transient;
        float                 points_level;
        float                 points_total;
        // points_* are used to measure the suitability of the queue entry
        // for a specific level. points_transient contains positive points
        // for a single which is playing just now and should not be
        // interrupted, and negative points for singles that have been
        // played recently. points_level is calculated based on the
        // single's affinity to the current level. points_totel is the
        // sum of both, plus a small random number for variation.
    } MusicQueueEntry;

    enum MusicQueueShuffleType { MUSICQUEUE_NEXT, MUSICQUEUE_RANDOM, MUSICQUEUE_LEVEL };

    class MusicQueue {
    public:
        MusicQueue(std::string title_, MusicQueueShuffleType shuffle_type_, int button_position_)
        : title(title_), button_position(button_position_),
          current_position_in_queue(-1), queue_entry(), defunc(false),
          shuffle_type(shuffle_type_) {}

        MusicQueue()
        : title(""), button_position(-1),
          current_position_in_queue(-1), queue_entry(), defunc(false),
          shuffle_type(MUSICQUEUE_NEXT) {}

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
        void calculate_level_points();

    private:
        int                           current_position_in_queue;
        std::string                   title;
        int                           button_position;
        std::vector<MusicQueueEntry>  queue_entry;
        MusicQueueShuffleType         shuffle_type;
        bool                          defunc;
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
        void setInGameMusicActive(bool active);

        bool defineMusicSingle(std::string title, std::string filename,
            float affinity_intelligence, float affinity_dexterity, float affinity_patience,
            float affinity_knowledge, float affinity_speed);
        bool playMusicSingle(std::string title);
        MusicSingle getMusicSingle(std::string title);
        std::string getCurrentMusicTitle();
        void setWaiting(float seconds);
        void stopWaiting();

        bool setActiveMusicQueue(std::string music_queue_title);
        std::string getActiveMusicQueueTitle() { return active_music_queue_title; }

        // --------- Interface to options menu -----------
        bool setMenuMusicQueue(std::string music_queue_title);
        bool setInGameMusicQueue(std::string music_queue_title);
        std::string getMenuMusicQueueTitle()   { return menu_music_queue_title; }
        std::string getInGameMusicQueueTitle() { return ingame_music_queue_title; }
        int getMenuMusicQueueCount();
        std::string getMusicQueueByPosition(int button_position);
        int getMusicQueueButtonPosition(std::string music_queue_title) {
            return music_queues[music_queue_title].getButtonPosition();
        }

    protected:
        MusicManager();
        
    private:
        static MusicManager     *theSingleton;
        MusicSingleRepository    music_singles;
        MusicQueueRepository     music_queues;
        std::string              active_music_queue_title;
        std::string              ingame_music_queue_title;
        std::string              menu_music_queue_title;
        float                    wait_length;   // in seconds
        MusicContext             music_context;
        bool                     is_waiting;    // true if a silent break is inserted
        bool                     ingame_music_active;
    };
    
}

#endif
