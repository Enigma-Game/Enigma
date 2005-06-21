-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #077

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "a"
set_actor("ac-blackball", 9.5, 2.5)
fill_floor("fl-abyss")

draw_floor("fl-samba",{1,1},{0,1},11)
draw_floor("fl-samba",{5,1},{0,1},11)
draw_floor("fl-samba",{9,1},{0,1},3)
draw_floor("fl-samba",{13,1},{0,1},11)
draw_floor("fl-samba",{17,1},{0,1},11)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-rock6",i-1,line)
      elseif c =="1" then                                                               
         set_item("it-wormhole",i-1,line, {targetx="5.5",targety="8.5",strength=0})
      elseif c =="2" then 
         set_item("it-wormhole",i-1,line, {targetx="9.5",targety="2.5",strength=0})
      elseif c =="3" then                                                               
         set_item("it-wormhole",i-1,line, {targetx="13.5",targety="8.5",strength=0})
      elseif c =="4" then                                                               
         set_item("it-wormhole",i-1,line, {targetx="17.5",targety="8.5",strength=0})
      elseif c =="5" then                                                               
         set_item("it-wormhole",i-1,line, {targetx="1.5",targety="8.5",strength=0})
      elseif c=="k" then
	 set_actor("ac-rotor", i-1+0.5,line+0.5, {mouseforce=1, range=6, force=20}) 
      elseif c=="O" then
	 oxyd(i-1,line) 
        end
    end
end
--              01234567890123456789
renderLine(00, "### ### ### ### ### ")
renderLine(01, "# # # # # # # # # # ")
renderLine(02, "# # #k# # # #k# # # ")
renderLine(03, "# # # # #3# # # # # ")
renderLine(04, "# # # # ### # # # # ")  
renderLine(05, "# # # #     # # # # ")
renderLine(06, "# # # #     # # # # ")
renderLine(07, "# # # #     # # # # ")
renderLine(08, "# # # #     # # # # ")
renderLine(09, "# # # #     # # # # ")
renderLine(10, "# # # #     # # # # ")
renderLine(11, "#1# #2#     #4# #5# ")
renderLine(12, "#O# #O#     #O# #O# ")
--              01234567890123456789
                              
oxyd_shuffle()
