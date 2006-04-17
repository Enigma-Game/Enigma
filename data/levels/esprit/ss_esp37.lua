-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #037
-- change rotor with spermbird, if implemented

-- Apr.2006: Replaced the rotor with a ghost, and
--   changed the puzzle-mechanism to avoid a second
--   way to solve the puzzle -> Revision 2   Andreas

Require("levels/lib/andreas_ghosts.lua")

levelw = 20
levelh = 25
create_world(levelw, levelh)
oxyd_default_flavor = "c"       -- Default flavor for oxyd stones.
set_actor("ac-blackball", 11.5,8.5, {player=0})
--set_actor("ac-rotor", 1.5,9.5, {range=30, force=40}) 
ghosts_set_ghost(1,9,"ac-rotor",1,ghosts_direction_intelligent,
                    {range=0, force=40, gohome=FALSE})

--fill_floor("fl-sand",0,0,levelw,levelh)
ghosts_set_railarea(0,0,levelw,levelh,1,"fl-sand")

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-rock1", i-1,line)
      elseif c=="1" then
	 set_item("it-wormhole", i-1,line, {targetx="4.5", targety="17.5",strength=0})         
      elseif c=="2" then
	 set_item("it-wormhole", i-1,line, {targetx="1.5", targety="2.5",strength=0})   
      elseif c=="3" then
	 set_item("it-wormhole", i-1,line, {targetx="4.5", targety="13.5",strength=0})   
      elseif c=="5" then
         puzzle(i-1,line, 7)  
         set_item("it-trigger", i-1,line, {action="callback", target="s1"})
      elseif c=="A" then
         set_item("it-trigger", i-1,line, {action="callback", target="s2"})
      elseif c=="B" then
         set_item("it-trigger", i-1,line, {action="callback", target="s3"})
      elseif c=="C" then
         set_item("it-trigger", i-1,line, {action="callback", target="s4"})
      elseif c=="6" then
         puzzle(i-1,line, 4)  	
      elseif c=="7" then
         puzzle(i-1,line, 13)        
      elseif c=="8" then
         puzzle(i-1,line, 10)  
         --set_item("it-magicwand",i-1,line)
      elseif c=="." then
         set_stone("st-door_b", i-1,line, {type="v"})   
      elseif c=="O" then
	 oxyd(i-1,line)
        end
    end  
end
--               01234567890123456789
renderLine(00 , "####################")
renderLine(01 , "#1#             # O#")
renderLine(02 , "# # ##### # ### #  #")
renderLine(03 , "# # #   # #   # #..#")
renderLine(04 , "# # # # # # # #    #")  
renderLine(05 , "# # # # # # # #### #")
renderLine(06 , "# # # #   # #      #")
renderLine(07 , "# # # ####5A###### #")
renderLine(08 , "# # # 7   BC       #")
renderLine(09 , "#   # ##### #### # #")
renderLine(10 , "##### #O  . . O#6# #")
renderLine(11 , "#     #   . .  # # #")
renderLine(12 , "# ######### #### # #")

renderLine(13 , "# #2             # #")
renderLine(14 , "# ####### # #### # #")
renderLine(15 , "#       # # #O # # #")
renderLine(16 , "####### # # #  # # #")
renderLine(17 , "#    3# # # #..# # #")
renderLine(18 , "# ##### # # #    # #")
renderLine(19 , "# #     # # #  # # #")
renderLine(20 , "# # ### # # #### # #")
renderLine(21 , "# #       # #    # #")
renderLine(22 , "# # #######8# #### #")
renderLine(23 , "#           #      #")
renderLine(24 , "####################")
--               01234567890123456789


state_s1 = 1
state_s2 = 1
state_s3 = 1
state_s4 = 1

function msgthisdoor(x,y,msg)  
  SendMessage(enigma.GetStone(x,y), msg)
end

function checkmydoors()
  local msg
  if      (state_s1 == 0) and (state_s2 == 0)
      and (state_s3 == 0) and (state_s4 == 0) then
    msg = "open"
  else
    msg = "close"
  end
  msgthisdoor(17,3,msg)
  msgthisdoor(18,3,msg)
  msgthisdoor(10,10,msg)
  msgthisdoor(12,10,msg)
  msgthisdoor(10,11,msg)
  msgthisdoor(12,11,msg)
  msgthisdoor(13,17,msg)
  msgthisdoor(14,17,msg)
end

state_s1 = 1
function s1()
  state_s1 = 1 - state_s1
  checkmydoors()
end
state_s2 = 1
function s2()
  state_s2 = 1 - state_s2
  checkmydoors()
end
state_s3 = 1
function s3()
  state_s3 = 1 - state_s3
  checkmydoors()
end
state_s4 = 1
function s4()
  state_s4 = 1 - state_s4
  checkmydoors()
end

oxyd_shuffle()

ghosts_init(0,0)
if difficult then
  enigma.SlopeForce=20
end
