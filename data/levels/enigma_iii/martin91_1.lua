--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.
 
Require("levels/lib/ant.lua")

create_world(39, 25)
fill_floor("fl-abyss")

if (not difficult) then
    enigma.Brittleness = 0.0
end


f1=cell{floor="fl-stone"}
f2=cell{floor="fl-rock"}
f3=cell{floor="fl-himalaya"}
f4=cell{floor="fl-gravel"}
f5=cell{floor="fl-leaves"}
f6=cell{floor="fl-rough-blue"}
f7=cell{floor="fl-rough-red"}
floor=cell{{{randomfloor, {f4,f2}}}}
fill_world_func(floor,1,1,37,23)

i0=cell{item="it-crack0"}
i1=cell{item="it-crack1"}
i2=cell{item="it-crack2"}
i3=cell{item="it-crack3"}
items=cell{{{randomfloor, {i0,i1,i2,i3}}}}
fill_world_func(items,1,1,37,23)

oxyd_default_flavor = "d"

oxyd( 5, 5)
oxyd(33, 5)
oxyd( 5,19)
oxyd(33,19)
oxyd(14,12)
oxyd(24,12)

oxyd_shuffle()

set_actor("ac-blackball", 17.5,6.5)

enigma.ConserveLevel = FALSE
















