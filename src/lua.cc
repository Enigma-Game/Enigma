/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007,2008,2009 Ronald Lamprecht
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
#include "SoundEffectManager.hh"
#include "options.hh"
#include "player.hh"
#include "Inventory.hh"
#include "ItemHolder.hh"
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
#define LUA_ID_OBJECT    "_ENIGMAOBJECT"
#define LUA_ID_POSITION  "_POSITION"
#define LUA_ID_POSITIONS "_POSITIONS"
#define LUA_ID_WORLD     "_WORLDOBJECT"
#define LUA_ID_NAMEOBJ   "_NAMEDOBJECTS"
#define LUA_ID_GROUP     "_GROUP"
#define LUA_ID_POLIST    "_POLIST"
#define LUA_ID_TILES     "_TILES"
#define LUA_ID_TILE      "_TILE"
#define LUA_ID_DEFAULT   "_DEFAULT"
/**
 * The tile dictionary or function that should be used to resolve tile keys.
 */
#define LUA_ID_RESOLVER  "_RESOLVER"
#define LUA_ID_FLOORKEY  "_FLOORKEY"

using namespace std;

using ecl::round_down;
using ecl::strf;

using enigma::GridPos;
using enigma::Object;
using enigma::GridObject;
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

static bool is_polist(lua_State *L, int idx) {
    return lua_isuserdata(L,idx) && checkMetadata(L, idx, LUA_ID_POLIST);
}

static bool is_world(lua_State *L, int idx) {
    return lua_isuserdata(L,idx) && checkMetadata(L, idx, LUA_ID_WORLD);
}

static bool is_default(lua_State *L, int idx) {
    return lua_isuserdata(L,idx) && checkMetadata(L, idx, LUA_ID_DEFAULT);
}

static int userType(lua_State *L) {
    std::string type = "unknown";
    if (is_object(L, -1)) type = "object";
    else if (is_position(L, -1)) type = "position";
    else if (is_tile(L, -1)) type = "tile";
    else if (is_tiles(L, -1)) type = "tiles";
    else if (is_group(L, -1)) type = "group";
    else if (is_world(L, -1)) type = "world";
    else if (is_polist(L, -1)) type = "polist";
    else if (is_default(L, -1)) type = "default";
    
    lua_pushstring(L, type.c_str());
    return 1;
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
    for (int i = 1; it != objects.end(); ++it) {
        if (*it) {  // existing object not NULL
            if (unique.find(*it) == unique.end()) {
                unique.insert(*it);
                pushobject(L, *it);
                lua_pushvalue(L, -1);    // second copy of object
                lua_rawseti(L, -3, i);   // group[i] = obj
                lua_pushinteger(L, i);
                lua_rawset(L, -3);       // group[obj] = i
                i++;
            }
        }
    }

    lua_setmetatable(L, -2);    
    return 1;
}

static ecl::V2 toPosition(lua_State *L, int idx) {
    double x = 0;
    double y = 0;
    if (is_position(L, idx)) {  // position
        lua_getmetatable(L, idx);
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
    
    //TODO stack cleanup - remove x, y !? check usage first!
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

static int pushNewPolist(lua_State *L, PositionList positions) {
    // NULL objects and duplicates entries in the list will be eliminated
    int *udata;
    udata = (int *)lua_newuserdata(L, sizeof(int));   // group user object
    *udata = 1;
    
    lua_newtable(L);  // individual metatable copy
    luaL_getmetatable(L, LUA_ID_POLIST);
    // copy metatable template
    lua_pushnil(L);  // first key
    while (lua_next(L, -2) != 0) {
         // key is at index -2 and value at index -1
         lua_pushvalue(L, -2);   // copy key
         lua_insert(L, -2);      // insert key copy below value
         lua_settable(L, -5);    // individual metatable
    }    
    lua_pop(L, 1);  // remove metatable template

    int i = 1;
    for (PositionList::iterator it = positions.begin(); it != positions.end(); ++it) {
        // polist[i] = position
        Value v = *it;
        if (v) {  // existing object not NULL
            ecl::V2 p(v);
            lua_pushnumber(L, p[0]);
            lua_rawseti(L, -2, i++);   
            lua_pushnumber(L, p[1]);
            lua_rawseti(L, -2, i++);
        }
    }

    lua_setmetatable(L, -2);    
    return 1;
}

static PositionList toPositionList(lua_State *L, int idx) {
    PositionList positions;
    
    if (is_polist(L, idx)) {
        lua_getmetatable(L, idx);
        int numPositions = lua_objlen(L, -1) / 2;
        for (int i = 1; i <= numPositions; ++i) {
            lua_rawgeti(L, -1, 2 * i - 1);   // x
            lua_rawgeti(L, -2, 2 * i);       // y
            positions.push_back(ecl::V2(lua_tonumber(L, -2), lua_tonumber(L, -1)));
            lua_pop(L, 2);          // x, y       
        }
        lua_pop(L, 1);          // the metatable        
    }
    return positions;
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
        case Value::GRIDPOS:
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
            else if (is_group(L, idx))
                return Value(toObjectList(L, idx));
            else if (is_position(L, idx))
                return Value(toPosition(L, idx));
            else if (is_default(L, idx))
                return Value();
            else {
                lua_pushvalue(L, idx);
                userType(L);
                std::string msg = ecl::strf("Cannot convert userdata type '%s' to Value.", lua_tostring(L, -1));
                throwLuaError(L, msg.c_str());
            }
            break;            
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
            throwLuaError(L, "Cannot convert type to Value.");
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
    if (obj && key) {
        try {
            obj->setAttrChecked(key, to_value(L, 3));
        }  
        catch (const XLevelRuntime &e) {
            throwLuaError (L, e.what());
        }
        catch (...) {
            throwLuaError (L, "uncaught exception");
        }
    } else
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

    try {
        push_value(L, obj->getAttrChecked(key));
    }  
    catch (const XLevelRuntime &e) {
        throwLuaError (L, e.what());
    }
    catch (...) {
        throwLuaError (L, "uncaught exception");
    }
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

    try {
        push_value(L, Value(obj->get_kind()));
    }  
    catch (const XLevelRuntime &e) {
        throwLuaError (L, e.what());
    }
    catch (...) {
        throwLuaError (L, "uncaught exception");
    }
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
    Value result;  // nil
    Value v;  // nil
    if (lua_gettop(L) >= 3)
        v = to_value(L, 3);
    if (!msg)
        throwLuaError(L,"Illegal message");
    else if (obj) {
        try {
            std::string new_msg = lua::NewMessageName(L, obj, msg);
            result = SendMessage (obj, new_msg, v);
        }
        catch (const XLevelRuntime &e) {
            throwLuaError (L, e.what());
        }
        catch (...) {
            throwLuaError (L, "uncaught exception");
        }
    }
    push_value(L, result);
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
    server::ConstantForce = v;
    return 0;
}

static int
en_add_rubber_band (lua_State *L)
{
    Actor  *a1       = dynamic_cast<Actor*> (to_object(L, 1));
    Object *o2       = to_object(L, 2);
    Actor  *a2       = dynamic_cast<Actor*>(o2);
    Stone  *st       = dynamic_cast<Stone*>(o2);

    if (!a1)
        throwLuaError(L, "AddRubberBand: First argument must be an actor\n");

    Object *obj = MakeObject("ot_rubberband");
    obj->setAttr("anchor1", a1);
    if (a2)
        obj->setAttr("anchor2", a2);
    else if (st)
        obj->setAttr("anchor2", st);
    else {
        obj->dispose();
        throwLuaError(L, "AddRubberBand: Second argument must be actor or stone\n");
    }
    obj->setAttr("strength", lua_tonumber (L, 3));
    obj->setAttr("length", lua_tonumber (L, 4));
    obj->setAttr("threshold", lua_tonumber (L, 5));
    AddOther(dynamic_cast<Other *>(obj));

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
    if (!lua_isstring(L, 3))
        throwLuaError(L, "AddScramble: Third argument must be one character of \"wsen\"");

    const char *dir     = lua_tostring(L,3);
    const char *allowed = "wsen";
    const char *found   = strchr(allowed, dir[0]);

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
    try {
        server::LoadedProxy->loadDependency(id);
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

static int newPosition(lua_State *L, int idx =1) {
    // (pos|obj|table|(num,num))
    if (is_table(L, idx)) {  // table 
        lua_rawgeti(L, idx, 1);
        lua_rawgeti(L, idx, 2);
    } else if (is_position(L, idx)) {
        lua_getmetatable(L, idx);
        lua_rawgeti(L, -1, 1);
        lua_rawgeti(L, -2, 2);
        lua_remove(L, -3);     // remove metatable
    } else if (is_object(L, idx)) {
        lua_pushstring(L, "x");
        lua_gettable(L, idx); 
        lua_pushstring(L, "y");
        lua_gettable(L, idx); 
    }
    
    if ((lua_type(L, -2) != LUA_TNUMBER) || (lua_type(L, -1) != LUA_TNUMBER)) {
        throwLuaError(L, "Invalid new position value type");
        return 0;
    }    

    return pushNewPosition(L);
}

static int minusPosition(lua_State *L) {
    // pos  + ?? (some undocumented garbage is on the stack!!)    
    lua_getmetatable(L, 1);
    lua_rawgeti(L, -1, 1);
    double x = lua_tonumber(L, -1);
    lua_rawgeti(L, -2, 2);
    double y = lua_tonumber(L, -1);
    lua_pushnumber(L, -x);
    lua_pushnumber(L, -y);

    return pushNewPosition(L);
}

static int addPositionsBase(lua_State *L, double factorArg2, bool scalarMultiplication =false) {
    // (pos|obj|table|polist) +|- (pos|obj|table|polist)
    if (!(is_polist(L, 1) || is_polist(L, 2) )) {
        // add two positions resulting a new position
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
    } else {
        // add a position to a list of postions resulting a list of positions
        PositionList pl;
        PositionList newpl;
        ecl::V2 offset;
        bool found = false;
        if (is_polist(L, 1)) {
            pl = toPositionList(L, 1);
            found = true;
        } else if (!scalarMultiplication) {
            newPosition(L, 1);
            offset = toPosition(L, -1);
        }
        if (is_polist(L, 2)) {
            if (found)
                throwLuaError(L, "Addition of two position lists not supported");                
            pl = toPositionList(L, 2);
        } else if (!scalarMultiplication) {
            newPosition(L, 2);
            offset = toPosition(L, -1);
        }
        lua_pop(L, 1);
        for (PositionList::iterator itr = pl.begin(); itr != pl.end(); ++itr) {
             ecl::V2 newpos = offset + factorArg2 * (ecl::V2)(*itr);
             newpl.push_back(newpos);
        }
        return pushNewPolist(L, newpl);
    }
    return pushNewPosition(L);
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

static int addPositions(lua_State *L) {
    if (!(is_group(L, 1) || is_group(L, 2) ))
        // (pos|obj|table|polist) + (pos|obj|table|polist)
        return addPositionsBase(L, 1);
    else
        return joinGroup(L);
}
    
static int subPositions(lua_State *L) {
    // (pos|obj|table|polist) - (pos|obj|table|polist)
    return addPositionsBase(L, -1);
}

static int scalarMultPositions(lua_State *L) {
    // (num|polist) * (num|polist)
    double factor = 0;
    if (lua_isnumber(L, 1)) {
        factor = lua_tonumber(L, 1);
    } else if (lua_isnumber(L, 2)) {
        factor = lua_tonumber(L, 2);
    } else {
        throwLuaError(L, "Position List - scalar multiplications with invalid value type.");
    }
    return addPositionsBase(L, factor, true);
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
    if (lua_gettop(L) < 1 || !is_position(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
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

static int existsPosition(lua_State *L) {
    // position guaranteed
    if (lua_gettop(L) < 1 || !is_position(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
    lua_getmetatable(L, -1);
    lua_rawgeti(L, -1, 1);
    lua_rawgeti(L, -2, 2);
    int x = round_down<int>(lua_tonumber(L, -2));
    int y = round_down<int>(lua_tonumber(L, -1));
    lua_pushboolean(L, IsInsideLevel(GridPos(x, y)));
    return 1;
}

static int pushNewTile(lua_State *L, int numArgs, std::string key="") {
    // numArgs 1 or 2 of type (table|tile)
    int *udata;
    udata=(int *)lua_newuserdata(L,sizeof(int));   // user object
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
    if (key != "") {
        lua_pushstring(L, key.c_str());
        lua_rawseti(L, -2, 3);
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
            obj->setAttrChecked(key, to_value(L, -2));  // name set gets handeled by Object
         } else if (lua_tointeger(L, -1) == 2) {  // second entry without a string key is taken as name
             if (!lua_isnumber(L, -2) && lua_isstring(L, -2))
                 obj->setAttrChecked("name", lua_tostring(L, -2));
         }
         lua_pop(L, 2);  // remove copy key + value, leave original key for loop
    }    
}

static int setAttributes(lua_State *L) {
    // 
    if (lua_gettop(L) < 1 || !is_object(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
    if (lua_gettop(L) < 2 || !is_table(L, -1)) {
        throwLuaError(L, "Multiple set of object attributes requires a table as argument.");
        return 0;        
    }
    Object *obj = to_object(L, 1);
    lua_rawgeti(L, -1, 1);  // check if any unnamed attribute exists
    if (!lua_isnil(L, -1)) {
        throwLuaError(L, "Set attribute: encountered attribute without key name");
        return 0;
    }
    lua_pop(L, 1);  // remove unnamed attribute or nil
    setObjectAttributes(obj, L);
    return 0;
}

static int getStoneItemFloor(lua_State *L, Object::ObjectType ot) {
    // position|table|obj|(num,num)|group|polist
    if (is_world(L, 1))      // world method?
        lua_remove(L, 1);    // no need of context
    if (is_group(L, 1)) {
        ObjectList srcList = to_value(L, 1);
        ObjectList objects;
        for (ObjectList::iterator itr = srcList.begin(); itr != srcList.end(); ++itr) {
            GridPos  p;
            if (GridObject *gobj = dynamic_cast<GridObject*>(*itr)) {
                p = gobj->getOwnerPos();
            } else if (Actor *a = dynamic_cast<Actor*>(*itr)) {
                p = GridPos(a->get_pos());
            } else {
                continue;  // no valid position
            }
            Object *obj = NULL;
            switch (ot) {
                case Object::FLOOR :
                    obj = GetFloor(p); break;
                case Object::ITEM :
                    obj = GetItem(p); break;
                case Object::STONE :
                    obj = GetStone(p); break;
            }
            if (obj != NULL) 
                objects.push_back(obj);
        }
        pushNewGroup(L, objects);
    } else if (is_polist(L, 1)) {
        PositionList positions = toPositionList(L, 1);
        ObjectList objects;
        for (PositionList::iterator itr = positions.begin(); itr != positions.end(); ++itr) {
            GridPos p = *itr;
            Object *obj = NULL;
            switch (ot) {
                case Object::FLOOR :
                    obj = GetFloor(p); break;
                case Object::ITEM :
                    obj = GetItem(p); break;
                case Object::STONE :
                    obj = GetStone(p); break;
            }
            if (obj != NULL) 
                objects.push_back(obj);
        }
        pushNewGroup(L, objects);
    } else {
        newPosition(L);          // unify all arg types to a position
        lua_getmetatable(L, -1);            
        lua_rawgeti(L, -1, 1);
        int x = round_down<int>(lua_tonumber(L, -1));
        lua_rawgeti(L, -2, 2);
        int y = round_down<int>(lua_tonumber(L, -1));
        Object *obj = NULL;
        switch (ot) {
            case Object::FLOOR :
                obj = GetFloor(GridPos(x, y)); break;
            case Object::ITEM :
                obj = GetItem(GridPos(x, y)); break;
            case Object::STONE :
                obj = GetStone(GridPos(x, y)); break;
        }
        pushobject(L, obj);
    }
    return 1;
}
static int getFloor(lua_State *L) {
    // optional world, position|table|obj|(num,num)|group
    return getStoneItemFloor(L, Object::FLOOR);
}

static int getItem(lua_State *L) {
    // optional world, position|table|obj|(num,num)|group
    return getStoneItemFloor(L, Object::ITEM);
}

static int getStone(lua_State *L) {
    // optional world, position|table|obj|(num,num)|group
    return getStoneItemFloor(L, Object::STONE);
}

static int killObjectBase(lua_State *L) {
    Object *obj = to_object(L, -1);
    if (obj) {   // ignore not existing object
        GridObject *gobj;
        switch (obj->getObjectType()) {
            case Object::OTHER :
                KillOther(dynamic_cast<Other *>(obj));
                break;
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
                if (gobj->isDisplayable())
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
    if (lua_gettop(L) < 1 || !(is_object(L, 1) || is_group(L, 1))) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
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
    if (lua_gettop(L) < 1 || !is_object(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
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
    if (lua_gettop(L) < 1 || !is_object(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
    Object * obj = to_object(L, 1);
    lua_pushboolean(L, (obj != NULL));
    return 1;
}

static int objectGetKind(lua_State *L) { 
    // object type is guaranteed
    if (lua_gettop(L) < 1 || !is_object(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
    Object * obj = to_object(L, 1);
    lua_pushstring(L, (obj != NULL) ? obj->getKind().c_str() : "");
    return 1;
}

static int objectIsKind(lua_State *L) { 
    // object type is guaranteed
    if (lua_gettop(L) < 1 || !is_object(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
    Object * obj = to_object(L, 1);
    if (!lua_isstring(L, 2)) {
        throwLuaError(L,"Illegal kind - no string");
        return 0;
    }
    std::string kind = lua_tostring(L, 2);
    
    lua_pushboolean(L, (obj != NULL) ? obj->isKind(kind) : false);
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
            // check if message is valid, otherwise ignore message
            if (obj->validateMessage(msg, val))
                answer = SendMessage (obj, msg, val);
        }
        catch (const XLevelRuntime &e) {
            throwLuaError (L, e.what());
        }
//        catch (...) {
//            throwLuaError (L, "uncaught exception x");
//        }
    }
    push_value(L, answer);
    return 1;
}

static int objectMessage(lua_State *L) {
    if (lua_gettop(L) < 1 || !is_object(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
    if (lua_gettop(L) < 2) {
        throwLuaError (L, "Message missing");
        return 0;
    } else if (lua_gettop(L) == 2) {
        lua_pushnil(L);
    }
    return objectMessageBase(L);
}

static int groupMessage(lua_State *L) {
    if (lua_gettop(L) < 1 || !is_group(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
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
        if (lua_isnil(L, -3)) 
            lua_pushnil(L);     // do not send messages to nil - nil may occur in groups
        else
            objectMessageBase(L);   // end up with return value on top
    }
    return numObjects >= 1 ? 1 : 0;  // return last message value if group was not empty
}

std::vector<std::string> messageLIFO;

static int objectDirectMessage(lua_State *L) {
    // object, value
    std::string message = messageLIFO.back();
    messageLIFO.pop_back();
    if (lua_gettop(L) < 1 || !is_object(L, 1)) {
        throwLuaError(L, "Message error: no target object - may be caused by usage of '.' instead of ':'");
        return 0;
    }
    lua_pushstring(L, message.c_str());
    lua_insert(L, 2);    // message below optional value
    return objectMessage(L);
}

static int groupDirectMessage(lua_State *L) {
    // group, value
    std::string message = messageLIFO.back();
    messageLIFO.pop_back();
    if (lua_gettop(L) < 1 || !is_group(L, 1)) {
        throwLuaError(L, "Message error: no target group - may be caused by usage of '.' instead of ':'");
        return 0;
    }
    lua_pushstring(L, message.c_str());
    lua_insert(L, 2);    // message below optional value
    return groupMessage(L);
}


static int newGroup(lua_State *L) {
    // (table | grp | (obj[,obj]))
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
    } else if (is_group(L, 1)) {
        objects = toObjectList(L, -1);
    } else if (is_object(L, 1)) {
        for (int i = 1; i <= lua_gettop(L); i++) {
            objects.push_back(to_object(L, i));
        }
    } else if (lua_gettop(L) > 0) { 
        throwLuaError(L, "New Group - argument is no object");
        return 0;
    }
    return pushNewGroup(L, objects);
}

static int intersectGroupBase(lua_State *L, bool isIntersect, bool equal =false) {
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
    if (!equal)
        return pushNewGroup(L, objects);
    else {
        // check on equality
        // remove NULL objects first
        pushNewGroup(L, objects);
        objects = toObjectList(L, -1);
        return !objects.size();
    }    
}

static int intersectGroup(lua_State *L) {
    // (grp|obj) * (grp|obj)
    return intersectGroupBase(L, true);    
}

static int differenceGroup(lua_State *L) {
    // (grp|obj) - (grp|obj)
    return intersectGroupBase(L, false);    
}

static int groupEquality(lua_State *L) {
    lua_pushboolean(L, intersectGroupBase(L, false, true));
    return 1;
}

static int newPolist(lua_State *L) {
    // (grp | table)
    PositionList positions;
    if (is_group(L, 1)) {
        ObjectList ol = toObjectList(L, 1);
        for (ObjectList::iterator itr = ol.begin(); itr != ol.end(); ++itr) {
            // eliminate invalid object references and add every object as value of its own
            if (*itr != NULL) {
                positions.push_back(*itr);
            }
        }
    } else if (is_table(L, 1)) {
        int len = lua_objlen(L, 1);
        for (int i = 1; i <= len; i++) {
            lua_rawgeti(L, 1, i);
            positions.push_back(to_value(L, -1));
            lua_pop(L, 1);
        }
    } else {
        throwLuaError(L, "New Polist - false arguments");
        return 0;
    }
    return pushNewPolist(L, positions);
}

static int joinPolist(lua_State *L) {
    // (polist|po) + (polist|po)
    if (!((is_polist(L, 1) || is_position(L, 1)) &&  (is_polist(L, 2) || is_position(L, 2) ))) {
        throwLuaError(L, "Join Polist - argument is no polist, position or object");
        return 0;
    }
    PositionList positions;
    for (int j = 1; j <= 2; j++) {
        if (is_polist(L, j)) {
            PositionList addList = toPositionList(L, j);
            positions.insert(positions.end(), addList.begin(), addList.end());      
        } else if (is_position(L, j)) {
            positions.push_back(toPosition(L, j));
        }
    }
    return pushNewPolist(L, positions);
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
        if ((keyStr.find('_') != 0) && ((obj != NULL && obj->validateMessage(keyStr, Value(Value::DEFAULT))) 
                || (obj == NULL && (keyStr == "exists" || keyStr == "kill")))) {
            // it is a valid public message - try to send it
            messageLIFO.push_back(keyStr);
            lua_pushcfunction(L, objectDirectMessage);
        } else if (obj != NULL) {
            val = obj->getAttrChecked(keyStr);
            // user attribute, existing system attribute or nil if no object
            push_value(L, val);
        } else {
            // access of no longer existing object - ignore it and return nil
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
        obj->setAttrChecked(key, to_value(L, 3));
    else if (obj)  // ignore NULL objects
        throwLuaError(L, strf("SetAttrib: invalid object or attribute name '%s'", key).c_str());
    return 0;
}

static int objectSound(lua_State *L) {
    if (lua_gettop(L) < 1 || !is_object(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
    if (!(lua_gettop(L) >= 2 && lua_isstring(L, 2))) {
        throwLuaError(L, "Sound name expected as first argument");
        return 0;        
    }
    double vol = 1.0;
    if (lua_gettop(L) >=3 && lua_isnumber(L, 3))
        vol = lua_tonumber(L, 3);
    
    GridObject *gobj = dynamic_cast<GridObject*>(to_object(L, 1));
    if (gobj != NULL) {
        if (!gobj->sound_event(lua_tostring(L, 2), vol)) {
            //throwLuaError(L, strf("Can't find sound '%s'", soundname).c_str());
            // Don't throw an error when no sound file was found.
            // Remember that user sound sets might be incomplete, and
            // absolutely correct levels could throw an error here.
            // Instead, write the "silence string" to the command line:
            sound::WriteSilenceString(lua_tostring(L, 2));
        }
    }
    return 0;
}


static int xyPosition(lua_State *L) {
    // position guaranteed
    if (lua_gettop(L) < 1 || !is_position(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
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
        throwLuaError(L, "Invalid position modification of x or y coordinate");
        return 0;
//        lua_getmetatable(L, 1);
//        lua_pushvalue(L, -2);
//        if (keyStr == "x") {
//            lua_rawseti(L, -2, 1);
//            return 0;
//        } else {
//            lua_rawseti(L, -2, 2);
//            return 0;
//        }
    }
    
    throwLuaError(L, "Invalid position index");
    return 0;
}

MethodMap positionsMethodeMap;

static int dispatchPositionsReadAccess(lua_State *L) {
//    Log << "Positions read key - " << lua_tostring(L, 2) << "\n";
    if (!lua_isstring(L, 2)) {     // sideeffect: numbers are converted to string
        throwLuaError(L, "Positions: key is not a string");
        return 0;
    }
    std::string name = lua_tostring(L, 2);
    
    if (name.find_first_of("*?") != std::string::npos) {
        // search all objects that match the template
        PositionList positions = GetNamedPositionList(name);
        return pushNewPolist(L, positions);
    } else {    
        // search for a unique position
        Value v = GetNamedPosition(name);
//        Log << "will push position of '" << name << "' as " << ((ecl::V2)v)[0] << " - " << ((ecl::V2)v)[1] << "\n";
        push_value(L, v);
    }
    return 1;
}

static int dispatchPositionsWriteAccess(lua_State *L) {
//    Log << "Positions write key - " << lua_tostring(L, 2) << "\n";
    if (!lua_isstring(L, 2)) {     // sideeffect: numbers are converted to string
        throwLuaError(L, "Position naming: key is not a string");
        return 0;
    }
    if (!is_position(L, 3)){
        throwLuaError(L, "Position naming: value is not a position");
        return 0;
    }
    NamePosition(toPosition(L, 3), lua_tostring(L, 2));
    return 0;
}

static int pushNewPositions(lua_State *L) {
    // positions is a singleton
    
    int *udata;
    udata=(int *)lua_newuserdata(L,sizeof(int));   // positions user object
    *udata = 1;
    
    luaL_getmetatable(L, LUA_ID_POSITIONS);
    lua_rawgeti(L, -1, 1);
    if (lua_isnil(L, -1)) {
        lua_newtable(L);            // this is the contents table that stores the data
        lua_rawseti(L, -3, 1);
    }
    lua_pop(L, 1); //nil or content table
    lua_setmetatable(L, -2);
    return 1;
}

static int newPositions(lua_State *L) {
    // positions, (pos|obj|table|(num,num))
    lua_remove(L, 1);   // remove extra arg
    if (lua_gettop(L) < 1) {
        throwLuaError(L, "New position call without arguments");
        return 0;
    }
    if (is_group(L, 1)) {
        return newPolist(L);
    } else if (is_table(L, 1)) {
        bool coordinates = true;
        if (lua_objlen(L, -1) == 0)  // empty table as empty po list
            coordinates = false;
        else {
            lua_rawgeti(L, 1, 1);   // examine first element
            if (is_position(L, -1))
                coordinates = false;
            lua_pop(L, 1);
        }
        if (coordinates)
            return newPosition(L);
        else
            return newPolist(L);
    } else
        return newPosition(L);
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

static int registerWorldUserMethod(lua_State *L) {
    // object, name, function
    if (!(lua_isstring(L, 2) && lua_isfunction(L, 3))) {
        throwLuaError(L, "World register method argument error");
        return 0;
    }
    lua_getmetatable(L, 1);  // world metatable
    lua_getfield(L, -1, "_usermethods");
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);      // the bogus nil
        lua_newtable(L);    // create usermethods
        lua_pushvalue(L, -1);   // dup to be poped by setfield
        lua_setfield(L, -3, "_usermethods");
    }
    lua_pushvalue(L, 2);  // the key string as index
    lua_pushvalue(L, 3);  // the function as value
    lua_settable(L, -3);
    return 0;
}

static int dispatchWorldReadAccess(lua_State *L) {
    // world, key
    if (lua_isstring(L, 2)) {
        std::string keyStr = lua_tostring(L, 2);
        // TODO check string
        MethodMap::iterator iter = worldMethodeMap.find(keyStr);
        if (iter != worldMethodeMap.end()) {
            // call method
            lua_pushcfunction(L, iter->second);
        } else {
            if (Value v = WorldProxy::instance()->getAttr(keyStr))
                push_value(L, v);
            else {
                lua_getmetatable(L, 1);  // world metatable
                lua_getfield(L, -1, "_usermethods");
                if (!lua_isnil(L, -1)) {
                    lua_pushvalue(L, 2);
                    lua_gettable(L, -2);
                    if (lua_isfunction(L, -1))
                        return 1;
                }
                throwLuaError(L, "World read access without a valid key");
                return 0;
            }
        }
        return 1;
    } else {
        throwLuaError(L, "World read access without giving an attribute name");
        return 0;
    }
}


static int setObjectByTable(lua_State *L, double x, double y, bool onlyFloors = false, ItemHolder *itemHolder =NULL) {
    // table at -1 
    int xi = round_down<int>(x);
    int yi = round_down<int>(y);
    Object *obj = NULL;
    
    if (!lua_istable(L, -1)) {
        throwLuaError(L, ecl::strf("World: object set of wrong type '%s'", lua_typename(L, -1)).c_str());
        return 0;
    }
    if (lua_objlen(L, -1) == 0) {  // empty table as set nothing operation
        return 0;
    }
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
    
    if (name == "nil") {
        lua_pop(L, 1);   // object type
        return 0;
    } else if (name == "fl_nil") {
        if (!onlyFloors)
            KillFloor(GridPos(xi, yi));
        lua_pop(L, 1);   // object type
        return 0;
    } else if (name == "st_nil") {
        if (!onlyFloors)
            KillStone(GridPos(xi, yi));
        lua_pop(L, 1);   // object type
        return 0;
    } else if (name == "it_nil") {
        if (!onlyFloors)
            KillItem(GridPos(xi, yi));
        lua_pop(L, 1);   // object type
        return 0;
    }
    
    obj = MakeObject(name.c_str());
    if (obj == NULL) {
        throwLuaError(L, ecl::strf("World: unknown object name '%s'", name.c_str()).c_str());
        return 0;
    }
    lua_pop(L, 1);   // object type
    setObjectAttributes(obj, L);
    if (itemHolder != NULL) {
        if (obj->getObjectType() != Object::ITEM) {
            DisposeObject(obj);
            throwLuaError(L, "Attempt to add an object to a bag or inventory that is no item");
        } else {
            Item * it = dynamic_cast<Item *>(obj);
            if (!it->isPortable()) {
                DisposeObject(obj);
                throwLuaError(L, "Attempt to add an item to a bag or inventory that is not portable");
            } else {
                itemHolder->add_item(it);
                // just in case itemholder is displayed
                player::RedrawInventory(player::GetInventory(player::CurrentPlayer())); 
            }
        }
    } else {
        switch (obj->getObjectType()) {
            case Object::FLOOR :
                if (Value odd = obj->getAttr("checkerboard")) {
                    if ((xi+yi)%2 != (int)odd) {
                        DisposeObject(obj);
                        break;
                    }
                }
                SetFloor(GridPos(xi,yi), dynamic_cast<Floor *>(obj));
                break;
            case Object::STONE :
                if (!onlyFloors) {
                    if (Value odd = obj->getAttr("checkerboard")) {
                        if ((xi+yi)%2 != (int)odd) {
                            DisposeObject(obj);
                            break;
                        }
                    }
                    SetStone(GridPos(xi,yi), dynamic_cast<Stone *>(obj));
                } else {
                    DisposeObject(obj);
                }
                break;
            case Object::ITEM  :
                if (!onlyFloors) {
                    if (Value odd = obj->getAttr("checkerboard")) {
                        if ((xi+yi)%2 != (int)odd) {
                            DisposeObject(obj);
                            break;
                        }
                    }
                    SetItem(GridPos(xi,yi), dynamic_cast<Item *>(obj));
                } else {
                    DisposeObject(obj);
                }
                break;
            case Object::ACTOR :
                if (!onlyFloors) {
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
                } else
                    DisposeObject(obj);
                break;
            case Object::OTHER :
                if (x < 0)
                    AddOther(dynamic_cast<Other *>(obj));
                break;
            default :
                throwLuaError(L, "World set of unknown object");
        }
    }
    return 0;
}

static int setObjectByTile(lua_State *L, double x, double y, bool onlyFloors = false, ItemHolder *itemHolder =NULL) {
    // tile at -1
    
    // this is a recursive function - ensure enough space on the stack
    if (lua_gettop(L) >  LUA_MINSTACK - 5)
        lua_checkstack(L, 10);         // guarantee another 10 free slots
        
    lua_getmetatable(L, -1);
    lua_rawgeti(L, -1, 1);    // first tile part
    if (is_tile(L, -1))
        setObjectByTile(L, x, y, onlyFloors, itemHolder);
    else
        setObjectByTable(L, x, y, onlyFloors, itemHolder);
    lua_pop(L, 1);  // tile or table
    lua_rawgeti(L, -1, 2);    // second optional tile part
    if (!lua_isnil(L, -1)) {
        if (is_tile(L, -1))
            setObjectByTile(L, x, y, onlyFloors, itemHolder);
        else
            setObjectByTable(L, x, y, onlyFloors, itemHolder);
    }
    lua_pop(L, 2);  // tile or table or nil + metatable
    return 0;
}

static int evaluateKey(lua_State *L) {
    // resolver, key, x, y
    // (ti|function|table), string, int, int
    if (!(lua_isnumber(L, -1) && lua_isnumber(L, -2) && lua_isstring(L, -3) &&
            (is_tiles(L, -4) || lua_isfunction(L, -4) || is_table(L, -4)))) {
        throwLuaError(L, "Resolver with false argument types");
        return 0;
    }
    std::string key = lua_tostring(L, -3);
    int x = lua_tointeger(L, -2);
    int y = lua_tointeger(L, -1);
    
    if (is_tiles(L, -4)) {
        lua_pushvalue(L, -3);       // duplicate key
        lua_gettable(L, -5);        // get tile entry in table, remove key duplicate
        return 1;
    } else if (lua_isfunction(L, -4)) {
        lua_pushvalue(L, -4);       // duplicate function
        lua_pushvalue(L, -4);       // duplicate key
        lua_pushvalue(L, -4);       // duplicate x
        lua_pushvalue(L, -4);       // duplicate y
        int retval=lua_pcall(L, 3, 1, 0);     // resolver(key,x,y) ->  tile
        if (retval!=0) {
            throwLuaError(L, ecl::strf("Error within tile key resolver: \n  %s", lua_tostring(L, -1)).c_str());
            return 0;
        }
        // check result - must be tile or table or nil
        if (!(is_tile(L, -1) || is_table(L, -1) || lua_isnil(L, -1))) {
            throwLuaError(L, ecl::strf("World init undefined tile '%s' at %d, %d", 
                    key.c_str(), x, y).c_str());
            return 0;
        }
        return 1;
    } else {
        lua_rawgeti(L, -4, 1);      // get resolver implementation at index 1
        lua_pushvalue(L, -5);       // duplicate table as resolver context
        lua_pushcfunction(L, evaluateKey);  // this evaluator for subsequent calls
        lua_pushvalue(L, -6);       // duplicate key
        lua_pushvalue(L, -6);       // duplicate x
        lua_pushvalue(L, -6);       // duplicate y
        int retval=lua_pcall(L, 5, 1, 0);     // resolver(context,evaluator,key,x,y) ->  tile
        if (retval!=0) { 
            throwLuaError(L, ecl::strf("Error within tile key resolver: \n  %s", lua_tostring(L, -1)).c_str());
            return 0;
        }
        // check result - must be tile or table or nil
        if (!(is_tile(L, -1) || is_table(L, -1) || lua_isnil(L, -1))) {
            throwLuaError(L, ecl::strf("World init undefined tile '%s' at %d, %d", 
                    key.c_str(), x, y).c_str());
            return 0;
        }
        return 1;
    }
    return 0;
}

static int setObjectByKey(lua_State *L, std::string key, int j, int i, bool onlyFloors = false) {
    lua_getfield(L, LUA_REGISTRYINDEX, LUA_ID_RESOLVER);
    lua_pushvalue(L, -1);
    lua_pushstring(L, key.c_str());
    lua_pushinteger(L, j);
    lua_pushinteger(L, i);
    
    evaluateKey(L);

    if (lua_isnil(L, -1)) {
        throwLuaError(L, ecl::strf("World init undefined tile '%s' at %d, %d", 
                key.c_str(), j, i).c_str());
        return 0;
    }
    if (lua_istable(L, -1))
        setObjectByTable(L, j, i, onlyFloors);
    else
        setObjectByTile(L, j, i, onlyFloors);

    lua_pop(L, 5);  // tile, y, x, key, resolver

    if (GetFloor(GridPos(j, i)) == NULL) {
        lua_pushvalue(L, -1);
        lua_getfield(L, LUA_REGISTRYINDEX, LUA_ID_FLOORKEY);
        lua_pushinteger(L, j);
        lua_pushinteger(L, i);
        
        evaluateKey(L);
        
        if (lua_isnil(L, -1)) {
            throwLuaError(L, ecl::strf("World init undefined default tile at %d, %d",  j, i).c_str());
            return 0;
        }
        if (lua_istable(L, -1))
            setObjectByTable(L, j, i, true);
        else
            setObjectByTile(L, j, i, true);   // limit to floor set
            
        lua_pop(L, 5);  // default tile
        if (GetFloor(GridPos(j, i)) == NULL) {
            throwLuaError(L, ecl::strf("World no floor at %d, %d", j, i).c_str());
            return 0;
        }
    }
    lua_pop(L, 1);  // resolver
    return 0;
}

static int finalizeResolvers(lua_State *L) {
    // next resolver on top of stack
    
    // this is a recursive function - ensure enough space on the stack
    if (lua_gettop(L) >  LUA_MINSTACK - 5)
        lua_checkstack(L, 10);         // guarantee another 10 free slots
    
    if (is_table(L, -1)) {
        lua_rawgeti(L, -1, 3);      // get subresolver at index 3
        finalizeResolvers(L);       // finalize subresolver first 
        lua_pop(L, 1);              // subresolver
        
        lua_rawgeti(L, -1, 2);      // get resolver finalization at index 2
        if (!lua_isnil(L, -1)) {
            lua_pushvalue(L, -2);       // duplicate table as resolver context
            int retval=lua_pcall(L, 1, 0, 0);     // resolver(context,evaluator,key,x,y) ->  tile
            if (retval!=0) {
                throwLuaError(L, ecl::strf("Error within tile key resolver finalization:\n %s", 
                        lua_tostring(L, -1)).c_str());
                return 0;
            }
        } else {
            lua_pop(L, 1);  // nil
        }
    }
}

static int createWorld(lua_State *L) {
    // world, resolver, default key, map
    // world, (ti|function|table), string, table
    // world, (ti|function|table), string, number, number
    // world, (ti|function|table), libmap-map
    if (server::WorldSized) {
        throwLuaError(L, "World recreation not allowed");
        return 0;
    }
    
    // Is third argument a table? If so, check for an entry called
    // "defaultkey" and insert it into the stack at position 3.
    // This is consistent with libmap, as a metamethod will be
    // triggered, but can be used more generally as well.
    if (is_table(L, 3)) {
        lua_getfield(L, 3, "defaultkey");
        if (not lua_isstring(L, -1))
            throwLuaError(L, "World create with false argument types");
        lua_insert(L, 3);
    }
    
    // check arguments
    int width = 0;
    int height = 0;
    int keyLength = 1;
    std::string defaultKey;
    if (!(   (is_tiles(L, 2) || lua_isfunction(L, 2) || is_table(L, 2))
          &&  lua_isstring(L, 3)
          && (is_table(L, 4) || (lua_isnumber(L, 4) && lua_isnumber(L, 5))))) {
        throwLuaError(L, "World create with false argument types");
        return 0;
    }
    
    // remember top resolver and default key for missing floors
    lua_pushvalue(L, 2);
    lua_setfield(L, LUA_REGISTRYINDEX, LUA_ID_RESOLVER);  
    lua_pushvalue(L, 3);
    lua_setfield(L, LUA_REGISTRYINDEX, LUA_ID_FLOORKEY);  
    
    defaultKey = lua_tostring(L, 3);
    keyLength = defaultKey.length();
    std::vector<std::string> lines;
    Log << "createWorld  keyLength " << keyLength <<"\n";
    
    if (is_table(L, 4)) {
        height = lua_objlen(L, 4);
        for (int i = 1; i <= height; i++) {
            lua_pushinteger(L, i);
            lua_gettable(L, -2);
            if (!lua_isstring(L, -1)) {
                throwLuaError(L, "World create map has enties that are no strings");
                return 0;
            }
            lines.push_back(lua_tostring(L, -1));
            lua_pop(L, 1);
            width = ecl::Max(width, (int)lines.back().length());
        }
        if (width % keyLength != 0) {
            throwLuaError(L, "World create map row with odd length");
            return 0;
        }
        width = width/keyLength;
    } else {
        width = lua_tointeger(L, 4);
        height = lua_tointeger(L, 5);
        if (width <= 0 || height <= 0) {
            throwLuaError(L, "World create with bad size");
            return 0;
        }
        for (int i = 0; i < height; i++)
            lines.push_back(""); 
    }
    
    Log << "createWorld  - w " << width << "  - h " << height << "\n";
    Resize(width, height);
    display::ResizeGameArea(ecl::Min<int>(20, width), ecl::Min<int>(13, height));
    
    for (int i = 0; i < height; i++) {
        std::string &line = lines[i];
        int lineLength = line.length();
        if (lineLength % keyLength != 0) {
            throwLuaError(L, "World create map row with odd length");
            return 0;
        }
        lineLength = lineLength / keyLength;
        for (int j = 0; j < width; j++) {
            std::string key;
            bool isDefault = false;
            if (j < lineLength) {
                key = line.substr(j*keyLength, keyLength);  // tiles key
            } else {
                key = defaultKey;
                isDefault = true;
            }
            setObjectByKey(L, key, j, i, isDefault);
        }
    }
    
    lua_getfield(L, LUA_REGISTRYINDEX, LUA_ID_RESOLVER);
    finalizeResolvers(L);
    lua_pop(L, 1);           // top resolver
    
    lua_pushinteger(L, width);
    lua_pushinteger(L, height);
    return 2;
}

static int dispatchWorldWriteAccess(lua_State *L) {
    // [string] = value
    // [object|position|table] = table|tile
    // [object|position|table] = nil (=> error)
    if (lua_isstring(L, 2)) {
        std::string name = lua_tostring(L, 2);
        // TODO check string
        
        WorldProxy::instance()->setAttr(name, to_value(L, 3));
        return 0;
    } else if ((is_object(L, 2) || is_position(L, 2) || is_table(L, 2) || is_group(L, 2) || is_polist(L, 2)) && 
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
        } else if (is_polist(L, 2)) {
            PositionList positions = toPositionList(L, 2);
            for (PositionList::iterator itr = positions.begin(); itr != positions.end(); ++itr) {
                GridPos p = *itr;
                x = p.x;
                y = p.y;
                lua_pushvalue(L, 3);
                if (is_table(L, -1))
                    setObjectByTable(L, x, y);
                else // is tile
                    setObjectByTile(L, x, y);

                lua_pop(L, 1);          // the tile/table  
            }
            return 0;
        }
        
        lua_pushvalue(L, 3);
        if (is_table(L, -1))
            setObjectByTable(L, x, y);
        else // is tile
            setObjectByTile(L, x, y);
        return 0;
    } else if ((is_object(L, 2) || is_position(L, 2) || is_table(L, 2) || is_group(L, 2)) && 
            lua_isnil(L, 3)) {
        throwLuaError(L, "World write access with nil value (undefined tile?)");
        return 0;
    } else {
        throwLuaError(L, "World write access with bad index or bad value");
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

static int addOther(lua_State *L) {
    // world, table | tile
    if (lua_gettop(L) < 1 || !is_world(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
    if (is_tile(L, 2)  || is_table(L, 2)) {
        if (is_table(L, -1))
            setObjectByTable(L, -1, -1);
        else // is tile
            setObjectByTile(L, -1, -1);
    } else if (lua_isnumber(L, 2) && (is_tile(L, 3)  || is_table(L, 3))) {
        int player = lua_tointeger(L, 2);
        if (player != YIN && player != YANG)
            throwLuaError(L, "World attempt to add objects to not existing player");
        if (is_table(L, -1))
            setObjectByTable(L, -1, -1, false, player::GetInventory(player));
        else // is tile
            setObjectByTile(L, -1, -1, false, player::GetInventory(player));         
    } else if (is_object(L, 2) && (is_tile(L, 3)  || is_table(L, 3))) {
        ItemHolder *ih = dynamic_cast<ItemHolder *>(to_object(L, 2));
        if (ih == NULL)
            throwLuaError(L, "World attempt to add objects to an object that can not take other objects");
        if (is_table(L, -1))
            setObjectByTable(L, -1, -1, false, ih);
        else // is tile
            setObjectByTile(L, -1, -1, false, ih);         
    }
    return 0;
}

static int shuffleOxyd(lua_State *L) {
    // world, {table}  -- table with 1=(group|obj|name), [2=(group|obj|name], 
    //                               min=[number], max=[number], circular=true, linear=true
    //                               log=("solution"|"count"|"all") 
    if (lua_gettop(L) < 1 || !is_world(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
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

static int externalData(lua_State *L) {
    // world, path  -- 
    if (!lua_isstring(L, 2)) {
            throwLuaError(L, "External data access without path id string");
            return 0;
    }
    lua_pushstring(L, server::LoadedProxy->getExternalData(lua_tostring(L, 2)).c_str());
    return 1;
}

MethodMap tileMethodeMap;

static int dispatchTileWriteAccess(lua_State *L) {
    throwLuaError(L, "Tile: illegal write access");
    return 0;
}

static int dispatchTileReadAccess(lua_State *L) {
    // tile, key
    if (lua_isstring(L, 2)) {
        std::string keyStr = lua_tostring(L, 2);
        MethodMap::iterator iter = tileMethodeMap.find(keyStr);
        if (iter != tileMethodeMap.end()) {
            // call method
            lua_pushcfunction(L, iter->second);
            return 1;
        }
    }
    throwLuaError(L, "Tile: illegal read access");
    return 0;
}

static void tileDeclForTable(lua_State *L) {
    // table (declaration), table (the result copy)
    if (lua_objlen(L, -2) == 0)   // empty declaration table 
        return;
    // deep copy of all table entries
    lua_newtable(L);    // the table copy for the result
    lua_pushnil(L);  // first key
    while (lua_next(L, -4) != 0) {
         // key is at index -2 and value at index -1
         // ignore all keys besides strings - ignore key 1, the object type
         lua_pushvalue(L, -2); // a copy of key for work
         if (lua_istable(L, -2)) {
            // a token as value - we need a deep copy
            lua_newtable(L);    // the table copy for the result
            lua_pushnil(L);  // first key
            while (lua_next(L, -4) != 0) {
                 // key is at index -2 and value at index -1
                 // ignore all keys besides strings - ignore key 1, the object type
                 lua_pushvalue(L, -2); // a copy of key for work
                 lua_pushvalue(L, -2); // a copy of value for work - no tokens within tokens allowed!
                 lua_rawset(L, -5);    // store
                 lua_pop(L, 1);  // remove value, leave original key for loop
            }
         } else {
            // all other values are no references - a copy is sufficient
            lua_pushvalue(L, -2); // a copy of value for work
         }
         lua_rawset(L, -5);    // store
         lua_pop(L, 1);  // remove original value, leave original key for loop
    }
    
    int resultsize = lua_objlen(L, -2);   // last index of result table
    lua_rawseti(L, -2, resultsize + 1);   // append tile key
    return;
}

static int tileDeclForTile(lua_State *L, bool isTop = false) {
    // tile, table (the result)
    
    // this is a recursive function - ensure enough space on the stack
    if (lua_gettop(L) >  LUA_MINSTACK - 5)
        lua_checkstack(L, 10);         // guarantee another 10 free slots
        
    lua_getmetatable(L, -2);
    if (!isTop) {
        lua_rawgeti(L, -1, 3);    // tile key or nil
        if (!lua_isnil(L, -1)) {
            int resultsize = lua_objlen(L, -3);   // last index of result table
            lua_rawseti(L, -3, resultsize + 1);   // append tile key
            lua_pop(L, 1);     // metatable
            return 0;
        }
        lua_pop(L, 1);     // tile key 
    }
    
    lua_rawgeti(L, -1, 1);    // first tile part
    lua_pushvalue(L, -3);     // result table
    if (is_tile(L, -2))
        tileDeclForTile(L);
    else
        tileDeclForTable(L);
    lua_pop(L, 2);  // tile or table and 
    lua_rawgeti(L, -1, 2);    // second optional tile part
    if (!lua_isnil(L, -1)) {
        lua_pushvalue(L, -3);     // result table
        if (is_tile(L, -2))
            tileDeclForTile(L);
        else
            tileDeclForTable(L);
        lua_pop(L, 1);  // result table
    }
    lua_pop(L, 2);  // tile or table or nil + metatable
    return 0;
}

static int tileDeclaration(lua_State *L) {
    // tile
    if (lua_gettop(L) != 1 || !is_tile(L, 1)) {
        throwLuaError(L, "Tile declaration with illegal arguments: possible syntax error - usage of '.' instead of ':'");
        return 0;        
    }
    lua_newtable(L); // the result
    tileDeclForTile(L, true);
    return 1;
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
    lua_rawgeti(L, -1, 1);    // content table
    lua_pushvalue(L, 2);      // copy key
    lua_rawget(L, -2);        // check for existing entry in table
    return 1;
}

static int dispatchTilesWriteAccess(lua_State *L) {
//    Log << "Tiles write key - " << lua_tostring(L, 2) << "\n";
    if (server::EnigmaCompatibility < 1.10) {
        throwLuaError(L, "Mismatch of new API 2 syntax with level compatibility < 1.10");
        return 0;
    }
    if (!lua_isstring(L, 2)) {     // sideeffect: numbers are converted to string
        throwLuaError(L, "Tiles: key is not a string");
        return 0;
    }
    if (is_table(L, 3) || is_tile(L, 3)) {
        // convert table to a tile
        pushNewTile(L, 1, lua_tostring(L, 2));
        lua_remove(L, 3);   
    } else {
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
    if (!(lua_isstring(L, 2)) || lua_type(L, 2) == LUA_TNUMBER) {
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
            obj->setAttrChecked(name, to_value(L, 3));
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
        if (i < 0)  // backward indices
            i = size + 1 + i;
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
        lua_pushvalue(L, 2);   // copy object as key
        lua_rawget(L, -2);     // get index
        if (lua_isnil(L, -1)) {
            // another copy of the object is used that results in another metatable hash,
            // we need to compare with all objects (call it a workaround for a lua caveat or bad design)
            Object *obj1 = to_object(L, 2);
            int numObjects = lua_objlen(L, -2);
            for (int i = 1; i <= numObjects; ++i) {
                lua_rawgeti(L, -2, i);  // the object
                Object *obj2 = to_object(L, -1);
                lua_pop(L, 1);          // the object        
                if (obj1 == obj2) {
                    lua_pushinteger(L, i);
                    break;
                }
            }            
        }
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

static int subGroup(lua_State *L) {
    // group
    if (lua_gettop(L) < 1 || !is_group(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
    ObjectList oldList = toObjectList(L, 1);
    int length = oldList.size();
    int start = 0;
    int end = length;
    if (lua_gettop(L) == 2 && lua_isnumber(L, 2)) {
        end = lua_tointeger(L, 2);
        if (end < 0) {
            start = length - end;
            end = length;
        }
    }
    if (lua_gettop(L) == 3 && lua_isnumber(L, 2) && lua_isnumber(L, 3)) {
        start = lua_tointeger(L, 2) - 1;
        end = lua_tointeger(L, 3);
        if (end < 0) {
            end = start - end;
        }
    }
    ObjectList newList;
    int i = 0;
    for (ObjectList::iterator itr = oldList.begin(); itr != oldList.end(); ++itr, i++) {
        if (i >= start && i < end)
            newList.push_back(*itr);
    }
    return pushNewGroup(L, newList);
}

static int shuffleGroup(lua_State *L) {
    // group
    if (lua_gettop(L) < 1 || !is_group(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
    ObjectList oldSort = toObjectList(L, 1);
    ObjectList newSort;
    std::vector<Object *> shuffleVector;
    for (ObjectList::iterator itr = oldSort.begin(); itr != oldSort.end(); ++itr)
        shuffleVector.push_back(*itr);
    int members = shuffleVector.size();
    for (int i = members - 1; i > 0; i--) {
        int j = IntegerRand(0, i);
        Object * obj = shuffleVector[j];
        shuffleVector[j] = shuffleVector[i];
        shuffleVector[i] = obj;
    }
    for (std::vector<Object *>::iterator itr = shuffleVector.begin(); itr != shuffleVector.end(); ++itr)
        newSort.push_back(*itr);
    return pushNewGroup(L, newSort);
}

static int sortGroup(lua_State *L) {
    // group
    if (lua_gettop(L) < 1 || !is_group(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;        
    }
    std::string command;
    if (lua_gettop(L) >= 2 && lua_isstring(L, 2)) {
        command = lua_tostring(L, 2);
    }
    ObjectList oldSort = toObjectList(L, 1);
    ObjectList newSort;
    if (command == "circular") {
        std::map<double, Object *> sortMap;
        double cx = 0;
        double cy = 0;  // center
        int num = 0;
        for (ObjectList::iterator itr = oldSort.begin(); itr != oldSort.end(); ++itr) {
            if (*itr != NULL) {
                ecl::V2 pos = Value(*itr);
                cx += pos[0];
                cy += pos[1];
                num++;
            } 
        }
        if (num > 0) {
            cx = cx/num;
            cy = cy/num;
            for (ObjectList::iterator itr = oldSort.begin(); itr != oldSort.end(); ++itr) {
                if (*itr != NULL) {
                    ecl::V2 pos = Value(*itr);
                    double alpha = std::atan2(pos[1] - cy, pos[0] - cx); 
                    sortMap[alpha] = *itr;
                }
            }
            for (std::map<double, Object *>::iterator itr = sortMap.begin(); itr != sortMap.end(); ++itr) {
                newSort.push_back(itr->second);
            }
        }
    } else if (command == "linear") {
        ecl::V2 dir;
        std::multimap<double, Object *> sortMap;
        if (lua_gettop(L) == 2 && oldSort.size() >= 2) {
            Object *front = oldSort.front();
            oldSort.pop_front();
            dir = (ecl::V2)(Value(oldSort.front())) - (ecl::V2)(Value(front));
            oldSort.push_front(front);
        } else if (lua_gettop(L) == 3 && is_position(L, 3)) {
            dir = toPosition(L, 3);
        } else {
            return pushNewGroup(L, oldSort);
        }
        dir.normalize();
        for (ObjectList::iterator itr = oldSort.begin(); itr != oldSort.end(); ++itr) {
            double d = dir * (ecl::V2)(Value(*itr));
            sortMap.insert(std::make_pair(d, *itr));
        }
        for (std::multimap<double, Object *>::iterator itr = sortMap.begin(); itr != sortMap.end(); ++itr)
            newSort.push_back(itr->second);
        
    } else {
        // default sort lexical by name
        std::map<std::string, Object *> sortMap;
        for (ObjectList::iterator itr = oldSort.begin(); itr != oldSort.end(); ++itr)
            sortMap[((*itr)->getAttr("name")).to_string()] = *itr;
        for (std::map<std::string, Object *>::iterator itr = sortMap.begin(); itr != sortMap.end(); ++itr)
            newSort.push_back(itr->second);
    }
    return pushNewGroup(L, newSort);
}

static int nearestGroup(lua_State *L) {
    // group
    if (lua_gettop(L) < 1 || !is_group(L, 1)) {
        throwLuaError(L, "Syntax error - usage of '.' instead of ':'");
        return 0;
    }
    if (!(lua_gettop(L) == 2 && is_object(L, 2))) {
        throwLuaError(L, "Syntax error: expected object reference as first argument");
        return 0;
    }

    ObjectList oldList = toObjectList(L, 1);
    Object *center = to_object(L, 2);
    if (center == NULL) {
        throwLuaError(L, "Error: reference object is no longer existing");
    }
    double mindist = -1;
    Object *candidate = NULL;
    
    for (ObjectList::iterator itr = oldList.begin(); itr != oldList.end(); ++itr) {
        if (mindist < 0) {
            candidate = *itr;
            mindist = center->squareDistance(candidate);
        } else {
            double newdist = center->squareDistance(*itr);
            
            // replace last candidate by new closer object, choose a unique candidate
            if (mindist > newdist || ((mindist == newdist) && (*itr)->isSouthOrEastOf(candidate))) {
                candidate = *itr;
                mindist = newdist;
            }
        }
    }
    pushobject(L, candidate);
    return 1;
}

MethodMap polistMethodeMap;

static int dispatchPolistWriteAccess(lua_State *L) {
    throwLuaError(L, "PoList: illegal write access");
    return 0;
}

static int dispatchPolistReadAccess(lua_State *L) {
    if (!(lua_isnumber(L, 2))) {
        throwLuaError(L, "Position List: illegal read access");
        return 0;
    }
    
    if (true /* lua_isnumber(L, 2) */) {
        int i = lua_tointeger(L, 2);
        lua_getmetatable(L, 1);
        int size = lua_objlen(L, -1)  / 2;
        if (i < 0)  // backward indices
            i = size + 1 + i;
        if (i >= 1 && i <= size) {
            lua_rawgeti(L, -1, 2*i - 1);
            lua_rawgeti(L, -2, 2*i);
            pushNewPosition(L);
        } else
            lua_pushnil(L);
    }
    return 1;
}

static int lengthPolist(lua_State *L) {
    lua_getmetatable(L, 1);
    int size = lua_objlen(L, -1);
//    Log << "Length List " << size << "\n";
    lua_pushinteger(L, size / 2);
    return 1;
}

static int polistEquality(lua_State *L) {
    // (polist) == (polist)
    if (!(is_polist(L, 1) &&  is_polist(L, 2))) {
        throwLuaError(L, "Comparison of polists - argument is no polist");
        return 0;
    }
    PositionList pl1 = toPositionList(L, 1);
    PositionList pl2 = toPositionList(L, 2);
    PositionList::iterator itr2 = pl2.begin();
    for (PositionList::iterator itr1 = pl1.begin(); itr1 != pl1.end(); ++itr1) {
         if (itr2 == pl2.end() || ((ecl::V2)(*itr1) !=  (ecl::V2)(*itr2))) {
             lua_pushboolean(L, false);
             return 1;
         }
         ++itr2;
    }
    lua_pushboolean(L, itr2 == pl2.end());
    return 1;
}

MethodMap defaultMethodeMap;

static int pushNewDefault(lua_State *L) {
    // tiles is a singleton
    
    int *udata;
    udata=(int *)lua_newuserdata(L,sizeof(int));   // user object
    *udata = 1;  // dummy
    
    luaL_getmetatable(L, LUA_ID_DEFAULT);
    lua_setmetatable(L, -2);
    return 1;
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
    {userType,                      "usertype"},
    {newGroup,                      "grp"},
//    {newPosition,                   "po"},
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
    {objectIsKind,                  "is"},
    {objectGetKind,                 "kind"},
    {killObject,                    "kill"},
    {objectMessage,                 "message"},
    {objectSound,                   "sound"},
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
    {minusPosition,                 "__unm"},      //  -obj
    {joinPolist,                    "__concat"},   //  obj .. obj
    {0,0}
};

static CFunction positionMethods[] = {
    {xyPosition,                    "xy"},
    {gridAlignPosition,             "grid"},
    {existsPosition,                "exists"},
    {0,0}
};

static CFunction positionsOperations[] = {
    {dispatchPositionsWriteAccess,  "__newindex"}, //  obj[key]=value
    {dispatchPositionsReadAccess,   "__index"},    //  obj[key]
    {newPositions,                  "__call"},
    {0,0}
};

static CFunction positionsMethods[] = {
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
    {createWorld,                   "__call"},
    {0,0}
};

static CFunction worldMethods[] = {
    {createWorld,                   "create"},
    {registerWorldUserMethod,       "_register"},
    {evaluateKey,                   "_evaluate"},
    {addOther,                      "add"},
    {getFloor,                      "fl"},
    {getItem,                       "it"},
    {getStone,                      "st"},
    {shuffleOxyd,                   "shuffleOxyd"},
    {externalData,                  "externalData"},
    {0,0}
};

static CFunction tileOperations[] = {
    {dispatchTileWriteAccess,       "__newindex"}, //  obj[key]=value
    {dispatchTileReadAccess,        "__index"},    //  obj[key]
    {appendTile,                    "__concat"},   //  obj .. obj
    {0,0}
};

static CFunction tileMethods[] = {
    {tileDeclaration,               "_declaration"},
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
    {groupEquality,                 "__eq"},       //  ==
    {iteratorGroup,                 "__call"},
    {0,0}
};

static CFunction groupMethods[] = {
    {groupMessage,                  "message"},
    {killObject,                    "kill"},
    {shuffleGroup,                  "shuffle"},
    {sortGroup,                     "sort"},
    {subGroup,                      "sub"},
    {nearestGroup,                  "nearest"},
    {0,0}
};

static CFunction polistOperations[] = {
    {dispatchPolistWriteAccess,     "__newindex"}, //  obj[key]=value
    {dispatchPolistReadAccess,      "__index"},    //  obj[key]
    {lengthPolist,                  "__len"},      //  #obj
    {addPositions,                  "__add"},      //  obj + obj
    {subPositions,                  "__sub"},      //  obj - obj
    {joinPolist,                    "__concat"},   //  obj .. obj
    {scalarMultPositions,           "__mul"},      //  obj * obj
    {polistEquality,                "__eq"},       //  ==
    {0,0}
};

static CFunction polistMethods[] = {
    {0,0}
};

static CFunction defaultOperations[] = {
    {0,0}
};

static CFunction defaultMethods[] = {
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

Error CallFunc(lua_State *L, const std::string funcpath, const Value& arg, Object *obj, bool expectFunction) {
    int retval;
    const char *funcname = funcpath.c_str();
    
//    Uint32 start_tick_time = SDL_GetTicks();   // meassure time for level loading
    if (funcpath.find('.')) {
        std::list<std::string> nodes;
        ecl::split_copy(funcpath, '.', std::back_inserter(nodes));
        for (std::list<std::string>::iterator itr = nodes.begin(); itr != nodes.end();) {
            if (itr == nodes.begin()) {
                lua_getglobal(L, (*itr).c_str());
            } else {
                lua_getfield(L, -1, (*itr).c_str());
                lua_remove(L, -2);
            }
            ++itr;
            if (itr != nodes.end() && !lua_istable(L, -1)) {
                if (expectFunction) {
                    lua_pushstring(L, ecl::strf("Super domain of '%s' is not a table!", (*itr).c_str()).c_str());
                    lua_setglobal(L, "_LASTERROR");
                    return ERRRUN;
                } else {
                    lua_pop(L, 1);
                    return NO_LUAERROR;
                }
            }
        }
    } else {
        lua_getglobal(L, funcname);
    }
    
    if (lua_isfunction(L, -1)) {
        push_value(L, arg);
        pushobject(L, obj);
        retval = lua_pcall(L, 2, 0, 0);
        if (retval != 0)  {  // error
            lua_setglobal(L, "_LASTERROR") ; //Set _LASTERROR to returned error message
        }
//        double exectime = (SDL_GetTicks() - start_tick_time)/1000.0;
//        if (exectime > -1.000)
//            Log << ecl::strf("Lua callback '%s' did take %g seconds\n", funcname, exectime);
        return _lua_err_code(retval);
    } else {
        if (expectFunction) {
            lua_pushstring(L, "No function of given name exists.");
            lua_setglobal(L, "_LASTERROR");
            return ERRRUN;
        } else {
            lua_pop(L, 1);
            return NO_LUAERROR;
        }
    }
}

std::string NewMessageName(lua_State *L, const Object *obj, const std::string &message) {
    std::string result;
    lua_getglobal(L, "MessageRenaming");
    if (lua_istable(L, -1)) {
        lua_pushstring(L, ecl::strf("%s__%s", obj->getClass().c_str(), message.c_str()).c_str());
        lua_gettable(L, -2);
        if (!lua_isnil(L, -1))
            result = lua_tostring(L, -1);
        else
            result = message;
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    return result;
}

void SetDefaultFloor(lua_State *L, int x, int y) {
    lua_getfield(L, LUA_REGISTRYINDEX, LUA_ID_FLOORKEY);
    std::string key = lua_tostring(L, -1);
    setObjectByKey(L, key, x, y, true);
    lua_pop(L, 1);
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

std::string LastError(lua_State *L) {
    lua_getglobal (L, "_LASTERROR");
    if (lua_isstring(L,-1)) {
        std::string msg = lua_tostring (L, -1);
        lua_pop(L, 1);
        return msg;
    } else {
        lua_pop(L, 1);
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



lua_State *InitLevel(int api) 
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
    if (api == 1) {
        tolua_px_open(L);
        tolua_display_open(L);
    }
    
    RegisterFuncs(L, levelfuncs);
    RegisterFuncs2(L, levelFuncs);

    // Create a new metatable for world objects
    RegisterLuaType(L, LUA_ID_OBJECT, objectOperations, objectMethods, objectMethodeMap);
    RegisterLuaType(L, LUA_ID_POSITION, positionOperations, positionMethods, positionMethodeMap);
    RegisterLuaType(L, LUA_ID_POSITIONS, positionsOperations, positionsMethods, positionsMethodeMap);
    RegisterLuaType(L, LUA_ID_NAMEOBJ, namedObjOperations, namedObjMethods, namedObjMethodeMap);
    RegisterLuaType(L, LUA_ID_WORLD, worldOperations, worldMethods, worldMethodeMap);
    RegisterLuaType(L, LUA_ID_TILE, tileOperations, tileMethods, tileMethodeMap);
    RegisterLuaType(L, LUA_ID_TILES, tilesOperations, tilesMethods, tilesMethodeMap);
    RegisterLuaType(L, LUA_ID_GROUP, groupOperations, groupMethods, groupMethodeMap);
    RegisterLuaType(L, LUA_ID_POLIST, polistOperations, polistMethods, polistMethodeMap);
    RegisterLuaType(L, LUA_ID_DEFAULT, defaultOperations, defaultMethods, defaultMethodeMap);
    
    pushNewNamedObj(L);
    RegisterObject(L, "no");
    
    pushNewWorld(L);
    RegisterObject(L, "wo");
    
    pushNewTiles(L);
    RegisterObject(L, "ti");
    
    pushNewPositions(L);
    RegisterObject(L, "po");

    pushNewDefault(L);
    RegisterObject(L, "DEFAULT");
    
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
