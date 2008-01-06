/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
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

#include "errors.hh"
#include "enigma.hh"
#include "ecl.hh"
#include "main.hh"
#include "Object.hh"
#include "server.hh"
#include "world.hh"

#include <iostream>
#include <ctime>
#include <set>
#include <vector>

using namespace std;
using namespace ecl;

namespace enigma {
    
/* -------------------- Game Type -------------------- */

static const char *versionName[GAMET_COUNT+1] = {
    "enigma", // same indices as enum GameType
    "oxyd1",
    "per.oxyd",
    "oxyd.extra",
    "oxyd.magnum",
    0
};

GameType GetGameType(std::string name) {
    GameType type = GAMET_UNKNOWN;
    for (int v = 0; v<GAMET_COUNT; ++v) {
        if (0 == strcmp(name.c_str(), versionName[v])) {
            type = GameType(v);
            break;
        }
    }
    return type;
}

std::string GetGameTypeName(GameType type) {
    if (type >= GAMET_FIRST && type <= GAMET_LAST)
        return versionName[type];
    else
        return "unknown";
}

/* -------------------- Direction -------------------- */

Direction reverse(Direction d) {
    static Direction rdir[] = { NODIR, EAST, NORTH, WEST, SOUTH };
    return rdir[d+1];
}

Direction rotate_cw (Direction d)
{
    static Direction rdir[] = { NODIR, NORTH, WEST, SOUTH, EAST };
    return rdir[d+1];
}

Direction rotate_ccw (Direction d)
{
    static Direction rdir[] = { NODIR, SOUTH, EAST, NORTH, WEST };
    return rdir[d+1];
}

Direction
direction_fromto(GridPos source, GridPos target)
{
    // source and target have to be adjacent

    int       dx = target.x-source.x;
    int       dy = target.y-source.y;
    Direction d  = NODIR;

    if (dx == 0) {
        if      (dy == -1) d = NORTH;
        else if (dy ==  1) d = SOUTH;
    }
    else if (dy == 0) {
        if      (dx == -1) d = WEST;
        else if (dx ==  1) d = EAST;
    }

    ASSERT(d != NODIR, XLevelRuntime,
        "direction_fromto: source and target not adjacent");
    return d;
}

string to_suffix(Direction d) {
    static const char *sfx[] = { "", "-w", "-s", "-e", "-n" };
    return sfx[d+1];
}

/* -------------------- DirectionBits -------------------- */

DirectionBits rotate(DirectionBits d, bool clockwise)
{
    if (clockwise) {
        d = DirectionBits(((d>>1) | (d<<3)) & ALL_DIRECTIONS);
    } else {
        d = DirectionBits(((d>>3) | (d<<1)) & ALL_DIRECTIONS);
    }
    return d;
}


/* -------------------- GridPos -------------------- */

GridPos::GridPos(const ecl::V2& p) 
: x (round_down<int>(p[0])), 
  y (round_down<int>(p[1])) 
{}


std::ostream& operator<<(std::ostream& os, const GridPos& val)
{
    return os << '(' << val.x << ',' << val.y << ')';
}

/* 
   516
   203
   748
*/
GridPos get_neighbour (GridPos p, int i)
{
    ASSERT (i >= 0 && i <= 9, XLevelRuntime, "get_neighbour: index out of bounds");
    static int xoff[9] = { 0,0,-1,1,0,-1,1,-1,1 };
    static int yoff[9] = { 0,-1,0,0,1,-1,-1,1,1 };
    return GridPos(p.x + xoff[i], p.y + yoff[i]);
}

/* -------------------- GridLoc -------------------- */

bool to_gridloc (const char *str, GridLoc &l) {
    GridLoc loc;
    const char *numstr = str + 3;

    if (strncmp (str, "fl(", 3) == 0)
        loc.layer = GRID_FLOOR;
    else if (strncmp (str, "it(", 3) == 0)
        loc.layer = GRID_ITEMS;
    else if (strncmp (str, "st(", 3) == 0)
        loc.layer = GRID_STONES;
    else
        numstr = str;
    
    if (2 != sscanf (numstr, "%d %d", &loc.pos.x, &loc.pos.y))
        return false;
    l = loc;
    return true;
}



/* -------------------- Random numbers -------------------- */

int32_t  SystemRandomState;

void Randomize(bool isLevel) {
    time_t seed = time(NULL) & 0x7fffffff;
    Randomize(seed, isLevel);
}

void Randomize(unsigned seed, bool isLevel) {
    if (isLevel) {
        server::RandomState = seed;
    } else {
        srand (seed);
        SystemRandomState =  seed;
    }
}

int Rand(bool isLevel) {
    int32_t rand = isLevel ? server::RandomState : SystemRandomState;
    // This is the 31 bit BSD rand generator. Its sequence is  x_{n+1} = (a x_n + c) mod m
    // with a = 1103515245, c = 12345 and m = 2^31. The seed specifies the initial value, x_1. 
    // The period of this generator is 2^31, and it uses 1 word of storage per generator. 
    rand = ((rand * 1103515245) + 12345) & 0x7fffffff;
    if (isLevel)
        server::RandomState = rand;
    else
        SystemRandomState = rand;
    return rand;
}

int IntegerRand(int min, int max, bool isLevel) {
    int r = round_down<int>((max-min+1) * (Rand(isLevel)/(ENIGMA_RAND_MAX+1.0)));  // 1.0 converts to double to avoid int overflow!
    return r+min;
}

double DoubleRand(double min, double max, bool isLevel) {
    return min + double(Rand(isLevel))/ENIGMA_RAND_MAX * (max-min);
}


/* -------------------- Time & Date -------------------- */

#define MAX_DATE_LENGTH 256
const char *date(const char *format) { // format see 'man strftime'
    static char *result = 0;
    char         buffer[MAX_DATE_LENGTH];

    time_t t;
    time(&t);

    struct tm *tm  = localtime(&t);
    strftime(buffer, MAX_DATE_LENGTH, format, tm);

    if (result) free(result);
    result = strdup(buffer);

    return result;
}


/* -------------------- Resource management -------------------- */

namespace
{
    struct FontDescr {
        // Variables
        string name;
        string ttf_name;
        int    ttf_size;
        string bitmap_name;
        int r, g, b;

        // Constructor
        FontDescr (const string &name_,
                   const string &ttf_name_,
                   int ttf_size_,
                   const string &bitmap_name_, 
                   int r_, int g_, int b_)
        : name (name_),
          ttf_name (ttf_name_),
          ttf_size (ttf_size_),
          bitmap_name (bitmap_name_), 
          r (r_), g(g_), b(b_)
        {}
    };

    class FontCache : public PtrCache<Font> {
    public:
        Font *acquire (const std::string &name) {
            Font *f = 0;
            if (m_fonts.has_key (name)) {
                const FontDescr &fd = m_fonts[name];
                f = load_ttf (fd.ttf_name, fd.ttf_size, fd.r, fd.g, fd.b);
                if (f == 0) {
                    std::cerr << "Could not load .ttf file " << fd.ttf_name << "\n";
                    f = load_bmf (fd.bitmap_name);
                }
            }
            else {
                f = load_bmf (name);
            }
            return f;
        }

        void define_font (const FontDescr &descr) {
            remove (descr.name); // remove entry in cache (if any)
            if (m_fonts.has_key (descr.name))
                m_fonts[descr.name]= descr;
            else
                m_fonts.insert (descr.name, descr);
        }
        
        void clear() {
            PtrCache<Font>::clear();  // crazy double cache - TODO cleanup 
            m_fonts.clear();
        }

    private:

        Font *load_bmf (const string &name) {
            string png, bmf;
            if (app.resourceFS->findFile(string("fonts/")+name+".png", png) &&
                app.resourceFS->findFile(string("fonts/")+name+".bmf", bmf))
            {
                return ecl::LoadBitmapFont(png.c_str(), bmf.c_str());
            }
            return 0;
        }

        Font *load_ttf (const string &name, int ptsize, int r, int g, int b) {
            string ttf;
            if (app.resourceFS->findFile(string("fonts/") + name, ttf))
                return ecl::LoadTTF (ttf.c_str(), ptsize, r, g, b);
            return 0;
        }

        // Variables
        ecl::Dict <FontDescr> m_fonts;
    };

    // ---------- Variables ----------

    FontCache  font_cache;
    ImageCache image_cache;
}

ecl::Surface *ImageCache::acquire (const std::string &name) 
{
    return ecl::LoadImage(name.c_str());
}

void DefineFont (const char *name, 
                         const char *ttf_name, 
                         int ttf_size,
                         const char *bmf_name,
                         int r, int g, int b)
{
    font_cache.define_font (FontDescr (name, ttf_name, ttf_size, bmf_name, r, g, b));
}

ecl::Font *GetFont (const char *name) 
{
    return font_cache.get(name);
}

void ClearFontCache() {
    font_cache.clear();
}

ecl::Surface *LoadImage(const char *name) 
{
    string filename;
    if (app.resourceFS->findImageFile (string(name) + ".png", filename)) 
        return ecl::LoadImage(filename.c_str());
    return 0;
}

ecl::Surface *GetImage(const char *name, const char *ext) 
{
    string filename;
    if (app.resourceFS->findImageFile (string(name) + ext, filename)) 
        return image_cache.get(filename);
    return 0;
}

ecl::Surface *RegisterImage (const char *name, ecl::Surface *s) 
{
    image_cache.store(name, s);
    return s;
}

void ClearImageCache() {
    image_cache.clear();
}

} // namespace enigma
