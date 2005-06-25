namespace
{

/* -------------------- SoundEffect -------------------- */

    struct SoundEffect {
        // Variables
        SoundName name;
        bool      has_position;
        ecl::V2    position;
        int       priority;
        double    volume;           // Volume between 0.0 and 1.0
        int       left;
        int       right;

        // Variables used internally by sound engine
        bool      active;
        double    playing_time;

        // Constructor
        SoundEffect ();
    };

/* -------------------- SoundEngine -------------------- */

    class SoundEngine {
    public:
        virtual ~SoundEngine() {}
        
        //! Returns true if successful.
        virtual bool init() = 0;
        virtual void shutdown() = 0;
        virtual bool is_initialized() const = 0;

        virtual void set_sound_volume (double soundvol) = 0;
        virtual void set_music_volume (double musicvol) = 0;

        // ---------- Music ----------

        virtual bool play_music (const std::string &filename) = 0;
        virtual void stop_music() = 0;
        virtual void fadeout_music() = 0;

        // ---------- Sound effects ----------

        virtual void clear_cache() = 0;
        virtual void define_sound (const SoundName &, const std::string &filename)=0;
        virtual void define_sound (const SoundName &, const SoundData &)=0;
        virtual void play_sound (const SoundEffect &s) = 0;
        virtual void set_listenerpos (ecl::V2 pos) = 0;
        virtual void tick (double dtime) 
        {}
    };


    class SoundEngine_Null : public SoundEngine {
    public:

        // SoundEngine interface
        bool init() { return true; }
        void shutdown() {}
        void clear_cache() {}

        bool is_initialized() const 
        { return true; }

        void set_sound_volume(double /*soundvol*/)
        {}

        void set_music_volume(double /*musicvol*/) 
        {}

        bool play_music (const std::string &/*filename*/) 
        { return false; }

        void stop_music() 
        {}

        void fadeout_music() 
        {}

        void play_sound (const SoundEffect &)
        {}

        void define_sound (const SoundName &, const std::string &/*filename*/)
        {}

        void define_sound (const SoundName &, const SoundData &)
        {}
        void set_listenerpos (ecl::V2 pos) 
        {}
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

        bool play_music (const std::string &filename);
        void stop_music();
        void fadeout_music();

        void play_sound(const SoundEffect &s);
        void clear_cache();
        void define_sound (const SoundName &, const std::string &filename);
        void define_sound (const SoundName &, const SoundData &);


        void set_listenerpos (ecl::V2 pos) { m_listenerpos = pos; }
        void tick (double dtime);
    private:
        // ---------- Private methods ----------
        Mix_Chunk *cache_sound(const std::string &name);

        void update_channel (int channel);
        int already_playing (const SoundName &name);


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
        vector<SoundEffect> m_channelinfo;
        ecl::V2      m_listenerpos;
        SDL_mutex  *m_mutex;
        static SoundEngine_SDL *m_instance;
    };
}
