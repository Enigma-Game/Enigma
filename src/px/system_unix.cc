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
 * $Id: system_unix.cc,v 1.4 2003/11/06 12:15:06 mhawlisch Exp $
 */
#include "system.hh"
#include "tools.hh"
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <locale.h>
#include <unistd.h>
#include <cassert>

#include <config.h>

using namespace px;
using std::string;

const char *px::PathSeparator = "/";

string px::ExpandPath (const string &pth)
{
    string path = pth;
    string::size_type p=path.find("~");
    if (p != string::npos) {
        string home;
        if (char *h = getenv("HOME"))
            home = h;
        path.replace(p, 1, home);
    }
    return path;
}

bool px::FileExists (const std::string &fname)
{
    struct stat s;
    return (stat(fname.c_str(), &s)==0 && S_ISREG(s.st_mode));
}

time_t px::FileModTime (const std::string &fname)
{
    struct stat s;
    if (stat(fname.c_str(), &s) == 0) {
        return s.st_mtime;
    }

    return 0;                   // beginning of time
}

bool px::FolderExists (const std::string &fname)
{
    struct stat s;
    return (stat(fname.c_str(), &s)==0 && S_ISDIR(s.st_mode));
}

bool px::FolderCreate (const std::string &fname)
{
    string parent_folder;
    string sub_folder;
    bool   ok = true;

    assert(!FolderExists(fname));

    if (split_path(fname, &parent_folder, &sub_folder)) {
        if (!FolderExists(parent_folder)) {
            ok = FolderCreate(parent_folder);
        }
    }

    if (ok) {
#ifdef __MINGW32__
        ok = mkdir(fname.c_str()) == 0;
#else
        ok = mkdir(fname.c_str(), 0777) == 0;
#endif
    }
    return ok;
}


std::string px::DefaultMessageLocale ()
{
    extern const char * sys_message_locale_name ();

    return sys_message_locale_name ();
}

std::string px::GetLanguageCode (const std::string &ln)
{
    if (ln == "C" || ln == "POSIX" || ln.size() < 2)
        return "en";

    return ln.substr (0, 2);
}

