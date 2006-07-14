-- A level for Enigma
-- Name: 	Where is it?
-- Filename: 	barry02.lua
-- Copyright: 	(c) Dec 2003 Barry Mead
-- Contact: 	bmead15@cox.net
-- License: 	GPL v2.0
-- Written by:  Barry and Lori Mead
Require("levels/lib/ant.lua")
      cells={}
      cells[" "]=cell{floor="fl-leaves"}
      cells["~"]=cell{floor="fl-abyss"}
      cells["`"]=cell{floor="fl-space"}
      cells["G"]=cell{stone="st-grate1"}
      cells["b"]=cell{stone="st-stoneimpulse"}
      cells["B"]=cell{stone="st-stone_break"}
      cells["V"]=cell{stone="st-break_invisible"}
      cells["v"]=cell{stone="st-stonebrush"}
      cells["I"]=cell{stone={"st-door-v", {name="doorA"}}}
      cells["S"]=cell{item={"it-trigger", {action="openclose",  target="doorA"}}}
      cells["i"]=cell{stone={"st-door-v", {name="doorB"}}}
      cells["F"]=cell{stone={"st-floppy", {action="openclose", target="doorB"}}}
      cells["#"]=cell{stone="st-greenbrown"}
      cells["M"]=cell{stone="st-greenbrown_move"}
      cells["c"]=cell{item={"it-crack1", {fixed=1}}}
      cells["n"]=cell{item={"it-document", {text="Paint Ahead!"}}}
      cells["H"]=cell{item="it-hammer"}
      cells["f"]=cell{item="it-abyss"}
      cells["p"]=cell{stone="st-bolder-n"}
      cells["T"]=cell{stone="st-greenbrown_hole"}
      cells["P"]=cell{item="it-brush"}

      level = {
        --01234567890123456789012345678901234567890
         "#######################################",--0
         "#0I          v    G#v     v 0    vvv#F#",--1
         "###vvvvvv    v    G#v v   v    v    # #",--2
         "#    v  v v  v    G#v vv  vvvvvvvvv # #",--3
         "#         v  v    GM   v            # #",--4
         "#vvvvv vvvvvvv    G#vvvv            # #",--5
         "#       v   v     G######T########### #",--6
         "#vvvvvv v v vvvvvvG#   v    0~~       #",--7
         "#         v       G# v v  ccc~~     ###",--8
         "#vvvvvvvvvvvvVvvvvv# v      0~~     iH#",--9
         "######vV Vvvv   V0G# ###########vv#####",--10
         "###v  V v   V   VvG#       fffffffffff#",--11
         "####B###############T##################",--12
         "#S             0# P#  v              0#",--13
         "#              G#  #  v    v      v   #",--14
         "#              G#  #  v    v      vvvv#",--15
         "#     vv  vv   GM  # vvvvvvv          #",--16
         "#     v    v   G#  #       v    vvvvvv#",--17
         "#vvvvvvv vvv   G#  #            v  O  #",--18
         "b          v   G#  ######vvvvvv v     #",--19
         "b vvvvvvbvvv   G#  ## 0##       v     #",--20
         "#         b    G## ##MM G       v     #",--21
         "#         v    G## ## M #vvv    v     #",--22
         "#vvvvvvvv v    G## #  M #             #",--23
         "b         v    0## ##########M#########",--24
         "b #######b######M M    ```````````   0#",--25
         "#       n         pb   ```````````    #",--26
         "#bb####################################" --27
        --01234567890123456789012345678901234567890
      }

      oxyd_default_flavor = "a"
      set_default_parent(cells[" "])
      create_world_by_map(level)
      set_item("it-floppy", 22, 22)
      set_item("it-spring2", 16, 16)
      oxyd_shuffle()


















