-- NAME, a level for Enigma
-- Copyright (C) YEAR AUTHOR
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 20
levelh = 13

create_world(levelw, levelh)

fill_floor("fl-abyss")

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
  		if c=="x" then
                        oxyd_default_flavor = "a"
			oxyd(i-1,line)
                elseif c=="#" then
                   set_stone("st-plain",i-1,line)
                elseif c=="E" then
                   set_stone("st-rubberband",i-1,line)

                elseif c==" " then
                   set_floor("fl-water",i-1,line)
                elseif c=="h" then
                   set_floor("fl-samba",i-1,line)

		end
	end	
end

renderLine(00 , "--------------------")
renderLine(01 , "######x#######x#####")
renderLine(02 , "#    hhh     hhh   #")
renderLine(03 , "#   hhhh hhh hhhh  #")
renderLine(04 , "#  hEh hEh hEh hEh #")
renderLine(05 , "#hhhhhhhhhhhhhhhhhh#")
renderLine(06 , "xhhh hhh hhh hhh hhx")
renderLine(07 , "#hhhhhhhhhhhhhhhhhh#")
renderLine(08 , "# hEh hEh hEh hEh  #")
renderLine(09 , "#  hhhh hhh hhhh   #")
renderLine(10 , "#   hhh     hhh    #")
renderLine(11 , "#####x#######x######")
renderLine(12 , "--------------------")

oxyd_shuffle()

-- Special
set_item("it-document", 11, 5, {text="Beachten Sie die Wasserlöcher!"})
bb=set_actor("ac-blackball", 8.5,5.5, {player=0, name="bb"})  
AddRubberBand(bb, enigma.GetStone(8,4),60)
