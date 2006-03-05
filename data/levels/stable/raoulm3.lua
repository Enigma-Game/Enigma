-- A meditation level for Enigma
-- Copyright (C) 2006 Raoul
-- Licensed under GPL v2.0 or above 
-- extrem schwer :)

-- GENERAL --
levelw = 22
levelh = 16
create_world(levelw, levelh)
--display.SetFollowMode(display.FOLLOW_SCROLLING)
display.SetFollowMode(display.FOLLOW_SMOOTH)
oxyd_default_flavor = "a"
enigma.WaterSinkSpeed=200

-- DESIGN --
boden="fl-water"
wand="st-glass"

fill_floor(boden, 0,0, level_width,level_height)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_floor("fl-water", i-1, line)
  		elseif c=="x" then
			oxyd(i-1,line)

  		elseif c=="a" then
set_floor("fl-gradient1",i-1 ,line)
  		elseif c=="b" then
set_floor("fl-gradient2",i-1 ,line)
  		elseif c=="c" then
set_floor("fl-gradient3",i-1 ,line)
  		elseif c=="d" then
set_floor("fl-gradient4",i-1 ,line)

  		elseif c=="f" then
set_floor("fl-gradient5",i-1 ,line)
  		elseif c=="g" then
set_floor("fl-gradient6",i-1 ,line)
  		elseif c=="h" then
set_floor("fl-gradient7",i-1 ,line)
  		elseif c=="i" then
set_floor("fl-gradient8",i-1 ,line)

  		elseif c=="k" then
set_floor("fl-gradient9",i-1 ,line)
  		elseif c=="l" then
set_floor("fl-gradient10",i-1 ,line)
  		elseif c=="m" then
set_floor("fl-gradient11",i-1 ,line)
  		elseif c=="n" then
set_floor("fl-gradient12",i-1 ,line)

		end
	end	
end

if difficult then
renderLine(00 , "######################")
renderLine(01 , "#fgbbbbbbbbbbfg fgbi #")
renderLine(02 , "#hiaaaaaaaaaahi hiagl#")
renderLine(03 , "#dc          dc dc   #")
renderLine(04 , "#fgbbbbbbbfg dc fgbfg#")
renderLine(05 , "#hiaaaaaaahi dc hiahi#")
renderLine(06 , "#         dc dc    dc#")  
renderLine(07 , "#fgbbbbfg dc fgbbbbfg#")
renderLine(08 , "#hiaaaahi dc hiaaaahi#")
renderLine(09 , "#dc    dc dc         #")
renderLine(10 , "#fgbfg dc fgbbbbbbbfg#")
renderLine(11 , "#hiahi dc hiaaaaaaahi#")
renderLine(12 , "#   dc dc          dc#")
renderLine(13 , "#fgbfg fgbbbbbbbbbbfg#")
renderLine(14 , "#hiahi hiaaaaaaaaaahi#") 
renderLine(15 , "######################")
else
renderLine(00 , "######################")
renderLine(01 , "#nlaaaaaaaaaanl nlak #")
renderLine(02 , "#mkbbbbbbbbbbmk mkbln#")
renderLine(03 , "#cd          cd cd   #")
renderLine(04 , "#nlaaaaaaanl cd nlanl#")
renderLine(05 , "#mkbbbbbbbmk cd mkbmk#")
renderLine(06 , "#         cd cd    cd#")  
renderLine(07 , "#nlaaaanl cd nlaaaanl#")
renderLine(08 , "#mkbbbbmk cd mkbbbbmk#")
renderLine(09 , "#cd    cd cd         #")
renderLine(10 , "#nlanl cd nlaaaaaaanl#")
renderLine(11 , "#mkbmk cd mkbbbbbbbmk#")
renderLine(12 , "#   cd cd          cd#")
renderLine(13 , "#fganl nlaaaaaaaaaanl#")
renderLine(14 , "#hibmk mkbbbbbbbbbbmk#") 
renderLine(15 , "######################")
end

oxyd_shuffle()

-- Special
set_floor("fl-gray",20, 1)
set_item("it-hollow", 20, 1);
set_item("it-document", 10, 8, {text="just equilibrate..."})
set_item("it-flagwhite",1,13)

set_actor("ac-whiteball-small", 1,13, {player=0})

--So ist es:
--renderLine(00 , "######################")
--renderLine(01 , "#nbbbbbbbbbbbbl nbbi #")
--renderLine(02 , "#dfaaaaaaaaaagc dfagl#")
--renderLine(03 , "#dc          dc dc   #")
--renderLine(04 , "#dhbbbbbbbbl dc dhbbl#")
--renderLine(05 , "#maaaaaaaagc dc maagc#")
--renderLine(06 , "#         dc dc    dc#")  
--renderLine(07 , "#nbbbbbbl dc dhbbbbic#")
--renderLine(08 , "#dfaaaagc dc maaaaaak#")
--renderLine(09 , "#dc    dc dc         #")
--renderLine(10 , "#dhbbl dc dhbbbbbbbbl#")
--renderLine(11 , "#maagc dc maaaaaaaagc#")
--renderLine(12 , "#   dc dc          dc#")
--renderLine(13 , "#fgbic dhbbbbbbbbbbic#")
--renderLine(14 , "#hiaak maaaaaaaaaaaak#") 
--renderLine(15 , "######################")








