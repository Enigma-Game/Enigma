-- NAME, a level for Enigma
-- Copyright (C) YEAR AUTHOR
-- Licensed under GPL v2.0 or above 

-- GENERAL --
Require("/levels/lib/libpuzzle.lua")

levelw = 39
levelh = 13

create_world(levelw, levelh)

fill_floor("fl-abyss",0,0,levelw,levelh)
fill_floor("fl-metal",0,1,levelw,levelh-2)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
  		if c=="X" then
			oxyd(i-1,line)
                elseif c=="#" then
                   set_stone("st-plain",i-1,line)

                elseif c=="O" then
                   set_stone("st-oneway_black-s",i-1,line)
                elseif c=="K" then
                   set_stone("st-wood",i-1,line)

                elseif c=="T" then
                   set_stone("st-turnstile",i-1,line)
                elseif c=="N" then
                   set_stone("st-turnstile-n",i-1,line)
                elseif c=="E" then
                   set_stone("st-turnstile-e",i-1,line)
                elseif c=="S" then
                   set_stone("st-turnstile-s",i-1,line)
                elseif c=="W" then
                   set_stone("st-turnstile-w",i-1,line)

                elseif c=="w" then
                   set_floor("fl-water",i-1,line)
                elseif c=="a" then
                   set_floor("fl-abyss",i-1,line)

		elseif c=="f" then
		   set_item("it-flagblack",i-1,line)
		elseif c=="s" then
		   set_item("it-spring1",i-1,line)
		end
	end	
end

renderLine(00 , "                                       ")
renderLine(01 , "#######################################")
renderLine(02 , "##### N      ### N     ################")
renderLine(03 , "###   TETETE   # T ###    aaaas w     #")
renderLine(04 , "##  # S S S  #  NS ##  ########## #   #")
renderLine(05 , "## X########### TE    #       ###  wWT#")
renderLine(06 , "##O####################       #   KN  #")
renderLine(07 , "#f X###################    N  # ## T  #")
renderLine(08 , "## ##  N N N ########     WTE #    S  #")
renderLine(09 , "##     TETETE  ###### #    www###  N  #")
renderLine(10 , "#####  S S S #        #    www     T  #")
renderLine(11 , "#######################################")
renderLine(12 , "                                       ")

puzzle({{1,1},{1,1}},24,8,"blue","no")

-- Special
set_item("it-document", 17, 10, {text="Haben Sie Ihre Fahne dabei?"})
set_actor("ac-blackball", 2.5,7.5, {player=0})  

