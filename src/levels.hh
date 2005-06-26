/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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
#ifndef LEVELS_HH
#define LEVELS_HH

#include "enigma.hh"
#include "game.hh"
#include <memory>               // for auto_ptr

namespace enigma_levels
{
    using namespace enigma;
    using std::string;

/* -------------------- Exceptions -------------------- */

    class XLevelLoading : public ecl::XGeneric {
    public:
        XLevelLoading (const std::string &msg)
        : XGeneric (msg)
        {}
    };

    class XLevelPackInit : public ecl::XGeneric {
    public:
        XLevelPackInit (const std::string &msg)
        : XGeneric (msg)
        {}
    };
    
    class XLevelRuntime : public ecl::XGeneric {
    public:
        XLevelRuntime (const std::string &msg)
        : XGeneric (msg)
        {}
    };
    
/* -------------------- LevelInfo -------------------- */

    /*! This datastructure contains information about individual
      levels. */
    struct LevelInfo
    {
        // Constructor
        LevelInfo();

        // Constants
        enum { DEFAULT_TIME = 99*60+59 };

        // Variables.
        GameType type;
        string   filename;              //< Filename of the level (exl. extension)
        string   indexname;             //< The name used in the options file, 
                                        //   empty string -> use filename entry
        string   name;                  //< Complete name of the level
        string   author;                //< Author of the level
        int      revision;              //< Revision # of this level
        bool     has_easymode;          //< whether level has an easymode
        int      par_time_easy;        //< Best time in seconds (for easy mode)
        int      par_time_normal;      //< Best time in seconds (for normal mode)
        string   par_time_easy_by;     //< player name(s) for 'best_time_easy'
        string   par_time_normal_by;   //< same for 'best_time_normal'
        int      par_moves;            //< Minimum moves to solve level

    };

/* -------------------- LevelStatus -------------------- */

    struct LevelStatus {
        LevelStatus(int easy=-1, int hard=-1, int fin=0, int solved_rev = 0);
        bool operator == (const LevelStatus& other) const;

        // Variables.

        int time_easy;         // user's best time in seconds (-1: NA)
        int time_hard;         // user's best time in seconds (-1: NA)
        int finished;          // Level solved? 0 = no,1=easy,2=hard,3=easy&hard
        int solved_revision;   // Revision #  that was solved
    };

/* -------------------- LevelPackInfo -------------------- */

    struct LevelPackInfo {
        // Variables
        string filename;        // directory name of level pack
        string displayname;     // name to be displayed in menu
        size_t nlevels;         // number of levels

        // Constructor
    };


/* -------------------- LevelPack -------------------- */

    class LevelPack {
    public:
        virtual ~LevelPack() {}

        /* ---------- LevelPack interface ---------- */

        /*! Return level pack's name */
        virtual string get_name() const = 0;

        /*! Return number of levels */
        virtual size_t size() const = 0;

        /*! Load a level into the game engine.  Throws XLevelLoading
          when an error occurs. */
        virtual void load_level (size_t index) = 0;

        /*! Return the level information for level INDEX */
        virtual const LevelInfo &get_info (size_t index) const = 0;

        virtual ecl::Surface *load_preview (size_t /*index*/) { return 0; }

        /*! Return revision number of level INDEX. */
        virtual int get_revision_number(size_t index) const = 0;

        /*! Return the preview version number (needed for Oxyd-levelpacks) */
        virtual int get_preview_version() const { return 1; }

        /*! Return the default SoundSet (see options::SoundSet for meaning) */
        virtual int get_default_SoundSet() const = 0;

        /*! Returns true if it's a twoplayer levelpack, but has no
          it-yinyang (needed to add it-yinyang to inventory if
          oxyd-linkgame is played as single-player) */
        virtual bool needs_twoplayers() const = 0;

        /*! Returns true if LevelPack may have previews */
        virtual bool may_have_previews() const = 0;
    };

/* -------------------- Level -------------------- */

    class Level {
    public:
        Level (LevelPack *levelpack, size_t index)
        : m_levelpack (levelpack),
          m_index (index)
        {}

        const LevelInfo &get_info () const;

        bool   get_status (LevelStatus &levelstat) const;
        void   set_status (const LevelStatus &levelstat);
        void   mark_solved ();
        void   mark_unsolved();

        int    get_par_time (int difficulty) const;
        bool   set_level_time (int difficulty, int time);
        string get_par_holder (int difficulty);
        bool   is_solved (int difficulty);
        int    get_best_user_time (int difficulty) const;
        int    get_revision () const;

        Level next_level (int difficulty, bool skip_solved, bool select_non_par);

        // Accessors
        size_t get_index () const { return m_index; }
        LevelPack *get_levelpack () const { return m_levelpack; }
    private:

        // Variables
        LevelPack *m_levelpack;
        size_t     m_index;
    };

    enum LevelAdvanceMode {
        advance_normal,         // honor SkipSolvedLevels/TimeHunting options
        advance_strictly_next,  // move to the next level
        advance_unsolved,       // move to the next unsolved / non-par level
    };

    bool advance_level (Level &l, LevelAdvanceMode m);

/* -------------------- Global variables -------------------- */

    extern std::vector<LevelPack *> LevelPacks;

/* -------------------- Functions -------------------- */

    unsigned     HighestAvailableLevel(LevelPack *lp);
    unsigned     CountSolvedLevels (LevelPack *lp, int difficulty, unsigned *par_counter);
    bool         LevelIsLocked (const Level &l);

    /* ---------- Levelpack management ---------- */

    LevelPack *FindLevelPack (const std::string &name);
    bool       FindLevel (const std::string &levelname, Level &level);
    int        IndexOfLevelPack(const LevelPack *lp);
    void       RegisterLevelPack (LevelPack *lp);

    // levelpack.cc
    void AddLevelPack (const char *init_file, const char *name);

    void AddSimpleLevelPack (const std::vector<std::string> &levels, 
                             const char *name);

    void AddZippedLevelPack (const char *zipfile);

}

namespace enigma
{
    namespace levels = enigma_levels;

    using levels::LevelStatus;
    using levels::Level;
}
#endif
