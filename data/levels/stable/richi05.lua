-- Way of the Oxyd, a level for Enigma
-- Copyright (C) 2005 Richi Buetzer
-- Licensed under GPL v2.0 or above

create_world(20, 13)
oxyd_default_flavor="b"

-- Designs
normalFloor="fl-sahara"
visibleFloor="fl-black"
stone="st-glass"
borderFloor="fl-sahara"

-- Floor Layer
fill_floor( normalFloor, 0, 0, 3, levelh)
fill_floor( "fl-abyss", 3, 0, 17, levelh)
fill_floor( normalFloor, 17, 0, levelw, levelh)

fill_floor( borderFloor, 0, 0, levelw,1)
fill_floor( borderFloor, 0, 12, levelw, 1)
fill_floor( borderFloor, 0, 0, 1, levelh)
fill_floor( borderFloor, 19, 0, 1, levelh)

-- Stone Layer
draw_border(stone) --Rand

oxyd(18,1)
oxyd(18,11)

-- Actor Layer
set_actor("ac-blackball", 1.5, 6.5, {player="0", mouseforce="1", name="actor"})

-- Code-Sektion
weg={}
i=0
aktX=2
aktY=1+random(10)
vorvorrichtung=2
vorrichtung=2
rnd=2
actor=enigma.GetNamedObject("actor")
verschwunden=FALSE
verdeckt={}

-- finde Weg
while aktX<16 do
   if aktX<4 then
    aktX=aktX+1
   elseif vorrichtung==2 then
       if rnd==1 and vorvorrichtung~=3 then aktY=aktY-1
       elseif rnd==3 and vorvorrichtung~=1 then aktY=aktY+1
       else aktX=aktX+1 end
   elseif vorrichtung==1 then
       if rnd==1 and vorvorrichtung~=3 then aktY=aktY-1
       else aktX=aktX+1 end
   else
       if rnd==3 and vorvorrichtung~=1 then aktY=aktY+1
       else aktX=aktX+1 end
   end

   vorvorrichtung=vorrichtung
   vorrichtung=rnd
   rnd=random(3)

   if(aktY<1) then aktY=aktY+1
   elseif(aktY>11) then aktY=aktY-1
   else i=i+1 weg[i]={aktX,aktY} end
   
end

-- zeichne Weg
for i,val in weg do
    set_floor(visibleFloor,weg[i][1],weg[i][2])
       set_item("it-trigger", weg[i][1],weg[i][2], {action="callback", target="einzelanzeige", invisible=TRUE})
end

-- loesche Weg
function verschwinde()
   if verschwunden==FALSE then
       for i, val in weg
       do
           set_floor("fl-abyss_fake",weg[i][1],weg[i][2])
           verdeckt[weg[i][1]..weg[i][2]]=TRUE
       end
      if not difficult then
         set_floor(normalFloor,7,weg[7][2]) 
         set_floor(normalFloor,12,weg[12][2])
         set_item("it-extralife",7,weg[7][2])
         set_item("it-extralife",12,weg[12][2])
      end
   end
   verschwunden=TRUE
end

-- mache aktuelles Feld sichtbar
function einzelanzeige()
   if verschwunden==FALSE then
    verschwinde()
   end
   x,y=enigma.GetPos(actor)
   if verdeckt[x..y]==TRUE then
       set_floor(normalFloor,x,y)
   end
   verdeckt[x..y]=FALSE
end


























