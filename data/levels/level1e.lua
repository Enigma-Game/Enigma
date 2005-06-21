--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-rock2")
fill_floor("fl-gray")

oxyd (1,1)
oxyd (14,5)

puzzle(4,8,PUZ_0001)
puzzle(2,8,PUZ_0100)
puzzle(3,8,PUZ_0101)
puzzle(7,8,PUZ_0001)
puzzle(5,8,PUZ_0100)
puzzle(6,8,PUZ_0101)
puzzle(5,5,PUZ_0001)
puzzle(4,11,PUZ_0100)
puzzle(4,5,PUZ_0101)

abyss(5,11)

-- document(12,3, "...geschafft")
document(12,3, "... did it!")
draw_stones("st-rock2", {9, 2}, {0,1},10)

set_item("it-trigger", 3,11, {action="openclose", target="door"})
set_stone("st-door", 9, 1 , {name="door", type="v"})

set_stone("st-rock2", 1,7)
set_stones("st-rock2", {{4,7},{5,7},{6,7},{3,10},{4,10}})

set_actor("ac-blackball", 7,3)
 
