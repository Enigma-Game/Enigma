TileSize = 48
SpriteSize = 60
ShadowSize = 61
MainFont = "MicroHei/wqy-microhei.ttc"
-- Alternative fonts are e.g.:
--   MainFont = "DejaVuSansCondensed.ttf"
--   MainFont = "vera_sans.ttf"

DefineFont ("timefont", MainFont, 40, "timefont", 180, 180, 180)
DefineFont ("smallalternative", MainFont, 14, "menufont")
DefineFont ("modesfont", MainFont, 16, "menufont", 70, 120, 255)
DefineFont ("menufont", MainFont, 16, "menufont")
DefineFont ("menufontsel", MainFont, 16, "menufont", 180, 180, 180)
DefineFont ("statusbarfont", MainFont, 24, "dreamorp24")

dofile(FindDataFile("models-2d.lua"))
