namespace
{

/* -------------------- SoundEvent -------------------- */

    struct SoundEvent {
        // Variables
        SoundName name;
        bool      has_position;
        ecl::V2   position;
        int       priority;
        double    volume;           // Volume between 0.0 and 1.0
        int       left;
        int       right;

        // Variables used internally by sound engine
        bool      active;
        double    playing_time;

        // Constructor
        SoundEvent ();
    };

/* -------------------- SoundEffect and SoundEffectRepository ---------------- */

/*! This class stores information about how to play sound events
    and provides methods to play them. */

    class SoundEffect {
    public:
        SoundEffect(string name_, string soundset_key_, string filename_,
                       double volume_, bool loop_, bool global_, int priority_,
                       double damp_max_, double damp_inc_, double damp_mult_,
                       double damp_min_, double damp_tick_, string silence_string_)
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

        void setFilename(string filename_) { filename = filename_; }
        string getFilename() const { return filename; }
        bool play(const ecl::V2 &pos = ecl::V2(), double vol = 1.0, bool glob = false);
        DampingData getDampingData() { return damp; }
        string getSoundSetKey() { return soundset_key; }
        string getSilenceString() { return silence_string; }

    private:
        string name;
        string filename;
        string soundset_key;
        string silence_string;
        double volume;
        bool loop;
        bool global;
        int priority;
        DampingData damp;
    };

    class SoundSet {
    public:
        SoundSet(string soundset_key_, int button_position_, OxydLib::OxydVersion oxyd_ver_)
        : soundset_key(soundset_key_), is_oxyd(true), oxyd_ver(oxyd_ver_),
          button_position(button_position_) {}

        SoundSet(string soundset_key_, int button_position_)
        : soundset_key(soundset_key_), is_oxyd(false),
          oxyd_ver(OxydLib::OxydVersion_Invalid), button_position(button_position_) {}

        SoundSet()
        : soundset_key(""), is_oxyd(false),
          oxyd_ver(OxydLib::OxydVersion_Invalid), button_position(-1) {}

        bool activate();
        OxydLib::OxydVersion getOxydVersion() { return oxyd_ver; }
        bool isOxyd() { return is_oxyd; }
        string getSoundSetKey() { return soundset_key; }
        int getButtonPosition() { return button_position; }
        void setButtonPosition(int pos) { button_position = pos; }

    private:
        string soundset_key;
        bool is_oxyd;
        OxydLib::OxydVersion oxyd_ver;
        int button_position;
    };
    
    typedef map<string, SoundEffect> SoundEffectRepository;
    typedef map<string, SoundSet> SoundSetRepository;

/* -------------------- Music and MusicQueue -------------------- */

/*! The class "MusicSingle" holds filename and default playing information
    for a single music file. Several music files can be combined into
    a "MusicQueue" to play in a given or random sequence. MusicQueues are
    used for menu music. One MusicQueue corresponds to one choice on the
    option menu's button. */

    class MusicSingle {
    public:
        MusicSingle(string title_, string filename_, Uint32 length_,
                    Uint32 loop_start_, Uint32 loop_end_, bool allows_loop_)
        : title(title_), filename(filename_), length(length_),
          loop_start(loop_start_), loop_end(loop_end_),
          allows_loop(allows_loop_), start_time() {}

        MusicSingle(string title_, string filename_)
        : title(title_), filename(filename_), length(0),
          loop_start(0), loop_end(0), allows_loop(false), start_time() {}

        MusicSingle()
        : title(""), filename(""), length(0), loop_start(0), loop_end(0),
          allows_loop(false) {}

        bool start();
        bool maybeLoopBack();

    private:
        string title;
        string filename;
        Uint32 length;      // in milliseconds
        Uint32 loop_start;  // where the loop starts
        Uint32 loop_end;    // where the loop should end (but continues playing until next tick)
        Uint32 start_time;  // number of milliseconds since SDL init
        bool allows_loop;
    };

    class MusicQueue {
    public:
        MusicQueue(string title_, int button_position_)
        : title(title_), button_position(button_position_),
          current_position_in_queue(-1), single_title() {}

        MusicQueue()
        : title(""), button_position(-1),
          current_position_in_queue(-1), single_title() {}

        bool start();
        bool next();
        string getCurrentMusicTitle();
        int getButtonPosition() { return button_position; }
        void setButtonPosition(int pos) { button_position = pos; }
        void appendSingle(string title);

    private:
        int current_position_in_queue;
        string title;
        int button_position;
        vector<string> single_title;
    };

    typedef map<string, MusicSingle> MusicSingleRepository;
    typedef map<string, MusicQueue> MusicQueueRepository;

    /*! MusicContext is a nominal condition to change the
      music during the next tick.
      NONE: during initialisation (don't play music now),
      MENU/GAME: play music suitable for menu or during game. */
    enum MusicContext { MUSIC_NONE, MUSIC_MENU, MUSIC_GAME };

        
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

        // ---------- Music and music repository ----------

        virtual bool play_music(const std::string &filename, double position) = 0;
        virtual void stop_music() = 0;
        virtual void fadeout_music() = 0;
        void setMusicContext(MusicContext context) { music_context = context; }
        MusicContext getMusicContext() { return music_context; }
        bool defineMusicSingle(string title, string filename);
        bool playMusicSingle(string title);
        bool setActiveMusicQueue(string music_queue_title);
        string getActiveMusicQueueTitle() { return active_music_queue; }
        void music_tick(double dtime);
        void init_music();
        string getMusicQueueByPosition(int button_position);
        int getMenuMusicQueueCount();
        int getMusicQueueButtonPosition(string music_queue_title) {
            return music_queues[music_queue_title].getButtonPosition();
        }
        
        // ---------- Sound effects ----------

        virtual void clear_cache() = 0;
        virtual void define_sound(const SoundName &, const SoundData &)=0;
        virtual bool play_sound(const SoundEvent &s) = 0;
        virtual void cache_sound(const SoundEffect &s) = 0;
        virtual void set_listenerpos(ecl::V2 pos) = 0;
        virtual void tick(double dtime) {}

        // ---------- Sound effect repository and sound sets ----------

        void setActiveSoundSetKey(string soundset_key) {active_sound_set_key=soundset_key;}
        string getActiveSoundSetKey() { return active_sound_set_key; }
        void setDefaultSoundSet(string soundset_name) {default_sound_set=soundset_name;}
        string getDefaultSoundSet() { return default_sound_set; }
        string effectKey(string effect_name, string soundset_name = "");
        DampingData getDampingData(string effect_name) {
            return sound_effects[effectKey(effect_name)].getDampingData(); }
        void defineSoundEffect(string soundset_key, string name, SoundEffect se) {
            sound_effects[effectKey(name, soundset_key)] = se;
        }
        bool emitSoundEvent (const std::string &eventname, const ecl::V2 &pos = ecl::V2 (), 
                             double volume = 1.0, bool force_global = false);
        void writeSilenceString (const std::string &eventname);
        void initSoundSets();
        bool defineSoundSet(string soundset_name, string soundset_key, int button_position);
        bool defineSoundSetOxyd(string soundset_name, string soundset_key,
                                OxydLib::OxydVersion oxyd_ver, int button_position);
        string getOxydSoundSet(OxydLib::OxydVersion oxyd_ver);

        int convertToOldSoundSetNumber(string soundset_name);
        string convertFromOldSoundSetNumber(int soundset_number);

        void setActiveSoundSet(string soundset_name);
        void preloadSoundEffects();

        void setSoundSetCount(int count) { sound_set_count = count; }
        int getSoundSetCount() { return sound_set_count; }
        
        int getSoundSetButtonPosition(string soundset_name) {
            return sound_sets[soundset_name].getButtonPosition();
        }
        string getSoundSetByPosition(int button_position);

    private:
        SoundSetRepository       sound_sets;
        SoundEffectRepository    sound_effects;
        string                   active_sound_set_key;
        string                   default_sound_set;
        int                      sound_set_count;
        MusicSingleRepository    music_singles;
        MusicQueueRepository     music_queues;
        string                   active_music_queue;
        MusicContext             music_context;
    };

    class SoundEngine_Null : public SoundEngine {
    public:

        // SoundEngine interface
        bool init() { return true; }
        void shutdown() {}
        void clear_cache() {}
        bool is_initialized() const { return true; }
        void set_sound_volume(double /*soundvol*/) {}
        void set_music_volume(double /*musicvol*/) {}
        bool play_music (const std::string &/*filename*/, double /*position*/) { return false; }
        void stop_music() {}
        void fadeout_music() {}

        bool play_sound (const SoundEvent &) {}
        void cache_sound(const SoundEffect &s) {}
        void define_sound (const SoundName &, const SoundData &) {}
        void set_listenerpos (ecl::V2 pos) {}
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
        void fadeout_music();

        bool play_sound(const SoundEvent &s);
        void cache_sound(const SoundEffect &s);
        void clear_cache();
        void define_sound (const SoundName &, const SoundData &);

        void set_listenerpos (ecl::V2 pos) { m_listenerpos = pos; }
        void tick (double dtime);
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
        vector<SoundEvent> m_channelinfo;
        ecl::V2      m_listenerpos;
        SDL_mutex  *m_mutex;
        static SoundEngine_SDL *m_instance;
    };
}
