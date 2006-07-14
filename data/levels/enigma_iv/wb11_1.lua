-- WB 11: Little Puzzles
-- (c) 2003 Jon Sneyers
-- Licensed under GPL v2.0 or above
-- 2003-12-20

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells[";"]=cell{floor="fl-tigris"}
cells[" "]=cell{floor="fl-samba"}
cells["/"]=cell{floor="fl-stone"}
cells[":"]=cell{floor="fl-leaves"}
cells["%"]=cell{floor="fl-sand"}
cells["w"]=cell{stone="st-wood"}
cells["W"]=cell{parent=cells[":"], stone={"st-wood"}}
cells["i"]=cell{stone="st-stoneimpulse"}
cells["I"]=cell{stone="st-stoneimpulse-hollow"}

cells["G"]=cell{stone="st-grate1"}
cells["#"]=cell{stone="st-rock6"}
cells["="]=cell{stone="st-rock1"}
cells["R"]=cell{stone="st-rock8"}
cells["B"]=cell{parent=cells[":"],stone="st-black4"}
cells["b"]=cell{parent=cells[":"],stone="st-white4"}
cells["@"]=cell{floor="fl-abyss"}
cells["."]=cell{floor="fl-water"}
cells["S"]=cell{floor="fl-space"}

cells["H"]=cell{stone="st-chameleon"}

cells["&"]=cell{{{oneway, EAST}}}
cells["%"]=cell{{{oneway, WEST}}}
cells["l"]=cell{{{oneway, SOUTH}}}
cells["j"]=cell{{{oneway, NORTH}}}

cells["V"]=cell{parent=cells[" "], stone="st-bolder-s"}
cells[">"]=cell{parent=cells[" "], stone="st-bolder-e"}
cells["<"]=cell{parent=cells[" "], stone="st-bolder-w"}
cells["^"]=cell{parent=cells[" "], stone="st-bolder-n"}

cells["t"]=cell{stone={face="st-door", attr={name="doorA", type="v"}}}
cells["T"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorA"}}}
cells["O"]=cell{actor={face="ac-blackball", attr={player=0}}}
cells["o"]=cell{actor={face="ac-whiteball", attr={player=1}}}
cells["0"]=oxyd
cells["y"]=cell{item= {face="it-yinyang"}}
cells["1"]=cell{{{document, "Oops... you're stuck...     Try again..."}}}
cells["2"]=cell{{{document, "Congratulations, you reached this side...      but can you get to the oxyds? "}}}
cells["3"]=cell{{{document, "Well done!"}}}


level = {      
--  012345678901234567890123456789012345678
   "=======================================",
   "= O ===            = www              =",
   "i       =====0==== =  w ....   ....   =",
   "iw====  &        = =  w......y......  =",
   "=    =  &   ===  = %w w.............  =",
   "= =  =  &   =T=  = % w  ...........   =",
   "= =jj=  ===== =  ===     .........    =",
   "= =           =    = ==== .......wwwww=",
   "=0===========i===  =SSSS=  .....   w  =",
   "===         =  0=  =SSSS    ...    w  =",
   "=   =l=l=l= =jl==  =SSSS=    .     w  =",
   "=   &  1  %        =0SSS=          w  =",
   "=ll==0===0==========================jj=",  -- _________________________
   "=   ........   =   % % & & & & % & %  =",
   "= w ...    . w = 0 =j=j=l=j=j=l=j=j=j==",
   "= ww. .    . 2 & = % & % % & & & % &  =",
   "= w . .    . w = = =j=l=l=j=l=j=j=j=l==",
   "=   ........   = = & & % & % & & & %  =",
   "==j==========l==j===================lj=",
   "0   3 t     %      =   =  ..0... ...  =",
   "==0   ============ = w   w...... ...  =",
   "0 =0  =  w ..... = =ww w w   ... .....=",
   "=  ==== wwww ...y= & w w     ... ...  =",
   "0o       w ..... = &   =   ..... ...  0",
   "==0==0=================================",
--  012345678901234567890123456789012345678
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()
oxyd_default_flavor = "a"

















