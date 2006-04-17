-- Per.Oxyd MP #13, a level for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above 

-- GENERAL --
Require("levels/lib/libpuzzle.lua")

levelw = 58
levelh = 13

create_world(levelw, levelh)

fill_floor("fl-samba",0,0,levelw, levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
  		if c=="O" then
                   oxyd(i-1,line)
                elseif c=="-" then
                   set_floor("fl-abyss",i-1,line)
                elseif c=="#" then
                   set_stone("st-marble",i-1,line)
                elseif c=="X" then
                   set_stone("st-stoneimpulse",i-1,line)
		end
	end	
end
--               0123456789012345678901234567890123456789012345678901234567
renderLine(00 , "----------------------------------------------------------")
renderLine(01 , "##########################################################")
renderLine(02 , "#    ##   #           #        # ##                      #")
renderLine(03 , "#     #               #  ###                             #")
renderLine(04 , "#                          #                             #")
renderLine(05 , "#                     ###  #                            O#")
renderLine(06 , "##### #### ####X#O#######  #O### #########################")
renderLine(07 , "#                  #       #                            O#")
renderLine(08 , "#                  #                                     #")
renderLine(09 , "#     #        # #         #                             #")
renderLine(10 , "#     #        # # #       #  ## #                       #")
renderLine(11 , "##########################################################")
renderLine(12 , "----------------------------------------------------------")

oxyd_shuffle()

--ACTORS
set_actor("ac-blackball", 3,3, {player=0})  
set_actor("ac-whiteball", 3,8, {player=1})  

set_item("it-yinyang",3,3)
set_item("it-yinyang",3,8)

--THE PUZZLES:
must_shuffle=0
art=""

puzzle({{1},{1},{1},{1},{1}},5,4)

puzzle({{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1},{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},{1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},6,4)

puzzle({{1,1,1,1,1}},14,8)

puzzle({{1,1,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1}},23,4)

puzzle({{0,0,1,0,0},{1,1,1,1,1},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{1,1,1,1,1},{0,0,1,0,0}},30,3)

--Oben
puzzle({{1},{1},{1}},44,2)
puzzle({{1,0},{1,2}},45,3)
puzzle({{1},{1}},46,4)
puzzle({{2,1},{0,1}},46,3)

--Unten
puzzle({{1},{1},{1}},44,8)
puzzle({{1,2},{1,0}},45,8)
puzzle({{1},{1}},46,7)
puzzle({{0,1},{2,1}},46,8)
