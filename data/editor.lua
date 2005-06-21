
function FloorGroup (l)
    local list = l.list or {l.name}
    editor.DefineFloorGroup(l.name, l.descr, getn(list), list)
end

function Floors (l)
    for i=1,getn(l) do
        FloorGroup {["name"]=l[i], descr=""}
    end
end

function ItemGroup (l)
    local list = l.list or {l.name}
    editor.DefineItemGroup(l.name, l.descr, getn(list), list)
end

function Items (l)
    for i=1,getn(l) do
        ItemGroup {["name"]=l[i], descr=""}
    end
end


function StoneGroup (l)
    local list = l.list or {l.name}
    editor.DefineStoneGroup(l.name, l.descr, getn(list), list)
end

function Stone(name, descr) StoneGroup{["name"]=name, ["descr"]=descr} end

----------------------------------------------------------------------
-- FLOORS
----------------------------------------------------------------------
Floors {"fl-normal", "fl-leaves", "fl-sand"}

----------------------------------------------------------------------
-- ITEMS
----------------------------------------------------------------------
Items{"it-brush", "it-brake", "it-cherry", "it-hammer", 
    "it-umbrella", "it-dynamite", "it-blackbomb", "it-whitebomb"}
ItemGroup {name="", 
    descr="Hills and Hollows", 
    list={"it-hill", "it-tinyhill", "it-hollow", "it-tinyhollow"}}
ItemGroup {name="it-shogun",
    descr="Shogun Dots",
    list={"it-shogun1", "it-shogun2", "it-shogun3"}}

----------------------------------------------------------------------
-- STONES
----------------------------------------------------------------------

StoneGroup {
    name="st-laser", 
    descr="Lasers", 
    list={"st-laser-n", "st-laser-e", "st-laser-s", "st-laser-w"}}
StoneGroup {
    name="st-disco",
    descr="Disco Stones",
    list={"st-disco-light", "st-disco-medium", "st-disco-dark"}}
StoneGroup {
    name="st-death",
    descr="Skull Stones (normal/invisible)",
    list={"st-death", "st-death-invisible"}}
Stone ("st-floppy0", "Floppy Switch")
StoneGroup {
    name = "st-shogun",
    descr = "Shogun Stones",
    list = {"st-shogun1", "st-shogun2","st-shogun4","st-shogun3","st-shogun5","st-shogun6","st-shogun7"}}
--     list={"st-shogun-s", "st-shogun-m", "st-shogun-l", 
--         "st-shogun-sm", "st-shogun-sl", "st-shogun-ml", 
--         "st-shogun-sml"}}
Stone("st-brownie", "Simple Stone")
