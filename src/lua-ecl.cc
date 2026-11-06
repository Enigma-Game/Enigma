/*
** Lua binding: px
** Generated automatically by tolua++-1.0.93 on Sun Sep 13 19:23:47 2026.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_px_open (lua_State* tolua_S);

#include <SDL3/SDL.h>
#include "ecl_font.hh"
#include "ecl_geom.hh"
#include "ecl_math.hh"
#include "ecl_video.hh"
using namespace ecl;

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Font (lua_State* tolua_S)
{
 Font* self = (Font*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Rect (lua_State* tolua_S)
{
 Rect* self = (Rect*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_V2 (lua_State* tolua_S)
{
 V2* self = (V2*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Surface (lua_State* tolua_S)
{
 Surface* self = (Surface*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Font");
 tolua_usertype(tolua_S,"Rect");
 tolua_usertype(tolua_S,"V2");
 tolua_usertype(tolua_S,"Surface");
}

/* get function: x of class  Rect */
#ifndef TOLUA_DISABLE_tolua_get_Rect_x
static int tolua_get_Rect_x(lua_State* tolua_S)
{
  Rect* self = (Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  Rect */
#ifndef TOLUA_DISABLE_tolua_set_Rect_x
static int tolua_set_Rect_x(lua_State* tolua_S)
{
  Rect* self = (Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  Rect */
#ifndef TOLUA_DISABLE_tolua_get_Rect_y
static int tolua_get_Rect_y(lua_State* tolua_S)
{
  Rect* self = (Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  Rect */
#ifndef TOLUA_DISABLE_tolua_set_Rect_y
static int tolua_set_Rect_y(lua_State* tolua_S)
{
  Rect* self = (Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: w of class  Rect */
#ifndef TOLUA_DISABLE_tolua_get_Rect_w
static int tolua_get_Rect_w(lua_State* tolua_S)
{
  Rect* self = (Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'w'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->w);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: w of class  Rect */
#ifndef TOLUA_DISABLE_tolua_set_Rect_w
static int tolua_set_Rect_w(lua_State* tolua_S)
{
  Rect* self = (Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'w'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->w = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: h of class  Rect */
#ifndef TOLUA_DISABLE_tolua_get_Rect_h
static int tolua_get_Rect_h(lua_State* tolua_S)
{
  Rect* self = (Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'h'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->h);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: h of class  Rect */
#ifndef TOLUA_DISABLE_tolua_set_Rect_h
static int tolua_set_Rect_h(lua_State* tolua_S)
{
  Rect* self = (Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'h'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->h = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Rect */
#ifndef TOLUA_DISABLE_tolua_px_ecl_Rect_new00
static int tolua_px_ecl_Rect_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Rect",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int xx = ((int)  tolua_tonumber(tolua_S,2,0));
  int yy = ((int)  tolua_tonumber(tolua_S,3,0));
  int ww = ((int)  tolua_tonumber(tolua_S,4,0));
  int hh = ((int)  tolua_tonumber(tolua_S,5,0));
  {
   Rect* tolua_ret = (Rect*)  new Rect(xx,yy,ww,hh);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Rect");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Rect */
#ifndef TOLUA_DISABLE_tolua_px_ecl_Rect_new00_local
static int tolua_px_ecl_Rect_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Rect",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int xx = ((int)  tolua_tonumber(tolua_S,2,0));
  int yy = ((int)  tolua_tonumber(tolua_S,3,0));
  int ww = ((int)  tolua_tonumber(tolua_S,4,0));
  int hh = ((int)  tolua_tonumber(tolua_S,5,0));
  {
   Rect* tolua_ret = (Rect*)  new Rect(xx,yy,ww,hh);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Rect");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Rect */
#ifndef TOLUA_DISABLE_tolua_px_ecl_Rect_delete00
static int tolua_px_ecl_Rect_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Rect",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Rect* self = (Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  V2 */
#ifndef TOLUA_DISABLE_tolua_px_ecl_V2_new00
static int tolua_px_ecl_V2_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"V2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   V2* tolua_ret = (V2*)  new V2();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"V2");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  V2 */
#ifndef TOLUA_DISABLE_tolua_px_ecl_V2_new00_local
static int tolua_px_ecl_V2_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"V2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   V2* tolua_ret = (V2*)  new V2();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"V2");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  V2 */
#ifndef TOLUA_DISABLE_tolua_px_ecl_V2_new01
static int tolua_px_ecl_V2_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"V2",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  double x = ((double)  tolua_tonumber(tolua_S,2,0));
  double y = ((double)  tolua_tonumber(tolua_S,3,0));
  {
   V2* tolua_ret = (V2*)  new V2(x,y);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"V2");
  }
 }
 return 1;
tolua_lerror:
 return tolua_px_ecl_V2_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  V2 */
#ifndef TOLUA_DISABLE_tolua_px_ecl_V2_new01_local
static int tolua_px_ecl_V2_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"V2",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  double x = ((double)  tolua_tonumber(tolua_S,2,0));
  double y = ((double)  tolua_tonumber(tolua_S,3,0));
  {
   V2* tolua_ret = (V2*)  new V2(x,y);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"V2");
  }
 }
 return 1;
tolua_lerror:
 return tolua_px_ecl_V2_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator[] of class  V2 */
#ifndef TOLUA_DISABLE_tolua_px_ecl_V2__geti00
static int tolua_px_ecl_V2__geti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"V2",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  V2* self = (V2*)  tolua_tousertype(tolua_S,1,0);
  int idx = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'",NULL);
#endif
  {
   double tolua_ret = (double)  self->operator[](idx);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.geti'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Surface */
#ifndef TOLUA_DISABLE_tolua_px_ecl_Surface_delete00
static int tolua_px_ecl_Surface_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Surface",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Surface* self = (Surface*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: width of class  Surface */
#ifndef TOLUA_DISABLE_tolua_px_ecl_Surface_width00
static int tolua_px_ecl_Surface_width00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Surface",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Surface* self = (Surface*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'width'",NULL);
#endif
  {
   int tolua_ret = (int)  self->width();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'width'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: height of class  Surface */
#ifndef TOLUA_DISABLE_tolua_px_ecl_Surface_height00
static int tolua_px_ecl_Surface_height00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Surface",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Surface* self = (Surface*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'height'",NULL);
#endif
  {
   int tolua_ret = (int)  self->height();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'height'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Font */
#ifndef TOLUA_DISABLE_tolua_px_ecl_Font_delete00
static int tolua_px_ecl_Font_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Font",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Font* self = (Font*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_lineskip of class  Font */
#ifndef TOLUA_DISABLE_tolua_px_ecl_Font_get_lineskip00
static int tolua_px_ecl_Font_get_lineskip00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Font",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Font* self = (Font*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_lineskip'",NULL);
#endif
  {
   int tolua_ret = (int)  self->get_lineskip();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_lineskip'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_height of class  Font */
#ifndef TOLUA_DISABLE_tolua_px_ecl_Font_get_height00
static int tolua_px_ecl_Font_get_height00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Font",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Font* self = (Font*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_height'",NULL);
#endif
  {
   int tolua_ret = (int)  self->get_height();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_height'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_width of class  Font */
#ifndef TOLUA_DISABLE_tolua_px_ecl_Font_get_width00
static int tolua_px_ecl_Font_get_width00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Font",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Font* self = (Font*)  tolua_tousertype(tolua_S,1,0);
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_width'",NULL);
#endif
  {
   int tolua_ret = (int)  self->get_width(text);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_width'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: render of class  Font */
#ifndef TOLUA_DISABLE_tolua_px_ecl_Font_render00
static int tolua_px_ecl_Font_render00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Font",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Surface",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Font* self = (Font*)  tolua_tousertype(tolua_S,1,0);
  Surface* s = ((Surface*)  tolua_tousertype(tolua_S,2,0));
  int x = ((int)  tolua_tonumber(tolua_S,3,0));
  int y = ((int)  tolua_tonumber(tolua_S,4,0));
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'render'",NULL);
#endif
  {
   self->render(s,x,y,text);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'render'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_px_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"ecl",0);
  tolua_beginmodule(tolua_S,"ecl");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Rect","Rect","",tolua_collect_Rect);
   #else
   tolua_cclass(tolua_S,"Rect","Rect","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Rect");
    tolua_variable(tolua_S,"x",tolua_get_Rect_x,tolua_set_Rect_x);
    tolua_variable(tolua_S,"y",tolua_get_Rect_y,tolua_set_Rect_y);
    tolua_variable(tolua_S,"w",tolua_get_Rect_w,tolua_set_Rect_w);
    tolua_variable(tolua_S,"h",tolua_get_Rect_h,tolua_set_Rect_h);
    tolua_function(tolua_S,"new",tolua_px_ecl_Rect_new00);
    tolua_function(tolua_S,"new_local",tolua_px_ecl_Rect_new00_local);
    tolua_function(tolua_S,".call",tolua_px_ecl_Rect_new00_local);
    tolua_function(tolua_S,"delete",tolua_px_ecl_Rect_delete00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"V2","V2","",tolua_collect_V2);
   #else
   tolua_cclass(tolua_S,"V2","V2","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"V2");
    tolua_function(tolua_S,"new",tolua_px_ecl_V2_new00);
    tolua_function(tolua_S,"new_local",tolua_px_ecl_V2_new00_local);
    tolua_function(tolua_S,".call",tolua_px_ecl_V2_new00_local);
    tolua_function(tolua_S,"new",tolua_px_ecl_V2_new01);
    tolua_function(tolua_S,"new_local",tolua_px_ecl_V2_new01_local);
    tolua_function(tolua_S,".call",tolua_px_ecl_V2_new01_local);
    tolua_function(tolua_S,".geti",tolua_px_ecl_V2__geti00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Surface","Surface","",tolua_collect_Surface);
   #else
   tolua_cclass(tolua_S,"Surface","Surface","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Surface");
    tolua_function(tolua_S,"delete",tolua_px_ecl_Surface_delete00);
    tolua_function(tolua_S,"width",tolua_px_ecl_Surface_width00);
    tolua_function(tolua_S,"height",tolua_px_ecl_Surface_height00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Font","Font","",tolua_collect_Font);
   #else
   tolua_cclass(tolua_S,"Font","Font","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Font");
    tolua_function(tolua_S,"delete",tolua_px_ecl_Font_delete00);
    tolua_function(tolua_S,"get_lineskip",tolua_px_ecl_Font_get_lineskip00);
    tolua_function(tolua_S,"get_height",tolua_px_ecl_Font_get_height00);
    tolua_function(tolua_S,"get_width",tolua_px_ecl_Font_get_width00);
    tolua_function(tolua_S,"render",tolua_px_ecl_Font_render00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_px (lua_State* tolua_S) {
 return tolua_px_open(tolua_S);
};
#endif

