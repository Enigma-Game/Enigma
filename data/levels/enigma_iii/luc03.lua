-- Coloured Doors, a level for Enigma
-- Copyright (C) 2005 Lukas Schueller
-- Licensed under GPL v2.0 or above  
-- Contact: Lucky_Luc@web.de

--define colours
RED = 0
GREEN = 1
YELLOW = 2
BLUE = 3
--------------------------------------------
--functions to open the doors
function open_doors_red()
  enigma.SendMessage(enigma.GetNamedObject("door_reda"),"openclose",nil)
  enigma.SendMessage(enigma.GetNamedObject("door_redb"),"openclose",nil)
  enigma.SendMessage(enigma.GetNamedObject("door_redc"),"openclose",nil)
end
function open_doors_green()
  enigma.SendMessage(enigma.GetNamedObject("door_greena"),"openclose",nil)
  enigma.SendMessage(enigma.GetNamedObject("door_greenb"),"openclose",nil)
  enigma.SendMessage(enigma.GetNamedObject("door_greenc"),"openclose",nil)
end
function open_doors_yellow()
  enigma.SendMessage(enigma.GetNamedObject("door_yellowa"),"openclose",nil)
  enigma.SendMessage(enigma.GetNamedObject("door_yellowb"),"openclose",nil)
  enigma.SendMessage(enigma.GetNamedObject("door_yellowc"),"openclose",nil)
end
function open_doors_blue()
  enigma.SendMessage(enigma.GetNamedObject("door_bluea"),"openclose",nil)
  enigma.SendMessage(enigma.GetNamedObject("door_blueb"),"openclose",nil)
  enigma.SendMessage(enigma.GetNamedObject("door_bluec"),"openclose",nil)
end
--------------------------------------------
--function to set a coloured field with trigger and oxyd stone
function set_base(color,x,y,setoxyd)
  if(setoxyd == nil) then
    setoxyd = 1
  end
  if(color == 0) then
    FLOORTILE = "fl-red"
    TRIGGERFUNCTION = "open_doors_red"
  end
  if(color == 1) then
    FLOORTILE = "fl-leaves"
    TRIGGERFUNCTION = "open_doors_green"
  end
  if(color == 2) then
    FLOORTILE = "fl-sahara"
    TRIGGERFUNCTION = "open_doors_yellow"
  end
  if(color == 3) then
    FLOORTILE = "fl-rough-blue"
    TRIGGERFUNCTION = "open_doors_blue"
  end
  draw_border("st-brick",x-1,y-1,6,5)
  fill_floor(FLOORTILE,x,y,4,3)
  if(setoxyd ~= 0) then
    oxyd(x+1,y+1)
  end
  set_item("it-trigger",x+2,y+1,{action="callback",target=TRIGGERFUNCTION})
end
------------------------------------------
--function to create coloured doors
function set_door(doorname,floorcolour,x,y,doortype)
  set_stone("st-door",x,y,{name=doorname,type=doortype})
  set_floor(floorcolour,x,y)
end
------------------------------------------
CreateWorld(20,13)
oxyd_default_flavor = "a"
------------------------------------------
--environment
draw_border("st-brick")
fill_floor("fl-gray",0,0,20,13)
set_base(GREEN,3,1)
set_base(RED,8,1)
set_base(RED,13,1)
set_base(RED,3,5)
set_base(YELLOW,8,5,0)
set_base(GREEN,13,5)
set_base(BLUE,3,9)
set_base(YELLOW,8,9)
set_base(BLUE,13,9)
set_door("door_yellowa","fl-sahara",7,2,"v")
set_door("door_yellowb","fl-sahara",9,4,"h")
set_door("door_yellowc","fl-sahara",9,8,"h")
set_door("door_bluea","fl-rough-blue",12,2,"v")
set_door("door_blueb","fl-rough-blue",14,4,"h")
set_door("door_bluec","fl-rough-blue",4,8,"h")
set_door("door_reda","fl-red",4,4,"h")
set_door("door_redb","fl-red",12,6,"v")
set_door("door_redc","fl-red",7,10,"v")
set_door("door_greena","fl-leaves",7,6,"v")
set_door("door_greenb","fl-leaves",14,8,"h")
set_door("door_greenc","fl-leaves",12,10,"v")
oxyd_shuffle()
----------------------------------------
--set actors
set_item("it-yinyang",8,6)
set_item("it-yinyang",11,6)
set_actor("ac-blackball",8.5,6.5,{player=0})
set_actor("ac-whiteball",11.5,6.5,{player=1})





















