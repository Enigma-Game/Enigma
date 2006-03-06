-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Blocks and Grates

levelw = 40
levelh = 13

create_world(levelw, levelh)

draw_border("st-greenbrown")
fill_floor("fl-hay")


function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone("st-greenbrown",i-1,line)
		elseif c == "z" then	
			set_actor("ac-blackball", i-.5,line+.5)
		elseif c == "o" then
			oxyd(i-1,line)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c == "*" then
			set_stone("st-brownie", i-1, line)
		elseif c == "+" then
			set_stone("st-wood", i-1, line)
		elseif c == "!" then
			abyss(i-1,line)
		elseif c == "u" then
			set_stone("st-bolder-n", i-1,line)
		elseif c == "d" then
			set_stone("st-bolder-s", i-1,line)
		end
	end
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"


renderLine(00,"#######################################")
renderLine(01,"#      ggggggg                     #o##")
renderLine(02,"#      g     g                     # ##")
renderLine(03,"#     gg g g gg         ############ ##")
renderLine(04,"#     +g+g+g+g+         !!!!!!!!!!!!* #")
renderLine(05,"#     g+ + + +g         ############u##")
renderLine(06,"#     + + + + +  z   +  ###############")
renderLine(07,"#     g+ + + +g         ############d##")
renderLine(08,"#     +g+g+g+g+         !!!!!!!!!!!!* #")
renderLine(09,"#     gg g g gg         ############ ##")
renderLine(10,"#      g     g                     # ##")
renderLine(11,"#      ggggggg                     #o##")
renderLine(12,"#######################################")

oxyd_shuffle()
















