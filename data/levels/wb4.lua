-- WB 4: Bridge Builders
-- (c) 2003 Jon Sneyers
-- Licensed under GPL v2.0 or above
-- 2003-12-20

dofile(enigma.FindDataFile("levels/ant.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells[";"]=cell{floor="fl-tigris"}
cells[":"]=cell{floor="fl-samba"}
cells["/"]=cell{floor="fl-stone"}
cells[" "]=cell{floor="fl-leaves"}
cells["%"]=cell{floor="fl-sand"}
cells["w"]=cell{stone="st-wood"}
cells["i"]=cell{stone="st-stoneimpulse"}

cells["#"]=cell{stone="st-rock3"}
cells["="]=cell{stone="st-rock6"}
cells["W"]=cell{stone="st-white4"}
cells["B"]=cell{stone="st-black4"}
cells["."]=cell{floor="fl-abyss"}
cells["@"]=cell{floor="fl-water"}

cells["H"]=cell{stone="st-chameleon"}

cells["&"]=cell{{{oneway, EAST}}}

cells["V"]=cell{parent=cells[" "], stone="st-bolder-s"}
cells[">"]=cell{parent=cells[" "], stone="st-bolder-e"}
cells["<"]=cell{parent=cells[" "], stone="st-bolder-w"}
cells["^"]=cell{parent=cells[" "], stone="st-bolder-n"}

cells["A"]=cell{stone={face="st-door", attr={name="doorA", type="v"}}}
cells["a"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorA"}}}
cells["D"]=cell{stone={face="st-door", attr={name="doorB", type="v"}}}
cells["d"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorB"}}}
cells["C"]=cell{stone={face="st-door", attr={name="doorC", type="h"}}}
cells["c"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorC"}}}
cells["O"]=cell{actor={face="ac-blackball", attr={player=0}}}
cells["o"]=cell{actor={face="ac-whiteball", attr={player=1}}}
cells["0"]=oxyd
cells["y"]=cell{item= {face="it-yinyang"}}
cells["+"]=cell{{{document, "White helps black, black helps white"}}}

-- Diffyculty settings had no effect ('1' and '2' are unused)
-- commented out --ralf
--
--if difficult == false then
    --cells["1"]=cells[";"]
    --cells["2"]=cells["#"]
--else
    --cells["1"]=oxyd
    --cells["2"]=oxyd
--end

level = {
--  012345678901234567890123456789012345678
   "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
   "@%%%%%%%%%%@=0000====0000=@@%%%%%%%%%%@",
   "@%0   #  y%@=      0     =@@%y # #  0%@",
   "@% a ###  %@======C= =====@@%  #+#   %@",
   "@%    #   %WWA =   0 =   DBB%  ###   %@",
   "@%%%%% %%%%@@= = = = = ===@@%%%% %%%%%@",
   "@@@@@% %@@@@@=   =d=   =@@@@@@@% %@@@@@",
   "@%%%%%#%%%%%%====c=  ===%%%%%%%% %%%%%@",
   "@%### ####0##%@@=   ==%0  #          %@",
   "@%  # #   # %%BB####WW%%  ###########@@",
   "@%O #   #   %W w....w B%  #         o%@",
   "@%%%%%%%%#%%%W w....w B%%%%%%%%%%%%%%%@",
   "@@@@@@@@@@@@@###....###@@@@@@@@@@@@@@@@",
--  012345678901234567890123456789012345678
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor = "c"
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()

