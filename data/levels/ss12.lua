-- A level for Enigma
-- Name:	Walls Of Sadness
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

levelw = 20
levelh = 13
create_world(levelw, levelh)

draw_checkerboard_floor( "fl-inverse", "fl-black", 0, 0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-death",i-1,line)
      elseif c=="h" then
         if (difficult) then -- hard
           hollow(i-1,line)
         end
      elseif c=="H" then
         if (not difficult) then -- easy
           hollow(i-1,line)
         end
      elseif c=="1" then
	 set_floor("fl-gradient",  i-1,  line, {type=3})
      elseif c=="2" then
	 set_floor("fl-gradient",  i-1,  line, {type=4})
      elseif c=="3" then
	 set_floor("fl-gradient",  i-1,  line, {type=1})
      elseif c=="4" then
	 set_floor("fl-gradient",  i-1,  line, {type=2})
      elseif c=="5" then
         set_floor("fl-gradient",  i-1,  line, {type=5})
      elseif c=="6" then
         set_floor("fl-gradient",  i-1,  line, {type=6})
      elseif c=="7" then
         set_floor("fl-gradient",  i-1,  line, {type=7})
      elseif c=="8" then
         set_floor("fl-gradient",  i-1,  line, {type=8})
      elseif c=="x" then
         set_actor("ac-whiteball-small", i,line+.5, {player=0, essential=1})
        end
    end
end
--              01234567890123456789
renderLine(00, "####################")
renderLine(01, "#533333333333333336#")
renderLine(02, "#1 x           x  2#")
renderLine(03, "#15336H 5336 H53362#")
renderLine(04, "#11##2  1##2  1##22#")
renderLine(05, "#11##2h 1##2 h1##22#")
renderLine(06, "#11##2  1##2  1##22#")
renderLine(07, "#11##2 h1##2h 1##22#")
renderLine(08, "#11##2  1##2  1##22#")
renderLine(09, "#17448 H7448H 74482#")
renderLine(10, "#1 x           x  2#")
renderLine(11, "#744444444444444448#")
renderLine(12, "####################")
--              01234567890123456789

