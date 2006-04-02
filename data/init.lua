------------------------------------------------------------------------
-- Copyright (C) 2002,2003,2004,2005 Daniel Heck
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
-- $Id: init.lua,v 1.31 2004/05/01 16:07:58 dheck Exp $
------------------------------------------------------------------------

TRUE = 1
FALSE = 0

EASY = 1
HARD = 2

difficult = options.Difficulty==2

function Tick (deltatime)
    -- do nothing by default
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
    dofile(enigma.FindDataFile(filename))
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
   return set_stone("st-bigbrick",x,y,{connections=conn})
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

------------------------
-- Loading XML levels --
------------------------

function XML_parseargs (s)
    local arg = {}
    gsub(s, "(%w+)=([\"'])(.-)%2", function (w, _, a)
                                       arg[w] = a
                                   end)
    return arg
end

function XML_collect (s)
    local stack = {n=0}
    local top = {n=0}
    tinsert(stack, top)
    local ni,c,label,args, empty
    local i, j = 1, 1
    while 1 do
        ni,j,c,label,args, empty = strfind(s, "<(%/?)(%w+)(.-)(%/?)>", j)
        if not ni then break end
        local text = strsub(s, i, ni-1)
        if not strfind(text, "^%s*$") then
            tinsert(top, text)
        end
        if empty == "/" then  -- empty element tag
            tinsert(top, {n=0, label=label, args=XML_parseargs(args), empty=1})
        elseif c == "" then   -- start tag
            top = {n=0, label=label, args=XML_parseargs(args)}
            tinsert(stack, top)   -- new level
        else  -- end tag
            local toclose = tremove(stack)  -- remove top
            top = stack[stack.n]
            if stack.n < 1 then
                error("nothing to close with "..label)
            end
            if toclose.label ~= label then
                error("trying to close "..toclose.label.." with "..label)
            end
            tinsert(top, toclose)
        end 
        i = j+1
    end
    local text = strsub(s, i)
    if not strfind(text, "^%s*$") then
        tinsert(stack[stack.n], text)
    end
    if stack.n > 1 then
        error("unclosed "..stack[stack.n].label)
    end
    return stack[1]
end


function enigma.LoadDataFile (filename)
    local fname = enigma.FindDataFile (filename)
    if fname then
        local fh = openfile (fname, "r")
        local data = read (fh, "*a")
        closefile (fh)
        return data
    end
    return nil
end


o_shuffle = "YES"
o_reset = "NO"

function LoadLevelXML (xmldata)

    function assert (val, msg) 
        if not(val) then 
            error (msg or "Invalid XML file") 
        end
    end
    
    function parseattribs(a)
        if a then
            return dostring("return {"..a.."}")
        end
    end
    
    function parseactor (xml)
        local a = xml.args
        local attribs = parseattribs(a.attribs) or {player = player}
        local player = a.player
        if player then
            attribs.player = player
        end
        
--         if not attribs.player then
--             error ("player number missing for actor")
--         end

        local ac = SetActor(a.kind, a.x, a.y, attribs)

	if a.num then 
            enigma.NameObject(ac, "xmlac"..a.num)
	end
    end

    function split (str)
        local t = {}
        local b, e, c
        b = 1
        while 1 do
            b,e,c = strfind (str, "([^%s]+)", b)
            if not b then break end
            tinsert (t, c)
            b = e+1
        end
        return t
    end

    function parserow (y, str, prefix, func)
        local x = 0
        local t=split (str)
        for i=1, getn(t) do
            local e=t[i]
            local n=tonumber (e)
            if n then
                if n < 0 then x = x-n
                elseif n > 0 then
                    -- repeat last object n times
                    for j=1,n do
                        func (lastobj, x, y)
                        x = x+1
                    end
                end
            else
                if e == "#" then func (lastobj, x, y); x = x+1
                elseif e == "-" then x = x+1 
                else
                    lastobj = prefix .. e
                    func (lastobj, x, y)
                    x = x+1
                end
            end
        end
    end

    function parsestone (xml)
        local l = xml.label
        local a = xml.args
        if l == 'oxyd' then
            oxyd (a.x, a.y, a.flavor, a.color)
        elseif l == 'stone' then
            set_stone (a.kind, a.x, a.y, parseattribs(a.attribs))
        elseif l == 'border' then
            draw_border (a.kind)
        elseif l == 'row' then
            parserow (a.y, xml[1], "st-", set_stone)
        end
    end

    function parseitem (xml)
        local l = xml.label
        local a = xml.args
        if l == 'item' then
            set_item (a.kind, a.x, a.y, parseattribs(a.attribs))
        elseif l == 'document' then
            
        elseif l == 'row' then
            parserow (a.y, xml[1], "it-", set_item)
        end
    end
    
    function parseoption (xml)
        local a = xml.args
        if a.name=="oxydflavor" then 
            oxyd_default_flavor = a.value

        elseif a.name == "shuffle" then 
            o_shuffle = a.value

        elseif a.name == "reset" then 
            o_reset = a.value

        elseif a.name == "scrolling" then
            display.SetFollowMode (tonumber (a.value))

        elseif a.name == "brittleness" then
            enigma.Brittleness = a.value

        elseif a.name == "slopeforce" then 
            enigma.SlopeForce = a.value

        elseif a.name == "flatforce" then 
            enigma.FlatForce = a.value

        elseif a.name == "frictionfactor" then
            enigma.FrictionFactor = a.value

        elseif a.name == "electricforce" then 
            enigma.ElectricForce = a.value

        elseif a.name == "bumperforce" then
            enigma.BumperForce = a.value

        elseif a.name == "magnetforce" then
            enigma.MagnetForce = a.value

        elseif a.name == "magnetrange" then
            enigma.MagnetRange = a.value

        elseif a.name == "wormholeforce" then
            enigma.WormholeForce = a.value

        elseif a.name == "wormholerange" then
            enigma.WormholeRange = a.value

        elseif a.name == "holeforce" then
            enigma.HoleForce = a.value
	end
    end

    function parsefloor (xml)
        local l = xml.label
        local a = xml.args
        if l == "fill" then
            fill_floor (a.kind, 0, 0, level_width, level_height)
        elseif l == "floor" then
            set_floor (a.kind, a.x, a.y, parseattribs(a.attribs))
        elseif l == 'row' then
            parserow (a.y, xml[1], "fl-", set_floor)
        end
    end

    -- 
    -- <signals>
    --   <signal from="" to="" />
    -- </signals>
    --
    function parsesignal (xml)
        local l = xml.label
        local a = xml.args
        enigma.AddSignal (a.from, a.to)
    end

    --
    -- Syntax of rubberband description:
    --
    -- <rubberbands>
    --   <rubberband from="name" to="name" strength="" length="" minlen="" />
    -- </rubberbands>
    --
    function parseband (xml)
        local l = xml.label
        local a = xml.args
        local obj1 = enigma.GetNamedObject (a.from)
        local obj2 = enigma.GetNamedObject (a.to)
        enigma.AddRubberBand (obj1, obj2, a.strength or 10, a.length or 2, a.minlen or 0)
    end

    function check_difficulty (d)
        return ((not d) 
                or (d == 'hard' and options.Difficulty==HARD) 
                or (d == 'easy' and options.Difficulty==EASY))
    end
    
    x = XML_collect(xmldata)[1]
    assert (x, "Error parsing XML file")
    assert (x.label == "level")
    CreateWorld (x.args.width, x.args.height)

    local luacode = nil
    
    for i = 1,getn(x) do
        local xx = x[i]
        local l = xx.label
        if l == "lua" then
	    if luacode then error("Only one <lua> tag is allowed") end
	    luacode = xx[1]
        elseif l == "actors" then
            for i = 1,getn(xx) do
                parseactor (xx[i])
            end
        elseif l == "option" then
            parseoption (xx)
        elseif l == "floors" then
            if check_difficulty (xx.args.difficulty) then
                for i=1,getn(xx) do
                    parsefloor (xx[i])
                end
            end
        elseif l == "items" then
            if check_difficulty (xx.args.difficulty) then
                for i = 1,getn(xx) do
                    parseitem (xx[i])
                end
            end
        elseif l == "stones" then
            if check_difficulty (xx.args.difficulty) then

                for i = 1,getn(xx) do
                    parsestone (xx[i])
                end
            end
        elseif l == "signals" then
            if check_difficulty (xx.args.difficulty) then
                for i = 1,getn(xx) do
                    parsesignal (xx[i]) 
                end
            end
        elseif l == "rubberbands" then
            if check_difficulty (xx.args.difficulty) then
                for i = 1,getn(xx) do
                    parseband (xx[i])
                end
            end
        end
    end
    if o_shuffle == "YES" then
        oxyd_shuffle()
    end
    if o_reset == "NO" then
        enigma.ConserveLevel = TRUE
    else
        enigma.ConserveLevel = FALSE
    end

    if luacode then
    	dostring (luacode)
    end
end
