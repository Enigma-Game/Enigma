-- OxydMagnumGold 17, a Clone for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 20
levelh = 13

create_world(levelw, levelh)

oxyd_default_flavor = "d"

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
  		if c=="X" then
            oxyd(i-1,line)
        elseif c=="#" then
            set_stone("st-grate1",i-1,line)
            set_floor("fl-metal",i-1,line)
        elseif c=="W" then
            set_stone("st-grate1",i-1,line)
            set_floor("fl-abyss",i-1,line)
        elseif c==" " then
            set_floor("fl-abyss",i-1,line)
        elseif c=="-" then
            set_floor("fl-metal",i-1,line)
		end
	end	
end

renderLine(00 , "                    ")
renderLine(01 , "  WWW    XX    WWW  ")
renderLine(02 , " --###-- -- --###-- ")
renderLine(03 , " ---###- -- -###--- ")
renderLine(04 , " ------- -- ------- ")
renderLine(05 , "    X    --    X    ")
renderLine(06 , "W------------------W")
renderLine(07 , "    X    --    X    ")
renderLine(08 , " ------- -- ------- ")
renderLine(09 , " ---###- -- -###--- ")
renderLine(10 , " --###-- -- --###-- ")
renderLine(11 , "  WWW    XX    WWW  ")
renderLine(12 , "                    ")

oxyd_shuffle()

set_item("it-extralife",9,3)
set_item("it-extralife",9,4)
set_item("it-extralife",9,8)
set_item("it-extralife",9,9)

set_item("it-extralife",10,3)
set_item("it-extralife",10,4)
set_item("it-extralife",10,8)
set_item("it-extralife",10,9)

set_item("it-springboard",9,2)
set_item("it-springboard",10,2)
set_item("it-springboard",9,10)
set_item("it-springboard",10,10)

-- Special
bb=set_actor("ac-blackball", 10.0,6.5, {player=0,name=bb})  
AddRubberBand(bb, enigma.GetStone(0, 6))
AddRubberBand(bb, enigma.GetStone(19, 6))
