-- Ingrowing -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-01-13

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

blockmap={
   "~+~",
   "+0+",
   "~+~",
}

blocksx = 9
blocksy = 5
blockw, blockh = get_map_size(blockmap)

sahara=     cell{floor="fl-sahara"}
tigris=     cell{floor="fl-tigris"}
metal=      cell{floor="fl-metal"}
solidfloor= cell{{{checkerfloor,{sahara,tigris; sidex=blockw, sidey=blockh, offset=1}}}}
borderstone=cell{{{randomfloor,{cell{stone={face="st-rock2"}},cell{stone={face="st-rock5"}},0.15}}}}
marble=     cell{actor={"ac-blackball", {player=0}}}

oxydstones = {n=0}

cells={}
cells["~"]=abyss
cells["0"]=cell{{{solidfloor},{oxyd}}}
cells[" "]=solidfloor

function place_new_oxyd_stone(count)
   count = count or 1

   for _=1,count do
      --pick place for new oxyd
      local mox,moy,ox,oy = 0
      local key = ""
      local num = oxydstones.n+1

      if num>14 then
	 return
      end

      repeat
	 mox, moy=random(0,blocksx-1),random(0,blocksy-1)
	 key = getkey(mox, moy)
	 ox, oy=mox*blockw+1, moy*blockh+1
      until (not(oxydstones[key]))and(((mod(mox,2)==0)and(mod(moy,2)==0)))

      --well, place found. Now construct triggering function
      local funcn = "trigger_"..num
      func0 = "function() "..
	      "  if (oxydstones[\""..key.."\"] == 0) then "..
	      "    oxydstones[\""..key.."\"] = 1 "..
	      "    place_new_oxyd_stone(2) "..
	      "  end "..
	      "end "

      dostring(funcn.." = "..func0)

      --add oxyd and triggers
      cells["+"]=cell{parent=metal,item={face="it-trigger",attr={action="callback", target=funcn, invisible=1}}}
      draw_map(ox, oy, blockmap)

      oxydstones[key] = 0
      oxydstones.n = oxydstones.n+1
   end
end

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

create_world(blocksx*blockw+2, blocksy*blockh+2)
fill_world_func(solidfloor)
draw_border_func(borderstone)
marble(blockw+1,1)

place_new_oxyd_stone(1)
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
display.SetFollowMode(display.FOLLOW_SCROLLING)
















