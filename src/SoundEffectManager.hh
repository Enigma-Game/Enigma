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
#ifndef SOUNDEFFECTMGR_HH_INCLUDED
#define SOUNDEFFECTMGR_HH_INCLUDED

#include "ecl_math.hh"
#include "oxydlib/OxydVersion.h"

#include <string>
#include <vector>
#include <map>

/** ----------- Survey of the formal data structure -----------
 *
 *  The wav-files are not to be accessed directly. There are several layers
 *  between the wav-data and the final EmitSoundEvent-function, to allow for the
 *  following uses:
 *    (a) Sound Sets, possibly user defined,
 *    (b) Access to oxyd's sound data,
 *    (c) Sound damping for loud objects with user defined data,
 *    (d) A "Default" mode which switches the sound set between level packs.
 *    (e) A "silence string" that can be written instead of playing the sound.
 *  For this, six layers of sound information exist.
 *  Note: A sound effect is given by a wav-file and several data how to play it.
 *        An object may choose a sound effect to play. Then it becomes a sound
 *        event (= sound effect + position etc.).
 *
 *    (1) SoundEngine
 *         -> Providing global information and methods for using SDL.
 *    (2) SoundData
 *         -> Holding technical details about how to play the wav-files.
 *    (3) SoundEffect
 *         -> Holding game-related information about how to play a sound
 *            effect. This includes the effect's filename, default volume,
 *            if it's looped, if it's played on a global scale by default,
 *            the default damping information for this event, etc.
 *    (4) SoundSet
 *         -> Holding the sound set key of a sound set (see below), and if
 *            it is connected to Oxyd. It also provides methods to
 *            activate a sound set.
 *    (5) SoundEvent
 *         -> A struct to hold information about a sound event, like the
 *            effect's name, position, priority, volume etc.
 *    (6) SoundDamping
 *         -> Each time an object makes noise, a SoundDamping-record
 *            is created with an entry of the objects address (as void *).
 *            This class modulates the volume with which the next effect
 *            connected to this object is played.
 *
 *  Sound events are normally invoked by "EmitSoundEvent(EFFECTNAME, ...)".
 *  EFFECTNAME can then be e.g. "pickup" or "laseron". This effect name does not
 *  yet define the sound effect completely. Instead, the actually activated sound
 *  set is looked up. It holds a string called sound set key (e.g. "Enigma" or
 *  "Oxyd*"), and together with the effect name they form the effect key, here
 *  "Enigma#pickup" or "Oxyd*#laseron". This effect key is looked up in the sound
 *  effect repository and results in a SoundEffect-entry, which is then played.
 *
 */

/** -------------- About the sound damping system --------------
 *
 *  The sound damping is not automatically used. Instead, it is called through
 *  the "World::getVolume"-function:
 *    sound::EmitSoundEvent (NAME, POS, getVolume(NAME, OBJECT_CALLING, DEF_VOLUME))
 *  OBJECT_CALLING need not be a real address, it is never dereferenced. It just
 *  holds as a representative of the calling object. The SoundDamping-records
 *  are evaluated and finally erased by World::tick_sound_dampings. 
 *
 *  The values used in the sound damping systes can be user defined. In the
 *  following, we use the defaults:
 *
 *  tick_sound_dampings is only to be evaluated every 10th tick (0.1s).
 *  Each damping factor (the ivar connected to the noisy object and its sound
 *  effect name) is reduced by 0.9. This is less than the 10th root of 0.5 (0.933),
 *  so after 1s the damping factor is reduced by more than one half. If the factor
 *  shrinks under 0.5, it is considered 0.
 *
 *  Examples:
 *    1) Frequency less than one sound event per 0.6 seconds.
 *       Then there is no damping at all.
 *    2) N events per second. For each event, factor (F) is raised by
 *       one. And each 0.1 seconds it is multiplied with 0.9. We now
 *       have N/10 events per 0.1 seconds, hence in equilibrium f
 *       oscillates between
 *            f = (f + N/10) * 0.9   =>   f = N
 *       and  f + N/10 = N * 1.1 (geometric series!).
 *  In particular, for large enough N, f is approximately proportional
 *  to N with half-life of less than a second. This is then evaluated
 *  in getVolume.
 * 
 *  World::getVolume returns the volume, if object OBJECT_CALLING wants
 *  to play sound effect NAME with default volume DEF_VOLUME. Often played
 *  sounds from always the same object are damped to reduce noise-level.
 *  Note that OBJECT_CALLING == NULL is explicitly allowed and used e.g.
 *  for all laser-sounds. The damping factor is increased by 1.0 for each
 *  event, and multiplied with 0.9 each 0.1 seconds, thereby approximately
 *  equals the average number of events per second.
 *
 */

/** -------------- Compatibility with 1.00 --------------
 *
 *  Enigma 1.01 uses a new option "SoundSetName", which replaces "SoundSet".
 *  To enable Enigma 1.00 to run on the same system, the "SoundSet" variable
 *  still exists. It is set to 0 (= "Default") for all user sound sets:
 *  When exiting 1.01 with a user sound set and starting 1.00, the default
 *  sound set will be activated for 1.00. Yet, as "SoundSetName" still shows
 *  the old value, 1.01 will use it to find its user sound set. Even if
 *  you change the sound set in 1.00, this will have no effect on the
 *  chosen sound set for 1.01. In contrast to this, if you change the 1.01
 *  sound set, the 1.00 "SoundSet"-variable will be adapted. Except for this
 *  "restriction", you can use two different sound sets for 1.00 and 1.01.
 *
 */

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

/* -------------------- Interface Functions -------------------- */

    void DefineSound (const SoundName &, const SoundData &);
    
    /*! Helper function for oxyd.cc */
    std::string GetOxydSoundSet(OxydLib::OxydVersion oxyd_ver);

    /*! Sound set handling */
    void InitSoundSets();
    void SetActiveSoundSet(std::string soundset_name);
    void SetDefaultSoundSet(std::string soundset_name);

    /*! Define a new sound event. */
    void DefineSoundEffect(std::string soundset_key, std::string name, std::string filename,
                           double volume, bool loop, bool global, int priority,
                           double damp_max, double damp_inc, double damp_mult,
                           double damp_min, double damp_tick, std::string silence_string);

    /*! Trigger a sound event.  Return whether the event was handled. */
    bool EmitSoundEvent (const std::string &eventname,
                         const ecl::V2 &pos = ecl::V2 (), 
                         double volume = 1.0, bool force_global = false);
    bool EmitSoundEventGlobal (const std::string &eventname, double volume = 1.0);

    /*! Send the silence string of a sound effect to command line. */
    void WriteSilenceString (const std::string &eventname);

    /*! Helper functions for options menu */
    int GetOptionSoundSetCount();
    int GetOptionSoundSet();
    void SetOptionSoundSet(int value);
    std::string GetOptionSoundSetText(int value);

/* -------------------- SoundDampingList ---------------- */

/*! This class stores object addresses and assigns volumes to
    sound events, based on the frequency of the event. */

    struct DampingData {
        double incr;
        double maxi;
        double mult;
        double mini;
        double tick;
    };

    class SoundDamping {
    private:
        std::string effect_name;
        const void *origin;
        float factor;
        DampingData damp;

    public:
        SoundDamping(std::string effect_name_, const void *origin_);
        bool is_equal(std::string name2, const void *origin2) {
            return (origin2 == origin) && (effect_name == name2);
        }
        float get_volume(float def_volume);
        bool tick();  // returns true if this entry should be erased
    };

/* -------------------- SoundEvent -------------------- */

    struct SoundEvent {
        // Variables
        SoundName name;
        bool      has_position;
        ecl::V2   position;
        int       priority;
        double    volume;           // Volume between 0.0 and 1.0
        double    range;            // How far can sound travel? (Default: 30.0)
        double    fullvol_range;    // If distance sound origin <= this value, play at full volume
        int       number_events;    // Increased when soundevents are merged
        int       left;
        int       right;

        // Variables used internally by sound engine
        bool      active;
        double    playing_time;

        // Constructor
        SoundEvent ();
        
        // Effective volume calculation
        double    effectiveVolume(double dist);
        
        // Merge another sound event onto this one.
        bool      merge(SoundEvent se);
    };

/* -------------------- SoundEffect and SoundEffectRepository ---------------- */

/*! This class stores information about how to play sound events
    and provides methods to play them. */

    class SoundEffect {
    public:
        SoundEffect(std::string name_, std::string soundset_key_, std::string filename_,
                       double volume_, bool loop_, bool global_, int priority_,
                       double damp_max_, double damp_inc_, double damp_mult_,
                       double damp_min_, double damp_tick_, std::string silence_string_)
        : name(name_), soundset_key(soundset_key_), filename(filename_), volume(volume_),
          loop(loop_), global(global_), priority(priority_),
          silence_string(silence_string_) {
            damp.maxi = damp_max_;
            damp.incr = damp_inc_;
            damp.mult = damp_mult_;
            damp.mini = damp_min_;
            damp.tick = damp_tick_;
        }

        SoundEffect()  // standard empty data set, compare sound-defaults.lua
        : name("EMPTY_EVENT"), filename(""), soundset_key(""), volume(1.0),
          loop(false), global(false), priority(1), silence_string("") {
            damp.maxi = 20.0;
            damp.incr =  1.0;
            damp.mult =  1.0;
            damp.mini =  0.5;
            damp.tick =  0.9;
        }

        void setFilename(std::string filename_) { filename = filename_; }
        std::string getFilename() const { return filename; }
        bool play(const ecl::V2 &pos = ecl::V2(), double vol = 1.0, bool glob = false);
        DampingData getDampingData() { return damp; }
        std::string getSoundSetKey() { return soundset_key; }
        std::string getSilenceString() { return silence_string; }

    private:
        std::string name;
        std::string filename;
        std::string soundset_key;
        std::string silence_string;
        double volume;
        bool loop;
        bool global;
        int priority;
        DampingData damp;
    };

    class SoundSet {
    public:
        SoundSet(std::string soundset_key_, int button_position_, OxydLib::OxydVersion oxyd_ver_)
        : soundset_key(soundset_key_), is_oxyd(true), oxyd_ver(oxyd_ver_),
          button_position(button_position_) {}

        SoundSet(std::string soundset_key_, int button_position_)
        : soundset_key(soundset_key_), is_oxyd(false),
          oxyd_ver(OxydLib::OxydVersion_Invalid), button_position(button_position_) {}

        SoundSet()
        : soundset_key(""), is_oxyd(false),
          oxyd_ver(OxydLib::OxydVersion_Invalid), button_position(-1) {}

        bool activate();
        OxydLib::OxydVersion getOxydVersion() { return oxyd_ver; }
        bool isOxyd() { return is_oxyd; }
        std::string getSoundSetKey() { return soundset_key; }
        int getButtonPosition() { return button_position; }
        void setButtonPosition(int pos) { button_position = pos; }

    private:
        std::string soundset_key;
        bool is_oxyd;
        OxydLib::OxydVersion oxyd_ver;
        int button_position;
    };
    
    typedef std::map<std::string, SoundEffect> SoundEffectRepository;
    typedef std::map<std::string, SoundSet> SoundSetRepository;


/* -------------------- SoundEffectManager -------------------- */

    class SoundEffectManager {
    public:
        static SoundEffectManager* instance();
        ~SoundEffectManager() {}

        // ---------- Sound effect repository and sound sets ----------

        void setActiveSoundSetKey(std::string soundset_key) {active_sound_set_key=soundset_key;}
        std::string getActiveSoundSetKey() { return active_sound_set_key; }
        void setDefaultSoundSet(std::string soundset_name) {default_sound_set=soundset_name;}
        std::string getDefaultSoundSet() { return default_sound_set; }
        std::string effectKey(std::string effect_name, std::string soundset_name = "");
        DampingData getDampingData(std::string effect_name) {
            return sound_effects[effectKey(effect_name)].getDampingData(); }
        void defineSoundEffect(std::string soundset_key, std::string name, SoundEffect se) {
            sound_effects[effectKey(name, soundset_key)] = se;
        }
        bool emitSoundEvent (const std::string &eventname, const ecl::V2 &pos = ecl::V2 (), 
                             double volume = 1.0, bool force_global = false);
        void writeSilenceString (const std::string &eventname);
        void initSoundSets();
        bool defineSoundSet(std::string soundset_name, std::string soundset_key, int button_position);
        bool defineSoundSetOxyd(std::string soundset_name, std::string soundset_key,
                                OxydLib::OxydVersion oxyd_ver, int button_position);
        std::string getOxydSoundSet(OxydLib::OxydVersion oxyd_ver);

        int convertToOldSoundSetNumber(std::string soundset_name);
        std::string convertFromOldSoundSetNumber(int soundset_number);

        void setActiveSoundSet(std::string soundset_name);
        void preloadSoundEffects();

        void setSoundSetCount(int count) { sound_set_count = count; }
        int getSoundSetCount() { return sound_set_count; }
        
        int getSoundSetButtonPosition(std::string soundset_name) {
            return sound_sets[soundset_name].getButtonPosition();
        }
        std::string getSoundSetByPosition(int button_position);
        
    protected:
        SoundEffectManager();
        
    private:
        static SoundEffectManager *theSingleton;
        SoundSetRepository         sound_sets;
        SoundEffectRepository      sound_effects;
        std::string                active_sound_set_key;
        std::string                default_sound_set;
        int                        sound_set_count;
    };
}

#endif
