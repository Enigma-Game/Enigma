-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #047
-- problem: killer should be a spermbird, which is seeking randomly for the marble

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "b" 
set_actor("ac-blackball",10.5,9.5, {player=0})
set_actor("ac-rotor", 10.5,3.5, {mouseforce=0, range=5, force=12}) 
fill_floor("fl-abyss",0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-rock4", i-1,line)
      elseif c=="f" then
         set_floor("fl-himalaya", i-1,line)
      elseif c=="F" then
         set_floor("fl-himalaya", i-1,line)
         set_item("it-extralife", i-1,line)
      elseif c=="O" then
	 oxyd(i-1,line)
        end
    end  
end
--               01234567890123456789
renderLine(00 , "#O########O######O##")
renderLine(01 , "Offffffffffffff  f #")
renderLine(02 , "#    f f      f  f #")
renderLine(03 , "#fffff f fff  f  f #")
renderLine(04 , "#f  f  f f f  f  f #")  
renderLine(05 , "#f  f  f f f  f  f #")
renderLine(06 , "#f  f  f O f  ffff #")
renderLine(07 , "#Fffffff   ffff  f #")
renderLine(08 , "#f  f  f   f  f  f #")
renderLine(09 , "#f  f  fffff  f  f #")
renderLine(10 , "#f  f         f  f #")
renderLine(11 , "#fffffffffffffFfff #")
renderLine(12 , "##########O#########")
--               01234567890123456789

oxyd_shuffle()