-- Dustwalkers -- the Enigma Level
-- (c) 2002 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-10 -- keeping up to date with latest additions to ant.lua

dofile(enigma.FindDataFile("levels/ant.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
function tick()
   for _=1,3 do
      local bid = random(1,getn(bolders))
      local dir = random(0,3)
      SendMessage(bolders[bid],"direction",dir)
   end
end
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
cells[" "]=cell{floor={face="fl-sand"}}
cells["#"]=cell{stone={face="st-greenbrown"}}
cells["x"]=cell{stone={face="st-grate1"}}
cells["@"]=cell{stone={face="st-wood"}}
cells["O"]=cell{actor={face="ac-blackball", attr={player=0}}}
cells["A"]=cell{stone={face="st-door", attr={name="doorA", type="v"}}}
cells["B"]=cell{stone={face="st-door", attr={name="doorB", type="v"}}}
cells["C"]=cell{stone={face="st-door", attr={name="doorC", type="h"}}}
cells["D"]=cell{stone={face="st-door", attr={name="doorD", type="h"}}}
cells["a"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorA"}}}
cells["b"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorB"}}}
cells["c"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorC"}}}
cells["d"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorD"}}}
cells["0"]=oxyd
bolders={}
cells["V"]=cell{parent={{add_multistone, "st-bolder-s", bolders}}}
cells["<"]=cell{parent={{add_multistone, "st-bolder-w", bolders}}}
cells["^"]=cell{parent={{add_multistone, "st-bolder-n", bolders}}}
cells["~"]=cell{stone={face="st-timer", attr={action="callback", target="tick", interval=0.10}}}

level = {
   "####################",
   "#  #     V V V  #  #",
   "#  A    # # # # # O#",
   "#  #           <#  #",
   "#  #            ##x#",
   "#  #    @      <   #",
   "#  #       @       #",
   "#  #           <   #",
   "#  #     c#x#d     #",
   "#  #      x x  <   #",
   "#  ## # # # # # # ##",
   "#00# ^ ^  #C#  ^ ^ #",
   "~########## ########",
   "#00# V V  #D#  V V #",
   "#  ## # # # # # # ##",
   "#  #      x x      #",
   "#  #      #x# @    #",
   "#  # b             #",
   "#  #            @  #",
   "#  #               #",
   "#  #   a           #",
   "#  B               #",
   "#  #               #",
   "#  #  ^ ^ ^ ^ ^ ^  #",
   "####################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
randomseed(enigma.GetTicks())
set_default_parent(cells[" "])
oxyd_default_flavor = "b"
create_world_by_map(level)
oxyd_shuffle()
