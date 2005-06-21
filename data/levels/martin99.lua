--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw =  10*19+1
levelh = 13
create_world(levelw, levelh)
fill_floor("fl-metal")

function create_room( nr, goal)
	draw_border( "st-plain", nr*19, 0, 20, 13)
	set_item( "it-wormhole", nr*19+18, 1+goal, {range=1, strength=10, targetx=nr*19+19+11.5, targety=6.5})
	for i=1, 9 do
		item = enigma.GetItem(nr*19+18,1+i)
		if item == nil then
			set_item( "it-wormhole", nr*19+18, 1+i, {range=1, strength=10, targetx=nr*19+1.5, targety=6.5})
		end
	end
	if (not difficult) then
		fill_floor( "fl-water", nr*19+1, 1, goal, 11)
	else
		fill_floor( "fl-water", nr*19+1, 1, 3, 11)
	end
end

create_room( 0, 3)
create_room( 1, 1)
create_room( 2, 4)
create_room( 3, 1)
create_room( 4, 5)
create_room( 5, 9)
create_room( 6, 2)
create_room( 7, 6)
create_room( 8, 5)

draw_border( "st-plain", 9*19, 0, 20, 13)
oxyd( 9*19+2, 2)
oxyd( 9*19+2, 10)
oxyd( 9*19+17, 2)
oxyd( 9*19+17, 10)

Document( 12,9, "P")
Document( 13,9, "I")
set_actor("ac-blackball", 11.5,6.5)
