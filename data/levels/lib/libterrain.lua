-- libterrain, a Library for Enigma
-- Copyright (C) 2006 Raoul
-- Licensed under GPL v2.0 or above
-- Version 1.0
-- Lua-lib zum einfachen erstellen von Bergen und Taelern.

----------------------------------------------------
--MOUNTAIN/VALLEY-RING:

function draw_ring(x0,y0,xw,yw,was)

--x0,y0: obere linke ecke
--xw,yw: breite, hoehe des rings
--was:entscheidet ob mountain- oder valleyring
--was=0: tal, was=1: berg

 --Machbarkeits-Test:
 if xw<2 or yw<2 then
  set_item("it-document",x0,y0,{text="ERROR: To small Values!"})
  return
 end

 if was==1 then
  --Ecken:
  set_floor("fl-gradient12",x0,y0)
  set_floor("fl-gradient10",x0+xw-1,y0)
  set_floor("fl-gradient11",x0,y0+yw-1)
  set_floor("fl-gradient9",x0+xw-1,y0+yw-1)
  --Kanten:
  draw_floor("fl-gradient2",{x0+1,y0},{1,0},xw-2)
  draw_floor("fl-gradient1",{x0+1,y0+yw-1},{1,0},xw-2)
  draw_floor("fl-gradient4",{x0,y0+1},{0,1},yw-2)
  draw_floor("fl-gradient3",{x0+xw-1,y0+1},{0,1},yw-2)

 else --was==0
  --Ecken:
  set_floor("fl-gradient5",x0,y0)
  set_floor("fl-gradient6",x0+xw-1,y0)
  set_floor("fl-gradient7",x0,y0+yw-1)
  set_floor("fl-gradient8",x0+xw-1,y0+yw-1)
  --Kanten:
  draw_floor("fl-gradient1",{x0+1,y0},{1,0},xw-2)
  draw_floor("fl-gradient2",{x0+1,y0+yw-1},{1,0},xw-2)
  draw_floor("fl-gradient3",{x0,y0+1},{0,1},yw-2)
  draw_floor("fl-gradient4",{x0+xw-1,y0+1},{0,1},yw-2)
 end

end
----------------------------------------------------
--MOUNTAIN/VALLEY-FULL

function draw_full(x0,y0,xw,yw,was)

 ok=TRUE
 xwm=xw
 ywm=yw
 x0m=x0
 y0m=y0

 while ok==TRUE do
  draw_ring(x0m,y0m,xwm,ywm,was)

  xwm=xwm-2
  ywm=ywm-2
  x0m=x0m+1
  y0m=y0m+1

  if xwm<2 or ywm<2 then
   ok=FALSE
  end
 end
end
----------------------------------------------------









