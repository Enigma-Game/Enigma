-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #038

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "b"
set_actor("ac-blackball", 10,6.5)

draw_checkerboard_floor( "fl-rough-blue", "fl-samba",0,0,levelw,levelh)
fill_floor("fl-abyss_fake",0,0,8,4)
fill_floor("fl-abyss_fake",12,0,8,4)
fill_floor("fl-abyss_fake",0,9,8,4)
fill_floor("fl-abyss_fake",12,9,8,4)
function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-rock1", i-1,line)
      elseif c=="G" then
	 set_stone("st-grate1", i-1,line)         
      elseif c=="H" then
	 set_stone("st-rock1_hole", i-1,line) 
      elseif c=="F" then
	 set_stone("st-fakeoxyd", i-1,line)   
      elseif c=="I" then
	 set_stone("st-invisible", i-1,line) 
      elseif c=="d" then
         document(i-1,line,"If you can read this message, you've cheated!")    	
      elseif c=="1" then
         set_item("it-trigger", i-1,line, {invisible=1,action="callback", target="s1"})     
      elseif c=="2" then
         set_item("it-trigger", i-1,line, {invisible=1,action="callback", target="s2"})                    
      elseif c=="O" then
	 oxyd(i-1,line)         
        end
    end  
end
--               01234567890123456789
renderLine(00 , "IIIIIIII####IIIIIIII")
renderLine(01 , "I   O   #GG#   O   I")
renderLine(02 , "I       #GG#       I")
renderLine(03 , "I       #GG#       I")
renderLine(04 , "#########GG#########")  
renderLine(05 , "#                  #")
renderLine(06 , "# O   O      O   O #")
renderLine(07 , "#                  #")
renderLine(08 , "#########GG#########")
renderLine(09 , "I       HGG#       I")
renderLine(10 , "I       #GG#       I")
renderLine(11 , "I2  F  1#GG#   F  dI")
renderLine(12 , "IIIIIIII####IIIIIIII") 
--               01234567890123456789

function s1()
set_stone("st-grate2", 4,4)
end

function s2()
set_stone("st-grate2", 15,4)    
end

oxyd_shuffle()







