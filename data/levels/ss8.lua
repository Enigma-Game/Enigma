-- A level for Enigma
-- Name:	Bavarian Madness
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "d"
set_actor("ac-blackball", 10, 6.5)
draw_checkerboard_floor( "fl-inverse", "fl-ice_001", 0, 0,levelw,levelh)
function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_floor("fl-abyss",i-1,line)
      elseif c =="m" then                                                               
         set_item("it-landmine",i-1,line, {targetx="1.5",targety="8.5",strength=0})
      elseif c=="O" then
	 oxyd(i-1,line) 
        end
    end
end
--              01234567890123456789
renderLine(00, "####################")
renderLine(01, "####     m  m   ####")
renderLine(02, "##Om  m       m  O##")
renderLine(03, "##mm      m     m ##")
renderLine(04, "##        m   m   ##")  
renderLine(05, "# m    m           #")
renderLine(06, "#   m       m      #")
renderLine(07, "#      m  m  m  m  #")
renderLine(08, "## m              ##")
renderLine(09, "##m  m    m     m ##")
renderLine(10, "##Om      m      O##")
renderLine(11, "####   m    m   ####")
renderLine(12, "####################")
--              01234567890123456789
                              
oxyd_shuffle()
