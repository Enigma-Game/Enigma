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
#include "SoundEngine.hh"
#include "main.hh"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3/SDL_mutex.h>

#include <string>
#include <cassert>

using namespace enigma;
using namespace sound;

/* -------------------- Local variables -------------------- */
namespace {

std::unique_ptr<SoundEngine> sound_engine;

bool sound_enabled = true;
bool music_enabled = true;
bool sound_enabled_temp = false;
bool sound_mute = false;
bool music_mute = false;

}  // namespace

/* -------------------- Interface Functions -------------------- */

void sound::Init(bool withMusic, bool withSound) 
{
    sound_enabled = withSound;
    music_enabled = withMusic;
    if (!sound_engine.get()) {
        sound_engine.reset(new SoundEngine_SDL);
    }

    if (sound_engine->init()) {
        sound::UpdateVolume();
    } else {
        sound_enabled = false;
        music_enabled = false;
        sound_engine.reset(new SoundEngine_Null);
    }
}

void sound::Shutdown() 
{
    if (sound_engine.get())
        sound_engine->shutdown();
}

void sound::Tick (double dtime)
{
    sound_engine->tick (dtime);
    sound::MusicTick(dtime);
}

void sound::TempDisableSound() {
    sound_enabled_temp = sound_enabled;
    sound_enabled      = false;
}

void sound::TempReEnableSound() {
    sound_enabled = sound_enabled_temp;
}

bool sound::IsSoundMute() {
    return !sound_enabled || sound_mute;
}

bool sound::IsMusicMute() {
    return !sound_enabled || !music_enabled || music_mute;
}

void sound::DefineSound (const SoundName &name, const SoundData &data)
{
    sound_engine->define_sound (name, data);
}

void sound::SetListenerPosition (const ecl::V2 &pos) 
{
    sound_engine->set_listenerpos (pos);
}

bool sound::PlaySound (const SoundName &name, const ecl::V2 &pos, double volume, int priority) 
{
    if (sound::IsSoundMute())
        return false;

    SoundEvent se;
    se.name         = name;
    se.has_position = true;
    se.position     = pos;
    se.priority     = priority;
    se.volume       = volume * options::GetDouble("SoundVolume");
    se.left = se.right = 0;

    return sound_engine->play_sound (se);
}

bool sound::PlaySoundGlobal (const SoundName &name, double volume, int priority) 
{
    if (sound::IsSoundMute())
        return false;

    SoundEvent se;
    se.name         = name;
    se.has_position = false;
    se.position     = ecl::V2();
    se.priority     = priority;
    se.volume       = volume * options::GetDouble("SoundVolume");
    se.left         = 255;
    se.right        = 255;

    return sound_engine->play_sound (se);
}

void sound::ClearCache()
{
    sound_engine->clear_cache();
}

void sound::CacheSound(const SoundEffect &s)
{
    sound_engine->cache_sound(s);
}

void sound::FadeoutMusic(bool blocking) 
{
    sound_engine->fadeout_music(blocking);
}

bool sound::PlayMusic (const std::string &name, double position) 
{
    if(sound::IsMusicMute() || name=="")
        return false;
    
    std::string fname;
    return app.resourceFS->findFile (name, fname) && sound_engine->play_music(fname, position);
}

void sound::StopMusic() {
    sound_engine->stop_music();
}

bool sound::IsMusicPlaying() {
    return sound_engine->is_music_playing();
}

void sound::SetSoundVolume (double vol)
{
    sound_engine->set_sound_volume (vol);
    sound_mute = (vol == 0.0);
}

void sound::SetMusicVolume (double vol)
{
    sound_engine->set_music_volume (vol);
    music_mute = (vol == 0.0);
}

void sound::UpdateVolume() 
{
    sound::SetSoundVolume(options::GetDouble("SoundVolume"));
    sound::SetMusicVolume(options::GetDouble("MusicVolume"));
}

/* -------------------- SoundEngine_SDL implementation -------------------- */

SoundEngine::SoundEngine()
{}

class MutexLock {
public:
    MutexLock (SDL_Mutex *m) {
        mutex = m;
        SDL_LockMutex (mutex);
    }
    ~MutexLock () {
        SDL_UnlockMutex (mutex);
    };
private:
    SDL_Mutex *mutex;
};

SoundEngine_SDL *SoundEngine_SDL::m_instance = 0;

SoundEngine_SDL::SoundEngine_SDL()
: m_initialized(false),
  m_soundvolume(1.0),
  m_musicvolume(1.0),
  m_mixer(nullptr),
  m_current_music(nullptr),
  m_music_track(nullptr),
  m_channels(32),
  m_tracks(),
  m_mutex(nullptr)
{
    assert (m_instance == 0);
    m_instance = this;
}

SoundEngine_SDL::~SoundEngine_SDL()
{
    shutdown();
    m_instance = 0;
}

bool SoundEngine_SDL::init() 
{
    if (!m_initialized) {
        // Initialize SDL audio subsystem
        if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
            fprintf(stderr, "Couldn't open SDL audio subsystem: %s\n", SDL_GetError());
            return false;
        }
        
        int vi = MIX_Version();
        Log << ecl::strf("SDL_mixer Version: %u.%u.%u\n",
                         SDL_VERSIONNUM_MAJOR(vi),
                         SDL_VERSIONNUM_MINOR(vi),
                         SDL_VERSIONNUM_MICRO(vi));

        if (!MIX_Init()) {
            Log << ecl::strf("Couldn't initialize SDL_mixer: %s\n", SDL_GetError());
            return false;
        }

        // Initialize SDL_mixer lib
        SDL_AudioSpec spec = {SDL_AUDIO_S16, 2, 44100};
        m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
        if (!m_mixer) { 
            fprintf(stderr, "Couldn't open mixer: %s\n", SDL_GetError());
            return false;
        }

        // Initialize channels/tracks
        m_channels = 32;
        m_channelinfo.resize(m_channels);
        m_tracks.resize(m_channels);

        for (int i = 0; i < m_channels; ++i) {
            m_tracks[i] = MIX_CreateTrack(m_mixer);
            MIX_SetTrackStoppedCallback(m_tracks[i], &channel_finished, (void*)(uintptr_t)i);
        }

        m_music_track = MIX_CreateTrack(m_mixer);

        m_mutex = SDL_CreateMutex();
        if (!m_mutex)
            return false;

        m_initialized = true;
    }
    return true;
}

void SoundEngine_SDL::shutdown() 
{
    if (m_initialized) {
        stop_music();
        for (int i = 0; i < m_channels; ++i) {
            if (m_tracks[i]) {
                MIX_DestroyTrack(m_tracks[i]);
                m_tracks[i] = nullptr;
            }
        }
        if (m_music_track) {
            MIX_DestroyTrack(m_music_track);
            m_music_track = nullptr;
        }
        clear_cache();
        if (m_mixer) {
            MIX_DestroyMixer(m_mixer);
            m_mixer = nullptr;
        }
        if (m_mutex) {
            SDL_DestroyMutex(m_mutex);
            m_mutex = nullptr;
        }
        MIX_Quit();
        m_initialized = false;
    }
}

void SoundEngine_SDL::clear_cache() 
{
    for (auto it = wav_cache.begin(); it != wav_cache.end(); ++it) {
        if (it->second)
            MIX_DestroyAudio(it->second);
    }
    wav_cache.clear();
}

void SoundEngine_SDL::set_sound_volume (double soundvol) 
{
    if (!m_initialized)
        return;

    m_soundvolume = ecl::Clamp(soundvol, 0.0, 1.0);
    if (m_mixer)
        MIX_SetMixerGain(m_mixer, static_cast<float>(m_soundvolume));
}

void SoundEngine_SDL::set_music_volume (double musicvol) 
{
    if (!m_initialized)
        return;

    m_musicvolume = ecl::Clamp(musicvol, 0.0, 1.0);
    if (m_music_track)
        MIX_SetTrackGain(m_music_track, static_cast<float>(m_musicvolume));
}

void SoundEngine_SDL::stop_music() 
{
    if (m_music_track)
        MIX_StopTrack(m_music_track, 0);
    if (m_current_music) {
        MIX_DestroyAudio(m_current_music);
        m_current_music = nullptr;
    }
}

bool SoundEngine_SDL::play_music (const std::string &filename, double position) 
{
    if (!m_initialized || !m_mixer || !m_music_track)
        return false;
    if (MIX_Audio *music = MIX_LoadAudio(m_mixer, filename.c_str(), false)) {
        if (m_current_music)
            MIX_DestroyAudio(m_current_music);
        m_current_music = music;
        MIX_SetTrackAudio(m_music_track, m_current_music);
        MIX_SetTrackLoops(m_music_track, -1);
        MIX_SetTrackGain(m_music_track, static_cast<float>(m_musicvolume));
        if (!MIX_PlayTrack(m_music_track, 0))
            Log << "MIX_PlayTrack: " << SDL_GetError() << "\n";
        if (position > 0) {
            Log << "Start music at position " << position << "\n";
            Sint64 frames = MIX_TrackMSToFrames(m_music_track, static_cast<Sint64>(position * 1000.0));
            if (!MIX_SetTrackPlaybackPosition(m_music_track, frames))
                Log << "MIX_SetTrackPlaybackPosition: " << SDL_GetError() << "\n";
        }
        return true;
    }
    return false;
}

void SoundEngine_SDL::fadeout_music(bool blocking)
{
    if (!m_music_track || !MIX_TrackPlaying(m_music_track))
        return;

    Sint64 frames = MIX_TrackMSToFrames(m_music_track, 500);
    MIX_StopTrack(m_music_track, frames);
    if (blocking) {
        while (MIX_TrackPlaying(m_music_track))
            SDL_Delay(10);
    }
}

void SoundEngine_SDL::update_channel (int channel)
{
    if (channel < 0 || channel >= (int)m_channelinfo.size() || !m_tracks[channel])
        return;

    SoundEvent &se = m_channelinfo[channel];

    double volume;
    if (se.has_position) {
        ecl::V2 distv = se.position - m_listenerpos;
        volume = se.effectiveVolume(length(distv));
    }
    else
    {
        volume = se.volume;
    }

    MIX_SetTrackGain(m_tracks[channel], static_cast<float>(ecl::Clamp(volume, 0.0, 1.0)));
}

int SoundEngine_SDL::already_playing (const SoundEvent &s)
{
    for (size_t i=0; i<m_channelinfo.size(); ++i) {
        const SoundEvent &se = m_channelinfo[i];

        if (se.active && se.name == s.name && se.playing_time < 0.05
            && (!se.has_position || !s.has_position ||
		ecl::length(se.position - s.position) < se.range + se.fullvol_range))
            return static_cast<int> (i);
    }
    return -1;
}

MIX_Audio *SoundEngine_SDL::cache_sound(const std::string &name)
{
    auto i = wav_cache.find(name);
    if (i == wav_cache.end()) {
        MIX_Audio *audio = nullptr;
        std::string filename;
        if (app.resourceFS->findFile("soundsets/" + name + ".wav", filename)) {
            if (m_mixer)
                audio = MIX_LoadAudio(m_mixer, filename.c_str(), true);
        } else {
            SDL_SetError("Sound not found in resources.");
        }
        if (audio)
            wav_cache.insert(name, audio);
        else
            enigma::Log << "Couldn't load sample '" << name << "': "
                        << SDL_GetError() << std::endl;
        return audio;
    }
    return i->second;
}

void SoundEngine_SDL::cache_sound(const SoundEffect &s) 
{
    std::string filename = s.getFilename();
    if (filename != "")
        cache_sound(filename);
}

bool SoundEngine_SDL::play_sound (const SoundEvent &s)
{
    int channel = already_playing (s);
    if (channel != -1) {
        MutexLock lock(m_instance->m_mutex);
        SoundEvent &se = m_channelinfo [channel];
        if(se.merge(s)) {
            update_channel(channel);
            return true;
        }
    }
    
    if (MIX_Audio *audio = cache_sound(s.name)) {
        channel = -1;
        {
            MutexLock lock(m_instance->m_mutex);
            for (int i = 0; i < m_channels; ++i) {
                if (!m_channelinfo[i].active || !MIX_TrackPlaying(m_tracks[i])) {
                    channel = i;
                    break;
                }
            }
        }

        if (channel != -1) {
            {
                MutexLock lock(m_instance->m_mutex);
                SoundEvent &se = m_channelinfo[channel];
                se = s;
                se.active       = true;
                se.playing_time = 0.0;
            }
            MIX_SetTrackAudio(m_tracks[channel], audio);
            MIX_SetTrackLoops(m_tracks[channel], 0);
            update_channel (channel);
            MIX_PlayTrack(m_tracks[channel], 0);
        }
        return true; // even if no free channel was found
    }
    return false;
}

bool SoundEngine_SDL::is_music_playing() {
    return m_music_track && MIX_TrackPlaying(m_music_track);
}

void SoundEngine_SDL::tick (double dtime)
{
    MutexLock lock(m_instance->m_mutex);
    for (size_t i=0; i<m_channelinfo.size(); ++i) {
        SoundEvent &se = m_channelinfo[i];
        if (se.active)
            se.playing_time += dtime;
    }
}

void SoundEngine_SDL::define_sound (
    const SoundName &name, 
    const SoundData &data)
{
    Uint32 bufsize = static_cast<Uint32> (data.buf.size());
    MIX_Audio *ch = ChunkFromRaw (&data.buf[0], bufsize,
                                  data.freq, SDL_AUDIO_S8, data.nchannels);
    if (ch != nullptr)
        wav_cache.insert(name, ch);
}

void SDLCALL SoundEngine_SDL::channel_finished (void *userdata, MIX_Track * /*track*/)
{
    int channel = (int)(uintptr_t)userdata;
    if (m_instance && channel >= 0 && channel < (int)m_instance->m_channelinfo.size()) {
        MutexLock lock(m_instance->m_mutex);
        SoundEvent &se = m_instance->m_channelinfo[channel];
        se.active = false;
    }
}

Sint8* SoundEngine_SDL::resample (const Sint8 *data, Uint32 len, int oldfreq,
                                  int newfreq, Uint32 *newlen_)
{
    assert (data);
    assert (len>0);
    assert (oldfreq > 0);
    assert (newfreq > 0);
    const int sample_size = 1;    //  8bit sample data

    float ratio = float(oldfreq) / float(newfreq);
    Uint32 newlen = ecl::round_down<int> (len / ratio);
    *newlen_ = newlen;
    Sint8 *newdata = (Sint8*) malloc (sample_size * newlen);
    if (!newdata)
        return 0;

    const Sint8 *src = data;
    Sint8       *dst = newdata;

    float srcinc = float (len-1) / float (newlen); 
    for (unsigned i=0; i<newlen; ++i) {
        int srcidx = ecl::round_down <int> (i * srcinc);
        float a2 = i*srcinc - srcidx;
        float a1 = 1.0f - a2;
        dst[i] = static_cast<Sint8> ((a1*src[srcidx] + a2*src[srcidx+1])/2);
    }
    return newdata;
}

MIX_Audio* SoundEngine_SDL::ChunkFromRaw (const Uint8 *buf, Uint32 len,
                                          int sfreq, int sformat, int schannels)
{
    if (sound::IsSoundMute() || !buf || !m_mixer)
        return nullptr;

    // Get destination format
    SDL_AudioSpec dst_spec;
    if (!MIX_GetMixerFormat(m_mixer, &dst_spec)) {
        dst_spec.freq = 44100;
        dst_spec.format = SDL_AUDIO_S16;
        dst_spec.channels = 2;
    }

    // Convert audio data
    SDL_AudioSpec src_spec = {static_cast<SDL_AudioFormat>(sformat), schannels, sfreq};
    Uint8* dst_data = nullptr;
    int dst_len = 0;
    if (SDL_ConvertAudioSamples(&src_spec, buf, len, &dst_spec, &dst_data, &dst_len)) {
        MIX_Audio* audio = MIX_LoadRawAudio(m_mixer, dst_data, dst_len, &dst_spec);
        SDL_free(dst_data);
        return audio;
    }
    return nullptr;
}
