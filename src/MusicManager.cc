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
    // TODO!
    if (active)
        sound::PlayMusic(options::GetString("LevelMusicFile"));
    else
        sound::StopMusic();
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

void sound::DefineMusicSingle(std::string title, std::string filename) {
    if(filename == "") {
        Log << "Warning: Tried to define music single '" << title
            << "' without file name. Skipped.\n";
        return;
    }
    MusicManager::instance()->defineMusicSingle(title, filename);
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
    MusicManager::instance()->setActiveMusicQueue(music_queue);
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
  wait_length(-1), music_context(MUSIC_NONE), is_waiting(false)
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
            switch(MusicManager::instance()->getMusicContext()) {
                case MUSIC_NONE:
                    break;
                case MUSIC_MENU:
                    if(active_music_queue_title != "")
                        music_queues[active_music_queue_title].onMusicEnded(false);
                    break;
                case MUSIC_LEVEL_LOADING:
                    // Fadeout ended while level is still loading.
                    // Just wait until load is complete.
                    break;
                case MUSIC_GAME:
                    // TODO
                    //if (options::GetBool("InGameMusic")) {
                    //}
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
            sound::FadeoutMusic();
            stopWaiting();
            if(active_music_queue_title != "") {
                // onMusicEnded(true) means: force music, no waiting!
                music_queues[active_music_queue_title].onMusicEnded(true);
            }
            music_context = new_context;
            break;
        case MUSIC_LEVEL_LOADING:
            sound::FadeoutMusic(false);
            stopWaiting();
            music_context = new_context;
            break;
        case MUSIC_GAME:        
            sound::FadeoutMusic(true);
            stopWaiting();
            music_context = new_context;
            // We do not force music here, but leave it to "tick" to handle.
            break;
        case MUSIC_NONE:
            sound::FadeoutMusic(false);
            break;
    }    
}

void MusicManager::init()
{
    // TODO: This is only temporary. Information will come 
    // from an xml file later.
    music_singles["Esprit"] =
        MusicSingle("Esprit", "music/menu/esprit.ogg");
    music_singles["Enigma Rag"] =
        MusicSingle("Enigma Rag", "music/menu/enigma_rag.ogg");
    music_singles["Pentagonal Dreams"] =
        MusicSingle("Pentagonal Dreams", "music/menu/pentagonal_dreams.s3m");

    music_queues["Default"] = MusicQueue("Default", 0);
    music_queues["Default"].appendSingle("Esprit", false);
    music_queues["Default"].appendSingle("Esprit", false);
    music_queues["Default"].appendSingleThenWait("Esprit", true, 8.0);
    music_queues["Default"].appendSingleThenWait("Enigma Rag", true, 8.0);
    music_queues["Default"].appendSingleThenWait("Pentagonal Dreams", true, 8.0);

    music_queues["Esprit"] = MusicQueue("Esprit", 1);
    music_queues["Esprit"].appendSingle("Esprit", false);

    music_queues["Enigma Rag"] = MusicQueue("Enigma Rag", 2);
    music_queues["Enigma Rag"].appendSingleThenWait("Enigma Rag", false, 8.0);

    music_queues["Pentagonal Dreams"] = MusicQueue("Pentagonal Dreams", 3);
    music_queues["Pentagonal Dreams"].appendSingle("Pentagonal Dreams", true);

    app.state->setProperty("MenuMusicQueue", "Default");
    active_music_queue_title = "Default";
    setMusicContext(MUSIC_MENU);
    tick(-1);
}

bool MusicManager::defineMusicSingle(std::string title, std::string filename)
{
    music_singles[title] = MusicSingle(title, filename);
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
        Log << "Warning: Tried to choose empty music queue title as menu music queue.\n";
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
    if (music_queues[music_queue_title].next()) {
        active_music_queue_title = music_queue_title;
        Log << "Switched to menu music queue '" << music_queue_title << "'.\n";
        return true;
    } else {
        active_music_queue_title = "";
        Log << "Warning: Problems loading menu music queue '" << music_queue_title << "'.\n";
        return false;
    }
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


/* -------------------- MusicSingle -------------------- */

bool MusicSingle::start()
{
    if(title == MusicManager::instance()->getCurrentMusicTitle())
        return true;
    if(sound::PlayMusic(filename))
        return true;
    return false;
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

    // Jump to next position in queue.
    // TODO: Add random
    current_position_in_queue = (current_position_in_queue + 1) % queue_entry.size();

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

