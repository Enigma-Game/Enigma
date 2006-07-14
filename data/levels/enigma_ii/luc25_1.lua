-- One Way Ticket, a level for enigma
-- Copyright (C) 2006 Lukas Schueller
-- Licensed under GPL v2.0 or above
-------------------------------------------
CreateWorld(39,13)
done = 0
-------------------------------------------
--environment
fill_floor("fl-leaves",0,0,39,13)
set_actor("ac-blackball",1.5,6.5,{player=0})
-------------------------------------------
--function to create one line of the level
function line(line,commands)
  lx = 1
  while(lx <= strlen(commands)) do
    i = strsub(commands,lx,lx)
    if(i == "#") then
      set_stone("st-greenbrown",lx-1,line-1)
    end
    if(i == "I") then
      set_stone("st-door",lx-1,line-1,{name="doora",type="v"})
    end
    if(i == "=") then
      set_stone("st-door",lx-1,line-1,{name="doorb",type="h"})
    end
    if(i == "-") then
      set_stone("st-door",lx-1,line-1,{name="doorc",type="h"})
    end
    if(i == "h") then
      set_stone("st-greenbrown_hole",lx-1,line-1)
    end
    if(i == "m") then
      set_stone("st-greenbrown_move",lx-1,line-1)
    end
    if(i == "F") then
      set_stone("st-floppy",lx-1,line-1,{action="openclose",target="doora"})
    end
    if(i == "A") then
      set_stone("st-key_a",lx-1,line-1,{action="openclose",target="doorb"})
    end
    if(i == "B") then
      set_stone("st-key_b",lx-1,line-1,{action="openclose",target="doorc"})
    end
    if(i == "f") then
      set_item("it-floppy",lx-1,line-1)
    end
    if(i == "a") then
      set_item("it-key_a",lx-1,line-1)
    end
    if(i == "b") then
      set_item("it-key_b",lx-1,line-1)
    end
    if(i == "t") then
      set_item("it-trigger",lx-1,line-1,{action="close",target="doora",invisible=1})
    end
    if(i == "s") then
      set_stone("st-switch",lx-1,line-1,{action="callback",target="draw_line"})
    end
    if(i == "o") then
      oxyd(lx-1,line-1)
    end
    lx = lx + 1
  end
end
----------------------------------------
--function that changes the 10th line (used for the solution)
function draw_line()
  if(done == 0) then
    line(10,"#        mh")
    done = 1
  end
end
-----------------------------------------
--environment
oxyd_default_flavor = "c"
line(01,"#######################################")
line(02,"#o#      ##a       #o                o#")
line(03,"#=A      ##        h                  #")
line(04,"#        ##        #                  #")
line(05,"#        ##        #                  #")
line(06,"#        F#        #                  #")
line(07,"#        I t       #        s         #")
line(08,"#        ##        #                  #")
line(09,"#        ##        #                  #")
line(10,"#        ##        #                  #")
line(11,"#-B      ##        #                  #")
line(12,"#o#     f##b       #o                o#")
line(13,"#######################################")
oxyd_shuffle()
enigma.ConserveLevel = FALSE












