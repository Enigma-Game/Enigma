-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #091

levelw = 39
levelh = 25
create_world(levelw, levelh)
oxyd_default_flavor = "a"
set_actor("ac-blackball", 35.5,19.5, {player=0})
display.SetFollowMode(display.FOLLOW_SCROLLING)
fill_floor("fl-floor_001",0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-stone1", i-1,line)
      elseif c=="W" then
         set_stone("st-greenbrown", i-1,line)
      elseif c=="M" then
         set_stone("st-greenbrown_move", i-1,line)
      elseif c=="X" then
         set_item("it-dynamite",i-1,line)
         set_stone("st-greenbrown_move", i-1,line)
      elseif c=="1" then
         set_stone("st-switch", i-1,line,{name="s1s", target="s1", action="callback"})
      elseif c=="2" then
         set_stone("st-switch", i-1,line,{target="s2", action="callback"})
      elseif c=="3" then
         set_stone("st-switch", i-1,line,{target="s3", action="callback"})
      elseif c=="4" then
         set_stone("st-switch", i-1,line,{name="s4s", target="s4", action="callback"})
      elseif c=="5" then
         set_stone("st-switch", i-1,line,{target="s5", action="callback"})
      elseif c=="6" then
         set_stone("st-switch", i-1,line,{target="s6", action="callback"})
      elseif c=="7" then
         set_stone("st-switch", i-1,line,{target="s7", action="callback"})
      elseif c=="8" then
         set_stone("st-switch", i-1,line,{target="s8", action="callback"})
      elseif c=="9" then
         set_stone("st-switch", i-1,line)
      elseif c=="A" then
         set_stone("st-switch", i-1,line,{target="sA", action="callback"})
      elseif c=="B" then
         set_stone("st-switch", i-1,line,{name="sBs", target="sB", action="callback"})
      elseif c=="C" then
         set_stone("st-switch", i-1,line,{target="sC", action="callback"})
      elseif c=="D" then
         set_stone("st-switch", i-1,line,{target="sD", action="callback"})
      elseif c=="E" then
         set_stone("st-switch", i-1,line,{target="sE", action="callback"})
      elseif c=="F" then
         set_stone("st-switch", i-1,line,{target="sF", action="callback"})
      elseif c=="0" then
         set_stone("st-switch", i-1,line,{name="s0s", target="s0", action="callback"})
      elseif c=="n" then
         set_stone("st-switch", i-1,line,{target="sn", action="callback"})
      elseif c=="Z" then
         set_stone("st-switch", i-1,line,{target="sZ", action="callback"})
      elseif c=="Y" then
         set_stone("st-switch", i-1,line,{target="sY", action="callback"})
      elseif c==">" then
         set_stone("st-oneway-e", i-1,line)
      elseif c=="_" then
         set_stone("st-oneway-s", i-1,line)
      elseif c=="a" then
         set_stone("st-oneway", i-1,line,{name="a", orientation=EAST})
      elseif c=="b" then
         set_stone("st-oneway", i-1,line,{name="b", orientation=EAST})
      elseif c=="f" then
         set_stone("st-oneway", i-1,line,{name="f", orientation=EAST})
      elseif c=="g" then
         set_stone("st-oneway", i-1,line,{name="g", orientation=EAST})
      elseif c=="h" then
         set_stone("st-oneway", i-1,line,{name="h", orientation=EAST})
      elseif c=="i" then
         set_stone("st-oneway", i-1,line,{name="i", orientation=EAST})
      elseif c=="m" then
         set_stone("st-oneway", i-1,line,{name="m", orientation=EAST})
      elseif c=="o" then
         set_stone("st-oneway", i-1,line,{name="o", orientation=EAST})
      elseif c=="p" then
         set_stone("st-oneway", i-1,line,{name="p", orientation=EAST})
      elseif c=="q" then
         set_stone("st-oneway", i-1,line,{name="q", orientation=EAST})
      elseif c=="r" then
         set_stone("st-oneway", i-1,line,{name="r", orientation=EAST})
      elseif c=="v" then
         set_stone("st-oneway", i-1,line,{name="v", orientation=EAST})
      elseif c=="w" then
         set_stone("st-oneway", i-1,line,{name="w", orientation=EAST})
      elseif c=="c" then
         set_stone("st-oneway", i-1,line,{name="c", orientation=SOUTH})
      elseif c=="d" then
         set_stone("st-oneway", i-1,line,{name="d", orientation=SOUTH})
      elseif c=="e" then
         set_stone("st-oneway", i-1,line,{name="e", orientation=SOUTH})
      elseif c=="j" then
         set_stone("st-oneway", i-1,line,{name="j", orientation=SOUTH})
      elseif c=="k" then
         set_stone("st-oneway", i-1,line,{name="k", orientation=SOUTH})
      elseif c=="l" then
         set_stone("st-oneway", i-1,line,{name="l", orientation=SOUTH})
      elseif c=="s" then
         set_stone("st-oneway", i-1,line,{name="s", orientation=SOUTH})
      elseif c=="t" then
         set_stone("st-oneway", i-1,line,{name="t", orientation=SOUTH})
      elseif c=="u" then
         set_stone("st-oneway", i-1,line,{name="u", orientation=SOUTH})
      elseif c=="O" then
	 oxyd(i-1,line)         
        end
    end  
end
--               012345678901234567890123456789012345678
renderLine(00 , "#######################################")
renderLine(01 , "#O    W1     W2    W3    W4     W     #")
renderLine(02 , "#     W      W     W     W      W     #")
renderLine(03 , "#     a      >     >     >      b     #")
renderLine(04 , "#     W      W     W     W      W     #")  
renderLine(05 , "#     W      W     W     X      W     #")
renderLine(06 , "#     WWWWcWWWWWdWWWWWeWMWWWW_WWW    Z#")
renderLine(07 , "#     W5     W6    W7    W8     W     #")
renderLine(08 , "#     W      X     M     W      W     #")
renderLine(09 , "#     f      g     h     i      >     #")
renderLine(10 , "#     W      W     W     W      W     #")
renderLine(11 , "#     W      W     W     W      W     #")
renderLine(12 , "#     WWMW_WWWWWjWWWWWkWWWWWWlWMW    n#") 
renderLine(13 , "#     W9     WA    WB    WC     W     #")
renderLine(14 , "#     W      M     W     W      W     #")
renderLine(15 , "#     m      o     p     q      r     #")
renderLine(16 , "#     W      W     W     W      W     #")
renderLine(17 , "#     W      W     W     W      W     #")  
renderLine(18 , "#     WWWWsWWWWW_MWWWWtWWWWWWuWWW    Y#")
renderLine(19 , "#     WD     WE    WF    W0     W     #")
renderLine(20 , "#     W      W     W     W      W     #")
renderLine(21 , "#     >      >     v     w      >     #")
renderLine(22 , "#     W      W     W     W      W     #")
renderLine(23 , "#O    W      W     W     W      W     #")
renderLine(24 , "#######################################")
--               012345678901234567890123456789012345678

oxyd_shuffle()

--             - - | | | - - - - | | | - - - - - | | | - -
--             a b c d e f g h i j k l m o p q r s t u v w
local flags = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
--             1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2

--                1 4 B 0
local switches = {0,0,0,0}

function s1()
%switches[1] = 1 - %switches[1]
%flags[6] = 1 - %flags[6]
%flags[3] = 1 - %flags[3]
if %flags[6] == 0 then
	set_stone("st-oneway", 6,9,{orientation=EAST})
    else
	set_stone("st-oneway", 6,9,{orientation=WEST})
end
if %flags[3] == 0 then
	set_stone("st-oneway", 10,6,{orientation=SOUTH})
    else
	set_stone("st-oneway", 10,6,{orientation=NORTH})
end
end

function s2()
%flags[4] = 1 - %flags[4]
%flags[7] = 1 - %flags[7]
if %flags[7] == 0 then
	set_stone("st-oneway", 13,9,{orientation=EAST})
    else
	set_stone("st-oneway", 13,9,{orientation=WEST})
end
if %flags[4] == 0 then
	set_stone("st-oneway", 16,6,{orientation=SOUTH})
    else
	set_stone("st-oneway", 16,6,{orientation=NORTH})
end
end

function s3()
%flags[5] = 1 - %flags[5]
%flags[8] = 1 - %flags[8]
if %flags[8] == 0 then
	set_stone("st-oneway", 19,9,{orientation=EAST})
    else
	set_stone("st-oneway", 19,9,{orientation=WEST})
end
if %flags[5] == 0 then
	set_stone("st-oneway", 22,6,{orientation=SOUTH})
    else
	set_stone("st-oneway", 22,6,{orientation=NORTH})
end
end

function s4()
%switches[2] = 1 - %switches[2]
%flags[11] = 1 - %flags[11]
if %flags[11] == 0 then
	set_stone("st-oneway", 22,12,{orientation=SOUTH})
    else
	set_stone("st-oneway", 22,12,{orientation=NORTH})
end
end

function s5()
if %switches[1] == 0 then enigma.SendMessage( enigma.GetNamedObject("s1s"), "on", 1)  -- Schalter umlegen.
	else enigma.SendMessage( enigma.GetNamedObject("s1s"), "off", 0)
	end
s1()
end


function s6()
%flags[9] = 1 - %flags[9]
if %flags[9] == 0 then
	set_stone("st-oneway", 25,9,{orientation=EAST})
    else
	set_stone("st-oneway", 25,9,{orientation=WEST})
end
end

function s7()
%flags[1] = 1 - %flags[1]
if %flags[1] == 0 then
	set_stone("st-oneway", 6,3,{orientation=EAST})
    else
	set_stone("st-oneway", 6,3,{orientation=WEST})
end
end

function s8()
%flags[20] = 1 - %flags[20]
if %flags[20] == 0 then
	set_stone("st-oneway", 29,18,{orientation=SOUTH})
    else
	set_stone("st-oneway", 29,18,{orientation=NORTH})
end
end

function sA()
%flags[15] = 1 - %flags[15]
%flags[21] = 1 - %flags[21]
if %flags[15] == 0 then
	set_stone("st-oneway", 19,15,{orientation=EAST})
    else
	set_stone("st-oneway", 19,15,{orientation=WEST})
end
if %flags[21] == 0 then
	set_stone("st-oneway", 19,21,{orientation=EAST})
    else
	set_stone("st-oneway", 19,21,{orientation=WEST})
end
end

function sB()
%switches[3] = 1 - %switches[3]
%flags[10] = 1 - %flags[10]
if %flags[10] == 0 then
	set_stone("st-oneway", 16,12,{orientation=SOUTH})
    else
	set_stone("st-oneway", 16,12,{orientation=NORTH})
end
end

function sC()
if %switches[3] == 0 then enigma.SendMessage( enigma.GetNamedObject("sBs"), "on", 1)  -- Schalter umlegen.
	else enigma.SendMessage( enigma.GetNamedObject("sBs"), "off", 0)
	end
sB()	
%flags[17] = 1 - %flags[17]
if %flags[17] == 0 then
	set_stone("st-oneway", 32,15,{orientation=EAST})
    else
	set_stone("st-oneway", 32,15,{orientation=WEST})
end
end

function sD()
%flags[13] = 1 - %flags[13]
%flags[14] = 1 - %flags[14]
%flags[18] = 1 - %flags[18]
if %flags[13] == 0 then
	set_stone("st-oneway", 6,15,{orientation=EAST})
    else
	set_stone("st-oneway", 6,15,{orientation=WEST})
end
if %flags[18] == 0 then
	set_stone("st-oneway", 10,18,{orientation=SOUTH})
    else
	set_stone("st-oneway", 10,18,{orientation=NORTH})
end
if %flags[14] == 0 then
	set_stone("st-oneway", 13,15,{orientation=EAST})
    else
	set_stone("st-oneway", 13,15,{orientation=WEST})
end
end

function sE()
%flags[19] = 1 - %flags[19]
if %flags[19] == 0 then
	set_stone("st-oneway", 22,18,{orientation=SOUTH})
    else
	set_stone("st-oneway", 22,18,{orientation=NORTH})
end
end

function sF()
%flags[15] = 1 - %flags[15]
if %flags[15] == 0 then
	set_stone("st-oneway", 19,15,{orientation=EAST})
    else
	set_stone("st-oneway", 19,15,{orientation=WEST})
end
end

function s0()
%switches[4] = 1 - %switches[4]
%flags[16] = 1 - %flags[16]
%flags[20] = 1 - %flags[20]
%flags[19] = 1 - %flags[19]
%flags[22] = 1 - %flags[22]
if %flags[16] == 0 then
	set_stone("st-oneway", 25,15,{orientation=EAST})
    else
	set_stone("st-oneway", 25,15,{orientation=WEST})
end
if %flags[19] == 0 then
	set_stone("st-oneway", 22,18,{orientation=SOUTH})
    else
	set_stone("st-oneway", 22,18,{orientation=NORTH})
end
if %flags[22] == 0 then
	set_stone("st-oneway", 25,21,{orientation=EAST})
    else
	set_stone("st-oneway", 25,21,{orientation=WEST})
end
if %flags[20] == 0 then
	set_stone("st-oneway", 29,18,{orientation=SOUTH})
    else
	set_stone("st-oneway", 29,18,{orientation=NORTH})
end
end

function sZ()
%flags[2] = 1 - %flags[2]
if %flags[2] == 0 then
	set_stone("st-oneway", 32,3,{name="b", orientation=EAST})
    else
	set_stone("st-oneway", 32,3,{name="b", orientation=WEST})
end
end

function sn()
%flags[12] = 1 - %flags[12]
if %flags[12] == 0 then
	set_stone("st-oneway", 29,12,{orientation=SOUTH})
    else
	set_stone("st-oneway", 29,12,{orientation=NORTH})
end
end

function sY()
if %switches[2] == 0 then enigma.SendMessage( enigma.GetNamedObject("s4s"), "on", 1)  -- Schalter umlegen.
	else enigma.SendMessage( enigma.GetNamedObject("s4s"), "off", 0)
	end
if %switches[4] == 0 then enigma.SendMessage( enigma.GetNamedObject("s0s"), "on", 1)
	else enigma.SendMessage( enigma.GetNamedObject("s0s"), "off", 0)
	end	
s0()
s4()
end





