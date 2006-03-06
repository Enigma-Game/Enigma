-- A level for Enigma
-- Name:	The Builder
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "a"
set_actor("ac-blackball", 8.5,6.5)
enigma.ConserveLevel=FALSE

fill_floor("fl-himalaya", 0,0, levelw,levelh)
fill_floor("fl-leaves", 14,7, 4,3)
fill_items( "it-burnable", 1, 1, 18, 11)
enigma.KillItem(16,7)
enigma.KillItem(17,8)
enigma.KillItem(16,9)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-wood", i-1,line)
      elseif c =="P" then
         set_stone("st-pull", i-1,line)
      elseif c =="f" then
         set_item("it-burnable_ignited", i-1,line)
      elseif c=="O" then                                                               
	 oxyd(i-1,line) 
      elseif c=="." then
	 set_item("it-seed", i-1,line) 
      elseif c=="o" then
	 set_item("it-seed_nowood", i-1,line)
        end
    end  
end
--               01234567890123456789
renderLine(00 , "####################")
renderLine(01 , "#Of     #   #     O#")
renderLine(02 , "#f      ###    o   #")
renderLine(03 , "#  #  o       P    #")
renderLine(04 , "#                  #")  
renderLine(05 , "#      #           #")
renderLine(06 , "#               #. #")
renderLine(07 , "#      .    .      #")
renderLine(08 , "#           P      #")
renderLine(09 , "#       #          #")
renderLine(10 , "#       o .   #  o #")
renderLine(11 , "#O               .O#")
renderLine(12 , "####################") 
--               01234567890123456789
















