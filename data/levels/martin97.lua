--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

dofile(enigma.FindDataFile("levels/ant.lua"))

cells={}

cells[" "]=cell{floor="fl-bluegray"}
cells["."]=cell{item="it-coin1"}
cells["D"]=cell{item={"it-document", {text="Take some money and build yourself the tools you need"}}}
cells["~"]=cell{floor="fl-water"}
cells[""]=cell{}
cells["#"]=cell{stone="st-glass"}
cells["*"]=cell{stone="st-glass_move"}
cells["/"]=cell{stone="st-mirror-p/"}
cells["v"]=cell{stone="st-mirror-3vt"}
cells["`"]=cell{stone="st-mirror-p\\"}
cells["-"]=cell{stone={"st-laser-w", {name="laser2"}}}
cells["l"]=cell{stone={"st-laser-n", {name="laser1"}}}
cells["M"]=cell{stone="st-wood"}
cells["$"]=cell{stone="st-death_invisible"}
cells["%"]=cell{stone="st-knight"}
cells["h"]=cell{stone="st-break_acblack"}
cells["t"]=cell{item={"it-trigger", {action="onoff", target="laser1"}}}
cells["T"]=cell{item={"it-trigger", {action="on", target="laser2"}}}

cells["o"]=cell{actor={"ac-blackball", {player=0, mouseforce=1}}}

level = {
   "",
   "",
   "",
   "",
   "",
   "",
   "",
   "",
   "",
   "",
   "",
   "/vvvvvvv` /vvvvvvv`                     ",
   "#########0###################0######### ",
   "#...             #`#~~~~~~~~~~~~~~~~  #-",
   "# o              ######      $      $M# ",
   "#                  *t #     %%%   $ $ # ",
   "#        0         ####     %0%   $ $ # ",
   "#                D #      $ %%%   $ $ # ",
   "#                  #          $   $ $ # ",
   "#  M               #   $    %%%   $ $ # ",
   "#        0         #        %0% $$$ $ # ",
   "#                  #        %%% $   $ # ",
   "#                  # $          $ $$$ # ",
   "#                  ~T   $   hhh $     # ",
   "#########0###################0######### ",
   "l                                       "   
}


set_default_parent(cells[" "])

create_world_by_map(level,cells)
                                                                                                                              
oxyd_shuffle()
