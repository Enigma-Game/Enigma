Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
cells={}
multiplayer_mode()
use_cells(cells, "1", "2")

cells[" "]=cell{floor="fl-sand"}
cells["#"]=cell{stone="st-metal"}
--cells["n"]=cell{stone="st-turnstile-n"}
cells["t"]=cell{stone="st-turnstile"}
cells["s"]=cell{stone="st-turnstile-s"}

stone1={}
stone2={}
actor1={}
actor2={}
cells["n"]=cell{{{add_multistone, "st-turnstile-n", stone1}}}
cells["N"]=cell{{{add_multistone, "st-turnstile-n", stone2}}}
cells["1"]=cell{{{add_multiobject, actor1, cells["1"]}}}
cells["2"]=cell{{{add_multiobject, actor2, cells["2"]}}}

level = {
   "####################",
   "#                  #",
   "#                  #",
   "#                  #",
   "#                  #",
   "#     1 n      N 2 #",
   "#       t      t   #",
   "#       s      s   #",
   "#                  #",
   "#                  #",
   "#                  #",
   "#                  #",
   "####################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

set_default_parent(" ")
oxyd_default_flavor = "d"
create_world_by_map(level)

add_rubber_band_pairs(actor1, stone1, 30, 0)
add_rubber_band_pairs(actor2, stone2, 300, 0)
--seltsames Verhalten bei 100000...

oxyd_shuffle()


















