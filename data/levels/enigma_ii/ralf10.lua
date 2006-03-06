-- A level for Enigma
-- Name:        Khun Phan
-- Filename:    ralf10.lua
-- Copyright: 	(C) May 2003 Ralf Westram
-- Contact: 	amgine@reallysoft.de
-- License: 	GPL v2.0 or above

dofile(enigma.FindDataFile("levels/lib/ralf.lua"))

--debug_mode()

level={
   "*************************",
   "o bbbbbbbbbbbbbbbbbbbbbb*",
   "* *********************b*",
   "*                     *b*",
   "*   ################# *b*",
   "*   # ^  ^     ^  ^ # *b*",
   "*   #<+=-+>   <+=-+># *b*",
   "*   # v  v     v  v # *b*",
   "*   # ^  ^  ^  ^  ^ # *b*",
   "*   #<] <+><+><+><+># *b*",
   "*   # |a =  |  v  v # *b*",
   "*   # =  |  =  ^  ^ # *b*",
   "*   #<+-=+><+><+><+># *b*",
   "*   # v  v  v  v  v # *b*",
   "*   # ^  ^     ^  ^ # *b*",
   "*   #<+=-+>   <+=-+># *b*",
   "*   # v  v     v  v # * o",
   "*   ################# ***",
   "*                       *",
   "*                     ! t",
   "*                       *",
   "*************************",
}

switchpad={
   " x  x ",
   "xx xxx",
   " x  x ",
   " x  x ",
   "xxxxxx",
   " x  x ",
}

bridges=0
function set_bridge(x,y)
   bridges = bridges+1
   set_floor("fl-bridge",x,y,{name="bridge"..bridges})
end

switches=0
function set_switch(x,y,toggle)
   switches=switches+1
   set_item("it-trigger",x,y,{action="openclose",target="bridge"..switches})
   if (toggle==1) then
      SendMessage("bridge"..switches, "openclose")
   end
end

function set_pad(pad,x,y)
   padw,padh = get_map_size(pad)
   for py=1,padh do
      for px=1,padw do
         local col = strsub(pad[py],px,px)
         if (col == "x") then
            set_switch(x+px,y+py,0)
         else
            set_switch(x+px,y+py,1)
         end
      end
   end
end

docx = 0
docy = 0
hints = 0

function hint()
   hints = hints+1

   if (hints<=2) then
      enigma.EmitSound(enigma.GetNamedObject("soundstone"), "itemtransform")
   end

   if hints==1 then
      document(docx, docy, "One of the puzzle stones needs to be placed onto the trigger field.                       Guess which.. :)");
   elseif hints==2 then
      document(docx, docy, "The level preview might help..");
   end
end

function store_doc_pos(x,y)
   docx,docy=x,y
end

cells={}


cells[" "] = cell{floor="fl-tigris"}
cells["#"] = cell{parent=cells[" "],stone="st-grate1"}
cells["*"] = cell{parent=cells[" "],stone="st-rock1"}
cells["a"] = cell{parent=cells[" "],actor={"ac-blackball", {player=0}}}
cells["o"] = cell{oxyd}

cells["i"] = cell{parent=cells[" "],stone="st-stoneimpulse"}
cells["I"] = cell{parent=cells[" "],stone="st-stoneimpulse_movable"}

cells["-"]=cell{{{puzzle, PUZ_0101}}}
cells[">"]=cell{{{puzzle, PUZ_0001}}}
cells["<"]=cell{{{puzzle, PUZ_0100}}}

cells["|"]=cell{{{puzzle, PUZ_1010}}}
cells["^"]=cell{{{puzzle, PUZ_0010}}}
cells["v"]=cell{{{puzzle, PUZ_1000}}}

cells["+"]=cell{{{puzzle, PUZ_1111}}}
cells["]"]=cell{{{puzzle, PUZ_1011}}}
cells["="]=cell{{{puzzle, PUZ_0000}}}
cells["/"]=cell{{{puzzle, PUZ_0110}}}
cells["`"]=cell{{{puzzle, PUZ_0011}}}
cells["."]=cell{{{puzzle, PUZ_1100}}}
cells[","]=cell{{{puzzle, PUZ_1001}}}

cells["b"]=cell{parent={set_bridge}}

cells["!"] = cell{floor="fl-sahara",parent={store_doc_pos}}
cells["t"] = cell{parent=cells[" "],stone={face="st-timer",attr={action="callback", target="hint", interval=180, name="soundstone"}}}

rs_create_world(level,cells)

set_pad(switchpad,xlo+13,ylo+7)

display.SetFollowMode(display.FOLLOW_SMOOTH)
oxyd_shuffle()















