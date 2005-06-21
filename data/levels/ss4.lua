-- A level for Enigma
-- Name:	Bank-O-Mat
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- a little bit like ESPRIT #026, much more difficult


levelw = 39
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "b"
set_actor("ac-blackball", 8.5,6.5)

fill_floor("fl-bumps",0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-fakeoxyd", i-1,line)
      elseif c =="a" then
         set_stone("st-door_c", i-1,line,{name="doora"})      
      elseif c =="b" then
         set_stone("st-door_c", i-1,line,{name="doorb"})         
      elseif c =="c" then
         set_stone("st-door_c", i-1,line,{name="doorc"})             
      elseif c=="D" then
	 set_stone("st-death", i-1,line)    
      elseif c=="." then
	 set_item("it-seed", i-1,line) 	      
      elseif c=="1" then
         set_stone("st-coinslot", i-1,line, {action="callback", target="s1"})   	
      elseif c=="2" then
         set_stone("st-coinslot", i-1,line, {action="callback", target="s2"})   	
      elseif c=="3" then
         set_stone("st-coinslot", i-1,line, {action="callback", target="s3"})   	
      elseif c=="4" then
         set_stone("st-coinslot", i-1,line, {action="callback", target="s4"})   	
      elseif c=="5" then
         set_stone("st-coinslot", i-1,line, {action="callback", target="s5"})     
      elseif c=="6" then
         set_stone("st-coinslot", i-1,line, {action="callback", target="s6"})
      elseif c=="7" then
         set_stone("st-coinslot", i-1,line, {action="callback", target="s7"})     
      elseif c=="8" then
         set_stone("st-coinslot", i-1,line, {action="callback", target="s8"})      
      elseif c=="G" then
         set_stone("st-glass1", i-1,line)     
      elseif c=="S" then
         set_stone("st-switch_black", i-1,line, {action="callback", target="money"})              
      elseif c=="O" then                                                               
	 oxyd(i-1,line)         
        end
    end  
end
--               012345678901234567890123456789012345678
renderLine(00 , "#######################################")
renderLine(01 , "#                GS#                  #")
renderLine(02 , "#    1.2 3 4       #                  #")
renderLine(03 , "#                  #                  #")
renderLine(04 , "#                  #                  #")  
renderLine(05 , "#                  a   D              #")
renderLine(06 , "# O                b   D            O #")
renderLine(07 , "#                  c   D              #")
renderLine(08 , "#                  #                  #")
renderLine(09 , "#                  #                  #")
renderLine(10 , "#    5 6 7 8       #                  #")
renderLine(11 , "#                  #                  #")
renderLine(12 , "#######################################") 
--               012345678901234567890123456789012345678

function s1() s(1) end
function s2() s(2) end
function s3() s(3) end
function s4() s(4) end
function s5() s(5) end
function s6() s(6) end
function s7() s(7) end
function s8() s(8) end

local flag = {0,0,0,0,0,0,0,0}

function s(num)
%flag[num] = 1-%flag[num]
if (%flag[1]+%flag[2]+%flag[3]+%flag[4]+%flag[5]+%flag[6]+%flag[7]+%flag[8]==8) then     -- open, if all inserted  
   SendMessage("doora", "open") 
   SendMessage("doorb", "open")
   SendMessage("doorc", "open")   
elseif (%flag[1]+%flag[2]+%flag[3]+%flag[4]+%flag[5]+%flag[6]+%flag[7]+%flag[8]==0) then -- close, if money is out
   SendMessage("doora", "close") 
   SendMessage("doorb", "close")
   SendMessage("doorc", "close") 
  end
end

j = 0

function money()
    j = j+1
    if (j <= 8) then
        set_item("it-coin2",16, 1)
    else 
        document(16,1,"Sorry for inconvenience, the Bank-O-Mat is empty.")
    end
end




