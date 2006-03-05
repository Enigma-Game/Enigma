-- Copyright (C) 2005 Raoul
-- Licensed under GPL v2.0
-- Grunddatei eines einfachen Levels ohne ant.lua

-- GENERAL --
levelw = 20
levelh = 13
create_world(levelw, levelh)
enigma.ConserveLevel = FALSE

-- FLOOR --
fill_floor("fl-metal", 0,0,20,13)



-- STONES --
draw_border("st-brick")
set_stone("st-puzzle", 6,6, {connections=PUZ_0010, oxyd="1"})
set_stone("st-puzzle", 6,8, {connections=PUZ_1000, oxyd="1"})
set_stone("st-puzzle", 8,6, {connections=PUZ_1000})
set_stone("st-puzzle", 8,8, {connections=PUZ_0010})


-- ITEMS --
set_item( "it-magicwand", 10, 10)


-- ACTORS --
local ac1=set_actor("ac-blackball", 4.5,3.25, {player=0})



-- OXYD --
oxyd(0,5)
oxyd(19,5)
oxyd_default_flavor = "d"
oxyd_shuffle()


















