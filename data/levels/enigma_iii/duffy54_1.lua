-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Keepaway

levelw=58
levelh=37

create_world( levelw, levelh)

fill_floor("fl-rock", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-invisible", i-1, line)
			abyss(i-1,line)
		elseif c =="&" then
			set_stone( "st-invisible", i-1, line)
			abyss(i-1,line)
		elseif c =="X" then
			set_stone( "st-death", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
--			fill_floor("fl-water", i-1,line, 1,1)
                elseif c == "~" then
--			abyss(i-1,line)
			fill_floor("fl-water", i-1,line, 1,1)
                elseif c == " " then
			abyss(i-1,line)
		elseif c=="z" then
                    set_actor("ac-blackball", i-1,line+.5)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="R" then
		   set_actor("ac-rotor", i-.5,line+.5, {player=1, mouseforce=0, range=100, force=30})
			abyss(i-1,line)
		elseif c=="Q" then
		   set_item("it-ring",i-1,line)
		elseif c=="A" then
		   set_floor("fl-gravel",i-1,line)
		elseif c=="x" then
		   set_floor("fl-rock",i-1,line)
	     end
	end	
     end

renderLine(00,"############################oo############################")
renderLine(01,"#                          !xx!                          #")
renderLine(02,"# R                        !QQ!                        R #")
renderLine(03,"#                          !QQ!                          #")
renderLine(04,"#                          !xx!                          #")
renderLine(05,"#                          !xx!                          #")
renderLine(06,"#                          !xx!                          #")
renderLine(07,"#                          !xx!                          #")
renderLine(08,"#                          !xx!                          #")
renderLine(09,"#                          !xx!                          #")
renderLine(10,"#                          !xx!                          #")
renderLine(11,"#                          !xx!                          #")
renderLine(12,"#                  !!!!!!!!!xx!!!!!!!!!                  #")
renderLine(13,"#                  !xxxxxxxxxxxxxxxxxx!                  #")
renderLine(14,"#                  !xxxxxxxxxxxxxxxxxx!                  #")
renderLine(15,"#                  !xxxxxxxxxxxxxxxxxx!                  #")
renderLine(16,"#!!!!!!!!!!!!!!!!!!!xxxxxxxxxxxxxxxxxx!!!!!!!!!!!!!!!!!!!#")
renderLine(17,"oxQQxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxQQxo")
renderLine(18,"oxQQxxxxxxxxxxxxxxxxxxxxxxxxxzxxxxxxxxxxxxxxxxxxxxxxxxQQxo") 
renderLine(19,"oxQQxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxQQxo")
renderLine(20,"#!!!!!!!!!!!!!!!!!!!xxxxxxxxxxxxxxxxxx!!!!!!!!!!!!!!!!!!!#")
renderLine(21,"#                  !xxxxxxxxxxxxxxxxxx!                  #")
renderLine(22,"#                  !xxxxxxxxxxxxxxxxxx!                  #")
renderLine(23,"#                  !xxxxxxxxxxxxxxxxxx!                  #")
renderLine(24,"#                  !!!!!!!!!xx!!!!!!!!!                  #")
renderLine(25,"#                          !xx!                          #")
renderLine(26,"#                          !xx!                          #")
renderLine(27,"#                          !xx!                          #")
renderLine(28,"#                          !xx!                          #")
renderLine(29,"#                          !xx!                          #")
renderLine(30,"#                          !xx!                          #")
renderLine(31,"#                          !xx!                          #")
renderLine(32,"#                          !xx!                          #")
renderLine(33,"#                          !QQ!                          #")
renderLine(34,"# R                        !QQ!                        R #")
renderLine(35,"#                          !xx!                          #")
renderLine(36,"############################oo############################")

oxyd_shuffle()
















