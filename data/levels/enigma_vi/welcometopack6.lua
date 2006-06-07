-- Welcome to Pack VI
-- Copyright (C) 2006 Raoul
-- Licensed under GPL v2.0

levelw = 20
levelh = 13

create_world(levelw, levelh)
enigma.ConserveLevel = FALSE

-- DESIGN --
boden="fl-black"
wand="st-glass"

draw_border(wand)
fill_floor(boden, 0,0,levelw,levelh)

if difficult then
 set_stone("st-fart",9,6,{name="fart1"})
 set_stone("st-timer",10,6,{action="trigger", target="fart1", interval=20})
end

-- ITEMS --
set_item("it-document", 14, 6, {text="Welcome to Enigma VI"})

-- ACTORS --
actor1=set_actor("ac-blackball", 5.5,6.5, {player=0})


oxyd( 3,4)
oxyd( 3,6)
oxyd( 3,8)
oxyd( 5,4)

oxyd( 5,8)
oxyd( 7,4)
oxyd( 7,6)
oxyd( 7,8)
oxyd(12,4)
oxyd(12,6)
oxyd(12,8)
oxyd(14,4)

oxyd(14,8)
oxyd(16,4)
oxyd(16,6)
oxyd(16,8)
oxyd_shuffle()






















