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
 * $Id: system.hh,v 1.1 2003/10/27 11:45:48 reallysoft Exp $
 */
#ifndef SYSTEM_HH
#define SYSTEM_HH

#include <string>
#include <ctime>

namespace ecl
{
    extern const char *PathSeparator;

    std::string ExpandPath (const std::string &path);

    bool        FileExists (const std::string &fname);
    std::time_t FileModTime (const std::string &fname);
        
    bool FolderExists (const std::string &fname);
    bool FolderCreate (const std::string &fname);

/* -------------------- Locales -------------------- */

    std::string DefaultMessageLocale ();

    std::string GetLanguageCode (const std::string &localename);
}
#endif
