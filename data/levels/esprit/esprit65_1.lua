-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

-- ESPRIT #065



levelw = 20

levelh = 13



create_world(levelw, levelh)

oxyd_default_flavor = "d"

set_actor("ac-blackball", 10,1.5)



fill_floor("fl-samba", 0,0, levelw,levelh)



function renderLine( line, pattern)

    for i=1, strlen(pattern) do

      local c = strsub( pattern, i, i)

      if c ==   "#" then

         set_stone("st-rock1", i-1,line)

      elseif c ==">" then

         set_stone("st-oneway_black-e",i-1,line)

      elseif c =="<" then
         set_stone("st-oneway_black-w",i-1,line)

      elseif c =="v" then                                                               

         set_item("it-wormhole",i-1,line, {name="warp",targetx="3.5",targety="3.5",strength=0})

      elseif c =="w" then                                                               

         set_item("it-wormhole",i-1,line, {name="warp",targetx="16.5",targety="11.5",strength=0})

      elseif c=="O" then                                                               

	 oxyd(i-1,line) 

        end

    end  

end

--               01234567890123456789

renderLine(00 , "####################")

renderLine(01 , "#     >      <     #")

renderLine(02 , "O     >      <     O")

renderLine(03 , "#     >      <     #")

renderLine(04 , "######>      <######")  

renderLine(05 , "#     >      <     #")

renderLine(06 , "O     >  vw  <     O")

renderLine(07 , "#     >      <     #")

renderLine(08 , "######>      <######")

renderLine(09 , "#     >      <     #")

renderLine(10 , "O     >      <     O")

renderLine(11 , "#     >      <     #")

renderLine(12 , "####################") 

--               01234567890123456789



oxyd_shuffle()












