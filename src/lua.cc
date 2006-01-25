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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "lua.hh"
#include "main.hh"
#include "world.hh"
#include "config.h"
#include "video.hh"
#include "server.hh"
#include "sound.hh"
#include "options.hh"

#include "lua-display.hh"
#include "lua-enigma.hh"
#include "lua-ecl.hh"

extern "C" {
#include "lualib.h"
#include "tolua.h"
}
#include "ecl.hh"
#include <cassert>

using namespace std;
using namespace enigma;
using namespace lua;

using ecl::round_down;
using ecl::strf;

using enigma::GridPos;
using world::Object;
using world::GridObject;
using world::ForceField;

namespace lua
{
    int PlaySoundGlobal (lua_State *L);
    int PlaySound (lua_State *L);
    int EmitSound (lua_State *L);
    int MakeObject (lua_State *L);
}

namespace 
{
    lua_State *level_state = 0;  // level-local state
    int object_tag;             // Lua tag for `Object's
    lua_State *global_state = 0; // global Lua state
}


/* -------------------- Helper routines -------------------- */

using enigma::Value;

void lua::SetTableVar (lua_State *L,
                       const char *tablename, 
                       const char *name, 
                       double value)
{
    lua_getglobal (L, tablename);
    lua_pushstring (L, name);
    lua_pushnumber (L, value);
    lua_rawset (L, -3);
    lua_pop (L, 1);
}

void lua::SetSoundTable (const char *name)
{
    CallFunc (global_state, "ActivateSoundTable", name);
}


static void
push_value(lua_State *L, const Value &val)
{
    switch (val.get_type()) {
    case Value::NIL: lua_pushnil(L); break;
    case Value::DOUBLE: lua_pushnumber(L, to_double(val)); break;
    case Value::STRING: lua_pushstring(L, to_string(val)); break;
    }
}

static Value
to_value(lua_State *L, int idx)
{
    switch (lua_type(L, idx)) {
    case LUA_TNIL: return Value();
    case LUA_TNUMBER: return Value(lua_tonumber(L,idx));
    case LUA_TSTRING: return Value(lua_tostring(L,idx));
    default: lua_error(L,"Cannot convert type to Value.");
    }
    return Value();
}

static bool
is_object(lua_State *L, int idx)
{
    return lua_isuserdata(L,idx) && lua_tag(L,idx)==object_tag;
}

static Object *
to_object(lua_State *L, int idx)
{
    if (lua_isnil(L,idx))
        return 0;

    if (!is_object(L,idx)) {
        lua_error(L, "Cannot convert type to an Object");
        return 0;
    }
    return static_cast<Object*>(lua_touserdata(L,idx));
}

static void
pushobject (lua_State *L, Object *obj)
{
    /* Lua does not allow NULL pointers in userdata variables, so
       convert them manually to `nil' values. */
    if (obj == 0)
        lua_pushnil(L);
    else
        lua_pushusertag(L, obj, object_tag);
}


/* -------------------- Interface routines -------------------- */

int lua::MakeObject (lua_State *L)
{
    const char *name = lua_tostring(L, 1);
    if (!name) {
        lua_error(L, "MakeObject: string expected as argument");
    }
    Object *obj = world::MakeObject(name);
    pushobject(L, obj);
    return 1;
}

static int
en_get_object_template(lua_State *L)
{
    Object *obj = world::GetObjectTemplate(lua_tostring(L, 1));
    pushobject(L, obj);
    return 1;
}

static int
en_set_attrib(lua_State *L)
{
    Object *obj = to_object(L,1);
    const char *key = lua_tostring(L,2);
    if (obj && key)
        obj->set_attrib(key, to_value(L, 3));
    else
        lua_error(L, strf("SetAttrib: invalid object or attribute name '%s'", key).c_str());
    return 0;
}

static int
en_get_attrib(lua_State *L)
{
    Object *obj = to_object(L,1);
    const char *key = lua_tostring(L,2);

    if (!obj) {
        lua_error(L, "GetAttrib: invalid object");
        return 0;
    }
    if (!key) {
        lua_error(L, "GetAttrib: invalid key");
        return 0;
    }

    if (0 == strcmp(key, "kind")) {
        lua_error(L, "GetAttrib: illegal attribute, use GetKind()");
        return 0;
    }

    const Value *v =  obj->get_attrib(key);
    if (!v) {
        lua_error(L, strf("GetAttrib: unknown attribute '%s'", key).c_str());
        lua_pushnil(L);
    }
    else
        push_value(L, *v);
    return 1;
}

static int
en_get_kind(lua_State *L)
{
    Object *obj = to_object(L,1);

    if (!obj) {
        lua_error(L, "GetKind: invalid object");
        return 0;
    }

    push_value(L, Value(obj->get_kind()));
    return 1;
}

static int
en_set_floor(lua_State *L)
{
    int x = round_down<int>(lua_tonumber(L, 1));
    int y = round_down<int>(lua_tonumber(L, 2));
    Floor *fl=0;

    if (lua_isnil(L, 3))
        fl = 0;
    else if (is_object(L,3)) {
        fl = static_cast<Floor*>(lua_touserdata(L,3));
    	if( ! fl)
	    lua_error(L, "object is no valid floor");
    } else
        lua_error(L, "argument 3 must be an Object or nil");
    world::SetFloor(GridPos(x,y), fl);
    return 0;
}

static int
en_set_item(lua_State *L)
{
    int x = round_down<int>(lua_tonumber(L, 1));
    int y = round_down<int>(lua_tonumber(L, 2));
    Item *it = dynamic_cast<Item*>(to_object(L, 3));
    if( ! it)
        lua_error(L, "object is no valid item");
    world::SetItem(GridPos(x,y), it);
    return 0;
}

static int
en_set_stone(lua_State *L)
{
    int x = round_down<int>(lua_tonumber(L, 1));
    int y = round_down<int>(lua_tonumber(L, 2));
    Stone *st = dynamic_cast<Stone*>(to_object(L, 3));
    if( ! st)
        lua_error(L, "object is no valid stone");
    world::SetStone(GridPos(x,y), st);
    return 0;
}

static int en_kill_stone(lua_State *L)
{
    int x = round_down<int>(lua_tonumber(L, 1));
    int y = round_down<int>(lua_tonumber(L, 2));
    world::KillStone(GridPos(x,y));
    return 0;
}

static int en_kill_item(lua_State *L)
{
    int x = round_down<int>(lua_tonumber(L, 1));
    int y = round_down<int>(lua_tonumber(L, 2));
    world::KillItem(GridPos(x,y));
    return 0;
}

static int
en_set_actor(lua_State *L)
{
    double x = lua_tonumber(L,1);
    double y = lua_tonumber(L,2);
    Actor *ac = dynamic_cast<Actor*>(to_object(L, 3));
    if( ! ac)
        lua_error(L, "object is no valid actor");
    world::AddActor(x, y, ac);
    return 0;
}


static int
en_send_message(lua_State *L)
{
    Object     *obj = to_object(L, 1);
    const char *msg = lua_tostring(L, 2);
    if (!msg)
        lua_error(L,"Illegal message");
    else if (obj) {
        try {
            world::SendMessage (obj, msg, to_value(L, 3));
        }
        catch (const enigma_levels::XLevelRuntime &e) {
            lua_error (L, e.what());
        }
        catch (...) {
            lua_error (L, "uncaught exception");
        }
    }
    return 0;
}

int lua::PlaySound (lua_State *L)
{
    const char *soundname = lua_tostring (L, 1);
    double      x         = lua_tonumber (L, 2);
    double      y         = lua_tonumber (L, 3);
    double      volume    = lua_tonumber (L, 4);

    sound::PlaySound (soundname, ecl::V2 (x, y), volume);

    return 0;
}

int lua::PlaySoundGlobal (lua_State *L)
{
    const char *soundname = lua_tostring (L, 1);
    double      volume    = lua_tonumber (L, 2);
    int         priority  = static_cast<int> (lua_tonumber (L, 3));

    sound::PlaySoundGlobal (soundname, volume, priority);

    return 0;
}


int lua::EmitSound (lua_State *L)
{
    Object     *obj       = to_object(L, 1);
    const char *soundname = lua_tostring(L, 2);

    if (!soundname)
        lua_error(L,"Illegal sound");
    else if (obj) {
        GridObject *gobj = dynamic_cast<GridObject*>(obj);
        if (gobj) {
            if (!gobj->sound_event (soundname)) 
                lua_error(L, strf("Can't find sound '%s'", soundname).c_str());
        }
    }
    else
        lua_error(L, "EmitSound: invalid object");

    return 0;
}

static int
en_name_object(lua_State *L)
{
    Object     *obj  = to_object(L, 1);
    const char *name = lua_tostring(L,2);

    if (!obj) 
        lua_error(L, "NameObject: Illegal object");
    else if (!name) 
        lua_error(L, "NameObject: Illegal name");
    else
        world::NameObject(obj, name);

    return 0;
}

static int
en_get_named_object(lua_State *L)
{
    Object *o = world::GetNamedObject(lua_tostring(L,1));
    pushobject(L, o);
    return 1;
}

static int
en_get_floor(lua_State *L)
{
    int x = round_down<int>(lua_tonumber(L, 1));
    int y = round_down<int>(lua_tonumber(L, 2));
    Object *o = world::GetFloor(GridPos(x, y));
    pushobject(L, o);
    return 1;
}
static int
en_get_item(lua_State *L)
{
    int x = round_down<int>(lua_tonumber(L, 1));
    int y = round_down<int>(lua_tonumber(L, 2));
    Object *o = world::GetItem(GridPos(x, y));
    pushobject(L, o);
    return 1;
}
static int
en_get_stone(lua_State *L)
{
    int x = round_down<int>(lua_tonumber(L, 1));
    int y = round_down<int>(lua_tonumber(L, 2));
    Object *o = world::GetStone(GridPos(x, y));
    pushobject(L, o);
    return 1;
}

static int
en_get_pos(lua_State *L)
{
    Object  *obj = to_object(L, 1);
    GridPos  p;

    if (!obj) {
        lua_error(L, "GetPos: Illegal object");
        return 0;
    }

    if (GridObject *gobj = dynamic_cast<GridObject*>(obj))
        p = gobj->get_pos();
    else if (Actor *a = dynamic_cast<Actor*>(obj)) {
        p = GridPos(a->get_pos());
    }
    else
        p = GridPos(-1, -1);

    lua_pushnumber(L, double(p.x));
    lua_pushnumber(L, double(p.y));
    return 2;
}

static int en_add_constant_force(lua_State *L) {
    ecl::V2 v;
    v[0] = lua_tonumber(L, 1);
    v[1] = lua_tonumber(L, 2);
    world::SetConstantForce (v);
    return 0;
}

static int
en_add_rubber_band (lua_State *L)
{
    Actor  *a1       = dynamic_cast<Actor*> (to_object(L, 1));
    Object *o2       = to_object(L, 2);
    Actor  *a2       = dynamic_cast<Actor*>(o2);
    Stone  *st       = dynamic_cast<Stone*>(o2);
    world::RubberBandData d;
    d.strength  = lua_tonumber (L, 3);
    d.length    = lua_tonumber (L, 4);
    d.minlength = lua_tonumber (L, 5);

    if (!a1)
        lua_error(L, "AddRubberBand: First argument must be an actor\n");
    else {
        if (a2)
            world::AddRubberBand (a1, a2, d);
        else if (st)
            world::AddRubberBand (a1, st, d);
        else
            lua_error(L, "AddRubberBand: Second argument must be actor or stone\n");
    }
    return 0;
}

static int
en_get_ticks(lua_State *L)
{
    lua_pushnumber(L, SDL_GetTicks());
    return 1;
}

static int
en_is_solved(lua_State *L)
{
    const char  *levelname = lua_tostring(L,1);
    int          solved    = 0;

    levels::Level level (0, 0);
    if (levels::FindLevel (levelname, level))
        solved = level.is_solved(options::GetInt("Difficulty"));
    else 
        lua_error(L, strf("IsSolved: Unknown level '%s'", levelname).c_str());
    lua_pushnumber(L, solved);
    return 1;
}

static int
en_add_scramble(lua_State *L)
{
    int         x       = round_down<int>(lua_tonumber(L, 1));
    int         y       = round_down<int>(lua_tonumber(L, 2));
    const char *dir     = lua_tostring(L,3);
    const char *allowed = "wsen";
    char       *found   = strchr(allowed, dir[0]);

    if (found && found[0]) 
        world::AddScramble(GridPos(x,y), enigma::Direction(found-allowed));
    else 
        lua_error(L, "AddScramble: Third argument must be one character of \"wsen\"");

    return 0;
}

static int
en_set_scramble_intensity(lua_State *L)
{
    world::SetScrambleIntensity(int(lua_tonumber(L, 1)));
    return 0;
}

static int
en_add_signal (lua_State *L) {
    const char *sourcestr = lua_tostring (L, 1);
    const char *targetstr = lua_tostring (L, 2);
    const char *msg = lua_tostring (L, 3);

    using namespace enigma;
    GridLoc source, target;

    if (sourcestr == 0 || !to_gridloc(sourcestr, source))
        lua_error (L, "AddSignal: invalid source descriptor");
    if (targetstr == 0 || !to_gridloc(targetstr, target)) 
        lua_error (L, "AddSignal: invalid target descriptor");
    if (msg == 0)
        msg = "signal";

    world::AddSignal (source, target, msg);
    return 0;
}

static CFunction globalfuncs[] = {
    {FindDataFile,          "FindDataFile"},
    {lua::PlaySoundGlobal,  "PlaySoundGlobal"},
    {lua::PlaySound,        "PlaySound"},
    {en_get_ticks,             "GetTicks"},
    {0,0}
};

static CFunction levelfuncs[] = {

    // internal functions

    {FindDataFile,          "FindDataFile"},
    {en_get_object_template,"GetObjectTemplate"},
    {lua::MakeObject,       "MakeObject"},
    {en_set_actor,          "SetActor"},

    // finding objects

    {en_get_named_object,   "GetNamedObject"},

    {en_get_floor,          "GetFloor"},
    {en_get_item,           "GetItem"},
    {en_get_stone,          "GetStone"},

    // information from objects

    {en_get_pos,            "GetPos"},
    {en_get_attrib,         "GetAttrib"},
    {en_get_kind,           "GetKind"},

    // manipulating objects

    {en_set_attrib,         "SetAttrib"},
    {en_send_message,       "SendMessage"},
    {en_name_object,        "NameObject"},

    // sound effects

    {lua::EmitSound,        "EmitSound"},
    {lua::PlaySound,        "PlaySound"},

    // manipulating level

    {en_set_floor,          "SetFloor"},
    {en_set_item,           "SetItem"},
    {en_set_stone,          "SetStone"},

    {en_kill_stone,         "KillStone"},
    {en_kill_item,          "KillItem"},


    // signals
    {en_add_signal,         "AddSignal"},

    // access/modify global data

    {en_get_ticks,          "GetTicks"},
    {en_is_solved,          "IsSolved"},

    {en_add_constant_force,     "AddConstantForce"},
    {en_add_rubber_band,        "AddRubberBand"},
    {en_add_scramble,           "AddScramble"},
    {en_set_scramble_intensity, "SetScrambleIntensity"},


    {0,0}
};


/* -------------------- lua:: functions -------------------- */

int lua::FindDataFile (lua_State *L) 
{
    const char *filename = lua_tostring(L, 1);
    string absfile;
    if (app.resourceFS->findFile(filename, absfile)) 
        lua_pushstring(L, absfile.c_str());
    else
        lua_pushnil (L);
    return 1;
}

void lua::RegisterFuncs(lua_State *L, CFunction *funcs) 
{
    lua_getglobal(L, "enigma");
    for (unsigned i=0; funcs[i].func; ++i) {
        lua_pushstring(L, funcs[i].name);
        lua_pushcfunction(L, funcs[i].func);
        lua_settable(L, -3);
    }
    lua_pop(L, 1);
}

int lua::CallFunc(lua_State *L, const char *funcname, const Value& arg) {
    lua_getglobal(L, funcname);
    push_value(L, arg);
    return lua_call(L, 1, 0);
}

int lua::CallFunc(lua_State *L, const char *funcname, const ByteVec& arg) {
    lua_getglobal(L, funcname);
    lua_pushlstring (L, &arg[0], arg.size());
    return lua_call(L, 1, 0);
}

int lua::Dofile(lua_State *L, const string &filename) 
{
    string fname;
    if (app.resourceFS->findFile(filename, fname)) {
        int oldtop = lua_gettop(L);
        int retval = lua_dofile(L, fname.c_str());
        lua_settop(L, oldtop);
        return retval;
    }
    return LUA_ERRFILE;
}

int lua::DoSysFile(lua_State *L, const string &filename) 
{
    string fname;
    if (app.systemFS->findFile(filename, fname)) {
        int oldtop = lua_gettop(L);
        int retval = lua_dofile(L, fname.c_str());
        lua_settop(L, oldtop);
        return retval;
    }
    return LUA_ERRFILE;
}

int lua::Dobuffer (lua_State *L, const ByteVec &luacode) {
    const char *buffer = reinterpret_cast<const char *>(&luacode[0]);
    return lua_dobuffer (L, buffer, luacode.size(), "buffer");
}

string lua::LastError (lua_State *L)
{
    lua_getglobal (L, "_LASTERROR");
    return string (lua_tostring (L, -1));
}


int lua::DoSubfolderfile(lua_State *L, const string &basefolder, const string &filename) {
    std::list <string> fnames = app.resourceFS->findSubfolderFiles(basefolder, filename);
    int retval = 0;
    while (fnames.size() > 0) {
        int oldtop = lua_gettop(L);
	string fname = fnames.front();
        retval = lua_dofile(L, fname.c_str());
	fnames.pop_front();
        lua_settop(L, oldtop);
    }
    return retval;
}

lua_State *lua::GlobalState() 
{
    if (global_state == 0) {
        lua_State *L = global_state = lua_open(0);

        lua_baselibopen(L);
        lua_strlibopen(L);
        lua_mathlibopen(L);
        lua_iolibopen(L);

        tolua_open(L);
        tolua_enigma_open(L);

        RegisterFuncs(L, globalfuncs);
    }
    return global_state;
}

void lua::ShutdownGlobal() 
{
    assert (global_state);
    lua_close(global_state);
    global_state = 0;
}

#ifdef _MSC_VER
#define snprintf _snprintf
#endif



lua_State *lua::InitLevel() 
{
    char buffer[255];

    lua_State *L = level_state = lua_open(0);

    lua_dostring (L, "options={}");
    snprintf (buffer, sizeof(buffer), "options.Difficulty = %d", 
              server::GetDifficulty());
    lua_dostring (L, buffer);

    lua_baselibopen(L);
    lua_strlibopen(L);
    lua_mathlibopen(L);
    lua_iolibopen(L);

    // Override _ALERT function so we can catch error messages
    lua_dostring (L, "function _ALERT(msg) _LASTERROR = msg end");


    tolua_open(L);
    tolua_enigma_open(L);
    tolua_px_open(L);
    tolua_display_open(L);

    RegisterFuncs(L, levelfuncs);

    // Create a new tag for world::Object objects
    object_tag = lua_newtag(L);
    return L;
}

lua_State *lua::LevelState ()
{
    return level_state;
}

void lua::ShutdownLevel() {
    if (level_state) {
        lua_close(level_state);
        level_state = 0;
    }
}
