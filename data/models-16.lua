TileSize = 16
SpriteSize = 20
ShadowSize = 21
MainFont = "MicroHei/wqy-microhei.ttc"
-- Alternative fonts are e.g.:
--   MainFont = "DejaVuSansCondensed.ttf"
--   MainFont = "vera_sans.ttf"

DefineFont ("timefont", MainFont, 18, "timefont", 180, 180, 180)
DefineFont ("smallalternative", MainFont, 7, "menufont")
DefineFont ("modesfont", MainFont, 9, "menufont", 70, 120, 255)
DefineFont ("menufont", MainFont, 9, "menufont")
DefineFont ("levelmenu", MainFont, 9, "levelmenu")
DefineFont ("menufontsel", MainFont, 10, "menufont", 180, 180, 180)
DefineFont ("statusbarfont", MainFont, 7, "dreamorp24")

dofile(FindDataFile("models-2d.lua"))
