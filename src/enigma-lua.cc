/*
** Lua binding: enigma
** Generated automatically by tolua 4.0a on Mon Apr 18 09:08:14 2005.
*/

#include "tolua.h"

/* Exported function */
int tolua_enigma_open (lua_State* tolua_S);
void tolua_enigma_close (lua_State* tolua_S);

#include "server.hh"
#include "world.hh"
using namespace enigma;
using ecl::Surface;
using ecl::Font;
using namespace server;
using namespace levels;
using namespace world;
#include "video.hh"
using namespace video;
using ecl::Screen;

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Item");
 tolua_usertype(tolua_S,"Object");
 tolua_usertype(tolua_S,"Screen");
 tolua_usertype(tolua_S,"Surface");
 tolua_usertype(tolua_S,"Font");
 tolua_usertype(tolua_S,"Floor");
 tolua_usertype(tolua_S,"Stone");
}

/* function: GetImage */
static int toluaI_enigma_enigma_GetImage00(lua_State* tolua_S)
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
  Surface* toluaI_ret = (Surface*)  GetImage(name);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Surface"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetImage'.");
 return 0;
}

/* function: GetImage */
static int toluaI_enigma_enigma_GetImage01(lua_State* tolua_S)
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
  const char* extension = ((const char*)  tolua_getstring(tolua_S,2,0));
 {
  Surface* toluaI_ret = (Surface*)  GetImage(name,extension);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Surface"));
 }
 }
 return 1;
tolua_lerror:
 return toluaI_enigma_enigma_GetImage00(tolua_S);
}

/* function: LoadImage */
static int toluaI_enigma_enigma_LoadImage00(lua_State* tolua_S)
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
  Surface* toluaI_ret = (Surface*)  LoadImage(name);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Surface"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LoadImage'.");
 return 0;
}

/* function: RegisterImage */
static int toluaI_enigma_enigma_RegisterImage00(lua_State* tolua_S)
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
  Surface* toluaI_ret = (Surface*)  RegisterImage(name,s);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Surface"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RegisterImage'.");
 return 0;
}

/* function: GetFont */
static int toluaI_enigma_enigma_GetFont00(lua_State* tolua_S)
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
  Font* toluaI_ret = (Font*)  GetFont(name);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Font"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFont'.");
 return 0;
}

/* function: date */
static int toluaI_enigma_enigma_date00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const char* format = ((const char*)  tolua_getstring(tolua_S,1,0));
 {
  const char* toluaI_ret = (const char*)  date(format);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'date'.");
 return 0;
}

/* get function: TwoPlayerGame */
static int toluaI_get_enigma_enigma_TwoPlayerGame(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)TwoPlayerGame);
 return 1;
}

/* set function: TwoPlayerGame */
static int toluaI_set_enigma_enigma_TwoPlayerGame(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  TwoPlayerGame = ((int)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: SingleComputerGame */
static int toluaI_get_enigma_enigma_SingleComputerGame(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)SingleComputerGame);
 return 1;
}

/* set function: SingleComputerGame */
static int toluaI_set_enigma_enigma_SingleComputerGame(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  SingleComputerGame = ((int)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: ConserveLevel */
static int toluaI_get_enigma_enigma_ConserveLevel(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)ConserveLevel);
 return 1;
}

/* set function: ConserveLevel */
static int toluaI_set_enigma_enigma_ConserveLevel(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  ConserveLevel = ((int)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: AllowTogglePlayer */
static int toluaI_get_enigma_enigma_AllowTogglePlayer(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)AllowTogglePlayer);
 return 1;
}

/* set function: AllowTogglePlayer */
static int toluaI_set_enigma_enigma_AllowTogglePlayer(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  AllowTogglePlayer = ((int)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: ShowMoves */
static int toluaI_get_enigma_enigma_ShowMoves(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)ShowMoves);
 return 1;
}

/* set function: ShowMoves */
static int toluaI_set_enigma_enigma_ShowMoves(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  ShowMoves = ((int)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: Brittleness */
static int toluaI_get_enigma_enigma_Brittleness(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)Brittleness);
 return 1;
}

/* set function: Brittleness */
static int toluaI_set_enigma_enigma_Brittleness(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  Brittleness = ((double)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: SlopeForce */
static int toluaI_get_enigma_enigma_SlopeForce(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)SlopeForce);
 return 1;
}

/* set function: SlopeForce */
static int toluaI_set_enigma_enigma_SlopeForce(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  SlopeForce = ((double)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FlatForce */
static int toluaI_get_enigma_enigma_FlatForce(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)FlatForce);
 return 1;
}

/* set function: FlatForce */
static int toluaI_set_enigma_enigma_FlatForce(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  FlatForce = ((double)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FrictionFactor */
static int toluaI_get_enigma_enigma_FrictionFactor(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)FrictionFactor);
 return 1;
}

/* set function: FrictionFactor */
static int toluaI_set_enigma_enigma_FrictionFactor(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  FrictionFactor = ((double)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: IceFriction */
static int toluaI_get_enigma_enigma_IceFriction(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)IceFriction);
 return 1;
}

/* set function: IceFriction */
static int toluaI_set_enigma_enigma_IceFriction(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  IceFriction = ((double)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: ElectricForce */
static int toluaI_get_enigma_enigma_ElectricForce(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)ElectricForce);
 return 1;
}

/* set function: ElectricForce */
static int toluaI_set_enigma_enigma_ElectricForce(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  ElectricForce = ((double)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: BumperForce */
static int toluaI_get_enigma_enigma_BumperForce(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)BumperForce);
 return 1;
}

/* set function: BumperForce */
static int toluaI_set_enigma_enigma_BumperForce(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  BumperForce = ((double)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: MagnetForce */
static int toluaI_get_enigma_enigma_MagnetForce(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)MagnetForce);
 return 1;
}

/* set function: MagnetForce */
static int toluaI_set_enigma_enigma_MagnetForce(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  MagnetForce = ((double)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: MagnetRange */
static int toluaI_get_enigma_enigma_MagnetRange(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)MagnetRange);
 return 1;
}

/* set function: MagnetRange */
static int toluaI_set_enigma_enigma_MagnetRange(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  MagnetRange = ((double)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: WaterSinkSpeed */
static int toluaI_get_enigma_enigma_WaterSinkSpeed(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)WaterSinkSpeed);
 return 1;
}

/* set function: WaterSinkSpeed */
static int toluaI_set_enigma_enigma_WaterSinkSpeed(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  WaterSinkSpeed = ((double)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: SwampSinkSpeed */
static int toluaI_get_enigma_enigma_SwampSinkSpeed(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)SwampSinkSpeed);
 return 1;
}

/* set function: SwampSinkSpeed */
static int toluaI_set_enigma_enigma_SwampSinkSpeed(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  SwampSinkSpeed = ((double)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: WormholeForce */
static int toluaI_get_enigma_enigma_WormholeForce(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)WormholeForce);
 return 1;
}

/* set function: WormholeForce */
static int toluaI_set_enigma_enigma_WormholeForce(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  WormholeForce = ((double)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: WormholeRange */
static int toluaI_get_enigma_enigma_WormholeRange(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)WormholeRange);
 return 1;
}

/* set function: WormholeRange */
static int toluaI_set_enigma_enigma_WormholeRange(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  WormholeRange = ((double)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: HoleForce */
static int toluaI_get_enigma_enigma_HoleForce(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(double)HoleForce);
 return 1;
}

/* set function: HoleForce */
static int toluaI_set_enigma_enigma_HoleForce(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  HoleForce = ((double)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: AddLevelPack */
static int toluaI_enigma_enigma_AddLevelPack00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  const char* init_file = ((const char*)  tolua_getstring(tolua_S,1,0));
  const char* name = ((const char*)  tolua_getstring(tolua_S,2,0));
 {
  AddLevelPack(init_file,name);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddLevelPack'.");
 return 0;
}

/* function: AddZippedLevelPack */
static int toluaI_enigma_enigma_AddZippedLevelPack00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const char* zipfile = ((const char*)  tolua_getstring(tolua_S,1,0));
 {
  AddZippedLevelPack(zipfile);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddZippedLevelPack'.");
 return 0;
}

/* function: SetCompatibility */
static int toluaI_enigma_enigma_SetCompatibility00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const char* version = ((const char*)  tolua_getstring(tolua_S,1,0));
 {
  SetCompatibility(version);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCompatibility'.");
 return 0;
}

/* function: MakeObject */
static int toluaI_enigma_world_MakeObject00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const char* kind = ((const char*)  tolua_getstring(tolua_S,1,0));
 {
  Object* toluaI_ret = (Object*)  MakeObject(kind);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Object"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MakeObject'.");
 return 0;
}

/* function: MakeFloor */
static int toluaI_enigma_world_MakeFloor00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const char* kind = ((const char*)  tolua_getstring(tolua_S,1,0));
 {
  Floor* toluaI_ret = (Floor*)  MakeFloor(kind);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Floor"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MakeFloor'.");
 return 0;
}

/* function: MakeItem */
static int toluaI_enigma_world_MakeItem00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const char* kind = ((const char*)  tolua_getstring(tolua_S,1,0));
 {
  Item* toluaI_ret = (Item*)  MakeItem(kind);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Item"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MakeItem'.");
 return 0;
}

/* function: MakeStone */
static int toluaI_enigma_world_MakeStone00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const char* kind = ((const char*)  tolua_getstring(tolua_S,1,0));
 {
  Stone* toluaI_ret = (Stone*)  MakeStone(kind);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Stone"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MakeStone'.");
 return 0;
}

/* function: GetNamedObject */
static int toluaI_enigma_world_GetNamedObject00(lua_State* tolua_S)
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
  Object* toluaI_ret = (Object*)  GetNamedObject(name);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Object"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNamedObject'.");
 return 0;
}

/* function: Resize */
static int toluaI_enigma_world_Resize00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  int w = ((int)  tolua_getnumber(tolua_S,1,0));
  int h = ((int)  tolua_getnumber(tolua_S,2,0));
 {
  Resize(w,h);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Resize'.");
 return 0;
}

/* function: DefineSimpleStone */
static int toluaI_enigma_world_DefineSimpleStone00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  const char* kind = ((const char*)  tolua_getstring(tolua_S,1,0));
  const char* sound = ((const char*)  tolua_getstring(tolua_S,2,0));
  int hollow = ((int)  tolua_getnumber(tolua_S,3,0));
  int glass = ((int)  tolua_getnumber(tolua_S,4,0));
 {
  DefineSimpleStone(kind,sound,hollow,glass);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DefineSimpleStone'.");
 return 0;
}

/* function: DefineSimpleStoneMovable */
static int toluaI_enigma_world_DefineSimpleStoneMovable00(lua_State* tolua_S)
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
  const char* kind = ((const char*)  tolua_getstring(tolua_S,1,0));
  const char* sound = ((const char*)  tolua_getstring(tolua_S,2,0));
  int glass = ((int)  tolua_getnumber(tolua_S,3,0));
 {
  DefineSimpleStoneMovable(kind,sound,glass);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DefineSimpleStoneMovable'.");
 return 0;
}

/* function: DefineSimpleFloor */
static int toluaI_enigma_world_DefineSimpleFloor00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  const char* kind = ((const char*)  tolua_getstring(tolua_S,1,0));
  double friction = ((double)  tolua_getnumber(tolua_S,2,0));
  double mousefactor = ((double)  tolua_getnumber(tolua_S,3,0));
 {
  DefineSimpleFloor(kind,friction,mousefactor);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DefineSimpleFloor'.");
 return 0;
}

/* function: GetScreen */
static int toluaI_enigma_video_GetScreen00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  Screen* toluaI_ret = (Screen*)  GetScreen();
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Screen"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScreen'.");
 return 0;
}

/* function: HideMouse */
static int toluaI_enigma_video_HideMouse00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  HideMouse();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HideMouse'.");
 return 0;
}

/* function: ShowMouse */
static int toluaI_enigma_video_ShowMouse00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  ShowMouse();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ShowMouse'.");
 return 0;
}

/* Open function */
int tolua_enigma_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_module(tolua_S,"enigma");
 tolua_constant(tolua_S,"enigma","NODIR",NODIR);
 tolua_constant(tolua_S,"enigma","NORTH",NORTH);
 tolua_constant(tolua_S,"enigma","EAST",EAST);
 tolua_constant(tolua_S,"enigma","SOUTH",SOUTH);
 tolua_constant(tolua_S,"enigma","WEST",WEST);
 tolua_function(tolua_S,"enigma","GetImage",toluaI_enigma_enigma_GetImage00);
 tolua_function(tolua_S,"enigma","GetImage",toluaI_enigma_enigma_GetImage01);
 tolua_function(tolua_S,"enigma","LoadImage",toluaI_enigma_enigma_LoadImage00);
 tolua_function(tolua_S,"enigma","RegisterImage",toluaI_enigma_enigma_RegisterImage00);
 tolua_function(tolua_S,"enigma","GetFont",toluaI_enigma_enigma_GetFont00);
 tolua_function(tolua_S,"enigma","date",toluaI_enigma_enigma_date00);
 tolua_module(tolua_S,"enigma");
 tolua_tablevar(tolua_S,"enigma","TwoPlayerGame",toluaI_get_enigma_enigma_TwoPlayerGame,toluaI_set_enigma_enigma_TwoPlayerGame);
 tolua_tablevar(tolua_S,"enigma","SingleComputerGame",toluaI_get_enigma_enigma_SingleComputerGame,toluaI_set_enigma_enigma_SingleComputerGame);
 tolua_tablevar(tolua_S,"enigma","ConserveLevel",toluaI_get_enigma_enigma_ConserveLevel,toluaI_set_enigma_enigma_ConserveLevel);
 tolua_tablevar(tolua_S,"enigma","AllowTogglePlayer",toluaI_get_enigma_enigma_AllowTogglePlayer,toluaI_set_enigma_enigma_AllowTogglePlayer);
 tolua_tablevar(tolua_S,"enigma","ShowMoves",toluaI_get_enigma_enigma_ShowMoves,toluaI_set_enigma_enigma_ShowMoves);
 tolua_tablevar(tolua_S,"enigma","Brittleness",toluaI_get_enigma_enigma_Brittleness,toluaI_set_enigma_enigma_Brittleness);
 tolua_tablevar(tolua_S,"enigma","SlopeForce",toluaI_get_enigma_enigma_SlopeForce,toluaI_set_enigma_enigma_SlopeForce);
 tolua_tablevar(tolua_S,"enigma","FlatForce",toluaI_get_enigma_enigma_FlatForce,toluaI_set_enigma_enigma_FlatForce);
 tolua_tablevar(tolua_S,"enigma","FrictionFactor",toluaI_get_enigma_enigma_FrictionFactor,toluaI_set_enigma_enigma_FrictionFactor);
 tolua_tablevar(tolua_S,"enigma","IceFriction",toluaI_get_enigma_enigma_IceFriction,toluaI_set_enigma_enigma_IceFriction);
 tolua_tablevar(tolua_S,"enigma","ElectricForce",toluaI_get_enigma_enigma_ElectricForce,toluaI_set_enigma_enigma_ElectricForce);
 tolua_tablevar(tolua_S,"enigma","BumperForce",toluaI_get_enigma_enigma_BumperForce,toluaI_set_enigma_enigma_BumperForce);
 tolua_tablevar(tolua_S,"enigma","MagnetForce",toluaI_get_enigma_enigma_MagnetForce,toluaI_set_enigma_enigma_MagnetForce);
 tolua_tablevar(tolua_S,"enigma","MagnetRange",toluaI_get_enigma_enigma_MagnetRange,toluaI_set_enigma_enigma_MagnetRange);
 tolua_tablevar(tolua_S,"enigma","WaterSinkSpeed",toluaI_get_enigma_enigma_WaterSinkSpeed,toluaI_set_enigma_enigma_WaterSinkSpeed);
 tolua_tablevar(tolua_S,"enigma","SwampSinkSpeed",toluaI_get_enigma_enigma_SwampSinkSpeed,toluaI_set_enigma_enigma_SwampSinkSpeed);
 tolua_tablevar(tolua_S,"enigma","WormholeForce",toluaI_get_enigma_enigma_WormholeForce,toluaI_set_enigma_enigma_WormholeForce);
 tolua_tablevar(tolua_S,"enigma","WormholeRange",toluaI_get_enigma_enigma_WormholeRange,toluaI_set_enigma_enigma_WormholeRange);
 tolua_tablevar(tolua_S,"enigma","HoleForce",toluaI_get_enigma_enigma_HoleForce,toluaI_set_enigma_enigma_HoleForce);
 tolua_function(tolua_S,"enigma","AddLevelPack",toluaI_enigma_enigma_AddLevelPack00);
 tolua_function(tolua_S,"enigma","AddZippedLevelPack",toluaI_enigma_enigma_AddZippedLevelPack00);
 tolua_function(tolua_S,"enigma","SetCompatibility",toluaI_enigma_enigma_SetCompatibility00);
 tolua_module(tolua_S,"world");
 tolua_cclass(tolua_S,"Object","Object");
 tolua_cclass(tolua_S,"Item","Object");
 tolua_cclass(tolua_S,"Stone","Object");
 tolua_function(tolua_S,"world","MakeObject",toluaI_enigma_world_MakeObject00);
 tolua_function(tolua_S,"world","MakeFloor",toluaI_enigma_world_MakeFloor00);
 tolua_function(tolua_S,"world","MakeItem",toluaI_enigma_world_MakeItem00);
 tolua_function(tolua_S,"world","MakeStone",toluaI_enigma_world_MakeStone00);
 tolua_function(tolua_S,"world","GetNamedObject",toluaI_enigma_world_GetNamedObject00);
 tolua_function(tolua_S,"world","Resize",toluaI_enigma_world_Resize00);
 tolua_function(tolua_S,"world","DefineSimpleStone",toluaI_enigma_world_DefineSimpleStone00);
 tolua_function(tolua_S,"world","DefineSimpleStoneMovable",toluaI_enigma_world_DefineSimpleStoneMovable00);
 tolua_function(tolua_S,"world","DefineSimpleFloor",toluaI_enigma_world_DefineSimpleFloor00);
 tolua_module(tolua_S,"video");
 tolua_function(tolua_S,"video","GetScreen",toluaI_enigma_video_GetScreen00);
 tolua_function(tolua_S,"video","HideMouse",toluaI_enigma_video_HideMouse00);
 tolua_function(tolua_S,"video","ShowMouse",toluaI_enigma_video_ShowMouse00);
 return 1;
}
/* Close function */
void tolua_enigma_close (lua_State* tolua_S)
{
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"enigma");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"enigma");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"world");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"video");
}
