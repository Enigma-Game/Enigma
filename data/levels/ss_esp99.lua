-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #099

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "b" 
set_actor("ac-blackball",9.5,5.5)
set_actor("ac-rotor", 1.5,2.5, {range=25, force=33})
set_actor("ac-rotor", 18.5,2.5, {range=25, force=33}) 
enigma.AddConstantForce(0,30)
fill_floor("fl-floor_001",0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-invisible", i-1,line)
      elseif c=="b" then
         set_stone("st-actorimpulse_invisible", i-1,line)
      elseif c=="O" then
	 oxyd(i-1,line)
        end
    end  
end
--               01234567890123456789
renderLine(00 , "####################")
renderLine(01 , "####################")
renderLine(02 , "#     O      O     #")
renderLine(03 , "#  O            O  #")
renderLine(04 , "#                  #")  
renderLine(05 , "#      O    O      #")
renderLine(06 , "#                  #")
renderLine(07 , "#                  #")
renderLine(08 , "#  O            O  #")
renderLine(09 , "#                  #")
renderLine(10 , "#                  #")
renderLine(11 , "#bbbbbbbbbbbbbbbbbb#")
renderLine(12 , "####################")
--               01234567890123456789

oxyd_shuffle()