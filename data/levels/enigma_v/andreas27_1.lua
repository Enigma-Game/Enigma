-- Pipes and Plumbing, a level for Enigma
-- Copyright (C) 2006 Andreas Lochmann
-- Licensed under GPL v2.0 or above 
-- Created with the help of BBE 1.05
-- Thanks for the idea to Ronald Lamprecht, who tested
-- this level in Enigma and real life!

Require("levels/lib/ant.lua")

fl_floodable = "fl-rock"

function file_oxyd(x,y,f)
    oxyd_default_flavor=f
    oxyd(x,y)
end

function set_switch(x,y)
  set_stone("st-switch", x, y, {action="callback", target="switchcall"})
end

function set_switch_2(x,y)
  set_stone("st-switch", x, y, {action="callback", target="sensorcall"})
end

levelh=13
levelw=20

enigma.FlatForce=30
enigma.SlopeForce=0.4
enigma.ElectricForce=30

cells={}
stones={}

stones[" "]=cell{}
cells[" "]=cell{}

cells["!"]=cell{floor=fl_floodable}
cells["#"]=cell{floor="fl-gradient2"}
cells["$"]=cell{floor="fl-stone"}

stones["!"]=cell{stone="st-block"}
stones["#"]=cell{stone="st-mail-n"}
stones["$"]=cell{stone="st-grate1"}
stones["%"]=cell{parent={{set_switch}}}
stones["2"]=cell{parent={{set_switch_2}}}
stones["&"]=cell{stone="st-wood"}
stones["'"]=cell{parent={{file_oxyd,"b"}}}



level={"$$$$$$$$$$$$$$$$$$$$",
       "$!!!!!!!!!!!!!!!!$!$",
       "$!$!!!!!!!!!!!!!!$!$",
       "$#$!!!!!!!!!!!!!!$!$",
       "$#$!!!!!!!!!!!!!!$!$",
       "$#$!!!!!!!!!!!!!!$!$",
       "$#$!!!!!!!!!!!!!!$!$",
       "$#$!!!!!!!!!!!!!!$!$",
       "$#$!!!!!!!!!!!!!!$!$",
       "$#$!!!!!!!!!!!!!!$!$",
       "$#$!!!!!!!!!!!!!!$!$",
       "$#$!!!!!!!!!!!!!!!!$",
       "$$$$$$$$$$$$$$$$$$$$"}

stmap={"!!!!!!!!!!!!!!!!!!!!",
       "!                !&!",
       "!#!              !&!",
       "!$!              !&'",
       "!$!              !&!",
       "!$!              !&!",
       "!$!      $%      !&!",
       "!$!              !&!",
       "!$!              !&!",
       "!$!              !&'",
       "!$!              !&!",
       "!$2              &&!",
       "!!!!!!!!!!!!!!!!!!!!"}

create_world_by_map(level)
draw_map(0,0,stmap,stones)

set_item("it-banana",1,11)

oxyd_shuffle()

enigma.ConserveLevel = FALSE

set_actor("ac-blackball", 4, 4, {player=0})
set_actor("ac-whiteball", 1.5, 11.5, {player=1,mouseforce=1,7})

set_stone("st-timer", 0, 2, {action="callback", target="calltimer", interval=0.01})
set_item("it-sensor", 1, 3, {action="callback", target="sensorcall"})

pipes = {}
pipes[1] = "it-pipe-v"
pipes[2] = "it-pipe-h"
pipes[3] = "it-pipe-sw"
pipes[4] = "it-pipe-es"
pipes[5] = "it-pipe-ne"
pipes[6] = "it-pipe-wn"

for x = 0, levelw-1 do
  for y = 0, levelh-1 do
    enigma.SetAttrib(enigma.GetFloor(x,y), "towater", 0)
  end
end

function setwater(x,y)
  if enigma.GetKind(enigma.GetFloor(x,y)) == fl_floodable then
    enigma.SetAttrib(enigma.GetFloor(x,y), "towater", 1)
  end
end

function setwood(x,y)
  local st = enigma.GetStone(x,y)
  if st ~= nil then
    local k = enigma.GetKind(st)
    if (k == "st-wood") or (k == "st-wood1") or (k == "st-wood2") then
      enigma.SetAttrib(enigma.GetFloor(x,y), "towater", 1)
    end
  end
end

timercount = 0
function calltimer()
  local x, y, it, fl
  for x = 1, levelw - 2 do
    for y = 1, levelh - 2 do
      it = enigma.GetItem(x,y)
      if it ~= nil then
        if enigma.GetKind(it) == "it-banana" then
          set_item("it-burnable", x, y)
          set_floor("fl-water", x, y)
          enigma.SetAttrib(enigma.GetFloor(x,y), "towater", 0)
        end        
      end
      if enigma.GetAttrib(enigma.GetFloor(x,y), "towater") == 1 then
        set_floor("fl-water", x, y)
        enigma.SetAttrib(enigma.GetFloor(x,y), "towater", 0)
      end
    end
  end
  timercount = timercount + 1
  if timercount > 50 then
    timercount = 0
    for x = 1, levelw - 2 do
      for y = 1, levelh - 2 do
        fl = enigma.GetKind(enigma.GetFloor(x,y))
        if fl == "fl-water" then
          setwater(x-1,y)
          setwater(x+1,y)
          setwater(x,y+1)
          setwater(x,y-1)
          if random(1,3) == 1 then  setwater(x+1,y+1)  end
          if random(1,3) == 1 then  setwater(x+1,y-1)  end
          if random(1,3) == 1 then  setwater(x-1,y+1)  end
          if random(1,3) == 1 then  setwater(x-1,y-1)  end
        end
        if    (fl == "fl-stwood1") or (fl == "fl-stwood2")
           or (fl == "fl-stwood") then          
          setwood(x-1,y)
          setwood(x+1,y)
          setwood(x,y+1)
          setwood(x,y-1)
        end
      end
    end
  end
end

function switchcall()
  if difficult then
    set_item(pipes[random(1,6)], 9, 6)
  else
    set_item(pipes[random(1,5)], 9, 6)
  end
end

function sensorcall()
  enigma.SlopeForce = 10
end

