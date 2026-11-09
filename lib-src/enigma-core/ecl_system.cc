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

#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <locale.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif

#if defined(_WIN32) || defined(__MINGW32__)
#include <windows.h>
#include <shellapi.h>
#include <direct.h>
#endif
#ifndef S_ISREG
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif
#ifndef S_ISDIR
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif

#if MACOSX
#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#endif

namespace ecl {

#if defined(_WIN32) || defined(__MINGW32__)
const char *PathSeparator = "\\";  // for path assembly
const char *PathsSeparator = ";";  // for listing paths in a string
#else
const char *PathSeparator = "/";   // for path assembly
const char *PathsSeparator = ":";  // for listing paths in a string
#endif
const char *PathSeparators = "/\\";  // for path splits

std::string ExpandPath(const std::string &pth) {
    std::string path = pth;
    std::string::size_type p = path.find('~');
    if (p != std::string::npos) {
        std::string home;
        if (char *h = getenv("HOME"))
            home = h;
        path.replace(p, 1, home);
    }
    return path;
}

std::string BeautifyPath(const std::string& path) {
    std::string foreignSeparators = PathSeparators;
    foreignSeparators.erase(foreignSeparators.find(PathSeparator));
    std::string result = path;
    for (std::string::size_type pos = result.find_first_of(foreignSeparators);
            pos != std::string::npos; pos = result.find_first_of(foreignSeparators)) {
        result.replace(pos, 1, PathSeparator);
    }
    return result;
}

bool FileExists(const std::string &fname) {
    struct stat s;
    return (stat(fname.c_str(), &s) == 0 && S_ISREG(s.st_mode));
}

time_t FileModTime(const std::string &fname) {
    struct stat s;
    if (stat(fname.c_str(), &s) == 0) {
        return s.st_mtime;
    }

    return 0;  // beginning of time
}

bool FolderExists(const std::string &fname) {
    struct stat s;
    return (stat(fname.c_str(), &s) == 0 && S_ISDIR(s.st_mode));
}

bool FolderCreate(const std::string &fname) {
    std::string parent_folder;
    std::string sub_folder;
    bool ok = true;

    assert(!FolderExists(fname));

    if (split_path(fname, &parent_folder, &sub_folder)) {
        if (!FolderExists(parent_folder)) {
            ok = FolderCreate(parent_folder);
        }
    }

    if (ok) {
        #if defined(_WIN32) || defined(__MINGW32__)
        ok = _mkdir(fname.c_str()) == 0;
        #else
        ok = mkdir(fname.c_str(), 0777) == 0;
        #endif
    }
    return ok;
}

bool BrowseUrl(const std::string& url) {
    bool result = true;
    #if defined(_WIN32) || defined(__MINGW32__)
    result = ((INT_PTR)ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL) >= 32);
    #elif MACOSX
    CFStringRef cfurlStr = CFStringCreateWithCString(NULL, url.c_str(), kCFStringEncodingASCII);

    // Create a URL object:
    CFURLRef cfurl = CFURLCreateWithString(NULL, cfurlStr, NULL);

    // Open the URL:
    result = LSOpenCFURLRef(cfurl, NULL) == noErr;

    // Release the created resources:
    CFRelease(cfurl);
    CFRelease(cfurlStr);
    #else
    result = (system(("xdg-open " + url + " &").c_str()) == 0);
    #endif
    return result;
}

bool ExploreFolder(const std::string& path) {
    bool result = true;
    #if defined(_WIN32) || defined(__MINGW32__)
    result = ((INT_PTR)ShellExecute(NULL, "explore", path.c_str(), NULL, NULL, SW_SHOWNORMAL) >= 32);
    #elif MACOSX
    CFStringRef cfurlStr = CFStringCreateWithCString(kCFAllocatorDefault, path.c_str(), kCFStringEncodingUTF8);

    // Create a file URL object:
    CFURLRef cfurl = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, cfurlStr, kCFURLPOSIXPathStyle, TRUE);

    // Open the file URL:
    result = LSOpenCFURLRef(cfurl, NULL) == noErr;

    // Release the created resources:
    CFRelease(cfurlStr);
    CFRelease(cfurl);
    #else
    result = (system(("xdg-open " + path + " &").c_str()) == 0);
    #endif
    return result;
}

#if defined(_WIN32) || defined(__MINGW32__)
// should be ecl_system_windows.cc ?
std::string ApplicationDataPath() {
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

std::string ToLowerCase(const std::string &filename) {
    std::string result;
    for (char c : filename)
        result.push_back(static_cast<char>(std::tolower(c)));
    return result;
}


std::set<std::string> UniqueFilenameSet(const std::set<std::string>& inSet) {
    std::set<std::string> result;
    std::set<std::string> lowerSet;
    for (const std::string& fileName : inSet) {
        std::string lowerCaseName = ToLowerCase(fileName);
        auto [it, success] = lowerSet.insert(lowerCaseName);
        if (success)
            result.insert(fileName);
    }
    return result;
}
#endif


std::string GetLanguageCode(const std::string &ln) {
    if (ln == "C" || ln == "POSIX" || ln.size() < 2)
        return "en";

    return ln.substr(0, 2);
}

} // namespace ecl