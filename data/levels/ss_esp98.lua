-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #098

levelw = 20
levelh = 49
create_world(levelw, levelh)
oxyd_default_flavor = "a"
set_actor("ac-blackball", 10,6.5)
fill_floor("fl-leaves",0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-greenbrown", i-1,line)
      elseif c=="c" then
         set_item("it-coin1", i-1,line)
      elseif c=="h" then
	 hammer(i-1,line)
	 set_stone("st-greenbrown_move", i-1,line)
      elseif c=="g" then
	 set_stone("st-grate2", i-1,line)
      elseif c =="1" then                                                               
         set_item("it-wormhole",i-1,line, {targetx="16.5",targety="18.5",strength=0})
         set_stone("st-greenbrown_move", i-1,line)
      elseif c =="2" then 
         set_stone("st-greenbrown_hole", i-1,line)
         set_item("it-wormhole",i-1,line, {targetx="10",targety="30.5",strength=0})
      elseif c =="3" then                                                               
         set_item("it-wormhole",i-1,line, {targetx="16.5",targety="6.5",strength=0})
      elseif c =="4" then                                                               
         set_stone("st-greenbrown_hole", i-1,line) 
         set_item("it-wormhole",i-1,line, {targetx="10",targety="6.5",strength=0})
      elseif c =="5" then                                                               
         set_item("it-wormhole",i-1,line, {targetx="10",targety="18.5",strength=0})
      elseif c =="6" then 
         set_stone("st-greenbrown_hole", i-1,line)                                                              
         set_item("it-wormhole",i-1,line, {targetx="16.5",targety="42.5",strength=0})
      elseif c =="7" then                                                               
         set_item("it-wormhole",i-1,line, {targetx="10",targety="42.5",strength=0})
      elseif c =="8" then                                                               
         set_stone("st-greenbrown_hole", i-1,line) 
         set_item("it-wormhole",i-1,line, {targetx="16.5",targety="30.5",strength=0})
      elseif c=="C" then
	 set_stone("st-coinslot",i-1,line)
      elseif c=="O" then
	 oxyd(i-1,line)         
        end
    end  
end
--               01234567890123456789
renderLine(00 , "####################")
renderLine(01 , "C                  #")
renderLine(02 , "# O              O #")
renderLine(03 , "#    gggggggggg    #")
renderLine(04 , "#    g        g    #")  
renderLine(05 , "#ccc g        g    #")
renderLine(06 , "#hcc g        g   12")
renderLine(07 , "#ccc g        g    #")
renderLine(08 , "#    g        g    #")
renderLine(09 , "#    gggggggggg    #")
renderLine(10 , "# O              O #")
renderLine(11 , "#                  #")
renderLine(12 , "####################") 
renderLine(13 , "#                  #")
renderLine(14 , "# O              O #")
renderLine(15 , "#                  #")
renderLine(16 , "#                  #")
renderLine(17 , "#                  #")  
renderLine(18 , "#                 34")
renderLine(19 , "#                  #")
renderLine(20 , "#                  #")
renderLine(21 , "#                  #")
renderLine(22 , "# O              O #")
renderLine(23 , "#                  #")
renderLine(24 , "####################")
renderLine(25 , "#                  #")
renderLine(26 , "# O              O #")
renderLine(27 , "#                  #")
renderLine(28 , "#                  #")
renderLine(29 , "#                  #")
renderLine(30 , "#                 56")
renderLine(31 , "#                  #")
renderLine(32 , "#                  #")
renderLine(33 , "#                  #")
renderLine(34 , "# O              O #")
renderLine(35 , "#                  #")
renderLine(36 , "####################")
renderLine(37 , "#                  #")
renderLine(38 , "# O              O #")
renderLine(39 , "#                  #")
renderLine(40 , "#                  #")
renderLine(41 , "#                  #")
renderLine(42 , "#                 78")  
renderLine(43 , "#                  #")
renderLine(44 , "#                  #")
renderLine(45 , "#                  #")
renderLine(46 , "# O              O #")
renderLine(47 , "#                  #")
renderLine(48 , "####################")
--               01234567890123456789

oxyd_shuffle()

