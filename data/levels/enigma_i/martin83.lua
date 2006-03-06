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
         fakeoxyd(i-1,line, 1,1)
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
         set_floor("fl-water",i-1,line)
      end
    end
end

renderLine( 0, "                    ")
renderLine( 1, "                    ")
renderLine( 2, "      #####         ")
renderLine( 3, "      #---#         ")
renderLine( 4, "      #-++# ###     ")
renderLine( 5, "      #-+-# O.O     ")
renderLine( 6, "      ###-###.#     ")
renderLine( 7, "       ##----.#     ")
renderLine( 8, "       #---#--#     ")
renderLine( 9, "       #---####     ")
renderLine(10, "       #####        ")
renderLine(11, "                    ")
renderLine(12, "                    ")

set_actor("ac-blackball", 7.5, 4.5)















