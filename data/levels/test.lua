levelw = 39
levelh = 37

-- create a new level and set some general options
create_world(levelw, levelh)
oxyd(8,3)
oxyd(11,1)
oxyd(3,1)
oxyd(12,20)
oxyd_shuffle()

fill_floor("fl-samba", 0,0, levelw,levelh)
draw_border("st-brownie")

set_stone("st-bolder-e", 1, 3)
set_stone("st-bolder-e", 2, 3)
set_stone("st-blocker", 3, 3, {name="blocker1"})

set_stone("st-switch", 1,5, {action="callback", target="bl1"})

function bl1()
        b1=enigma.GetNamedObject("blocker1")
        SendMessage(b1, "trigger")
end

set_stone("st-bolder-w",18, 3)
set_stone("st-bolder-w",17, 3)
set_stone("st-blocker",10, 3)
set_item("it-blocker", 8, 4, {name="blocker2"})

set_stone("st-switch", 8, 6, {action="callback", target="bl2"})

function bl2()
        b1=enigma.GetNamedObject("blocker2")
        SendMessage(b1, "signal")
end

set_stone("st-bolder-e", 1, 9)
set_stone("st-rock4", 2, 9)
set_stone("st-stoneimpulse", 1, 10)
set_stone("st-blocker", 2, 8)

set_stone ("st-mail-n", 4, 20)
set_stone ("st-mail-e", 5, 21)
set_stone ("st-mail-s", 6, 22)
set_stone ("st-mail-w", 7, 23)

set_item ("it-pipe-h", 10, 21)
set_item ("it-pipe-h", 11, 21)
set_item ("it-pipe-v", 12, 21)
set_item ("it-pipe-ne", 13, 21)
set_item ("it-pipe-es", 14, 21)
set_item ("it-pipe-sw", 15, 21)
-- set_item("it-spring1", 2,5);
-- set_item("it-spring2", 2,6);

set_stone("st-switch", 1,15, {action="onoff", target="laser"})
set_attrib(laser(1,14, FALSE, EAST), "name", "laser")
--fill_floor("fl-abyss", 3,4, 2 ,1)

--set_item("it-blackbomb",9,4)
--set_item("it-blackbomb",10,4)
--set_item("it-blackbomb",11,4)
--set_item("it-hammer",12,4)
--set_stone("st-magic", 1,1)
--dynamite(9,5)

--set_item("it-hollow",10,5)
set_item("it-seed", 5,14)
--set_item("it-seed", 6,7)
--set_item("it-seed", 6,8)

--set_stone("st-grate1", 6,10)

--for i=1,10 do
--    set_item("it-pipe", 6+i,7, {type=i})
--end

--set_attrib (mirrorp ( 5, 4, 1, 0,1), "name", "mirror01")

--set_stone("st-fourswitch", 7,5, {action="callback", target="ms1"})

-- function ms1()
--        mir1=enigma.GetNamedObject("mirror01")
--        SendMessage(mir1, "trigger")
-- end

-- set_item("it-wormhole", 20,14, {targetx="25", targety="20"})

set_stone("st-scissors", 17,10)
set_stone("st-rubberband", 10,10)
set_stone("st-rubberband", 12,12)
set_stone("st-rubberband", 14,14)
set_item("it-magicwand", 10,9)

set_item("it-flagwhite", 10,8)

-- set_item("it-puller", 10,21, {orientation=NORTH})
-- set_item("it-puller", 11,21, {orientation=EAST})
-- set_item("it-puller", 12,21, {orientation=SOUTH})
-- set_item("it-puller", 13,21, {orientation=WEST})

local ac=set_actor("ac-blackball", 3.5,16.5)
local st=set_stone("st-brownie", 10,6)
--AddRubberBand(ac, st)

set_stone("st-rotator-left", 20,20)
set_stone("st-rotator-right", 20, 22)

set_actor("ac-top", 10,10)

set_item("it-glasses", 5, 27)
set_item("it-glasses", 5, 28)
set_stone("st-marble_hole", 4, 29)
set_stone("st-rock1_hole", 5, 29)
set_stone("st-rock2_hole", 6, 29)
set_stone("st-rock3_hole", 4, 30)
set_item("it-glasses", 5, 30)
set_stone("st-greenbrown_hole", 6, 30)
set_stone("st-bluegray_hole", 4, 31)
set_stone("st-grate1", 5, 31)
set_stone("st-grate2", 6, 31)

set_stone("st-death_invisible", 7, 29)
set_stone("st-death_invisible", 7, 30)
set_stone("st-death_invisible", 7, 31)


set_item("it-seed", 10, 30)
set_item("it-seed_wood", 11, 30)
set_item("it-seed_nowood", 12, 30)
set_item("it-seed_volcano", 13, 30)


set_stone("st-switch", 19, 24, {action="trigger", target="burnable1"})
set_item("it-brush", 18, 21)
fill_items( "it-burnable", 20, 25, 10, 10)
set_item( "it-extinguisher", 18, 25)
set_item("it-burnable", 19, 27, {name="burnable1"})
set_item("it-dynamite", 22, 27)
set_item("it-blackbomb", 25, 27)
set_item("it-blackbomb", 28, 27)
document(22,32,"Burn me")
set_item("it-extinguisher", 24, 32)

display.SetFollowMode(display.FOLLOW_SCROLLING)
