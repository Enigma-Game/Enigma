-- Pneumatic Delivery
-- (c) 2004 Manuel König

Require("levels/ant.lua")
multiplayer_mode()

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
cells[" "]=cell{floor="fl-wood"}
cells["+"]=cell{item={"it-sensor", {action="callback", target="disable_conserve_level"}}}
cells["#"]=cell{stone="st-rock5"}
cells["A"]=cell{stone="st-black4"}
cells["a"]=cell{stone="st-white4"}
cells["B"]=cell{stone="st-mail-n"}
cells["b"]=cell{stone="st-mail-e"}
cells["C"]=cell{stone="st-mail-s"}
cells["c"]=cell{stone="st-turnstile", floor="fl-abyss"}
cells["D"]=cell{stone="st-turnstile-n", floor="fl-abyss"}
cells["d"]=cell{stone="st-turnstile-e", floor="fl-abyss"}
cells["E"]=cell{stone="st-turnstile-s", floor="fl-abyss"}
cells["e"]=cell{stone="st-turnstile-w", floor="fl-abyss"}
cells["F"]=cell{item="it-puller-n"}
cells["f"]=cell{item="it-puller-e"}
cells["G"]=cell{item="it-puller-s"}
cells["g"]=cell{item="it-puller-w"}
cells["H"]=cell{stone="st-stoneimpulse"}
cells["h"]=cell{stone={"st-fourswitch", {action="callback", target="mirrorswitch"}}}
cells["I"]=cell{stone="st-shogun-l"}
cells["i"]=cell{stone="st-shogun-m"}
cells["J"]=cell{stone="st-shogun-s", item={"it-shogun-l", {target="door2", action="openclose"}}}
cells["j"]=cell{stone="st-rotator_move-left"}
cells["k"]=cell{item={"it-document", {text="This is my first landscape, dedicated to my father. I hope you'll enjoy it!"}}}
cells["L"]=cell{stone={"st-laser-e", {name="laser", on=FALSE}}}
cells["l"]=cell{stone={"st-switch_white", {target="laser", on=FALSE, action="onoff"}}}
cells["N"]=cell{item="it-pipe-ne"}
cells["Q"]=cell{stone={"st-laserswitch", {action="callback", target="lasertrigger1"}}}
cells["q"]=cell{stone={"st-laserswitch", {action="callback", target="lasertrigger2"}}}
cells["R"]=cell{stone={"st-laserswitch", {action="callback", target="lasertrigger3"}}}
cells["r"]=cell{stone={"st-laserswitch", {action="callback", target="lasertrigger4"}}}
cells["S"]=cell{stone={"st-door", {type="v", name="door2"}}}
cells["s"]=cell{stone={"st-door", {type="h", name="door3"}}}
cells["T"]=cell{stone={"st-door", {type="v", name="door1"}}}
cells["t"]=cell{stone={"st-pmirror", {movable=0, transparent=0, orientation=2}}}
cells["U"]=cell{stone={"st-pmirror", {movable=0, transparent=1, orientation=4}}}
cells["u"]=cell{stone={"st-3mirror", {movable=0, transparent=0, orientation=1}}}
cells["V"]=cell{stone={"st-3mirror", {movable=0, transparent=0, orientation=2}}}
cells["v"]=cell{stone={"st-pmirror", {movable=1, transparent=0, orientation=1}}}
cells["w"]=cell{item={"it-trigger", {action="callback", target="trigger1"}}}
cells["X"]=cell{stone="st-grate2"}
cells["x"]=cell{stone="st-grate3"}
cells["Y"]=cell{stone={"st-pmirror", {movable=0, transparent=0, orientation=4, name="mirror1"}}}
cells["y"]=cell{stone={"st-switch_black", {action="callback", target="stonedoor"}}}
cells["Z"]=cell{item={"it-trigger", {action="callback", target="trigger2"}}}
cells["z"]=cell{item={"it-trigger", {action="callback", target="trigger3"}}}
cells["*"]=cell{stone={"st-door_c", {name="door4"}}}
cells[","]=cell{floor="fl-water"}
puzzles2 = {}
cells["?"]=cell{parent={{add_multicell, puzzles2}}}


if (difficult) then
cells["n"]=cell{item="it-pipe-sw"}
cells["P"]=cell{item="it-hammer"}
cells["p"]=cell{stone="st-stone_break"}
cells["K"]=cell{stone="st-knight"}
cells["$"]=cells["?"]
cells["m"]=cell{item="it-pipe-v"}
cells["M"]=cell{item="it-pipe-h"}
else
cells["n"]=cells[" "]
cells["P"]=cells[" "]
cells["p"]=cells[" "]
cells["K"]=cells[" "]
cells["m"]=cells[" "]
cells["M"]=cells[" "]
end



      level = {
      --	    1	      2		3
      --  012345678901234567890123456789012345678
         "#######################################",
         "# q      u  Q#    rxYx         #   #yM#",
         "#            #     xxx     ## W wW ##n#",
         "#L       U   X     x 1 v   # Wz Z#  Tm#",
         "#            #   $ A   ##### ### #  #k#",
         "# t      V  R#???? #  Fb..##     #  #K#",
         "#p############ ?N? #B###ecd#####C####0#",
         "#s## X #  ,,I# ?m? #  #  . .........###",
         "#mf# J H  j  X ????# 2# D. ec.. . ecd##",
         "#G###H #i# # X $   #  ..cN. . .ec. .  #",
         "#F#MPH   X # X     a    E+#ecdc ...#  #",
         "#0hM S   X   #     #l0#####.. EG ..##K#",
         "#####################################0#"
      --  012345678901234567890123456789012345678
      --	    1	      2		3
      }


--        -- [STONE DOOR] --
stonedoorswitchgreen=0
function stonedoor()
    if (stonedoorswitchgreen==0) then
      stonedoorswitchgreen=1
      set_stone("st-grate2", 31,1)
    elseif (stonedoorswitchgreen==1) then
      stonedoorswitchgreen=0
      set_stone("st-rock5", 31,1)
    end
end
--        -- [/STONE DOOR] --

--        -- [THREE TRIGGERS] --
local switchesdoor1={0, 0, 0}
function trigger1 () triggerdoor1(1) end 
function trigger2 () triggerdoor1(2) end 
function trigger3 () triggerdoor1(3) end
doorsopen1=0

function triggerdoor1(num)
   %switchesdoor1[num] = 1-%switchesdoor1[num]
   alldoor1=1
   for x=1,3 do
      if (%switchesdoor1[x]==0) then
         alldoor1 = 0
	end
   end
   if (doorsopen1==0 and alldoor1==1) then
      enigma.SendMessage(d1, "open", nil)
      doorsopen1 = 1
   elseif (doorsopen1==1 and alldoor1==0) then
      enigma.SendMessage(d1, "close", nil)
      doorsopen1 = 0
   end
end
--       -- [/THREE TRIGGERS] --

--        -- [FOURSWITCH] --
function mirrorswitch()
    mir=enigma.GetNamedObject("mirror1")
    enigma.SendMessage(mir, "turn", nil)
end
--       -- [/FOURSWITCH] --

--        -- [LASERSWITCHES] --
local switchesdoor3={0, 0, 0, 0}
function lasertrigger1 () triggerdoor3(1) end 
function lasertrigger2 () triggerdoor3(2) end 
function lasertrigger3 () triggerdoor3(3) end
function lasertrigger4 () triggerdoor3(4) end
doorsopen3=0

function triggerdoor3(num)
   %switchesdoor3[num] = 1-%switchesdoor3[num]
   allswitches3=1
   for x=1,4 do
      if (%switchesdoor3[x]==0) then
         allswitches3 = 0
	end
   end
   if (doorsopen3==0 and allswitches3==1) then
      enigma.SendMessage(d3, "open", nil)
      doorsopen3 = 1
   elseif (doorsopen3==1 and allswitches3==0) then
      enigma.SendMessage(d3, "close", nil)
      doorsopen3 = 0
   end
end
--       -- [/LASERSWITCHES] --

      oxyd_default_flavor = "d"
      set_default_parent(cells[" "])
      create_world_by_map(level)

      --       --  [PUZZLE] --
function scramble(x,y,d) enigma.AddScramble(x,y,d) end
enigma.SetScrambleIntensity(50)
scramble(14,5,"e")
scramble(15,5,"s")
scramble(18,8,"w")
scramble(17,8,"n")
--       -- [/PUZZLE] --

function disable_conserve_level ()
enigma.ConserveLevel = FALSE
end

d1=enigma.GetNamedObject("door1")
d3=enigma.GetNamedObject("door3")
render_puzzles(puzzles2, puzzle2)

fill_floor("fl-water", 14,5, 4, 1)
fill_floor("fl-water", 17,5, 1, 4)
set_floor("fl-water",18,8)

set_floor("fl-abyss",25,9)
set_floor("fl-abyss",25,10)
set_floor("fl-wood",27,8)
set_floor("fl-wood",34,8)
set_floor("fl-wood",36,8)

if (not difficult) then
set_item("it-pipe-sw",25,5)
set_item("it-pipe-v",25,6)
set_item("it-pipe-v",25,7)
set_item("it-pipe-h",26,9)
set_item("it-pipe-h",26,9)
set_item("it-pipe-h",3,10)
set_item("it-pipe-h",33,7)
set_item("it-pipe-v",37,1)
end

oxyd_shuffle()