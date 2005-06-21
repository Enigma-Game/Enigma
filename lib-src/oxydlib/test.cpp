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

#include "DatFile.h"
#include "FileUtils.h"
#include "VecUtils.h"
#include "Level.h"
#include "Bitmap.h"
#include <stdio.h>

void parseUnparseDat(char *inFile, char *outFile, OxydVersion ver)
{
  ByteVec fileData;

  printf("%s... \n", inFile);
  bool success = readFile(inFile, &fileData);
  if (!success) {
    printf("read failed.\n");
    return;
  }

  DatFile datFile;
  string msg;
  success = parseDatFile(fileData, ver, &datFile, &msg);
  if (success) {
    printf("parse succeeded.\n");
  } else {
    printf("parse failed.\n");
    printf("Error: %s\n", msg.c_str());
    return;
  }

#if 0
  for (int nUserLevel = 20; nUserLevel < 80; nUserLevel++) {
    int nLevel = nUserLevel - 1;
    if (nUserLevel % 20 > 15) {
      continue;
    }

    Level level;
    level.setInit(true);
    level.setEmpty(false);
    level.resize(20, 11);

    level.setNumMarbles(1);
    Marble *pMarble = level.getMarbleForWrite(0);
    pMarble->setMarbleType(MarbleType_Black);
    pMarble->setX(48);
    pMarble->setY(48);

    Grid *pSurfaces = level.getGridForWrite(GridType_Surfaces);
    Grid *pPieces = level.getGridForWrite(GridType_Pieces);
    Grid *pObjects = level.getGridForWrite(GridType_Objects);

    unsigned int x, y;
    for (y = 0; y < level.getHeight(); y++) {
      for (x = 0; x < level.getWidth(); x++) {
	pSurfaces->set(x, y, 0x09);
	pPieces->set(x, y, 0x00);
	pObjects->set(x, y, 0x00);
      }
    }
    for (y = 0; y < level.getHeight(); y++) {
      pPieces->set(0, y, 0x1c);
      pPieces->set(level.getWidth() - 1, y, 0x1c);
    }
    for (x = 0; x < level.getWidth(); x++) {
      pPieces->set(x, 0, 0x1c);
      pPieces->set(x, level.getHeight() - 1, 0x1c);
    }

    int i;
    for (i = ((nUserLevel % 20) * 16) + 0x00;
	 i <= ((nUserLevel % 20) * 16) + 0x0f;
	 i++) {
      if (i >= 0xfd) {
	continue;
      }

      unsigned char b = (unsigned char)i;

      x = (i & 0x07) * 2 + 3;
      y = ((i & 0x08) >> 3) * 4 + 3;

      Grid *pGrid = 0;

      int nGrid = nUserLevel / 20;
      if (nGrid == 1) {
	if (i >= 0x60) {
	  continue;
	}
	pGrid = pSurfaces;
      } else if (nGrid == 2) {
	if (i >= 0xbf) {
	  continue;
	}
	pGrid = pPieces;
      } else if (nGrid == 3) {
	if (i >= 0x7a) {
	  continue;
	}
	pGrid = pObjects;
      }

      pGrid->set(x, y, b);
      //pGrid->set(x+1, y, b);
      //pGrid->set(x, y+1, b);
      //pGrid->set(x+1, y+1, b);
    }

    //level.getSpecialItemsForWrite()->push_back("g(0)()(0)");
    //level.getSpecialItemsForWrite()->push_back("r");

    ByteVec newLevelData;
    success = unparseLevel(level, &newLevelData, &msg);
    if (!success) {
      printf("Problem unparsing level: %s", msg.c_str());
    } else {
      ByteVec *pLevelData = datFile.getLevelForWrite(nLevel);
      pLevelData->resize(newLevelData.size());
      copy(newLevelData.begin(), newLevelData.end(), pLevelData->begin());
    }
  }
#endif

#if 0
  {
    Level level;
    level.setInit(true);
    level.setEmpty(false);
    level.resize(20, 11);

    level.setHarmlessMeditationMarbles(true);

    level.setNumMarbles(2);
    {
      Marble *pMarble = level.getMarbleForWrite(0);
      pMarble->setMarbleType(MarbleType_Black);
      pMarble->setX(48);
      pMarble->setY(272);
    }
    {
      Marble *pMarble = level.getMarbleForWrite(1);
      pMarble->setMarbleType(MarbleType_White);
      pMarble->setX(80);
      pMarble->setY(272);
    }
    /*
    {
      Marble *pMarble = level.getMarbleForWrite(2);
      pMarble->setMarbleType(MarbleType_Horse);
      pMarble->setX(112);
      pMarble->setY(48);
    }
    {
      Marble *pMarble = level.getMarbleForWrite(3);
      pMarble->setMarbleType(MarbleType_Meditation);
      pMarble->setX(144);
      pMarble->setY(48);
    }
    */

    Grid *pSurfaces = level.getGridForWrite(GridType_Surfaces);
    Grid *pPieces = level.getGridForWrite(GridType_Pieces);
    Grid *pObjects = level.getGridForWrite(GridType_Objects);

    unsigned int x, y;
    for (y = 0; y < level.getHeight(); y++) {
      for (x = 0; x < level.getWidth(); x++) {
	pSurfaces->set(x, y, 0x01);
	pPieces->set(x, y, 0x00);
	pObjects->set(x, y, 0x00);
      }
    }
    for (y = 0; y < level.getHeight(); y++) {
      //pPieces->set(0, y, 0x1c);
      pPieces->set(level.getWidth() - 1, y, 0x1c);
    }
    for (x = 0; x < level.getWidth(); x++) {
      pPieces->set(x, 0, 0x1c);
      pPieces->set(x, level.getHeight() - 1, 0x1c);
    }

    pPieces->set(level.getWidth() - 1, level.getHeight() - 1, 0x00);
    pSurfaces->set(level.getWidth() - 1, level.getHeight() - 1, 0x00);

    for (x = 1; x < level.getWidth() - 2; x++) {
      pPieces->set(x, 1, 0x2e);
      pPieces->set(x, 3, 0x73);
      pPieces->set(x, 5, 0x2b);
      pPieces->set(x, 7, 0x7c);
      level.addRecipient(SignalLocation(x, 1, GridType_Pieces),
			 SignalLocation(x, 3, GridType_Pieces));
      level.addRecipient(SignalLocation(x, 1, GridType_Pieces),
			 SignalLocation(x, 5, GridType_Pieces));
      level.addRecipient(SignalLocation(x, 1, GridType_Pieces),
			 SignalLocation(x, 7, GridType_Pieces));
      {
	string str;
	str.append("~(");
	{
	  char buf[1000];
	  sprintf(buf, "%d", x + level.getWidth());
	  str.append(buf);
	}
	str.append(")(");
	{
	  char buf[1000];
	  sprintf(buf, "%d", x);
	  str.append(buf);
	}
	str.append(")");
	level.getSpecialItemsForWrite()->push_back(str);
      }
    }

    ByteVec newLevelData;
    success = unparseLevel(level, &newLevelData, &msg);
    if (!success) {
      printf("Problem unparsing level: %s", msg.c_str());
    } else {
      ByteVec *pLevelData = datFile.getLevelForWrite(101 - 1);
      pLevelData->resize(newLevelData.size());
      copy(newLevelData.begin(), newLevelData.end(), pLevelData->begin());
    }
  }
#endif

#if 1
  for (int nLevel = 0; nLevel < 200; nLevel++) {
    printf("Level %d:\n", nLevel + 1);
    Level level;
    success = parseLevel(datFile.getLevel(nLevel), &level, &msg);
    if (!success) {
      printf("Parse error: %s\n", msg.c_str());
    } else {
      /*
      if (nLevel + 1 == 35) {
	for (int y = 0; y < level.getHeight(); y++) {
	  for (int x = 0; x < level.getWidth(); x++) {
	    level.getSurfacesForWrite()->set(x, y, 0x51);
	  }
	}
      }
      */
      success = unparseLevel(level, datFile.getLevelForWrite(nLevel), &msg);
      if (!success) {
	printf("Unparse error: %s\n", msg.c_str());
      } else {
	/*
	for (int nItem = 0;
	     nItem < (int)level.getSpecialItems().size();
	     nItem++) {
	  printf("  %s\n", level.getSpecialItems()[nItem].c_str());
	}
	*/
      }
    }
  }
#endif

#if 0
  set<string> chunkNames;
  datFile.getChunkNames(&chunkNames);
  printf("Number of chunks: %d\n", chunkNames.size());
  set<string>::const_iterator chunkIter = chunkNames.begin();
  set<string>::const_iterator chunkEnd = chunkNames.end();
  for (; chunkIter != chunkEnd; ++chunkIter) {
    const string &chunkName = *chunkIter;
    //if (nChunk != 63) { continue; }
    printf("Chunk '%s'\n", chunkName.c_str());
    ByteVec &chunk = *datFile.getChunkForWrite(chunkName);
    if (chunkName.find(".PIB") == chunkName.size() - 4) {
      Bitmap bitmap;
      Palette palette;
      printf("Parsing bitmap.\n");
      success = parseBitmap(chunk, ver, &bitmap, &palette, &msg);
      if (!success) {
	printf("Bitmap parse error: %s\n", msg.c_str());
      } else {
	printf("Unparsing bitmap.\n");
	success = unparseBitmap(bitmap, palette, ver, &chunk, &msg);
	if (!success) {
	  printf("Bitmap unparse error: %s\n", msg.c_str());
	} else {
	}
      }
    }
  }
#endif

  success = unparseDatFile(datFile, &fileData, &msg);
  if (success) {
    printf("dat file unparsed.\n");
  } else {
    printf("dat file unparse failed.\n");
    printf("Error: %s\n", msg.c_str());
    return;
  }

  success = writeFile(outFile, fileData);
  if (!success) {
    printf("Error write dat file.\n");
  }
}

int main()
{
  cout << "Hello, world!\n\n";

  parseUnparseDat("peroxyd.bak",
		  "peroxyd.pup",
		  OxydVersion_PerOxyd);

  return 0;
}
