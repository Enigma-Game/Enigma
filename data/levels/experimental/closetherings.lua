-- Copyright (C) 2005 Raoul
-- Licensed under GPL v2.0

-- GENERAL --
levelw = 20
levelh = 39
mitte = 19


create_world(levelw, levelh)
enigma.ConserveLevel = TRUE


-- FLOOR --
fill_floor("fl-metal", 0,0,levelw,levelh)
set_floor("fl-abyss",3,mitte)
set_floor("fl-abyss",7,mitte)
set_floor("fl-abyss",12,mitte)
set_floor("fl-abyss",16,mitte)

-- STONES --
draw_border("st-glass",0,0,levelw,levelh)
draw_stones("st-glass", {1,mitte}, {1,0}, 2)
draw_stones("st-glass", {4,mitte}, {1,0}, 3)
draw_stones("st-glass", {8,mitte}, {1,0}, 4)
draw_stones("st-glass", {13,mitte}, {1,0}, 3)
draw_stones("st-glass", {17,mitte}, {1,0}, 2)

set_stone("st-puzzle", 6,6, {connections=PUZ_1010, oxyd="1"})
set_stone("st-puzzle", 6,5, {connections=PUZ_0110, oxyd="1"})
set_stone("st-puzzle", 7,5, {connections=PUZ_0101, oxyd="1"})
set_stone("st-puzzle", 8,5, {connections=PUZ_0011, oxyd="1"})
set_stone("st-puzzle", 8,6, {connections=PUZ_1010, oxyd="1"})
set_stone("st-puzzle", 8,7, {connections=PUZ_1001, oxyd="1"})
set_stone("st-puzzle", 7,7, {connections=PUZ_0101, oxyd="1"})

set_stone("st-puzzle", 10,10, {connections=PUZ_1010, oxyd="1"})
set_stone("st-puzzle", 10,11, {connections=PUZ_1001, oxyd="1"})
set_stone("st-puzzle", 9,11, {connections=PUZ_0101, oxyd="1"})
set_stone("st-puzzle", 8,11, {connections=PUZ_1100, oxyd="1"})
set_stone("st-puzzle", 8,10, {connections=PUZ_1010, oxyd="1"})
set_stone("st-puzzle", 8,9, {connections=PUZ_0110, oxyd="1"})
set_stone("st-puzzle", 9,9, {connections=PUZ_0101, oxyd="1"})

set_stone("st-puzzle", 5,13, {connections=PUZ_0101, oxyd="1"})
set_stone("st-puzzle", 6,13, {connections=PUZ_0011, oxyd="1"})
set_stone("st-puzzle", 6,14, {connections=PUZ_1010, oxyd="1"})
set_stone("st-puzzle", 6,15, {connections=PUZ_1001, oxyd="1"})
set_stone("st-puzzle", 5,15, {connections=PUZ_0101, oxyd="1"})
set_stone("st-puzzle", 4,15, {connections=PUZ_1100, oxyd="1"})
set_stone("st-puzzle", 4,14, {connections=PUZ_1010, oxyd="1"})

set_stone("st-puzzle", 15,15, {connections=PUZ_0101, oxyd="1"})
set_stone("st-puzzle", 14,15, {connections=PUZ_1100, oxyd="1"})
set_stone("st-puzzle", 14,14, {connections=PUZ_1010, oxyd="1"})
set_stone("st-puzzle", 14,13, {connections=PUZ_0110, oxyd="1"})
set_stone("st-puzzle", 15,13, {connections=PUZ_0101, oxyd="1"})
set_stone("st-puzzle", 16,13, {connections=PUZ_0011, oxyd="1"})
set_stone("st-puzzle", 16,14, {connections=PUZ_1010, oxyd="1"})

--
set_stone("st-puzzle", 5,25, {connections=PUZ_1010})
set_stone("st-puzzle", 5,24, {connections=PUZ_0110})
set_stone("st-puzzle", 6,24, {connections=PUZ_0101})
set_stone("st-puzzle", 7,24, {connections=PUZ_0011})
set_stone("st-puzzle", 7,25, {connections=PUZ_1010})
set_stone("st-puzzle", 7,26, {connections=PUZ_1001})
set_stone("st-puzzle", 6,26, {connections=PUZ_0101})

set_stone("st-puzzle", 14,23, {connections=PUZ_0101})
set_stone("st-puzzle", 15,23, {connections=PUZ_0011})
set_stone("st-puzzle", 15,24, {connections=PUZ_1010})
set_stone("st-puzzle", 15,25, {connections=PUZ_1001})
set_stone("st-puzzle", 14,25, {connections=PUZ_0101})
set_stone("st-puzzle", 13,25, {connections=PUZ_1100})
set_stone("st-puzzle", 13,24, {connections=PUZ_1010})

set_stone("st-puzzle", 10,30, {connections=PUZ_1010})
set_stone("st-puzzle", 10,31, {connections=PUZ_1001})
set_stone("st-puzzle", 9,31, {connections=PUZ_0101})
set_stone("st-puzzle", 8,31, {connections=PUZ_1100})
set_stone("st-puzzle", 8,30, {connections=PUZ_1010})
set_stone("st-puzzle", 8,29, {connections=PUZ_0110})
set_stone("st-puzzle", 9,29, {connections=PUZ_0101})

set_stone("st-puzzle", 14,34, {connections=PUZ_0101})
set_stone("st-puzzle", 13,34, {connections=PUZ_1100})
set_stone("st-puzzle", 13,33, {connections=PUZ_1010})
set_stone("st-puzzle", 13,32, {connections=PUZ_0110})
set_stone("st-puzzle", 14,32, {connections=PUZ_0101})
set_stone("st-puzzle", 15,32, {connections=PUZ_0011})
set_stone("st-puzzle", 15,33, {connections=PUZ_1010})

----
set_stone("st-puzzle", 9,23, {connections=PUZ_1100, oxyd="1"})
set_stone("st-puzzle", 11,26, {connections=PUZ_0110, oxyd="1"})
set_stone("st-puzzle", 9,34, {connections=PUZ_1001, oxyd="1"})
set_stone("st-puzzle", 4,31, {connections=PUZ_0011, oxyd="1"})

set_stone("st-puzzle", 8,14, {connections=PUZ_1100})
set_stone("st-puzzle", 9,8, {connections=PUZ_0110})
set_stone("st-puzzle", 4,4, {connections=PUZ_1001})
set_stone("st-puzzle", 3,15, {connections=PUZ_0011})

-- ITEMS --
set_item("it-trigger", 5, 8, {action="callback", target="d1"})
set_item("it-trigger", 12, 4, {action="callback", target="d2"})
set_item("it-trigger", 13, 6, {action="callback", target="d3"})
set_item("it-trigger", 11, 14, {action="callback", target="d4"})

set_item("it-trigger", 13, 29, {action="callback", target="d1"})
set_item("it-trigger", 10, 23, {action="callback", target="d2"})
set_item("it-trigger", 15, 27, {action="callback", target="d3"})
set_item("it-trigger", 4, 29, {action="callback", target="d4"})

set_item( "it-magicwand", 8, 14)

-- ACTORS --
local ac1=set_actor("ac-blackball", 4.5,3.25, {player=0})



-- OXYD --
oxyd(7,6)
oxyd(9,10)
oxyd(5,14)
oxyd(15,14)

oxyd(6,25)
oxyd(14,24)
oxyd(9,30)
oxyd(14,33)


oxyd_default_flavor = "d"
oxyd_shuffle()
display.SetFollowMode(display.FOLLOW_SMOOTH)


-------
so1=FALSE
op1=FALSE

function d1()
  if so1==FALSE then
    if op1==FALSE then
      --SendMessage("door1", "open")
      set_floor("fl-sahara",3,mitte)
      op1=TRUE
    elseif op1==TRUE then
      --SendMessage("door1", "close")
      set_floor("fl-abyss",3,mitte)
      op1=FALSE
    end
  elseif so1==TRUE then
    --SendMessage("door1", "close")
      set_floor("fl-abyss",3,mitte)
    so1=FALSE
  end
end

so2=FALSE
op2=FALSE

function d2()
  if so2==FALSE then
    if op2==FALSE then
      --SendMessage("door2", "open")
      set_floor("fl-sahara",7,mitte)
      op2=TRUE
    elseif op2==TRUE then
      --SendMessage("door2", "close")
      set_floor("fl-abyss",7,mitte)
      op2=FALSE
    end
  elseif so2==TRUE then
      --SendMessage("door2", "close")
      set_floor("fl-abyss",7,mitte)
      so2=FALSE
  end
end

so3=FALSE
op3=FALSE

function d3()
  if so3==FALSE then
    if op3==FALSE then
      --SendMessage("door3", "open")
      set_floor("fl-sahara",12,mitte)
      op3=TRUE
    elseif op3==TRUE then
      --SendMessage("door3", "close")
      set_floor("fl-abyss",12,mitte)
      op3=FALSE
    end
  elseif so3==TRUE then
      --SendMessage("door3", "close")
      set_floor("fl-abyss",12,mitte)
      so3=FALSE
  end
end

so4=FALSE
op4=FALSE

function d4()
  if so4==FALSE then
    if op4==FALSE then
      --SendMessage("door4", "open")
      set_floor("fl-sahara",16,mitte)
      op4=TRUE
    elseif op4==TRUE then
      --SendMessage("door4", "close")
      set_floor("fl-abyss",16,mitte)
      op4=FALSE
    end
  elseif so4==TRUE then
      --SendMessage("door4", "close")
      set_floor("fl-abyss",16,mitte)
      so4=FALSE
  end
end




display.SetFollowMode(display.FOLLOW_SCROLLING)














