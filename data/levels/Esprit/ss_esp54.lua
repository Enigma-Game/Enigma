-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

-- ESPRIT #054



levelw = 20

levelh = 13

create_world(levelw, levelh)

oxyd_default_flavor = "a"

set_actor("ac-blackball", 10,2.5, {player=0})

fill_floor("fl-abyss",0,0,levelw,levelh)

enigma.SlopeForce = 150

enigma.SlopeForce2 = 150



function renderLine( line, pattern)

    for i=1, strlen(pattern) do

      local c = strsub( pattern, i, i)

      if c ==   "#" then

         set_stone("st-actorimpulse", i-1,line)

      elseif c=="D" then

	 set_stone("st-death",i-1,line)

	 set_item("it-magnet",i-1,line)

      elseif c=="W" then

	 set_stone("st-greenbrown",i-1,line)	 

      elseif c=="1" then

	 set_floor("fl-gradient",  i-1,  line, {type=23})         

      elseif c=="2" then

	 set_floor("fl-gradient",  i-1,  line, {type=24})   

      elseif c=="3" then

	 set_floor("fl-gradient",  i-1,  line, {type=21})      	 

      elseif c=="5" then

         set_floor("fl-gradient",  i-1,  line, {type=5})   

      elseif c=="6" then

         set_floor("fl-gradient",  i-1,  line, {type=6})

      elseif c=="a" then

	 set_floor("fl-gradient",  i-1,  line, {type=24})

	 set_item("it-wormhole",i-1,line, {targetx=" 2.5",targety="1.5",strength=0,range=0})          

      elseif c=="b" then

         set_floor("fl-gradient",  i-1,  line, {type=23})         	 

         set_item("it-wormhole",i-1,line, {targetx=" 17.5",targety="1.5",strength=0,range=0})

      elseif c=="O" then

	 oxyd(i-1,line)         

        end

    end  

end

--               01234567890123456789

renderLine(00 , "####################")

renderLine(01 , "#555555555666666666#")

renderLine(02 , " #5555O5O56O6O6666# ")

renderLine(03 , "  #55353533636366#  ")

renderLine(04 , "   #55O5O56O6O66#   ")

renderLine(05 , "    #5535336366#    ")  

renderLine(06 , "     #55O56O66#     ")

renderLine(07 , "      #333333#      ")

renderLine(08 , "       #3333#       ")

renderLine(09 , "       #3333#       ")

renderLine(10 , "WWWWWWW#3333#WWWWWWW")

renderLine(11 , "Da2222222211111111bD")

renderLine(12 , "WWWWWWWWWWWWWWWWWWWW") 

--               01234567890123456789



oxyd_shuffle()















