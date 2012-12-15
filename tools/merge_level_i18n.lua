#!/usr/bin/env lua
-- Simple 
-- Copyright (c) 2006 Ronald Lamprecht
-- License: GPL2
-- Usage: lua merge_level_i18n.lua lang
-- Requires Lua 5.0 or above

level_i18n = "level_i18n.cc"
lang = arg[1]

stdout = io.output()
stdout:write("lua-merge " .. lang .. " end\n")
new_po = io.output(lang .. "_n.po")
org_po = io.lines(lang .. ".po")
org_s = 0
org_eof = false
duplicates = {}
dup_i = 0
li = 0

for line_i18n in io.lines(level_i18n) do
    li = li+1
    if (string.find(line_i18n,"//level#",1,true) == 1) then
        level = string.sub(line_i18n, 8)
        author = nil
        comment = nil
        use = nil
        check = nil
    end
    if (string.find(line_i18n,"//author#",1,true) == 1) then
        author = string.sub(line_i18n, 9)
    end
    if (string.find(line_i18n,"//comment#",1,true) == 1) then
        comment = string.sub(line_i18n, 10)
    end
    if (string.find(line_i18n,"//" .. lang .. "#  use",1,true) == 1) then
        use = string.sub(line_i18n, 5)
    end
    if (string.find(line_i18n,"//" .. lang .. "#  check",1,true) == 1) then
        check = string.sub(line_i18n, 5)
    end
    if (string.find(line_i18n,"gettext",1,true) == 1) then
        local search_path = "po/" .. level_i18n
        local search_ref = search_path .. ":" .. li
        local search = "#: " .. search_ref
        local found = false
        local trans_com = {}
        local trans_i = 0
        local autom_com = nil
        for i=0, dup_i -1, 1 do
            if (string.find(duplicates[i],search_ref,1,true) ~= nil) then
                found = true
            end
        end
        while (org_eof == false and found == false) do
            line_org = org_po(org_s, line_org)
            if line_org == nil then
                org_eof = true
            else
                if (string.find(line_org,search,1,true) == 1) then
                    found = true
                    if level ~= nil then new_po:write(level .. "\n") end
                    if author ~= nil then new_po:write(author .. "\n") end
                    if comment ~= nil then new_po:write(comment .. "\n") end
                    if use ~= nil then new_po:write(use .. "\n") end
                    if check ~= nil then new_po:write(check .. "\n") end
                    for i=0, trans_i - 1, 1 do
                        new_po:write(trans_com[i] .. "\n")
                    end
                    if autom_com ~= nil then new_po:write(autom_com .. "\n") end
                    new_po:write(line_org .. "\n")
                    -- look for duplicate references
                    if string.len(line_org) > string.len(search) then
                        duplicates[dup_i] = string.sub(line_org, string.len(search))
                        dup_i = dup_i + 1
                    end
                elseif (string.find(line_org,"#: ",1,true) == 1) then
                    -- foreign references
                    for i=0, trans_i - 1, 1 do
                        new_po:write(trans_com[i] .. "\n")
                    end
                    trans_i = 0
                    if autom_com ~= nil then new_po:write(autom_com .. "\n") end
                    autom_com = nil
                    new_po:write(line_org .. "\n")
                    -- look for duplicate references to the level_i18n file
                    if (string.find(line_org, search_path,1, true)) then
                        duplicates[dup_i] = line_org
                        dup_i = dup_i + 1                    
                    end
                elseif (string.find(line_org,"#.",1,true) == 1) then
                    autom_com = line_org
                elseif (string.find(line_org,"#,",1,true) == 1) then
                    new_po:write(line_org .. "\n")
    
                -- skip old level comments
                elseif (string.find(line_org,"#  level:",1,true) == 1) then
                elseif (string.find(line_org,"#  author:",1,true) == 1) then
                elseif (string.find(line_org,"#  comment:",1,true) == 1) then
                elseif (string.find(line_org,"#  use:",1,true) == 1) then
                elseif (string.find(line_org,"#  check:",1,true) == 1) then
                
                elseif (string.find(line_org,"#",1,true) == 1) then
                    trans_com[trans_i] = line_org
                    trans_i = trans_i + 1
                else
                    for i=0, trans_i - 1, 1 do
                        new_po:write(trans_com[i] .. "\n")
                    end
                    trans_i = 0
                    if autom_com ~= nil then new_po:write(autom_com .. "\n") end
                    autom_com = nil
                    new_po:write(line_org .. "\n")
                end
            end
        end
        comment = nil
        use = nil
        check = nil
    end
  
end

if org_eof == false then
    line_org = org_po(org_s, line_org)
    while (line_org ~= nil) do
        new_po:write(line_org .. "\n")
        line_org = org_po(org_s, line_org)
    end
end

os.remove(lang .. ".po~")
if (os.rename(lang .. ".po", lang .. ".po~") == nil) then
    stdout:write("error in rename 1\n")
end
new_po:flush()
new_po:close()
if( os.rename(lang .. "_n.po", lang .. ".po") == nil) then
    stdout:write("error in rename 2\n")
end

