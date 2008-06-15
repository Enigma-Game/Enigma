set_item("it-floppy", 18, 5)
set_item("it-floppy", 31, 36)
set_item("it-floppy", 34, 32)
set_item("it-floppy", 32, 28)
set_item("it-floppy", 30, 10)
set_item("it-floppy", 55, 15)
set_stone("st-door-h", 9, 3, {name = "safe_door"})
kill_item(9, 5)

xpositions = {6, 7, 8, 10, 11, 12}
floppystate = {}

for j, x in pairs(xpositions) do
  set_stone("st-floppy", x, 4, {action = "callback", target = "floppy_callback"})
end

function floppy_callback(onoff, sender)
  local x, y = enigma.GetPos(sender)
  floppystate[x] = 1 - (floppystate[x] or 0)
  local total = 0
  for j, x in pairs(xpositions) do
    total = total + (floppystate[x] or 0)
  end
  if total == 6 then
    SendMessage(enigma.GetNamedObject("safe_door"), "open")
  else
    SendMessage(enigma.GetNamedObject("safe_door"), "close")
  end
end

