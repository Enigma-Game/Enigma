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
#include "errors.hh"
#include "options.hh"
#include "MusicManager.hh"
#include "SoundEngine.hh"
#include "main.hh"

#include "SDL.h"
#include "SDL_mixer.h"

#include <string>
#include <cassert>

using namespace std;
using namespace enigma;
using namespace sound;


/* -------------------- Interface Functions -------------------- */

bool sound::StartMenuMusic()
{
    if(MusicManager::instance()->getMusicContext() != MUSIC_MENU) {
        sound::FadeoutMusic();
        MusicManager::instance()->setMusicContext(MUSIC_MENU);
    }
}

bool sound::StartLevelMusic()
{
    if(MusicManager::instance()->getMusicContext() != MUSIC_GAME) {
        sound::FadeoutMusic();
        MusicManager::instance()->setMusicContext(MUSIC_GAME);
    }
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
    MusicManager::instance()->tick(dtime);
}

void sound::InitMusic()
{
    MusicManager::instance()->init();
}

void sound::DefineMusicSingle(string title, string filename) {
    // TODO: include play_till, replay_from, volume etc
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
    string music_queue = app.state->getString("MenuMusicQueue");
    int pos = MusicManager::instance()->getMusicQueueButtonPosition(music_queue);
    assert(pos >= 0);
    return pos;
}

void sound::SetOptionMenuMusic(int value)
{
    string music_queue = MusicManager::instance()->getMusicQueueByPosition(value);
    app.state->setProperty("MenuMusicQueue", music_queue);
    MusicManager::instance()->setActiveMusicQueue(music_queue);
}

string sound::GetOptionMenuMusicText(int value)
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
: music_singles(), music_queues(), active_music_queue(""),
  music_context(MUSIC_NONE)
{}    

void MusicManager::tick(double dtime)
{
    static double cumulated_dtime = 0;
    cumulated_dtime += dtime;
    if((cumulated_dtime > 0.2) || (dtime < 0.0)) {
        if(!sound::IsMusicPlaying()) {
            // Music has ended or not even begun
            switch(MusicManager::instance()->getMusicContext()) {
                case MUSIC_MENU:
                    if(active_music_queue != "")
                        music_queues[active_music_queue].next();                
                    break;
                case MUSIC_GAME:
                    // TODO
                    //if (options::GetBool("InGameMusic")) {
                    //}
                    break;
            }
        } else {
            // Music is still running. Check if we should reloop.
            if(getCurrentMusicTitle() != "") {
                music_singles[getCurrentMusicTitle()].maybeLoopBack();
            }
        }
        cumulated_dtime = 0;
    }
}

void MusicManager::init()
{
    // TODO: This is only temporary. Information will come 
    // from an xml file later.
    music_singles["Esprit Loop"] =
        MusicSingle("Esprit Loop", "music/menu/esprit.ogg", 178180, 10690, 21600, true);
    music_singles["Esprit"] =
        MusicSingle("Esprit", "music/menu/esprit.ogg", 178180, 10690, 21600, false);
    music_singles["Pentagonal Dreams"] =
        MusicSingle("Pentagonal Dreams", "music/menu/menu.s3m");

    music_queues["Default"] = MusicQueue("Default", 0);
    music_queues["Default"].appendSingle("Esprit");
    music_queues["Default"].appendSingle("Pentagonal Dreams");

    music_queues["Esprit"] = MusicQueue("Esprit", 1);
    music_queues["Esprit"].appendSingle("Esprit");

    music_queues["Pentagonal Dreams"] = MusicQueue("Pentagonal Dreams", 2);
    music_queues["Pentagonal Dreams"].appendSingle("Pentagonal Dreams");

    music_queues["Loop test"] = MusicQueue("Loop test", 3);
    music_queues["Loop test"].appendSingle("Esprit Loop");

    app.state->setProperty("MenuMusicQueue", "Default");
    active_music_queue = "Default";
    setMusicContext(MUSIC_MENU);
    tick(-1);
}

bool MusicManager::defineMusicSingle(string title, string filename)
{
    music_singles[title] = MusicSingle(title, filename);
    Log << "Added music single '" << title << "'.\n";
    return true;
}

bool MusicManager::playMusicSingle(string title)
{
    return music_singles[title].start();
}

bool MusicManager::setActiveMusicQueue(string music_queue_title)
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
    if (active_music_queue != "")
        music_queues[active_music_queue].leave();
    // Switch to new queue if possible.
    if (music_queues[music_queue_title].next()) {
        active_music_queue = music_queue_title;
        Log << "Switched to menu music queue '" << music_queue_title << "'.\n";
        return true;
    } else {
        active_music_queue = "";
        Log << "Warning: Problems loading menu music queue '" << music_queue_title << "'.\n";
        return false;
    }
}

string MusicManager::getMusicQueueByPosition(int button_position)
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

string MusicManager::getCurrentMusicTitle() {
    if(sound::IsMusicPlaying() && (active_music_queue != ""))
        return music_queues[active_music_queue].getCurrentMusicTitle();
    else
        return "";
}


/* -------------------- MusicSingle -------------------- */

bool MusicSingle::start()
{
    if(title == MusicManager::instance()->getCurrentMusicTitle())
        return true;
    sound::FadeoutMusic();
    if(sound::PlayMusic(filename)) {
        start_time = SDL_GetTicks();
        return true;
    }
    return false;
}

bool MusicSingle::maybeLoopBack()
{
    Uint32 current_ticks = SDL_GetTicks();
    if((allows_loop) && (current_ticks >= start_time + loop_end)) {
        Uint32 position = current_ticks + loop_start - start_time - loop_end;
        if(loop_end > loop_start)
            while(position >= loop_end)
                position = position + loop_start - loop_end;
        start_time = current_ticks - position;
        sound::StopMusic();
        if(sound::PlayMusic(filename, position/1000.0)) {
            start_time = current_ticks - position;
            return true;
        } else
            return false;
    } else
        return false;
}

/* -------------------- Music Queue -------------------- */

string MusicQueue::getCurrentMusicTitle()
{
    if(current_position_in_queue == -1)
        return "";
    else
        return single_title[current_position_in_queue];
}

void MusicQueue::appendSingle(string title)
{
    single_title.push_back(title);
}

bool MusicQueue::start()
{
    if(single_title.size() > 0) {
        current_position_in_queue = 0;
        return MusicManager::instance()->playMusicSingle(single_title[0]);
    } else
        return false;
}

bool MusicQueue::next()
{
    if(current_position_in_queue == -1)
        // Queue did not start yet. Request first title instead.
        return start();
    else {
        // TODO: Add random
        current_position_in_queue++;
        if(current_position_in_queue >= single_title.size())
            current_position_in_queue = 0;
        string single = single_title[current_position_in_queue];
        Log << "Play next in queue " << title << ": " << single << ".\n";
        return MusicManager::instance()->playMusicSingle(single);
    }
}

void MusicQueue::leave()
{
    /*! We have the following choices, to determine where
      to start the queue next time. Remember that "next"
      will be called and auto-increase current_position_in_queue.
      Complete reset: current_position_in_queue = -1;
      Start with second song: current_position_in_queue = 0;
      Restart the current: decrease current_position_in_queue by one
      Start the next position in queue: don't change anything. */
}
