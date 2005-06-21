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
#include <cassert>
#include "VecUtils.h"
#include "FileUtils.h"
#include "DatFile.h"
#include "Level.h"

#include "EnigmaNames.cpp"

using namespace std;
using namespace OxydLib;
using namespace enigma_names;

void usage()
{
  printf("Usage:\n");
  printf("  printstats [options] <datfile>\n");
  printf("\n");
  printf("Description:\n");
  printf("  The printstats program parses the Oxyd data file specified by <datfile>\n");
  printf("  and displays statistics about the contents of the levels.  For each\n");
  printf("  type of surface, piece, or object, printstats displays the level\n");
  printf("  numbers where it is used, as well as the level numbers where it is used\n");
  printf("  as a signal sender or recipient (pieces and objects only).\n");
  printf("\n");
  printf("Options:\n");
  printf("  -v1  Oxyd 1\n");
  printf("  -vm  Oxyd Magnum\n");
  printf("  -vg  Oxyd Magnum Gold\n");
  printf("  -vp  PerOxyd (default)\n");
  printf("  -ve  Oxyd Extra\n");
  printf("  -n   Use enigma names for objects\n");
  printf("\n");
}

void printObjectValue(int num, Mapping *m, GridType type) {
    if (m) {
        static int max_length[GridType_Count] = { -1, -1, -1 };

        if (max_length[type]<0) {
            for (int t = GridType_First; t <= GridType_Last; ++t) {
                const char *(Mapping::*getName)(int) const = 0;

                switch (t) {
                    case GridType_Surfaces: getName = &Mapping::floorName; break;
                    case GridType_Pieces:   getName = &Mapping::stoneName; break;
                    case GridType_Objects:  getName = &Mapping::itemName; break;
                    default : assert(0); break;
                }

                int max = -1;
                for (int i = 0; i<256; ++i) {
                    const char *name = (m->*getName)(i);
                    if (name) {
                        int len          = strlen(name);
                        if (len>max) max = len;
                    }
                }
                max_length[t] = max;
            }
        }

        const char *name = 0;

        switch (type) {
            case GridType_Surfaces: name = m->floorName(num); break;
            case GridType_Pieces:   name = m->stoneName(num); break;
            case GridType_Objects:  name = m->itemName(num); break;
            default : assert(0); break;
        }
        printf("%-*s ", max_length[type], name ? name : "");
    }
    printf("%02x:", num);
}

int main(int argc, char *argv[])
{
  OxydVersion ver            = OxydVersion_PerOxyd;
  bool        bGotDatFile    = false;
  bool        useEnigmaNames = false;
  string      strDatFile;

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
    } else if (strArg.compare("-n") == 0) {
      useEnigmaNames = true;
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

  Mapping *enigmaMapping = 0;
  if (useEnigmaNames) enigmaMapping = new Mapping(ver);

  OxydLib::ByteVec fileData;
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

  set<int> used[GridType_Count][256];
  set<int> usedAsSender[GridType_Count][256];
  set<int> usedAsRecipient[GridType_Count][256];

  for (int nLevel = 0; nLevel < 200; nLevel++) {
    OxydLib::Level level;
    success = parseLevel(datFile.getLevel(nLevel), &level, &msg);
    if (!success) {
      printf("Error parsing level %d:\n", nLevel + 1);
      printf("%s\n", msg.c_str());
      continue;
//       exit(1);
    }

    if (level.isEmpty()) {
      continue;
    }

    {
      for (int gridTypeInt = GridType_First;
           gridTypeInt <= GridType_Last;
           gridTypeInt++) {
        GridType gridType = GridType(gridTypeInt);

        for (unsigned int y = 0; y < level.getHeight(); y++) {
          for (unsigned int x = 0; x < level.getWidth(); x++) {
            unsigned char byteVal = level.getGrid(gridType).get(x, y);
            used[gridType][byteVal].insert(nLevel);
          }
        }

        if (gridType == GridType_Surfaces) {
          continue;
        }
      }
    }

    {
      set<SignalLocation> senders;
      level.getSenders(&senders);
      set<SignalLocation>::const_iterator senderIter = senders.begin();
      set<SignalLocation>::const_iterator senderEnd = senders.end();
      for (; senderIter != senderEnd; ++senderIter) {
        const SignalLocation &sender = *senderIter;

        unsigned char senderByteVal =
          level.getGrid(sender.getGridType()).get(sender.getX(),
                                                  sender.getY());
        usedAsSender[sender.getGridType()][senderByteVal].insert(nLevel);

        int numRecipients = level.getNumRecipients(sender);
        for (int nRecipient = 0;
             nRecipient < numRecipients;
             nRecipient++) {
          const SignalLocation &recipient =
            level.getRecipient(sender, nRecipient);

          unsigned char recipientByteVal =
            level.getGrid(recipient.getGridType()).get(recipient.getX(),
                                                       recipient.getY());
          usedAsRecipient[recipient.getGridType()][recipientByteVal]
            .insert(nLevel);
        }
      }
    }
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

      for (int byteValInt = 0; byteValInt < 256; byteValInt++) {
        printObjectValue(byteValInt, enigmaMapping, gridType);
        // printf("%02x:", byteValInt);

        const set<int> &intSet = used[gridType][byteValInt];
        set<int>::const_iterator iter = intSet.begin();
        set<int>::const_iterator end = intSet.end();
        for (; iter != end; ++iter) {
          int nLevel = *iter;
          printf(" %d", nLevel + 1);
        }

        printf("\n");
      }

      printf("\n");
    }
  }

  {
    for (int gridTypeInt = GridType_First;
         gridTypeInt <= GridType_Last;
         gridTypeInt++) {
      GridType gridType = GridType(gridTypeInt);

      if (gridType == GridType_Surfaces) {
        continue;
      }

      for (int useType = 1; useType <= 2; useType++) {
        switch (gridType) {
        case GridType_Pieces:   printf("Pieces (");   break;
        case GridType_Objects:  printf("Objects (");  break;
        default: exit(1);
        }

        switch (useType) {
        case 1: printf("senders"); break;
        case 2: printf("recipients"); break;
        default: exit(1);
        }

        printf("):\n");

        for (int byteValInt = 0; byteValInt < 256; byteValInt++) {
          printObjectValue(byteValInt, enigmaMapping, gridType);
          // printf("%02x:", byteValInt);

          const set<int> &intSet =
            (useType == 1 ? usedAsSender : usedAsRecipient)
            [gridType][byteValInt];
          set<int>::const_iterator iter = intSet.begin();
          set<int>::const_iterator end = intSet.end();
          for (; iter != end; ++iter) {
            int nLevel = *iter;
            printf(" %d", nLevel + 1);
          }

          printf("\n");
        }

        printf("\n");
      }
    }
  }

  delete enigmaMapping;

  return 0;
}
