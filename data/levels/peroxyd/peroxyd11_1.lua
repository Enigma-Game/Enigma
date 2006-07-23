-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- Per.Oxyd Single #011

levelw = 58
levelh = 37

create_world(levelw, levelh)

oxyd_default_flavor = "a"
fill_floor("fl-metal", 0, 0, levelw,levelh)
set_actor("ac-blackball", 32.5,18.5)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c == "#" then
         set_stone("st-metal",i-1,line)
      elseif c == "D" then                      
	 set_stone("st-death", i-1,line)
      elseif c=="d" then
	 doorh( i-1,line, {name="door"})
	 set_floor("fl-space", i-1,line)
      elseif c == "C" then                      
	 set_stone("st-coinslot", i-1,line,{target="door", action="openclose"})
      elseif c=="1" then	
	 set_item("it-coin1", i-1,line)
      elseif c=="2" then	
	 set_item("it-coin2", i-1,line)
      elseif c=="4" then	
	 set_item("it-coin4", i-1,line)
      elseif c=="s" then	
	 set_item("it-spring1", i-1,line)
      elseif c=="c" then	
	 set_item("it-coffee", i-1,line)
      elseif c=="a" then	
	 set_floor("fl-abyss", i-1,line)
      elseif c=="0" then	
	 set_floor("fl-space", i-1,line)	 
      elseif c=="W" then                                                                
         set_item("it-magnet",i-1,line,{name="magnetW", on=0})
         set_floor("fl-space",i-1,line)	 
      elseif c=="X" then                                                                
         set_item("it-magnet",i-1,line,{name="magnetX", on=0})
         set_floor("fl-space",i-1,line)
      elseif c=="Y" then                                                                
         set_item("it-magnet",i-1,line,{name="magnetY", on=0})
         set_floor("fl-space",i-1,line)
      elseif c=="Z" then                                                                
         set_item("it-magnet",i-1,line,{name="magnetZ", on=0})
         set_floor("fl-space",i-1,line)
      elseif c=="w" then
	 set_item("it-trigger",i-1,line, {action="onoff", target="magnetW"})         
      elseif c=="x" then
	 set_item("it-trigger",i-1,line, {action="onoff", target="magnetX"})   
      elseif c=="y" then
	 set_item("it-trigger",i-1,line, {action="onoff", target="magnetY"})     	 
      elseif c=="z" then
         set_item("it-trigger",i-1,line, {action="onoff", target="magnetZ"})
      elseif c=="n" then
         document(i-1,line,"Rest on the bright spot for a minute")
      elseif c=="+" then                                                                
         set_item("it-trigger",i-1,line, {invisible=1, action="callback", target="s1"})
         set_floor("fl-light",i-1,line)
      elseif c=="~" then                                                                
         set_item("it-crack0",i-1,line)
      elseif c=="-" then                                                                
         --set_item("it-calculator",i-1,line)
      elseif c == "O" then
         oxyd( i-1, line)
        end
    end
end

--              0123456789012345678901234567890123456789012345678901234567
renderLine(00, "##########################################################")
renderLine(01, "#                                                        #")
renderLine(02, "#                                                        #")
renderLine(03, "#                                                        #")
renderLine(04, "#                                                        #")
renderLine(05, "#                  D######################################")
renderLine(06, "#                  00000000000WwXxYyZz000000000000000000O#")
renderLine(07, "#                  D######################################")
renderLine(08, "#                                                        #")
renderLine(09, "#                                                        #")
renderLine(10, "#                                                        #")
renderLine(11, "#                                                        #")
renderLine(12, "#                  ####################                  #")
renderLine(13, "#                  #                  #                Dd#")
renderLine(14, "#                  #                  #                C0#")
renderLine(15, "#                  #                  #               s#0#")
renderLine(16, "#                  #     +            #                #0#")
renderLine(17, "#                  #                  #                #0#")
renderLine(18, "#                  # -      n         #                #0#")
renderLine(19, "#                  #                  #                #0#")
renderLine(20, "#                  #                  #                #0#")
renderLine(21, "#                  #                  #                #0#")
renderLine(22, "#                  #                  #                #0#")
renderLine(23, "#                  #                  #                #0#")
renderLine(24, "#                  ####################                #0#")
renderLine(25, "#                                                      #0#")
renderLine(26, "#                                                      #0#")
renderLine(27, "#                                                      #0#")
renderLine(28, "#                                                      #0#")
renderLine(29, "# ~                                                    #0#")
renderLine(30, "#  ~  ~                                                #0#")
renderLine(31, "#aaaaaaa                                               #0#")
renderLine(32, "# 4    a                                               #0#")
renderLine(33, "#   c  a                                               #0#")
renderLine(34, "# 1  1 a                                               #0#")
renderLine(35, "#1 2   a                                               #O#")
renderLine(36, "##########################################################")
--              0123456789012345678901234567890123456789012345678901234567

flag = 1
time_old = 3600

function s1()
flag = 1 - flag
time = enigma.date("%S") + enigma.date("%M")*60
a = (time-time_old) * flag -- only when waiting ON the trigger
if a > 60 then
 set_stone("st-metal_hole",19,18)
 document(19,18,"You'll do anything you're told!")
 end
time_old=time
end

oxyd_shuffle()












