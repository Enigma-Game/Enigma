-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Blocks and Water

rooms_wide=3
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock1", i-1, line)
		elseif c =="%" then
			set_stone( "st-greenbrown", i-1, line)
		   set_floor("fl-water",i-1,line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "I" then
			oxyd( i-1, line)
		   set_floor("fl-water",i-1,line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-1,line+.5, {player=0})
		elseif c=="y" then
		   set_actor("ac-whiteball", i-1,line+.5, {player=1})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c == "G" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		   set_floor("fl-water",i-1,line)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="s" then
			set_floor("fl-sand",i-1,line)
	     end
	end	
end

renderLine(00,"##########################################################")
renderLine(01,"#                  #                  #   G~~~~ +~~~~~~ o#")
renderLine(02,"#  ++++ +g+g+ + +  g                  # ++~~~~~+ ~~~~~~  #")
renderLine(03,"#~~~~~~~~~~~~~~~~~~#  ##############  # + ~~~~~ +~~~~~~  #")
renderLine(04,"#~~sss~~%%%%~~sss~~#  ~~~~~ ~~ ~~~~~  # ++~~~~~+ ~~~~~~  #")
renderLine(05,"#~~~s~~~~~~~~~~s~~~#  ~~~~ ++++ ~~~~  # + G~~~~ +~~~~~~  #")
renderLine(06,"#~~~~~~~~II~~~~~~~~#  ~~~+#  z #+~~~  g ++~~~~~+ ~~~~~~  #")
renderLine(07,"#~~~s~~~~~~~~~~s~~~#  ~~~~ ++++ ~~~~  # + G~~~~ +~~~~~~  #")
renderLine(08,"#~~sss~~%%%%~~sss~~#  ~~~~~ ~~ ~~~~~  # ++~~~~~+ ~~~~~~  #")
renderLine(09,"#~~~~~~~~~~~~~~~~~~#  ##############  # + ~~~~~ +~~~~~~  #")
renderLine(10,"#  + + +g+g+ ++++  g                  # ++~~~~~+ ~~~~~~  #")
renderLine(11,"#                  #                  #   G~~~~ +~~~~~~ o#")
renderLine(12,"##########################################################")

oxyd_shuffle()