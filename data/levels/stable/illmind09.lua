-- deja-vu, a level for Enigma
-- Copyright (C) 2005 illmind
-- Licensed under GPL v2.0 or above 

-- level dimensions and standards
levelw = 39
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "d"
draw_border("st-rock1")
fill_floor("fl-black", 0,0, level_width,level_height)

-- extra stones in level
draw_stones("st-rock1", {19,1}, {0,1}, 11)
draw_stones("st-rock1", {14,2}, {1,0}, 5)
draw_stones("st-rock1", {14,4}, {1,0}, 5)
draw_stones("st-rock1", {14,6}, {1,0}, 5)
draw_stones("st-rock1", {14,8}, {1,0}, 5)
draw_stones("st-rock1", {14,10}, {1,0}, 5)
draw_stones("st-rock1", {33,2}, {1,0}, 5)
draw_stones("st-rock1", {33,4}, {1,0}, 5)
draw_stones("st-rock1", {33,6}, {1,0}, 5)
draw_stones("st-rock1", {33,8}, {1,0}, 5)
draw_stones("st-rock1", {33,10}, {1,0}, 5)
draw_stones("st-wood", {8,10}, {1,0}, 3)
draw_stones("st-wood", {27,10}, {1,0}, 3)
draw_stones("st-stone_break", {1,8}, {0,1}, 4)
draw_stones("st-stone_break", {2,4}, {1,0}, 5)
draw_stones("st-stone_break", {21,4}, {1,0}, 5)
draw_stones("st-stone_break", {3,8}, {0,1}, 4)
draw_stones("st-stone_break", {20,8}, {0,1}, 4)
draw_stones("st-stone_break", {22,8}, {0,1}, 4)
draw_stones("st-stone_break", {4,11}, {1,0}, 4)
draw_stones("st-stone_break", {23,11}, {1,0}, 4)
set_stones("st-glass1_hole", {{2,11},{21,11}})
set_stones("st-chameleon", {{1,1},{20,1}})
set_stones("st-stone_break", {{4,5},{4,6},{5,6},{5,7},{6,7},{4,8},{6,8},{5,9},{6,9},{7,9},{4,10},{5,10}})
set_stones("st-stone_break", {{23,5},{23,6},{24,6},{24,7},{25,7},{23,8},{25,8},{24,9},{25,9},{26,9},{23,10},{24,10}})
set_stones("st-wood", {{4,7},{4,9},{5,8},{23,7},{23,9},{24,8},{1,7},{20,7}})
set_stones("st-glass", {{8,1},{8,2},{8,4},{1,5},{3,5},{5,5},{6,5},{7,5},{2,6},{3,6},{6,6}})
set_stones("st-glass", {{2,7},{3,7},{7,7},{2,8},{7,8},{8,8},{8,9},{8,11},{2,9},{2,10}})
set_stones("st-glass", {{27,1},{27,2},{27,4},{20,5},{22,5},{24,5},{25,5},{26,5},{21,6},{22,6},{25,6}})
set_stones("st-glass", {{21,7},{22,7},{26,7},{21,8},{26,8},{27,8},{27,9},{27,11},{21,9},{21,10}})

-- objects and items
set_item("it-hammer", 8,10)
set_item("it-hammer", 27,10)
set_item("it-yinyang", 1,1)
set_item("it-yinyang", 20,1)

-- trapdoors vertical
set_stone("st-door", 14,  1, {name="illdeja01a", type="v"})
set_stone("st-door", 14,  3, {name="illdeja02a", type="v"})
set_stone("st-door", 14,  5, {name="illdeja03a", type="v"})
set_stone("st-door", 14,  7, {name="illdeja04a", type="v"})
set_stone("st-door", 14,  9, {name="illdeja05a", type="v"})
set_stone("st-door", 14,  11, {name="illdeja06a", type="v"})
set_stone("st-door", 33,  1, {name="illdeja01b", type="v"})
set_stone("st-door", 33,  3, {name="illdeja02b", type="v"})
set_stone("st-door", 33,  5, {name="illdeja03b", type="v"})
set_stone("st-door", 33,  7, {name="illdeja04b", type="v"})
set_stone("st-door", 33,  9, {name="illdeja05b", type="v"})
set_stone("st-door", 33,  11, {name="illdeja06b", type="v"})

-- southbound laser stones
set_stone("st-laser", 2,5, {name="illdeja07a", on=TRUE, dir=SOUTH})
set_stone("st-laser", 8,5, {name="illdeja08a", on=TRUE, dir=SOUTH})
set_stone("st-laser", 13,0, {name="illdeja10a", on=TRUE, dir=SOUTH})
set_stone("st-laser", 21,5, {name="illdeja07b", on=TRUE, dir=SOUTH})
set_stone("st-laser", 27,5, {name="illdeja08b", on=TRUE, dir=SOUTH})
set_stone("st-laser", 32,0, {name="illdeja10b", on=TRUE, dir=SOUTH})

-- westbound laser stones
set_stone("st-laser", 8,3, {name="illdeja09a", on=TRUE, dir=WEST})
set_stone("st-laser", 27,3, {name="illdeja09b", on=TRUE, dir=WEST})

-- trigger platforms
set_item("it-trigger", 2,2, {action="openclose", target="illdeja01a"})
set_item("it-trigger", 4,2, {action="openclose", target="illdeja02a"})
set_item("it-trigger", 6,2, {action="openclose", target="illdeja03a"})
set_item("it-trigger", 10,2, {action="openclose", target="illdeja04a"})
set_item("it-trigger", 11,2, {action="openclose", target="illdeja05a"})
set_item("it-trigger", 12,2, {action="openclose", target="illdeja06a"})
set_item("it-trigger", 4,7, {action="onoff", target="illdeja07a"})
set_item("it-trigger", 5,8, {action="onoff", target="illdeja08a"})
set_item("it-trigger", 4,9, {action="onoff", target="illdeja09a"})
set_item("it-trigger", 1,6, {action="onoff", target="illdeja10a"})
set_item("it-trigger", 21,2, {action="openclose", target="illdeja01b"})
set_item("it-trigger", 23,2, {action="openclose", target="illdeja02b"})
set_item("it-trigger", 25,2, {action="openclose", target="illdeja03b"})
set_item("it-trigger", 29,2, {action="openclose", target="illdeja04b"})
set_item("it-trigger", 30,2, {action="openclose", target="illdeja05b"})
set_item("it-trigger", 31,2, {action="openclose", target="illdeja06b"})
set_item("it-trigger", 23,7, {action="onoff", target="illdeja07b"})
set_item("it-trigger", 24,8, {action="onoff", target="illdeja08b"})
set_item("it-trigger", 23,9, {action="onoff", target="illdeja09b"})
set_item("it-trigger", 20,6, {action="onoff", target="illdeja10b"})

-- documents
set_item("it-document", 11,3, {text=" when  you've  been  particular  places"})
set_item("it-document", 30,3, {text=" when  you've  been  particular  places"})
set_item("it-document", 11,4, {text=" that  you  know  you've  never  seen  before"})
set_item("it-document", 30,4, {text=" that  you  know  you've  never  seen  before"})
set_item("it-document", 11,5, {text=" can  you  be  sure  ?"})
set_item("it-document", 30,5, {text=" can  you  be  sure  ?"})
set_item("it-document", 11,7, {text=" feel  like  I've  been  here  before"})
set_item("it-document", 30,7, {text=" feel  like  I've  been  here  before"})
set_item("it-document", 11,9, {text=" quote:  Iron  Maiden   -   Deja-Vu"})
set_item("it-document", 30,9, {text=" quote:  Iron  Maiden   -   Deja-Vu"})
set_item("it-document", 11,11, {text=" for  illmind's  deja  vu  try  the  top  left  corner  ..."})
set_item("it-document", 30,11, {text=" for  illmind's  deja  vu  try  the  top  left  corner  ..."})

-- oxyd stones
oxyd(18,1)
oxyd(18,3)
oxyd(18,5)
oxyd(18,7)
oxyd(18,9)
oxyd(18,11)
oxyd(37,1)
oxyd(37,3)
oxyd(37,5)
oxyd(37,7)
oxyd(37,9)
oxyd(37,11)
oxyd_shuffle()

-- multiplayer function with no yinyang
function multiplayer_mode()
actor={"ac-blackball", {player=0, mouseforce=1}}
actor={"ac-whiteball", {player=1, mouseforce=1}}
end

-- blackball
set_actor("ac-blackball", 11.5, 1.5, {player=0})

-- whiteball
set_actor("ac-whiteball", 30.5, 1.5, {player=1})


















