-- A level for Enigma
-- Name: 	Easy shifting..
-- Filename: 	ralf01.lua
-- Copyright: 	(c) Feb 2003 Ralf Westram
-- Contact: 	amgine@reallysoft.de
-- License: 	GPL v2.0 or above

floortile1 = "fl-hay"
floortile2 = "fl-rough"
walltile = "st-greenbrown"
wallhole = "st-greenbrown_hole"

levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "b"       -- Default flavor for oxyd stones.

draw_border(walltile)
fill_floor(floortile1)
fill_floor(floortile2, 0, 0,3,3)
fill_floor(floortile2,17, 0,3,3)
fill_floor(floortile2, 0,10,3,3)
fill_floor(floortile2,17,10,3,3)

px = 0
py = 0

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone(walltile, i, line)
        elseif c == "%" then
            set_stone(wallhole, i, line)
        elseif c == "&" then
            set_stone( "st-invisible_magic", i, line)
        elseif c == "d" then
           set_stone( "st-death", i, line)
        elseif c == "m" then
           set_item( "it-magicwand", i, line)
        elseif c == "$" then
            set_stone( "st-brownie", i, line)
        elseif c == "@" then
            px = i
            py = line
        end
    end
end

--               123456789012345678
renderLine(  1, "o    |   |       o")
renderLine(  2, "   ###   #  dmd   ")
renderLine(  3, "#&##.....##%###%##")
renderLine(  4, "   %%.#.##   &  | ")
renderLine(  5, "   *# # # $#### #*")
renderLine(  6, "   ## $$$ $   &&#%")
renderLine(  7, "   #  # @ #=# #   ")
renderLine(  8, "   #=### ## # ##=#")
renderLine(  9, "   #  |     $$ #  ")
renderLine( 10, "   # ###  ###  &  ")
renderLine( 11, "o  #  *#  #*%  # o")

set_item( "it-trigger", 5, 3, {action="openclose",target="door1"})
set_item( "it-trigger", 6, 3, {action="openclose",target="door2"})
set_item( "it-trigger", 7, 3, {action="openclose",target="door3"})
set_item( "it-trigger", 8, 3, {action="openclose",target="door4"})
set_item( "it-trigger", 9, 3, {action="openclose",target="door5"})
set_item( "it-trigger", 6, 4, {action="openclose",target="door6"})
set_item( "it-trigger", 8, 4, {action="openclose",target="door7"})

set_stone(   "st-door", 7, 9, {name="door1", type="v"})
set_stone(   "st-door", 5, 8, {name="door2", type="h"})
set_stone(   "st-door",17, 4, {name="door3", type="v"})
set_stone(   "st-door",17, 8, {name="door4", type="h"})
set_stone(   "st-door",10, 1, {name="door5", type="v"})
set_stone(   "st-door",12, 7, {name="door6", type="h"})
set_stone(   "st-door", 6, 1, {name="door7", type="v"})

set_attrib( laser(4,5,TRUE,WEST), "name", "laser")
set_stone("st-switch", 7,11, {action="onoff", target="laser"})
document(12,11, "Dedicated to my friend Frank.")
document(18,5, "This level is quite straightforward, isn't it? Just continue.. ;-)")
set_actor("ac-blackball", px+0.5 , py+0.5)

oxyd(1,11)
oxyd(18,11)
oxyd(1,1)
oxyd(18,1)

















