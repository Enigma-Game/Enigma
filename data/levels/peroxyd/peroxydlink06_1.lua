-- Per.Oxyd MP #6, a level for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above 
-- Per.Oxyd Clone

-- GENERAL --
levelw = 39
levelh = 25

Require("levels/lib/libpuzzle.lua")

create_world(levelw, levelh)

fill_floor("fl-abyss")
fill_floor("fl-concrete",0,2,levelw,levelh-4)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
  		if c=="x" then
			oxyd(i-1,line)
                elseif c=="#" then
			set_stone("st-rock2",i-1,line)
                   
                elseif c==" " then
                   set_floor("fl-concrete",i-1,line)

                elseif c=="s" then
                   set_stone("st-black1",i-1,line)
                elseif c=="w" then
                   set_stone("st-white1",i-1,line)

		end
	end	
end

renderLine(00 , "---------------------------------------")
renderLine(01 , "---------------------------------------")
renderLine(02 , "#######################################")
renderLine(03 , "#sss               #               www#")
renderLine(04 , "#sxs               #               wxw#")
renderLine(05 , "#sss               #               www#")
renderLine(06 , "#www               #               sss#")
renderLine(07 , "#wxw               #               sxs#")
renderLine(08 , "#www         # ######### #         sss#")
renderLine(09 , "#sss                               www#")
renderLine(10 , "#sxs         #           #         wxw#")
renderLine(11 , "#sss         #     #     #         www#")
renderLine(12 , "##############  #######  ##############")
renderLine(13 , "#www         #     #     #         sss#")
renderLine(14 , "#wxw         #           #         sxs#")
renderLine(15 , "#www                               sss#")
renderLine(16 , "#sss         # ######### #         www#")
renderLine(17 , "#sxs               #               wxw#")
renderLine(18 , "#sss               #               www#")
renderLine(19 , "#www               #               sss#")
renderLine(20 , "#wxw               #               sxs#")
renderLine(21 , "#www               #               sss#")
renderLine(22 , "#######################################")
renderLine(23 , "---------------------------------------")
renderLine(24 , "---------------------------------------")

oxyd_default_flavor = "a"
oxyd_shuffle()

-- ACTORS --
set_actor("ac-blackball", 11.5,5.5, {player=0})
set_actor("ac-whiteball", 11.5,15.5, {player=1})
set_item("it-yinyang", 11,5)
set_item("it-yinyang", 11,15)

puzzle({{1,1,1,1,1,1,1,1,1,1},{1,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,1},{1,1,1,1,1,1,1,1,1,1}},14,10,"blue","no")



