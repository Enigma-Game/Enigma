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
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3/SDL_mutex.h>

#include <string>
#include <vector>

namespace sound
{

/* -------------------- Interface Functions -------------------- */

    void Init(bool withMusic =true, bool withSound =true);
    void Shutdown();

    void Tick(double dtime);

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
        bool init() override { return true; }
        void shutdown() override {}
        bool is_initialized() const override { return true; }
        void set_sound_volume(double /*soundvol*/) override {}
        void set_music_volume(double /*musicvol*/) override {}
        bool play_music (const std::string &/*filename*/, double /*position*/) override { return false; }
        void stop_music() override {}
        void fadeout_music(bool blocking =true) override {}
        bool is_music_playing() override { return false; }
        void clear_cache() override {}
        void define_sound (const SoundName &, const SoundData &) override {}
        bool play_sound (const SoundEvent &) override { return false;}
        void cache_sound(const SoundEffect &s) override {}
        void set_listenerpos (ecl::V2 pos) override {}
        void tick(double /*dtime*/) override {}
    };

    class SoundEngine_SDL : public SoundEngine {
    public:
        SoundEngine_SDL();
        ~SoundEngine_SDL();

        // ---------- SoundEngine interface ----------
        bool init() override;        
        void shutdown() override;
        bool is_initialized() const override { return m_initialized; }
        void set_sound_volume(double soundvol) override;
        void set_music_volume(double musicvol) override;
        bool play_music (const std::string &filename, double position) override;
        void stop_music() override;
        void fadeout_music(bool blocking =true) override;
        bool is_music_playing() override;
        void clear_cache() override;
        void define_sound (const SoundName &, const SoundData &) override;
        bool play_sound(const SoundEvent &s) override;
        void cache_sound(const SoundEffect &s) override;
        void set_listenerpos (ecl::V2 pos) override { m_listenerpos = pos; }
        void tick (double dtime) override;

        /*! These functions convert raw audio data with a specified format to
          the mixer's audio format.  This is used for converting the original
          Oxyd sounds to a format usable by Enigma. */

        Sint8* resample (const Sint8 *data, Uint32 len, int oldfreq,
                         int newfreq, Uint32 *newlen_);
        MIX_Audio *ChunkFromRaw (const Uint8 *buf, Uint32 len,
                                 int freq, int format, int channels);

    private:
        // ---------- Private methods ----------
        MIX_Audio *cache_sound(const std::string &name);

        void update_channel (int channel);
        int already_playing (const SoundEvent &s);

        static void SDLCALL channel_finished (void *userdata, MIX_Track *track);

        // ---------- Variables ----------
        bool                    m_initialized;
        double                  m_soundvolume;
        double                  m_musicvolume;
        MIX_Mixer              *m_mixer;
        MIX_Audio              *m_current_music;
        MIX_Track              *m_music_track;
        int                     m_channels;
        std::vector<MIX_Track*> m_tracks;
        ecl::Dict<MIX_Audio*>   wav_cache;
        std::vector<SoundEvent> m_channelinfo;
        ecl::V2                 m_listenerpos;
        SDL_Mutex              *m_mutex;
        static SoundEngine_SDL *m_instance;
    };
}

#endif
