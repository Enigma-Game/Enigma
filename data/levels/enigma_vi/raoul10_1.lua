-- Sleeping Serpent, a level for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above 
-- I love Puzzlestones :-)

-- GENERAL --
levelw = 20
levelh = 15
create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
oxyd_default_flavor = "b"
enigma.WaterSinkSpeed=5
display.SetFollowMode(display.FOLLOW_SCROLLING)

-- DESIGN --
boden="fl-black"

fill_floor(boden, 0,0, level_width,level_height)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
  		if c=="x" then
			oxyd(i-1,line)
-- Serpent
  		elseif c=="a" then
set_stone("st-puzzle2-ns",i-1,line)
set_item("it-trigger",i-1,line,{action="callback", target="wasser", invisible=TRUE})
  		elseif c=="b" then
set_stone("st-puzzle2-ew",i-1,line)
set_item("it-trigger",i-1,line,{action="callback", target="wasser", invisible=TRUE})
  	
  		elseif c=="d" then
set_stone("st-puzzle2-sw",i-1,line)
set_item("it-trigger",i-1,line,{action="callback", target="wasser", invisible=TRUE})
  		elseif c=="e" then
set_stone("st-puzzle2-nw",i-1,line)
set_item("it-trigger",i-1,line,{action="callback", target="wasser", invisible=TRUE})
  		elseif c=="f" then
set_stone("st-puzzle2-ne",i-1,line)
set_item("it-trigger",i-1,line,{action="callback", target="wasser", invisible=TRUE})
  		elseif c=="g" then
set_stone("st-puzzle2-es",i-1,line)
set_item("it-trigger",i-1,line,{action="callback", target="wasser", invisible=TRUE})

  		elseif c=="h" then
set_stone("st-puzzle2-e",i-1,line)
set_item("it-trigger",i-1,line,{action="callback", target="wasser", invisible=TRUE})

-- Ring
  		elseif c=="u" then
set_stone("st-puzzle2-ns",i-1,line)
  		elseif c=="v" then
set_stone("st-puzzle2-ew",i-1,line)

  		elseif c=="p" then
set_stone("st-puzzle2-sw",i-1,line)
  		elseif c=="q" then
set_stone("st-puzzle2-nw",i-1,line)
  		elseif c=="r" then
set_stone("st-puzzle2-ne",i-1,line)
  		elseif c=="s" then
set_stone("st-puzzle2-es",i-1,line)

-- Wall
  		elseif c=="6" then
set_stone("st-puzzle-ns",i-1,line)
  		elseif c=="8" then
set_stone("st-puzzle-ew",i-1,line)

  		elseif c=="1" then
set_stone("st-puzzle-sw",i-1,line)
  		elseif c=="2" then
set_stone("st-puzzle-nw",i-1,line)
  		elseif c=="3" then
set_stone("st-puzzle-ne",i-1,line)
  		elseif c=="4" then
set_stone("st-puzzle-es",i-1,line)


		end
	end	
end

renderLine(00 , "48888888888888888881")
renderLine(01 , "6x                x6")
renderLine(02 , "6  gbbbbbbbbbbbbd  6")
renderLine(03 , "6  ax           fd 6")
renderLine(04 , "6  fbbbbbbbbbbd  a 6")
renderLine(05 , "6            xa  a 6")  
renderLine(06 , "6svp hbbbbbbbbe  a 6")  
renderLine(07 , "6u u             a 6")  
renderLine(08 , "6rvq hbbbbbbbbd  a 6")
renderLine(09 , "6            xa  a 6")
renderLine(10 , "6  gbbbbbbbbbbe  a 6")
renderLine(11 , "6  ax           ge 6")
renderLine(12 , "6  fbbbbbbbbbbbbe  6")
renderLine(13 , "6x                x6") 
renderLine(14 , "38888888888888888882")

oxyd_shuffle()

-- Special
set_item("it-document", 3, 7, {text="Don't wake it up !"})
set_actor("ac-blackball", 2.5,7.5, {player=0})                    

-- Code --
-- Serpent-water
don2=1;
function wasser()
 don2=don2+1;
 if don2==89 then
  fill_floor("fl-water", 0,0, level_width,level_height)
 end
end













