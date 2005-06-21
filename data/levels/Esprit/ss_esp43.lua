-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- Esprit #043

levelw = 58
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "a"
set_actor("ac-blackball", 7.5,7.5, {player=0})

fill_floor("fl-hay", 0,0, levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-greenbrown", i-1,line)
      elseif c =="h" then
         set_stone("st-greenbrown_move", i-1,line)
	 hammer(i-1,line)
      elseif c=="w" then	
         set_stone("st-volcano", i-1,line,{name="vulcano2"})
      elseif c =="t" then
         set_stone("st-grate1", i-1,line)
         set_item("it-trigger", i-1,line, {invisible=1, action="callback", target="s1"})
      elseif c=="v" then	
         set_stone("st-volcano", i-1,line,{name="vulcano1"})
      elseif c=="m" then	
         set_stone("st-greenbrown_move", i-1,line)
      elseif c =="1" then
         set_item("it-coffee", i-1,line)
      elseif c =="2" then
         set_item("it-cherry", i-1,line)
      elseif c =="3" then
         set_item("it-coin4", i-1,line)
      elseif c =="4" then
         set_item("it-spade", i-1,line)
      elseif c =="5" then
         set_item("it-extinguisher", i-1,line)
      elseif c =="6" then
         set_item("it-extralife", i-1,line)
      elseif c =="7" then
         set_item("it-key_b", i-1,line)
      elseif c =="8" then
         set_item("it-coin1", i-1,line)
      elseif c =="9" then
         set_item("it-flagblack", i-1,line)
      elseif c =="0" then
         set_item("it-brush", i-1,line)   
      elseif c =="f" then              
         set_item("it-floppy", i-1,line)
      elseif c =="." then              
         set_item("it-seed", i-1,line)                                                                         
      elseif c=="O" then                                                               
	 oxyd(i-1,line)         
        end
    end  
end
--               0123456789012345678901234567890123456789012345678901234567
renderLine(00 , "##########################################################")
renderLine(01 , "#1233456781190     #                  #                  #")
renderLine(02 , "#f################ #                  #                  #")
renderLine(03 , "# #              # #                  #              m   #")
renderLine(04 , "# #              # #           .   .  #                m #")  
renderLine(05 , "# #              # m                  t .     m          #")
renderLine(06 , "#                 vm    O    ...O     t .      wh O  O m #")
renderLine(07 , "#O  O              #         ... .    t .     m          #")
renderLine(08 , "#                  #         ......   #                m #")
renderLine(09 , "#                  #            ...   # . .              #")
renderLine(10 , "#                  #                  #              m   #")
renderLine(11 , "#                  #                  #                  #")
renderLine(12 , "##########################################################") 
--               0123456789012345678901234567890123456789012345678901234567

oxyd_shuffle()

function s1()
enigma.SendMessage(enigma.GetNamedObject("vulcano1"), "trigger", nil)
enigma.SendMessage(enigma.GetNamedObject("vulcano2"), "trigger", nil)
end

