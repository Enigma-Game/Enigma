-- A level for Enigma
-- Name:	Electric Funeral
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

dofile(enigma.FindDataFile("levels/ant.lua"))
cells={}

cells[" "]=cell{floor="fl-leaves"}
cells["b"]=cell{floor="fl-gravel"}
cells["#"]=cell{stone="st-rock1"}
cells["h"]=cell{parent=cells["b"],item="it-hollow"}
cells["+"]=cell{stone="st-chargeplus"}
cells["-"]=cell{stone="st-chargeminus"}
cells["0"]=cell{stone="st-chargezero"}

level = {
   "#########-########0#",     
   "#                  #",     
   "#                  #",     
   "#                  #",     
   "#      bhhhb       #",     
   "#      hbbbh       #",     
   "+      hb+bh       +",     
   "#      hbbbh       #",     
   "#      bhhhb       #",     
   "#                  #",     
   "#                  #",     
   "#                  #",     
   "#0#######-##########"      
}   
set_default_parent(cells[" "])
    
create_world_by_map(level,cells)

local ac1=set_actor("ac-whiteball-small", 6,1.5, {player=0})
local ac2=set_actor("ac-whiteball-small", 8,9, {player=0})                                          
local ac3=set_actor("ac-whiteball-small", 5,3, {player=0})                                          
local ac4=set_actor("ac-whiteball-small", 8,8, {player=0})                                          
local ac5=set_actor("ac-whiteball-small", 10,5, {player=0})
local ac6=set_actor("ac-whiteball-small", 8,6, {player=0})
local ac7=set_actor("ac-whiteball-small", 9,7, {player=0})
local ac8=set_actor("ac-whiteball-small", 8,4, {player=0})
local ac9=set_actor("ac-whiteball-small", 8,2, {player=0})
local ac10=set_actor("ac-whiteball-small", 9,5, {player=0})
local ac11=set_actor("ac-whiteball-small", 9,4, {player=0})
local ac12=set_actor("ac-whiteball-small", 9,3, {player=0})

                                          
AddRubberBand(ac1, ac2, 6, 1)
AddRubberBand(ac2, ac3, 6, 1)
AddRubberBand(ac3, ac4, 6, 1)    
AddRubberBand(ac4, ac5, 6, 1)    
AddRubberBand(ac5, ac6, 6, 1)
AddRubberBand(ac6, ac7, 6, 1)
AddRubberBand(ac7, ac8, 6, 1)    
AddRubberBand(ac8, ac9, 6, 1) 
AddRubberBand(ac9, ac10, 6, 1)
AddRubberBand(ac10, ac11, 6, 1)
AddRubberBand(ac11, ac12, 6, 1)    
AddRubberBand(ac12, ac1, 6, 1) 
                               
                                 
                                 
                                 
