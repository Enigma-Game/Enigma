-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #092
-- if st-thief is fixed (stealing after delta t) please remove the comments of
--  set_floor("fl-gradient",  i-1,  line, {type=...


levelw = 20
levelh = 25
create_world(levelw, levelh)
oxyd_default_flavor = "b"       -- Default flavor for oxyd stones.
set_actor("ac-blackball", 9.5,5.5)

fill_floor("fl-rough-blue",0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-bluegray", i-1,line)
      elseif c=="h" then
         set_stone("st-bluegray_hole", i-1,line) 
         --set_floor("fl-gradient",  i-1,  line, {type=3})
      elseif c=="x" then
         set_stone("st-bluegray_hole", i-1,line) 
         --set_floor("fl-gradient",  i-1,  line, {type=4})
      elseif c=="T" then
         set_stone("st-thief", i-1,line)
      elseif c=="D" then
         set_stone("st-death", i-1,line) 
      elseif c=="k" then
	 set_item("it-key_c", i-1,line)
      elseif c=="a" then
         set_stone("st-bluegray_hole", i-1,line) 
	 set_item("it-key_a", i-1,line)
	 --set_floor("fl-gradient",  i-1,  line, {type=4})
      elseif c=="n" then
         set_stone("st-bluegray_hole", i-1,line) 
	 document(i-1,line,"Take the key to the left first!")
	 --set_floor("fl-gradient",  i-1,  line, {type=4})
      elseif c=="N" then
         set_stone("st-bluegray_hole", i-1,line) 
	document(i-1,line,"XYZZY")
	--set_floor("fl-gradient",  i-1,  line, {type=4})
      elseif c=="K" then
	 set_item("it-cherry", i-1,line)         
      elseif c=="E" then
	 set_item("it-extralife", i-1,line)   
      elseif c=="1" then
	 set_item("it-coin1", i-1,line)   
      elseif c=="2" then
         set_item("it-coin2", i-1,line)
      elseif c=="4" then
         set_item("it-coin4", i-1,line)
      elseif c=="S" then
         set_item("it-spade",i-1,line)
      elseif c=="P" then
         set_item("it-brush",i-1,line)
      elseif c=="f" then
         set_item("it-flagblack",i-1,line)
      elseif c=="F" then
         set_item("it-extinguisher",i-1,line)
      elseif c=="H" then
         set_item("it-hammer",i-1,line)
      elseif c=="A" then
         set_stone("st-key_a", i-1, line, {action="openclose", target="door"}) 
      elseif c=="|" then
         set_stone("st-door_b", i-1,line, {name="door", type="v"}) 
      elseif c=="-" then
         set_stone("st-door_b", i-1,line, {type="v"}) 
      elseif c=="c" then
         set_stone("st-stone_break", i-1,line) 
      elseif c=="O" then
	 oxyd(i-1,line)
        end
    end  
end
--               01234567890123456789
renderLine(00 , "####################")
renderLine(01 , "#      K    K      #")
renderLine(02 , "#  E k  1 O  1   1 #")
renderLine(03 , "#K              1  #")
renderLine(04 , "# 4    1   1      1#")  
renderLine(05 , "#E O            2  #")
renderLine(06 , "# S 4         K    #")
renderLine(07 , "#    P      fk  K  #")
renderLine(08 , "#  TaT  TnT  TNT   #")
renderLine(09 , "#  ThT  ThT  ThT  F#")
renderLine(10 , "#  TxT  TxT  TxT O #")
renderLine(11 , "# DThTH ThT  ThTD  #")
renderLine(12 , "####x###TxT###x#####")
renderLine(13 , "# DThT  ThT  ThTD  #")
renderLine(14 , "#  TxT  ThT  TxT   #")
renderLine(15 , "#  ThT  ThT  ThT   #")
renderLine(16 , "#  TxT  TxT  TxT   #")
renderLine(17 , "#  #-#  #A#  #|# 2 #")
renderLine(18 , "#    22        2   #")
renderLine(19 , "#  2        2      #")
renderLine(20 , "#        2 2       #")
renderLine(21 , "#  2 2  ccc     2  #")
renderLine(22 , "#       cOc  2   2 #")
renderLine(23 , "#       ccc        #")
renderLine(24 , "####################")
--               01234567890123456789

oxyd_shuffle()










