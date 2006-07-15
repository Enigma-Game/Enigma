-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

-- ESPRIT #028

-- question: how can I turn a fourswitch by a timer?



enigma.ConserveLevel = FALSE

levelw = 20

levelh = 13

create_world(levelw, levelh)

oxyd_default_flavor = "b"

set_actor("ac-blackball", 8,6.5)

set_actor("ac-top", 13.5,10.5, {range=15, force=10})

fill_floor("fl-water",0,0,levelw,levelh)

fill_floor("fl-bluegreenx",4,1,levelw-9,levelh-2)



function renderLine( line, pattern)

    for i=1, strlen(pattern) do

      local c = strsub( pattern, i, i)

      if c ==   "#" then

         set_stone("st-fakeoxyd", i-1,line)

      elseif c=="B" then

         set_stone("st-actorimpulse", i-1,line)

      elseif c=="S" then                                                                

         set_stone("st-fourswitch", i-1,line, {name="switch",action="callback", target="s1"})

      elseif c=="T" then

         set_stone( "st-timer", i-1,line, {action="callback", target="s1"})      

      elseif c=="1" then                                                                

	 set_item("it-magnet",i-1,line,{name="magnet1", on=1})

      elseif c=="2" then                                                                

         set_item("it-magnet",i-1,line,{name="magnet2", on=0})

      elseif c=="3" then                                                                

         set_item("it-magnet",i-1,line,{name="magnet3", on=0})

      elseif c=="4" then                                                                

         set_item("it-magnet",i-1,line,{name="magnet4", on=0})

      elseif c=="O" then

	 oxyd(i-1,line)

        end

    end  

end

--               01234567890123456789

renderLine(00 , "                    ")

renderLine(01 , "    #####O#####     ")

renderLine(02 , "    #    1    #     ")

renderLine(03 , "    #         #     ")

renderLine(04 , "    #         #     ")  

renderLine(05 , "    #         #     ")

renderLine(06 , "    O4   B   2O     ")

renderLine(07 , "    #         #     ")

renderLine(08 , "    #         #     ")

renderLine(09 , "    #         #     ")

renderLine(10 , "    #    3    #     ")

renderLine(11 , "    #####O##ST#     ")

renderLine(12 , "                    ")

--               01234567890123456789



x=1



function s1() 

SendMessage("magnet"..x, "off")

x=x+1

if x==5 then x=1 end

SendMessage("magnet"..x, "on")

SendMessage("switch", "turn", WEST)

end



oxyd_shuffle()












