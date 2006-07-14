-- WB 12: Big Adventures
-- (c) 2003 Jon Sneyers
-- Licensed under GPL v2.0 or above
-- 2003-12-20

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells[";"]=cell{floor="fl-tigris"}
cells[" "]=cell{floor="fl-samba"}
cells["/"]=cell{floor="fl-stone"}
cells["9"]=cell{floor="fl-leaves"}
cells["6"]=cell{floor="fl-sand"}
cells[":"]=cell{floor="fl-rough"}
cells["5"]=cell{floor="fl-swamp"}
cells["w"]=cell{stone="st-wood"}
cells["W"]=cell{parent=cells[":"], stone={"st-wood"}}
cells["x"]=cell{stone="st-stoneimpulse"}
cells["X"]=cell{stone="st-stoneimpulse-hollow"}

cells["#"]=cell{stone="st-grate1"}
cells["="]=cell{stone="st-rock1"}
cells["R"]=cell{stone="st-rock7"}
cells["r"]=cell{parent=cells[":"],stone="st-stone_break"}
cells["p"]=cell{item="it-hammer"}
cells["Q"]=cell{item="it-blackbomb"}
cells["+"]=cell{item="it-spring2"}

cells["."]=cell{floor="fl-abyss"}
cells["@"]=cell{floor="fl-water"}
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

cells["a"]=cell{stone={face="st-door", attr={name="doorA", type="v"}}}
cells["b"]=cell{stone={face="st-door", attr={name="doorB", type="v"}}}
cells["c"]=cell{stone={face="st-door", attr={name="doorC", type="v"}}}
cells["d"]=cell{stone={face="st-door", attr={name="doorD", type="h"}}}
cells["e"]=cell{stone={face="st-door", attr={name="doorE", type="h"}}}
cells["f"]=cell{stone={face="st-door", attr={name="doorF", type="h"}}}
cells["g"]=cell{stone={face="st-door", attr={name="doorG", type="h"}}}
cells["h"]=cell{stone={face="st-door", attr={name="doorH", type="v"}}}
cells["i"]=cell{stone={face="st-door", attr={name="doorI", type="h"}}}
cells["k"]=cell{stone={face="st-door", attr={name="doorK", type="v"}}}
cells["m"]=cell{stone={face="st-door", attr={name="doorM", type="v"}}}
cells["n"]=cell{stone={face="st-door", attr={name="doorN", type="v"}}}
cells["A"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorA"}}}
cells["B"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorB"}}}
cells["C"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorC"}}}
cells["D"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorD"}}}
cells["E"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorE"}}}
cells["F"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorF"}}}
cells["G"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorG"}}}
cells["H"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorH"}}}
cells["I"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorI"}}}
cells["K"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorK"}}}
cells["M"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorM"}}}
cells["N"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorN"}}}

cells["z"]=cell{stone={face="st-door", attr={name="doorZ", type="h"}}}
cells["Z"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorZ"}}}
cells["y"]=cell{stone={face="st-door", attr={name="doorY", type="h"}}}
cells["Y"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorY"}}}

cells["O"]=cell{actor={face="ac-blackball", attr={player=0}}}
cells["0"]=oxyd
cells["1"]=cell{{{document, "Too many wooden blocks!  Try the other way..."}}}
cells["2"]=cell{{{document, "Greetings, adventurer!    The road ahead is long and dangerous, so be careful!"}}}
cells["3"]=cell{{{document, "Well done, adventurer!    Good luck!"}}}
cells["4"]=cell{{{document, "Congratulations, adventurer!  You did it!"}}}


level = {      
--  0123456789012345678|012345678901234567|0123456789012345678
   "==========================================================",
   "=H@@@@@@@@W::rR6666&  =     =     =G= =SSSSS SSSSSSSSSS=K=",
   "=@@@@@@@@WW:rR666:@=  =     =  @  = = =SSSSS SSSSSSSSSS=S=",
   "=@@@@@@@WW:rR666:@@= w=w =  =  =  = = &  SSSwSSSSSS   S=S=",
   "=@@@@@@WW:rR666:@@@=  w  =     =  @ = =SSSSS SSSSSS   S=S=",
   "=@@@@@WW:rR666:@   =     =     =      =SSSSS SSSSSS   SSS=",
   "=@@@@WW:rR666:@@ @ ==============================SSSSSSSS=",
   "=@@@WW:rR666:@   @ %     @@@@@@@@@@@@@=         =SSSSSSSS=",
   "=@@WW:rR666:@@ @@@@=@   @@   @@ @@    = @@@@@@@@=SSSSSSSS=",
   "=@WW:rR666:@@@    @=@@ @@ + @@ + @@ + %w  w@w   =SSSSSSSS=",
   "=WW:rR666:@@@@@@@ @=@@@@   @@     @@  =@@ @@@ @@=======  =",
   "=W:rR666:@        @=@@@@@@@@@@@@@@@@@@=         =        =",
   "=::Rr666R=l====================================j====ll====",
   "=:::Rr66r=         =0=0===0=0=    2   = & % & & =      @ =",
   "=:::rR66R= w w w   =         =   === O=j=j=j=l=l=     w@ =",
   "=::WRr66r=  w w  = = 0=0=0=0 =   ====== & % % % =@@@   @w=",
   "=::Rr666R= w w w = =    4    =www== @@=j=j=l=l=j=E @@@@@@=",
   "=:WR666Rr=#@@=#@@= =0=0   0=0=      @@& & & & & =  @   @ =",
   "=:rR666R:=@@@#@@@= =====j=========  @@===========  @ w @1=",
   "=:R666R::=@#@#@#@= = k = =AM #=========F  @     =@@@ w @w=",
   "=:R666r::=@@@=@@@= =d=g=i=B      #### =  @@@ ww =@     @ =",
   "=:rR66rR:=@RR=@RR= =e= h =CN #=# #  # %  @@@  w =@   @@w =",
   "=:R666R::=   w   = =f==========#wwwww = @@@@ ww =@@@@@@w =",
   "=:R666r::=       =Y= a b c m n        =D @@@    %      ww=",
   "=:rR66RR:=ll==============================================",
   "=::r666r:=  =Z  3= & =SSS=   =www w ww=@@@@@@555==========",
   "=::R666R:=  == w = = =S=S= = = wQw wQ =  w  =555=55555====",
   "=::rR66r:=   ==y==z= =S=S=S= &w w w w & w w =  w&5555555==",
   "=::Rr66R:==        =S=S=SSS============  w  =555=55555555=",
   "=::rR66r::==========SSS=====xXXXXXXXXx=@@@@@@555===555555=",
   "=::R666R:::::::::::=====xxxxxxxxXxxxXx==============55555=",
   "=:Rr66rRrRrRrRrRrW:=   =<xxx   xXxXXXx=  555555===5555555=",
   "=:rR666rRr666666RrR= =       xXxXxXxxx= =5555555555555555=",
   "=:Rr666666666666666% =======x=XXXxXxXX% =5=555555555555===",
   "=::Rr66666rRrRr6666% =I    =<=>=xXXxXx===5================",
   "=::WRrRrRrR:::RrRrR===  =<x  =  xXXXXX=55555555555555555p=",
   "=========================xx==xxxxxxxxx===================="
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()
oxyd_default_flavor = "a"

















