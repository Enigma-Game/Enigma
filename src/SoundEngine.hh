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
#ifndef SOUNDENGINE_HH_INCLUDED
#define SOUNDENGINE_HH_INCLUDED

#include "ecl_math.hh"
#include "ecl_dict.hh"

#include "SoundEffectManager.hh"
#include "MusicManager.hh"
#include "SDL_mixer.h"
#include "SDL_mutex.h"

#include <string>
#include <vector>

namespace sound
{

/* -------------------- Interface Functions -------------------- */

    void Init();
    void Shutdown();

    void Tick(double dtime);

    void DisableSound();
    void EnableSound();
    void DisableMusic();
    bool PlayMusic(const std::string &name, double position = 0.0);
    void FadeoutMusic(bool blocking =true);

    /*! Stop any music currently playing. */
    void StopMusic();
    bool IsMusicPlaying();

    void TempDisableSound();
    void TempReEnableSound();
    bool IsSoundMute();
    bool IsMusicMute();

    void SetListenerPosition (const ecl::V2 &pos);
    bool PlaySound(const SoundName &, const ecl::V2 &pos,
                    double relative_volume = 1.0, int priority=0);
    bool PlaySoundGlobal(const SoundName &, double relative_volume = 1.0, int priority=0);
    
    void ClearCache();
    void CacheSound(const SoundEffect &s);
    void SetSoundVolume(double vol);
    void SetMusicVolume(double vol);

    /*! Set the sound and music volume to the values in SoundVolume
      and MusicVolume. */
    void UpdateVolume();

/* -------------------- SoundEngine -------------------- */

    class SoundEngine {
    public:
        SoundEngine();
        virtual ~SoundEngine() {}
        
        //! Returns true if successful.
        virtual bool init() = 0;
        virtual void shutdown() = 0;
        virtual bool is_initialized() const = 0;

        virtual void set_sound_volume (double soundvol) = 0;
        virtual void set_music_volume (double musicvol) = 0;

        // ---------- Music ----------

        virtual bool play_music(const std::string &filename, double position) = 0;
        virtual void stop_music() = 0;
        virtual void fadeout_music(bool blocking =true) = 0;
        virtual bool is_music_playing() = 0;
        
        // ---------- Sound effects ----------

        virtual void clear_cache() = 0;
        virtual void define_sound(const SoundName &, const SoundData &) = 0;
        virtual bool play_sound(const SoundEvent &s) = 0;
        virtual void cache_sound(const SoundEffect &s) = 0;
        virtual void set_listenerpos(ecl::V2 pos) = 0;
        virtual void tick(double dtime) = 0;
    };

    class SoundEngine_Null : public SoundEngine {
    public:

        // SoundEngine interface
        bool init() { return true; }
        void shutdown() {}
        bool is_initialized() const { return true; }
        void set_sound_volume(double /*soundvol*/) {}
        void set_music_volume(double /*musicvol*/) {}
        bool play_music (const std::string &/*filename*/, double /*position*/) { return false; }
        void stop_music() {}
        void fadeout_music(bool blocking =true) {}
        bool is_music_playing() { return false; }
        void clear_cache() {}
        void define_sound (const SoundName &, const SoundData &) {}
        bool play_sound (const SoundEvent &) { return false;}
        void cache_sound(const SoundEffect &s) {}
        void set_listenerpos (ecl::V2 pos) {}
        void tick(double /*dtime*/) {}
    };

    class SoundEngine_SDL : public SoundEngine {
    public:
        SoundEngine_SDL();
        ~SoundEngine_SDL();

        // ---------- SoundEngine interface ----------
        bool init();        
        void shutdown();
        bool is_initialized() const { return m_initialized; }
        void set_sound_volume(double soundvol);
        void set_music_volume(double musicvol);
        bool play_music (const std::string &filename, double position);
        void stop_music();
        void fadeout_music(bool blocking =true);
        bool is_music_playing();
        void clear_cache();
        void define_sound (const SoundName &, const SoundData &);
        bool play_sound(const SoundEvent &s);
        void cache_sound(const SoundEffect &s);
        void set_listenerpos (ecl::V2 pos) { m_listenerpos = pos; }
        void tick (double dtime);

        /*! These functions convert raw audio data with a specified format to
          the mixer's audio format.  This is used for converting the original
          Oxyd sounds to a format usable by Enigma. */

        Sint8* resample (const Sint8 *data, Uint32 len, int oldfreq,
                         int newfreq, Uint32 *newlen_);
        Mix_Chunk *ChunkFromRaw (const Uint8 *buf, Uint32 len,
                                 int freq, int format, int channels);

    private:
        // ---------- Private methods ----------
        Mix_Chunk *cache_sound(const std::string &name);

        void update_channel (int channel);
        int already_playing (const SoundEvent &s);


        static void channel_finished (int channel);


        // ---------- Variables ----------
        bool       m_initialized;
        int        m_soundvolume;
        int        m_musicvolume;
        Mix_Music *m_current_music;
        int        m_freq;
        Uint16     m_format;
        int        m_channels;
        ecl::Dict<Mix_Chunk*> wav_cache;
        std::vector<SoundEvent> m_channelinfo;
        ecl::V2      m_listenerpos;
        SDL_mutex  *m_mutex;
        static SoundEngine_SDL *m_instance;
    };
}

#endif
