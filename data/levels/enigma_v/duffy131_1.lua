-- Ocean Cruising, a level for Enigma
-- Copyright (C) 2005 Jacob Scott
-- Licensed under GPL v2.0 or above 

rooms_wide=2
rooms_high=3

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)

enigma.WaterSinkSpeed=40

fill_floor("fl-hay", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-death", i-1, line)
			set_floor("fl-water",i-1,line)
		elseif c == "o" then
			oxyd( i-1, line)
			set_floor("fl-water",i-1,line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c == "S" then
		   set_floor("fl-swamp",i-1,line)
		elseif c == "T" then
		   set_floor("fl-swamp",i-1,line)
		   set_item("it-extralife",i-1,line)
		elseif c == "W" then
		   set_floor("fl-wood",i-1,line)
		elseif c == "w" then
		   set_floor("fl-wood",i-1,line)
		   set_item("it-extralife",i-1,line)
		elseif c == "M" then
		   set_floor("fl-metal",i-1,line)
		elseif c == "m" then
		   set_floor("fl-metal",i-1,line)
		   set_item("it-extralife",i-1,line)
		elseif c == "L" then
		   set_floor("fl-leaves",i-1,line)
		elseif c == "s" then
		   set_floor("fl-sand",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-1,line+.5, {player=0})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="F" then
		   set_item("it-flagblack",i-1,line)
	     end
	end	
end

renderLine(00,"#######################################")
renderLine(01,"#           ~~~~~~~~~~~~~~~~~~~~~~~~~~#")
renderLine(02,"#    F   ~~~o~~SS~~~~~~~~~~~~~SS~~o~~~#")
renderLine(03,"#   z  ~~~~~~~~SSS~~~~~~~~~SSSSSS~~~~~#")
renderLine(04,"#     ~~~~~~~~~~~~~~~~~~~~~SSSSS~~~~~~#")
renderLine(05,"#~~~~~~~~~~~~~~~~~~~~~~~~~~~~SSS~~~~~~#")
renderLine(06,"#~~~~~~~~~~~~~LL~~~~~~WWW~~~~~~~~MMM~~#")
renderLine(07,"#~~~SSS~~~~~~LLLL~~WWWWWW~~~~~~~MMmM~~#")
renderLine(08,"#~~~SSSSSS~~LLLLL~~WWWWW~~~~~~~MMMMM~~#")
renderLine(09,"#~~~SSSSSS~~LLLL~~~WWWWWW~~~~~~~MMMM~~#")
renderLine(10,"#~~~~SSSSS~~~~LL~~~~~~~~~~~~~~~~~~~~~~#")
renderLine(11,"#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#")
renderLine(12,"#~~~~~~WWW~~~~~~~~~~~~~~~~~~~~      ~~#")
renderLine(13,"#~~~~~~WWWWW~~~~~~~~~~~~~~~~      ~~~~#")
renderLine(14,"#~~~~~WWWWWWW~~~~~~~~~~~~~~~      ~~~~#")
renderLine(15,"#~~~~o~~WWWWW~~~~~~~~~~~~~~       ~~~~#")
renderLine(16,"#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#")
renderLine(17,"#~~~~~~~~~~~~~~~~~~~~o~~~~~~~~~~~~~~~~#")
renderLine(18,"#~~~~~~  ~~~~~~~~~~~~~~~~~~~~WWWWW~~~~#")
renderLine(19,"#~~~~~~   ~~~~~~~~~~~~~~~~~~~~WWWWW~~~#")
renderLine(20,"#~~~~~~   ~~~~~~~~~LLL~~~~~~~~WwWW~~~~#")
renderLine(21,"#~~~~~~~   ~~~~~~~~~LL~~~~~~~WWWWW~~~~#")
renderLine(22,"#~~~~~~~~~~~~MMM~~~~~~~~~~~~~WWWW~~~~~#")
renderLine(23,"#~~sss~~~~~~~MMMM~~~~~~~~~~~~~~~~~~~~~#")
renderLine(24,"#~~sss~~~~~~~MMMMM~~~~~~sssss~~~~~~~~~#")
renderLine(25,"#~~ss~~~~~~~~~~MMMM~~~sssssss~~~~~~~~~#")
renderLine(26,"#~~s~~~~SSSS~~~MMMM~~~sssss~~~~~~~~~~~#")
renderLine(27,"#~~~~~~SSSSS~~~~~~~~~~~sss~~~~~~~~~~~~#")
renderLine(28,"#~~~~~~SSTSS~~~~~~~~~~~~~~~~~~~~~~~~~~#")
renderLine(29,"#~~~~~~~SSSSS~~~~~~~~~~~~~~~~SSSSS~~~~#")
renderLine(30,"#~~~~~~~~~SSS~~~~~~~~~~~~~~~~~SSSSS~~~#")
renderLine(31,"#~~~~~~~~~~~~~~~~~WW~~~~~~~~~SSSSSS~~~#")
renderLine(32,"#~~~     ~~~~~WWWWWWW~~~~~~~~SSSSS~~~~#")
renderLine(33,"#~~~      ~~WWWWWWWWWo~~~~~~~~~SSS~~~~#")
renderLine(34,"#~~~~    ~~~~WWWWW~~~~~~~~~~~~~~~~o~~~#")
renderLine(35,"#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#")
renderLine(36,"#######################################")

oxyd_shuffle()

display.SetFollowMode(display.FOLLOW_SCROLLING)






















