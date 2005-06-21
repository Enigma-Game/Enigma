-- A level for Enigma
-- Name:	Where's the Money?
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- a little bit like ESPRIT #048

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "c"
set_actor("ac-whiteball", 10,9.5, {player=0, controllers=1})

fill_floor("fl-wood",0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "H" then
         set_stone("st-chameleon", i-1,line)
      elseif c =="-" then
         set_stone("st-chameleon", i-1,line) 
         set_item("it-coin4", i-1,line)      
      elseif c =="." then
         set_stone("st-chameleon", i-1,line)      
         if difficult==false then
            set_item("it-extralife", i-1,line) 
            end  
      elseif c ==":" then
         set_stone("st-chameleon", i-1,line)      
         if difficult==false then      
             set_item("it-coin4", i-1,line) 
         end                    
      elseif c =="W" then
         set_stone("st-actorimpulse", i-1,line)   
         set_floor("fl-abyss", i-1,line) 
      elseif c =="B" then
         set_stone("st-stone_break", i-1,line) 
         set_item("it-dynamite", i-1,line)         
      elseif c =="w" then
         set_stone("st-actorimpulse", i-1,line) 
         set_floor("fl-abyss", i-1,line)                    
      elseif c =="a" then
         set_stone("st-door_c", i-1,line,{name="doora"})    
         set_item("it-dynamite", i-1,line)
      elseif c =="b" then
         set_stone("st-door_c", i-1,line,{name="doorb"})        
	 set_item("it-dynamite", i-1,line)
      elseif c=="c" then                                 
	 set_item("it-coin4", i-1,line)                  
      elseif c=="A" then
	 set_floor("fl-abyss", i-1,line) 	      
      elseif c=="1" then
         set_stone("st-coinslot", i-1,line, {action="callback", target="s1"})   	
      elseif c=="2" then
         set_stone("st-coinslot", i-1,line, {action="callback", target="s2"})   	
      elseif c=="3" then
         set_stone("st-coinslot", i-1,line, {action="callback", target="s3"})   	
      elseif c=="4" then
         set_stone("st-coinslot", i-1,line, {action="callback", target="s4"})   	
      elseif c=="O" then                                                               
	 oxyd(i-1,line)  
         set_item("it-dynamite", i-1,line)
        end
    end  
end
--               01234567890123456789        
renderLine(00 , "WAWAWAWAWAWWAWAWAWAW")
renderLine(01 , "A-HHHHHHHHHHHHHHHH-A")
renderLine(02 , "WHAwAwAwAwAAwAwAwAHW")
renderLine(03 , "AHwBBBBBBBBBBBBBBwHA")
renderLine(04 , "W-AOBBOBBBBOBBBBBA-W")
renderLine(05 , "AHwBBBBBBBBBBBBBBwHA")
renderLine(06 , "WHABBBBOBBBBOBBBOAHW")
renderLine(07 , "AHwBBBBBBBBBBBBBBwHA")
renderLine(08 , "W.AAAAAAAabAAAAAAA.W")
renderLine(09 , "A:WAAAAAA  AAAAAAW:A")
renderLine(10 , "W-   c        c   -W")
renderLine(11 , "A- AA AAAAAAAA AA -A")
renderLine(12 , "WA1AA2AAAAAAAA3AA4AW")
--               01234567890123456789

function s1() s(1) end
function s2() s(2) end
function s3() s(3) end
function s4() s(4) end

local flag = {0,0,0,0,}

function s(num)
%flag[num] = 1-%flag[num]
if (%flag[1]+%flag[2]+%flag[3]+%flag[4]==4) then                     -- open, if all inserted  
   SendMessage("doora", "open") 
   SendMessage("doorb", "open")
elseif (%flag[1]+%flag[2]+%flag[3]+%flag[4]==0) then                 -- close, if money is out
   SendMessage("doora", "close") 
   SendMessage("doorb", "close")
  end
end
