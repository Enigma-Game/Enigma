-- Take the Earth, a level for Enigma
-- Copyright (C) 2006 Andreas Lochmann
-- Licensed under GPL v2.0 or above 

Require("levels/lib/andreas_ghosts.lua")

levelh=13
levelw=20

enigma.SlopeForce = 0.0
enigma.ConserveLevel = FALSE

create_world( levelw, levelh)

draw_border("st-break_acwhite") --"st-likeoxydb-open"
draw_stones("st-timeswitch", {0,0}, {0,1}, levelh,
  {action="callback", target="switchfunc"})
draw_stones("st-timeswitch", {0,0}, {1,0}, levelw-2,
  {action="callback", target="switchfunc"})
draw_stones("st-timeswitch", {levelw-3,0}, {0,1}, levelh,
  {action="callback", target="switchfunc"})
draw_stones("st-timeswitch", {0,levelh-1}, {1,0}, levelw-2,
  {action="callback", target="switchfunc"})

set_stone("st-door-h", levelw-2, 1, {name="door1"})
set_stone("st-door-h", levelw-2, 2, {name="door2"})
set_stone("st-door-h", levelw-2, 3, {name="door3"})
set_stone("st-door-h", levelw-2, 4, {name="door4"})
set_stone("st-door-h", levelw-2, 5, {name="door5"})
set_stone("st-door-h", levelw-2, 7, {name="door6"})
set_stone("st-door-h", levelw-2, 8, {name="door7"})
set_stone("st-door-h", levelw-2, 9, {name="door8"})
set_stone("st-door-h", levelw-2, 10, {name="door9"})
set_stone("st-door-h", levelw-2, 11, {name="door10"})

set_stone("st-timer", 0, levelh-1,
  {action="callback", target="timerfunc", interval=6.0})

ghosts_set_railarea(0,0,levelw,levelh,1,"fl-space")

set_actor("ac-top", 5.5, 5.5, {range=20, force=5, gohome=FALSE})
if difficult then
  set_actor("ac-top", 8.5, 4.5, {range=20, force=5, gohome=FALSE})
end

ghosts_set_ghost(10,10,"ac-blackball", 1, ghosts_direction_intelligent, {player=0})
ghosts_init(0,0)
ghosts_check_for_walls = 0

oxyd(levelw-2,0)
oxyd(levelw-2,levelh-1)
oxyd_shuffle()

timercount = 0
function switchfunc()
  timercount = 0
  local j
  for j = 1, 10 do
    SendMessage("door"..tostring(j), "close")
  end
end
function timerfunc()
  timercount = timercount + 1
  if timercount < 11 then
    SendMessage("door"..tostring(timercount), "open")
  end
  if timercount > 9 then  
    draw_stones("st-break_acwhite", {0,1}, {0,1}, levelh-2)
    draw_stones("st-break_acwhite", {1,0}, {1,0}, levelw-3)
    draw_stones("st-break_acwhite", {levelw-3,1}, {0,1}, levelh)
    draw_stones("st-break_acwhite", {1,levelh-1}, {1,0}, levelw-3)
    set_stone("st-grate2", levelw-3, 6)
  end
end


