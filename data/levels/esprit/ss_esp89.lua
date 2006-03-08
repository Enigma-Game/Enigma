-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #089

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "a"       -- Default flavor for oxyd stones.
set_actor("ac-blackball",3.5,2.5)
set_actor("ac-killerball", 12.5,10.5, {mouseforce=1, controllers=1}) 

fill_floor("fl-water",0,0,levelw,levelh)
fill_floor("fl-sand",1,1,18,11)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-invisible", i-1,line)
      elseif c=="O" then
	 oxyd(i-1,line)
        end
    end  
end
--               01234567890123456789
renderLine(00 , "                    ")
renderLine(01 , " ################## ")
renderLine(02 , "#O# # #   #       O#")
renderLine(03 , "# # # # ### #####  #")
renderLine(04 , "# #       #     #  #")  
renderLine(05 , "# ######  ####  #  #")
renderLine(06 , "#    O#         #  #")
renderLine(07 , "#  #  #    # #O    #")
renderLine(08 , "#  #  #### # ### # #")
renderLine(09 , "#  #       #     # #")
renderLine(10 , "#O #       #     #O#")
renderLine(11 , " ################## ")
renderLine(12 , "                    ")
--               01234567890123456789

oxyd_shuffle()










