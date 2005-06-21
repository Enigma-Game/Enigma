-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #037
-- change rotor with spermbird, if implemented

levelw = 20
levelh = 25
create_world(levelw, levelh)
oxyd_default_flavor = "c"       -- Default flavor for oxyd stones.
set_actor("ac-blackball", 11.5,8.5)
set_actor("ac-rotor", 1.5,9.5, {range=30, force=40}) 

fill_floor("fl-sand",0,0,levelw,levelh)

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
      elseif c=="6" then
         puzzle(i-1,line, 4)  	
      elseif c=="7" then
         puzzle(i-1,line, 13)        
      elseif c=="8" then
         puzzle(i-1,line, 10)  
         set_item("it-magicwand",i-1,line)
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
renderLine(07 , "# # # ####5 ###### #")
renderLine(08 , "# # # 7            #")
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

x=2
function s1()
x=x-1
if x == 0 then           -- stones vanish if the initial activated trigger is released by cluster exploding
  enigma.KillStone(17,3)
  enigma.KillStone(18,3)
  enigma.KillStone(10,10)
  enigma.KillStone(12,10)
  enigma.KillStone(10,11)
  enigma.KillStone(12,11)
  enigma.KillStone(13,17)
  enigma.KillStone(14,17)
  end
end

oxyd_shuffle()