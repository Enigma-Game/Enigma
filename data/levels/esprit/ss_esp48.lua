-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- a little bit like ESPRIT #048

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "c"
set_actor("ac-blackball", 10,8.5)

fill_floor("fl-light",0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-rock3", i-1,line)
      elseif c =="c" then
         set_item("it-coin4", i-1,line)   
      elseif c =="B" then
         set_stone("st-rock3_break", i-1,line) 
         set_item("it-dynamite", i-1,line)         
      elseif c=="A" then
	 set_floor("fl-abyss", i-1,line) 	      
      elseif c =="a" then
         set_stone("st-door_c", i-1,line,{name="doora"})    
         set_item("it-dynamite", i-1,line)
      elseif c =="b" then
         set_stone("st-door_c", i-1,line,{name="doorb"})        
	 set_item("it-dynamite", i-1,line)
      elseif c=="1" then
         set_stone("st-coinslot", i-1,line, {action="callback", target="s1"})   	
      elseif c=="2" then
         set_stone("st-coinslot", i-1,line, {action="callback", target="s2"})   	
      elseif c=="3" then
         set_stone("st-coinslot", i-1,line, {action="callback", target="s3"})   	
      elseif c=="O" then                                                               
	 oxyd(i-1,line)  
         set_item("it-dynamite", i-1,line)
        end
    end  
end
--               01234567890123456789        
renderLine(00 , "####################")
renderLine(01 , "#BBBBBBBBBBBBBBBBBB#")
renderLine(02 , "#BBOBBOBBBBOBBBBBBB#")
renderLine(03 , "#BBBBBBBBBBBBBBBBBB#")
renderLine(04 , "#BBBBBBOBBBBOBBBOBB#")
renderLine(05 , "#BBBBBBBBBBBBBBBBBB#")
renderLine(06 , "#AAAAAAAAabAAAAAAAA#")
renderLine(07 , "#AAAAAAAA  AAAAAAAA#")
renderLine(08 , "#AAAAAAAA  AAAAAAAA#")
renderLine(09 , "#AAAAAAAA  AAAAAAAA#")
renderLine(10 , "#c c c c c c c c c #")
renderLine(11 , "#AA AAAAA AAAAAA AA#")
renderLine(12 , "###1#####2######3###")
--               01234567890123456789

function s1() s(1) end
function s2() s(2) end
function s3() s(3) end

local flag = {0,0,0}

function s(num)
%flag[num] = 1-%flag[num]
if (%flag[1]+%flag[2]+%flag[3]==3) then -- open, if all inserted  
   SendMessage("doora", "open") 
   SendMessage("doorb", "open")
elseif (%flag[1]+%flag[2]+%flag[3]==0) then  -- close, if money is out
   SendMessage("doora", "close") 
   SendMessage("doorb", "close")
  end
end









