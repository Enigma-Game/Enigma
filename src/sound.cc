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
#include "enigma.hh"
#include "options.hh"
#include "sound.hh"
#include "main.hh"
#include "lua.hh"

#include "SDL.h"
#include "SDL_mixer.h"

#include <string>
#include <iostream>
#include <cassert>
#include <memory>

extern "C" {
#include "lua.h"
}


using namespace std;
using namespace enigma;
using namespace sound;

#include "sound_internal.hh"

/*! This function converts raw audio data with a specified format to
  the mixer's audio format.  This is used for converting the original
  Oxyd sounds to a format usable by Enigma. */
Mix_Chunk *ChunkFromRaw (const Uint8 *buf, Uint32 len,
                         int freq, int format, int channels);


/* -------------------- SoundEffect implementation -------------------- */
SoundEffect::SoundEffect ()
: name(""), has_position(false), position(),
  priority (0), volume (0.0),
  left (0), right(0), active (false),
  playing_time (0)
{
}

bool not_active (const SoundEffect &s)
{
    return !s.active;
}

bool lower_priority (const SoundEffect &s, const SoundEffect &t)
{
    return s.priority < t.priority;
}



/* -------------------- SoundEngine_SDL implementation -------------------- */

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
}


void SoundEngine_SDL::stop_music() 
{
    Mix_HaltMusic();
    Mix_FreeMusic(m_current_music);
    m_current_music = 0;
}

bool SoundEngine_SDL::play_music (const std::string &filename) 
{
    if (Mix_Music *music = Mix_LoadMUS(filename.c_str())) {
        if (m_current_music)
            Mix_FreeMusic (m_current_music);
        m_current_music = music;
        Mix_PlayMusic (m_current_music, -1);
        Mix_VolumeMusic (m_musicvolume);
        return true;
    }
    return false;
}

void SoundEngine_SDL::fadeout_music()
{
    while (Mix_FadingMusic() != MIX_NO_FADING)
        SDL_Delay(10);

    if (Mix_PlayingMusic()) {
        Mix_FadeOutMusic(500);
        SDL_Delay(400);
    }
    while (Mix_PlayingMusic())
        SDL_Delay(10);
}

void SoundEngine_SDL::update_channel (int channel)
{
    // If distance sound origin <= this value, play at full volume
    const double fullvol_range = 0.2;

    // How far can sound travel?
    const double range = 30;    

    SoundEffect &se = m_channelinfo[channel];

    double volume;
    int    left;
    int    right;
    if (se.has_position) {
        ecl::V2 distv = se.position - m_listenerpos;
        double dist = max(0.0, length(distv) - fullvol_range);

        int xdist = int(distv[0] * options::GetDouble("StereoSeparation"));

        left  = ecl::Clamp (255 - xdist, 0, 255);
        right = ecl::Clamp (255 + xdist, 0, 255);
        volume = (1 - dist/range) * se.volume;
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

int SoundEngine_SDL::already_playing (const SoundName &name)
{
    for (size_t i=0; i<m_channelinfo.size(); ++i) {
        const SoundEffect &se = m_channelinfo[i];

        if (se.active && se.name == name && se.playing_time < 0.05)
            return i;
    }
    return -1;
}


Mix_Chunk *SoundEngine_SDL::cache_sound(const std::string &name)
{
    ecl::Dict<Mix_Chunk*>::iterator i=wav_cache.find(name);
    if (i == wav_cache.end()) {
        Mix_Chunk *ch = 0;
        string filename;
        if (enigma::FindFile("sound/" + name + ".wav", filename))
             ch = Mix_LoadWAV(filename.c_str());
        if (ch != 0)
            wav_cache.insert(name, ch);
        else
	    enigma::Log << "Couldn't load sample: " << Mix_GetError() << endl;
        return ch;
    } else
        return i->second;
}

void SoundEngine_SDL::play_sound (const SoundEffect &s)
{
    int channel = already_playing (s.name);
    if (channel != -1) {
        MutexLock (m_instance->m_mutex);
        SoundEffect &se = m_channelinfo [channel];
        if (se.has_position) {
            se.position = (se.position + s.position) / 2;
            update_channel (channel);
            return;
        }
    }
    
    if (Mix_Chunk *chunk = cache_sound(s.name)) {
	channel = -1; //Mix_GroupOldest(-1);

        channel = Mix_PlayChannel(channel, chunk, 0);

        if (channel != -1) {
            {
                MutexLock (m_instance->m_mutex);
                SoundEffect &se = m_channelinfo[channel];
                se = s;
                se.active       = true;
                se.playing_time = 0.0;
            }
            update_channel (channel);
        }
    }
}

void SoundEngine_SDL::tick (double dtime)
{
    MutexLock (m_instance->m_mutex);
    for (size_t i=0; i<m_channelinfo.size(); ++i) {
        SoundEffect &se = m_channelinfo[i];
        if (se.active)
            se.playing_time += dtime;
    }
}


void SoundEngine_SDL::define_sound (
    const SoundName &/*name*/,
    const std::string &/*filename*/)
{
}

void SoundEngine_SDL::define_sound (
    const SoundName &name, 
    const SoundData &data)
{
    Mix_Chunk *ch= ChunkFromRaw (&data.buf[0], data.buf.size(),
                                 data.freq, AUDIO_S8, data.nchannels);
    if (ch != 0)
        wav_cache.insert(name, ch);
}

void SoundEngine_SDL::channel_finished (int channel)
{
    MutexLock (m_instance->m_mutex);
    SoundEffect &se = m_instance->m_channelinfo[channel];
    se.active = false;
}




/* -------------------- Local variables -------------------- */
namespace
{
    auto_ptr<SoundEngine> sound_engine;

    bool sound_enabled      = true;
    bool music_enabled      = true;
    bool sound_enabled_temp = false;

    string     current_music_name;
}



/* -------------------- Functions -------------------- */

void sound::Init() 
{
    if (!sound_engine.get()) {
        sound_engine.reset(new SoundEngine_SDL);
    }

    if (sound_engine->init()) {
        options::UpdateVolume();
    }
    else {
        sound_enabled = false;
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
}

void sound::DisableSound() {
    if (sound_enabled) {
        sound_enabled = false;
        Shutdown();
    }
}

void sound::EnableSound() {
    if (!sound_enabled) {
        sound_enabled = true;
        Init();
    }
}

void sound::TempDisableSound() {
    sound_enabled_temp = sound_enabled;
    sound_enabled      = false;
}

void sound::TempReEnableSound() {
    sound_enabled = sound_enabled_temp;
}

void sound::DisableMusic() {
    music_enabled = false;
}

void sound::SetListenerPosition (const ecl::V2 &pos) 
{
    sound_engine->set_listenerpos (pos);
}

void sound::PlaySound (const SoundName &name, const ecl::V2 &pos, double volume, int priority) 
{
    if (!sound_enabled)
        return;

    SoundEffect se;
    se.name         = name;
    se.has_position = true;
    se.position     = pos;
    se.priority     = priority;
    se.volume       = volume * options::GetDouble("SoundVolume");
    se.left = se.right = 0;

    sound_engine->play_sound (se);
}

void sound::PlaySoundGlobal (const SoundName &name, double volume, int priority) 
{
    SoundEffect se;
    se.name         = name;
    se.has_position = false;
    se.position     = ecl::V2();
    se.priority     = priority;
    se.volume       = volume * options::GetDouble("SoundVolume");
    se.left         = 255;
    se.right        = 255;

    sound_engine->play_sound (se);
}

void sound::FadeoutMusic() 
{
    sound_engine->fadeout_music();
}

void sound::PlayMusic (const std::string &name) 
{
    if (!sound_enabled || !music_enabled || name==current_music_name)
        return;

    FadeoutMusic();

    string fname;
    if (file::FindFile (name, fname) && sound_engine->play_music (fname))
        current_music_name = name;
    else
        current_music_name = "";

}

void sound::StopMusic() {
    sound_engine->stop_music();
    current_music_name = "";
}

void sound::StopMusic (const std::string &name) {
    if (name==current_music_name)
        StopMusic();
}

void sound::ClearCache()
{
    sound_engine->clear_cache();
}

void sound::DefineSound (const SoundName &name, const SoundData &data)
{
    sound_engine->define_sound (name, data);
}

void sound::SetSoundVolume (double vol)
{
    sound_engine->set_sound_volume (vol);
}

void sound::SetMusicVolume (double vol)
{
    sound_engine->set_music_volume (vol);
}

/* SDL_ConvertAudio is only capable of changing the sound frequency by
   integer powers of 2 (i.e., by a factor of ... 1/4 1/2 1 2 ...).
   The sound files used by Oxyd are sampled at 6kHz which we must
   convert to roughly 22kHz.  This function resamples between any two
   frequencies using simple linear interpolation.  It is not capable
   of changing the sample format or dealing with more than one
   channel.

   FIXME: We should apply a lowpass filter after reampling to get rid
   of the artifacts introduced by linear interpolation or use a better
   interpolator.
*/
namespace
{
    Sint8 *resample (const Sint8 *data, Uint32 len, int oldfreq, int newfreq, 
                     Uint32 *newlen_)
    {
        assert (data);
        assert (len>0);
        assert (oldfreq > 0);
        assert (newfreq > 0);
        const int sample_size = 1;    //  8bit sample data

        float ratio = float(oldfreq) / float(newfreq);
        size_t newlen = ecl::round_down<int> (len / ratio);
        *newlen_ = newlen;
        Sint8 *newdata = (Sint8*) malloc (sample_size * newlen);
        if (!newdata)
            return 0;

        const Sint8 *src = data;
        Sint8       *dst = newdata;

        float srcinc = float (len-1) / float (newlen); 
        for (unsigned i=0; i<newlen; ++i) {
            int srcidx = ecl::round_down <int> (i * srcinc); // srcpos);
            float a2 = i*srcinc - srcidx;
            float a1 = 1.0f - a2;
            dst[i] = static_cast<Sint8> ((a1*src[srcidx] + a2*src[srcidx+1])/2);
        }
        return newdata;
    }

//     typedef float (*FilterFunc)(float);

//     inline double sinc(double x)
//     {
//         if (x == 0)
//             return 1;
//         x *= M_PI;
//         return sin(x)/x;
//     }

//     class Sinc {
//     public:
//         static const int support = 4;

//         static double func (double x) {
//             if (x < -4 || x > 4)
//                 return 0;
//             else
//                 return sinc(x);
//         }
//     };


//     class Lanczos2 {
//     public:
//         static const int support = 2;

//         static double func (double x) {
//             if (x < -2 || x > 2)
//                 return 0;
//             else
//                 return sinc(x) * sinc(x/2);
//         }
//     };

//     class Lanczos3 {
//     public:
//         static const int support = 3;

//         static double func (double x) {
//             if (x < -3 || x > 3)
//                 return 0;
//             else
//                 return sinc(x) * sinc(x/3);
//         }
//     };


//     template <class Filter, class SampleT>
//     class Resampler {
//         double ratio;
//         vector<double> G;
//     public:
//         Resampler (double ratio_)
//         : ratio (ratio_)
//         {
//         }

//         size_t calc_destlen (size_t sourcelen)
//         {
//             return static_cast<size_t> ((sourcelen / ratio + 1) 
//                                         * sizeof (SampleT));
//         }

//         void resample (const SampleT *source, size_t sourcelen, 
//                        SampleT *dest, size_t destlen)
//         {
//             double        factor  = (double)sourcelen / destlen;
//             int           support = Filter::support;
//             double         dx      = ecl::Max(factor, 1.0);
//             vector<double> F (2*support + 1);

//             SampleT *dp = dest;
//             for (size_t t=0; t<destlen; ++t) 
//             {
//                 double center = (t+0.5)*factor; // kernel center in the source
//                 int   start  = ecl::Max<int>(0, center-support + 0.5);
//                 int   stop   = ecl::Min<int>(sourcelen, center+support+0.5);
//                 int   n      = stop-start;

//                 // Calculate filter coefficients
//                 double c      = 0;
//                 double pos = start-center+0.5;
//                 for (int j=0; j<n; ++j) {
//                     F[j] = Filter::func(pos/dx);
//                     c += F[j];
//                     pos++;
//                 }
//                 // Normalize filter
//                 if (c != 0 && c!=1)
//                     for (int j=0; j<n; ++j)
//                         F[j] /= c;

//                 // Calculate next destination value
//                 const SampleT *sp = source + start;
//                 double accu = 0;
//                 for (int j=n-1; j>=0; --j)
//                     accu += F[j] * sp[j];
//                 *dp++ = static_cast<SampleT> (accu); 
// //                 *dp++ = static_cast<SampleT> (ecl::Clamp<double> (accu, -128, 127));
//             }
//         }

//     };
}

Mix_Chunk * ChunkFromRaw (const Uint8 *buf, Uint32 len,
                          int sfreq, int sformat, int schannels)
{
    if (!sound_enabled || !buf)
        return 0;

    // Get destination format
    int dfreq, dchannels;
    Uint16 dformat;
    Mix_QuerySpec (&dfreq, &dformat, &dchannels);

    // Resample
//     Resampler<Sinc, Sint8> r (double(sfreq)/dfreq);
//     size_t newlen = r.calc_destlen(len);
//     Uint8 *newbuf = (Uint8*) malloc (newlen);
//     r.resample ((Sint8*)buf, len, (Sint8*)newbuf, newlen);

    // Resample
    Uint32 newlen=0;
    Uint8 *newbuf = (Uint8*)resample((const Sint8*)buf, len, sfreq, dfreq, &newlen);

    // Convert audio data
    SDL_AudioCVT cvt;
    if (!SDL_BuildAudioCVT (&cvt, sformat, schannels, dfreq,
                            dformat, dchannels, dfreq))
        return 0;

    cvt.buf = (Uint8*) malloc(newlen * cvt.len_mult);
    cvt.len = newlen;
    memcpy(cvt.buf, newbuf, newlen);
    free(newbuf);

    SDL_ConvertAudio(&cvt);

    Mix_Chunk *chunk = Mix_QuickLoad_RAW(cvt.buf, cvt.len_cvt);
    chunk->allocated = 1;
    return chunk;
}

bool sound::SoundEvent (const std::string &eventname, const ecl::V2 &pos, double volume)
{
    lua_State *L = lua::GlobalState();

    lua_getglobal (L, "SoundEvent");
    lua_pushstring (L, eventname.c_str());
    lua_pushnumber (L, pos[0]);
    lua_pushnumber (L, pos[1]);
    lua_pushnumber (L, volume);
    lua_call (L, 4, 1);

    int success = static_cast<int> (lua_tonumber (L, 1));
    lua_pop (L, 1);
    return success != 0;
}
