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
#include "ecl_system.hh"
#include "ecl_util.hh"

#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <locale.h>
#include <unistd.h>
#include <cassert>

#include <config.h>

#ifdef __MINGW32__
#include <windows.h>
#include <shellapi.h>
#elif MACOSX
#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#endif

using namespace ecl;
using std::string;

#ifdef __MINGW32__
const char *ecl::PathSeparator = "\\";  // for path assembly
const char *ecl::PathsSeparator = ";";  // for listing paths in a string
#else
const char *ecl::PathSeparator = "/";   // for path assembly
const char *ecl::PathsSeparator = ":";  // for listing paths in a string
#endif
const char *ecl::PathSeparators = "/\\";  // for path splits

std::string ecl::ExpandPath(const string &pth) {
    std::string path = pth;
    std::string::size_type p = path.find("~");
    if (p != string::npos) {
        std::string home;
        if (char *h = getenv("HOME"))
            home = h;
        path.replace(p, 1, home);
    }
    return path;
}

std::string ecl::BeautifyPath(const std::string path) {
    std::string foreignSeparators = ecl::PathSeparators;
    foreignSeparators.erase(foreignSeparators.find(ecl::PathSeparator));
    std::string result = path;
    for (std::string::size_type pos = result.find_first_of(foreignSeparators);
         pos != std::string::npos; pos = result.find_first_of(foreignSeparators)) {
        result.replace(pos, 1, ecl::PathSeparator);
    }
    return result;
}

bool ecl::FileExists(const std::string &fname) {
    struct stat s;
    return (stat(fname.c_str(), &s) == 0 && S_ISREG(s.st_mode));
}

time_t ecl::FileModTime(const std::string &fname) {
    struct stat s;
    if (stat(fname.c_str(), &s) == 0) {
        return s.st_mtime;
    }

    return 0;  // beginning of time
}

bool ecl::FolderExists(const std::string &fname) {
    struct stat s;
    return (stat(fname.c_str(), &s) == 0 && S_ISDIR(s.st_mode));
}

bool ecl::FolderCreate(const std::string &fname) {
    string parent_folder;
    string sub_folder;
    bool ok = true;

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

bool ecl::BrowseUrl(const std::string url) {
    bool result = true;
#ifdef __MINGW32__
    result == ((INT_PTR)ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL) >= 32);
#elif MACOSX
    CFStringRef cfurlStr = CFStringCreateWithCString(NULL, url.c_str(), kCFStringEncodingASCII);

    // Create a URL object:
    CFURLRef cfurl = CFURLCreateWithString(NULL, cfurlStr, NULL);

    // Open the URL:
    LSOpenCFURLRef(cfurl, NULL);

    // Release the created resources:
    CFRelease(cfurl);
    CFRelease(cfurlStr);
#else
    result = (system(("xdg-open " + url + " &").c_str()) == 0);
#endif
    return result;
}

bool ecl::ExploreFolder(const std::string path) {
    bool result = true;
#ifdef __MINGW32__
    result == ((INT_PTR)ShellExecute(NULL, "explore", path.c_str(), NULL, NULL, SW_SHOWNORMAL) >= 32);
#elif MACOSX
    FSRef fref;
    FSPathMakeRef((UInt8 *)path.c_str(), &fref, NULL);
    LSOpenFSRef(&fref, NULL);
#else
    result = (system(("xdg-open " + path + " &").c_str()) == 0);
#endif
    return result;
}

#ifdef __MINGW32__
// should be ecl_system_windows.cc ?
std::string ecl::ApplicationDataPath() {
    typedef HRESULT(WINAPI * SHGETFOLDERPATH)(HWND, int, HANDLE, DWORD, LPTSTR);
#define CSIDL_FLAG_CREATE 0x8000
#define CSIDL_APPDATA 0x1A
#define SHGFP_TYPE_CURRENT 0

    HINSTANCE shfolder_dll;
    SHGETFOLDERPATH SHGetFolderPath;

    static bool didRun = false;
    static std::string result = "";  // remember exec results

    if (!didRun) {
        didRun = true;
        /* load the shfolder.dll to retreive SHGetFolderPath */
        if ((shfolder_dll = LoadLibrary("shfolder.dll")) != NULL) {
            SHGetFolderPath = (SHGETFOLDERPATH)GetProcAddress(shfolder_dll, "SHGetFolderPathA");
            if (SHGetFolderPath != NULL) {
                TCHAR szPath[MAX_PATH] = "";

                /* get the "Application Data" folder for the current user */
                if (S_OK == SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL,
                                            SHGFP_TYPE_CURRENT, szPath)) {
                    result = szPath;
                }
            }
            FreeLibrary(shfolder_dll);
        }
    }
    return result;
}

void ecl::ToLowerCase(std::string &filename) {
    for (int i = 0; i < filename.length(); i++) {
        char c = filename[i];
        if (c >= 'A' && c <= 'Z')
            filename.replace(i, 1, 1, c - 'A' + 'a');
    }
}

std::set<std::string> ecl::UniqueFilenameSet(std::set<std::string> inSet) {
    std::set<std::string>::iterator it = inSet.begin();
    std::set<std::string> outSet;
    std::set<std::string> lowerSet;
    for (; it != inSet.end(); it++) {
        std::string name = *it;
        ecl::ToLowerCase(name);
        if (lowerSet.find(name) == lowerSet.end()) {
            outSet.insert(*it);
            lowerSet.insert(name);
        }
    }
    return outSet;
}

#endif

std::string ecl::GetLanguageCode(const std::string &ln) {
    if (ln == "C" || ln == "POSIX" || ln.size() < 2)
        return "en";

    return ln.substr(0, 2);
}
