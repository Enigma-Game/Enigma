/*
 * Copyright (C) 2006 Ronald Lamprecht
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
 */

// Whoever knows how to integrate this code into file.cc should do it!
// The problem to solve is the clashing macro definitions in "zipios-config.h"
// and "config.h"
// The static var "zip" should be a private ivar of GameFS

#include "file.hh"
#include "zipios++/zipfile.h"
#include <istream>

using namespace enigma;
using namespace std;

static std::auto_ptr<zipios::ZipFile> zip;
static std::string lastZipPath;

        
bool enigma::findInZip(std::string zipPath, std::string zippedFilename1,
        std::string zippedFilename2, string &dest, 
        std::auto_ptr<std::istream> &isresult) {

        // reuse last opened zip if possible
        if (lastZipPath != zipPath) {
             zip.reset (new zipios::ZipFile (zipPath));
             lastZipPath = zipPath;
        }
        std::auto_ptr<istream> isptr (zip->getInputStream (zippedFilename2));
        if(isptr.get() != 0) {
            isresult = isptr;
            dest = zippedFilename2;
            return true;
        }
        isptr.reset(zip->getInputStream (zippedFilename1));
        if(isptr.get() != 0) {
            isresult = isptr;
            dest = zippedFilename1;
            return true;
        }
        return false;
}
