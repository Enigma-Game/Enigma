-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #032
-- problem: trigger under grates -> Killstone doesn't work, therefore fl-abyss

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "b"
set_actor("ac-blackball", 10,6.5)
fill_floor("fl-woven",0,0,levelw,levelh)
function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-fakeoxyd", i-1,line)
      elseif c=="." then                 
         set_item("it-extralife", i-1,line)
      elseif c=="G" then                 
         set_stone("st-grate2", i-1,line)
         set_floor("fl-abyss", i-1,line)
      elseif c=="g" then                 
         set_stone("st-grate2", i-1,line)
         set_floor("fl-black", i-1,line)         
      elseif c=="O" then
	 oxyd(i-1,line)
        end
    end  
end
--               01234567890123456789
renderLine(00 , "####################")
renderLine(01 , "# O  #  #..#  #  # #")
renderLine(02 , "# G  G .G. g  g  G #")
renderLine(03 , "# #  O  #  #  #  # #")
renderLine(04 , "# g  G  G  G  G  G #")  
renderLine(05 , "# #  #  O  #  #  # #")
renderLine(06 , "# G  G  G  G  G  g #")
renderLine(07 , "# #  #  #  O  #  # #")
renderLine(08 , "# G  G  G  G  G  G #")
renderLine(09 , "# #  #  #  #  O  # #")
renderLine(10 , "# G  g  g .G  G  G #")
renderLine(11 , "# #  #  #..#  # .O #")
renderLine(12 , "####################")
--               01234567890123456789
                     
oxyd_shuffle()
