#SUBDIRS = gfx schemas levels music soundsets gfx16 gfx32 gfx40 gfx48 gfx64
add_subdirectory(fonts)
add_subdirectory(music)

install(FILES
        api1init.lua
        api2init.lua
        enigma_conf.lua
        models.lua
        models-2d.lua
        models-16.lua
        models-32.lua
        models-40.lua
        models-48.lua
        startup.lua
        sound-defaults.lua
        compat.lua
        security.lua
        ratings.xml
        DESTINATION data)

