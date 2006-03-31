-- Welcome II, a level for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 20
levelh = 13
create_world(levelw, levelh)
enigma.ConserveLevel = FALSE

-- DESIGN --
boden="fl-rough-blue"
wand="st-blue-sand"

-- FLOOR --
fill_floor(boden, 0,0,levelw,levelh)

-- STONES --
draw_border(wand)

if difficult then
 set_stone("st-fart",3,6,{name="fart1"})
 set_stone("st-timer",16,6,{action="trigger", target="fart1", interval=12.5})
end

-- ITEMS --
set_item("it-document", 11, 6, {text="Welcome to Enigma II"})

-- ACTORS --
actor1=set_actor("ac-blackball", 8.5,6.5, {player=0})

-- OXYD --
oxyd(3,3)
oxyd(16,3)
oxyd(3,9)
oxyd(16,9)

oxyd(6,6)
oxyd(13,6)

oxyd_default_flavor = "d"
oxyd_shuffle()













