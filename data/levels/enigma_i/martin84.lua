--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)

oxyd_default_flavor = "b"

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c =="#" then
         set_floor("fl-samba",i-1,line)
         set_stone("st-death",i-1,line)
      elseif c == "O" then
         set_floor("fl-samba",i-1,line)
         oxyd( i-1, line)
      elseif c == "." then
         set_floor("fl-water",i-1,line)
      elseif c == "+" then
         set_floor("fl-samba",i-1,line)
         set_stone("st-wood",i-1,line)
      elseif c == "-" then
         set_floor("fl-samba",i-1,line)
      else
         set_floor("fl-abyss",i-1,line)
      end
    end
end

renderLine( 0, "        #O#         ")
renderLine( 1, "        #.#         ")
renderLine( 2, "        #-#         ")
renderLine( 3, "        #-#         ")
renderLine( 4, "        #-#         ")
renderLine( 5, "        #+##########")
renderLine( 6, "#########--+------.O")
renderLine( 7, "O.------+--#########")
renderLine( 8, "##########+#        ")
renderLine( 9, "         #-#        ")
renderLine(10, "         #-#        ")
renderLine(11, "         #.#        ")
renderLine(12, "         #O#        ")

oxyd_shuffle()

set_actor("ac-blackball", 10, 7)















