-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #069

levelw = 39
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "b"
set_actor("ac-blackball", 27.5,6.5)
display.SetFollowMode(display.FOLLOW_SCROLLING)

fill_floor("fl-rough-red",0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-stone1", i-1,line)
      elseif c =="m" then
         set_item("it-coin4", i-1,line)
      elseif c=="w" then
         set_floor("fl-wood",  i-1,  line)
         set_item("it-burnable_ignited", i-1,  line)
      elseif c=="1" then
         set_stone("st-door_c", i-1,line, {name="door1", type="v"})
      elseif c=="2" then
         set_stone("st-door_c", i-1,line, {name="door2", type="v"})
      elseif c=="3" then
         set_stone("st-door_c", i-1,line, {name="door3", type="v"})
      elseif c=="4" then
         set_stone("st-door_c", i-1,line, {name="door4", type="v"})
      elseif c=="5" then
         set_stone("st-door_c", i-1,line, {name="door5", type="v"})
      elseif c=="6" then
         set_stone("st-door_c", i-1,line, {name="door6", type="v"})
      elseif c=="7" then
         set_stone("st-door_c", i-1,line, {name="door7", type="v"})
      elseif c=="9" then
         set_stone("st-door_c", i-1,line, {name="door9", type="v"})
      elseif c =="a" then
         set_item("it-key_a", i-1,line)                  
      elseif c =="b" then
         set_item("it-key_b", i-1,line)
      elseif c =="c" then
         set_item("it-key_c", i-1,line)
      elseif c =="I" then
         set_stone("st-coinslot", i-1,line, {action="openclose", target="door9"})
      elseif c =="E" then
         set_stone("st-coinslot", i-1,line, {action="openclose", target="door5"})
      elseif c =="A" then
         set_stone("st-key_a", i-1, line, {action="openclose", target="door1"})
      elseif c =="B" then
         set_stone("st-key_b", i-1, line, {action="openclose", target="door2"})
      elseif c =="C" then
         set_stone("st-key_c", i-1, line, {action="openclose", target="door3"})
      elseif c=="D" then
	 set_stone("st-switch",i-1,line, {action="openclose", target="door4"})         
      elseif c=="F" then
	 set_stone("st-switch",i-1,line, {action="openclose", target="door6"})   
      elseif c=="G" then
	 set_stone("st-switch",i-1,line, {action="openclose", target="door7"})
      elseif c=="r" then
	 set_actor("ac-rotor", i,line+1, {mouseforce=0, range=50, force=70})
      elseif c=="R" then
	 set_actor("ac-rotor", i,line+1, {mouseforce=0, range=6, force=20})
      elseif c=="O" then
	 oxyd(i-1,line)         
        end
    end  
end
--               012345678901234567890123456789012345678
renderLine(00 , "#######################################")
renderLine(01 , "#m          ###        ###            #")
renderLine(02 , "#m          #O#        #O#            #")
renderLine(03 , "###A        #4#D       #3#C        ####")
renderLine(04 , "#R #                 b             7O##")  
renderLine(05 , "#  1                          m    ####")
renderLine(06 , "####                           m  wGw #")
renderLine(07 , "#r 2                   c          www #")
renderLine(08 , "#  #                                 m#")
renderLine(09 , "###B      #5#E    #6#F    #9#I        #")
renderLine(10 , "#         #O#     #O#     #O#m        #")
renderLine(11 , "#         ###     ###     ###        a#")
renderLine(12 , "#######################################") 
--               012345678901234567890123456789012345678

oxyd_shuffle()
