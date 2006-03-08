-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

-- ESPRIT #063



levelw = 20

levelh = 13

create_world(levelw, levelh)

oxyd_default_flavor = "b"

set_actor("ac-blackball", 9.5,6.5)

set_actor("ac-killerball", 5.5,6.5, {mouseforce=1, controllers=1})

set_actor("ac-killerball", 13.5,6.5, {mouseforce=1, controllers=1})

fill_floor("fl-abyss",0,0,levelw,levelh)



function renderLine( line, pattern)

    for i=1, strlen(pattern) do

      local c = strsub( pattern, i, i)

      if c ==   "F" then

         set_stone("st-fakeoxyd", i-1,line)

      elseif c=="#" then

         set_floor("fl-normal",  i-1,  line) 

      elseif c=="a" then

         set_floor("fl-gradient",  i-1,  line, {type=12})   

      elseif c=="b" then

         set_floor("fl-gradient",  i-1,  line, {type=10})         

      elseif c=="c" then

         set_floor("fl-gradient",  i-1,  line, {type=11})   

      elseif c=="d" then

         set_floor("fl-gradient",  i-1,  line, {type=9})   		 	 	 

      elseif c=="1" then

	 set_floor("fl-gradient",  i-1,  line, {type=3})         

      elseif c=="2" then

	 set_floor("fl-gradient",  i-1,  line, {type=4})   

      elseif c=="3" then

	 set_floor("fl-gradient",  i-1,  line, {type=1})   

      elseif c=="4" then

	 set_floor("fl-gradient",  i-1,  line, {type=2})         

      elseif c=="5" then

         set_floor("fl-gradient",  i-1,  line, {type=5})   

      elseif c=="6" then

         set_floor("fl-gradient",  i-1,  line, {type=6})   	

      elseif c=="7" then

         set_floor("fl-gradient",  i-1,  line, {type=7})         

      elseif c=="8" then

         set_floor("fl-gradient",  i-1,  line, {type=8})

      elseif c=="O" then

	 oxyd(i-1,line)         

        end

    end  

end

--               01234567890123456789

renderLine(00 , "                    ")

renderLine(01 , "    FOFF   FFOF     ")

renderLine(02 , "    F56F   F56F     ")

renderLine(03 , "    O12OFOFO12O     ")

renderLine(04 , "    F1c33333d2F     ")  

renderLine(05 , "    F1#######2F     ")

renderLine(06 , "    O1#######2O     ")

renderLine(07 , "    F1#######2F     ")

renderLine(08 , "    F1a44444b2F     ")

renderLine(09 , "    O12OFOFO12O     ")

renderLine(10 , "    F78F   F78F     ")

renderLine(11 , "    FOFF   FFOF     ")

renderLine(12 , "                    ") 

--               01234567890123456789



oxyd_shuffle()











