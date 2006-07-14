-- A meditation level for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above 

-- GENERAL --
Require("levels/lib/libterrain.lua")
levelw = 20
levelh = 13

create_world(levelw, levelh)

display.SetFollowMode(display.FOLLOW_SCROLLING)

fill_floor("fl-water",0,0,levelw,levelh)

set_floor("fl-sand",9,6)
set_floor("fl-sand",10,6)

if difficult then
 x1=random(9)
 x2=random(9)
 
 draw_full(0,x1,3,3,0)
 draw_full(17,x2,3,3,0)

 set_floor("fl-sand",1,x1+1)
 set_floor("fl-sand",3,x1+1)
 set_floor("fl-sand",18,x2+1)
 set_floor("fl-sand",16,x2+1)

 set_item("it-hollow", 1, x1+1);
 set_item("it-hollow", 18, x2+1);

else

 draw_full(0,9,3,3,0)
 draw_full(17,1,3,3,0)

 set_floor("fl-sand",1,10)
 set_floor("fl-sand",3,10)
 set_floor("fl-sand",18,2)
 set_floor("fl-sand",16,2)
 
 set_item("it-hollow", 1, 10);
 set_item("it-hollow", 18, 2);

end

-- Grosses Tal
draw_full(4,1,12,11,0)

-- Special
set_actor("ac-whiteball-small", 4.5,1.5, {player=0})
set_actor("ac-whiteball-small", 15.5,11.5, {player=0})

