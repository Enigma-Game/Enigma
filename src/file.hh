/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2006,2007,2008,2009 Ronald Lamprecht
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

#include "ecl_error.hh"
#include "video.hh"

#include <iosfwd>
#include <vector>
#include <list>
#include <memory>

namespace enigma
{
    typedef std::vector<char> ByteVec;
    typedef std::string FileName;

    enum FSType {
        FS_DIRECTORY,
        FS_ZIPFILE,
    };

    struct FSEntry {
        FSType      type;
        std::string location;

        FSEntry (FSType type_, std::string location_)
        : type (type_), location (std::move(location_))
        {}
    };

    struct DirEntry {
        std::string name;
        bool is_dir;
    };

    class DirIter {
    public:
        static DirIter * instance(const std::string &path);
        virtual ~DirIter ();
        virtual bool open(const std::string &path) = 0;
        virtual bool get_next (DirEntry &entry) = 0;
    protected:
        DirIter();
    };

    enum FindImageReturnCode {
        IMAGE_NOT_FOUND = 0,
        IMAGE_FOUND, // might still be a different image, but no scaling necessary
        IMAGE_NEEDS_SCALING_32_TO_16,
        IMAGE_NEEDS_SCALING_48_TO_64,
        IMAGE_NEEDS_SCALING_32_TO_64,
    };

    /**
     * A GameFS is a list of directories that are searched when
     * Enigma tries to find a data file (for example a png image). The
     * data path is usually specified as a single string like
     * ".:~/.enigma:/usr/local/share/enigma" list with entries separated by
     * the OS dependent separating ":" or ";" (ecl::PathsSeparator).
     * 
     * <p> When searching for files, the file name may of course include
     * subdirectory names. For example:</p>
     * <p><code>
     *   findFile("graphics/bomb.png")
     *   -> "/usr/local/share/enigma/graphics/bomb.png"
     *   or "/home/user/.enigma/graphics/bomb.png"
     * </code></p>
     * <p> Preconfigured GameFS instances are accessable via the Application
     * instance, f.e. <code>app.resourceFS</code></p>
     */
    class GameFS {
    public:
        GameFS();

        void clear() { entries.clear(); }
                
        void append_dir (const std::string &path);
        void prepend_dir (const std::string &path);

        void prepend_zip (const std::string &filename);
        void setDataPath (const std::string &p);
        std::string getDataPath();
        std::vector<std::string> getPaths();

        /**
         * Search first occurence of a file on the GameFS. The file can be
         * a path component like "levels/index.lua".
         * @param filename the searched filename
         * @param dest     the expanded full path of the first occurence.
         * @return  has a file been found.
         */
        bool findFile(const std::string &filename, std::string &dest) const;
                
        /**
         * Search first occurence of a file on the GameFS. The file can be
         * a path component like "levels/e1/hello.lua". Zip archives are searched,
         * too. The path above would be looked up in "levels/e1.zip" as "hello.lua"
         * and as "e1/hello.lua". Plain files preceed zipped onces on every
         * search directory.
         * @param filename the searched filename
         * @param dest     the expanded full path of the first occurence.
         * @param isptr    an opened istream of the first occurence in case
         *                 the file is zipped
         * @return  has a file been found.
         */
        bool findFile(const std::string &filename, std::string &dest, 
                std::unique_ptr<std::istream> &isptr) const;
                
        /**
         * Search first occurence of a file on the GameFS. The file can be
         * a path component like "levels/index.lua". If the file can not be
         * located a message is logged - use this method if you have good
         * reasons to expect the file being found.
         * @param filename the searched filename
         * @return  the expanded full path of the first occurence or ""
         */
        std::string findFile(const std::string &filename);
        
        /**
         * Lists the paths of all files with a given name that reside in
         * a subfolder of the given basepath.
         * The basepath itself is not searched for files. Only direct one
         * level deep subfolders are searched. Of course all components of
         * the GameFS are searched for! Used for searching index like files.
         * @param folder    the basefolder path component, f.e. "levels"
         * @param filename  the searched filename, f.e. "index.lua"
         * @return  a list of fully expanded paths to matching files
         */
        std::list <std::string> findSubfolderFiles (const std::string &folder,
                const std::string &filename) const;
                
        /** Find an image file named `f' in the tileset-dependent
         * graphics directories (e.g. "gfx??") or in "gfx" and store
         * the path in `dst_path' -- a different file might be chosen
         * if file f was not found. Returns a return code encoding
         * the necessary scaling.
         */
        FindImageReturnCode findImageFile (const FileName &f, std::string &dst_path);

    private:
        // Variables
        std::vector<FSEntry> entries;
    };

/* -------------------- Helper functions -------------------- */
    
    bool InitCurl();
    void ShutdownCurl();
    void Downloadfile(std::string url, ByteVec &dst);
    
    /*! Load a complete file/input stream `is' into `dst'.  */
    std::istream &Readfile (std::istream &is, ByteVec &dst, int blocksize=512);
    bool Copyfile(std::string fromPath, std::string toPath);
    
    // banned code to file_zip.cc due to macro clashes
    bool findInZip(std::string zipPath, std::string zippedFilename1,
            std::string zippedFilename2, std::string &dest, 
        std::unique_ptr<std::istream> &isresult);
    
    bool readFromZipStream(std::istream &zipFile, std::ostream &contents);

    std::string extractFromZipString(std::string zipString, std::string fileName);

} // namespace enigma
#endif
