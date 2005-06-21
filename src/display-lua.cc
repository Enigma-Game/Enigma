/*
** Lua binding: display
** Generated automatically by tolua 4.0a on Wed Mar 16 15:43:49 2005.
*/

#include "tolua.h"

/* Exported function */
int tolua_display_open (lua_State* tolua_S);
void tolua_display_close (lua_State* tolua_S);

#include "display.hh"
#include "d_models.hh"
using namespace display;
using px::Rect;
using px::Surface;

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Surface");
 tolua_usertype(tolua_S,"Rect");
}

/* function: GetSurface */
static int toluaI_display_GetSurface00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const char* name = ((const char*)  tolua_getstring(tolua_S,1,0));
 {
  Surface* toluaI_ret = (Surface*)  GetSurface(name);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Surface"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSurface'.");
 return 0;
}

/* function: CropSurface */
static int toluaI_display_CropSurface00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const Surface"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"Rect"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  const Surface* s = ((const Surface*)  tolua_getusertype(tolua_S,1,0));
  Rect r = *((Rect*)  tolua_getusertype(tolua_S,2,0));
 {
  Surface* toluaI_ret = (Surface*)  CropSurface(s,r);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Surface"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CropSurface'.");
 return 0;
}

/* function: DefineImageModel */
static int toluaI_display_DefineImageModel00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"Surface"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  const char* name = ((const char*)  tolua_getstring(tolua_S,1,0));
  Surface* s = ((Surface*)  tolua_getusertype(tolua_S,2,0));
 {
  DefineImageModel(name,s);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DefineImageModel'.");
 return 0;
}

/* function: SetFollowMode */
static int toluaI_display_display_SetFollowMode00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  FollowMode m = ((FollowMode)  tolua_getnumber(tolua_S,1,0));
 {
  SetFollowMode(m);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFollowMode'.");
 return 0;
}

/* function: DefineImage */
static int toluaI_display_display_DefineImage00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  const char* name = ((const char*)  tolua_getstring(tolua_S,1,0));
  const char* fname = ((const char*)  tolua_getstring(tolua_S,2,0));
  int xoff = ((int)  tolua_getnumber(tolua_S,3,0));
  int yoff = ((int)  tolua_getnumber(tolua_S,4,0));
  int padding = ((int)  tolua_getnumber(tolua_S,5,0));
 {
  int toluaI_ret = (int)  DefineImage(name,fname,xoff,yoff,padding);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DefineImage'.");
 return 0;
}

/* function: DefineSubImage */
static int toluaI_display_display_DefineSubImage00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"Rect"),0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  const char* name = ((const char*)  tolua_getstring(tolua_S,1,0));
  const char* fname = ((const char*)  tolua_getstring(tolua_S,2,0));
  int xoff = ((int)  tolua_getnumber(tolua_S,3,0));
  int yoff = ((int)  tolua_getnumber(tolua_S,4,0));
  Rect r = *((Rect*)  tolua_getusertype(tolua_S,5,0));
 {
  int toluaI_ret = (int)  DefineSubImage(name,fname,xoff,yoff,r);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DefineSubImage'.");
 return 0;
}

/* function: DefineRandModel */
static int toluaI_display_display_DefineRandModel00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TTABLE,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  const char* name = ((const char*)  tolua_getstring(tolua_S,1,0));
  int n = ((int)  tolua_getnumber(tolua_S,2,0));
  const char** names = (const char**) malloc(n*sizeof(char*));
 {
 if (!tolua_arrayistype(tolua_S,3,LUA_TSTRING,n,0))
 goto tolua_lerror;
 else
 {
 int i;
 for(i=0; i<n;i++)
  names[i] = ((const char*)  tolua_getfieldstring(tolua_S,3,i+1,0));
 }
 }
 {
  DefineRandModel(name,n,names);
 }
 {
 int i;
 for(i=0; i<n;i++)
 tolua_pushfieldstring(tolua_S,3,i+1,(const char*) names[i]);
 }
 free(names);
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DefineRandModel'.");
 return 0;
}

/* function: DefineShadedModel */
static int toluaI_display_display_DefineShadedModel00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  const char* name = ((const char*)  tolua_getstring(tolua_S,1,0));
  const char* model = ((const char*)  tolua_getstring(tolua_S,2,0));
  const char* shade = ((const char*)  tolua_getstring(tolua_S,3,0));
 {
  DefineShadedModel(name,model,shade);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DefineShadedModel'.");
 return 0;
}

/* function: DefineOverlayImage */
static int toluaI_display_display_DefineOverlayImage00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TTABLE,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  const char* name = ((const char*)  tolua_getstring(tolua_S,1,0));
  int n = ((int)  tolua_getnumber(tolua_S,2,0));
  const char** images = (const char**) malloc(n*sizeof(char*));
 {
 if (!tolua_arrayistype(tolua_S,3,LUA_TSTRING,n,0))
 goto tolua_lerror;
 else
 {
 int i;
 for(i=0; i<n;i++)
  images[i] = ((const char*)  tolua_getfieldstring(tolua_S,3,i+1,0));
 }
 }
 {
  DefineOverlayImage(name,n,images);
 }
 {
 int i;
 for(i=0; i<n;i++)
 tolua_pushfieldstring(tolua_S,3,i+1,(const char*) images[i]);
 }
 free(images);
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DefineOverlayImage'.");
 return 0;
}

/* function: DefineComposite */
static int toluaI_display_display_DefineComposite00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  const char* name = ((const char*)  tolua_getstring(tolua_S,1,0));
  const char* bgname = ((const char*)  tolua_getstring(tolua_S,2,0));
  const char* fgname = ((const char*)  tolua_getstring(tolua_S,3,0));
 {
  DefineComposite(name,bgname,fgname);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DefineComposite'.");
 return 0;
}

/* function: DefineAnim */
static int toluaI_display_display_DefineAnim00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  const char* name = ((const char*)  tolua_getstring(tolua_S,1,0));
  bool loop_p = ((bool)  tolua_getbool(tolua_S,2,0));
 {
  DefineAnim(name,loop_p);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DefineAnim'.");
 return 0;
}

/* function: AddFrame */
static int toluaI_display_display_AddFrame00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  const char* name = ((const char*)  tolua_getstring(tolua_S,1,0));
  const char* model = ((const char*)  tolua_getstring(tolua_S,2,0));
  double time = ((double)  tolua_getnumber(tolua_S,3,0));
 {
  AddFrame(name,model,time);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddFrame'.");
 return 0;
}

/* function: DefineAlias */
static int toluaI_display_display_DefineAlias00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  const char* name = ((const char*)  tolua_getstring(tolua_S,1,0));
  const char* othername = ((const char*)  tolua_getstring(tolua_S,2,0));
 {
  DefineAlias(name,othername);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DefineAlias'.");
 return 0;
}

/* Open function */
int tolua_display_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_function(tolua_S,NULL,"GetSurface",toluaI_display_GetSurface00);
 tolua_function(tolua_S,NULL,"CropSurface",toluaI_display_CropSurface00);
 tolua_function(tolua_S,NULL,"DefineImageModel",toluaI_display_DefineImageModel00);
 tolua_module(tolua_S,"display");
 tolua_constant(tolua_S,"display","FOLLOW_SCROLLING",FOLLOW_SCROLLING);
 tolua_constant(tolua_S,"display","FOLLOW_SCREEN",FOLLOW_SCREEN);
 tolua_constant(tolua_S,"display","FOLLOW_SCREENSCROLLING",FOLLOW_SCREENSCROLLING);
 tolua_constant(tolua_S,"display","FOLLOW_SMOOTH",FOLLOW_SMOOTH);
 tolua_function(tolua_S,"display","SetFollowMode",toluaI_display_display_SetFollowMode00);
 tolua_function(tolua_S,"display","DefineImage",toluaI_display_display_DefineImage00);
 tolua_function(tolua_S,"display","DefineSubImage",toluaI_display_display_DefineSubImage00);
 tolua_function(tolua_S,"display","DefineRandModel",toluaI_display_display_DefineRandModel00);
 tolua_function(tolua_S,"display","DefineShadedModel",toluaI_display_display_DefineShadedModel00);
 tolua_function(tolua_S,"display","DefineOverlayImage",toluaI_display_display_DefineOverlayImage00);
 tolua_function(tolua_S,"display","DefineComposite",toluaI_display_display_DefineComposite00);
 tolua_function(tolua_S,"display","DefineAnim",toluaI_display_display_DefineAnim00);
 tolua_function(tolua_S,"display","AddFrame",toluaI_display_display_AddFrame00);
 tolua_function(tolua_S,"display","DefineAlias",toluaI_display_display_DefineAlias00);
 return 1;
}
/* Close function */
void tolua_display_close (lua_State* tolua_S)
{
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"GetSurface");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CropSurface");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"DefineImageModel");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"display");
}
