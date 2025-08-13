/*
 * Copyright (C) 2006, 2024 Ronald Lamprecht, Andreas Lochmann
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

// TODO: Integrate into file.cc.
// The original problem (clashing macro definitions in "zipios-config.h") should be fixed now.

#include "file.hh"
#include "errors.hh"
#include <istream>
#include <ostream>
#include <fstream>
#include <ctime>
#include <zlib.h>

using namespace enigma;
using namespace std;

// TODO: The static vars "lastZipPath" and "cachedZipString" should be private ivars of GameFS
static std::string lastZipPath;
static std::string cachedZipString;

bool enigma::findInZip(std::string zipPath, std::string zippedFilename1,
                       std::string zippedFilename2, std::string &dest,
                       std::string &inflatedString) {
    // reuse last opened zip if possible
    if (lastZipPath != zipPath) {
        std::basic_ifstream<char> ifs(zipPath.c_str(), ios::binary | ios::in);
        if(not ifs) // error opening zip file
            return false;
        ByteVec intermediary;
        Readfile(ifs, intermediary);
        cachedZipString = std::string(intermediary.begin(), intermediary.end());
        lastZipPath = zipPath;
    }
    if(extractFromZipString(cachedZipString, zippedFilename2, inflatedString)) {
        dest = zippedFilename2;
        return true;
    }
    if(extractFromZipString(cachedZipString, zippedFilename1, inflatedString)) {
        dest = zippedFilename1;
        return true;
    }
    return false;
}

bool enigma::extractFromZipString(std::string zipString, std::string fileName, std::string &inflatedString) {
    size_t compressedSize = 0;
    size_t uncompressedSize = 0;
    size_t posLocalHeader = 0;
    Bytef *ptrCompressed = NULL;
    Bytef *ptrUncompressed = NULL;
    try {
        // Search for the first occurence of fileName, from back to front.
        size_t pos = zipString.rfind(fileName);
        while(pos != std::string::npos) {
            // get compressed size at pos - 26
            compressedSize = ecl::string_to_uint32(zipString.substr(pos - 26, 4));
            // get uncompressed size at pos - 22
            uncompressedSize = ecl::string_to_uint32(zipString.substr(pos - 22, 4));
            // get position of beginning of local header at pos - 4
            posLocalHeader = ecl::string_to_uint32(zipString.substr(pos - 4, 4));
            // check the signature
            if(zipString.substr(posLocalHeader, 4) == "\x50\x4B\x03\x04")
                // we found it
                break;
            pos = zipString.rfind(fileName, pos - 1);
        }
        if (pos == std::string::npos)
            return false;
        uint32_t fileNameLength = ecl::string_to_uint32(zipString.substr(posLocalHeader + 26, 2));
        ASSERT(fileNameLength == fileName.length(), XFrontend, "Error in zip file.");
        uint32_t extraFieldLength = ecl::string_to_uint32(zipString.substr(posLocalHeader + 28, 2));
        std::string compressedData = zipString.substr(
            posLocalHeader + 30 + fileNameLength + extraFieldLength, uncompressedSize);

        // We need to allocate enough memory to save the inflated (decompressed) xml-file.
        ptrCompressed = (Bytef*)(compressedData.data());
        ptrUncompressed = (Bytef*)(calloc(uncompressedSize, 1));
        ASSERT(ptrUncompressed != NULL, XFrontend, "Could not allocate memory for zip inflation.");

        // Prepare a zlib-stream for inflation.
        z_stream inflateStream;
        inflateStream.zalloc = Z_NULL;
        inflateStream.zfree = Z_NULL;
        inflateStream.opaque = Z_NULL;
        inflateStream.avail_in = compressedSize;
        inflateStream.next_in = ptrCompressed;
        inflateStream.avail_out = uncompressedSize;
        inflateStream.next_out = ptrUncompressed;

        // Inflate using zlib.
        int err = inflateInit2(&inflateStream, -15);
        ASSERT(err != Z_MEM_ERROR, XFrontend, "Not enough memory allocated for inflation.");
        ASSERT(err == Z_OK, XFrontend, "Error during zip inflation init.");
        err = inflate(&inflateStream, Z_FINISH);
        inflatedString = std::string((char*)ptrUncompressed, uncompressedSize);
        //Log << inflatedString << "\n";
        ASSERT(err != Z_NEED_DICT, XFrontend, "Z_NEED_DICT");
        ASSERT(err != Z_DATA_ERROR, XFrontend, "Z_DATA_ERROR");
        ASSERT(err != Z_STREAM_ERROR, XFrontend, "Z_STREAM_ERROR");
        ASSERT(err != Z_MEM_ERROR, XFrontend, "Z_MEM_ERROR");
        ASSERT(err != Z_BUF_ERROR, XFrontend, "Z_BUF_ERROR");
        ASSERT(err != Z_OK, XFrontend, "Z_OK");
        ASSERT(err == Z_OK || err == Z_STREAM_END, XFrontend, "Error during zip inflation.");
        inflateEnd(&inflateStream);
        ASSERT(err != Z_OK, XFrontend, "Z_OK during inflateEnd");
        ASSERT(err == Z_OK || err == Z_STREAM_END, XFrontend, "Error during zip inflation cleanup.");
        ASSERT(uncompressedSize == (int)(inflateStream.next_out - ptrUncompressed), XFrontend,
            "Uncompressed size incorrect. Maybe a broken zip file?");
        inflatedString = std::string((char*)ptrUncompressed, uncompressedSize);
    }
    catch (std::exception &e) {
        if(ptrUncompressed)
            free(ptrUncompressed);
        throw XFrontend("Error unzipping " + fileName + ": " + e.what());
    }
    if(ptrUncompressed)
        free(ptrUncompressed);
    return true;
}
