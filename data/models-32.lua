TileSize = 32
SpriteSize = 40
ShadowSize = 41
MainFont = "MicroHei/wqy-microhei.ttc"
LevelMenuFont = "" -- will load levelmenu.bmf instead
-- Alternative fonts are e.g.:
--   MainFont = "DejaVuSansCondensed.ttf"
--   MainFont = "vera_sans.ttf"

DefineFont ("timefont", MainFont, 32, "timefont", 180, 180, 180)
DefineFont ("smallalternative", MainFont, 14, "menufont")
DefineFont ("modesfont", MainFont, 16, "menufont", 70, 120, 255)
DefineFont ("menufont", MainFont, 16, "menufont")
DefineFont ("levelmenu", LevelMenuFont, 16, "levelmenu")
DefineFont ("menufontsel", MainFont, 16, "menufont", 180, 180, 180)
DefineFont ("statusbarfont", MainFont, 18, "dreamorp24")

dofile(FindDataFile("models-2d.lua"))
