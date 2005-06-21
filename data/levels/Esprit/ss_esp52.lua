-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #052

levelw = 39
levelh = 25
create_world(levelw, levelh)
oxyd_default_flavor = "b"
set_actor("ac-blackball", 9.5,9.5, {player=0})
fill_floor("fl-gravel",0,0,levelw,levelh)
display.SetFollowMode(display.FOLLOW_SCROLLING)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-rock6", i-1,line)
      elseif c=="g" then
         set_stone("st-grate3",i-1,line) 
      elseif c=="i" then
         set_stone("st-invisible",i-1,line)
      elseif c=="G" then
         set_stone("st-grate1",i-1,line)
      elseif c=="T" then
         set_item("it-trigger", i-1,line, {invisible=1,action="callback", target="s1"}) 
      elseif c=="X" then
         set_stone("st-door_c", i-1,line,{name="door"}) 
      elseif c=="d" then
         document(i-1,line, "Sunday, bloody Sunday!") 
      elseif c=="O" then
	 oxyd(i-1,line)         
        end
    end  
end
--               012345678901234567890123456789012345678
renderLine(00 , "#######################################")
renderLine(01 , "#            iiiiii#                  #")
renderLine(02 , "#          i iT    #                  #")
renderLine(03 , "#  gg  g   g g g   X     gg  g   g    #")
renderLine(04 , "# gggg gg  g g g   gi   gggg gg  g    #")  
renderLine(05 , "# g  g gg  g g  g g#    g  g gg  g    #")
renderLine(06 , "# g  g g g g g   gi#    g  g g g g    #")
renderLine(07 , "# g  g g  gg g   gi#    g  g g  gg    #")
renderLine(08 , "# gggg g  gg g   gi#    gggg g  gg    #")
renderLine(09 , "#  gg ig   g ggg gi#     gg ig   g    #")
renderLine(10 , "#          i       #                  #")
renderLine(11 , "O          i      d#                  #")
renderLine(12 , "############################GG#########") 
renderLine(13 , "#                                     #")
renderLine(14 , "#                                     #")
renderLine(15 , "#  gg  g  g g   g ggg     g   g   gig #")
renderLine(16 , "# g  g g  g gg  g g  g    g   g   g g #")
renderLine(17 , "# g    g  g gg  g g   g  g g   g g  g #")  
renderLine(18 , "#  gg  g  g g g g g   g  g g    g   g #")
renderLine(19 , "#    g g  g g  gg g   g ggggg   g   gi#")
renderLine(20 , "#ig  g g  gig  gg g  g  g   g   g     #")
renderLine(21 , "#  gg   gg  g   giggg ig     giig   O #")
renderLine(22 , "#                                     #")
renderLine(23 , "#                                     #")
renderLine(24 , "#######################################")
--               012345678901234567890123456789012345678

local day=enigma.date("%A")                               

function s1()                               
if %day == "Sunday" then
  enigma.SendMessage(enigma.GetNamedObject("door"), "open", nil)
  end
end                              
                               
oxyd_shuffle()
