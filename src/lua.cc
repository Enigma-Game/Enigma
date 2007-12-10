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
#include "lua.hh"
#include "errors.hh"
#include "main.hh"
#include "world.hh"
#include "config.h"
#include "video.hh"
#include "server.hh"
#include "sound.hh"
#include "options.hh"
#include "WorldProxy.hh"
#include "lev/Index.hh"
#include "lev/Proxy.hh"
#include "stones/OxydStone.hh"
#include <list>

#ifndef CXXLUA
extern "C" {
#include "lualib.h"
#include "tolua++.h"
}
#else
#include "lualib.h"
#include "tolua++.h"
#endif 


#include "lua-display.hh"
#include "lua-enigma.hh"
#include "lua-ecl.hh"

#include "ecl.hh"
#include <cassert>

#include "nls.hh"

// Lua Registry keys of metatables for user objects
#define LUA_ID_OBJECT   "_ENIGMAOBJECT"
#define LUA_ID_POSITION "_POSITION"
#define LUA_ID_WORLD    "_WORLDOBJECT"
#define LUA_ID_NAMEOBJ  "_NAMEDOBJECTS"
#define LUA_ID_GROUP    "_GROUP"
#define LUA_ID_TILES    "_TILES"
#define LUA_ID_TILE     "_TILE"

using namespace std;

using ecl::round_down;
using ecl::strf;

using enigma::GridPos;
using enigma::Object;
using enigma::GridObject;
using enigma::ForceField;
using enigma::WorldProxy;

namespace enigma { namespace lua {
     
    lua_State *level_state = 0;  // level-local state
    lua_State *global_state = 0; // global Lua state

    lua::Error _lua_err_code (int i)
    {
        switch (i) {
            case 0: return NO_LUAERROR;
            case LUA_ERRRUN: return ERRRUN;
            case LUA_ERRFILE: return ERRFILE;
            case LUA_ERRSYNTAX: return ERRSYNTAX;
            case LUA_ERRMEM: return ERRMEM;
            case LUA_ERRERR: return ERRERR;
        }
        assert (!"Should never get there!");
    }

    void throwLuaError(lua_State * L, const char * message) {
        std::string backtrace = message;
        backtrace += "\nBacktrace:\n";
        lua_Debug dbgInfo;
        int frame = 0;
        while (lua_getstack(L, frame, &dbgInfo)) {
            lua_getinfo(L, "Sl", &dbgInfo);
            if (dbgInfo.source[0] == '@')
                // lua code loaded from file
                backtrace += ecl::strf("#%d %s: %d\n", frame, dbgInfo.source,
                                dbgInfo.currentline);
            else if (dbgInfo.source[0] == '-' && dbgInfo.source[1] == '-' &&
                dbgInfo.source[2] == '@') {
                // lua code loaded from string
                std::string code = dbgInfo.source;
                std::string::size_type slashPosFilenameEnd = code.find('\n');
                std::string::size_type slashPosLineStart;
                std::string::size_type slashPosLineEnd = slashPosFilenameEnd;
                for (int i = 1; i < dbgInfo.currentline; i++) {
                    slashPosLineStart = slashPosLineEnd;
                    slashPosLineEnd = code.find('\n', slashPosLineStart + 1);
                }
                backtrace += ecl::strf("#%d %s: %d\n  (%s)\n", frame, 
                        code.substr(2, slashPosFilenameEnd - 2).c_str(),
                        dbgInfo.currentline - 1,
                        code.substr(slashPosLineStart + 1, slashPosLineEnd - 
                                slashPosLineStart - 1).c_str());
            }
            frame++;
        }
        luaL_error(L, backtrace.c_str());
        
    }


/* -------------------- Helper routines -------------------- */

using enigma::Value;

void SetTableVar (lua_State *L,
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

static bool checkMetadata (lua_State *L, int idx, const char *name) {
    bool result = false;
    if (lua_getmetatable(L, idx)) {  // does it have a metatable?
        lua_getfield(L, LUA_REGISTRYINDEX, name);  // get correct metatable
        if (lua_rawequal(L, -1, -2)) {  // does it have the correct mt?
            result = true;
        } else {  // check if it is dynamic metatable
            lua_getfield(L, -2, "_id");
            if (lua_isstring(L, -1) && (strcmp(lua_tostring(L, -1), name) == 0))
                result = true;
            lua_pop(L, 1);
        }
        lua_pop(L, 2);  // remove both metatables
    }
    return result;
}

static bool is_table(lua_State *L, int idx) {
    return lua_istable(L,idx);
}

static bool is_object(lua_State *L, int idx) {
    return lua_isuserdata(L,idx) && checkMetadata(L, idx, LUA_ID_OBJECT);
}

static bool is_position(lua_State *L, int idx) {
    return lua_isuserdata(L,idx) && checkMetadata(L, idx, LUA_ID_POSITION);
}

static bool is_tile(lua_State *L, int idx) {
    return lua_isuserdata(L,idx) && checkMetadata(L, idx, LUA_ID_TILE);
}

static bool is_tiles(lua_State *L, int idx) {
    return lua_isuserdata(L,idx) && checkMetadata(L, idx, LUA_ID_TILES);
}

static bool is_group(lua_State *L, int idx) {
    return lua_isuserdata(L,idx) && checkMetadata(L, idx, LUA_ID_GROUP);
}

static bool is_world(lua_State *L, int idx) {
    return lua_isuserdata(L,idx) && checkMetadata(L, idx, LUA_ID_WORLD);
}

static Object *to_object(lua_State *L, int idx) {
    
    if (lua_isnil(L,idx) && server::EnigmaCompatibility < 1.10)
        return NULL;

    if (!is_object(L,idx)) {
        throwLuaError(L, "Cannot convert type to an Object");
        return NULL;
    }
    int id = *(static_cast<int *>(lua_touserdata(L,idx)));
    Object *obj = NULL;
    if (id != -1)
       obj = Object::getObject(id);

    return obj;  // NULL if object does no longer exist
}

static void pushobject(lua_State *L, Object *obj) {
    int *udata;
    /* Lua does not allow NULL pointers in userdata variables, so
       convert them manually to `nil' values. */
    if (obj == NULL && server::EnigmaCompatibility < 1.10) {
        lua_pushnil(L);   // nil as not existing object
    } else {
        udata=(int *)lua_newuserdata(L,sizeof(int));
        if (obj != NULL)
            *udata = obj->getId();
        else
            *udata = -1;  // object with id 0 as not existing object
        luaL_getmetatable(L, LUA_ID_OBJECT);
        lua_setmetatable(L, -2);
    }
}

static std::list<Object *> toObjectList(lua_State *L, int idx) {
    std::list<Object *> objects;
    
    if (is_group(L, idx)) {
        lua_getmetatable(L, idx);
        int numObjects = lua_objlen(L, -1);
        for (int i = 1; i <= numObjects; ++i) {
            lua_rawgeti(L, -1, i);  // the object
            objects.push_back(to_object(L, -1));
            lua_pop(L, 1);          // the object        
        }
        lua_pop(L, 1);          // the metatable        
    }
    return objects;
}

static int pushNewGroup(lua_State *L, std::list<Object *> objects) {
    // NULL objects and duplicates entries in the list will be eliminated
    int *udata;
    udata=(int *)lua_newuserdata(L,sizeof(int));   // group user object
    *udata = 1;
    
    lua_newtable(L);  // individual metatable copy
    luaL_getmetatable(L, LUA_ID_GROUP);
    // copy metatable template
    lua_pushnil(L);  // first key
    while (lua_next(L, -2) != 0) {
         // key is at index -2 and value at index -1
         lua_pushvalue(L, -2);   // copy key
         lua_insert(L, -2);      // insert key copy below value
         lua_settable(L, -5);    // individual metatable
    }    
    lua_pop(L, 1);  // remove metatable template

    std::set<Object *> unique;
    std::list<Object *>::iterator it = objects.begin();
    for (int i = 1; it != objects.end(); ++it, ++i) {
        if (*it) {  // existing object not NULL
            if (unique.find(*it) == unique.end()) {
                unique.insert(*it);
                pushobject(L, *it);
                lua_pushvalue(L, -1);    // second copy of object
                lua_rawseti(L, -3, i);   // group[i] = obj
                lua_pushinteger(L, i);
                lua_rawset(L, -3);       // group[obj] = i
            }
        }
    }

    lua_setmetatable(L, -2);    
    return 1;
}

static ecl::V2 toPosition(lua_State *L, int idx) {
    double x = 0;
    double y = 0;
    if (is_position(L, -1)) {  // position
        lua_getmetatable(L, -1);
        lua_rawgeti(L, -1, 1);
        lua_rawgeti(L, -2, 2);
        x = lua_tonumber(L, -2);
        y = lua_tonumber(L, -1);
        lua_pop(L, 3);
    }
    return ecl::V2(x, y);
}

static int pushNewPosition(lua_State *L) {
    // x at -2, y at -1
    int *udata;
    udata=(int *)lua_newuserdata(L,sizeof(int));   // position user object
    *udata = 1;
    
    lua_newtable(L);  // individual metatable copy
    luaL_getmetatable(L, LUA_ID_POSITION);
    // copy metatable template
    lua_pushnil(L);  // first key
    while (lua_next(L, -2) != 0) {
         // key is at index -2 and value at index -1
         lua_pushvalue(L, -2);   // copy key
         lua_insert(L, -2);      // insert key copy below value
         lua_settable(L, -5);    // individual metatable
    }    
    lua_pop(L, 1);  // remove metatable template

    lua_pushvalue(L, -4);
    lua_rawseti(L, -2, 1);
    lua_pushvalue(L, -3);
    lua_rawseti(L, -2, 2);

    lua_setmetatable(L, -2);    
    return 1;
}

static int pushNewPosition(lua_State *L, ecl::V2 pos) {
    lua_pushnumber(L, pos[0]);   
    lua_pushnumber(L, pos[1]);
    pushNewPosition(L); 
    lua_remove(L, -3);    // pos[0]
    lua_remove(L, -2);    // pos[1]
    return 1;
}

static void push_value(lua_State *L, const Value &val) {
    switch (val.getType()) {
        case Value::NIL:
        case Value::DEFAULT :
            lua_pushnil(L);
            break;
        case Value::DOUBLE:
            lua_pushnumber(L, val.get_double());
            break;
        case Value::STRING:
            lua_pushstring(L, val.get_string());
            break;
        case Value::BOOL:
            if (server::EnigmaCompatibility < 1.10) {
                if (val.to_bool())
                    lua_pushnumber(L, 1);
                else
                    lua_pushnil(L);
            } else {
                lua_pushboolean(L, val.to_bool());
            }
            break;
        case Value::OBJECT:
            pushobject(L, (Object *)val);
            break;
        case Value::GROUP:
            pushNewGroup(L, val);
            break;
        case Value::POSITION:
            pushNewPosition(L, val);
            break;
        case Value::TOKENS: {
            TokenList tokens = val;
            lua_newtable(L);
            int n = 1;
            for (TokenList::iterator it = tokens.begin(); it != tokens.end(); ++it, n++) {
                push_value(L, *it);
                lua_rawseti(L, -2, n);
            }
            break;
        }
    }
}

static Value to_value(lua_State *L, int idx) {
    switch (lua_type(L, idx)) {
        case LUA_TNIL: 
            return Value();
        case LUA_TNUMBER: 
            return Value(lua_tonumber(L,idx));
        case LUA_TSTRING: 
            return Value(lua_tostring(L,idx));
        case LUA_TBOOLEAN: 
            return Value((bool)lua_toboolean(L,idx));
        case LUA_TUSERDATA:
            if (is_object(L, idx))
                return Value(to_object(L, idx));
            else  if (is_group(L, idx))
                return Value(toObjectList(L, idx));
            else  if (is_position(L, idx))
                return Value(toPosition(L, idx));
        case LUA_TTABLE: {
            TokenList tokens;
            int i = 1;
            lua_rawgeti(L, idx, i);
            while (!lua_isnil(L, -1)) {
                tokens.push_back(to_value(L, -1));
                lua_pop(L, 1);  // this value
                i++;
                lua_rawgeti(L, idx, i);
            }
            lua_pop(L, 1);  // nil value
            return Value(tokens);
        }
        default: 
            throwLuaError(L,"Cannot convert type to Value.");
    }
    return Value();
}


/* -------------------- Interface routines -------------------- */


int en_make_object (lua_State *L)
{
    const char *name = lua_tostring(L, 1);
    if (!name) {
        throwLuaError(L, "MakeObject: string expected as argument");
    }
    Object *obj = MakeObject(name);
    if (obj == NULL)
        throwLuaError(L, ecl::strf("MakeObject: unknown object name '%s'", name).c_str());
    pushobject(L, obj);
    return 1;
}

static int
en_get_object_template(lua_State *L)
{
    Object *obj = GetObjectTemplate(lua_tostring(L, 1));
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
        throwLuaError(L, strf("SetAttrib: invalid object or attribute name '%s'", key).c_str());
    return 0;
}

static int en_get_attrib(lua_State *L) {
    Object *obj = to_object(L,1);
    const char *key = lua_tostring(L,2);

    if (!obj) {
        throwLuaError(L, "GetAttrib: invalid object");
        return 0;
    } else if (!key) {
        throwLuaError(L, "GetAttrib: invalid key");
        return 0;
    } else if (0 == strcmp(key, "kind")) {
        throwLuaError(L, "GetAttrib: illegal attribute, use GetKind()");
        return 0;        
    }

    push_value(L, obj->getAttr(key));
    return 1;
}

static int
en_get_kind(lua_State *L)
{
    Object *obj = to_object(L,1);

    if (!obj) {
        throwLuaError(L, "GetKind: invalid object");
        return 0;
    }

    push_value(L, Value(obj->get_kind()));
    return 1;
}

static int
en_is_same_object(lua_State *L)
{
    Object *obj1 = to_object(L,1);
    Object *obj2 = to_object(L,2);

    lua_pushboolean(L, obj1 == obj2);
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
    else {
         fl = dynamic_cast<Floor *>(to_object(L, 3));
         if (!fl) {
             throwLuaError(L, "object argument 3 must be a floor or nil");
         }
    }
    SetFloor(GridPos(x,y), fl);
    return 0;
}

static int
en_set_item(lua_State *L)
{
    int x = round_down<int>(lua_tonumber(L, 1));
    int y = round_down<int>(lua_tonumber(L, 2));
    Item *it = dynamic_cast<Item*>(to_object(L, 3));
    if( ! it) {
        throwLuaError(L, "object is no valid item");
    }
    SetItem(GridPos(x,y), it);
    return 0;
}

static int
en_set_stone(lua_State *L)
{
    int x = round_down<int>(lua_tonumber(L, 1));
    int y = round_down<int>(lua_tonumber(L, 2));
    Stone *st = dynamic_cast<Stone*>(to_object(L, 3));
    if( ! st)
        throwLuaError(L, "object is no valid stone");
    SetStone(GridPos(x,y), st);
    return 0;
}

static int en_kill_stone(lua_State *L)
{
    int x = round_down<int>(lua_tonumber(L, 1));
    int y = round_down<int>(lua_tonumber(L, 2));
    KillStone(GridPos(x,y));
    return 0;
}

static int en_kill_item(lua_State *L)
{
    int x = round_down<int>(lua_tonumber(L, 1));
    int y = round_down<int>(lua_tonumber(L, 2));
    KillItem(GridPos(x,y));
    return 0;
}

static int
en_set_actor(lua_State *L)
{
    double x = lua_tonumber(L,1);
    double y = lua_tonumber(L,2);
    Actor *ac = dynamic_cast<Actor*>(to_object(L, 3));
    if( ! ac)
        throwLuaError(L, "object is no valid actor");
    if (IsInsideLevel(GridPos(round_down<int>(x), round_down<int>(y))))
        AddActor(x, y, ac);
    else
        throwLuaError(L, "position is outside of world");
    return 0;
}


static int
en_send_message(lua_State *L)
{
    Object     *obj = to_object(L, 1);
    const char *msg = lua_tostring(L, 2);
    Value v;
    if (!msg)
        throwLuaError(L,"Illegal message");
    else if (obj) {
        try {
            v = SendMessage (obj, msg, to_value(L, 3));
        }
        catch (const XLevelRuntime &e) {
            throwLuaError (L, e.what());
        }
        catch (...) {
            throwLuaError (L, "uncaught exception");
        }
    }
    push_value(L, v);
    return 0;
}

int EmitSound (lua_State *L)
{
    Object     *obj       = to_object(L, 1);
    const char *soundname = lua_tostring(L, 2);
    double vol = 1.0;

    if (lua_isnumber(L, 3)) 
        vol  = lua_tonumber(L, 3);
    if (!soundname)
        throwLuaError(L,"Illegal sound");
    else if (obj) {
        GridObject *gobj = dynamic_cast<GridObject*>(obj);
        if (gobj) {
            if (!gobj->sound_event (soundname, vol)) {
                //throwLuaError(L, strf("Can't find sound '%s'", soundname).c_str());
                // Don't throw an error when no sound file was found.
                // Remember that user sound sets might be incomplete, and
                // absolutely correct levels could throw an error here.
                // Instead, write the "silence string" to the command line:
                sound::WriteSilenceString(soundname);
            }
        }
    }
    else
        throwLuaError(L, "EmitSound: invalid object");

    return 0;
}

int EmitSoundGlobal (lua_State *L)
{
    const char *soundname = lua_tostring(L, 1);
    double vol = 1.0;

    if (lua_isnumber(L, 3)) 
        vol  = lua_tonumber(L, 3);
    if (!soundname)
        throwLuaError(L,"Illegal sound");
    else
        sound::EmitSoundEventGlobal(soundname, vol);

    return 0;
}

static int
en_name_object(lua_State *L)
{
    Object     *obj  = to_object(L, 1);
    const char *name = lua_tostring(L, 2);

    if (!obj) 
        throwLuaError(L, "NameObject: Illegal object");
    else if (!name) 
        throwLuaError(L, "NameObject: Illegal name");
    else
        NameObject(obj, name);

    return 0;
}

static int
en_get_named_object(lua_State *L)
{
    Object *o = GetNamedObject(lua_tostring(L,1));
    pushobject(L, o);
    return 1;
}

static int
en_get_floor(lua_State *L)
{
    int x = round_down<int>(lua_tonumber(L, 1));
    int y = round_down<int>(lua_tonumber(L, 2));
    Object *o = GetFloor(GridPos(x, y));
    pushobject(L, o);
    return 1;
}
static int
en_get_item(lua_State *L)
{
    int x = round_down<int>(lua_tonumber(L, 1));
    int y = round_down<int>(lua_tonumber(L, 2));
    Object *o = GetItem(GridPos(x, y));
    pushobject(L, o);
    return 1;
}
static int
en_get_stone(lua_State *L)
{
    int x = round_down<int>(lua_tonumber(L, 1));
    int y = round_down<int>(lua_tonumber(L, 2));
    Object *o = GetStone(GridPos(x, y));
    pushobject(L, o);
    return 1;
}

static int
en_get_pos(lua_State *L)
{
    Object  *obj = to_object(L, 1);
    GridPos  p;

    if (!obj) {
        throwLuaError(L, "GetPos: Illegal object");
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
    SetConstantForce (v);
    return 0;
}

static int
en_add_rubber_band (lua_State *L)
{
    Actor  *a1       = dynamic_cast<Actor*> (to_object(L, 1));
    Object *o2       = to_object(L, 2);
    Actor  *a2       = dynamic_cast<Actor*>(o2);
    Stone  *st       = dynamic_cast<Stone*>(o2);
    RubberBandData d;
    d.strength  = lua_tonumber (L, 3);
    d.length    = lua_tonumber (L, 4);
    d.minlength = lua_tonumber (L, 5);

    if (!a1)
        throwLuaError(L, "AddRubberBand: First argument must be an actor\n");
    else {
        if (a2)
            AddRubberBand (a1, a2, d);
        else if (st)
            AddRubberBand (a1, st, d);
        else
            throwLuaError(L, "AddRubberBand: Second argument must be actor or stone\n");
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
    // Function depreceated
    // - filename is no longer a useful reference for levels
    // - levels should not depend on external data for reasons of
    //   network compatibility and level journaling
    throwLuaError(L, "Usage of depreceated function \"IsSolved()\"");

//    const char  *levelname = lua_tostring(L,1);
//    int          solved    = 0;
//    
//    levels::Level level (0, 0);
//    if (levels::FindLevel (levelname, level)) {
//        solved = level.is_solved(options::GetInt("Difficulty"));
//    } else 
//    if (solved)
//        lua_pushnumber(L, solved);
//    else
//        lua_pushnil(L);
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
        AddScramble(GridPos(x,y), enigma::Direction(found-allowed));
    else 
        throwLuaError(L, "AddScramble: Third argument must be one character of \"wsen\"");

    return 0;
}

static int
en_set_scramble_intensity(lua_State *L)
{
    SetScrambleIntensity(int(lua_tonumber(L, 1)));
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
        throwLuaError (L, "AddSignal: invalid source descriptor");
    if (targetstr == 0 || !to_gridloc(targetstr, target)) 
        throwLuaError (L, "AddSignal: invalid target descriptor");
    if (msg == 0)
        msg = "signal";

    AddSignal (source, target, msg);
    return 0;
}

int loadLib(lua_State *L) 
{
    const char *id = lua_tostring(L, 1);
    lev::Proxy * curProxy = lev::Index::getCurrentProxy();
    try {
        curProxy->loadDependency(id);
    } catch (XLevelLoading &err) {
        throwLuaError(L, err.what());
    }
    return 1;
}


/* -------------------- new functions -------------------- */

/**
 * Replacement of Lua's random function based on Enigma's central random
 * function with support of calculation of parallel worlds in seperated
 * threads.
 */
static int mathRandom (lua_State *L) {
    lua_Number r = (lua_Number)(Rand()) / (lua_Number)(ENIGMA_RAND_MAX+1.0);
    switch (lua_gettop(L)) {      // check number of arguments
        case 0: {                 // no arguments
            lua_pushnumber(L, r); // Number between 0 and 1 
            break;
        }
        case 1: {  // only upper limit
            int u = luaL_checkint(L, 1);
            luaL_argcheck(L, 1<=u, 1, "interval is empty");
            lua_pushnumber(L, floor(r*u)+1);  // int between 1 and `u'
            break;
        }
        case 2: {  // lower and upper limits 
            int l = luaL_checkint(L, 1);
            int u = luaL_checkint(L, 2);
            luaL_argcheck(L, l<=u, 2, "interval is empty");
            lua_pushnumber(L, floor(r*(u-l+1))+l);  // int between `l' and `u'
            break;
        }
        default: return luaL_error(L, "wrong number of arguments");
    }
    return 1;
}

static int newPosition(lua_State *L) {
    // (pos|obj|table|(num,num))
    if (is_table(L, 1)) {  // table 
        lua_rawgeti(L, 1, 1);
        lua_rawgeti(L, 1, 2);
    } else if (is_position(L, 1)) {
        lua_getmetatable(L, 1);
        lua_rawgeti(L, 2, 1);
        lua_rawgeti(L, 2, 2);
    } else if (is_object(L, 1)) {
        lua_pushstring(L, "x");
        lua_gettable(L, 1); 
        lua_pushstring(L, "y");
        lua_gettable(L, 1); 
    }
    
    if ((lua_type(L, -2) != LUA_TNUMBER) || (lua_type(L, -1) != LUA_TNUMBER)) {
        throwLuaError(L, "Invalid new position value type");
        return 0;
    }    

    return pushNewPosition(L);
}

static int addPositionsBase(lua_State *L, int factorArg2) {
    // (pos|obj|table) +|- (pos|obj|table)
    double x = 0;
    double y = 0;
    for (int i = 1; i<=2; i++) {
        if (is_table(L, i)) {  // table
            lua_rawgeti(L, i, 1);
            lua_rawgeti(L, i, 2);
        } else if (is_position(L, i)) {
            lua_getmetatable(L, i);
            lua_rawgeti(L, -1, 1);
            lua_rawgeti(L, -2, 2);
            lua_remove(L, -3);
        } else if (is_object(L, i)) {
            lua_pushstring(L, "x");
            lua_gettable(L, i); 
            lua_pushstring(L, "y");
            lua_gettable(L, i); 
        } else {
            throwLuaError(L, "Invalid add position value type");
        }
        if ((lua_type(L, -2) != LUA_TNUMBER) || (lua_type(L, -1) != LUA_TNUMBER)) {
            throwLuaError(L, "Invalid add position value type");
            return 0;
        }            
        x += (i == 1 ? 1 : factorArg2) *  lua_tonumber(L, -2);
        y += (i == 1 ? 1 : factorArg2) *  lua_tonumber(L, -1);
        lua_pop(L, 2);
    }

    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    return pushNewPosition(L);
}

static int addPositions(lua_State *L) {
    // (pos|obj|table) + (pos|obj|table)
    return addPositionsBase(L, 1);
}
    
static int subPositions(lua_State *L) {
    // (pos|obj|table) - (pos|obj|table)
    return addPositionsBase(L, -1);
}

static int centerPosition(lua_State *L) {
    // #(pos|obj)
    if (is_position(L, 1)) {  // position
        lua_getmetatable(L, 1);
        lua_rawgeti(L, -1, 1);
        lua_rawgeti(L, -2, 2);
        lua_remove(L, -3);
    } else if (is_object(L, 1)) {
        lua_pushstring(L, "x");
        lua_gettable(L, 1); 
        lua_pushstring(L, "y");
        lua_gettable(L, 1); 
    } else {
        throwLuaError(L, "Invalid center position value type");
    }
    if ((lua_type(L, -2) != LUA_TNUMBER) || (lua_type(L, -1) != LUA_TNUMBER)) {
        throwLuaError(L, "Invalid center position value type");
        return 0;
    }            
    double x = round_down<int>(lua_tonumber(L, -2)) + 0.5;
    double y = round_down<int>(lua_tonumber(L, -1)) + 0.5;
    lua_pop(L, 2);
    
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    return pushNewPosition(L);
}

static int multiplyPositionsBase(lua_State *L, bool isDiv = false) {
    // (pos|num) (*|/) (pos|num)
    double x = 0;
    double y = 0;
    double f = 1;
    for (int i = 1; i<=2; i++) {
        if (is_position(L, i)) {
            lua_getmetatable(L, i);
            lua_rawgeti(L, -1, 1);
            x = lua_tonumber(L, -1);
            lua_rawgeti(L, -2, 2);
            y = lua_tonumber(L, -1);
            lua_pop(L, 3);
        } else if (lua_isnumber(L, i)) {
            f = lua_tonumber(L, i);
        } else {
            throwLuaError(L, "Invalid multiplication position value type");
            return 0;
        }
    }

    if (isDiv) {
        x /= f;
        y /= f;
    } else {
        x *= f;
        y *= f;
    }
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    return pushNewPosition(L);
}

static int multiplyPositions(lua_State *L) {
    // (pos|num) * (pos|num)
    if (is_position(L, 1) && is_position(L, 2)) {
        throwLuaError(L, "Multiplication of two positions not allowed");
        return 0;        
    }
    return multiplyPositionsBase(L);
}
 
static int dividePositions(lua_State *L) {
    // (pos|num) * (pos|num)
    if (!is_position(L, 1) || !lua_isnumber(L, 2)) {
        throwLuaError(L, "Division of position with wrong types");
        return 0;        
    }
    return multiplyPositionsBase(L, true);
}

static int gridAlignPosition(lua_State *L) {
    // position guaranteed
    lua_getmetatable(L, 1);
    lua_rawgeti(L, -1, 1);
    lua_rawgeti(L, -2, 2);
    double x = round_down<int>(lua_tonumber(L, -2));
    double y = round_down<int>(lua_tonumber(L, -1));
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    pushNewPosition(L);
    return 1;
}

static int pushNewTile(lua_State *L, int numArgs) {
    // numArgs 1 or 2 of type (table|tile)
    int *udata;
    udata=(int *)lua_newuserdata(L,sizeof(int));   // position user object
    *udata = 1;
    
    lua_newtable(L);  // individual metatable copy
    luaL_getmetatable(L, LUA_ID_TILE);
    // copy metatable template
    lua_pushnil(L);  // first key
    while (lua_next(L, -2) != 0) {
         // key is at index -2 and value at index -1
         lua_pushvalue(L, -2);   // copy key
         lua_insert(L, -2);      // insert key copy below value
         lua_settable(L, -5);    // individual metatable
    }    
    lua_pop(L, 1);  // remove metatable template

    lua_pushvalue(L, -2 - numArgs);
    lua_rawseti(L, -2, 1);
    if (numArgs == 2) {
        lua_pushvalue(L, -3);
        lua_rawseti(L, -2, 2);
    }
    lua_setmetatable(L, -2);    
    return 1;
}



static void setObjectAttributes(Object *obj, lua_State *L) {
    // L, -1 is table with key, values
    if (obj == NULL || !lua_istable(L, -1)) 
        return;
    // process all table entries
    lua_pushnil(L);  // first key
    while (lua_next(L, -2) != 0) {
         // key is at index -2 and value at index -1
         // ignore all keys besides strings - ignore key 1, the object type
         lua_pushvalue(L, -2); // a copy of key for work
         if (!lua_isnumber(L, -1) && lua_isstring(L, -1)) {
            std::string key = lua_tostring(L, -1);
            if (key == "name") {
                if (lua_isstring(L, -2))
                    NameObject(obj, lua_tostring(L, -2));
            } else {
                obj->set_attrib(key, to_value(L, -2));
            }
         }
         lua_pop(L, 2);  // remove copy key + value, leave original key for loop
    }    
}

static int setAttributes(lua_State *L) {
    // 
    Object *obj = to_object(L, 1);
    setObjectAttributes(obj, L);
    return 0;
}

static int getFloor(lua_State *L) {
    // position|table|obj|(num,num)
    if (is_world(L, 1))      // world method?
        lua_remove(L, 1);    // no need of context
    newPosition(L);          // unify all arg types to a position
    lua_getmetatable(L, -1);            
    lua_rawgeti(L, -1, 1);
    int x = round_down<int>(lua_tonumber(L, -1));
    lua_rawgeti(L, -2, 2);
    int y = round_down<int>(lua_tonumber(L, -1));
    Object *o = GetFloor(GridPos(x, y));
    pushobject(L, o);
    return 1;
}

static int getItem(lua_State *L) {
    // position|table|obj|(num,num)
    if (is_world(L, 1))      // world method?
        lua_remove(L, 1);    // no need of context
    newPosition(L);          // unify all arg types to a position
    lua_getmetatable(L, -1);            
    lua_rawgeti(L, -1, 1);
    int x = round_down<int>(lua_tonumber(L, -1));
    lua_rawgeti(L, -2, 2);
    int y = round_down<int>(lua_tonumber(L, -1));
    Object *o = GetItem(GridPos(x, y));
    pushobject(L, o);
    return 1;
}

static int getStone(lua_State *L) {
    // position|table|obj|(num,num)
    if (is_world(L, 1))      // world method?
        lua_remove(L, 1);    // no need of context
    newPosition(L);          // unify all arg types to a position
    lua_getmetatable(L, -1);            
    lua_rawgeti(L, -1, 1);
    int x = round_down<int>(lua_tonumber(L, -1));
    lua_rawgeti(L, -2, 2);
    int y = round_down<int>(lua_tonumber(L, -1));
    Object *o = GetStone(GridPos(x, y));
    pushobject(L, o);
    return 1;
}

static int killObjectBase(lua_State *L) {  // TODO Itemholder owner objects
    Object *obj = to_object(L, -1);
    if (obj) {   // ignore not existing object
        GridObject *gobj;
        switch (obj->getObjectType()) {
            case Object::FLOOR :
                gobj = dynamic_cast<GridObject*>(obj);
                KillFloor(gobj->get_pos());
                break;
            case Object::STONE :
                gobj = dynamic_cast<GridObject*>(obj);
                KillStone(gobj->get_pos());
                break;
            case Object::ITEM  :
                gobj = dynamic_cast<GridObject*>(obj);
                KillItem(gobj->get_pos());
                break;
            case Object::ACTOR :
            default :
                throwLuaError(L, "Kill of object type not allowed");
        }
    }
    return 0;
}

static int killObject(lua_State *L) {
    if (lua_gettop(L) != 1) {
         throwLuaError(L, "Kill of object allows no arguments");
         return 0;
    }
    if (is_group(L, 1)) {
        lua_getmetatable(L, 1);
        int numObjects = lua_objlen(L, -1);
        for (int i = 1; i <= numObjects; ++i) {
            lua_rawgeti(L, -1, i);  // the object
            killObjectBase(L);
            lua_pop(L, 1);  // the object
        }
    } else {
        killObjectBase(L);
    }
    return 0;
}

static int xyObject(lua_State *L) {
    // object guaranteed
    Object *obj = to_object(L,1);
    GridPos  p;
    if (GridObject *gobj = dynamic_cast<GridObject*>(obj)) {
        p = gobj->getOwnerPos();
    } else if (Actor *a = dynamic_cast<Actor*>(obj)) {
        p = GridPos(a->get_pos());
    } else {
        throwLuaError(L, "xy access to not existing object");
        return 0;
    }
    lua_pushnumber(L, double(p.x));
    lua_pushnumber(L, double(p.y));
    return 2;    
}

static int objectEquality(lua_State *L) {
    Object *obj1 = to_object(L,1);
    Object *obj2 = to_object(L,2);

    lua_pushboolean(L, (obj1 == obj2) && obj1);
    return 1;
}

static int objectExistance(lua_State *L) { 
    // object type is guaranteed
    Object * obj = to_object(L, 1);
    lua_pushboolean(L, (obj != NULL));
    return 1;
}

static int objectMessageBase(lua_State *L) {
    // (object, string , value)
    Object     *obj = to_object(L, -3);
    if (!lua_isstring(L, -2)) {
        throwLuaError(L,"Illegal message - no string");
        return 0;
    }
    std::string msg = lua_tostring(L, -2);
    
    Value answer;
    Value val = to_value(L, -1);
    
    if (obj) {   // ignore not existing objects
        try {
            answer = SendMessage (obj, msg, val);
        }
        catch (const XLevelRuntime &e) {
            throwLuaError (L, e.what());
        }
        catch (...) {
            throwLuaError (L, "uncaught exception");
        }
    }
    push_value(L, answer);
    return 1;
}

static int objectMessage(lua_State *L) {
    if (lua_gettop(L) < 2) {
        throwLuaError (L, "Message missing");
        return 0;
    } else if (lua_gettop(L) == 2) {
        lua_pushnil(L);
    }
    return objectMessageBase(L);
}

static int groupMessage(lua_State *L) {
    if (lua_gettop(L) < 2) {
        throwLuaError (L, "Message missing");
        return 0;
    } else if (lua_gettop(L) == 2) {
        lua_pushnil(L);
    }
    
    lua_getmetatable(L, 1);
    int numObjects = lua_objlen(L, -1);
    lua_settop(L, 8);   // add 4 nil - simulate previous loop run
    for (int i = 1; i <= numObjects; ++i) {
        lua_pop(L, 4);          // pop args and return value from last loop
        lua_rawgeti(L, -1, i);  // the object
        lua_pushvalue(L, 2);    // copy the message
        lua_pushvalue(L, 3);    // copy message value
        objectMessageBase(L);   // end up with return value on top
    }
    return numObjects >= 1 ? 1 : 0;  // return last message value if group was not empty
}

std::vector<std::string> messageLIFO;

static int objectDirectMessage(lua_State *L) {
    std::string message = messageLIFO.back();
    messageLIFO.pop_back();
    lua_pushstring(L, message.c_str());
    lua_insert(L, 2);    // message below optional value
    return objectMessage(L);
}

static int groupDirectMessage(lua_State *L) {
    std::string message = messageLIFO.back();
    messageLIFO.pop_back();
    lua_pushstring(L, message.c_str());
    lua_insert(L, 2);    // message below optional value
    return groupMessage(L);
}


static int newGroup(lua_State *L) {
    // (table | (obj[,obj]))
    std::list<Object *> objects;
    if (is_table(L, 1)) {
       if (lua_gettop(L) > 1) {
            throwLuaError(L, "New Group - more arguments than table of objects");
            return 0;
       }
       lua_pushnil(L);  // first key
       while (lua_next(L, 1) != 0) {
           // key at -2, value at -1
           objects.push_back(to_object(L, -1));
           lua_pop(L, 1);  // remove value; keep key for next iteration
       }
    } else if (is_object(L, 1)) {
        for (int i = 1; i <= lua_gettop(L); i++) {
            objects.push_back(to_object(L, i));
        }
    } else {
        throwLuaError(L, "New Group - argument is no object");
        return 0;
    }
    return pushNewGroup(L, objects);
}

static int joinGroup(lua_State *L) {
    // (grp|obj) + (grp|obj)
    if (!((is_group(L, 1) || is_object(L, 1)) &&  (is_group(L, 2) || is_object(L, 2)))) {
        throwLuaError(L, "Join Group - argument is no object or group");
        return 0;
    }
    std::list<Object *> objects;
    for (int j = 1; j <= 2; j++) {
        if (is_group(L, j)) {
            lua_getmetatable(L, j);
            int numObjects = lua_objlen(L, -1);
            for (int i = 1; i <= numObjects; ++i) {
                lua_rawgeti(L, -1, i);  // the object
                objects.push_back(to_object(L, -1));
                lua_pop(L, 1);          // the object        
            }
            lua_pop(L, 1);          // the metatable        
        } else {
            objects.push_back(to_object(L, j));
        }
    }
    return pushNewGroup(L, objects);    
}

static int intersectGroupBase(lua_State *L, bool isIntersect) {
    // (grp|obj) (*|-) (grp|obj)
    if (!((is_group(L, 1) || is_object(L, 1)) &&  (is_group(L, 2) || is_object(L, 2)))) {
        throwLuaError(L, "Intersect or Diff Group - argument is no object or group");
        return 0;
    }
    std::list<Object *> objects;
    std::set<Object *> objSet;
    
    if (is_group(L, 2)) {
        lua_getmetatable(L, 2);
        int numObjects = lua_objlen(L, -1);
        for (int i = 1; i <= numObjects; ++i) {
            lua_rawgeti(L, -1, i);  // the object
            objSet.insert(to_object(L, -1));
            lua_pop(L, 1);          // the object        
        }
        lua_pop(L, 1);          // the metatable        
    } else {
        objSet.insert(to_object(L, 2));
    }
    
    if (is_group(L, 1)) {
        lua_getmetatable(L, 1);
        int numObjects = lua_objlen(L, -1);
        for (int i = 1; i <= numObjects; ++i) {
            lua_rawgeti(L, -1, i);  // the object
            Object * obj = to_object(L, -1);
            if ((isIntersect && objSet.find(obj) != objSet.end()) ||
                    (!isIntersect && objSet.find(obj) == objSet.end()))
                objects.push_back(obj);
            lua_pop(L, 1);          // the object        
        }
        lua_pop(L, 1);          // the metatable        
    } else {
        Object * obj = to_object(L, 2);
        if ((isIntersect && objSet.find(obj) != objSet.end()) ||
                (!isIntersect && objSet.find(obj) == objSet.end()))
            objects.push_back(obj);
    }
    return pushNewGroup(L, objects);    
}

static int intersectGroup(lua_State *L) {
    // (grp|obj) * (grp|obj)
    return intersectGroupBase(L, true);    
}

static int differenceGroup(lua_State *L) {
    // (grp|obj) - (grp|obj)
    return intersectGroupBase(L, false);    
}

MethodMap objectMethodeMap;

static int dispatchObjectReadAccess(lua_State *L) {
    int keyType = lua_type(L, 2);
    
    std::string keyStr;
    MethodMap::iterator iter;
    double keyNum = 0;
    
    switch (keyType) {
        case LUA_TSTRING:
            keyStr = lua_tostring(L, 2); 
            iter = objectMethodeMap.find(keyStr);
            break;
        case LUA_TNUMBER:
            keyNum = lua_tonumber(L, 2); break;
        default:
            throwLuaError(L, "Invalid object read index type");
            return 0;
    }
    
    if (keyType == LUA_TSTRING && iter != objectMethodeMap.end()) {
        // call method -- the existance of the object is not guaranteed!
        lua_pushcfunction(L, iter->second);
        return 1;
    } 
    
    Object *obj = to_object(L,1);

    if (obj && (keyType == LUA_TSTRING) && ((keyStr == "x") || (keyStr == "y"))) {
//    if (obj && (((keyType == LUA_TSTRING) &&
//            ((keyStr == "x") || (keyStr == "y"))) || 
//            ((keyType == LUA_TNUMBER) && ((keyNum == 1) || (keyNum == 2))))) {
        GridPos  p;
        if (GridObject *gobj = dynamic_cast<GridObject*>(obj)) {
            p = gobj->getOwnerPos();
        } else if (Actor *a = dynamic_cast<Actor*>(obj)) {
            p = GridPos(a->get_pos());
        } else {
            p = GridPos(-1, -1);
        }
        
        if (keyStr == "x")  {
//        if (((keyType == LUA_TSTRING) && (keyStr == "x")) ||
//                 ((keyType == LUA_TNUMBER) && (keyNum == 1))) {
            lua_pushnumber(L, double(p.x));
            return 1;
        } else {
            lua_pushnumber(L, double(p.y));
            return 1;
        }
    } else {
        // attribute
        Value val;  // nil
        if (obj) 
            val = obj->getAttr(keyStr);
        if ((keyStr.find('_') != 0) && val.isDefault()) {
            // object exists and no user attribute - no existing system attribute
            // try to send message
            messageLIFO.push_back(keyStr);
            lua_pushcfunction(L, objectDirectMessage);
        } else {
            // user attribute, existing system attribute or nil if no object
            push_value(L, val);
        }
        return 1;
    }
    return 0;
}

static int dispatchObjectWriteAccess(lua_State *L){
    Object *obj = to_object(L,1);
    const char *key = lua_tostring(L,2);
    if (obj && key)
        obj->set_attrib(key, to_value(L, 3));
    else if (obj)  // ignore NULL objects
        throwLuaError(L, strf("SetAttrib: invalid object or attribute name '%s'", key).c_str());
    return 0;
}



static int xyPosition(lua_State *L) {
    // position guaranteed
    lua_getmetatable(L, 1);            
    lua_rawgeti(L, -1, 1);
    lua_rawgeti(L, -2, 2);
    return 2;    
}

static int positionEquality(lua_State *L) {
    // position type is guaranteed
    lua_getmetatable(L, 1);
    lua_rawgeti(L, -1, 1);
    lua_rawgeti(L, -2, 2);
    lua_getmetatable(L, 2);
    lua_rawgeti(L, -1, 1);
    lua_rawgeti(L, -2, 2);

    lua_pushboolean(L, lua_equal(L, -5, -2) && lua_equal(L, -4, -1));
    return 1;
}


MethodMap positionMethodeMap;


static int dispatchPositionReadAccess(lua_State *L) {
    // L,1 is a table
    int keyType = lua_type(L, 2);
    
    std::string keyStr;
    MethodMap::iterator iter;
    double keyNum = 0;
    
    switch (keyType) {
        case LUA_TSTRING:
            keyStr = lua_tostring(L, 2); 
            iter = positionMethodeMap.find(keyStr);
            break;
        case LUA_TNUMBER:
            keyNum = lua_tonumber(L, 2); break;
        default:
            throwLuaError(L, "Invalid position read index type");
            return 0;
    }
    
    if (keyType == LUA_TSTRING && iter != positionMethodeMap.end()) {
        // call method
        lua_pushcfunction(L, iter->second);
        return 1;
    } 
    
    if ((keyType == LUA_TSTRING) && ((keyStr == "x") || (keyStr == "y"))) {
//    if (((keyType == LUA_TSTRING) &&
//            ((keyStr == "x") || (keyStr == "y"))) || 
//            ((keyType == LUA_TNUMBER) && ((keyNum == 1) || (keyNum == 2)))) {
        
        lua_getmetatable(L, 1);
        if (keyStr == "x") {
//        if (((keyType == LUA_TSTRING) && (keyStr == "x")) ||
//                 ((keyType == LUA_TNUMBER) && (keyNum == 1))) {
            lua_rawgeti(L, -1, 1);
        } else {
            lua_rawgeti(L, -1, 2);
        }
        return 1;
    }
    
    throwLuaError(L, "Invalid position index");
    return 0;
}

static int dispatchPositionWriteAccess(lua_State *L) {
    // L,1 is a table
    int keyType = lua_type(L, 2);
    // L,3 is value
    
    std::string keyStr;
    double keyNum = 0;
    
    switch (keyType) {
        case LUA_TSTRING:
            keyStr = lua_tostring(L, 2);  break;
        case LUA_TNUMBER:
            keyNum = lua_tonumber(L, 2); break;
        default:
            throwLuaError(L, "Invalid position write index type");
            return 0;
    }
    
    if (lua_type(L, 3) != LUA_TNUMBER) {
            throwLuaError(L, "Invalid position write value type");
            return 0;
    }
     
    if ((keyType == LUA_TSTRING) && ((keyStr == "x") || (keyStr == "y"))) {
//    if (((keyType == LUA_TSTRING) &&
//            ((keyStr == "x") || (keyStr == "y"))) || 
//            ((keyType == LUA_TNUMBER) && ((keyNum == 1) || (keyNum == 2)))) {
        lua_getmetatable(L, 1);
        lua_pushvalue(L, -2);
        if (keyStr == "x") {
//        if (((keyType == LUA_TSTRING) && (keyStr == "x")) ||
//                 ((keyType == LUA_TNUMBER) && (keyNum == 1))) {
            lua_rawseti(L, -2, 1);
            return 0;
        } else {
            lua_rawseti(L, -2, 2);
            return 0;
        }
    }
    
    throwLuaError(L, "Invalid position index");
    return 0;
}

MethodMap namedObjMethodeMap;


static int dispatchNamedObjReadAccess(lua_State *L) {
    // string with *,? wildcards are groups, rest is single object
    if (!lua_isstring(L, 2)) {
        throwLuaError(L, "Named object access without giving a name");
        return 0;
    }
    std::string name = lua_tostring(L, 2);
    
    if (name.find_first_of("*?") != std::string::npos) {
        // search all objects that match the template
        std::list<Object *> group = GetNamedGroup(name);
        return pushNewGroup(L, group);
    } else {    
        // search for a unique object
        Object *obj = GetNamedObject(name);
        pushobject(L, obj);
    }
    return 1;
}

static int dispatchNamedObjWriteAccess(lua_State *L) {
    // $string -- is reserved
    Object *obj  = to_object(L, 3);
    if (!obj) {   // no need to name not existing object
        return 0;
    } else if (!lua_isstring(L, 2)) {
        throwLuaError(L, "NameObject: Illegal name");
        return 0;
    }
    std::string name = lua_tostring(L, 2);
    if (name.find('$') == 0) {
        throwLuaError(L, "NameObject: names with leading '$' are reserved");
        return 0;
    }
    NameObject(obj, name);
    return 0;
}

static int pushNewNamedObj(lua_State *L) {
    // x at -2, y at -1
    int *udata;
    udata=(int *)lua_newuserdata(L,sizeof(int));   // position user object
    *udata = 1;
    
    luaL_getmetatable(L, LUA_ID_NAMEOBJ);
    lua_setmetatable(L, -2);
    return 1;
}


MethodMap worldMethodeMap;


static int dispatchWorldReadAccess(lua_State *L) {
    // 
    if (lua_isstring(L, 2)) {
        std::string keyStr = lua_tostring(L, 2);
        // TODO check string
        MethodMap::iterator iter = worldMethodeMap.find(keyStr);
        if (iter != positionMethodeMap.end()) {
            // call method
            lua_pushcfunction(L, iter->second);
        } else {
            Value v = WorldProxy::instance()->getAttr(keyStr);
            push_value(L, v);
        }
        return 1;
    } else {
        throwLuaError(L, "Named object access without giving a name");
        return 0;
    }
}


static int setObjectByTable(lua_State *L, double x, double y) {
    // table at -1 
    int xi = round_down<int>(x);
    int yi = round_down<int>(y);
    Object *obj = NULL;
    lua_rawgeti(L, -1, 1);
    if (!lua_isstring(L, -1)) {
        throwLuaError(L, "World: object type string missing");
        return 0;
    }
    std::string name = lua_tostring(L, -1);
    if (name.find('#') == 0 ) {
        // position to be centered
        x = xi + 0.5;
        y = yi + 0.5;
        name = name.substr(1);
    }
    
    if (name == "fl-nil") {
        KillFloor(GridPos(xi, yi));
        return 0;
    } else if (name == "st-nil") {
        KillStone(GridPos(xi, yi));
        return 0;
    } else if (name == "it-nil") {
        KillItem(GridPos(xi, yi));
        return 0;
    }
    
    obj = MakeObject(name.c_str());
    if (obj == NULL) {
        throwLuaError(L, ecl::strf("World: unknown object name '%s'", name.c_str()).c_str());
        return 0;
    }
    lua_pop(L, 1);   // object type
    setObjectAttributes(obj, L);
    switch (obj->getObjectType()) {
        case Object::FLOOR :
            if (Value odd = obj->getAttr("checkerboard")) {
                if ((xi+yi)%2 != (int)odd)
                    break;
            }
            SetFloor(GridPos(xi,yi), dynamic_cast<Floor *>(obj));
            break;
        case Object::STONE :
            SetStone(GridPos(xi,yi), dynamic_cast<Stone *>(obj));
            break;
        case Object::ITEM  :
            SetItem(GridPos(xi,yi), dynamic_cast<Item *>(obj));
            break;
        case Object::ACTOR :
            lua_rawgeti(L, -1, 2);
            if (lua_isnumber(L, -1))
                x += lua_tonumber(L, -1);
            lua_rawgeti(L, -2, 3);
            if (lua_isnumber(L, -1))
                y += lua_tonumber(L, -1);
            lua_pop(L, 2);               
            if (IsInsideLevel(GridPos(round_down<int>(x), round_down<int>(y)))) 
                AddActor(x, y, dynamic_cast<Actor *>(obj));
            else
                throwLuaError(L, "World: actor addition to position outside of world");
            break;
        default :
            throwLuaError(L, "World set of unknown object");
    }
    return 0;
}

static int setObjectByTile(lua_State *L, double x, double y) {
    // tile at -1
    
    // this is a recursive function - ensure enough space on the stack
    if (lua_gettop(L) >  LUA_MINSTACK - 5)
        lua_checkstack(L, 10);         // guarantee another 10 free slots
        
    lua_getmetatable(L, -1);
    lua_rawgeti(L, -1, 1);    // first tile part
    if (is_tile(L, -1))
        setObjectByTile(L, x, y);
    else
        setObjectByTable(L, x, y);
    lua_pop(L, 1);  // tile or table
    lua_rawgeti(L, -1, 2);    // second optional tile part
    if (!lua_isnil(L, -1)) {
        if (is_tile(L, -1))
            setObjectByTile(L, x, y);
        else
            setObjectByTable(L, x, y);
    }
    lua_pop(L, 2);  // tile or table or nil + metatable
    return 0;
}

static int initWorld(lua_State *L) {
    // world, (ti|function), string, table
    if (server::WorldInitialized) {
        throwLuaError(L, "World reinitialization not allowed");
        return 0;
    }
    
    int width = 0;
    int height = 0;
    int keyLength = 1;
    std::string defaultKey;
    if (!(is_tiles(L, 2) && lua_isstring(L, 3) && is_table(L, 4))) {
        throwLuaError(L, "World init with false argument types");
        return 0;
    }
    defaultKey = lua_tostring(L, 3);
    keyLength = defaultKey.length();
    height = lua_objlen(L, 4);
    std::vector<std::string> lines;
    Log << "initWorld  keyLength " << keyLength <<"\n";
    for (int i = 1; i <= height; i++) {
        lua_pushinteger(L, i);
        lua_gettable(L, -2);
        if (!lua_isstring(L, -1)) {
            throwLuaError(L, "World init table has enties that are no strings");
            return 0;
        }
        lines.push_back(lua_tostring(L, -1));
        lua_pop(L, 1);
        width = ecl::Max(width, (int)lines.back().length());
    }
    if (width % keyLength != 0) {
        throwLuaError(L, "World init table row with odd length");
        return 0;
    }
    width = width/keyLength;
    Log << "initWorld  - w " << width << "  - h " << height << "\n";

    Resize(width, height);
    
    luaL_getmetatable(L, LUA_ID_TILES);
    lua_rawgeti(L, -1, 1);               // tiles content table
    for (int i = 0; i < height; i++) {
        std::string &line = lines[i];
        int lineLength = line.length();
        if (lineLength % keyLength != 0) {
            throwLuaError(L, "World init table row with odd length");
            return 0;
        }
        lineLength = lineLength / keyLength;
        for (int j = 0; j < width; j++) {
            std::string key;
            if (j < lineLength) {
                key = line.substr(j*keyLength, keyLength);  // tiles key
            } else {
                key = defaultKey;
            }
            lua_pushstring(L, key.c_str());
            lua_rawget(L, -2);        // get tile entry in table
            if (lua_isnil(L, -1)) {
                throwLuaError(L, ecl::strf("World init undefined tile '%s' at %d, %d", 
                        key.c_str(), j, i).c_str());
                return 0;
            }
            setObjectByTile(L, j, i);
            lua_pop(L, 1);  // tile
        }
    }
    lua_pushinteger(L, width);
    lua_pushinteger(L, height);
    return 2;
}

static int dispatchWorldWriteAccess(lua_State *L) {
    // [object|position|table] = table|tile
    // [string] = value
    if (lua_isstring(L, 2)) {
        std::string name = lua_tostring(L, 2);
        // TODO check string
        
        WorldProxy::instance()->set_attrib(name, to_value(L, 3));
        return 0;
    } else if ((is_object(L, 2) || is_position(L, 2) || is_table(L, 2) || is_group(L, 2)) && 
            ((is_table(L, 3)) || is_tile(L, 3))) {
        // set object
        double x = -1;
        double y = -1;
        if (is_table(L,2)) {
            lua_rawgeti(L, 2, 1);
            lua_rawgeti(L, 2, 2);
            if ((lua_type(L, -2) != LUA_TNUMBER) || (lua_type(L, -1) != LUA_TNUMBER)) {
                throwLuaError(L, "Invalid position value type");
                return 0;
            }
            x = lua_tonumber(L, -2);
            y = lua_tonumber(L, -1);
            lua_pop(L, 2);
        } else if (is_position(L, 2)) {  // position
            lua_getmetatable(L, 2);
            lua_rawgeti(L, -1, 1);
            x = lua_tonumber(L, -1);
            lua_rawgeti(L, -2, 2);
            y = lua_tonumber(L, -1);
            lua_remove(L, -3);
        } else if (is_object(L, 2)) {
            Object *obj = to_object(L, 2);
            GridPos  p;
            if (GridObject *gobj = dynamic_cast<GridObject*>(obj)) {
                p = gobj->getOwnerPos();
            } else if (Actor *a = dynamic_cast<Actor*>(obj)) {
                p = GridPos(a->get_pos());
            } else {  // NULL object
                // ignore not existing objects
                return 0;                
            }
            x = p.x;
            y = p.y;
        } else if (is_group(L, 2)) {
            lua_getmetatable(L, 2);
            int numObjects = lua_objlen(L, -1);
            for (int i = 1; i <= numObjects; ++i) {
                lua_rawgeti(L, -1, i);  // the object
                Object *obj = to_object(L, -1);
                if (obj) {              // ignore not existing objects
                    GridPos  p;
                    if (GridObject *gobj = dynamic_cast<GridObject*>(obj)) {
                        p = gobj->getOwnerPos();
                    } else if (Actor *a = dynamic_cast<Actor*>(obj)) {
                        p = GridPos(a->get_pos());
                    }
                    x = p.x;
                    y = p.y;
                    lua_pushvalue(L, 3);
                    if (is_table(L, -1))
                        setObjectByTable(L, x, y);
                    else // is tile
                        setObjectByTile(L, x, y);

                    lua_pop(L, 1);          // the tile/table        
                }
                lua_pop(L, 1);          // the object        
            }
            return 0;
        }
        
        lua_pushvalue(L, 3);
        if (is_table(L, -1))
            setObjectByTable(L, x, y);
        else // is tile
            setObjectByTile(L, x, y);
        return 0;
    } else {
        throwLuaError(L, "World write access with bad index");
        return 0;
    }
}


static int pushNewWorld(lua_State *L) {
    int *udata;
    udata=(int *)lua_newuserdata(L,sizeof(int));   // position user object
    *udata = 1;
    
    luaL_getmetatable(L, LUA_ID_WORLD);
    lua_setmetatable(L, -2);
    return 1;
}

static int shuffleOxyd(lua_State *L) {
    // world, {table}  -- table with 1=(group|obj|name), [2=(group|obj|name], 
    //                               min=[number], max=[number], circular=true, linear=true
    //                               log=("solution"|"count"|"all") 
    OxydStone::LogType logFlag = OxydStone::NOTHING;
    for (int i = 2; i <= lua_gettop(L); i++) {
        if (!is_table(L, i)) {
            throwLuaError(L, "Shuffle oxyds - rule is not a table");
            return 0;
        }
        lua_rawgeti(L, i, 1);
        if (!(is_group(L, -1) || is_object(L, -1) || lua_isstring(L, -1))) {
            throwLuaError(L, "Shuffle oxyds - rule first group fault");
            return 0;
        }
        Value group1 = to_value(L, -1);
        lua_pop(L, 1);
        lua_rawgeti(L, i, 2);
        if (!(is_group(L, -1) || is_object(L, -1) || lua_isstring(L, -1) || lua_isnil(L, -1))) {
            throwLuaError(L, "Shuffle oxyds - rule second group fault");
            return 0;
        }
        Value group2 = to_value(L, -1);
        bool is_pair = !lua_isnil(L, -1); 
        lua_pop(L, 1);
        
        lua_getfield(L, i, "log");
        if (!lua_isnil(L, -1) && lua_isstring(L, -1)) {
            if (0 == strcmp(lua_tostring(L,-1), "solution") && logFlag < OxydStone::SOLUTION) 
                logFlag = OxydStone::SOLUTION;
            else if (0 == strcmp(lua_tostring(L,-1), "count") && logFlag < OxydStone::COUNT)
                logFlag = OxydStone::COUNT;
            else if (0 == strcmp(lua_tostring(L,-1), "all") && logFlag < OxydStone::ALL)
                logFlag = OxydStone::ALL;                
        }
        lua_pop(L, 1);
        
        unsigned short limit;
        
        lua_getfield(L, i, "min");
        if (!lua_isnil(L, -1)) {
            limit = (unsigned short) ecl::Clamp<int>(lua_tointeger(L, -1), 0, OxydStone::numColorsAvailable());
            OxydStone::addShuffleRule(is_pair ? OxydStone::RULE_PAIR_MIN : OxydStone::RULE_SINGLE_MIN, 
                    (unsigned short) limit, group1, group2);
        }
        lua_pop(L, 1);

        lua_getfield(L, i, "max");
        if (!lua_isnil(L, -1)) {
            limit = (unsigned short) ecl::Clamp<int>(lua_tointeger(L, -1), 0, OxydStone::numColorsAvailable());            
            OxydStone::addShuffleRule(is_pair ? OxydStone::RULE_PAIR_MAX : OxydStone::RULE_SINGLE_MAX, 
                    (unsigned short) limit, group1, group2);
        }
        lua_pop(L, 1);

        if (!is_pair) {
            lua_getfield(L, i, "linear");
            if (lua_isboolean(L, -1) && lua_toboolean(L, -1)) {
                ObjectList oxyds = group1;
                Object *firstOxyd = NULL;
                for (ObjectList::iterator i = oxyds.begin(); i != oxyds.end(); ++i) {
                    if (firstOxyd == NULL)
                        firstOxyd = *i;
                    else {
                        OxydStone::addShuffleRule(OxydStone::RULE_PAIR_MAX, 0, Value(firstOxyd), Value(*i));
                        firstOxyd = *i;
                    }
                }
            }
            lua_pop(L, 1);
            
            lua_getfield(L, i, "circular");
            if (lua_isboolean(L, -1) && lua_toboolean(L, -1)) {
                ObjectList oxyds = group1;
                Object *firstOxyd = NULL;
                Object *leftOxyd = NULL;
                for (ObjectList::iterator i = oxyds.begin(); i != oxyds.end(); ++i) {
                    if (firstOxyd == NULL) {
                        firstOxyd = *i;
                        leftOxyd = *i;
                    } else {
                        OxydStone::addShuffleRule(OxydStone::RULE_PAIR_MAX, 0, Value(leftOxyd), Value(*i));
                        leftOxyd = *i;
                    }
                }
                if (firstOxyd != NULL && firstOxyd != leftOxyd)
                    OxydStone::addShuffleRule(OxydStone::RULE_PAIR_MAX, 0, Value(firstOxyd), Value(leftOxyd));
            }
            lua_pop(L, 1);
        }
        
    }
    OxydStone::shuffleColors(logFlag);
    return 0;
}

MethodMap tileMethodeMap;

static int dispatchTileWriteAccess(lua_State *L) {
    throwLuaError(L, "Tile: illegal write access");
    return 0;
}

static int dispatchTileReadAccess(lua_State *L) {
    throwLuaError(L, "Tile: illegal read access");
    return 0;
}

static int appendTile(lua_State *L) {
    // (tile|table) .. (tile|table)
    if (!((is_tile(L, 1)||is_table(L, 1)) && (is_tile(L, 1)||is_table(L, 1)))) {
        throwLuaError(L, "Tile concat of unsupported types");
        return 0;
    }
    // TODO check tables for position content
    
    // Lua currently does not provide a default metatable for tables - thus we cannot define
    // a concat of new tables like  "{"fl-hay"} .. {"it-seed"}"
    // If this feature is added to Lua we would have to add:
//    if (is_table(L, 1) && is_table(L, 2)) {
//        pushNewTile(L, 1);  // wrap second table into a tile;
//        lua_remove(L, 2);   // replace table by tile
//    }
    pushNewTile(L, 2);   // wrap both arguments into a new tile
    return 1;
}

MethodMap tilesMethodeMap;

static int dispatchTilesReadAccess(lua_State *L) {
//    Log << "Tiles read key - " << lua_tostring(L, 2) << "\n";
    if (!lua_isstring(L, 2)) {     // sideeffect: numbers are converted to string
        throwLuaError(L, "Tiles: key is not a string");
        return 0;
    }
    lua_getmetatable(L, 1);
    lua_rawgeti(L, -1, 1);   // content table
    lua_pushvalue(L, 2);      // copy key
    lua_rawget(L, -2);        // check for existing entry in table
    return 1;
}

static int dispatchTilesWriteAccess(lua_State *L) {
//    Log << "Tiles write key - " << lua_tostring(L, 2) << "\n";
    if (!lua_isstring(L, 2)) {     // sideeffect: numbers are converted to string
        throwLuaError(L, "Tiles: key is not a string");
        return 0;
    }
    if (is_table(L, 3)) {
        // convert table to a tile
        pushNewTile(L, 1);
        lua_remove(L, 3);   
    } else if (!is_tile(L, 3)) {
        throwLuaError(L, "Tiles: value is not a tile or table");
        return 0;
    }
    lua_getmetatable(L, 1);
    lua_rawgeti(L, -1, 1);   // content table
    lua_pushvalue(L, 2);      // copy key
    lua_rawget(L, -2);        // check for existing entry in table
    if (!lua_isnil(L, -1)) {
        throwLuaError(L, ecl::strf("Tiles: redefinition of key '%s'", lua_tostring(L, 2)).c_str());
        return 0;
    }
    lua_pop(L, 1);            // remove nil
    lua_pushvalue(L, 2);
    lua_pushvalue(L, 3);
    lua_rawset(L, -3);        // store tile value for key
    return 0;
}

static int pushNewTiles(lua_State *L) {
    // tiles is a singleton
    
    int *udata;
    udata=(int *)lua_newuserdata(L,sizeof(int));   // position user object
    *udata = 1;
    
    luaL_getmetatable(L, LUA_ID_TILES);
    lua_rawgeti(L, -1, 1);
    if (lua_isnil(L, -1)) {
        lua_newtable(L);            // this is the contents table that stores the data
        lua_rawseti(L, -3, 1);
    }
    lua_pop(L, 1); //nil or content table
    lua_setmetatable(L, -2);
    return 1;
}

static int newTile(lua_State *L) {
    // ti(table)
    if (!(is_table(L, 2) && lua_gettop(L) == 2)) {
        throwLuaError(L, "New tile with wrong arguments");
        return 0;
    }
    
    pushNewTile(L, 1);
    return 1;
}

MethodMap groupMethodeMap;

static int dispatchGroupWriteAccess(lua_State *L) {
    if (!(lua_isstring(L, 2))) {
        throwLuaError(L, "Group: illegal attribute write access");
        return 0;
    }
    std::string name = lua_tostring(L, 2);
    lua_getmetatable(L, 1);
    int numObjects = lua_objlen(L, -1);
    for (int i = 1; i <= numObjects; ++i) {
        lua_rawgeti(L, -1, i);  // the object
        Object *obj = to_object(L, -1);
        if (obj)                // ignore not existing objects
            obj->set_attrib(name, to_value(L, 3));
        lua_pop(L, 1);          // the object        
    }
    return 0;
}

static int dispatchGroupReadAccess(lua_State *L) {
    if (!(lua_isnumber(L, 2) || lua_isstring(L, 2) || is_object(L,2))) {
        throwLuaError(L, "Group: illegal read access");
        return 0;
    }
    
    if (lua_isnumber(L, 2)) {
        int i = lua_tointeger(L, 2);
        lua_getmetatable(L, 1);
        int size = lua_objlen(L, -1);
        if (i >= 1 && i <= size)
            lua_rawgeti(L, -1, i);
        else
            pushobject(L, NULL);
    } else if(lua_isstring(L, 2)) {
        std::string keyStr = lua_tostring(L, 2); 
        MethodMap::iterator iter;
        iter = groupMethodeMap.find(keyStr);
        if (iter != groupMethodeMap.end()) {
            // call method
            lua_pushcfunction(L, iter->second);
        } else {
            // try to send message to all objects
            messageLIFO.push_back(keyStr);
            lua_pushcfunction(L, groupDirectMessage);
        }
    } else {
        lua_getmetatable(L, 1);
        lua_pushvalue(L, 2);   // copy last object as key
        lua_rawget(L, -2);     // get last index        
    }
    return 1;
}

static int lengthGroup(lua_State *L) {
    lua_getmetatable(L, 1);
    int size = lua_objlen(L, -1);
//    Log << "Length Group " << size << "\n";
    lua_pushinteger(L, size);
    return 1;
}

static int iteratorGroup(lua_State *L) {
    // generic for loop iterator function
    // var_1 = _f(_s, _var) with _var == nil on first access, var_1 == nil on end
    // on stack: group, _s, _var
    if (!(is_group(L, 1)))
        throwLuaError(L, "Group: iterator first arg not a group");
    lua_getmetatable(L, 1);
    int size = lua_objlen(L, -1);
    if (lua_isnil(L, 3)) {   // first iterator loop access
        if (size == 0) {     // an empty group
            lua_pushnil(L);
            return 1;
        } else {
            lua_rawgeti(L, -1, 1);  // get the first object
            return 1;
        }
    } else {
        lua_pushvalue(L, 3);   // copy last object as key
        lua_rawget(L, -2);     // get last index
        int i = lua_tointeger(L, -1);
        lua_rawgeti(L, -2, ++i);  // get next object
        return 1;
    }
}

static CFunction globalfuncs[] = {
    {FindDataFile,          "FindDataFile"},
//    {lua::PlaySoundGlobal,  "PlaySoundGlobal"},
//    {lua::PlaySound,        "PlaySound"},
    {en_get_ticks,             "GetTicks"},
    {0,0}
};

static CFunction levelfuncs[] = {

    // internal functions

    {FindDataFile,          "FindDataFile"},
    {loadLib,               "LoadLib"},
    {en_get_object_template,"GetObjectTemplate"},
    {en_make_object,        "MakeObject"},
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
    {en_is_same_object,     "IsSameObject"},

    // manipulating objects

    {en_set_attrib,         "SetAttrib"},
    {en_send_message,       "SendMessage"},
    {en_name_object,        "NameObject"},

    // sound effects

    {lua::EmitSound,        "EmitSound"},

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

    {mathRandom,                    "random"},
    {0,0}
};

static CFunction levelFuncs[] = {
    {mathRandom,                    "random"},
    {newGroup,                      "grp"},
    {newPosition,                   "po"},
    {getFloor,                      "fl"},
    {getItem,                       "it"},
    {getStone,                      "st"},
    {0,0}
};


static CFunction objectOperations[] = {
    {dispatchObjectWriteAccess,     "__newindex"}, //  obj[key]=value
    {dispatchObjectReadAccess,      "__index"},    //  obj[key]
    {objectEquality,                "__eq"},       //  ==
    {addPositions,                  "__add"},      //  obj + obj
    {subPositions,                  "__sub"},      //  obj - obj
    {centerPosition,                "__len"},      //  #obj
    {objectExistance,               "__unm"},      //  -obj
    {0,0}
};

static CFunction objectMethods[] = {
    {objectExistance,               "exists"},
    {killObject,                    "kill"},
    {objectMessage,                 "message"},
    {setAttributes,                 "set"},
    {xyObject,                      "xy"},
    {0,0}
};

static CFunction positionOperations[] = {
    {dispatchPositionWriteAccess,   "__newindex"}, //  obj[key]=value
    {dispatchPositionReadAccess,    "__index"},    //  obj[key]
    {positionEquality,              "__eq"},       //  ==
    {addPositions,                  "__add"},      //  obj + obj
    {subPositions,                  "__sub"},      //  obj - obj
    {multiplyPositions,             "__mul"},      //  obj * obj
    {dividePositions,               "__div"},      //  obj / obj
    {centerPosition,                "__len"},      //  #obj
    {0,0}
};

static CFunction positionMethods[] = {
    {xyPosition,                    "xy"},
    {gridAlignPosition,             "grid"},
    {0,0}
};

static CFunction namedObjOperations[] = {
    {dispatchNamedObjWriteAccess,   "__newindex"}, //  obj[key]=value
    {dispatchNamedObjReadAccess,    "__index"},    //  obj[key]
    {0,0}
};

static CFunction namedObjMethods[] = {
    {0,0}
};

static CFunction worldOperations[] = {
    {dispatchWorldWriteAccess,      "__newindex"}, //  obj[key]=value
    {dispatchWorldReadAccess,       "__index"},    //  obj[key]
    {initWorld,                     "__call"},
    {0,0}
};

static CFunction worldMethods[] = {
    {initWorld,                     "init"},
    {getFloor,                      "fl"},
    {getItem,                       "it"},
    {getStone,                      "st"},
    {shuffleOxyd,                   "shuffleOxyd"},
    {0,0}
};

static CFunction tileOperations[] = {
    {dispatchTileWriteAccess,       "__newindex"}, //  obj[key]=value
    {dispatchTileReadAccess,        "__index"},    //  obj[key]
    {appendTile,                    "__concat"},   //  obj .. obj
    {0,0}
};

static CFunction tileMethods[] = {
    {0,0}
};

static CFunction tilesOperations[] = {
    {dispatchTilesWriteAccess,      "__newindex"}, //  obj[key]=value
    {dispatchTilesReadAccess,       "__index"},    //  obj[key]
    {newTile,                       "__call"},
    {0,0}
};

static CFunction tilesMethods[] = {
    {0,0}
};

static CFunction groupOperations[] = {
    {dispatchGroupWriteAccess,      "__newindex"}, //  obj[key]=value
    {dispatchGroupReadAccess,       "__index"},    //  obj[key]
    {lengthGroup,                   "__len"},      //  #obj
    {joinGroup,                     "__add"},      //  obj + obj
    {intersectGroup,                "__mul"},      //  obj * obj
    {differenceGroup,               "__sub"},      //  obj - obj
    {iteratorGroup,                 "__call"},
    {0,0}
};

static CFunction groupMethods[] = {
    {groupMessage,                  "message"},
    {killObject,                    "kill"},
    {0,0}
};


/* -------------------- lua:: functions -------------------- */

int FindDataFile (lua_State *L) 
{
    const char *filename = lua_tostring(L, 1);
    string absfile;
    if (app.resourceFS->findFile(filename, absfile)) 
        lua_pushstring(L, absfile.c_str());
    else
        lua_pushnil (L);
    return 1;
}

void RegisterFuncs(lua_State *L, CFunction *funcs) 
{
    lua_getglobal(L, "enigma");
    for (unsigned i=0; funcs[i].func; ++i) {
        lua_pushstring(L, funcs[i].name);
        lua_pushcfunction(L, funcs[i].func);
        lua_settable(L, -3);
    }
    lua_pop(L, 1);
}

void RegisterFuncs2(lua_State *L, CFunction *funcs) {
    lua_newtable(L);    // create Namespace "en" 
    for (unsigned i=0; funcs[i].func; ++i) {
        // function within namspace
        lua_pushstring(L, funcs[i].name);
        lua_pushcfunction(L, funcs[i].func);
        lua_settable(L, -3);
        
        // function in global space 
        lua_pushcfunction(L, funcs[i].func);
        lua_setglobal(L, funcs[i].name);
    }
    lua_setglobal(L, "en");
}

void RegisterObject(lua_State *L, std::string name) {
    lua_pushvalue(L, -1);  // make a copy
    lua_setglobal(L, name.c_str());
    
    lua_getglobal(L, "en");
    lua_pushstring(L, name.c_str());
    lua_pushvalue(L, -3);
    lua_settable(L, -3);
    lua_pop(L, 2);
}

void RegisterLuaType(lua_State *L, std::string registryKey, CFunction *ops,
        CFunction *methods, MethodMap &methodMap) {

    luaL_newmetatable(L, registryKey.c_str());
    lua_pushstring(L, "_id");
    lua_pushstring(L, registryKey.c_str());
    lua_settable(L, -3);
    for (unsigned i=0; ops[i].func; ++i) {
        lua_pushstring(L, ops[i].name);
        lua_pushcfunction(L, ops[i].func);
        lua_settable(L, -3);
    }
    lua_pop(L, 1);  // remove mt pointer
    
    for (unsigned i=0; methods[i].func; ++i) {
        methodMap[methods[i].name] = methods[i].func;
    }
}

bool IsFunc(lua_State *L, const char *funcname) {
    bool result;
    
    lua_getglobal(L, funcname);
    result = lua_isfunction(L, -1);
    lua_pop(L, 1);
    return result;
}

Error CallFunc(lua_State *L, const char *funcname, const Value& arg, Object *obj) {
    int retval;
    lua_getglobal(L, funcname);
    push_value(L, arg);
    pushobject(L, obj);
    retval=lua_pcall(L,2,0,0);
    if (retval!=0) // error
    {
        lua_setglobal (L, "_LASTERROR") ; //Set _LASTERROR to returned error message
    }
    return _lua_err_code(retval);
}

Error CallFunc(lua_State *L, const char *funcname, const ByteVec& arg) {
  int retval;
    lua_getglobal(L, funcname);
    lua_pushlstring (L, &arg[0], arg.size());
    retval=lua_pcall(L,1,0,0);
    if (retval!=0) // error
    {
        lua_setglobal (L, "_LASTERROR") ; //Set _LASTERROR to returned error message
    }
    return _lua_err_code(retval);
}

Error DoAbsoluteFile(lua_State *L, const string &filename)
{
    int oldtop = lua_gettop(L);
    int retval = luaL_loadfile(L, filename.c_str());
    if (retval!=0) // error
    {
        lua_setglobal (L, "_LASTERROR") ; //Set _LASTERROR to returned error message
    }
    else 
    {
      retval= lua_pcall(L, 0, 0, 0);
      if (retval!=0) // error
      {
        lua_setglobal (L, "_LASTERROR") ; //Set _LASTERROR to returned error message
      }
    }
    lua_settop(L, oldtop);
    return _lua_err_code(retval);
}
    
Error DoGeneralFile(lua_State *L, GameFS * fs, const string &filename)
{
    string completefn;
    if (fs->findFile(filename, completefn)) {
        return lua::DoAbsoluteFile(L, completefn);
    }
    else {
        return _lua_err_code(LUA_ERRFILE);
    }
}

Error Dofile(lua_State *L, const string &filename) 
{
    return lua::DoGeneralFile(L, app.resourceFS, filename);
}

Error DoSysFile(lua_State *L, const string &filename) 
{
    return lua::DoGeneralFile(L, app.systemFS, filename);
}

void CheckedDoFile (lua_State *L, GameFS * fs, std::string const& fname)
{
    string completefn;
    if (!fs->findFile(fname, completefn))
    {
        fprintf(stderr, _("Cannot find '%s'.\n"), fname.c_str());
        fprintf(stderr, _("Your installation may be incomplete or invalid.\n"));
        exit (1);
    }

    lua::Error status = lua::DoAbsoluteFile(L, completefn);
    if (status != lua::NO_LUAERROR) {
        fprintf(stderr, _("There was an error loading '%s'.\n"), completefn.c_str());
        fprintf(stderr, _("Your installation may be incomplete or invalid.\n"));
        fprintf(stderr, _("Error: '%s'\n"), lua::LastError(L).c_str());
        exit (1);
    }
}


Error Dobuffer (lua_State *L, const ByteVec &luacode) {
    int retval;
    const char *buffer = reinterpret_cast<const char *>(&luacode[0]);

    retval=luaL_loadbuffer(L, buffer, luacode.size(), "buffer");
    if (retval!=0) { // error
        lua_setglobal (L, "_LASTERROR") ; //Set _LASTERROR to returned error message
    } else {
        retval= lua_pcall(L, 0, 0, 0);
        if (retval!=0) { // error
            lua_setglobal (L, "_LASTERROR") ; //Set _LASTERROR to returned error message
        }
    }
    return _lua_err_code(retval);
}

string LastError (lua_State *L) {
    lua_getglobal (L, "_LASTERROR");
    if (lua_isstring(L,-1)) {
        return string (lua_tostring (L, -1));
    } else {
        return "Lua Error. No error message available.";
    }
}


Error DoSubfolderfile(lua_State *L, const string &basefolder, const string &filename) {
    std::list <string> fnames = app.resourceFS->findSubfolderFiles(basefolder, filename);
    int retval = 0;
    while (fnames.size() > 0) {
        int oldtop = lua_gettop(L);
        string fname = fnames.front();
        retval = luaL_loadfile(L, fname.c_str());
        if (retval!=0)  { // error
            lua_setglobal (L, "_LASTERROR") ; //Set _LASTERROR to returned error message
        } else {
            retval= lua_pcall(L, 0, 0, 0);
            if (retval!=0) { // error
                lua_setglobal (L, "_LASTERROR") ; //Set _LASTERROR to returned error message
            }
        }
        fnames.pop_front();
        lua_settop(L, oldtop);
    }
    return _lua_err_code(retval);
}

lua_State *GlobalState() 
{
    if (global_state == 0) {
        lua_State *L = global_state = lua_open();

        luaL_openlibs(L);
        CheckedDoFile(L, app.systemFS, "compat.lua");

        tolua_open(L);
        tolua_enigma_open(L);

        RegisterFuncs(L, globalfuncs);
    }
    return global_state;
}

void ShutdownGlobal() 
{
    assert (global_state);
    lua_close(global_state);
    global_state = 0;
}

#ifdef _MSC_VER
#define snprintf _snprintf
#endif



lua_State *InitLevel() 
{
    char buffer[255];

    lua_State *L = level_state = lua_open();
    luaL_dostring (L, "options={}");
    snprintf (buffer, sizeof(buffer), "options.Difficulty = %d", 
              server::GetDifficulty());
    luaL_dostring (L, buffer);

    luaL_openlibs(L);

    tolua_open(L);
    tolua_enigma_open(L);
    tolua_px_open(L);
    tolua_display_open(L);

    
    RegisterFuncs(L, levelfuncs);
    RegisterFuncs2(L, levelFuncs);

    // Create a new metatable for world objects
    RegisterLuaType(L, LUA_ID_OBJECT, objectOperations, objectMethods, objectMethodeMap);
    RegisterLuaType(L, LUA_ID_POSITION, positionOperations, positionMethods, positionMethodeMap);
    RegisterLuaType(L, LUA_ID_NAMEOBJ, namedObjOperations, namedObjMethods, namedObjMethodeMap);
    RegisterLuaType(L, LUA_ID_WORLD, worldOperations, worldMethods, worldMethodeMap);
    RegisterLuaType(L, LUA_ID_TILE, tileOperations, tileMethods, tileMethodeMap);
    RegisterLuaType(L, LUA_ID_TILES, tilesOperations, tilesMethods, tilesMethodeMap);
    RegisterLuaType(L, LUA_ID_GROUP, groupOperations, groupMethods, groupMethodeMap);
    
    pushNewNamedObj(L);
    RegisterObject(L, "no");
    
    pushNewWorld(L);
    RegisterObject(L, "wo");
    
    pushNewTiles(L);
    RegisterObject(L, "ti");
    
    return L;
}

lua_State *LevelState ()
{
    return level_state;
}

void ShutdownLevel() {
    if (level_state) {
        lua_close(level_state);
        level_state = 0;
    }
}

}} // namespace enigma::lua
