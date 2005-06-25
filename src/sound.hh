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
 * $Id: sound.hh,v 1.8 2004/05/22 13:04:29 dheck Exp $
 */
#ifndef SOUND_HH
#define SOUND_HH

#include "ecl_math.hh"

#include <string>
#include <vector>

namespace sound
{
/* -------------------- Data types -------------------- */

    typedef std::string SoundName;

    typedef std::vector <unsigned char> ByteVec;

    struct SoundData {
        ByteVec  buf;
        unsigned freq;
        size_t   samplesize;
        bool     signedp;
        int      nchannels;
    };

/* -------------------- Functions -------------------- */

    void Init();
    void Shutdown();

    void Tick (double dtime);

    void DisableSound();
    void EnableSound();
    void DisableMusic();

    void TempDisableSound();
    void TempReEnableSound();

    void SetListenerPosition (const ecl::V2 &pos);
    void PlaySound (const SoundName &, const ecl::V2 &pos, double relative_volume = 1.0, int priority=0);
    void PlaySoundGlobal (const SoundName &, double relative_volume = 1.0, int priority=0);

    void PlayMusic (const std::string &name);
    void FadeoutMusic();

    /*! Stop any music currently playing. */
    void StopMusic();

    /*! Stop music only if it has the specified name, otherwise
      continue playing. */
    void StopMusic (const std::string &name);

    /*! Trigger a sound event that is dispatched to the Lua function
      `SoundEvent'.  Return true if the event was handles, false
      otherwise. */
    bool SoundEvent (const std::string &eventname,
                     const ecl::V2 &pos = ecl::V2 (), 
                     double volume = 1.0);

    void ClearCache();
    void DefineSound (const SoundName &, const SoundData &);
    void SetSoundVolume (double vol);
    void SetMusicVolume (double vol);

}

#endif
