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
#include "Level.h"

using namespace std;
using namespace OxydLib;

void usage()
{
  printf("Usage:\n");
  printf("  printlevels [options] <datfile>\n");
  printf("\n");
  printf("Description:\n");
  printf("  The printlevels program parses the Oxyd data file specified by <datfile>,\n");
  printf("  and displays some information about each level.  The contents of the\n");
  printf("  surface, piece, and object grids are displayed, as is the table of\n");
  printf("  signals.\n");
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

  for (int nLevel = 0; nLevel < 200; nLevel++) {
    printf("Level %d:\n", nLevel + 1);

    Level level;
    success = parseLevel(datFile.getLevel(nLevel), &level, &msg);
    if (!success) {
      printf("Error parsing level:\n");
      printf("%s\n", msg.c_str());
      printf("\n");
      continue;
    }

    if (level.isEmpty()) {
      printf("Level is empty.\n");
      printf("\n");
      continue;
    }

    {
      for (int gridTypeInt = GridType_First;
           gridTypeInt <= GridType_Last;
           gridTypeInt++) {
        GridType gridType = GridType(gridTypeInt);

        switch (gridType) {
        case GridType_Surfaces: printf("Surfaces:\n"); break;
        case GridType_Pieces:   printf("Pieces:\n");   break;
        case GridType_Objects:  printf("Objects:\n");  break;
        default: exit(1);
        }

        for (unsigned int y = 0; y < level.getHeight(); y++) {
          for (unsigned int x = 0; x < level.getWidth(); x++) {
            unsigned char byteVal = level.getGrid(gridType).get(x, y);
            if (x > 0) {
              printf(" ");
            }
            printf("%02x", byteVal);
          }
          printf("\n");
        }

        printf("\n");
      }
    }

    {
      printf("Signals:\n");

      set<SignalLocation> senders;
      level.getSenders(&senders);
      set<SignalLocation>::const_iterator senderIter = senders.begin();
      set<SignalLocation>::const_iterator senderEnd = senders.end();
      for (; senderIter != senderEnd; ++senderIter) {
        const SignalLocation &sender = *senderIter;
        printf("  %s (%3u, %3u)\n",
               sender.getGridType() == GridType_Pieces ? "Piece "
               : (sender.getGridType() == GridType_Surfaces ? "Surface" : "Object"),
               sender.getX(),
               sender.getY());

        int numRecipients = level.getNumRecipients(sender);
        for (int nRecipient = 0;
             nRecipient < numRecipients;
             nRecipient++) {
          const SignalLocation &recipient =
            level.getRecipient(sender, nRecipient);
          printf("   -> %s (%3u, %3u)\n",
                 recipient.getGridType() == GridType_Pieces ? "Piece "
                 : (recipient.getGridType() == GridType_Surfaces ? "Surface" : "Object"),
                 recipient.getX(),
                 recipient.getY());
        }
      }

      printf("\n");
    }
  }

  return 0;
}
