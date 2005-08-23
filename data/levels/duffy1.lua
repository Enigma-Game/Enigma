-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Meditation Puzzle

levelw = 20
levelh = 13

create_world( levelw, levelh)
draw_border("st-brownie")
fill_floor("fl-leaves", 0,0,levelw,levelh)

set_stone( "st-greenbrown", 1, 7)
set_stone( "st-greenbrown", 2, 7)
set_stone( "st-greenbrown", 3, 7)
fill_floor("fl-water", 4,7, 1,1)
set_stone( "st-greenbrown", 5, 7)
set_stone( "st-greenbrown", 6, 7)
set_stone( "st-greenbrown", 7, 7)
set_stone( "st-greenbrown", 8, 7)
set_stone( "st-greenbrown", 9, 7)
set_stone( "st-greenbrown", 10, 7)
set_stone( "st-greenbrown", 11, 7)
set_stone( "st-greenbrown", 12, 7)
set_stone( "st-greenbrown", 13, 7)
set_stone( "st-greenbrown", 14, 7)
set_stone( "st-greenbrown", 15, 7)
set_stone( "st-greenbrown", 16, 7)
fill_floor("fl-water", 17,7, 1,1)
set_stone( "st-greenbrown", 18, 7)

set_stone( "st-greenbrown", 17, 2)

set_stone( "st-greenbrown", 16, 6)
set_stone( "st-greenbrown", 18, 6)

set_stone( "st-greenbrown", 8, 8)

set_stone( "st-greenbrown", 2, 1)
set_stone( "st-greenbrown", 2, 2)
set_stone( "st-greenbrown", 2, 3)
set_stone( "st-greenbrown", 2, 5)
fill_floor("fl-water", 2,6, 1,1)

set_stone( "st-greenbrown", 11, 1)
set_stone( "st-greenbrown", 11, 2)
fill_floor("fl-water", 11,4, 1,1)
set_stone( "st-greenbrown", 11, 5)
set_stone( "st-greenbrown", 11, 6)

fill_floor("fl-water", 3,10, 1,1)
fill_floor("fl-water", 4,10, 1,1)
fill_floor("fl-water", 13,10, 1,1)

fill_floor("fl-metal", 17,1, 1,1)

set_stone( "st-greenbrown", 12, 3)
set_stone( "st-greenbrown", 13, 3)
set_stone( "st-greenbrown", 15, 3)
set_stone( "st-greenbrown", 16, 3)
set_stone( "st-greenbrown", 17, 3)
set_stone( "st-greenbrown", 18, 3)
set_stone( "st-greenbrown", 12, 2)


set_stone( "st-wood", 15, 5)
set_stone( "st-wood", 15, 2)

set_stone( "st-greenbrown", 9, 4)

oneway( 17, 6, enigma.NORTH)

set_stone( "st-shogun-l", 13,5)
set_stone( "st-shogun-s",4, 2)
set_stone( "st-shogun-m", 12,10)


set_stone( "st-wood", 5, 3)
set_stone( "st-wood", 7, 3)

set_stone("st-door", 17, 1 , {name="door3", type="v"})
set_item("it-trigger", 1,6, {action="openclose", target="door3"})

set_stone("st-door", 2, 4 , {name="door1", type="v"})
set_item("it-trigger", 13,1, {action="openclose", target="door1"})

set_stone("st-door", 14, 3 , {name="door2", type="h"})
shogundot3(8,2, {target="door2", action="openclose"})

---draw_stones("st-stone_break", {10,5}, {1,0},3)

hollow(1,1)
hollow(18,2)

set_actor("ac-whiteball-small", 8,2, {player=0})
set_actor("ac-whiteball-small", 2,9, {player=0})

