-- Enigma Level
-- Title : Beam0r
-- Author: Peter Santo
-- Email : enigma@exaweb.de
-- Date  : Oct 20, 2003
-- My first Enigma level. It is not too difficult, 
-- but I like finding solutions for laser beam puzzles.
-- This level is public domain

Require("levels/ant.lua")

oxyd_default_flavor = "b"

cells={}
cells[" "]=cell{floor = "fl-acblack"}
cells["!"]=cell{parent=cells[" "], stone="st-glass"}
cells["#"]=cell{parent={cells[" "], {oxyd}}}

cells["+"]=cell{parent=cells[" "], stone={"st-laser-e", {name="laser"}}}

cells["0"]=cell{parent=cells[" "], stone={"st-coinslot", {target = "laser", action = "onoff"}}}

cells["$"]=cell{parent=cells[" "], item={"it-coin1"}}

cells["a"]=cell{parent=cells[" "], stone={"st-pmirror", {name="mirror1"}}}
cells["b"]=cell{parent=cells[" "], stone={"st-pmirror", {name="mirror2"}}}
cells["c"]=cell{parent=cells[" "], stone={"st-pmirror", {name="mirror3"}}}
cells["d"]=cell{parent=cells[" "], stone={"st-pmirror", {name="mirror4"}}}
cells["e"]=cell{parent=cells[" "], stone={"st-pmirror", {name="mirror5"}}}
cells["f"]=cell{parent=cells[" "], stone={"st-pmirror", {name="mirror6"}}}
cells["g"]=cell{parent=cells[" "], stone={"st-pmirror", {name="mirror7"}}}
cells["h"]=cell{parent=cells[" "], stone={"st-pmirror", {name="mirror8"}}}
cells["i"]=cell{parent=cells[" "], stone={"st-pmirror", {name="mirror9"}}}
cells["j"]=cell{parent=cells[" "], stone={"st-pmirror", {name="mirror10"}}}
cells["k"]=cell{parent=cells[" "], stone={"st-pmirror", {name="mirror11"}}}

cells["A"]=cell{parent=cells[" "],item={"it-trigger", {action="turn", target="mirror1"}}}
cells["B"]=cell{parent=cells[" "],item={"it-trigger", {action="turn", target="mirror2"}}}
cells["C"]=cell{parent=cells[" "],item={"it-trigger", {action="turn", target="mirror3"}}}
cells["D"]=cell{parent=cells[" "],item={"it-trigger", {action="turn", target="mirror4"}}}
cells["E"]=cell{parent=cells[" "],item={"it-trigger", {action="turn", target="mirror5"}}}
cells["F"]=cell{parent=cells[" "],item={"it-trigger", {action="turn", target="mirror6"}}}
cells["G"]=cell{parent=cells[" "],item={"it-trigger", {action="turn", target="mirror7"}}}
cells["H"]=cell{parent=cells[" "],item={"it-trigger", {action="turn", target="mirror8"}}}
cells["I"]=cell{parent=cells[" "],item={"it-trigger", {action="turn", target="mirror9"}}}
cells["J"]=cell{parent=cells[" "],item={"it-trigger", {action="turn", target="mirror10"}}}
cells["K"]=cell{parent=cells[" "],item={"it-trigger", {action="turn", target="mirror11"}}}

cells["&"]=cell{parent=cells[" "], actor={"ac-blackball",{player=0}}}

level={"   #   #   #   #    ",
       "  #a   b   c   d#   ",
       "                    ",
       "  +    e   f   g    ",
       "                    ",
       "  #h   i   j   k#   ",
       "   #   #   #   #    ",
       "     !!!!!!!!!      ",
       "     !A$B$C$D!      ",
       "     !$$$$$$$!      ",
       "     0$$E&F$G!      ",
       "     !$$$$$$$!      ",
       "     !H$I$J$K!      "}
       
set_default_parent(cells["ÿ"])
create_world_by_map(level)
oxyd_shuffle()
