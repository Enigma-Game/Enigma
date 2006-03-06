-- Killer Hills, a level for Enigma
-- Copyright (C) 2005 Lukas Schueller
-- Licensed under GPL v2.0 or above
---------------------------------------------------
-- Function to create Hills with 4 different sizes
-- You may take this function for your level if you need it
-- The first parameter is the size of the hill (1-4), the 2nd and 3rd are x and y position
function create_mountain(size, x,y)
-- Very small hill
  if(size == 1) then
    set_item("it-tinyhill", x,y)
  end
-- Small hill
  if(size == 2) then
    set_item("it-hill", x,y)
  end
-- hill on 4 fields
  if(size == 3) then
    set_floor("fl-gradient", x,y, {type=12})
    set_floor("fl-gradient", x+1,y, {type=10})
    set_floor("fl-gradient", x,y+1, {type=11})
    set_floor("fl-gradient", x+1,y+1, {type=9})
  end
-- hill on 16 fields
  if(size == 4) then
    set_floor("fl-gradient", x,y, {type=12})
    set_floor("fl-gradient", x+3,y, {type=10})
    set_floor("fl-gradient", x,y+3, {type=11})
    set_floor("fl-gradient", x+3,y+3, {type=9})
    set_floor("fl-gradient", x+1,y, {type=2})
    set_floor("fl-gradient", x+2,y, {type=2})
    set_floor("fl-gradient", x,y+1, {type=4})
    set_floor("fl-gradient", x,y+2, {type=4})
    set_floor("fl-gradient", x+1,y+3, {type=1})
    set_floor("fl-gradient", x+2,y+3, {type=1})
    set_floor("fl-gradient", x+3,y+1, {type=3})
    set_floor("fl-gradient", x+3,y+2, {type=3})
    set_floor("fl-gradient", x+1,y+1, {type=12})
    set_floor("fl-gradient", x+2,y+1, {type=10})
    set_floor("fl-gradient", x+1,y+2, {type=11})
    set_floor("fl-gradient", x+2,y+2, {type=9})
  end
end
----------------------------------------------------------
-- Environment
CreateWorld(20, 13) 
draw_border("st-yellow")
fill_floor("fl-metal", 0,0, 20,13)
create_mountain(1, 1,1)
create_mountain(1, 8,1)
create_mountain(1, 11,11)
create_mountain(1, 18,11)
create_mountain(1, 9,6)
create_mountain(1, 10,6)
create_mountain(1, 2,5)
create_mountain(1, 17,7)
create_mountain(2, 3,2)
create_mountain(2, 16,2)
create_mountain(2, 3,10)
create_mountain(2, 16,10)
create_mountain(2, 9,3)
create_mountain(2, 9,9)
create_mountain(2, 10,3)
create_mountain(2, 10,9)
create_mountain(2, 8,9)
create_mountain(2, 11,3)
create_mountain(3, 5,2)
create_mountain(3, 13,1)
create_mountain(3, 17,1)
create_mountain(3, 8,4)
create_mountain(3, 10,7)
create_mountain(3, 13,9)
create_mountain(3, 1,10)
create_mountain(3, 5,10)
create_mountain(4, 4,5)
create_mountain(4, 12,4)
oxyd(0,1)
oxyd(1,0)
oxyd(0,11)
oxyd(1,12)
oxyd(19,1)
oxyd(18,0)
oxyd(18,12)
oxyd(19,11)
oxyd_shuffle()
set_actor("ac-blackball", 10,6.5, {player=0})
-- Killer Balls
set_actor("ac-killerball", 10,3.5, {mouseforce=2, controllers=1})
set_actor("ac-killerball", 10,9.5, {mouseforce=2, controllers=1})
set_actor("ac-killerball", 6.5,6.5, {mouseforce=2, controllers=1})
set_actor("ac-killerball", 13.5,6.5, {mouseforce=2, controllers=1})






















