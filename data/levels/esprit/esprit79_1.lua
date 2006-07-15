-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #079

create_world(20, 13)
oxyd_default_flavor = "a"
set_actor("ac-blackball", 10.5, 6.5)
fill_floor("fl-concrete")
enigma.ConserveLevel=FALSE

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-brick",i-1,line)
      elseif c =="c" then                                                               
         set_item("it-coin1", i-1,line)
      elseif c =="M" then 
         set_stone("st-rock1_move",i-1,line)
      elseif c =="m" then                                                               
         set_stone("st-rock1_move",i-1,line)
         set_item("it-spade", i-1,line)
      elseif c =="o" then                                                               
         set_item("it-hollow",i-1,line)
      elseif c =="*" then                                                               
         set_item("it-hill",i-1,line)
      elseif c=="C" then
         set_stone("st-coinslot", i-1,line,{target="s1", action="callback"})
      elseif c=="1" then
         set_stone("st-door_c", i-1,line, {name="door1", type="v"})
      elseif c=="2" then
         set_stone("st-door_c", i-1,line, {name="door2", type="v"})
      elseif c=="R" then
	 --set_actor("ac-rotor", i-1+0.5,line+0.5, {mouseforce=1, range=25, force=40})
         set_actor("ac-rotor", i-1+0.5,line+0.5, {range=25, force=40})
      elseif c=="O" then
	 oxyd(i-1,line) 
        end
    end
end
--              01234567890123456789
renderLine(00, "####################")
renderLine(01, "#O 1***o****o******#")
renderLine(02, "# R2o*o***o**o**o**#")
renderLine(03, "####C############*o#")
renderLine(04, "#################o*#")  
renderLine(05, "#                oo#")
renderLine(06, "#       M  M   o   #")
renderLine(07, "#              M o #")
renderLine(08, "#  c cc         oo #")
renderLine(09, "# c cc    m  M ooo #")
renderLine(10, "# c c c      ooo   #")
renderLine(11, "#            o    O#")
renderLine(12, "####################")
--              01234567890123456789
                              
oxyd_shuffle()

function s1()
  SendMessage("door1", "openclose")
  SendMessage("door2", "openclose")
end











