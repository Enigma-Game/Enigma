-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

-- ESPRIT #036

-- not implemented yet: using cherry makes invisible for the rotors



levelw = 39

levelh = 13

create_world(levelw, levelh)

oxyd_default_flavor = "b"

set_actor("ac-blackball", 27.5,6.5)



fill_floor("fl-concrete",0,0,levelw,levelh)



function renderLine( line, pattern)

    for i=1, strlen(pattern) do

      local c = strsub( pattern, i, i)

      if c ==   "#" then

         set_stone("st-marble", i-1,line)

      elseif c=="M" then

	 set_stone("st-marble_move", i-1,line)         

      elseif c=="H" then

	 set_stone("st-marble_hole", i-1,line) 

	 set_actor("ac-rotor", i-1+0.5,line+0.5, {range=4, force=30})  

      elseif c=="W" then

	 set_item("it-wormhole",i-1,line, {name="worm1",targetx=" 6.5",targety="6.5",strength=0})   

      elseif c=="w" then

	 set_item("it-wormhole",i-1,line, {name="worm2",targetx=" 32.5",targety="6.5",strength=0})    

      elseif c=="K" then

         set_item("it-cherry",i-1,line)    	

      elseif c=="1" then

         set_item("it-trigger", i-1,line, {invisible=1,action="callback", target="s1"})     

      elseif c=="2" then

         set_item("it-trigger", i-1,line, {invisible=1,action="callback", target="s2"})                    

      elseif c=="O" then

	 oxyd(i-1,line)         

        end

    end  

end

--               012345678901234567890123456789012345678

renderLine(00 , "#######################################")

renderLine(01 , "#####################H###########H#####")

renderLine(02 , "##                                   ##")

renderLine(03 , "## ##################O###########O## ##")

renderLine(04 , "## ################################# ##")  

renderLine(05 , "## ################################# ##")

renderLine(06 , "## ##w1            M K   K   K  2W## ##")

renderLine(07 , "## ################################# ##")

renderLine(08 , "## ################################# ##")

renderLine(09 , "## ##################O###########O## ##")

renderLine(10 , "##                                   ##")

renderLine(11 , "#####################H###########H#####")

renderLine(12 , "#######################################") 

--               012345678901234567890123456789012345678



local flag = {0,0}



function s1()

   flag[1] = 1-flag[1]

set_attribs(enigma.GetNamedObject("worm1"), {targetx=30*flag[1]+6.5}) 

end



function s2()

   flag[2] = 1-flag[2]                                                 

set_attribs(enigma.GetNamedObject("worm2"), {targetx=32.5-(30*flag[2])})

end



oxyd_shuffle()


























