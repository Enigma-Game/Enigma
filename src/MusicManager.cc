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
#include "errors.hh"
#include "options.hh"
#include "MusicManager.hh"
#include "SoundEngine.hh"
#include "main.hh"
#include "lev/RatingManager.hh"
#include "lev/Index.hh"

#include "SDL.h"
#include "SDL_mixer.h"

#include <string>
#include <cassert>

using namespace enigma;
using namespace sound;

/* -------------------- Interface Functions -------------------- */

void sound::StartMenuMusic()
{
    MusicManager::instance()->setMusicContext(MUSIC_MENU);
}

void sound::StartLevelLoadMusic()
{
    MusicManager::instance()->setMusicContext(MUSIC_LEVEL_LOADING);
}

void sound::StartLevelMusic()
{
    MusicManager::instance()->setMusicContext(MUSIC_GAME);
}

void sound::SetInGameMusicActive(bool active)
{
    MusicManager::instance()->setInGameMusicActive(active);
}

void sound::MusicTick(double dtime)
{
    if(!sound::IsMusicMute())
        MusicManager::instance()->tick(dtime);
}

void sound::InitMusic()
{
    MusicManager::instance()->init();
}

void sound::DefineMusicSingle(std::string title, std::string filename,
        float affinity_intelligence, float affinity_dexterity, float affinity_patience,
        float affinity_knowledge, float affinity_speed)
{
    if(filename == "") {
        Log << "Warning: Tried to define music single '" << title
            << "' without file name. Skipped.\n";
        return;
    }
    MusicManager::instance()->defineMusicSingle(title, filename,
        affinity_intelligence, affinity_dexterity, affinity_patience,
        affinity_knowledge, affinity_speed);
}

/* -------------------- Sound option helpers -------------------- */

/*! These functions are used in OptionsMenu.cc for the MenuMusicButton. */

int sound::GetOptionMenuMusicCount()
{        
    return MusicManager::instance()->getMenuMusicQueueCount();
}

int sound::GetOptionMenuMusic()
{
    std::string music_queue = app.state->getString("MenuMusicQueue");
    int pos = MusicManager::instance()->getMusicQueueButtonPosition(music_queue);
    assert(pos >= 0);
    return pos;
}

void sound::SetOptionMenuMusic(int value)
{
    std::string music_queue = MusicManager::instance()->getMusicQueueByPosition(value);
    app.state->setProperty("MenuMusicQueue", music_queue);
    MusicManager::instance()->setMenuMusicQueue(music_queue);
}

std::string sound::GetOptionMenuMusicText(int value)
{
    return MusicManager::instance()->getMusicQueueByPosition(value);
}

/* -------------------- MusicManager -------------------- */

MusicManager *MusicManager::theSingleton = 0;
    
MusicManager* MusicManager::instance() {
    if (theSingleton == 0) {
        theSingleton = new MusicManager();
    }
    return theSingleton;
}

MusicManager::MusicManager()
: music_singles(), music_queues(), active_music_queue_title(""),
  ingame_music_queue_title(""), menu_music_queue_title(""),
  wait_length(-1), music_context(MUSIC_NONE), is_waiting(false),
  ingame_music_active(false)
{}

void MusicManager::tick(double dtime)
{
    static double cumulated_dtime = 0;
    if(!sound::IsMusicPlaying()) {
        if(is_waiting && (dtime > 0.0)) {
            // No music is playing, but we are actively counting seconds:
            // This is a silent phase of determined length ("waiting").
            cumulated_dtime += dtime;
            if(cumulated_dtime > wait_length) {
                cumulated_dtime = 0;
                stopWaiting();
                music_queues[active_music_queue_title].onWaitEnded();
            }        
        } else {
            // Music has really ended or not even begun.
            switch(getMusicContext()) {
                case MUSIC_NONE:
                    break;
                case MUSIC_GAME:
                    if(ingame_music_active)
                        music_queues[active_music_queue_title].onMusicEnded(false);
                    break;
                case MUSIC_MENU:
                    if(active_music_queue_title != "")
                        music_queues[active_music_queue_title].onMusicEnded(false);
                    break;
                case MUSIC_LEVEL_LOADING:
                    // Fadeout ended while level is still loading.
                    // Just wait until load is complete.
                    break;
                default:
                    Log << "Error: getMusicContext() returns an invalid type. Will ignore this.\n";
            }
        }
    }
}

void MusicManager::setMusicContext(MusicContext new_context)
{
    if(new_context == music_context)
        return;  // Nothing to do.

    switch(new_context) {
        case MUSIC_MENU:
            //sound::FadeoutMusic();
            //stopWaiting();
            //if(active_music_queue_title != "") {
            //    // onMusicEnded(true) means: force music, no waiting!
            //    music_queues[active_music_queue_title].onMusicEnded(true);
            //}
            Log << "Switching to menu music.\n";
            music_context = new_context;
            setActiveMusicQueue(getMenuMusicQueueTitle());
            break;
        case MUSIC_LEVEL_LOADING:
            Log << "Switching to level load music.\n";
            if(music_context == MUSIC_GAME) {
                // Switching from one level to another.
                music_context = new_context;
            } else {
                // Switching from menu to level, stop menu music.
                music_queues[active_music_queue_title].calculate_level_points();
                music_context = new_context;                
                setActiveMusicQueue(getInGameMusicQueueTitle());
            }
            break;
        case MUSIC_GAME:
            Log << "Switching to level music.\n";
            // TODO: Stop music if current music is not suitable.
            music_queues[active_music_queue_title].calculate_level_points();
            music_context = new_context;
            //sound::FadeoutMusic();
            setActiveMusicQueue(getInGameMusicQueueTitle());
            // We do not force music here, but leave it to "tick" to handle.
            break;
        case MUSIC_NONE:
            Log << "Switching to no music.\n";
            sound::FadeoutMusic(false);
            break;
    }    
}

void MusicManager::init()
{
    // TODO: This is only temporary. Information will come 
    // from an xml file later.
    
    // Menu Music
    music_singles["Esprit"] =
        MusicSingle("Esprit", "music/menu/esprit.ogg");
    music_singles["Pentagonal Dreams"] =
        MusicSingle("Pentagonal Dreams", "music/menu/pentagonal_dreams.s3m");
    
    // Level Music
    music_singles["Across The Ice"] =
        MusicSingle("Across The Ice", "music/game/across_the_ice.ogg", 0, 0.5, 0, 0, 0.5);
    music_singles["In Space"] =
        MusicSingle("In Space", "music/game/in_space.ogg", 0.3, 0, 0.4, 0, -0.3);
    music_singles["Meditation"] =
        MusicSingle("Meditation", "music/game/meditation.ogg", 0.4, 0.2, 0.4, 0, 0);
    music_singles["On The Water"] =
        MusicSingle("On The Water", "music/game/on_the_water.ogg", 0.2, 0.2, 0.2, 0.2, 0.4);
    music_singles["Puzzle"] =
        MusicSingle("Puzzle", "music/game/puzzle.ogg", 0.4, 0, 0.3, 0, 0.3);
    music_singles["Skull Stones"] =
        MusicSingle("Skull Stones", "music/game/skull_stones.ogg", 0, 0.5, -0.5, 0, 0);
    music_singles["Swamp"] =
        MusicSingle("Swamp", "music/game/swamp.ogg", 0.4, 0.1, -0.3, 0, 0.2);

    // Menu and Level Music
    music_singles["Enigma Rag"] =
        MusicSingle("Enigma Rag", "music/menu/enigma_rag.ogg", 0.4, -0.2, 0, 0, 0.4);

    // Menu Music Queues
    music_queues["Default"] = MusicQueue("Default", MUSICQUEUE_NEXT, 0);
    music_queues["Default"].appendSingle("Esprit", false);
    music_queues["Default"].appendSingle("Esprit", false);
    music_queues["Default"].appendSingleThenWait("Esprit", true, 8.0);
    music_queues["Default"].appendSingleThenWait("Enigma Rag", true, 8.0);
    music_queues["Default"].appendSingleThenWait("Pentagonal Dreams", true, 8.0);

    music_queues["Esprit"] = MusicQueue("Esprit", MUSICQUEUE_NEXT, 1);
    music_queues["Esprit"].appendSingle("Esprit", false);

    music_queues["Enigma Rag"] = MusicQueue("Enigma Rag", MUSICQUEUE_NEXT, 2);
    music_queues["Enigma Rag"].appendSingleThenWait("Enigma Rag", false, 8.0);

    music_queues["Pentagonal Dreams"] = MusicQueue("Pentagonal Dreams", MUSICQUEUE_NEXT, 3);
    music_queues["Pentagonal Dreams"].appendSingle("Pentagonal Dreams", true);

    active_music_queue_title = app.state->getString("MenuMusicQueue");
    // Set the default menu music queue, if saved queue doesn't exist.
    if(music_queues.find(active_music_queue_title) == music_queues.end())
    {
        Log << "Warning: Did not find specified menu music queue '"
            << active_music_queue_title << "', will switch to default.\n";
        active_music_queue_title = "Default";
        app.state->setProperty("MenuMusicQueue", active_music_queue_title);
    }

    // Level Music Queue
    music_queues["In Game"] = MusicQueue("In Game", MUSICQUEUE_LEVEL, 4);
    music_queues["In Game"].appendSingleThenWait("Across The Ice", true, 3.0);
    music_queues["In Game"].appendSingleThenWait("In Space", true, 3.0);
    music_queues["In Game"].appendSingleThenWait("Meditation", true, 3.0);
    music_queues["In Game"].appendSingleThenWait("On The Water", true, 3.0);
    music_queues["In Game"].appendSingleThenWait("Puzzle", true, 3.0);
    music_queues["In Game"].appendSingleThenWait("Skull Stones", true, 3.0);
    music_queues["In Game"].appendSingleThenWait("Swamp", true, 3.0);
    music_queues["In Game"].appendSingleThenWait("Enigma Rag", true, 3.0);
    
    menu_music_queue_title = app.state->getString("MenuMusicQueue");
    // Set the default menu music queue, if saved queue doesn't exist.
    if(music_queues.find(menu_music_queue_title) == music_queues.end())
    {
        Log << "Warning: Did not find specified menu music queue '"
            << menu_music_queue_title << "', will switch to default.\n";
        menu_music_queue_title = "Default";
        app.state->setProperty("MenuMusicQueue", menu_music_queue_title);
    }
    ingame_music_active = app.prefs->getBool("InGameMusic");
    ingame_music_queue_title = "In Game";   
    
    // setMusicContext will set active_music_queue_title as well;
    // after this, tick will start the music.
    setMusicContext(MUSIC_MENU);
    tick(-1);
}

bool MusicManager::defineMusicSingle(std::string title, std::string filename,
            float affinity_intelligence, float affinity_dexterity, float affinity_patience,
            float affinity_knowledge, float affinity_speed)
{
    music_singles[title] = MusicSingle(title, filename, affinity_intelligence,
        affinity_dexterity, affinity_patience, affinity_knowledge, affinity_speed);
    Log << "Added music single '" << title << "'.\n";
    return true;
}

bool MusicManager::playMusicSingle(std::string title)
{
    if(music_singles.find(title) == music_singles.end()) {
        // Single doesn't exist! Returning "false" will make the queue
        // mark its entry as "no_music", so this single will be ignored.
        return false;
    }
    return music_singles[title].start();
}

void MusicManager::setWaiting(float seconds)
{
    is_waiting = true;
    wait_length = seconds;
}

void MusicManager::stopWaiting()
{
    is_waiting = false;
    wait_length = -1;
}

bool MusicManager::setActiveMusicQueue(std::string music_queue_title)
{
    if (music_queue_title == "") {
        Log << "Warning: Tried to choose empty music queue title as active music queue.\n";
        return false;
    }
    if (music_queue_title == getActiveMusicQueueTitle()) {
        // Current queue is aready running.
        return true;
    }
    // Stop current music and leave this queue.
    sound::FadeoutMusic();
    if (active_music_queue_title != "")
        music_queues[active_music_queue_title].leave();
    // Switch to new queue if possible.
    stopWaiting();
    active_music_queue_title = music_queue_title;
    Log << "Switched to music queue '" << music_queue_title << "'.\n";
    return true;
    // We leave it to tick to start the new queue.
}

bool MusicManager::setMenuMusicQueue(std::string music_queue_title)
{
    if (music_queue_title == "") {
        Log << "Warning: Tried to choose empty music queue title as menu music queue.\n";
        return false;
    }
    if (menu_music_queue_title == music_queue_title)
        return true;
    menu_music_queue_title = music_queue_title;
    if(getMusicContext() == MUSIC_MENU)
        setActiveMusicQueue(music_queue_title);
}

bool MusicManager::setInGameMusicQueue(std::string music_queue_title)
{
    if (music_queue_title == "") {
        Log << "Warning: Tried to choose empty music queue title as in-game music queue.\n";
        return false;
    }
    if (ingame_music_queue_title == music_queue_title)
        return true;
    ingame_music_queue_title = music_queue_title;
    if(getMusicContext() == MUSIC_GAME)
        setActiveMusicQueue(music_queue_title);
}

std::string MusicManager::getMusicQueueByPosition(int button_position)
{
    for (MusicQueueRepository::iterator i = music_queues.begin();
             i != music_queues.end(); ++i)
        if((*i).second.getButtonPosition() == button_position)
            return (*i).first;
    return "";
}

int MusicManager::getMenuMusicQueueCount()
{
    int count = 0;
    for (MusicQueueRepository::iterator i = music_queues.begin();
             i != music_queues.end(); ++i)
        if((*i).second.getButtonPosition() != -1)
            count++;
    return count;
}

std::string MusicManager::getCurrentMusicTitle() {
    if(sound::IsMusicPlaying() && (active_music_queue_title != ""))
        return music_queues[active_music_queue_title].getCurrentMusicTitle();
    else
        return "";
}

void MusicManager::setInGameMusicActive(bool active)
{
    ingame_music_active = active;
    if ((getMusicContext() == MUSIC_GAME) && (!ingame_music_active))
    {
        sound::FadeoutMusic();
        if (active_music_queue_title != "")
            music_queues[active_music_queue_title].leave();
    }
}

MusicSingle MusicManager::getMusicSingle(std::string title)
{
    if(music_singles.find(title) != music_singles.end())
        return music_singles[title];
    else
    {
        Log << "Warning: Did not find music single " << title << ".\n";
        return MusicSingle();
    }
}

/* -------------------- Music Single -------------------- */

bool MusicSingle::start()
{
    if(title == MusicManager::instance()->getCurrentMusicTitle())
        return true;
    if(sound::PlayMusic(filename))
        return true;
    return false;
}

float MusicSingle::affinity(float lev_int, float lev_dex, float lev_pat,
                            float lev_kno, float lev_spe)
{
    return   (affinity_intelligence * lev_int)
           + (affinity_dexterity * lev_dex)
           + (affinity_patience * lev_pat)
           + (affinity_knowledge * lev_kno)
           + (affinity_speed * lev_spe);
}


/* -------------------- Music Queue -------------------- */

std::string MusicQueue::getCurrentMusicTitle()
{
    if(current_position_in_queue == -1)
        return "";
    else
        return queue_entry[current_position_in_queue].title;
}

void MusicQueue::appendSingle(std::string title, bool fadeout_on_end)
{
    MusicQueueEntry new_entry;
    new_entry.type = MUSICQUEUE_SINGLE;
    new_entry.title = title;
    new_entry.fadeout_on_end = fadeout_on_end;
    // TODO: fadeout_on_end currently doesn't work, as the music
    //       ends before it is noticed that the next entry of the
    //       queue should be read.
    new_entry.wait_length = -1;
    new_entry.no_music = false;
    new_entry.points_transient = 0;
    new_entry.points_level = 0;
    new_entry.points_total = 0;
    queue_entry.push_back(new_entry);
}

void MusicQueue::appendSingleThenWait(std::string title, bool fadeout_on_end, float seconds)
{
    appendSingle(title, fadeout_on_end);
    (queue_entry.back()).wait_length = seconds;
}

void MusicQueue::appendWait(float seconds)
{
    MusicQueueEntry new_entry;
    new_entry.type = MUSICQUEUE_WAIT;
    new_entry.title = "";
    new_entry.fadeout_on_end = false;
    new_entry.wait_length = seconds;
    new_entry.no_music = true;
    new_entry.points_transient = 0;
    new_entry.points_level = 0;
    new_entry.points_total = 0;
    queue_entry.push_back(new_entry);
}

bool MusicQueue::start()
{
    current_position_in_queue = -1;
    return next();
    // We explicitly allow the queue to start silent.
    // Otherwise use next(true), i.e. force_music.
    // "next()" will also take care of empty queues.
}

void MusicQueue::calculate_level_points()
{
    lev::RatingManager *theRatingMgr = lev::RatingManager::instance();
    lev::Proxy *currentLevel = (lev::Index::getCurrentIndex())->getCurrent();
    MusicManager *theMusicMgr = MusicManager::instance();
    float currentInt = theRatingMgr->getIntelligence(currentLevel);
    float currentDex = theRatingMgr->getDexterity(currentLevel);
    float currentPat = theRatingMgr->getPatience(currentLevel);
    float currentKno = theRatingMgr->getKnowledge(currentLevel);
    float currentSpe = theRatingMgr->getSpeed(currentLevel);
    // If the requested level does not exist, theRatingMgr will
    // return 0 instead. We shift ratings such that 2.8 is seen as 0.
    // Special handling for knowledge == 6 (set to 0).
    //Log << currentInt << "/" << currentDex << "/" << currentPat << "/" <<
    // currentKno << "/" << currentSpe << "\n";
    currentInt = (currentInt == 0) ? 0 : (currentInt - 2.8);
    currentDex = (currentDex == 0) ? 0 : (currentDex - 2.8);
    currentPat = (currentPat == 0) ? 0 : (currentPat - 2.8);
    currentKno = ((currentKno == 0) || (currentKno == 6)) ? 0 : (currentKno - 2.8);
    currentSpe = (currentSpe == 0) ? 0 : (currentSpe - 2.8);
    //Log << currentInt << "/" << currentDex << "/" << currentPat << "/" <<
    // currentKno << "/" << currentSpe << "\n";
    for(int j = 0; j < queue_entry.size(); j++)
        if(queue_entry[j].type == MUSICQUEUE_SINGLE)
        {
            std::string title = queue_entry[j].title;
            queue_entry[j].points_level =
                (theMusicMgr->getMusicSingle(title)).affinity(
                currentInt, currentDex, currentPat, currentKno, currentSpe);
            //Log << title << ": " << queue_entry[j].points_level << "\n";
        }
}

bool MusicQueue::next(bool force_music)
{
    if(defunc)
        return false;

    if(queue_entry.size() <= 0) {
        Log << "Music queue is empty. No music will be played.\n";
        return false;
    }

    if(current_position_in_queue != -1) {
        // Another queue entry ended before this. Maybe we have to fade out.
        MusicQueueEntry old_entry = queue_entry[current_position_in_queue];
        if(old_entry.fadeout_on_end && sound::IsMusicPlaying())
            sound::FadeoutMusic();
    }

    int new_position = (current_position_in_queue >= 0) ? current_position_in_queue : 0;
    switch(shuffle_type) {
        case MUSICQUEUE_NEXT:
            // Jump to next position in queue.
            new_position = (current_position_in_queue + 1) % queue_entry.size();
            break;
        case MUSICQUEUE_RANDOM:
            // Jump to a random position in queue other than the current.
            if(queue_entry.size() < 2)
                break; // Don't change current_position.
            while (new_position == current_position_in_queue)
                new_position = IntegerRand(0, queue_entry.size() - 1, false);
            break;
        case MUSICQUEUE_LEVEL:
            // Calculate the optimal single (or maybe silence phase).
            if(current_position_in_queue >= 0)
                queue_entry[current_position_in_queue].points_transient += -10;
            for(int j = 0; j < queue_entry.size(); j++)
            {
                queue_entry[j].points_transient = queue_entry[j].points_transient / 2.0;
                queue_entry[j].points_total = queue_entry[j].points_transient
                    + queue_entry[j].points_level + IntegerRand(0, 300, false) / 1000.0;
                Log << j << ": " << queue_entry[j].title << ": " << queue_entry[j].points_level
                    << " + " << queue_entry[j].points_transient << " -> " <<
                    queue_entry[j].points_total << "\n";
            }
            float best_value = queue_entry[new_position].points_total;
            for(int j = 0; j < queue_entry.size(); j++)
                if (queue_entry[j].points_total >= best_value)
                {
                    best_value = queue_entry[j].points_total;
                    new_position = j;
                }
            Log << "Chose " << queue_entry[new_position].title << "\n";
            break;
    }
    current_position_in_queue = new_position;
        
    MusicQueueEntry current_entry = queue_entry[current_position_in_queue];
    bool success = false;
    switch(current_entry.type) {
        case MUSICQUEUE_SINGLE:
            Log << "Play next in queue " << title << ": " << current_entry.title << ".\n";
            success = MusicManager::instance()->playMusicSingle(current_entry.title);
            queue_entry[current_position_in_queue].no_music = !success;
            if(!success)
                assureQueueHasMusic();
            return success;
            break;
        case MUSICQUEUE_WAIT:
            if(force_music)
                return false;  // this will cause "tick" to call "next" again.
            Log << "Music queue starts waiting (" << current_entry.wait_length << "s).\n";
            MusicManager::instance()->setWaiting(current_entry.wait_length);
            return true;
            break;
        default:
            Log << "Error: Current music queue entry is of invalid type. Will ignore this.\n";
            return false;
    }
}

bool MusicQueue::onMusicEnded(bool force_music)
{
    if((current_position_in_queue >= 0) && (current_position_in_queue < queue_entry.size())) {
        MusicQueueEntry current_entry = queue_entry[current_position_in_queue];
        if((current_entry.wait_length > 0) && (!current_entry.no_music) && (!force_music))
        {
            Log << "Music queue starts conditional waiting (" << current_entry.wait_length << "s).\n";
            MusicManager::instance()->setWaiting(current_entry.wait_length);
            return true;
        } else
            return next(force_music);
    }
    return start();
}

bool MusicQueue::onWaitEnded()
{
    return next();
}

void MusicQueue::leave()
{
    /*! We have the following choices to determine where to start the queue
      next time. Remember that "next" will be called and auto-increase
      current_position_in_queue.
      Complete reset: current_position_in_queue = -1;
      Start with n-th song: current_position_in_queue = n - 2;
      Restart the current: decrease current_position_in_queue by one.
      Start the next position in queue: don't change anything.
      However, what we want is: Jump to the next real music, skip all kinds of
      silent breaks. Cycle through until the end of the queue; if nothing is
      found, always start the queue from the beginning. */
    int total = queue_entry.size();
    while(   (queue_entry[(current_position_in_queue + 1) % total].type != MUSICQUEUE_SINGLE)
          && (current_position_in_queue < total)) {
        current_position_in_queue++;
    }
    if(current_position_in_queue >= total)
        current_position_in_queue = -1;
}

void MusicQueue::assureQueueHasMusic()
{
    /*! This is called when a queue might have no music at all,
      e.g. because all of its referenced files don't exist. Each time a
      file is missing, its entry is changed to "no_music = true". Pure
      waiting phases have "no_music = true" just as well. If all entries
      are "no_music = true", then the queue is set to "defunc = true". */
    bool queue_has_no_music = true;
    for(std::vector<MusicQueueEntry>::const_iterator iter = queue_entry.begin();
            iter != queue_entry.end(); ++iter)
        queue_has_no_music = queue_has_no_music && (*iter).no_music;
    if(queue_has_no_music) {
        defunc = true;
        Log << "Warning: Queue " << title << " has no playable music! (Files missing?) Deactivated it.\n";
    }
}

