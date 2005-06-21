/*
** Lua binding: editor
** Generated automatically by tolua 4.0a on Mon Jul 14 09:51:26 2003.
*/

#include "tolua.h"

/* Exported function */
int tolua_editor_open (lua_State* tolua_S);
void tolua_editor_close (lua_State* tolua_S);

#include "editor.hh"
using namespace editor;

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
}

/* function: DefineFloorGroup */
static int toluaI_editor_editor_DefineFloorGroup00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TTABLE,0) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  const char* name = ((const char*)  tolua_getstring(tolua_S,1,0));
  const char* descr = ((const char*)  tolua_getstring(tolua_S,2,0));
  int nentries = ((int)  tolua_getnumber(tolua_S,3,0));
  const char** entries = (const char**) malloc(nentries*sizeof(char*));
 {
 if (!tolua_arrayistype(tolua_S,4,LUA_TSTRING,nentries,0))
 goto tolua_lerror;
 else
 {
 int i;
 for(i=0; i<nentries;i++)
  entries[i] = ((const char*)  tolua_getfieldstring(tolua_S,4,i+1,0));
 }
 }
 {
  DefineFloorGroup(name,descr,nentries,entries);
 }
 {
 int i;
 for(i=0; i<nentries;i++)
 tolua_pushfieldstring(tolua_S,4,i+1,(const char*) entries[i]);
 }
 free(entries);
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DefineFloorGroup'.");
 return 0;
}

/* function: DefineItemGroup */
static int toluaI_editor_editor_DefineItemGroup00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TTABLE,0) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  const char* name = ((const char*)  tolua_getstring(tolua_S,1,0));
  const char* descr = ((const char*)  tolua_getstring(tolua_S,2,0));
  int nentries = ((int)  tolua_getnumber(tolua_S,3,0));
  const char** entries = (const char**) malloc(nentries*sizeof(char*));
 {
 if (!tolua_arrayistype(tolua_S,4,LUA_TSTRING,nentries,0))
 goto tolua_lerror;
 else
 {
 int i;
 for(i=0; i<nentries;i++)
  entries[i] = ((const char*)  tolua_getfieldstring(tolua_S,4,i+1,0));
 }
 }
 {
  DefineItemGroup(name,descr,nentries,entries);
 }
 {
 int i;
 for(i=0; i<nentries;i++)
 tolua_pushfieldstring(tolua_S,4,i+1,(const char*) entries[i]);
 }
 free(entries);
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DefineItemGroup'.");
 return 0;
}

/* function: DefineStoneGroup */
static int toluaI_editor_editor_DefineStoneGroup00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TTABLE,0) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  const char* name = ((const char*)  tolua_getstring(tolua_S,1,0));
  const char* descr = ((const char*)  tolua_getstring(tolua_S,2,0));
  int nentries = ((int)  tolua_getnumber(tolua_S,3,0));
  const char** entries = (const char**) malloc(nentries*sizeof(char*));
 {
 if (!tolua_arrayistype(tolua_S,4,LUA_TSTRING,nentries,0))
 goto tolua_lerror;
 else
 {
 int i;
 for(i=0; i<nentries;i++)
  entries[i] = ((const char*)  tolua_getfieldstring(tolua_S,4,i+1,0));
 }
 }
 {
  DefineStoneGroup(name,descr,nentries,entries);
 }
 {
 int i;
 for(i=0; i<nentries;i++)
 tolua_pushfieldstring(tolua_S,4,i+1,(const char*) entries[i]);
 }
 free(entries);
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DefineStoneGroup'.");
 return 0;
}

/* Open function */
int tolua_editor_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_module(tolua_S,"editor");
 tolua_function(tolua_S,"editor","DefineFloorGroup",toluaI_editor_editor_DefineFloorGroup00);
 tolua_function(tolua_S,"editor","DefineItemGroup",toluaI_editor_editor_DefineItemGroup00);
 tolua_function(tolua_S,"editor","DefineStoneGroup",toluaI_editor_editor_DefineStoneGroup00);
 return 1;
}
/* Close function */
void tolua_editor_close (lua_State* tolua_S)
{
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"editor");
}
