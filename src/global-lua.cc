/*
** Lua binding: global
** Generated automatically by tolua 4.0a on Wed Oct 27 11:10:28 2004.
*/

#include "tolua.h"

/* Exported function */
int tolua_global_open (lua_State* tolua_S);
void tolua_global_close (lua_State* tolua_S);

#include "enigma.hh"
using namespace enigma;

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
}

/* function: DefineFont */
static int toluaI_global_DefineFont00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,8)
 )
 goto tolua_lerror;
 else
 {
  const char* name = ((const char*)  tolua_getstring(tolua_S,1,0));
  const char* ttf_name = ((const char*)  tolua_getstring(tolua_S,2,0));
  int ttf_size = ((int)  tolua_getnumber(tolua_S,3,0));
  const char* bmf_name = ((const char*)  tolua_getstring(tolua_S,4,0));
  int r = ((int)  tolua_getnumber(tolua_S,5,255));
  int g = ((int)  tolua_getnumber(tolua_S,6,255));
  int b = ((int)  tolua_getnumber(tolua_S,7,255));
 {
  DefineFont(name,ttf_name,ttf_size,bmf_name,r,g,b);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DefineFont'.");
 return 0;
}

/* Open function */
int tolua_global_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_function(tolua_S,NULL,"DefineFont",toluaI_global_DefineFont00);
 return 1;
}
/* Close function */
void tolua_global_close (lua_State* tolua_S)
{
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"DefineFont");
}
