-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #068

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "b"
set_actor("ac-blackball", 10,6.5)
fill_floor("fl-space",0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-metal", i-1,line)
      elseif c=="D" then
	 set_stone("st-death",i-1,line)
      elseif c=="F" then
	 set_floor("fl-metal",i-1,line)	 
      elseif c=="L" then
	 set_item("it-extralife",i-1,line)		 
      elseif c=="1" then                                                                
	 set_item("it-magnet",i-1,line,{name="magnet1", on=0})
	 set_floor("fl-metal",i-1,line)
      elseif c=="2" then                                                                
         set_item("it-magnet",i-1,line,{name="magnet2", on=0})
         set_floor("fl-metal",i-1,line)
      elseif c=="3" then                                                                
         set_item("it-magnet",i-1,line,{name="magnet3", on=0})
         set_floor("fl-metal",i-1,line)
      elseif c=="4" then                                                                
         set_item("it-magnet",i-1,line,{name="magnet4", on=0})   
         set_floor("fl-metal",i-1,line)
      elseif c=="a" then
	 set_stone("st-switch_black",i-1,line, {action="onoff", target="magnet1"})         
      elseif c=="b" then
	 set_stone("st-switch_black",i-1,line, {action="onoff", target="magnet2"})   
      elseif c=="c" then
	 set_stone("st-switch_black",i-1,line, {action="onoff", target="magnet3"})     	 
      elseif c=="d" then
         set_stone("st-switch_black",i-1,line, {action="onoff", target="magnet4"}) 
      elseif c=="O" then
	 oxyd(i-1,line)         
        end
    end  
end
--               01234567890123456789
renderLine(00 , "###O##O######O##O###")
renderLine(01 , "#1  FFFFFDDFFFFF  2#")
renderLine(02 , "b     F      F     d")
renderLine(03 , "#F    L           F#")
renderLine(04 , "#F                F#")  
renderLine(05 , "#F                F#")
renderLine(06 , "#D       FF       D#")
renderLine(07 , "#F                F#")
renderLine(08 , "#F                F#")
renderLine(09 , "#F           L    F#")
renderLine(10 , "a     F      F     c")
renderLine(11 , "#3  FFFFFDDFFFFF  4#")
renderLine(12 , "###O##O######O##O###") 
--               01234567890123456789

oxyd_shuffle()

















