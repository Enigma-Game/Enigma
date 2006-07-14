-- A level for Enigma
-- Name:	FIRE, FIRE!
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "c"
set_actor("ac-blackball", 8.5,6.5)
enigma.ConserveLevel=FALSE

fill_floor("fl-black", 0,0, levelw,levelh)
fill_items( "it-burnable", 0, 0, 20, 13)
enigma.KillItem(11,1) -- no burning water

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-blocker", i-1,line)
      elseif c =="I" then
         set_stone("st-stoneimpulse",i-1,line)
      elseif c =="W" then
         set_stone("st-wood", i-1,line)
      elseif c =="P" then
         set_stone("st-pull", i-1,line)
         set_item("it-extinguisher", i-1,line)         
      elseif c =="c" then
         set_item("it-crack3", i-1,line) 
      elseif c =="b" then
         set_item("it-brush", i-1,line)       
      elseif c =="f" then
         set_floor("fl-water", i-1,line)
      elseif c=="O" then                                                               
	 oxyd(i-1,line) 
      elseif c=="." then
	 set_item("it-burnable_ignited", i-1,line) 
        end
    end  
end
--               01234567890123456789
renderLine(00 , "####################")
renderLine(01 , "#O        cfc     O#")
renderLine(02 , "# .        c     . #")
renderLine(03 , "#                  #")
renderLine(04 , "#                  #")  
renderLine(05 , "#        WWW       #")
renderLine(06 , "#        WIP       #")
renderLine(07 , "#        WWW       #")
renderLine(08 , "#                  #")
renderLine(09 , "#                  #")
renderLine(10 , "# .              . #")
renderLine(11 , "#O        b       O#")
renderLine(12 , "####################") 
--               01234567890123456789

















