/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
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
#include "file.hh"
#include "enigma.hh"
#include "video.hh"
#include "main.hh"

#include "ecl_system.hh"

#include "config.h"

#include <sys/types.h>
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#include <algorithm>
#include <ios>
#include <iostream>
#include <fstream>

using namespace enigma;
using namespace ecl;
using namespace std;


namespace
{
    struct DirEntry {
        std::string name;
        bool is_dir;
    };

/* -------------------- DirIter (POSIX) -------------------- */

#ifdef HAVE_DIRENT_H

    class DirIter {
    public:
        DirIter (const std::string &path) : m_dir (NULL), m_entry (NULL) {
            open (path);
        }
        ~DirIter () {
            if (m_dir != NULL)
                closedir (m_dir);
        }

        bool open (const std::string &path) {
            m_dir = opendir (path.c_str());
            return m_dir != 0;
        }
        bool get_next (DirEntry &entry) {
            m_entry = readdir(m_dir);
            if (m_entry != NULL) {
                entry.name = m_entry->d_name;
                entry.is_dir = false;
                return true;
            }
            return false;
        }
    private:
        DIR           *m_dir;
        struct dirent *m_entry;
    };


/* -------------------- DirIter (Win32) -------------------- */

#elif defined (_MSC_VER)

#include <windows.h>

    class DirIter {
    public:
        DirIter (const std::string &path) 
        : m_handle (INVALID_HANDLE_VALUE)
        {
            open (path);
        }
        ~DirIter () {
            close();
        }

        bool open (const std::string &path) {
            std::string glob (path);
            glob += "\\*.*";
            m_handle = FindFirstFile (glob.c_str(), &m_dir);
            return m_handle != INVALID_HANDLE_VALUE;
        }
        bool get_next (DirEntry &entry) {
            if (m_handle != INVALID_HANDLE_VALUE) {
                entry.name = m_dir.cFileName;
                entry.is_dir = false;
                if (!FindNextFile (m_handle, &m_dir)) 
                    close();
                return true;
            }
            return false;
        }
    private:
        void close () {
            if (m_handle != INVALID_HANDLE_VALUE) {
                FindClose (m_handle);
                m_handle = INVALID_HANDLE_VALUE;
            }
        }

        // Variables.
        WIN32_FIND_DATA m_dir;
        HANDLE          m_handle;
    };

#endif

}

/* -------------------- FileHandle_Dir -------------------- */
// 
// FileHandle_Dir::FileHandle_Dir (const std::string &name)
// : m_name (name)
// {
// }
// 
// bool FileHandle_Dir::exists() const
// {
//     return true;
// }
// 
// void FileHandle_Dir::read (ByteVec &buffer)
// {
//     std::ifstream ifs(m_name.c_str());
//     enigma::readfile (ifs, buffer);
// }
// 


/* -------------------- GameFS implementation -------------------- */

GameFS::GameFS() 
: entries ()
{
}

void GameFS::append_dir (const string &path)
{
    std::string full_path = ecl::ExpandPath (path);
    entries.push_back (FSEntry (FS_DIRECTORY, full_path));
}

void GameFS::prepend_dir (const string &path)
{
    std::string full_path = ecl::ExpandPath (path);
    entries.insert (entries.begin(), FSEntry (FS_DIRECTORY, full_path));
}

void GameFS::prepend_zip (const std::string &filename)
{
    std::string path = ecl::ExpandPath (filename);
    entries.insert (entries.begin(), FSEntry (FS_ZIPFILE, path));
}

void GameFS::setDataPath (const string &p) 
{
    clear();

    std::vector<std::string> datapaths;
    split_copy (p, *ecl::PathsSeparator, back_inserter(datapaths));
    for (unsigned i=0; i<datapaths.size(); ++i)
        append_dir (datapaths[i]);
}

std::string GameFS::getDataPath() {
    std::string path;
    
    for (unsigned i=0, size=entries.size(); i < size; ++i) {
        const FSEntry &e = entries[i];
        if (i>0)
            path += ecl::PathsSeparator;
        path += e.location;
    }
    return path;
}

bool GameFS::findFile (const string &filename, string &dest) const 
{
    for (unsigned i=0; i<entries.size(); ++i) {
        const FSEntry &e = entries[i];

        switch (e.type) {
        case FS_DIRECTORY:
            {
                string complete_name = e.location + ecl::PathSeparator + filename;
                if (ecl::FileExists(complete_name))
                {
                    dest = complete_name;
                    return true;
                }
            } break;

        case FS_ZIPFILE:
            {
            } break;
        }
    }
    return false;
}

// enigma::FileHandle *GameFS::findFile (const FileName &n)
// {
//     string fname;
//     if (findFile (n, fname)) {
//         return new FileHandle_Dir (fname);
//     }
//     return 0;
// }

std::string GameFS::findFile(const string &filename)
{
    string found_file;
    if (!findFile(filename, found_file)) {
        enigma::Log << "File not found: " << filename << endl;
        return filename;
    }
    return found_file;
}

std::list <string>
GameFS::findSubfolderFiles(const string &folder, const string &filename) const
{
    std::list <string> matches;

    for (unsigned i=0; i<entries.size(); ++i) {
        const FSEntry &e = entries[i];

        switch (e.type) {
        case FS_DIRECTORY: {
            string complete_name = e.location + ecl::PathSeparator + folder;
            if (ecl::FolderExists(complete_name)) {
                DirIter iter (complete_name);
                DirEntry entry;
                while (iter.get_next (entry)) {
                    if (entry.name != "." && entry.name != "..") {
                        string tmp_name = complete_name + ecl::PathSeparator
                            + entry.name + ecl::PathSeparator + filename;
                        if (ecl::FileExists (tmp_name))
                            matches.push_back (tmp_name);
                    }
                }
            }
            break;
        }
        case FS_ZIPFILE:
            break;
        }
    }
    return matches;
}


/* First search in video mode specific directory, then in "gfx/". */ 
bool GameFS::findImageFile (const string &basename, string &filename)
{
    const video::VMInfo *vminfo = video::GetInfo();
    string fname = string(vminfo->gfxdir) + basename;
    if (!findFile(fname, filename)) {
        fname = string ("gfx/") + basename;
        return findFile(fname, filename);
    }
    return true;
}

/* -------------------- Helper functions -------------------- */

std::istream &
enigma::Readfile (std::istream &is, ByteVec &dest, int blocksize)
{
    size_t len = dest.size();
    int nread=0;
    do {
        dest.resize(dest.size() + blocksize);
        is.read (&dest[len], blocksize);
        nread = is.gcount();
        len += nread;
    } while (nread == blocksize);
    dest.resize(len);
    return is;
}

