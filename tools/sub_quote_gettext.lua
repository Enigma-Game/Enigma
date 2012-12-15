#!/usr/bin/env lua
-- sub_quote_gettext.lua
-- Copyright (c) 2006 Ronald Lamprecht
-- License: GPL2
-- Usage: lua sub_quote_gettext.lua level_i18n.in > level_i18n.cc 
-- Requires Lua 5.0 or above

-- substitutes inner not unescaped quotes to C-escaped quotes in gettext lines.

stdout = io.output()
for line in io.lines(arg[1]) do
    if (string.find(line,"gettext(",1,true) == 1) then
        stdout:write("gettext(\"" )
        s = string.gsub(string.sub(line, 10, string.len(line)-2), "\"", "\\\"")
        stdout:write(s)
        stdout:write("\")\n" )
    else
        stdout:write(line .. "\n" )
    end
end
