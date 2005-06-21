namespace file
{
    using std::string;


    enum FSType {
        FS_DIRECTORY,
        FS_ZIPFILE,
    };

    struct FSEntry {
        FSType      type;
        std::string location;

        FSEntry (FSType type_, const std::string &location_)
        : type (type_), location (location_)
        {}
    };

    class GameFS {
    public:
        GameFS::GameFS();

        void clear() { entries.clear(); }
		
        void append_dir (const string &path);
        void prepend_dir (const string &path);

        void prepend_zip (const string &filename);

        FileHandle *find_file (const FileName &);
        
        bool find_file(const string &filename, string &dest) const;
		
        std::list <string> find_files (const string &folder,
                                       const string &filename) const;
		
    private:
        // Variables
        std::vector<FSEntry> entries;
    };


    class FileHandle_Dir : public FileHandle {
        std::string m_name;
        std::string m_path;
        bool m_exists;          // File exists

    public:
        FileHandle_Dir (const std::string &name);

        // FileHandle interface.
        bool   exists() const;
        void   read (ByteVec &buffer);
    };

    class FileHandle_Zip : public FileHandle {
    public:
        FileHandle_Zip();

        // FileHandle interface.
        bool   exists() const;
        void   read (ByteVec &buffer);
    };
}
