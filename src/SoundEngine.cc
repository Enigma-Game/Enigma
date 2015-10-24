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

#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_mutex.h"

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
    MutexLock (SDL_mutex *m) {
        mutex = m;
        SDL_mutexP (mutex);
    }
    ~MutexLock () {
        SDL_mutexV (mutex);
    };
private:
    SDL_mutex *mutex;
};

SoundEngine_SDL *SoundEngine_SDL::m_instance = 0;

SoundEngine_SDL::SoundEngine_SDL()
: m_initialized(false),
  m_soundvolume (MIX_MAX_VOLUME),
  m_musicvolume (MIX_MAX_VOLUME),
  m_current_music (0),
  m_freq (MIX_DEFAULT_FREQUENCY),
  m_format (MIX_DEFAULT_FORMAT),
  m_channels (MIX_DEFAULT_CHANNELS)
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
        if (SDL_InitSubSystem (SDL_INIT_AUDIO) == -1) {
            fprintf(stderr, "Couldn't open SDL audio subsystem: %s\n", SDL_GetError());
            return false;
        }
        
        const SDL_version* vi = Mix_Linked_Version();
        Log <<  ecl::strf("SDL_mixer Version: %u.%u.%u\n", vi->major, vi->minor, vi->patch);
#ifdef SDL_MIX_INIT
        int mix_flags = MIX_INIT_OGG | MIX_INIT_MOD;
        if (Mix_Init(mix_flags) & mix_flags != mix_flags) {
            Log << ecl::strf( "Couldn't initialize SDL_mixer: %s\n", Mix_GetError());
            return false;
        }
#endif

        // Initialize SDL_mixer lib
        if (Mix_OpenAudio(m_freq, m_format, m_channels, 1024) < 0) {
            fprintf(stderr, "Couldn't open mixer: %s\n", Mix_GetError());
            return false;
        }

        // Update number of available channels
        m_channels = Mix_GroupCount (-1);
        m_channelinfo.resize (m_channels);

        Mix_ChannelFinished (&channel_finished);
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
	Mix_FreeMusic(m_current_music);
        Mix_CloseAudio();
        clear_cache();
        SDL_DestroyMutex (m_mutex);
#ifdef SDL_MIX_INIT
        Mix_Quit();
#endif
        m_initialized = false;
    }
}

void SoundEngine_SDL::clear_cache() 
{
    for (ecl::Dict<Mix_Chunk*>::iterator it = wav_cache.begin(); it != wav_cache.end(); ++it)
        Mix_FreeChunk(it->second);
    wav_cache.clear();
}

void SoundEngine_SDL::set_sound_volume (double soundvol) 
{
    if (!m_initialized)
        return;                 // SDL_mixer crashes without this check

    m_soundvolume = ecl::round_down<int>(ecl::Clamp(soundvol, 0.0, 1.0) * MIX_MAX_VOLUME);
    Mix_Volume (-1, m_soundvolume);
}

void SoundEngine_SDL::set_music_volume (double musicvol) 
{
    if (!m_initialized)
        return;                 // SDL_mixer crashes without this check

    m_musicvolume = ecl::round_down<int>(ecl::Clamp(musicvol, 0.0, 1.0) * MIX_MAX_VOLUME);
    Mix_VolumeMusic (m_musicvolume);
    //Mix_SetPanning(MIX_CHANNEL_POST, m_musicvolume, m_musicvolume);
}


void SoundEngine_SDL::stop_music() 
{
    Mix_HaltMusic();
    Mix_FreeMusic(m_current_music);
    m_current_music = 0;
}

bool SoundEngine_SDL::play_music (const std::string &filename, double position) 
{
    if (Mix_Music *music = Mix_LoadMUS(filename.c_str())) {
        if (m_current_music)
            Mix_FreeMusic (m_current_music);
        m_current_music = music;
        if(Mix_PlayMusic (m_current_music, 1) == -1)
            Log << "Mix_PlayMusic: " << Mix_GetError() << "\n";
        if(position > 0) {
            Log << "Start music at position " << position << "\n";
            if(Mix_SetMusicPosition(position) == -1)
                Log << "Mix_SetMusicPosition: " << Mix_GetError() << "\n";
        }
        Mix_VolumeMusic (m_musicvolume);
        //Mix_SetPanning(MIX_CHANNEL_POST, m_musicvolume, m_musicvolume);
        return true;
    }
    return false;
}

void SoundEngine_SDL::fadeout_music(bool blocking)
{
    while (Mix_FadingMusic() != MIX_NO_FADING) {
        if (!blocking)
            return;
        SDL_Delay(10);
    }

    if (Mix_PlayingMusic()) {
        Mix_FadeOutMusic(500);
        if (!blocking)
            return;
        SDL_Delay(400);
    }
    while (Mix_PlayingMusic())
        SDL_Delay(10);
}

void SoundEngine_SDL::update_channel (int channel)
{
    SoundEvent &se = m_channelinfo[channel];

    double volume;
    int    left;
    int    right;
    if (se.has_position) {
        ecl::V2 distv = se.position - m_listenerpos;
        int xdist = int(distv[0] * options::GetDouble("StereoSeparation"));
        left  = ecl::Clamp (255 - xdist, 0, 255);
        right = ecl::Clamp (255 + xdist, 0, 255);
        volume = se.effectiveVolume(length(distv));
    }
    else
    {
        volume = se.volume;
        left = se.left;
        right = se.right;
    }

    Mix_SetPanning (channel, left, right);

    int mixvol = ecl::round_down<int>(volume * MIX_MAX_VOLUME);
    Mix_Volume(channel, ecl::Clamp(mixvol, 0, MIX_MAX_VOLUME));
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

Mix_Chunk *SoundEngine_SDL::cache_sound(const std::string &name)
{
    ecl::Dict<Mix_Chunk*>::iterator i=wav_cache.find(name);
    if (i == wav_cache.end()) {
        Mix_Chunk *ch = 0;
        std::string filename;
        if (app.resourceFS->findFile("soundsets/" + name + ".wav", filename))
             ch = Mix_LoadWAV(filename.c_str());
        else 
            // Sounds from other resources shoudl return correct error:
            Mix_SetError("Sound not found in resources.");
        if (ch != 0)
            wav_cache.insert(name, ch);
        else
            enigma::Log << "Couldn't load sample '" << name << "': "
                        << Mix_GetError() << std::endl;
        return ch;
    } else
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
        MutexLock (m_instance->m_mutex);
        SoundEvent &se = m_channelinfo [channel];
        if(se.merge(s)) {
            update_channel(channel);
            return true;
        }
    }
    
    if (Mix_Chunk *chunk = cache_sound(s.name)) {
        channel = -1; //Mix_GroupOldest(-1);

        channel = Mix_PlayChannel(channel, chunk, 0);

        if (channel != -1) {
            {
                MutexLock (m_instance->m_mutex);
                SoundEvent &se = m_channelinfo[channel];
                se = s;
                se.active       = true;
                se.playing_time = 0.0;
            }
            update_channel (channel);
        }
        return true; // even if no free channel was found
    } else
        return false;
}

bool SoundEngine_SDL::is_music_playing() {
    return Mix_PlayingMusic() || Mix_PausedMusic();
}

void SoundEngine_SDL::tick (double dtime)
{
    MutexLock (m_instance->m_mutex);
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
    Mix_Chunk *ch= ChunkFromRaw (&data.buf[0], bufsize,
                                 data.freq, AUDIO_S8, data.nchannels);
    if (ch != 0)
        wav_cache.insert(name, ch);
}

void SoundEngine_SDL::channel_finished (int channel)
{
    MutexLock (m_instance->m_mutex);
    SoundEvent &se = m_instance->m_channelinfo[channel];
    se.active = false;
}

/*! SDL_ConvertAudio is only capable of changing the sound frequency by
  integer powers of 2 (i.e., by a factor of ... 1/4 1/2 1 2 ...).
  The sound files used by Oxyd are sampled at 6kHz which we must
  convert to roughly 22kHz.  This function resamples between any two
  frequencies using simple linear interpolation.  It is not capable
  of changing the sample format or dealing with more than one
  channel.

  FIXME: We should apply a lowpass filter after reampling to get rid
  of the artifacts introduced by linear interpolation or use a better
  interpolator. */
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

Mix_Chunk* SoundEngine_SDL::ChunkFromRaw (const Uint8 *buf, Uint32 len,
                                           int sfreq, int sformat, int schannels)
{
    if (sound::IsSoundMute() || !buf)
        return 0;

    // Get destination format
    int dfreq, dchannels;
    Uint16 dformat;
    Mix_QuerySpec (&dfreq, &dformat, &dchannels);

    // Resample
    Uint32 newlen=0;
    Uint8 *newbuf = (Uint8*)resample((const Sint8*)buf, len, sfreq, dfreq, &newlen);

    // Convert audio data
    SDL_AudioCVT cvt;
    if (!SDL_BuildAudioCVT (&cvt, sformat, schannels, dfreq,
                            dformat, dchannels, dfreq))
        return 0; // memory leak!

    cvt.buf = (Uint8*) malloc(newlen * cvt.len_mult);
    cvt.len = newlen;
    memcpy(cvt.buf, newbuf, newlen);
    free(newbuf);

    SDL_ConvertAudio(&cvt);

    Mix_Chunk *chunk = Mix_QuickLoad_RAW(cvt.buf, cvt.len_cvt);
    chunk->allocated = 1;
    return chunk;
}

