-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

levelw = 39
levelh = 13
display.SetFollowMode(display.FOLLOW_SCROLLING)
create_world(levelw, levelh)
oxyd_default_flavor = "c"
set_actor("ac-blackball", 2.5,6.5)

fill_floor("fl-black", 0,0, levelw,levelh)
fill_items( "it-burnable", 0, 0, 24, 13)
draw_floor("fl-metal",{24,0},{0,1},13)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-plain", i-1,line)
      elseif c =="G" then
         set_stone("st-grate2", i-1,line)
      elseif c =="b" then
         set_item("it-brush", i-1,line)
      elseif c =="s" then
         set_item("it-spade", i-1,line)
      elseif c =="d" then
         set_item("it-dynamite", i-1,line) 
      elseif c =="f" then
         set_item("it-extinguisher", i-1,line)
      elseif c =="~" then
         set_item("it-burnable", i-1,line, {name="burnable1"})    
      elseif c =="+" then
         set_item("it-burnable", i-1,line, {name="burnable2"})                
      elseif c =="t" then
         set_item("it-trigger", i-1,line, {invisible=1, action="callback", target="s1"})
      elseif c =="k" then
         set_item("it-key_a", i-1,line)
      elseif c =="B" then
         set_stone("st-key_a", i-1, line, {action="openclose", target="door1"})
      elseif c =="D" then
         set_stone("st-key_a", i-1, line, {action="openclose", target="door2"})
      elseif c =="A" then
         set_stone("st-door_c", i-1, line,  {name="door1"})
      elseif c =="C" then
         set_stone("st-door_c", i-1, line,  {name="door2"})
      elseif c =="e" then
         set_stone("st-door_c", i-1, line,  {name="door3"})
      elseif c =="E" then
         set_stone("st-coinslot", i-1,line, {action="openclose", target="door3"})
      elseif c=="m" then	
         set_item("it-coin2", i-1,line,{value=3})
      elseif c =="1" then
         set_item("it-extralife", i-1,line)
      elseif c =="2" then
         set_item("it-magicwand", i-1,line)
      elseif c =="3" then
         set_item("it-document", i-1,line)
      elseif c =="4" then
         set_item("it-cherry", i-1,line)
      elseif c =="5" then
         set_item("it-weight", i-1,line)
      elseif c =="6" then
         set_item("it-hammer", i-1,line)
      elseif c =="7" then
         set_item("it-sword", i-1,line)
      elseif c =="8" then
         set_item("it-wrench", i-1,line)
      elseif c =="9" then
         set_item("it-seed", i-1,line)
      elseif c =="0" then
         set_item("it-brake", i-1,line)   
      elseif c =="u" then              
         set_item("it-umbrella", i-1,line)
                                                                         
      elseif c=="O" then                                                               
	 oxyd(i-1,line)         
        end
    end  
end
--               012345678901234567890123456789012345678
renderLine(00 , "#######################################")
renderLine(01 , "#bbb#uuuu#ffff#12 3#76 3#             #")
renderLine(02 , "#bbb#uuuu#ffff#4564#2 18# O    O    O #")
renderLine(03 , "#bbb#uuuu#ffff#8907#8907#             #")
renderLine(04 , "#bbb#uuuu#ffff# 4mm# 666#             #")  
renderLine(05 , "#   #AB###CD###GGGG#GGGG#             #")
renderLine(06 , "#   t     ~             E             #")
renderLine(07 , "#k  #GGGG#GGGG#         e             #")
renderLine(08 , "#sss#dddd#    #         #             #")
renderLine(09 , "#sss#dddd#    #         #             #")
renderLine(10 , "#sss#dddd#    #         # O    O    O #")
renderLine(11 , "#sss#dddd#    #        +#             #")
renderLine(12 , "#######################################") 
--               012345678901234567890123456789012345678

function s1()
SendMessage("burnable1", "ignite")
SendMessage("burnable2", "ignite")
end

