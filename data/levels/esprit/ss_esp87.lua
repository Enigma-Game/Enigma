-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #087

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "b" 
set_actor("ac-blackball",9.5,5.5)

fill_floor("fl-abyss",0,0,levelw,levelh)
fill_floor("fl-woven",7,1,5,11)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
          if difficult then
              set_stone("st-death", i-1,line)
          else
              set_stone("st-greenbrown", i-1,line)
          end
      elseif c=="x" then
         set_actor("ac-top", i-.5,line+.5, {range=5, force=12})
      elseif c=="O" then
	 oxyd(i-1,line)
        end
    end  
end
--               01234567890123456789
renderLine(00 , "      #######       ")
renderLine(01 , "      #O   O#       ")
renderLine(02 , "      # #x# #       ")
renderLine(03 , "      #     #       ")
renderLine(04 , "      # # # #       ")  
renderLine(05 , "      #     #       ")
renderLine(06 , "      # # # #       ")
renderLine(07 , "      #     #       ")
renderLine(08 , "      # # # #       ")
renderLine(09 , "      #     #       ")
renderLine(10 , "      # #x# #       ")
renderLine(11 , "      #O   O#       ")
renderLine(12 , "      #######       ")
--               01234567890123456789

oxyd_shuffle()









