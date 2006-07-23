--<level height="13" width="20">
	--<info>
	--<name>Step gently!</name>
	--<author>Manuel Koenig</author>
	--<copyright>Copyright (C) 2006 Manuel Koenig</copyright>
	--<license>GPL v2.0 or above</license>
	--</info>

	--<option name="reset" value="NO" />
	--<option name="oxydflavor" value="a" />

	--<actors>
	--</actors>


--<lua>
--multiplayer_mode()

--Inserted by Raoul to get a LUA Level
--Remove this when code the Level to the "new" XML:
levelw = 20
levelh = 13
create_world(levelw, levelh)
--

fill_floor("fl-wood")
fill_floor("fl-abyss", 0, 11, 20, 2) 

draw_border("st-metal", 0, 0, 20, 11)
fill_stones("st-metal", 1, 10, 18, 1) -- delete this line, if display.ResizeGameArea (20,11) is set
fill_stones("st-metal", 9, 1, 2, 9) 

oxyd(0,4)
oxyd(5,10)
oxyd(9,1)
oxyd(10,9)
oxyd(14,0)
oxyd(19,7)

for n=1,72 do
	set_floor("fl-bridge", ((n-1)/9)+1 , mod((n-1),9)+1, {name="bridge"..n})
	set_item("it-trigger", ((n-1)/9)+11, mod((n-1),9)+1, {target="bridge"..n , action="openclose"})
end

set_floor("fl-acblack", 4, 5)
set_floor("fl-acwhite", 14, 5)
set_item("it-yinyang", 14, 5)
set_item("it-yinyang", 4, 5)
local ac1 = set_actor("ac-blackball", 4.5, 5.5, {player=0})
local ac2 = set_actor("ac-whiteball", 14.5,5.5, {player=1}) 
local ac3 = set_actor("ac-whiteball-small", 15.5, 5.5, {controllers=0})
AddRubberBand(ac2, ac3, 50, 0.2)
oxyd_shuffle()
--</lua>
--</level>


