--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 13)
oxyd_default_flavor = "a"

draw_border("st-death")

for i=0, 10 do
    draw_floor("fl-gradient", {2*i,0}, {0,1},level_height, {type=4})
    draw_floor("fl-gradient", {2*i+1,0}, {0,1},level_height, {type=3})
end

oxyd( 3, 0)
oxyd( 4, 0)
oxyd( 9, 0)
oxyd(10, 0)
oxyd(15, 0)
oxyd(16, 0)
oxyd( 3,12)
oxyd( 4,12)
oxyd( 9,12)
oxyd(10,12)
oxyd(15,12)
oxyd(16,12)
oxyd_shuffle()

set_actor("ac-blackball", 10, 6)
