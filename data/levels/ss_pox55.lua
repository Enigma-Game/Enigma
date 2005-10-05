-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- Per.Oxyd Single #055

dofile(enigma.FindDataFile("levels/ant.lua"))
cells={}

cells[" "]=cell{floor="fl-rough"}
cells["s"]=cell{floor="fl-space"}
cells["#"]=cell{stone="st-marble"}
cells["r"]=cell{stone="st-rotator-left"}
cells["D"]=cell{parent=cells["s"],stone="st-death_invisible"}
cells["b"]=cell{parent=cells[" "],stone="st-blocker"}
cells["M"]=cell{{{mirrorp, TRUE, FALSE, 2}}}
cells["R"]=cell{parent=cells[" "],stone="st-rotator_move-left"}
cells["B"]=cell{stone={"st-bolder-e",  {name="bolder1"}}}
cells["c"]=cell{parent=cells[" "],stone="st-rock3_break"} 
cells["S"]=cell{stone={"st-switch", {action="onoff",  target="laser1"}}}
cells["t"]=cell{item= {"it-trigger",{action="callback", target="s1"}}}
cells["T"]=cell{item= {"it-trigger",{action="callback", target="s2"}}}
cells[">"]=cell{stone="st-oneway-e"}
cells["<"]=cell{stone="st-oneway-w"}
cells["L"]=cell{stone={"st-laser", {on=FALSE, dir=NORTH, name="laser1"}}}
cells["O"]=cell{oxyd}
cells["w"]=cell{parent=cells[" "],item="it-wrench"}
cells["d"]=cell{parent=cells[" "],item="it-floppy"}
cells["x"]=cell{actor={"ac-blackball",{player=0}}}
cells["X"]=cell{stone={"st-floppy", {action="callback", target="s3"}}}

level = {
--  012345678901234567890123456789012345678
   "ssssssssssssssssssssDDDDDDDDDDDDDDDDDDD",--00     
   "#r##################ssssssssssssssssssD",--01     
   "# B   x  B  w      RssssssssssssssssssD",--02     
   "# ################ #ssssssssssssssssssD",--03     
   "# # S         # d# #ssssssssssssssssssD",--04     
   "# < >    R R     # >ssssssssssssssssssD",--05     
   "# ### M   ###    # X##ssssssssssssssssD",--06     
   "# > >  #R #Tb  R # <O#ssssssssssssssssD",--07     
   "# #O#Bc   ### M  # ###ssssssssssssssssD",--08     
   "#t#############L## #ssssssssssssssssssD",--09     
   "r                  #ssssssssssssssssssD",--10     
   "##################r#ssssssssssssssssssD",--11     
   "ssssssssssssssssssssDDDDDDDDDDDDDDDDDDD" --12     
}   
set_default_parent(cells[" "])

create_world_by_map(level,cells)

local flag = {0,0,0}

function s1()
%flag[1] = 1 - %flag[1]
if %flag[1] == 0 then
    set_stone("st-oneway-w", 2,5)
    set_stone("st-oneway-e", 2,7)
 else
    set_stone("st-oneway-e", 2,5)
    set_stone("st-oneway-w", 2,7)  
end
end

function s2()
%flag[2] = 1 - %flag[2]
if %flag[2] == 0 then
    set_stone("st-oneway-e", 4,5)
    set_stone("st-oneway-e", 4,7)
 else
    set_stone("st-oneway-w", 4,5)
    set_stone("st-oneway-w", 4,7)  
end
end

function s3()
%flag[3] = 1 - %flag[3]
if %flag[3] == 0 then
    set_stone("st-oneway-e", 19,5)
    set_stone("st-oneway-w", 19,7)
 else
    set_stone("st-oneway-w", 19,5)
    set_stone("st-oneway-e", 19,7)  
end
end                                                             
                                 
                                 
