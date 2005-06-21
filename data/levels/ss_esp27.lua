-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #027

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "b"
set_actor("ac-blackball", 9.5,6.5)
draw_checkerboard_floor( "fl-black", "fl-leaves", 0, 0, levelw, levelh)
for x=0,9 do
    for y=0,5 do
	set_floor("fl-light",2*x+1,2*y+1)
	end
    end
function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-invisible", i-1,line)
      elseif c=="W" then
         set_item("it-wormhole",i-1,line, {targetx=" 18.5",targety="6.5",strength=5})
      elseif c=="w" then                                                                
         set_item("it-wormhole",i-1,line, {targetx=" 9.5",targety="6.5",strength=5})
      elseif c=="O" then
	 oxyd(i-1,line)
        end
    end  
end
--               01234567890123456789
renderLine(00 , "####################")
renderLine(01 , "#W#           #    #")
renderLine(02 , "# O## ##### # # #O #")
renderLine(03 , "# #  #      # # #  #")
renderLine(04 , "# #  ## ##  # # ####")  
renderLine(05 , "#    ##  #  # # ####")
renderLine(06 , "# O#  # #.# # # #O #")
renderLine(07 , "##### # # # # # #  #")
renderLine(08 , "## ## # #   # # #  #")
renderLine(09 , "#   # #  ###    #  #")
renderLine(10 , "# O # #       ###O #")
renderLine(11 , "#             #w   #")
renderLine(12 , "####################")
--               01234567890123456789

oxyd_shuffle()
