--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
oxyd_default_flavor = "d"
fill_floor("fl-abyss")
fill_floor("fl-samba", 1, 3, 6, 7)
fill_floor("fl-samba",13, 3, 6, 7)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c =="w" then
         set_stone("st-wood",i-1,line)
      elseif c == "r" then
         set_stone("st-rock5",i-1,line)
      elseif c == "s" then
         set_stone("st-mail-s",i-1,line)
      elseif c == "n" then
         set_stone("st-mail-n",i-1,line)
      elseif c == "h" then
         set_item("it-pipe-h",i-1,line)
      elseif c == "v" then
         set_item("it-pipe-v",i-1,line)
      elseif c == "N" then
         set_item("it-pipe-ne",i-1,line)
      elseif c == "E" then
         set_item("it-pipe-es",i-1,line)
      elseif c == "S" then
         set_item("it-pipe-sw",i-1,line)
      elseif c == "W" then
         set_item("it-pipe-wn",i-1,line)
      elseif c == "d" then
         dynamite(i-1,line)
      elseif c == "B" then
         set_stone("st-bombs",i-1,line)
      elseif c == "o" then
         oxyd( i-1, line)
      elseif c == "i" then
         yy1( "white",  i-1, line)
      elseif c == "j" then
         yy1( "black",  i-1, line)
      elseif c == "x" then
         document(i-1,line,"Everything's been counted carefully!")
      elseif c == "y" then
         document(i-1,line,"Parts of the tubes disappear during detonation!")
      elseif c == "a" then
         set_actor("ac-blackball", i-.5,line+.5)
         set_item("it-yinyang", i-1, line+.5)
      elseif c == "b" then
         set_actor("ac-whiteball", i-.5,line+.5)
         set_item("it-yinyang", i-1, line+.5)	 
        end
    end
end

function yy1( color, x, y)
        stone = format( "st-%s1", color)
        set_stone( stone, x, y)
end

--              01234567890123456789
renderLine(00, "                    ")
renderLine(01, "                    ")
renderLine(02, "                    ")
renderLine(03, " ojrrjo      rnrrrr ")
renderLine(04, " jxd vj      ryEvSr ")
renderLine(05, " r  wvr      rdvvhr ")
renderLine(06, " rWwbwr      rwvwhr ")
renderLine(07, " rhhhhB      BNwahr ")
renderLine(08, " rNEh r      ihhhhi ")
renderLine(09, " rrrrsr      oirrio ")
renderLine(10, "                    ")
renderLine(11, "                    ")
renderLine(12, "                    ")
--              01234567890123456789

oxyd_shuffle()
