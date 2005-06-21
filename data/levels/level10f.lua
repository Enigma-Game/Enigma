--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 37

create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
oxyd_default_flavor = "a"
fill_floor("fl-normal")

SetDefaultAttribs("st-rubberband", {strength=20})

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c =="#" then
         set_stone( "st-rubberband", i-1, line)
      elseif c == "o" then
         oxyd( i-1, line)
      elseif c == "d" then
         set_stone("st-death",i-1,line)
      elseif c == "z" then
         set_actor("ac-blackball", i-.5,line+.5)
      elseif c == "b" then
         set_stone("st-stone_break",i-1,line)
      elseif c == "g" then
         set_stone("st-door_a",i-1,line)
      elseif c=="h" then
         hammer(i-1,line)
        end
    end
end

renderLine(00, "####################")
renderLine(01, "#                  #")
renderLine(02, "#     bbb#bb       #")
renderLine(03, "#   # bbbbb    #   #")
renderLine(04, "#    bbbbbb        #")
renderLine(05, "#    bbbbb         #")
renderLine(06, "#   bbb       #    #")
renderLine(07, "#  dodb  #         #")
renderLine(08, "#  dddb            #")
renderLine(09, "#    bb            #")
renderLine(10, "#   #bb      dod   #")
renderLine(11, "# bbbbb      ddd   #")
renderLine(12, "#bb#bbb   #        #")
renderLine(13, "#bbbb          #   #")
renderLine(14, "#                # #")
renderLine(15, "#       #          #")
renderLine(16, "#                  #")
renderLine(17, "#  #               #")
renderLine(18, "#               dod#")
renderLine(19, "#           #  hddd#")
renderLine(20, "#                  #")
renderLine(21, "# dod  #       #   #")
renderLine(22, "# ddd              #")
renderLine(23, "#                  #")
renderLine(24, "#           #      #")
renderLine(25, "#     dod          #")
renderLine(26, "#     ddd      #   #")
renderLine(27, "#         #        #")
renderLine(28, "#     #       #    #")
renderLine(29, "#                  #")
renderLine(30, "#  #        #      #")
renderLine(31, "#                  #")
renderLine(32, "#                  #")
renderLine(33, "#          #       #")
renderLine(34, "# dod              #")
renderLine(35, "# ddd     z        #")
renderLine(36, "#########ggg########")

oxyd_shuffle()

enigma.AddConstantForce(0,50)
display.SetFollowMode(display.FOLLOW_SCROLLING)
