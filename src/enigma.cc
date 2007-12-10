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
#include "objects.hh"
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


/* -------------------- Value implementation -------------------- */

Value::Value() : type (NIL) {
}

Value::Value(const char* str) : type (STRING) {
    val.str = new char[strlen(str)+1];
    strcpy(val.str, str);
}

Value::Value(double d) : type (DOUBLE) {
     val.dval[0] = d;
}

Value::Value(int i) : type (DOUBLE) {
     val.dval[0] = i;
}

Value::Value(bool b) : type (BOOL) {
     val.dval[0] = b;
}

Value::Value(Object *obj) : type (OBJECT) {
     if (obj != NULL)
         val.dval[0] = obj->getId();
     else
         val.dval[0] = 0;
}

Value::Value(ObjectList aList) : type (GROUP) {
    std::string descriptor;
    ObjectList::iterator it;
    for (it = aList.begin(); it != aList.end(); ++it) {
        if (*it == NULL)
            descriptor.append("$0,");
        else {
            Value v = (*it)->getAttr("name");
            if (v && v.type == STRING && strcmp(v.val.str, "") != 0) {
                descriptor.append(v);
                descriptor.append(",");
            } else {
                descriptor.append(ecl::strf("$%d,", (*it)->getId()));
            }   
        }
    }
    val.str =  new char[descriptor.size() + 1];
    strcpy(val.str, descriptor.c_str());
//    Log << "Value ObjectList '" << descriptor << "'\n";
}

Value::Value(TokenList aList) : type (TOKENS) {
    std::string descriptor;
    TokenList::iterator it;
    for (it = aList.begin(); it != aList.end(); ++it) {
        switch ((*it).type) {
            case STRING :
                ASSERT((*it).val.str[0] != 0, XLevelRuntime, "TokenList: illegal empty string value");                
                descriptor.append((*it).val.str);
                break;
            case OBJECT :
                descriptor.append(ecl::strf("$%d", (int)((*it).val.dval[0])));
                break;
            case GROUP :
                descriptor.append("%");
                descriptor.append((*it).val.str);
                break;
            default :
                ASSERT(false, XLevelRuntime, "TokenList: illegal value type");
                break;
        }
        descriptor.append(";");
    }
    val.str =  new char[descriptor.size() + 1];
    strcpy(val.str, descriptor.c_str());
 
//    Log << "Value TokenList '" << descriptor << "'\n";
}

Value::Value(ecl::V2 pos) : type (POSITION) {
     val.dval[0] = pos[0];
     val.dval[1] = pos[1];
}

Value::Value(GridPos gpos) : type (POSITION) {
     val.dval[0] = gpos.x;
     val.dval[1] = gpos.y;
}

Value::Value(Type t) : type (t) {
    switch (t) {
        case POSITION :
            val.dval[1] = 0;
            // fall thorough
        case DOUBLE :
            val.dval[0] = 0;
            break;
        case STRING :
        case GROUP :
        case TOKENS :
            val.str = new char[1];
            val.str[0] = 0;
            break;
        case BOOL :
            val.dval[0] = false;
            break;
        case OBJECT :
            val.dval[0] = (double) NULL;
            break;
    }
}

Value::~Value() { 
    clear(); 
}


Value::Value(const string& str) : type(STRING) {
    val.str = new char[str.length()+1];
    strcpy(val.str, str.c_str());
}

Value::Value (const Value& other) : type(NIL) {
    this->operator=(other);
}

Value& Value::operator= (const Value& other) {
    if (this != &other) {
        switch (other.type) {
            case STRING:
                assign(other.val.str);
                break;
            case GROUP:
                assign(other.val.str);
                type = GROUP;
                break;
            default:
                clear();
                type = other.type;
                val = other.val;
        }
    }
    return *this;
}

bool Value::operator==(const Value& other) const {
    if (type != other.type)
        return false;
    else
        switch (type) {
            case DOUBLE :
            case BOOL :
            case OBJECT :
                return val.dval[0] == other.val.dval[0];
            case STRING :
            case GROUP :
            case TOKENS :
                return strcmp(val.str, other.val.str) == 0;
            case POSITION :
                return (val.dval[0] == other.val.dval[0]) && (val.dval[1] == other.val.dval[1]);
        }
    return true;
}

bool Value::operator!=(const Value& other) const {
    return ! (*this == other);
}

bool Value::operator==(int i) const {
    return (int) *this == i;
}

bool Value::operator!=(int i) const {
    return (int) *this != i;
}

Value::operator bool() const {
    if (isDefault())
        return false;
    else
        return true;
}

Value::operator double() const {
    switch (type) {
        case DOUBLE: 
        case BOOL: 
            return val.dval[0];
        case STRING:
            return atof(val.str);  // TODO use strtod and eval remaining part of string
        default:
            return 0.0;
    }
}

Value::operator int() const {
    switch (type) {
        case DOUBLE:
        case BOOL: 
            return round_nearest<int>(val.dval[0]);
        case STRING: 
            return atoi(val.str);  //TODO use strtol and eval remaining part of string
        default: return 0;
    }
}

Value::operator Object *() const {
    switch (type) {
        case OBJECT:
            return Object::getObject(round_nearest<int>(val.dval[0]));
        case STRING:
            return GetNamedObject(val.str);            
        default: 
            return NULL;
    }
}

Value::operator ObjectList() const {
    ObjectList result;
    switch (type) {
        case STRING:
            if (std::string(val.str).find_first_of("*?") != std::string::npos) {
                // wildcards in object name - we need to add all objects
                result = GetNamedGroup(val.str);
                break;
            }
            // else it is a single object name - fall through
        case OBJECT:
            result.push_back(*this);
            break;
        case GROUP:
            std::vector<std::string> vs;
            ecl::split_copy(std::string(val.str), ',', back_inserter(vs));
            for (std::vector<std::string>::iterator it = vs.begin(); it != vs.end(); ++it) {
                if (it->size() > 0) {
                    if ((*it)[0] == '$') {
                        result.push_back(Object::getObject(atoi((*it).c_str() + 1)));
                    } else {
                        result.push_back(GetNamedObject(*it));
                    }
                }
            }
            break;
    }
    return result;
}

Value::operator TokenList() const {
    TokenList result;
    switch (type) {
        case OBJECT:
        case STRING:
        case GROUP:
            result.push_back(*this);
            break;
        case TOKENS:
            std::vector<std::string> vs;
            ecl::split_copy(std::string(val.str), ';', back_inserter(vs));
            for (std::vector<std::string>::iterator it = vs.begin(); it != vs.end(); ++it) {
                if (it->size() > 0) {
                    if ((*it)[0] == '$') {
                        // an object id
                        Value v(OBJECT);
                        v.val.dval[0] = atoi((*it).c_str() + 1);
                        result.push_back(v);
                    } else if ((*it)[0] == '%'){
                        // a group
                        Value v(NIL);
                        v.assign((*it).c_str() + 1);
                        v.type = GROUP;
                        result.push_back(v);
                    } else {
                        // a string
                        result.push_back(Value(*it));
                    }
                }
            }
            break;
    }
    return result;
}

Value::operator ecl::V2() const {
    switch (type) {
        case POSITION:
            return ecl::V2(val.dval[0], val.dval[1]);
        default:
            return ecl::V2(0, 0);
    }
}

Value::operator GridPos() const {
    switch (type) {
        case POSITION:
            return GridPos(round_down<int>(val.dval[0]), round_down<int>(val.dval[1]));
        default:
            return GridPos(0, 0);
    }
}

Value::operator const char*() const {
    static std::string s;
    switch (type) {
        case Value::DOUBLE:
            s = ecl::strf("%g", val.dval[0]);  // need drop of trailing zeros and point for int
            return s.c_str();
        case Value::STRING: 
            return val.str;
        case Value::NIL:
        case Value::DEFAULT:
        default: 
            s.clear();
            return s.c_str();
    }
}

void Value::assign(const char* s) {
    clear();
    type = STRING;
    val.str = new char[strlen(s)+1];
    strcpy(val.str, s);
}

void Value::assign(double d) { 
    clear(); type=DOUBLE; val.dval[0]=d; 
}

void Value::clear() {
    switch (type) {
        case STRING:
        case GROUP:
    	   delete[] val.str;
           break;
    }
    type = NIL;
}

Value::Type Value::getType() const {
    return type;
}

double Value::get_double() const throw(){
    ASSERT(type == DOUBLE, XLevelRuntime, "get_double: type not double");
    return val.dval[0];
}

const char* Value::get_string() const throw() {
    ASSERT(type == STRING, XLevelRuntime, "get_string: type not string");
    return val.str;
}

bool Value::isDefault() const {
    return type == DEFAULT;
}

std::string Value::to_string() const{
    return std::string(*this);
}

bool Value::to_bool() const {
    switch (type) {
        case BOOL :
            return val.dval[0];
        case NIL :
        case DEFAULT :
            return false;
        default :
            return true;
    }
}


int to_int(const Value &v) {
    return v;
}

bool to_bool(const Value &v) {
    return v.to_bool();
}

double to_double(const Value &v) {
    return v;
}

std::string to_string(const Value &v) {
    return v.to_string();
}

Direction to_direction (const Value &v) {
    int val = Clamp(to_int(v), 0, 3);
    return static_cast<Direction>(val);
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
