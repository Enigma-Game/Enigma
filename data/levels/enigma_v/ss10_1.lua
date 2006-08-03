-- A level for Enigma
-- Name:	Jumpin' Jack Flash
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

levelw = 20
levelh = 13
create_world(levelw, levelh)
set_actor("ac-blackball",2,3.5)
set_actor("ac-whiteball-small", 17.5,6.5,{player=0,controllers=1})
fill_floor("fl-rough",0,0,levelw,levelh)
local laser1 = laser(14,2, TRUE, EAST)
function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-greenbrown", i-1,line)
      elseif c=="h" then
         set_item("it-hollow", i-1,line,{essential=1})
      elseif c=="b" then
         if difficult==FALSE then
          set_item("it-spade", i-1,line)
         end
      elseif c=="G" then
         set_stone("st-white1", i-1,line)
      elseif c=="s" then
         set_stone("st-greenbrown_move", i-1,line)
         set_item("it-springboard", i-1,line)
      elseif c=="w" then
         set_stone("st-3mirror", i-1,line, {movable=1})
         set_item("it-spring2", i-1,line)
      elseif c=="y" then
         set_stone("st-yinyang1", i-1,line)
      elseif c=="g" then
         set_stone("st-greenbrown_hole", i-1,line)
         set_item ("it-document", i-1, line, {text="This is a meditation landscape: put a small marble into every hole..."})
      elseif c=="x" then
         set_stone("st-grate3", i-1,line)
      elseif c=="H" then
         set_stone("st-greenbrown_hole", i-1,line)
         set_item("it-hollow", i-1,line, {name="hollow", essential=1})
      elseif c=="S" then
         set_stone("st-switch", i-1,line,{action="callback", target="s1"})
      elseif c=="a" then
         set_floor("fl-abyss", i-1,line)
      elseif c=="c" then
         set_floor("fl-water", i-1,line)
      elseif c=="l" then
         set_floor("fl-leaves", i-1,line)
      elseif c=="O" then
	 oxyd(i-1,line)
        end
    end
end
--               01234567890123456789
renderLine(00 , "####################")
renderLine(01 , "#G aaacclccaaab   h#")
renderLine(02 , "#  aaacclccaaa xxxx#")
renderLine(03 , "#  aaacclccaaa     #")
renderLine(04 , "#  aaacclccaaah h  #")
renderLine(05 , "#  aaacclccaaa     #")
renderLine(06 , "# saaacclccaaa w   #")
renderLine(07 , "#  aaacclccaaa     #")
renderLine(08 , "#  aaacclccaaah h  #")
renderLine(09 , "#  aaacclccaaa     #")
renderLine(10 , "#  aaacclccaaa     #")
renderLine(11 , "#g aaacclccaaa    yS")
renderLine(12 , "####################")
--               01234567890123456789

function s1()
 SendMessage(laser1, "onoff")
end















