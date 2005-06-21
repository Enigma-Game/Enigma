-- Unfair -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-04-25
Require("levels/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
cells={}
use_cells(cells, "O", "W")

cells[" "]=cell{floor="fl-metal"}
cells["#"]=cell{stone="st-rock2"}
cells["%"]=cell{stone="st-swap"}

cells[">"]=cell{{{gradient, FLAT_FORCE_E}}}
cells["<"]=cell{{{gradient, FLAT_FORCE_W}}}
cells["^"]=cell{{{gradient, FLAT_FORCE_N}}}
cells["v"]=cell{{{gradient, FLAT_FORCE_S}}}
cells["`"]=cells[">"]

cells["w"]=cell{{cells["W"], cells["`"]}}
cells["s"]=cell{item="it-spring1"}

cells["-"]=doorh

action_A  =cell{{{cells[">"], 1, 4}; mode=-1}}
cells["A"]=cell{item={"it-trigger", {action="callback", target="action_A"}}}

cells["B"]=cell{{{switch, "doorB", "openclose"}}}
cells["b"]=cell{{cells["`"], {doorv, {name="doorB"}}}}

success_C =function() send_message_named("doorC", "open", nil); end;
failed_C  =function() send_message_named("doorC", "close", nil); end;
states_C  =bool_table(3, 0, bool_and, success_C, failed_C)
action_C  =cell{{{bool_set, states_C}}}
cells["C"]=cell{item={"it-trigger", {action="callback", target="action_C"}}}
cells["c"]=cell{{{doorh, {name="doorC"}}}}

cells["E"]=cell{{{switch, "doorE", "close"}}}
cells["e"]=cell{{cells[" "], {doorv, {name="doorE"}}}}

slopes={}
pivots={}
cells["*"]=cell{{{add_multicell, slopes, 1}}}
cells["&"]=cell{{{add_multicell, pivots, slopes}, cells[" "]}}

cells["O"]=cell{{cells["O"], {document, "I don't believe that you are the Marble. I won't let you pass until you prove me wrong: you have to be faster than my champion!"}}}
cells["2"]=cell{actor={"ac-blackball", {player=1, mouseforce=0}}}
cells["+"]=cell{{{document, "Congratulations! You may pass!"}}}

level = {
   "...................................................................................................................",
   "...................................................................................................................",
   "#################......#########################################################################............#######",
   "#```#```````````########``````````````````#``````````>>`vBvvvvvv`<vvv`D```````^vvvvvvvvvvv`````##############*****#",
   "#2`````````````````````w``````````````````.`````````<^vvv#>>>>>>``>vvvv>>>>>>>```````w```````````````````.` E*  &*#",
   "#####``````````````#####``````````````````#````````````>>b^^`````#-#>>>v^^^^D`^^^^^^^^^^^^`````##############* 0 *#",
   "....################...###########################################s#############################            #*   *#",
   "#####              #####                            W W W# W   C #c# ..     #                  #      ======#* 0 *#",
   "#O  A                  W.                           W  W  W W  C     ..                       W.   %W =XX   e* + *#",
   "#   #           ########                            W W  # W   C     ..     #                  #      =W    #*****#",
   "#################......#########################################################################      =     #######",
   "...............................................................................................##############......",
   "..................................................................................................................."
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

set_default_parent(" ")
oxyd_default_flavor = "d"
create_world_by_map(level)

send_message_named("doorE", "open", nil)

spread_tag(pivots)
render_slopes(slopes, -1)

oxyd_shuffle()
enigma.ConserveLevel=FALSE
