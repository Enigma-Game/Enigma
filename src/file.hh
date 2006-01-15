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
#ifndef FILE_HH_INCLUDED
#define FILE_HH_INCLUDED

#include <iosfwd>
#include <vector>
#include <list>
#include "ecl_error.hh"

namespace file
{
    typedef std::vector<char> ByteVec;
    typedef std::string FileName;
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

/* -------------------- FileHandles -------------------- */

    class FileHandle {
    public:
        virtual ~FileHandle() {}

        // FileHandle interface.
        virtual bool   exists() const = 0;
        virtual void   read (ByteVec &buffer) = 0;
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
    
    /**
     *
     */
    class GameFS {
    public:
        GameFS::GameFS();

        void clear() { entries.clear(); }
		
        void append_dir (const string &path);
        void prepend_dir (const string &path);

        void prepend_zip (const string &filename);
        void setDataPath (const string &p); 

        FileHandle *find_file (const FileName &);
        
        bool find_file(const string &filename, string &dest) const;
		
        std::list <string> find_files (const string &folder,
                                       const string &filename) const;
		
    private:
        // Variables
        std::vector<FSEntry> entries;
    };

/* -------------------- Helper functions -------------------- */

    /*! Load a complete file/input stream `is' into `dst'.  */
    std::istream &readfile (std::istream &is, ByteVec &dst, int blocksize=512);

/* -------------------- Searching for files -------------------- */

    /*
     * The "data path" is a list of directories that are searched when
     * Enigma tries to find a data file (for example a png image). The
     * data path is usually specified as a single string like
     * ".:~/.enigma:/usr/local/share/enigma" with the ":" separating
     * the list entries.
     *
     * When searching for files, the file name may of course include
     * subdirectory names. For example:
     * 
     *   FindDataFile("graphics/bomb.png")
     *   -> "/usr/local/share/enigma/graphics/bomb.png"
     *   or "/home/user/.enigma/graphics/bomb.png"
     */
    
    /*! Set the current data path. */
    void SetDataPath (const std::string &p);

    /*! Add a new path to the front of the path list. */
    void AddDataPath (const std::string &p);

    /*! Find a file named `f'.  Return `f' none could be found. */ 
    std::string FindDataFile (const FileName &f);
    std::string FindDataFile (const std::string &path, 
                              const FileName &f);
    std::list <std::string> FindDataFiles(const std::string &path, 
                                          const FileName &f);

    /*! Find a file `f' and store the full path in `dst_path'. */
    bool FindFile (const FileName &f, std::string &dst_path);

    /*! Find an image file named `f' in the resolution-dependent
      graphics directories "gfx??" or in "gfx" and store the
      path in `dst_path'.  Returns true if successful. */
    bool FindImageFile (const FileName &f, std::string &dst_path);

    FileHandle *FindFile (const FileName &f);
}
#endif
