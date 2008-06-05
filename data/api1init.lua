------------------------------------------------------------------------
-- Copyright (C) 2002,2003,2004,2005 Daniel Heck
-- Copyright (C) 2007,2008 Ronald Lamprecht
--
-- This program is free software; you can redistribute it and/or
-- modify it under the terms of the GNU General Public License
-- as published by the Free Software Foundation; either version 2
-- of the License, or (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License along
-- with this program; if not, write to the Free Software Foundation, Inc.,
-- 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
--
------------------------------------------------------------------------

-----------------------------------------------
-- Use Enigma internal random implementation --
-----------------------------------------------

if type(en) == "table" then
    math.random = en.random
else
    math.random = enigma.random
end
random = math.random
-- randseed is issued by Enigma application and must not be disturbed
math.randomseed = function () end

-------------------------------
-- Compatibility for old API --
-------------------------------

enigma._MakeObject = enigma.MakeObject
enigma._GetKind = enigma.GetKind
enigma._SetAttrib = enigma.SetAttrib
enigma._GetAttrib = enigma.GetAttrib
enigma._GetObjectTemplate = enigma.GetObjectTemplate

RenamingObjectsOld2New = {
}

RenamingObjectsNew2Old = {
    it_blocker = "it-blocker",
    it_blocker_new = "it-blocker-new",
    it_brush = "it-brush",
    it_coin_s = "it-coin1",
    it_coin_m = "it-coin2",
    it_coin_l = "it-coin4",
    it_cross = "it-cross",
    it_extralife = "it-extralife",
    it_floppy = "it-floppy",
    it_glasses = "it-glasses",
    it_glasses_broken = "it-glasses-broken",
    it_hammer = "it-hammer",
    it_magnet = "it-magnet",
    it_magnet_on = "it-magnet-on",
    it_magnet_off = "it-magnet-off",
    it_sword = "it-sword",
    it_trigger = "it-trigger",
    it_umbrella = "it-umbrella",
    it_vortex = "it-vortex-open",
    it_vortex_open = "it-vortex-open",
    it_vortex_closed = "it-vortex-closed",
    it_wormhole_on = "it-wormhole",
    it_wormhole_off = "it-wormhole-off",
    it_wrench = "it-wrench",
    st_actorimpulse = "st-actorimpulse",
    st_actorimpulse_invisible = "st-actorimpulse_invisible",
    st_blocker = "st-blocker",
    st_blocker_new = "st-blocker-growing",
    st_bluesand = "st-blue-sand",
    st_bluesand_w = "st-bigbluesand-w",
    st_bluesand_s = "st-bigbluesand-s",
    st_bluesand_sw = "st-bigbluesand-sw",
    st_bluesand_e = "st-bigbluesand-e",
    st_bluesand_ew = "st-bigbluesand-ew",
    st_bluesand_es = "st-bigbluesand-es",
    st_bluesand_esw = "st-bigbluesand-esw",
    st_bluesand_n = "st-bigbluesand-n",
    st_bluesand_nw = "st-bigbluesand-nw",
    st_bluesand_ns = "st-bigbluesand-ns",
    st_bluesand_nsw = "st-bigbluesand-nsw",
    st_bluesand_ne = "st-bigbluesand-ne",
    st_bluesand_new = "st-bigbluesand-new",
    st_bluesand_nes = "st-bigbluesand-nes",
    st_bluesand_nesw = "st-bigbluesand-nesw",
    st_boulder = "st-bolder",
    st_boulder_n = "st-bolder-n",
    st_boulder_e = "st-bolder-e",
    st_boulder_s = "st-bolder-s",
    st_boulder_w = "st-bolder-w",
    st_brick = "st-brick",
    st_brick_w = "st-bigbrick-w",
    st_brick_s = "st-bigbrick-s",
    st_brick_sw = "st-bigbrick-sw",
    st_brick_e = "st-bigbrick-e",
    st_brick_ew = "st-bigbrick-ew",
    st_brick_es = "st-bigbrick-es",
    st_brick_esw = "st-bigbrick-esw",
    st_brick_n = "st-bigbrick-n",
    st_brick_nw = "st-bigbrick-nw",
    st_brick_ns = "st-bigbrick-ns",
    st_brick_nsw = "st-bigbrick-nsw",
    st_brick_ne = "st-bigbrick-ne",
    st_brick_new = "st-bigbrick-new",
    st_brick_nes = "st-bigbrick-nes",
    st_brick_nesw = "st-bigbrick-nesw",
    st_coinslot = "st-coinslot",
    st_floppy = "st-floppy",
    st_fourswitch = "st-fourswitch",
    st_laser_w = "st-laser-w",
    st_laser_s = "st-laser-s",
    st_laser_e = "st-laser-e",
    st_laser_n = "st-laser-n",
    st_laserswitch = "st-laserswitch",
    st_laserflop = "st-lasertimeswitch",
    st_lightpassenger = "st-lightpassenger",
    st_lightpassenger_off = "st-lightpassenger_off",
    st_monoflop = "st-timeswitch",
    st_oxyd = "st-oxyd",
    st_panel = "st-wood_001",
    st_polarswitch = "st-polarswitch",
    st_rotator_cw = "st-rotator-right",
    st_rotator_ccw = "st-rotator-left",
    st_rotator_cw_movable = "st-rotator_move-right",
    st_rotator_ccw_movable = "st-rotator_move-left",
    st_switch = "st-switch",
    st_switch_black = "st-switch_black",
    st_switch_white = "st-switch_white",
    st_timer = "st-timer",
    st_window = "st-window"
}

for k,v in pairs(RenamingObjectsNew2Old) do
    RenamingObjectsOld2New[v] = k
end

function enigma.MakeObject(name)
    if name == "st-key" then
        local obj = enigma._MakeObject("st_key")
        -- Old API keycode default was 0 for st-key, but 1 for it-key!
        enigma._SetAttrib(obj, "code", 0)
        return obj
    elseif name == "st-key_a" then
        local obj = enigma._MakeObject("st_key")
        enigma._SetAttrib(obj, "code", 1)
        return obj
    elseif name == "st-key_b" then
        local obj = enigma._MakeObject("st_key")
        enigma._SetAttrib(obj, "code", 2)
        return obj
    elseif name == "st-key_c" then
        local obj = enigma._MakeObject("st_key")
        enigma._SetAttrib(obj, "code", 3)
        return obj
    elseif name == "st-switch" then
        local obj = enigma._MakeObject("st_switch")
        enigma._SetAttrib(obj, "instant", true)
        return obj
    elseif name == "st-switch_black" then
        local obj = enigma._MakeObject("st_switch_black")
        enigma._SetAttrib(obj, "instant", true)
        return obj
    elseif name == "st-switch_white" then
        local obj = enigma._MakeObject("st_switch_white")
        enigma._SetAttrib(obj, "instant", true)
        return obj
    elseif name == "it-key_a" or  name == "it-key" then
        local obj = enigma._MakeObject("it_key")
        enigma._SetAttrib(obj, "code", 1)
        return obj
    elseif name == "it-key_b" then
        local obj = enigma._MakeObject("it_key")
        enigma._SetAttrib(obj, "code", 2)
        return obj
    elseif name == "it-key_c" then
        local obj = enigma._MakeObject("it_key")
        enigma._SetAttrib(obj, "code", 3)
        return obj
    elseif name == "it-sensor" then
        local obj = enigma._MakeObject("it_sensor")
        enigma._SetAttrib(obj, "invisible", true)
        return obj
    elseif name == "it-inversesensor" then
        local obj = enigma._MakeObject("it_sensor")
        enigma._SetAttrib(obj, "invisible", true)
        enigma._SetAttrib(obj, "inverse", true)
        return obj
    elseif name == "it-wormhole" then
        local obj = enigma._MakeObject("it_wormhole_on")
        enigma._SetAttrib(obj, "scissor", false)
        return obj
    elseif name == "it-wormhole-off" then
        local obj = enigma._MakeObject("it_wormhole_off")
        enigma._SetAttrib(obj, "scissor", false)
        return obj
    elseif name == "it-vortex-open" then
        local obj = enigma._MakeObject("it_vortex_open")
        enigma._SetAttrib(obj, "scissor", false)
        return obj
    elseif name == "it-vortex-closed" then
        local obj = enigma._MakeObject("it_vortex_closed")
        enigma._SetAttrib(obj, "scissor", false)
        enigma._SetAttrib(obj, "autoclose", true)
        return obj
    end

    newname = RenamingObjectsOld2New[name]
    
    if name == "st-laser" then
        newname = "st_laser_e"
    end
    
    if newname ~= nil then
        return enigma._MakeObject(newname)
    else
        return enigma._MakeObject(name)
    end
end

function enigma.GetKind(obj)
    local _newname = enigma._GetKind(obj)
    local _oldname = RenamingObjectsNew2Old[_newname]

    if _newname == "st_key" then
        local code = enigma._GetAttrib(obj, "code")
        if code == 0 then
            return "st-key"
        elseif code == 1 then
            return "st-key_a"
        elseif code == 2 then
            return "st-key_b"
        elseif code == 3 then
            return "st-key_c"
        else -- arbitrary keycodes
            return "st-key"
        end
    end
    if _newname == "it_key" then
        local code = enigma._GetAttrib(obj, "code")
        if code == 1 then
            return "it-key_a"
        elseif code == 2 then
            return "it-key_b"
        elseif code == 3 then
            return "it-key_c"
        else -- arbitrary keycodes
            return "it-key_a"
        end
    end
    if _newname == "it_sensor" then
        local code = enigma._GetAttrib(obj, "inverse")
        if code == false then
            return "it-sensor"
        else
            return "it-inversesensor"
        end
    end
    if string.sub(_newname, 1, 8) == "st_laser" then
        return "st-laser"
    end
    if _oldname ~= nil then
        return _oldname
    else
        return _newname
    end
end

function enigma.SetAttrib(obj, key, val)
     local _val = val
     local _key = key
     local _obj_name = enigma.GetKind(obj)

     if _obj_name == "st-oxyd" then
         if key == "color" then
	     _val = 0 + val   -- convert to int
             _key = "oxydcolor"
	 end
     end
     if key == "connections" and _obj_name ~= "st-puzzle" then
         if val == 1 then _val = ""
         elseif  val == 2  then _val = "w"
         elseif  val == 3  then _val = "s"
         elseif  val == 4  then _val = "sw"
         elseif  val == 5  then _val = "e"
         elseif  val == 6  then _val = "ew"
         elseif  val == 7  then _val = "es"
         elseif  val == 8  then _val = "esw"
         elseif  val == 9  then _val = "n"
         elseif  val == 10 then _val = "nw"
         elseif  val == 11 then _val = "ns"
         elseif  val == 12 then _val = "nsw"
         elseif  val == 13 then _val = "ne"
         elseif  val == 14 then _val = "new"
         elseif  val == 15 then _val = "nes"
         elseif  val == 16 then _val = "nesw"
         end
     end
     if key == "keycode" then
         _key = "code"
     end
     if key == "delay" then
         _key = "interval"
     end
     if key == "dir" or key == "direction" then
         _key = "orientation"
     end
     if key == "on" then
         _key = "state"
     end
     if key == "value" then
         _key = "coin_value"
     end
     if key == "force" and (_obj_name == "st-actorimpulse" or
             _obj_name == "st-actorimpulse_invisible")  then
         _key = "strength"
     end
     if key == "friction_factor" then
         _key = "friction"
     end
     if key == "gradient_factor" then
         _key = "gradient"
     end
     if key == "targetx" then
         local d = enigma._GetAttrib(obj, "destination")
         if (en.usertype(d) == "position") then
             _val = po(val + 0, d.y)
         else
             _val = po(val + 0, 0)
         end
         _key = "destination"
     end
     if key == "targety" then
         local d = enigma._GetAttrib(obj, "destination")
         if (en.usertype(d) == "position") then
             _val = po(d.x, val + 0)
         else
             _val = po(0, val + 0)
         end
         _key = "destination"
     end
     if key == "target1" or key == "target2" or key == "target3" or key == "target4" then
        local i = 0 + string.sub(key, -1)  -- the target number
        local d = enigma._GetAttrib(obj, "destination")
        if (d == nil) or (type(d) ~= "table") then
            d = {}
        end

        local c = {}

        for str in string.gmatch(val, "%d+") do
            table.insert(c, 0 + str)
        end
        d[i] = enigma.GetFloor(c[1], c[2]) -- requires floor to be set first (invalid for Nat method - level Bodyguard)
        if d[i] == nil then
             error("Actor Horse without floor error")
        end
        for j = 1, i do
           if d[j] == nil then
              d[j] = d[i]
           end
        end
        _key = "destination"
        _val = d
     end
     if key == "invisible" then
         if val == 1 then _val = true else _val = false end
     end
     enigma._SetAttrib(obj, _key, _val)
end

function enigma.GetAttrib(obj, key)
     local _key = key
     local _obj_name = enigma.GetKind(obj)
     if key == "keycode" then
         _key = "code"
     end
     if key == "delay" then
         _key = "interval"
     end
     if key == "value" then
         _key = "coin_value"
     end
     if key == "force" and (_obj_name == "st-actorimpulse" or
             _obj_name == "st-actorimpulse_invisible")  then
         _key = "strength"
     end
     if key == "friction_factor" then
         _key = "friction"
     end
     if key == "gradient_factor" then
         _key = "gradient"
     end
     if key == "blackball" or key == "whiteball" then
         _key = "color"
     end
     if _obj_name == "st-oxyd" then
         _key = "oxydcolor"
     end
     if key == "dir" or key == "direction" then
         _key = "orientation"
     end
     if key == "on" then
         _key = "state"
     end

     local val = enigma._GetAttrib(obj, _key)
     
     if key == "blackball" then
        if val == 0 then val = 1 else val = 0 end
     end
     if key == "whiteball" then
        if val == 1 then val = 1 else val = 0 end
     end
     if key == "connections" and _obj_name ~= "st-puzzle" then
         if val == "" then val = 1
         elseif  val == "w"    then val = 2
         elseif  val == "s"    then val = 3
         elseif  val == "sw"   then val = 4
         elseif  val == "e"    then val = 5
         elseif  val == "ew"   then val = 6
         elseif  val == "es"   then val = 7
         elseif  val == "esw"  then val = 8
         elseif  val == "n"    then val = 9
         elseif  val == "nw"   then val = 10
         elseif  val == "ns"   then val = 11
         elseif  val == "nsw"  then val = 12
         elseif  val == "ne"   then val = 13
         elseif  val == "new"  then val = 14
         elseif  val == "nes"  then val = 15
         elseif  val == "nesw" then val = 16
         end
     end
     if _obj_name == "st-oxyd" then
         if key == "color" then
	     val = "" .. val   -- convert to string
	 end
     end
     if key == "targetx" then
         local d = enigma._GetAttrib(obj, "destination")
         if (en.usertype(d) == "position") then
             val = d.x
         end
     end
     if key == "targety" then
         local d = enigma._GetAttrib(obj, "destination")
         if (en.usertype(d) == "position") then
             val = d.y
         end
     end
     if key == "invisible" then
         if val == false then val = 0 else val = 1 end
     end
     return val
end

function enigma.GetObjectTemplate(kind)
    local _kind = RenamingObjectsOld2New[kind]
    if _kind == nil then
        _kind = kind
    end
    if kind == "st-key" or kind == "st-key_a" or kind == "st-key_b" or kind == "st-key_c" then
        _kind = "st_key"
    end
    return enigma._GetObjectTemplate(_kind)
end

MessageRenaming = {
    it_blocker__trigger = "toggle",
    it_blocker__openclose = "toggle",
    ["it-burnable__trigger"] = "ignite",
    ["it-burnable_ignited__trigger"] = "ignite",
    ["it-burnable_burning__trigger"] = "ignite",
    ["it-burnable_fireproof__trigger"] = "ignite",
    ["it-burnable_ash__trigger"] = "ignite",
    ["it-burnable_oil__trigger"] = "ignite",
    ["it-hill__trigger"] = "flip",
    ["it-hollow__trigger"] = "flip",
    it_magnet__onoff = "toggle",
    ["it-tinyhill__trigger"] = "flip",
    ["it-tinyhollow__trigger"] = "flip",
    it_vortex__openclose = "toggle",
    it_vortex__trigger = "toggle",
    it_wormhole__onoff = "toggle",
    ["st-black1__trigger"] = "signal",
    ["st-black2__trigger"] = "signal",
    ["st-black3__trigger"] = "signal",
    ["st-black4__trigger"] = "signal",
    st_blocker__openclose = "toggle",
    st_blocker__trigger = "toggle",
    st_blocker_new__openclose = "toggle",
    st_blocker_new__trigger = "toggle",
    st_boulder__direction = "orientate",
    ["st-fart__trigger"] = "toggle",
    st_fourswitch__trigger = "toggle",
    st_floppy__onoff = "toggle",
    st_key__onoff = "toggle",
    st_laser__onoff = "toggle",
    ["st-stoneimpulse__trigger"] = "signal",
    ["st-stoneimpulse-hollow__trigger"] = "signal",
    ["st-stoneimpulse_movable__trigger"] = "signal",
    st_lightpassenger__trigger = "toggle",
    st_lightpassenger__onoff = "toggle",
    ["st-mirror__trigger"] = "turn",
    st_oxyd__trigger = "open",
    ["st-plain__trigger"] = "signal",
    ["st-plain_hole__trigger"] = "signal",
    st_polarswitch__onoff = "toggle",
    st_switch__onoff = "toggle",
    ["st-volcano__trigger"] = "toggle",
    ["st-white1__trigger"] = "signal",
    ["st-white2__trigger"] = "signal",
    ["st-white3__trigger"] = "signal",
    ["st-white4__trigger"] = "signal"
}
----------------------
-- old API 1.0      --
----------------------

TRUE = 1
FALSE = 0

EASY = 1
HARD = 2

difficult = options.Difficulty==2

function Tick (deltatime)
    -- do nothing by default
end

if difficult then
    enigma.SetScrambleIntensity(10)
else
    enigma.SetScrambleIntensity(3)
end

----------------------
-- Global variables --
----------------------

level_width = 0
level_height = 0

oxyd_default_flavor = "b"       -- Default flavor for oxyd stones.
oxyd_count = 0
oxyd_current_color = 0

DefaultAttribs = {}

---------------------
-- Helper routines --
---------------------

function PrintTable(t)
    for i,v in pairs(t) do
        if type(v)=="table" then
            print (i.." - "..v[1]..","..v[2])
        else
            print (i.." - "..v)
        end
    end
end


function Require(filename)
    enigma.LoadLib(string.sub(filename,8,string.len(filename)-4))
end


----------------------------------
-- Routines for creating levels --
----------------------------------

function CreateWorld(w, h)
    oxyd_default_flavor = "b"
    oxyd_count = 0
    oxyd_current_color = 0
    DefaultAttribs = {}

    level_width = w
    level_height = h
    world.Resize(w, h)
end


function SetDefaultAttribs(objname, attribs)
    local list = DefaultAttribs[objname]
    if list then
        for key,val in pairs(attribs) do list[key] = val end
    else
        DefaultAttribs[objname]= attribs
    end
end

function SetAttrib(obj, key,val)
    if key == "name" then
        enigma.NameObject(obj, val)
    end
    enigma.SetAttrib(obj, key, val)
end

GetAttrib = enigma.GetAttrib
GetKind   = enigma.GetKind

function set_attribs(obj, attrs)
    if not attrs then return end
    if type(attrs) ~= "table" then
        error("Can't assign attributes, expected table but got "..type(attrs)..".")
    end
    for key,val in pairs(attrs) do
	set_attrib(obj, key, val)
    end
end

function MakeObject(name, attrs)
    local obj = enigma.MakeObject(name)
    local a=DefaultAttribs[name]
    if a then set_attribs(obj, a) end
    set_attribs(obj, attrs)
    return obj
end

function set_floor(name, x, y, attrs)
    local obj = make_object(name, attrs)
    enigma.SetFloor(x,y,obj)
    return obj
end
function set_item(name, x, y, attrs)
    local obj = make_object(name, attrs)
    enigma.SetItem(x,y,obj)
    return obj
end
function set_stone(name, x,y, attrs)
    local obj=make_object(name, attrs)
    enigma.SetStone(x,y,obj)
    return obj
end

kill_stone = enigma.KillStone
kill_item = enigma.KillItem

function SendMessage (target, msg, arg)
    if type(target) == "string" then
        enigma.SendMessage (enigma.GetNamedObject (target), msg, arg)
    else
        enigma.SendMessage (target, msg, arg)
    end
end

function SetAttribs (target, attribs)
    if type (target) == "string" then
        set_attribs(enigma.GetNamedObject(target), attribs)
    else
        set_attribs(target, attribs)
    end
end


function AddRubberBand(obj1,obj2, strength,length, minlen)
    enigma.AddRubberBand(obj1,obj2, strength or 10, length or 2, minlen or 0)
end

function fill_floor(name, x0,y0, w,h)
    if x0 == nil then x0 = 0 end
    if y0 == nil then y0 = 0 end
    if w  == nil then w  = level_width end
    if h  == nil then h  = level_height end
    for y=y0,y0+h-1 do
	for x=x0,x0+w-1 do
            set_floor(name, x, y) 
        end
    end
end

function fill_items(name, x0,y0,w,h)
    for y=y0,y0+h-1 do
	for x=x0,x0+w-1 do set_item(name, x, y) end
    end
end

function fill_stones(name, x0,y0, w,h)
    for y=y0,y0+h-1 do
        for x=x0,x0+w-1 do set_stone(name, x, y) end
    end
end

function draw_floor(name, xy0, xystep, n, attrs)
    local x,y = xy0[1],xy0[2]
    for i=1,n do
	set_floor(name, x, y, attrs)
	x = x+xystep[1]
	y = y+xystep[2]
    end
end

function draw_checkerboard_floor (name1, name2, x, y, w, h, attrs)
    if x == nil then x = 0 end
    if y == nil then y = 0 end
    if w  == nil then w  = level_width end
    if h  == nil then h  = level_height end
    for i=1,h do
        for j=1,w do
             if mod(i,2) == mod(j,2) then
                 set_floor (name1, x+j-1, y+i-1, attrs )
             else
                 set_floor (name2, x+j-1, y+i-1, attrs )
            end
        end
    end
end

function draw_items(name, xy0, xystep, n, attrs)
    local x,y = xy0[1],xy0[2]
    for i=1,n do
	set_item(name, x, y, attrs)
	x = x+xystep[1]
	y = y+xystep[2]
    end
end

function draw_stones(name, xy0, xystep, n, attrs)
    local x,y = xy0[1],xy0[2]
    for i=1,n do
	set_stone(name, x, y, attrs)
	x = x+xystep[1]
	y = y+xystep[2]
    end
end

function draw_border(stonename, x0, y0, w, h)
    if x0 == nil then x0 = 0 end
    if y0 == nil then y0 = 0 end
    if w  == nil then w  = level_width end
    if h  == nil then h  = level_height end
    draw_stones(stonename, {x0,y0}, {1,0}, w)
    draw_stones(stonename, {x0,y0+h-1},{1,0}, w)
    draw_stones(stonename, {x0,y0}, {0,1}, h)
    draw_stones(stonename, {x0+w-1,y0},{0,1}, h)
end

function set_stones(name, poslist, attrs)
    for i,xy in pairs(poslist) do
	set_stone(name, xy[1], xy[2], attrs)
    end
end

function SetActor(name, x, y, attrs)
    local obj=make_object(name, attrs)
    enigma.SetActor(x, y,obj)
    return obj
end

---------------------------------------------
-- Creation of particular kinds of objects --
---------------------------------------------
function fakeoxyd(x,y) 
    return set_stone("st-fakeoxyd",x,y) 
end


-- Create an oxyd stone with the current default flavor.
function oxyd(x, y, flavor, color)
    local f = flavor or oxyd_default_flavor
    local c = color or oxyd_current_color
    local a = {flavor=f, color=""..c}
    local obj = set_stone("st-oxyd", x, y, a)

    -- if "color" argument not provided, use next available color
    if not color then
        oxyd_count = oxyd_count +1

        if oxyd_count == 2 then
            oxyd_count = 0
            oxyd_current_color = oxyd_current_color + 1
        end
    end

    return obj
end

-- Shuffle the colors of all oxyd stones in the current landscape.
function oxyd_shuffle()
    enigma.SendMessage(enigma.GetObjectTemplate("st-oxyd"), "shuffle", nil)
end

-- Close all oxyd stones in the current landscape
function oxyd_closeall()
    enigma.SendMessage(enigma.GetObjectTemplate("st-oxyd"), "closeall", nil)
end

function oneway(x,y,orient)
    return set_stone("st-oneway",x,y,{orientation=orient})
end
function laser(x,y,is_on,dir)
    return set_stone("st-laser",x,y,{on=is_on, dir=dir})
end

function mirrorp(x,y,movable, transp,orient)
    return set_stone("st-pmirror", x, y, {movable=movable, transparent=transp,
                         orientation=orient})
end
function mirror3(x,y,movable, transp, orient)
    return set_stone("st-3mirror", x, y, {movable=movable, transparent=transp,
                         orientation = orient})
end

PUZ_0000=1                      -- hollow
PUZ_0001=2                      -- w
PUZ_0010=3                      -- s
PUZ_0011=4                      -- sw
PUZ_0100=5                      -- e
PUZ_0101=6                      -- ew
PUZ_0110=7                      -- es
PUZ_0111=8                      -- esw
PUZ_1000=9                      -- n
PUZ_1001=10                     -- nw
PUZ_1010=11                     -- ns
PUZ_1011=12                     -- nsw
PUZ_1100=13                     -- ne
PUZ_1101=14                     -- new
PUZ_1110=15                     -- nes
PUZ_1111=16                     -- nesw

-- functions using puzzle-style tiles:

function puzzle(x, y, conn)
    return set_stone("st-puzzle", x,y, {connections=conn})
end
function puzzle2(x, y, conn)
    return set_stone("st-puzzle", x,y, {connections=conn,oxyd=1})
end
function bigbrick(x, y, conn)
   return set_stone("st-bigbrick-n",x,y,{connections=conn})
end

--

function switch(x,y,target,action)
    return set_stone("st-switch", x,y, {target=target, action=action})
end

function abyss(x,y) set_floor("fl-abyss",x,y) end




-----------
-- ITEMS --
-----------
function hollow(x,y) set_item("it-hollow", x,y) end
function Document(x,y,t) set_item("it-document", x, y, {text=t}) end
function hammer(x,y) set_item("it-hammer",x,y) end
function dynamite(x,y) set_item("it-dynamite",x,y) end
function bomb(x,y) set_item("it-blackbomb",x,y) end
function keya(x,y) set_item("it-key", x,y, {keycode=1.0}) end
function keyb(x,y) set_item("it-key", x,y, {keycode=2.0}) end
function keyc(x,y) set_item("it-key", x,y, {keycode=3.0}) end

function shogundot1(x,y,attrs) set_item("it-shogun-s", x, y, attrs) end
function shogundot2(x,y,attrs) set_item("it-shogun-m", x, y, attrs) end
function shogundot3(x,y,attrs) set_item("it-shogun-l", x, y, attrs) end

function Wormhole(x,y,targetx, targety, attribs)
    local attrs = attribs or {}
    attrs.targetx = targetx
    attrs.targety = targety

    set_item("it-wormhole", x,y, attrs)
end

function Doorh(x,y,attrs)
    local attrs = attrs or {}
    attrs.type="h"
    set_stone("st-door",x,y,attrs)
end
function Doorv(x,y,attrs)
    local attrs = attrs or {}
    attrs.type="v"
    set_stone("st-door",x,y,attrs)
end

EAST = enigma.EAST
NORTH = enigma.NORTH
SOUTH = enigma.SOUTH
WEST = enigma.WEST

---------------
-- GRADIENTS --
---------------

SLOPE_S         = 1
SLOPE_N         = 2
SLOPE_E         = 3
SLOPE_W         = 4
SLOPE_LARGE_SE  = 5
SLOPE_LARGE_SW  = 6
SLOPE_LARGE_NE  = 7
SLOPE_LARGE_NW  = 8
SLOPE_SMALL_SE  = 9
SLOPE_SMALL_NE  = 10
SLOPE_SMALL_SW  = 11
SLOPE_SMALL_NW  = 12
SLOPE_S_FORCE_W = 13
SLOPE_N_FORCE_W = 14
SLOPE_S_FORCE_E = 15
SLOPE_N_FORCE_E = 16
SLOPE_E_FORCE_N = 17
SLOPE_W_FORCE_N = 18
SLOPE_E_FORCE_S = 19
SLOPE_W_FORCE_S = 20
FLAT_FORCE_S    = 21
FLAT_FORCE_N    = 22
FLAT_FORCE_E    = 23
FLAT_FORCE_W    = 24

function Gradient( x, y, type )
   if (type==nil) then error("Illegal gradient type"); end
   if (type>=1 and type<=24) then
      set_floor( "fl-gradient", x, y, {type=type} )
   else
      error("Unknown gradient type '"..type.."'");
   end
end


Signal = enigma.AddSignal
SetItem = set_item

----------------------------------------------------
-- Define lowercase aliases for various functions --
----------------------------------------------------

create_world = CreateWorld
get_attrib = GetAttrib
get_kind = GetKind
set_attrib = SetAttrib
make_object = MakeObject
set_actor = SetActor

document = Document
doorh    = Doorh
doorv    = Doorv
gradient = Gradient
wormhole = Wormhole

