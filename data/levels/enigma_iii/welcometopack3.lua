-- Welcome III, a level for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 20
levelh = 13
create_world(levelw, levelh)
enigma.ConserveLevel = FALSE

-- DESIGN --
boden="fl-brick"
wand="st-brick"

-- FLOOR --
fill_floor(boden, 0,0,levelw,levelh)

-- STONES --
draw_border(wand)

if difficult then
 set_stone("st-fart",19,0,{name="fart1"})
 set_stone("st-timer",0,0,{action="trigger", target="fart1", interval=15})
end

-- ITEMS --
set_item("it-document", 9, 6, {text="Welcome to Enigma III"})

-- ACTORS --
actor1=set_actor("ac-blackball", 3.5,4.5, {player=0})

-- OXYD --
oxyd(6,3)
oxyd(9,3)
oxyd(12,3)

oxyd(6,6)
oxyd(12,6)

oxyd(6,9)
oxyd(9,9)
oxyd(12,9)

oxyd_default_flavor = "d"
oxyd_shuffle()













