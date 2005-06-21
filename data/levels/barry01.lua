-- A level for Enigma
-- Name: 	Solvable?
-- Filename: 	barry01.lua
-- Copyright: 	(c) Dec 2003 Barry Mead
-- Contact: 	bmead15@cox.net
-- License: 	GPL v2.0
-- Written by:  Barry and Lori Mead
      Require("levels/ant.lua")
      cells={}
      cells[" "]=cell{floor="fl-leaves"}
      cells["&"]=cell{floor="fl-abyss"}
      cells["#"]=cell{stone="st-greenbrown"}
      cells["b"]=cell{stone="st-stoneimpulse"}
      cells["k"]=cell{stone="st-brick"}
      cells["c"]=cell{stone="st-glass"}
      cells["I"]=cell{parent=cells[" "], stone="st-greenbrown_move", item="it-coin1"}
      cells["g"]=cell{parent=cells[" "], stone="st-grate1", item="it-seed"}
      cells["M"]=cell{stone="st-greenbrown_move"}
      cells["t"]=cell{item={"it-trigger", {action="callback", target="t1"}}}
      cells["G"]=cell{stone="st-grate1"}
      cells["K"]=cell{stone="st-brick_magic"}
      cells["L"]=cell{stone={"st-laser", {on=FALSE, dir=WEST, name="laser1"}}}
      cells["l"]=cell{stone={"st-switch", {action="onoff",  target="laser1"}}}
      cells["s"]=cell{item="it-spring1"}
      cells["w"]=cell{item="it-magicwand"}
      cells["d"]=cell{item={"it-document", {text="Make an umbrella."}}}
      cells["N"]=cell{item={"it-document", {text="Switches ABCD - C WANDS BE"}}}
      cells["n"]=cell{item={"it-document", {text="Touch the floor switch 5-times then impulse."}}}
      cells["~"]=cell{stone={"st-timer", {action="trigger", target="fart", interval=18}}}
      cells["*"]=cell{stone={"st-fart", {name="fart"}}}
      cells["-"]=cell{stone={"st-door-h", {name="door_wand"}}}
      cells["@"]=cell{stone={"st-door-h", {name="door_walkA"}}}
      cells["$"]=cell{stone={"st-door-h", {name="door_walkB"}}}
      cells["A"]=cell{stone={"st-fourswitch", {action="callback", target="sa"}}}
      cells["B"]=cell{stone={"st-fourswitch", {action="callback", target="sb"}}}
      cells["C"]=cell{stone={"st-fourswitch", {action="callback", target="sc"}}}
      cells["D"]=cell{stone={"st-fourswitch", {action="callback", target="sd"}}}
      cells["S"]=cell{stone="st-swap"}

      level = {
         "~##################*#####################",--0
         "# ABCD #W#  #w#    #           #0#    ###",--1
         "#      WsW  #n#    #           #@#    ###",--2
         "#      #W#  #-#    #                  ###",--3
         "#  0            0  #                  ###",--4
         "#                  #                  ###",--5
         "# d       O        #            t     # #",--6
         "#                  #                  IM#",--7
         "#              &&& #                  #b#",--8
         "#  0           &0& cGKk    L          #b#",--9
         "#              &&& M  K        #$#    #b#",--10
         "bS      W          #gKK        #0#   N#b#",--11
         "bb#########################l###########b#",--12
         "bbbbbbbbbbb bbbbbbbbbbbbbbbbbbbbbbbbbbbb#" --13
	--00000000001111111111222222222233333333334
	--01234567890123456789012345678901234567890
      }

      oxyd_default_flavor = "a"
      set_default_parent(cells[" "])
      create_world_by_map(level)
      oxyd_shuffle()
      
      local ff = {0,0,0,0,0}
      fc = 0

      function sa()
        %ff[1] = %ff[1] + 1
	if (%ff[1] >= 4) then %ff[1] = 0 end
	compute_gate()
      end

      function sb()
        %ff[2] = %ff[2] + 1
	if (%ff[2] >= 4) then %ff[2] = 0 end
	compute_gate()
      end

      function sc()
        %ff[3] = %ff[3] + 1
	if (%ff[3] >= 4) then %ff[3] = 0 end
	compute_gate()
      end

      function sd()
        %ff[4] = %ff[4] + 1
	if (%ff[4] >= 4) then %ff[4] = 0 end
	compute_gate()
      end

      function compute_gate()
        if (%ff[1] == 0) and (%ff[2] == 1) and (%ff[3] == 3) and (%ff[4] == 2) then
	  send_message_named("door_wand", "open", nil)
	else
	  send_message_named("door_wand", "close", nil)
	end
      end

      function t1()
	  fc = fc + 1
	  if (fc == 10) then
            set_stone("st-stoneimpulse",11,13)
	  end

	  %ff[5] = 1 - %ff[5]
	  if (%ff[5] == 1) then
	    send_message_named("door_walkA", "open", nil)
	    send_message_named("door_walkB", "open", nil)
	  else
	    send_message_named("door_walkA", "close", nil)
	    send_message_named("door_walkB", "close", nil)
	  end
      end
