-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

-- ESPRIT #035

-- change rotors with spermbirds if implemented!



levelw = 39

levelh = 25

create_world(levelw, levelh)

oxyd_default_flavor = "b"

set_actor("ac-blackball", 18.5,11.5)

fill_floor("fl-marble",0,0,levelw,levelh)



function renderLine( line, pattern)

    for i=1, strlen(pattern) do

      local c = strsub( pattern, i, i)

      if c ==   "#" then

         set_stone("st-marble", i-1,line)

      elseif c=="r" then        

	 set_actor("ac-rotor", i-.5,line+.5, {range=7, force=40}) -- range means stones between

      elseif c=="c" then

         set_item("it-coffee",i-1,line) 

      elseif c=="d" then

         set_item("it-dynamite",i-1,line) 

      elseif c=="O" then

	 oxyd(i-1,line)         

        end

    end  

end

--               012345678901234567890123456789012345678

renderLine(00 , "#######################################")

renderLine(01 , "#.                                   .#")

renderLine(02 , "#                                     #")

renderLine(03 , "#   c                               c #")

renderLine(04 , "#                                     #")  

renderLine(05 , "#                        c            #")

renderLine(06 , "# d      Or                 rO        #")

renderLine(07 , "#                                     #")

renderLine(08 , "#                                     #")

renderLine(09 , "#d   d                                #")

renderLine(10 , "#                                     #")

renderLine(11 , "#                                     #")

renderLine(12 , "#                 OO                  #") 

renderLine(13 , "#                 OO                  #")

renderLine(14 , "#                                     #")

renderLine(15 , "#   c                                 #")

renderLine(16 , "#                                c    #")

renderLine(17 , "#                                  d  #")  

renderLine(18 , "#        Ord             d  rO        #")

renderLine(19 , "#                 c            d      #")

renderLine(20 , "#      d                              #")

renderLine(21 , "#                            d        #")

renderLine(22 , "#             d                   d   #")

renderLine(23 , "#.                                   .#")

renderLine(24 , "#######################################")

--               012345678901234567890123456789012345678

                               

                               

oxyd_shuffle()











