/*
 * Copyright (C) 2003,2004 Daniel Heck
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
 * $Id: levels.cc,v 1.8 2004/05/22 13:04:30 dheck Exp $
 */

#include "main.hh"
#include "sound.hh"
#include "options.hh"
#include "display.hh"
#include "video.hh"
#include "server.hh"
#include <cassert>
#include <algorithm>

using namespace std;
using namespace enigma;
using namespace enigma::levels;

/* -------------------- LevelInfo -------------------- */

LevelInfo::LevelInfo() 
{
    type            = enigma::GAMET_ENIGMA;
    revision        = 1;
    has_easymode    = false;
    par_time_easy   = -1; //DEFAULT_TIME;
    par_time_normal = -1; //DEFAULT_TIME;
}



/* -------------------- LevelStatus implementation -------------------- */

LevelStatus::LevelStatus(int easy, int hard, int finished_, int solved_rev)
: time_easy (easy),
  time_hard (hard),
  finished (finished_),
  solved_revision (solved_rev)
{
}

bool LevelStatus::operator == (const LevelStatus& other) const
{
    return (time_easy == other.time_easy &&
            time_hard == other.time_hard &&
            finished == other.finished &&
            solved_revision == other.solved_revision);
}


/* -------------------- Level implementation -------------------- */

int Level::get_revision () const
{
    return m_levelpack->get_revision_number(m_index);
}

void Level::set_status (const LevelStatus &levelstat) 
{
    options::SetLevelStatus(get_info().filename, levelstat);
}

bool Level::set_level_time (int difficulty, int time)
{
    int revision = m_levelpack->get_revision_number (m_index);

    LevelStatus levelstat (-1, -1, difficulty, revision);

    if (get_status (levelstat)) {
        if (levelstat.finished == 0 || 
            (levelstat.solved_revision >= 1 && levelstat.solved_revision < revision))
        {
            // reset level status if level marked as not finished (the
            // user can do so explicitly in the level menu) or if only
            // an old revision of the level was solved.
            levelstat.time_hard = -1;
            levelstat.time_easy = -1;
            levelstat.finished  = 0;
        }
        levelstat.finished        |= difficulty;
        levelstat.solved_revision  = revision;
    }

    bool new_record = false;
    int& time_curr  = (difficulty == enigma::DIFFICULTY_EASY) 
        ? levelstat.time_easy : levelstat.time_hard;

    if (time_curr > time || time_curr == -1) {
        time_curr  = time;
        new_record = true;
    }
    set_status (levelstat);
    return new_record;
}

int Level::get_par_time (int difficulty) const
{
    const LevelInfo &levelinfo = get_info();
    int par_time = difficulty == DIFFICULTY_HARD
        ? levelinfo.par_time_normal
        : levelinfo.par_time_easy;

    if (par_time == LevelInfo::DEFAULT_TIME)
        return -1;
    else
        return par_time;
}

string Level::get_par_holder (int difficulty) 
{
    const LevelInfo &levelinfo = get_info();

    return difficulty == DIFFICULTY_HARD
        ? levelinfo.par_time_normal_by
        : levelinfo.par_time_easy_by;
}


bool Level::get_status (LevelStatus &levelstat) const
{
    return options::GetLevelStatus (get_info().filename, levelstat);
}

const LevelInfo &Level::get_info () const
{
    return m_levelpack->get_info(m_index);
}


int Level::get_best_user_time (int difficulty) const
{
    const LevelInfo &levelinfo = get_info();
    LevelStatus levelstat;

    int best_user_time = -1;
    if (get_status (levelstat)) {
        if (levelinfo.has_easymode) {
            if ((levelstat.finished & difficulty) != 0) {
                best_user_time = (difficulty == DIFFICULTY_HARD) // XXX should use difficulty parameter
                    ? levelstat.time_hard 
                    : levelstat.time_easy;
            }
        }
        else {
            // If level is the same in easy and hard mode use the user's
            // best time in either difficulty level.
            if (levelstat.finished) {
                if (levelstat.time_hard > 0) 
                    best_user_time = levelstat.time_hard;
                if (levelstat.time_easy > 0 && (levelstat.time_easy < best_user_time || best_user_time<0))
                    best_user_time = levelstat.time_easy;
            }
        }
        if (levelstat.solved_revision < levelinfo.revision)
            best_user_time = -1;
    }
    assert (best_user_time >= -1);
    return best_user_time;
}

bool Level::is_solved (int difficulty)
{
    const LevelInfo &levelinfo = get_info();
    LevelStatus levelstat;

    if (get_status(levelstat))
        return ((levelstat.finished & difficulty) ||
                (!levelinfo.has_easymode && levelstat.finished));
    return false;
}


void Level::mark_solved () 
{
    // set state to "solved"
    LevelStatus      levelstat;

    // get current status (if available)
    get_status (levelstat);

    if (levelstat.finished != 3) {
        levelstat.finished = 3; // both easy and difficult
        levelstat.solved_revision = get_info().revision;
        set_status (levelstat);
    }
}

void Level::mark_unsolved () 
{
    LevelStatus      levelstat;
    if (get_status (levelstat) && levelstat.finished) {
        levelstat.finished  = 0;
        set_status (levelstat);
    }
}

/* Returns level index 0 if no suitable ``next level'' could be found. */
Level Level::next_level (int difficulty,
                         bool skip_solved,
                         bool select_non_par) // always takes non-par levels (overides skip_solved)
{
    LevelPack *lp = get_levelpack();
    Level nextl (*this);

    size_t size  = lp->size();
    size_t max_available = HighestAvailableLevel(lp);

    bool found = false;
    while (nextl.m_index<size+1 && !found) {
        ++nextl.m_index;
        bool available = nextl.m_index <= max_available;
        if (!available)
            continue;

        if (skip_solved || select_non_par) {
            bool solved = nextl.is_solved (difficulty);
            if (!solved) // always play unsolved levels
                found = true;
            else { // solved levels
                if (select_non_par) {
                    int  par_time       = nextl.get_par_time (difficulty);
                    int  best_user_time = nextl.get_best_user_time (difficulty);
                    bool need_par       = best_user_time<0 || (par_time>0 && best_user_time>par_time);

                    if (need_par)
                        found = true;
                }
                else if (!skip_solved)
                    found = true;
            }
        }
        else
            found = true;
    }
    if (!found)
        nextl.m_index = 0;
    return nextl;
}


/* -------------------- Global variables -------------------- */
std::vector<LevelPack *>  levels::LevelPacks;


/* -------------------- Functions -------------------- */

bool levels::advance_level (Level &l, LevelAdvanceMode m)
{
    bool skip_solved  = options::GetBool("SkipSolvedLevels");
    bool take_non_par = skip_solved == true && options::GetBool("TimeHunting");

    switch (m) {
    case advance_normal:
        break;
    case advance_strictly_next:
        skip_solved = false;
        take_non_par = false;
        break;
    case advance_unsolved:
        skip_solved = true;
        break;
    };

    Level nextl  = l.next_level (options::GetDifficulty(),
                                 skip_solved, take_non_par);

    if (nextl.get_index()) {
        l = nextl; 
        return true;
    }
    return false;
}


unsigned
levels::CountSolvedLevels (LevelPack *lp, int difficulty, unsigned *par_counter)
{
    unsigned cnt                  = 0;
    if (par_counter) *par_counter = 0;

    for (size_t i=0; i<lp->size(); ++i) {
        Level            level (lp, i);
        LevelStatus      levelstat;

        if (level.get_status(levelstat)) {
            if (levelstat.finished >= difficulty) {
                ++cnt;
            }
            else if (levelstat.finished == 1 && !level.get_info().has_easymode) {
                // level has been finished in easy-mode, but has no easymode
                // (due to an old&fixed enigma-bug user time was saved as easy user time)
                ++cnt;
            }

            if (par_counter) {
                int user_time = level.get_best_user_time(difficulty);
                int par_time  = level.get_par_time(difficulty);

                if (user_time != -1 && 
                    (user_time <= par_time || par_time == -1)) {
                    // user solved the level after time recording was implemented into enigma
                    // and reached par. Levels w/o par count as par as well.
                    ++(*par_counter);
                }
            }
        }
    }
    return cnt;
}

unsigned
levels::HighestAvailableLevel(LevelPack *lp)
{
    if (enigma::WizardMode || // No restriction in WizardMode
        lp->get_name() == "TestLevels") // and test-levelpack
        return lp->size()-1;

    return lp->size()-1;        // ####


    // Otherwise the player can choose out of 10 unsolved levels
    unsigned max_available = 10-1; // level numbers start at 0

    LevelStatus levelstat;

    for (size_t i = 0; i < lp->size() && i <= max_available; i++) {
        Level level (lp, i);
        if (level.get_status (levelstat) && levelstat.finished != 0)
            max_available++;  // for each solved level, an additional level is available;
    }
    return max_available;
}

bool levels::LevelIsLocked (const Level &level)
{
    if (!options::GetBool("Tournament"))
        return false;               // lock levels only in tournament mode

    unsigned max_available = HighestAvailableLevel (level.get_levelpack());
    int finished = 0;

    LevelStatus levelstat;
    if (level.get_status(levelstat))
        finished = levelstat.finished;

    return !(level.get_index() <= max_available || finished >= options::GetDifficulty());
}


/* -------------------- Levelpack management -------------------- */


LevelPack *levels::FindLevelPack (const std::string &name)
{
    std::vector<LevelPack*>::iterator 
        i = LevelPacks.begin(),
        end = LevelPacks.end();

    for (; i != end; ++i) {
        LevelPack *lp = *i;
        if (lp->get_name() == name)
            return lp;
    }
    return 0;
}

bool levels::FindLevel(const std::string &levelname, Level &level) {
    std::vector<LevelPack*>::iterator 
        i = LevelPacks.begin(),
        end = LevelPacks.end();

    for (; i != end; ++i) {
        LevelPack *lp     = *i;
        int        nlevels = lp->size();
        for (int j = 0; j<nlevels; ++j) {
            const LevelInfo& info = lp->get_info(j);
            if (info.filename == levelname) {
                level = Level (lp,j);
                return true;
            }
        }
    }
    return false;
}


int levels::IndexOfLevelPack(const LevelPack *lp) {
    return std::distance(LevelPacks.begin(),
                         std::find(LevelPacks.begin(), LevelPacks.end(), lp));
}

void levels::RegisterLevelPack (LevelPack *lp)
{
    LevelPacks.push_back(lp);
}


