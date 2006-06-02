-- a meditation level for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 20
levelh = 13

create_world(levelw, levelh)

fill_floor("fl-water",0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)

  		if c=="x" then
                   set_item("it-hollow",i-1,line)
                   set_floor("fl-black",i-1,line)

                elseif c=="s" then
                   set_item("it-springboard",i-1,line)
                   set_floor("fl-black",i-1,line)

                elseif c=="#" then
                   set_stone("st-glass",i-1,line)

                elseif c=="o" then
                   set_floor("fl-black",i-1,line)
		end
	end	
end

if difficult then
 x = math.random()
 if x < 0.25 then
  renderLine(00 , "                    ")
  renderLine(01 , " #####        ##### ")
  renderLine(02 , " #ooo#        #ooo# ")
  renderLine(03 , " #oxo          oso# ")
  renderLine(04 , " #ooo  ##  ##  ooo# ")
  renderLine(05 , " ##ooo  ####  ooo## ")
  renderLine(06 , "  #oxo        oso#  ")
  renderLine(07 , " ##ooo  ####  ooo## ")
  renderLine(08 , " #ooo  ##  ##  ooo# ")
  renderLine(09 , " #oxo          oso# ")
  renderLine(10 , " #ooo#        #ooo# ")
  renderLine(11 , " #####        ##### ")
  renderLine(12 , "                    ")
 else
  renderLine(00 , "                    ")
  renderLine(01 , " #####        ##### ")
  renderLine(02 , " #ooo#        #ooo# ")
  renderLine(03 , " #oxo          oso# ")
  renderLine(04 , " #ooo##      ##ooo# ")
  renderLine(05 , " ##ooo#      #ooo## ")
  renderLine(06 , "  #oxo        oso#  ")
  renderLine(07 , " ##ooo#      #ooo## ")
  renderLine(08 , " #ooo##      ##ooo# ")
  renderLine(09 , " #oxo          oso# ")
  renderLine(10 , " #ooo#        #ooo# ")
  renderLine(11 , " #####        ##### ")
  renderLine(12 , "                    ")
 end
else
 x = math.random()
 if x < 0.25 then
  renderLine(00 , "                    ")
  renderLine(01 , " #####        ##### ")
  renderLine(02 , " #ooo##########ooo# ")
  renderLine(03 , " #oxo   oooo   oso# ")
  renderLine(04 , " #ooo          ooo# ")
  renderLine(05 , " ##ooo   oo   ooo## ")
  renderLine(06 , "  #oxo   oo   oso#  ")
  renderLine(07 , " ##ooo   oo   ooo## ")
  renderLine(08 , " #ooo          ooo# ")
  renderLine(09 , " #oxo   oooo   oso# ")
  renderLine(10 , " #ooo##########ooo# ")
  renderLine(11 , " #####        ##### ")
  renderLine(12 , "                    ")
 else
  renderLine(00 , "                    ")
  renderLine(01 , " ################## ")
  renderLine(02 , " #ooo  o    o  ooo# ")
  renderLine(03 , " #oxo  o    o  oso# ")
  renderLine(04 , " #ooo  oo  oo  ooo# ")
  renderLine(05 , " ##ooo  o  o  ooo## ")
  renderLine(06 , "  #oxo  o  o  oso#  ")
  renderLine(07 , " ##ooo  o  o  ooo## ")
  renderLine(08 , " #ooo  oo  oo  ooo# ")
  renderLine(09 , " #oxo  o    o  oso# ")
  renderLine(10 , " #ooo  o    o  ooo# ")
  renderLine(11 , " ################## ")
  renderLine(12 , "                    ")
 end
end

-- Actors:
set_actor("ac-whiteball-small", 17.5,3.5, {player=0})  
set_actor("ac-whiteball-small", 15.5,6.5, {player=0})  
set_actor("ac-whiteball-small", 17.5,9.5, {player=0})
