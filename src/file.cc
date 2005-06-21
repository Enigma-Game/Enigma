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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id: file.cc,v 1.4 2004/05/22 13:04:31 dheck Exp $
 */
#include "file.hh"
#include "file_internal.hh"
#include "enigma.hh"
#include "video.hh"
#include "main.hh"

#include "px/system.hh"

#include <sys/types.h>
#include <dirent.h>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace file;
using namespace px;
using namespace std;


/* -------------------- FileHandle_Dir -------------------- */

FileHandle_Dir::FileHandle_Dir (const std::string &name)
: m_name (name)
{
}

bool FileHandle_Dir::exists() const
{
    return true;
}

void FileHandle_Dir::read (ByteVec &buffer)
{
    std::ifstream ifs(m_name.c_str());
    file::readfile (ifs, buffer);
}



/* -------------------- GameFS implementation -------------------- */

GameFS::GameFS() 
: entries ()
{
}

void GameFS::append_dir (const string &path)
{
    std::string full_path = px::ExpandPath (path);
    entries.push_back (FSEntry (FS_DIRECTORY, full_path));
}

void GameFS::prepend_dir (const string &path)
{
    std::string full_path = px::ExpandPath (path);
    entries.insert (entries.begin(), FSEntry (FS_DIRECTORY, full_path));
}

void GameFS::prepend_zip (const std::string &filename)
{
    std::string path = px::ExpandPath (filename);
    entries.insert (entries.begin(), FSEntry (FS_ZIPFILE, path));
}


bool GameFS::find_file (const string &filename, string &dest) const 
{
    for (unsigned i=0; i<entries.size(); ++i) {
        const FSEntry &e = entries[i];

        switch (e.type) {
        case FS_DIRECTORY:
            {
                string complete_name = e.location + px::PathSeparator + filename;
                if (px::FileExists(complete_name))
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

FileHandle *GameFS::find_file (const FileName &n)
{
    string fname;
    if (find_file (n, fname)) {
        return new FileHandle_Dir (fname);
    }
    return 0;
}

std::list <string>
GameFS::find_files(const string &folder, const string &filename) const
{
    std::list <string> matches;

    for (unsigned i=0; i<entries.size(); ++i) {
        const FSEntry &e = entries[i];

        switch (e.type) {
        case FS_DIRECTORY:
            {
                string complete_name = e.location + px::PathSeparator + folder;
                if (px::FolderExists(complete_name))
                {
                    DIR *dir = opendir( complete_name.c_str());
                    struct dirent *entry;
                    while((entry = readdir(dir)) != NULL)
                    {
                        if( strcmp( entry->d_name, ".") != 0 && strcmp( entry->d_name, "..") != 0)
                        {
                            string tmp_name = complete_name + px::PathSeparator
                                + string(entry->d_name) + px::PathSeparator + filename;
                            if (px::FileExists (tmp_name))
                                matches.push_back (tmp_name);
                        }
                    }
                    closedir(dir);
                }
            } break;
        case FS_ZIPFILE:
            break;
        }
    }
    return matches;
}

/* -------------------- Helper functions -------------------- */

std::istream &
file::readfile (std::istream &is, file::ByteVec &dest, int blocksize)
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



/* -------------------- Local variables -------------------- */

namespace 
{
    GameFS gamefs;
}


/* -------------------- Functions -------------------- */

void file::SetDataPath (const string &p) 
{
    gamefs.clear();

    std::vector<std::string> datapaths;
    split_copy (p, ':', back_inserter(datapaths));

    for (unsigned i=0; i<datapaths.size(); ++i)
        gamefs.append_dir (datapaths[i]);
}

void file::AddDataPath (const string &path)
{
    gamefs.prepend_dir (path);
}

bool file::FindFile (const string &fname, string &dst_fname) 
{
    return gamefs.find_file(fname, dst_fname);
}

FileHandle *file::FindFile (const FileName &n)
{
    return gamefs.find_file (n);
}


string file::FindDataFile(const string &filename)
{
    string found_file;
    if (!gamefs.find_file(filename, found_file)) {
        enigma::Log << "File not found: " << filename << endl;
        return filename;
    }
    return found_file;
}

string file::FindDataFile(const string &path, const string &filename) 
{
    return FindDataFile(path+"/"+filename);
}

std::list <string>
file::FindDataFiles(const string &path, const string &filename)
{
    return gamefs.find_files(path, filename);
}


/* First search in video mode specific directory, then in "gfx/". */ 
bool file::FindImageFile (const string &basename, string &filename)
{
    const video::VMInfo *vminfo = video::GetInfo();
    string fname = string(vminfo->gfxdir) + basename;
    if (!FindFile(fname, filename)) {
        fname = string ("gfx/") + basename;
        return FindFile(fname, filename);
    }
    return true;
}
