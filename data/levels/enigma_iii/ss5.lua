-- A level for Enigma
-- Name:	Emergency Exit
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

levelw = 39
levelh = 13
enigma.ConserveLevel = FALSE		-- dh
create_world(levelw, levelh)
oxyd_default_flavor = "c"
set_actor("ac-blackball", 10,6.5)

fill_floor("fl-water",0,1,1,13)
fill_floor("fl-concrete",1,0,19,13)
fill_floor("fl-abyss",20,0,19,13)


function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-blue-sand", i-1,line)
      elseif c =="C" then                                
         set_stone("st-stone_break", i-1,line) 
      elseif c =="A" then
         set_floor("fl-bridge", i-1,line,{type="x",name="bridge"})
         SendMessage("bridge", "close")        
      elseif c =="E" then
         set_stone("st-blocker", i-1,line,{name="exit"})            
      elseif c=="W" then
	 set_stone("st-window", i-1,line)    
      elseif c=="S" then
         set_stone("st-switch_black", i-1,line, {action="trigger", target="exit"})              
      elseif c=="T" then
         set_stone( "st-timer", i-1,line, {action="callback", target="breakdam"})      
      elseif c=="D" then
         document(i-1,line, "Flood Control Dam #3 seems rather unstable. Be quick or be dead.")                 
      elseif c=="O" then                                                               
	 oxyd(i-1,line)         
        end
    end  
end
--               012345678901234567890123456789012345678
renderLine(00 , "##################S###################T")
renderLine(01 , "C                #W#                  #")
renderLine(02 , "C                  #                  #")
renderLine(03 , "C                  #                  #")
renderLine(04 , "C                  #                  #")  
renderLine(05 , "C                  #                  #")
renderLine(06 , "CD                 EAAA               #")
renderLine(07 , "C                  #  AAA             #")
renderLine(08 , "C                  #    AAA           #")
renderLine(09 , "C                  #      AAA         #")
renderLine(10 , "C                  #        AAA       #")
renderLine(11 , "C                  #          AAAAAAAAO")
renderLine(12 , "###########O###########################") 
--               012345678901234567890123456789012345678

x=0

function breakdam()
x=x+1
if x==1 then 
draw_stones("st-stone_break",{0,1},{0,1},11)
elseif x==2 then 
  enigma.KillStone(0,1)
  enigma.KillStone(0,2)
  enigma.KillStone(0,3)
  enigma.KillStone(0,4)
  enigma.KillStone(0,5)
  enigma.KillStone(0,6)
  enigma.KillStone(0,7)
  enigma.KillStone(0,8)      
  enigma.KillStone(0,9)
  enigma.KillStone(0,10)
  enigma.KillStone(0,11)        
elseif x==3 then 
  draw_floor("fl-water",{1,1},{0,1},11)                                                
  draw_floor("fl-water",{2,1},{0,1},11)  
  draw_floor("fl-water",{3,1},{0,1},11)  
elseif x==4 then
  draw_floor("fl-water",{4,1},{0,1},11)  
  draw_floor("fl-water",{5,1},{0,1},11)  
  draw_floor("fl-water",{6,1},{0,1},11)  
  draw_floor("fl-water",{7,1},{0,1},11)  
  draw_floor("fl-water",{8,1},{0,1},11)  
elseif x==5 then
  draw_floor("fl-water",{9,1},{0,1},11)  
  draw_floor("fl-water",{10,1},{0,1},11)  
  draw_floor("fl-water",{11,1},{0,1},11)  
  draw_floor("fl-water",{12,1},{0,1},11)  
  draw_floor("fl-water",{13,1},{0,1},11)  
elseif x==6 then
  draw_floor("fl-water",{14,1},{0,1},11)  
  draw_floor("fl-water",{15,1},{0,1},11)  
  draw_floor("fl-water",{16,1},{0,1},11)  
  draw_floor("fl-water",{17,1},{0,1},11)  
  draw_floor("fl-water",{18,1},{0,1},11)  
  draw_floor("fl-water",{19,1},{0,1},11)  
elseif x==7 then
  draw_floor("fl-water",{20,1},{0,1},11)  
  draw_floor("fl-water",{21,1},{0,1},11)  
  draw_floor("fl-water",{22,1},{0,1},11)  
  draw_floor("fl-water",{23,1},{0,1},11)  
  draw_floor("fl-water",{24,1},{0,1},11)  
  draw_floor("fl-water",{25,1},{0,1},11)  
elseif x==8 then
  draw_floor("fl-water",{26,1},{0,1},11)  
  draw_floor("fl-water",{27,1},{0,1},11)  
  draw_floor("fl-water",{28,1},{0,1},11)  
  draw_floor("fl-water",{29,1},{0,1},11)  
  draw_floor("fl-water",{30,1},{0,1},11)  
  draw_floor("fl-water",{31,1},{0,1},11)  
  draw_floor("fl-water",{32,1},{0,1},11)  
elseif x==9 then
  draw_floor("fl-water",{33,1},{0,1},11)  
  draw_floor("fl-water",{34,1},{0,1},11)  
  draw_floor("fl-water",{35,1},{0,1},11)  
  draw_floor("fl-water",{36,1},{0,1},11)  
  draw_floor("fl-water",{37,1},{0,1},11)  
  draw_floor("fl-water",{38,1},{0,1},11)  
  draw_floor("fl-water",{39,1},{0,1},11)  
  end
end



















