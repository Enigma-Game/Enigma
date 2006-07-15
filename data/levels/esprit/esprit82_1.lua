-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

-- Esprit #82



levelw = 58

levelh = 13

create_world(levelw, levelh)

display.SetFollowMode(display.FOLLOW_SCROLLING)

oxyd_default_flavor = "b"

set_actor("ac-rotor", 3, 10.5, {mouseforce=0, range=70, force=50})

set_actor("ac-blackball", 51.5, 10.5)



fill_floor("fl-bluegray", 0,0, levelw,levelh)

enigma.AddConstantForce(0,40)



function renderLine( line, pattern)

    for i=1, strlen(pattern) do

      local c = strsub( pattern, i, i)

      if c ==   "#" then

         set_stone("st-glass2", i-1,line)

      elseif c =="b" then

         set_stone("st-actorimpulse", i-1,line)

      elseif c =="c" then

         set_item("it-coffee", i-1,line)

      elseif c =="." then

         set_item("it-extralife", i-1,line)

      elseif c=="O" then                                                               

	 oxyd(i-1,line)         

        end

    end  

end

--               0123456789012345678901234567890123456789012345678901234567

renderLine(00 , "##########################################################")

renderLine(01 , "#                                                        #")

renderLine(02 , "#  O  c   O   c  O   .  c     O       c    O     c    O  #")

renderLine(03 , "#                                                        #")

renderLine(04 , "#                                                        #")  

renderLine(05 , "#                                                        #")

renderLine(06 , "#                                                        #")

renderLine(07 , "#                                                        #")

renderLine(08 , "#                                                        #")

renderLine(09 , "#                                                        #")

renderLine(10 , "#                                                        #")

renderLine(11 , "#                                                        #")

renderLine(12 , "#bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb#") 

--               0123456789012345678901234567890123456789012345678901234567



oxyd_shuffle()












