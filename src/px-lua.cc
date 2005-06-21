/*
** Lua binding: px
** Generated automatically by tolua 4.0a on Fri Sep 12 22:07:20 2003.
*/

#include "tolua.h"

/* Exported function */
int tolua_px_open (lua_State* tolua_S);
void tolua_px_close (lua_State* tolua_S);

#include "SDL.h"
#include "px/px.hh"
using namespace px;

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"GS");
 tolua_usertype(tolua_S,"Font");
 tolua_usertype(tolua_S,"V2");
 tolua_usertype(tolua_S,"Surface");
 tolua_usertype(tolua_S,"Drawable");
 tolua_usertype(tolua_S,"Screen");
 tolua_usertype(tolua_S,"Rect");
}

/* get function: x of class  Rect */
static int toluaI_get_px_px_Rect_x(lua_State* tolua_S)
{
  Rect* self = (Rect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'");
 tolua_pushnumber(tolua_S,(double)self->x);
 return 1;
}

/* set function: x of class  Rect */
static int toluaI_set_px_px_Rect_x(lua_State* tolua_S)
{
  Rect* self = (Rect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'");
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  self->x = ((int)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: y of class  Rect */
static int toluaI_get_px_px_Rect_y(lua_State* tolua_S)
{
  Rect* self = (Rect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'");
 tolua_pushnumber(tolua_S,(double)self->y);
 return 1;
}

/* set function: y of class  Rect */
static int toluaI_set_px_px_Rect_y(lua_State* tolua_S)
{
  Rect* self = (Rect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'");
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  self->y = ((int)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: w of class  Rect */
static int toluaI_get_px_px_Rect_w(lua_State* tolua_S)
{
  Rect* self = (Rect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'w'");
 tolua_pushnumber(tolua_S,(double)self->w);
 return 1;
}

/* set function: w of class  Rect */
static int toluaI_set_px_px_Rect_w(lua_State* tolua_S)
{
  Rect* self = (Rect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'w'");
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  self->w = ((int)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: h of class  Rect */
static int toluaI_get_px_px_Rect_h(lua_State* tolua_S)
{
  Rect* self = (Rect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'h'");
 tolua_pushnumber(tolua_S,(double)self->h);
 return 1;
}

/* set function: h of class  Rect */
static int toluaI_set_px_px_Rect_h(lua_State* tolua_S)
{
  Rect* self = (Rect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'h'");
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  self->h = ((int)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* method: new of class  Rect */
static int toluaI_px_px_Rect_new00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  int xx = ((int)  tolua_getnumber(tolua_S,2,0));
  int yy = ((int)  tolua_getnumber(tolua_S,3,0));
  int ww = ((int)  tolua_getnumber(tolua_S,4,0));
  int hh = ((int)  tolua_getnumber(tolua_S,5,0));
 {
  Rect* toluaI_ret = (Rect*)  new Rect(xx,yy,ww,hh);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Rect"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.");
 return 0;
}

/* method: delete of class  Rect */
static int toluaI_px_px_Rect_delete00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Rect"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  Rect* self = (Rect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'");
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.");
 return 0;
}

/* method: new of class  V2 */
static int toluaI_px_px_V2_new00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  V2* toluaI_ret = (V2*)  new V2();
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"V2"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.");
 return 0;
}

/* method: new of class  V2 */
static int toluaI_px_px_V2_new01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  double x = ((double)  tolua_getnumber(tolua_S,2,0));
  double y = ((double)  tolua_getnumber(tolua_S,3,0));
 {
  V2* toluaI_ret = (V2*)  new V2(x,y);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"V2"));
 }
 }
 return 1;
tolua_lerror:
 return toluaI_px_px_V2_new00(tolua_S);
}

/* method: operator&[] of class  V2 */
static int toluaI_px_px_V2_operator_set00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"V2"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  V2* self = (V2*)  tolua_getusertype(tolua_S,1,0);
  int idx = ((int)  tolua_getnumber(tolua_S,2,0));
  double toluaI_value = ((double)  tolua_getnumber(tolua_S,3,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator&[]'");
 self->operator[](idx) =  toluaI_value;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'operator_set'.");
 return 0;
}

/* method: operator[] of class  V2 */
static int toluaI_px_px_V2_operator_get00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"V2"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  V2* self = (V2*)  tolua_getusertype(tolua_S,1,0);
  int idx = ((int)  tolua_getnumber(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'");
 {
  double toluaI_ret = (double)  self->operator[](idx);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'operator_get'.");
 return 0;
}

/* method: new of class  GS */
static int toluaI_px_px_GS_new00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const Rect"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  const Rect* clipr = ((const Rect*)  tolua_getusertype(tolua_S,2,0));
 {
  GS* toluaI_ret = (GS*)  new GS(*clipr);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"GS"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.");
 return 0;
}

/* method: delete of class  GS */
static int toluaI_px_px_GS_delete00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"GS"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  GS* self = (GS*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'");
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.");
 return 0;
}

/* get function: cliprect of class  GS */
static int toluaI_get_px_px_GS_cliprect(lua_State* tolua_S)
{
  GS* self = (GS*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'cliprect'");
 tolua_pushusertype(tolua_S,(void*)&self->cliprect,tolua_tag(tolua_S,"Rect"));
 return 1;
}

/* set function: cliprect of class  GS */
static int toluaI_set_px_px_GS_cliprect(lua_State* tolua_S)
{
  GS* self = (GS*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'cliprect'");
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"Rect"),0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  self->cliprect = *((Rect*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: pcolor of class  GS */
static int toluaI_get_px_px_GS_pcolor(lua_State* tolua_S)
{
  GS* self = (GS*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pcolor'");
 tolua_pushnumber(tolua_S,(double)self->pcolor);
 return 1;
}

/* set function: pcolor of class  GS */
static int toluaI_set_px_px_GS_pcolor(lua_State* tolua_S)
{
  GS* self = (GS*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pcolor'");
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  self->pcolor = ((PackedColor)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* method: delete of class  Drawable */
static int toluaI_px_px_Drawable_delete00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Drawable"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  Drawable* self = (Drawable*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'");
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.");
 return 0;
}

/* method: map_color of class  Drawable */
static int toluaI_px_px_Drawable_map_color00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Drawable"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  Drawable* self = (Drawable*)  tolua_getusertype(tolua_S,1,0);
  int r = ((int)  tolua_getnumber(tolua_S,2,0));
  int g = ((int)  tolua_getnumber(tolua_S,3,0));
  int b = ((int)  tolua_getnumber(tolua_S,4,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'map_color'");
 {
  PackedColor toluaI_ret = (PackedColor)  self->map_color(r,g,b);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'map_color'.");
 return 0;
}

/* method: map_color of class  Drawable */
static int toluaI_px_px_Drawable_map_color01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Drawable"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  Drawable* self = (Drawable*)  tolua_getusertype(tolua_S,1,0);
  int r = ((int)  tolua_getnumber(tolua_S,2,0));
  int g = ((int)  tolua_getnumber(tolua_S,3,0));
  int b = ((int)  tolua_getnumber(tolua_S,4,0));
  int a = ((int)  tolua_getnumber(tolua_S,5,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'map_color'");
 {
  PackedColor toluaI_ret = (PackedColor)  self->map_color(r,g,b,a);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 return toluaI_px_px_Drawable_map_color00(tolua_S);
}

/* method: blit of class  Drawable */
static int toluaI_px_px_Drawable_blit00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Drawable"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const GS"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"Surface"),0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  Drawable* self = (Drawable*)  tolua_getusertype(tolua_S,1,0);
  const GS* gs = ((const GS*)  tolua_getusertype(tolua_S,2,0));
  int x = ((int)  tolua_getnumber(tolua_S,3,0));
  int y = ((int)  tolua_getnumber(tolua_S,4,0));
  Surface* s = ((Surface*)  tolua_getusertype(tolua_S,5,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'blit'");
 {
  self->blit(*gs,x,y,s);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'blit'.");
 return 0;
}

/* method: blit of class  Drawable */
static int toluaI_px_px_Drawable_blit01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Drawable"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const GS"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"Surface"),0) ||
 !tolua_istype(tolua_S,6,tolua_tag(tolua_S,"const Rect"),0) ||
 !tolua_isnoobj(tolua_S,7)
 )
 goto tolua_lerror;
 else
 {
  Drawable* self = (Drawable*)  tolua_getusertype(tolua_S,1,0);
  const GS* gs = ((const GS*)  tolua_getusertype(tolua_S,2,0));
  int x = ((int)  tolua_getnumber(tolua_S,3,0));
  int y = ((int)  tolua_getnumber(tolua_S,4,0));
  Surface* s = ((Surface*)  tolua_getusertype(tolua_S,5,0));
  const Rect* r = ((const Rect*)  tolua_getusertype(tolua_S,6,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'blit'");
 {
  self->blit(*gs,x,y,s,*r);
 }
 }
 return 0;
tolua_lerror:
 return toluaI_px_px_Drawable_blit00(tolua_S);
}

/* method: get_pixel of class  Drawable */
static int toluaI_px_px_Drawable_get_pixel00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Drawable"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  Drawable* self = (Drawable*)  tolua_getusertype(tolua_S,1,0);
  int x = ((int)  tolua_getnumber(tolua_S,2,0));
  int y = ((int)  tolua_getnumber(tolua_S,3,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_pixel'");
 {
  Uint32 toluaI_ret = (Uint32)  self->get_pixel(x,y);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_pixel'.");
 return 0;
}

/* method: set_pixel of class  Drawable */
static int toluaI_px_px_Drawable_set_pixel00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Drawable"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const GS"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  Drawable* self = (Drawable*)  tolua_getusertype(tolua_S,1,0);
  const GS* gs = ((const GS*)  tolua_getusertype(tolua_S,2,0));
  int x = ((int)  tolua_getnumber(tolua_S,3,0));
  int y = ((int)  tolua_getnumber(tolua_S,4,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_pixel'");
 {
  self->set_pixel(*gs,x,y);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_pixel'.");
 return 0;
}

/* method: set_pixels of class  Drawable */
static int toluaI_px_px_Drawable_set_pixels00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Drawable"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const GS"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  Drawable* self = (Drawable*)  tolua_getusertype(tolua_S,1,0);
  const GS* gs = ((const GS*)  tolua_getusertype(tolua_S,2,0));
  int n = ((int)  tolua_getnumber(tolua_S,3,0));
  const int x = ((const int)  tolua_getnumber(tolua_S,4,0));
  const int y = ((const int)  tolua_getnumber(tolua_S,5,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_pixels'");
 {
  self->set_pixels(*gs,n,&x,&y);
 tolua_pushnumber(tolua_S,(double)x);
 tolua_pushnumber(tolua_S,(double)y);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_pixels'.");
 return 0;
}

/* method: hline of class  Drawable */
static int toluaI_px_px_Drawable_hline00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Drawable"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const GS"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  Drawable* self = (Drawable*)  tolua_getusertype(tolua_S,1,0);
  const GS* gs = ((const GS*)  tolua_getusertype(tolua_S,2,0));
  int x = ((int)  tolua_getnumber(tolua_S,3,0));
  int y = ((int)  tolua_getnumber(tolua_S,4,0));
  int w = ((int)  tolua_getnumber(tolua_S,5,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hline'");
 {
  self->hline(*gs,x,y,w);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hline'.");
 return 0;
}

/* method: vline of class  Drawable */
static int toluaI_px_px_Drawable_vline00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Drawable"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const GS"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  Drawable* self = (Drawable*)  tolua_getusertype(tolua_S,1,0);
  const GS* gs = ((const GS*)  tolua_getusertype(tolua_S,2,0));
  int x = ((int)  tolua_getnumber(tolua_S,3,0));
  int y = ((int)  tolua_getnumber(tolua_S,4,0));
  int h = ((int)  tolua_getnumber(tolua_S,5,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'vline'");
 {
  self->vline(*gs,x,y,h);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'vline'.");
 return 0;
}

/* method: line of class  Drawable */
static int toluaI_px_px_Drawable_line00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Drawable"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const GS"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,7)
 )
 goto tolua_lerror;
 else
 {
  Drawable* self = (Drawable*)  tolua_getusertype(tolua_S,1,0);
  const GS* gs = ((const GS*)  tolua_getusertype(tolua_S,2,0));
  int x1 = ((int)  tolua_getnumber(tolua_S,3,0));
  int y1 = ((int)  tolua_getnumber(tolua_S,4,0));
  int x2 = ((int)  tolua_getnumber(tolua_S,5,0));
  int y2 = ((int)  tolua_getnumber(tolua_S,6,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'line'");
 {
  self->line(*gs,x1,y1,x2,y2);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'line'.");
 return 0;
}

/* method: box of class  Drawable */
static int toluaI_px_px_Drawable_box00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Drawable"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const GS"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,7)
 )
 goto tolua_lerror;
 else
 {
  Drawable* self = (Drawable*)  tolua_getusertype(tolua_S,1,0);
  const GS* gs = ((const GS*)  tolua_getusertype(tolua_S,2,0));
  int x = ((int)  tolua_getnumber(tolua_S,3,0));
  int y = ((int)  tolua_getnumber(tolua_S,4,0));
  int w = ((int)  tolua_getnumber(tolua_S,5,0));
  int h = ((int)  tolua_getnumber(tolua_S,6,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'box'");
 {
  self->box(*gs,x,y,w,h);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'box'.");
 return 0;
}

/* method: size of class  Drawable */
static int toluaI_px_px_Drawable_size00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const Drawable"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const Drawable* self = (const Drawable*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'size'");
 {
  Rect toluaI_ret = (Rect)  self->size();
 {
#ifdef __cplusplus
 void* toluaI_clone = new Rect(toluaI_ret);
#else
 void* toluaI_clone = tolua_copy(tolua_S,(void*)&toluaI_ret,sizeof(Rect));
#endif
 tolua_pushusertype(tolua_S,tolua_doclone(tolua_S,toluaI_clone,tolua_tag(tolua_S,"Rect")),tolua_tag(tolua_S,"Rect"));
 }
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'size'.");
 return 0;
}

/* method: delete of class  Surface */
static int toluaI_px_px_Surface_delete00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Surface"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  Surface* self = (Surface*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'");
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.");
 return 0;
}

/* method: width of class  Surface */
static int toluaI_px_px_Surface_width00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Surface"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  Surface* self = (Surface*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'width'");
 {
  int toluaI_ret = (int)  self->width();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'width'.");
 return 0;
}

/* method: height of class  Surface */
static int toluaI_px_px_Surface_height00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Surface"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  Surface* self = (Surface*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'height'");
 {
  int toluaI_ret = (int)  self->height();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'height'.");
 return 0;
}

/* method: get_surface of class  Screen */
static int toluaI_px_px_Screen_get_surface00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Screen"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  Screen* self = (Screen*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_surface'");
 {
  Surface* toluaI_ret = (Surface*)  self->get_surface();
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Surface"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_surface'.");
 return 0;
}

/* method: update_all of class  Screen */
static int toluaI_px_px_Screen_update_all00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Screen"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  Screen* self = (Screen*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'update_all'");
 {
  self->update_all();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'update_all'.");
 return 0;
}

/* method: update_rect of class  Screen */
static int toluaI_px_px_Screen_update_rect00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Screen"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const Rect"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  Screen* self = (Screen*)  tolua_getusertype(tolua_S,1,0);
  const Rect* r = ((const Rect*)  tolua_getusertype(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'update_rect'");
 {
  self->update_rect(*r);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'update_rect'.");
 return 0;
}

/* method: flush_updates of class  Screen */
static int toluaI_px_px_Screen_flush_updates00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Screen"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  Screen* self = (Screen*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'flush_updates'");
 {
  self->flush_updates();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flush_updates'.");
 return 0;
}

/* method: set_caption of class  Screen */
static int toluaI_px_px_Screen_set_caption00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Screen"),0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  Screen* self = (Screen*)  tolua_getusertype(tolua_S,1,0);
  const char* str = ((const char*)  tolua_getstring(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_caption'");
 {
  self->set_caption(str);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_caption'.");
 return 0;
}

/* method: delete of class  Font */
static int toluaI_px_px_Font_delete00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Font"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  Font* self = (Font*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'");
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.");
 return 0;
}

/* method: get_lineskip of class  Font */
static int toluaI_px_px_Font_get_lineskip00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Font"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  Font* self = (Font*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_lineskip'");
 {
  int toluaI_ret = (int)  self->get_lineskip();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_lineskip'.");
 return 0;
}

/* method: get_height of class  Font */
static int toluaI_px_px_Font_get_height00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Font"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  Font* self = (Font*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_height'");
 {
  int toluaI_ret = (int)  self->get_height();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_height'.");
 return 0;
}

/* method: get_width of class  Font */
static int toluaI_px_px_Font_get_width00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Font"),0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  Font* self = (Font*)  tolua_getusertype(tolua_S,1,0);
  const char* str = ((const char*)  tolua_getstring(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_width'");
 {
  int toluaI_ret = (int)  self->get_width(str);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_width'.");
 return 0;
}

/* method: render of class  Font */
static int toluaI_px_px_Font_render00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Font"),0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  Font* self = (Font*)  tolua_getusertype(tolua_S,1,0);
  const char* str = ((const char*)  tolua_getstring(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'render'");
 {
  Surface* toluaI_ret = (Surface*)  self->render(str);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Surface"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'render'.");
 return 0;
}

/* method: render of class  Font */
static int toluaI_px_px_Font_render01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Font"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"Surface"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  Font* self = (Font*)  tolua_getusertype(tolua_S,1,0);
  Surface* s = ((Surface*)  tolua_getusertype(tolua_S,2,0));
  int x = ((int)  tolua_getnumber(tolua_S,3,0));
  int y = ((int)  tolua_getnumber(tolua_S,4,0));
  const char* str = ((const char*)  tolua_getstring(tolua_S,5,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'render'");
 {
  self->render(s,x,y,str);
 }
 }
 return 0;
tolua_lerror:
 return toluaI_px_px_Font_render00(tolua_S);
}

/* Open function */
int tolua_px_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_module(tolua_S,"px");
 tolua_cclass(tolua_S,"Rect","");
 tolua_tablevar(tolua_S,"Rect","x",toluaI_get_px_px_Rect_x,toluaI_set_px_px_Rect_x);
 tolua_tablevar(tolua_S,"Rect","y",toluaI_get_px_px_Rect_y,toluaI_set_px_px_Rect_y);
 tolua_tablevar(tolua_S,"Rect","w",toluaI_get_px_px_Rect_w,toluaI_set_px_px_Rect_w);
 tolua_tablevar(tolua_S,"Rect","h",toluaI_get_px_px_Rect_h,toluaI_set_px_px_Rect_h);
 tolua_function(tolua_S,"Rect","new",toluaI_px_px_Rect_new00);
 tolua_function(tolua_S,"Rect","delete",toluaI_px_px_Rect_delete00);
 tolua_cclass(tolua_S,"V2","");
 tolua_function(tolua_S,"V2","new",toluaI_px_px_V2_new00);
 tolua_function(tolua_S,"V2","new",toluaI_px_px_V2_new01);
 tolua_function(tolua_S,"V2","operator_set",toluaI_px_px_V2_operator_set00);
 tolua_function(tolua_S,"V2","operator_get",toluaI_px_px_V2_operator_get00);
 tolua_cclass(tolua_S,"GS","");
 tolua_function(tolua_S,"GS","new",toluaI_px_px_GS_new00);
 tolua_function(tolua_S,"GS","delete",toluaI_px_px_GS_delete00);
 tolua_tablevar(tolua_S,"GS","cliprect",toluaI_get_px_px_GS_cliprect,toluaI_set_px_px_GS_cliprect);
 tolua_tablevar(tolua_S,"GS","pcolor",toluaI_get_px_px_GS_pcolor,toluaI_set_px_px_GS_pcolor);
 tolua_cclass(tolua_S,"Drawable","");
 tolua_function(tolua_S,"Drawable","delete",toluaI_px_px_Drawable_delete00);
 tolua_function(tolua_S,"Drawable","map_color",toluaI_px_px_Drawable_map_color00);
 tolua_function(tolua_S,"Drawable","map_color",toluaI_px_px_Drawable_map_color01);
 tolua_function(tolua_S,"Drawable","blit",toluaI_px_px_Drawable_blit00);
 tolua_function(tolua_S,"Drawable","blit",toluaI_px_px_Drawable_blit01);
 tolua_function(tolua_S,"Drawable","get_pixel",toluaI_px_px_Drawable_get_pixel00);
 tolua_function(tolua_S,"Drawable","set_pixel",toluaI_px_px_Drawable_set_pixel00);
 tolua_function(tolua_S,"Drawable","set_pixels",toluaI_px_px_Drawable_set_pixels00);
 tolua_function(tolua_S,"Drawable","hline",toluaI_px_px_Drawable_hline00);
 tolua_function(tolua_S,"Drawable","vline",toluaI_px_px_Drawable_vline00);
 tolua_function(tolua_S,"Drawable","line",toluaI_px_px_Drawable_line00);
 tolua_function(tolua_S,"Drawable","box",toluaI_px_px_Drawable_box00);
 tolua_function(tolua_S,"Drawable","size",toluaI_px_px_Drawable_size00);
 tolua_cclass(tolua_S,"Surface","Drawable");
 tolua_function(tolua_S,"Surface","delete",toluaI_px_px_Surface_delete00);
 tolua_function(tolua_S,"Surface","width",toluaI_px_px_Surface_width00);
 tolua_function(tolua_S,"Surface","height",toluaI_px_px_Surface_height00);
 tolua_cclass(tolua_S,"Screen","");
 tolua_function(tolua_S,"Screen","get_surface",toluaI_px_px_Screen_get_surface00);
 tolua_function(tolua_S,"Screen","update_all",toluaI_px_px_Screen_update_all00);
 tolua_function(tolua_S,"Screen","update_rect",toluaI_px_px_Screen_update_rect00);
 tolua_function(tolua_S,"Screen","flush_updates",toluaI_px_px_Screen_flush_updates00);
 tolua_function(tolua_S,"Screen","set_caption",toluaI_px_px_Screen_set_caption00);
 tolua_cclass(tolua_S,"Font","");
 tolua_function(tolua_S,"Font","delete",toluaI_px_px_Font_delete00);
 tolua_function(tolua_S,"Font","get_lineskip",toluaI_px_px_Font_get_lineskip00);
 tolua_function(tolua_S,"Font","get_height",toluaI_px_px_Font_get_height00);
 tolua_function(tolua_S,"Font","get_width",toluaI_px_px_Font_get_width00);
 tolua_function(tolua_S,"Font","render",toluaI_px_px_Font_render00);
 tolua_function(tolua_S,"Font","render",toluaI_px_px_Font_render01);
 return 1;
}
/* Close function */
void tolua_px_close (lua_State* tolua_S)
{
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"px");
}
