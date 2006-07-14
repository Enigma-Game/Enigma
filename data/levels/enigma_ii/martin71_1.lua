--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

enigma.ConserveLevel=FALSE                  -- \dh\

hitcounter = 0
function turnbolder( dir)
   -- ONLY UPON TRIGGER STEP-ON
   hitcounter = hitcounter +1
   if (mod(hitcounter,2)==0) then
      return 0
   end
   -- GET DIRECTION
   local bolder_dir=enigma.GetAttrib(enigma.GetNamedObject("bolder1"), "direction")
   -- SET NEW DIRECTION
   if dir == "left" then
   	bolder_dir = bolder_dir -1
   	while (bolder_dir<0) do
   	   bolder_dir = bolder_dir +4
   	end
   else
   	bolder_dir = bolder_dir +1
	while (bolder_dir>3) do
	   bolder_dir = bolder_dir -4
	end
   end

   SendMessage("bolder1", "direction", bolder_dir)

end

function turnbolder1()
	turnbolder( "left")
end

function turnbolder2()
	turnbolder( "right")
end

create_world( 5*19+1, 5*12+1)

draw_border("st-death")
fill_floor( "fl-water")

fill_floor( "fl-sand", 40, 25, 16, 11)
fill_floor( "fl-water", 46, 29, 4, 3)

fill_floor( "fl-sand", 19, 28, 6, 5)
fill_floor( "fl-sand", 25, 29, 15, 3)
fill_floor( "fl-sand", 9, 25, 10, 11)

fill_floor( "fl-sand", 71, 28, 6, 5)
fill_floor( "fl-sand", 56, 29, 15, 3)
fill_floor( "fl-sand", 77, 25, 10, 11)

fill_floor( "fl-sand", 46, 12, 4, 5)
fill_floor( "fl-sand", 47, 17, 2, 8)
fill_floor( "fl-sand", 41, 5, 14, 7)

fill_floor( "fl-sand", 46, 44, 4, 5)
fill_floor( "fl-sand", 47, 36, 2, 8)
fill_floor( "fl-sand", 41, 49, 14, 7)

set_item( "it-trigger", 48, 30, {action="callback", target="turnbolder2"})
set_item( "it-trigger", 47, 30, {action="callback", target="turnbolder1"})
set_item("it-magicwand", 43, 27)
local st = set_stone("st-bolder", 13, 30, {name="bolder1", direction=2})
local ac = set_actor("ac-blackball", 11.5, 30.5)

oxyd( 10,25)
oxyd( 10,35)
oxyd( 85,25)
oxyd( 85,35)
oxyd( 41, 6)
oxyd( 54, 6)
oxyd( 41,54)
oxyd( 54,54)
oxyd_shuffle()

AddRubberBand( ac, st, 5, 2)
















