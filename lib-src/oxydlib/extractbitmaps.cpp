// Copyright 2003 Jeremy Sawicki
//
// This file is part of OxydLib.
//
// OxydLib is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// OxydLib is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OxydLib; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include <stdio.h>
#include "OxydVersion.h"
#include <string>
#include "VecUtils.h"
#include "FileUtils.h"
#include "DatFile.h"
#include "Bitmap.h"

using namespace std;
using namespace OxydLib;

void usage()
{
  printf("Usage:\n");
  printf("  extractbitmaps [options] <datfile> [<dir>]\n");
  printf("\n");
  printf("Description:\n");
  printf("  The extractbitmaps program parses the Oxyd data file specified by\n");
  printf("  <datfile> and saves each 16-color bitmap as a .ppm file.  The bitmaps\n");
  printf("  are placed in the directory specified by <dir>, or if no directory is\n");
  printf("  specified, in the current directory.\n");
  printf("\n");
  printf("Options:\n");
  printf("  -v1  Oxyd 1\n");
  printf("  -vm  Oxyd Magnum\n");
  printf("  -vg  Oxyd Magnum Gold\n");
  printf("  -vp  PerOxyd (default)\n");
  printf("  -ve  Oxyd Extra\n");
  printf("\n");
}

int main(int argc, char *argv[])
{
  OxydVersion ver = OxydVersion_PerOxyd;
  bool bGotDatFile = false;
  string strDatFile;
  bool bGotDir = false;
  string strDir;

  int nArg = 1;
  while (nArg < argc) {
    string strArg(argv[nArg]);
    nArg++;

    if (strArg.compare("-v1") == 0) {
      ver = OxydVersion_Oxyd1;
    } else if (strArg.compare("-vm") == 0) {
      ver = OxydVersion_OxydMagnum;
    } else if (strArg.compare("-vg") == 0) {
      ver = OxydVersion_OxydMagnumGold;
    } else if (strArg.compare("-vp") == 0) {
      ver = OxydVersion_PerOxyd;
    } else if (strArg.compare("-ve") == 0) {
      ver = OxydVersion_OxydExtra;
    } else if (!bGotDatFile) {
      strDatFile = strArg;
      bGotDatFile = true;
    } else if (!bGotDir) {
      strDir = strArg;
      bGotDir = true;
    } else {
      usage();
      exit(1);
    }
  }

  if (!bGotDatFile) {
    usage();
    exit(1);
  }

  ByteVec fileData;
  bool success = readFile(strDatFile, &fileData);
  if (!success) {
    printf("Error reading DAT file.\n");
    exit(1);
  }

  DatFile datFile;
  string msg;
  success = parseDatFile(fileData, ver, &datFile, &msg);
  if (!success) {
    printf("Error parsing DAT file:\n");
    printf("%s\n", msg.c_str());
    exit(1);
  }

  set<string> chunkNames;
  datFile.getChunkNames(&chunkNames);

  Palette twoColorPalette;
  twoColorPalette.setNumColors(2);
  twoColorPalette.setColor(0, 0x0000, 0x0000, 0x0000);
  twoColorPalette.setColor(1, 0xffff, 0xffff, 0xffff);

  Palette goodPalette;
  goodPalette.setNumColors(16);
  {
    const ByteVec *pChunk = datFile.getChunk("SPRITEA.PIB");
    if (pChunk) {
      Bitmap bitmap;
      Palette palette;
      if (parseBitmap(*pChunk, &bitmap, &palette)) {
        goodPalette = palette;
      }
    }
  }

  set<string>::const_iterator chunkIter = chunkNames.begin();
  set<string>::const_iterator chunkEnd = chunkNames.end();
  for (; chunkIter != chunkEnd; ++chunkIter) {
    const string &chunkName = *chunkIter;
    const ByteVec &chunk = *datFile.getChunk(chunkName);

    int dotPos = chunkName.rfind('.');
    if (dotPos >= 0) {
      string ext = chunkName.substr(dotPos);
      if (ext.compare(".PIB") == 0) {
        Bitmap bitmap;
        Palette palette;
        success = parseBitmap(chunk, &bitmap, &palette, &msg);
        if (!success) {
          printf("Error parsing bitmap %s:\n", chunkName.c_str());
          printf("%s\n", msg.c_str());
        } else {
          const Palette *pPalette = &palette;
          if (bitmap.getBitDepth() == 1) {
            pPalette = &twoColorPalette;
          } else {
            bool allBlack = true;
            for (int nColor = 0; nColor < 16; nColor++) {
              unsigned int red, green, blue;
              palette.getColor(nColor, &red, &green, &blue);
              if (red != 0x0000 || green != 0x0000 || blue != 0x0000) {
                allBlack = false;
                break;
              }
            }
            if (allBlack) {
              pPalette = &goodPalette;
            }
          }
          ByteVec ppm;
          bitmapToPPM(bitmap, *pPalette, &ppm);

          string outFileName;
          if (bGotDir) {
            outFileName.append(strDir);
            outFileName.append("/");
          }
          outFileName.append(chunkName);
          outFileName.append(".ppm");
          success = writeFile(outFileName, ppm);
          if (!success) {
            printf("Error writing bitmap '%s'.\n", outFileName.c_str());
          }
        }
      }
    }
  }

  return 0;
}
