#
# Makefile prototype for Enigma.
#
bin_PROGRAMS = enigma


EXTRA_DIST = \
	enigma.ico \
	enigma.rc


CXXFLAGS = @CXXFLAGS@\
	-DSYSTEM_DATA_DIR=\"$(datadir)/enigma\" \
	-DLOCALEDIR=\"$(localedir_enigma)\" \
	-I$(top_srcdir)/lib-src/zipios++  \
	-I$(top_builddir)/lib-src/zipios++ \
	-I$(top_srcdir)/lib-src/lua \
	-I$(top_srcdir)/lib-src/enigma-core \
	-I$(top_srcdir)/lib-src/enet/include \
	-I$(top_srcdir)/lib-src \
	-I$(top_builddir)/intl @SDL_CFLAGS@


enigma_SOURCES = 		\
	actors.hh 		\
	actors.cc 		\
	actors_internal.hh	\
	AttributeDescriptor.cc	\
	AttributeDescriptor.hh	\
	client.cc 		\
	client.hh 		\
	client_internal.hh	\
	DOMErrorReporter.hh     \
	DOMErrorReporter.cc     \
	DOMSchemaResolver.cc	\
	DOMSchemaResolver.hh	\
	d_models.cc 		\
	d_models.hh		\
	display.hh 		\
	display.cc 		\
	display_internal.hh	\
	d_engine.hh		\
	editor.cc 		\
	editor.hh 		\
	editor_impl.hh		\
	enigma.cc 		\
	enigma.hh		\
	errors.hh		\
	file.cc 		\
	file.hh 		\
	file_zip.cc 		\
	floors.cc 		\
	floors.hh		\
	fwd.hh			\
	game.cc			\
	game.hh			\
	GridObject.cc 		\
	GridObject.hh 		\
	Inventory.cc		\
	Inventory.hh		\
	ItemHolder.hh		\
	items.cc 		\
	items.hh		\
	KindDescriptor.cc	\
	KindDescriptor.hh	\
	laser.cc 		\
	laser.hh		\
	LocalToXML.cc		\
	LocalToXML.hh		\
	lua.cc 			\
	lua.hh			\
	lua-display.cc 		\
	lua-display.hh		\
	lua-editor.cc 		\
	lua-editor.hh		\
	lua-enigma.cc 		\
	lua-enigma.hh		\
	lua-global.cc 		\
	lua-global.hh		\
	lua-ecl.cc 		\
	lua-ecl.hh		\
	main.cc 		\
	main.hh			\
	MessageDescriptor.cc	\
	MessageDescriptor.hh	\
	netgame.cc 		\
	netgame.hh 		\
	network.cc		\
	network.hh		\
	nls.cc 			\
	nls.hh			\
	Object.cc 		\
	Object.hh 		\
	ObjectValidator.cc	\
	ObjectValidator.hh	\
	oxyd.cc			\
	oxyd.hh 		\
	oxyd_internal.hh	\
	ox_extra.cc		\
	ox_magnum.cc		\
	ox_oxyd1.cc		\
	ox_peroxyd.cc		\
	options.cc 		\
	options.hh		\
	player.cc 		\
	player.hh		\
	PreferenceManager.cc	\
	PreferenceManager.hh	\
	PropertyManager.cc	\
	PropertyManager.hh	\
	server.cc 		\
	server.hh		\
	server_internal.hh	\
	SoundEffectManager.cc   \
	SoundEffectManager.hh   \
	SoundEngine.cc          \
	SoundEngine.hh 		\
	MusicManager.cc		\
	MusicManager.hh         \
	StateObject.cc		\
	StateObject.hh		\
	StateManager.cc		\
	StateManager.hh		\
	stones.cc 		\
	stones.hh 		\
	stones_internal.hh 	\
	Utf8ToXML.cc		\
	Utf8ToXML.hh		\
	util.cc 		\
	util.hh			\
	utilXML.cc		\
	utilXML.hh		\
	Value.cc		\
	Value.hh		\
	video.cc 		\
	video.hh		\
	WorldProxy.cc		\
	WorldProxy.hh		\
	world.cc 		\
	world.hh 		\
	world_internal.hh	\
	XMLtoLocal.cc		\
	XMLtoLocal.hh		\
	XMLtoUtf8.cc		\
	XMLtoUtf8.hh		\
	actors/Balls.cc		\
	actors/Balls.hh		\
	actors/BugActor.cc	\
	actors/BugActor.hh	\
	actors/HorseActor.cc	\
	actors/HorseActor.hh	\
	actors/KillerActor.cc	\
	actors/KillerActor.hh	\
	actors/Rotors.cc	\
	actors/Rotors.hh	\
	floors/BridgeFloor.cc	\
	floors/BridgeFloor.hh	\
	floors/FloodStream.cc	\
	floors/FloodStream.hh	\
	floors/SimpleFloors.cc	\
	floors/SimpleFloors.hh	\
	floors/SlopeFloor.cc	\
	floors/SlopeFloor.hh	\
	floors/ThiefFloor.cc	\
	floors/ThiefFloor.hh	\
	gui/ErrorMenu.cc	\
	gui/ErrorMenu.hh	\
	gui/GameMenu.cc		\
	gui/GameMenu.hh		\
	gui/HelpMenu.cc		\
	gui/HelpMenu.hh		\
	gui/InfoMenu.cc		\
	gui/InfoMenu.hh		\
	gui/LevelInspector.cc	\
	gui/LevelInspector.hh	\
	gui/LevelMenu.cc	\
	gui/LevelMenu.hh	\
	gui/LevelPackComposer.cc \
	gui/LevelPackComposer.hh \
	gui/LevelPackConfig.cc	\
	gui/LevelPackConfig.hh	\
	gui/LevelPackMenu.cc	\
	gui/LevelPackMenu.hh	\
	gui/LevelPreviewCache.cc \
	gui/LevelPreviewCache.hh \
	gui/LevelWidget.cc	\
	gui/LevelWidget.hh	\
	gui/LPGroupConfig.cc	\
	gui/LPGroupConfig.hh	\
	gui/MainMenu.cc		\
	gui/MainMenu.hh		\
	gui/Menu.cc		\
	gui/Menu.hh		\
	gui/MonospacedLabel.cc	\
	gui/MonospacedLabel.hh	\
	gui/OptionsMenu.cc	\
	gui/OptionsMenu.hh	\
	gui/ScreenshotViewer.cc	\
	gui/ScreenshotViewer.hh	\
	gui/SearchMenu.cc	\
	gui/SearchMenu.hh	\
	gui/TextField.cc	\
	gui/TextField.hh	\
	gui/widgets.cc		\
	gui/widgets.hh		\
	items/BagItem.cc	\
	items/BagItem.hh	\
	items/BrakeItem.cc	\
	items/BrakeItem.hh	\
	items/BlockerItem.cc	\
	items/BlockerItem.hh	\
	items/Bomb.cc		\
	items/Bomb.hh		\
	items/Bottle.cc		\
	items/Bottle.hh		\
	items/BurnableItem.cc	\
	items/BurnableItem.hh	\
	items/Coin.cc		\
	items/Coin.hh		\
	items/CompatibilityItems.cc	\
	items/CompatibilityItems.hh	\
	items/Crack.cc		\
	items/Crack.hh		\
	items/CrossItem.cc	\
	items/CrossItem.hh	\
	items/DocumentItem.cc	\
	items/DocumentItem.hh	\
	items/Drop.cc		\
	items/Drop.hh		\
	items/Dynamite.cc	\
	items/Dynamite.hh	\
	items/Extinguisher.cc	\
	items/Extinguisher.hh	\
	items/ExtraLife.cc	\
	items/ExtraLife.hh	\
	items/GlassesItem.cc	\
	items/GlassesItem.hh	\
	items/Hammer.cc		\
	items/Hammer.hh		\
	items/Landmine.cc	\
	items/Landmine.hh	\
	items/Magnet.cc		\
	items/Magnet.hh		\
	items/Meditation.cc	\
	items/Meditation.hh	\
	items/PipeItem.cc	\
	items/PipeItem.hh	\
	items/PullerItem.cc	\
	items/PullerItem.hh	\
	items/RubberbandItem.cc	\
	items/RubberbandItem.hh	\
	items/SeedItem.cc	\
	items/SeedItem.hh	\
	items/Sensor.cc		\
	items/Sensor.hh		\
	items/ShogunDot.cc	\
	items/ShogunDot.hh	\
	items/SimpleItems.cc	\
	items/SimpleItems.hh	\
	items/StripItem.cc	\
	items/StripItem.hh	\
	items/SurpriseItem.cc	\
	items/SurpriseItem.hh	\
	items/Sword.cc		\
	items/Sword.hh		\
	items/TrapItem.cc	\
	items/TrapItem.hh	\
	items/Trigger.cc	\
	items/Trigger.hh	\
	items/Umbrella.cc	\
	items/Umbrella.hh	\
	items/Vortex.cc		\
	items/Vortex.hh		\
	items/WormHole.cc	\
	items/WormHole.hh	\
	lev/Index.cc		\
	lev/Index.hh		\
	lev/RatingManager.cc	\
	lev/RatingManager.hh	\
	lev/PersistentIndex.cc	\
	lev/PersistentIndex.hh	\
	lev/Proxy.cc		\
	lev/Proxy.hh		\
	lev/ScoreManager.cc	\
	lev/ScoreManager.hh	\
	lev/VolatileIndex.cc	\
	lev/VolatileIndex.hh	\
	others/CannonBall.cc	\
	others/CannonBall.hh	\
	others/CounterGadget.cc	\
	others/CounterGadget.hh	\
	others/Other.cc		\
	others/Other.hh		\
	others/Rubberband.cc	\
	others/Rubberband.hh	\
	others/TimerGadget.cc	\
	others/TimerGadget.hh	\
	others/Wire.cc		\
	others/Wire.hh		\
	stones/ActorImpulseStone.cc	\
	stones/ActorImpulseStone.hh	\
	stones/BlockerStone.cc	\
	stones/BlockerStone.hh	\
	stones/BoulderStone.cc	\
	stones/BoulderStone.hh	\
	stones/BrakeStone.cc	\
	stones/BrakeStone.hh	\
	stones/BreakStone.cc	\
	stones/BreakStone.hh	\
	stones/ChameleonStone.cc	\
	stones/ChameleonStone.hh	\
	stones/ChessStone.cc	\
	stones/ChessStone.hh	\
	stones/ClusterStone.cc	\
	stones/ClusterStone.hh	\
	stones/CoinSlot.cc	\
	stones/CoinSlot.hh	\
	stones/CompatibilityStones.cc	\
	stones/CompatibilityStones.hh	\
	stones/DeathStone.cc	\
	stones/DeathStone.hh	\
	stones/DiscoStone.cc	\
	stones/DiscoStone.hh	\
	stones/DispenserStone.cc	\
	stones/DispenserStone.hh	\
	stones/Door.cc		\
	stones/Door.hh		\
	stones/FakeStone.cc	\
	stones/FakeStone.hh	\
	stones/FlatStone.cc	\
	stones/FlatStone.hh	\
	stones/BoxStone.cc	\
	stones/BoxStone.hh	\
	stones/FloppySwitch.cc	\
	stones/FloppySwitch.hh	\
	stones/FourSwitch.cc	\
	stones/FourSwitch.hh	\
	stones/GhostStone.cc	\
	stones/GhostStone.hh	\
	stones/GlassStones.cc	\
	stones/GlassStones.hh	\
	stones/GreenBrownStone.cc	\
	stones/GreenBrownStone.hh	\
	stones/IceBlock.cc	\
	stones/IceBlock.hh	\
	stones/JambStone.cc	\
	stones/JambStone.hh	\
	stones/KeySwitch.cc	\
	stones/KeySwitch.hh	\
	stones/KnightStone.cc	\
	stones/KnightStone.hh	\
	stones/LaserStone.cc	\
	stones/LaserStone.hh	\
	stones/LaserSwitch.cc	\
	stones/LaserSwitch.hh	\
	stones/LightPassengerStone.cc	\
	stones/LightPassengerStone.hh	\
	stones/MagicStone.cc	\
	stones/MagicStone.hh	\
	stones/MailStone.cc	\
	stones/MailStone.hh	\
	stones/MirrorStone.cc	\
	stones/MirrorStone.hh	\
	stones/MonoFlopStone.cc	\
	stones/MonoFlopStone.hh	\
	stones/OneWayStone.cc	\
	stones/OneWayStone.hh	\
	stones/OxydStone.cc	\
	stones/OxydStone.hh	\
	stones/PassageStone.cc	\
	stones/PassageStone.hh	\
	stones/PlasterStone.cc	\
	stones/PlasterStone.hh	\
	stones/PolarSwitchStone.cc	\
	stones/PolarSwitchStone.hh	\
	stones/PortalStone.cc	\
	stones/PortalStone.hh	\
	stones/PullStone.cc	\
	stones/PullStone.hh	\
	stones/PuzzleStone.cc	\
	stones/PuzzleStone.hh	\
	stones/QuakeStone.cc	\
	stones/QuakeStone.hh	\
	stones/RotatorStone.cc	\
	stones/RotatorStone.hh	\
	stones/RubberbandStone.cc	\
	stones/RubberbandStone.hh	\
	stones/ScissorsStone.cc	\
	stones/ScissorsStone.hh	\
	stones/ShogunStone.cc	\
	stones/ShogunStone.hh	\
	stones/SimpleStones.cc	\
	stones/SimpleStones.hh	\
	stones/StandardStones.cc	\
	stones/StandardStones.hh	\
	stones/StoneImpulse.cc	\
	stones/StoneImpulse.hh	\
	stones/SurpriseStone.cc	\
	stones/SurpriseStone.hh	\
	stones/SwapStone.cc	\
	stones/SwapStone.hh	\
	stones/Switch.cc	\
	stones/Switch.hh	\
	stones/ThiefStone.cc	\
	stones/ThiefStone.hh	\
	stones/TimerStone.cc	\
	stones/TimerStone.hh	\
	stones/Turnstile.cc	\
	stones/Turnstile.hh	\
	stones/VolcanoStone.cc	\
	stones/VolcanoStone.hh	\
	stones/WindowStone.cc	\
	stones/WindowStone.hh	\
	stones/YieldingStone.cc	\
	stones/YieldingStone.hh

datadir = @datadir@

if MINGW32

  localedir_enigma := data/locale
  mingw_ldadd := enigma_ico.o -lws2_32
  enigma_ico.o: enigma.rc enigma.ico
	$(WINDRES) $(srcdir)/enigma.rc --include-dir=$(srcdir) -O coff -o enigma_ico.o

else # not MINGW32

  localedir_enigma := $(datadir)/locale
  mingw_ldadd =

endif

enigma_LDADD = \
	@LIBINTL@ \
	$(top_builddir)/lib-src/enigma-core/libecl.a \
	$(top_builddir)/lib-src/lua/liblua.a \
	$(top_builddir)/lib-src/enet/libenet.a \
	$(top_builddir)/lib-src/oxydlib/liboxyd.a \
	$(top_builddir)/lib-src/zipios++/src/libzipios.a \
	-lz \
	$(mingw_ldadd) \
	@SDL_LIBS@

# Rule for creating Lua interfaces from package descriptions using 'tolua'
TOLUA=@TOLUA@
.PHONY: tolua
tolua:
	@if [ "x${TOLUA}" != "x" ]; then \
		echo "Running ${TOLUA}..."; \
		${TOLUA} -H lua-display.hh -n display -o lua-display.cc display-lua.pkg; \
		${TOLUA} -H lua-editor.hh -n editor -o lua-editor.cc editor-lua.pkg; \
		${TOLUA} -H lua-enigma.hh -n enigma -o lua-enigma.cc enigma-lua.pkg; \
		${TOLUA} -H lua-global.hh -n global -o lua-global.cc global-lua.pkg; \
		${TOLUA} -H lua-ecl.hh -n px -o lua-ecl.cc ecl-lua.pkg; \
	else \
		echo "$<:0: Warning: Can't update lua interface - tolua not installed "; \
	fi


#------------------ Packaging rules for Mac OSX ------------------------

bundle_root := $(top_builddir)/etc/macfiles
bundle_name := $(bundle_root)/Enigma.app
# Special target to copy all needed game files to Enigma.app/Contents/Resources.
# For Mac OS X bundle.
.PHONY: bundle
bundle:
	-rm -rf $(bundle_root)
	mkdir -p $(bundle_name)/Contents/MacOS
	mkdir -p $(bundle_name)/Contents/Resources/data/
	echo "APPL????" > $(bundle_name)/Contents/PkgInfo
	cp $(top_builddir)/etc/Info.plist $(bundle_name)/Contents/
	cp $(top_builddir)/etc/enigma.icns $(bundle_name)/Contents/Resources/
	tar --exclude .svn -c -f - -C $(top_builddir)/data levels | tar xpf - -C $(bundle_name)/Contents/Resources/data
	cp $(top_builddir)/data/*.lua $(bundle_name)/Contents/Resources/data/
	cp $(top_builddir)/data/*.xml $(bundle_name)/Contents/Resources/data/
	mkdir -p $(bundle_name)/Contents/Resources/data/gfx
	cp $(top_builddir)/data/gfx/*.png $(bundle_name)/Contents/Resources/data/gfx
	mkdir -p $(bundle_name)/Contents/Resources/data/gfx/flags25x15
	cp $(top_builddir)/data/gfx/flags25x15/*.png $(bundle_name)/Contents/Resources/data/gfx/flags25x15
	mkdir -p $(bundle_name)/Contents/Resources/data/gfx32
	cp $(top_builddir)/data/gfx32/*.png $(bundle_name)/Contents/Resources/data/gfx32
	cp $(top_builddir)/data/gfx32/*.jpg $(bundle_name)/Contents/Resources/data/gfx32
	mkdir -p $(bundle_name)/Contents/Resources/data/gfx40
	cp $(top_builddir)/data/gfx40/*.png $(bundle_name)/Contents/Resources/data/gfx40
	cp $(top_builddir)/data/gfx40/*.jpg $(bundle_name)/Contents/Resources/data/gfx40
	mkdir -p $(bundle_name)/Contents/Resources/data/gfx48
	cp $(top_builddir)/data/gfx48/*.png $(bundle_name)/Contents/Resources/data/gfx48
	cp $(top_builddir)/data/gfx48/*.jpg $(bundle_name)/Contents/Resources/data/gfx48
	mkdir -p $(bundle_name)/Contents/Resources/data/schemas
	cp $(top_builddir)/data/schemas/*.xml $(bundle_name)/Contents/Resources/data/schemas
	cp $(top_builddir)/data/schemas/*.xsd $(bundle_name)/Contents/Resources/data/schemas
	mkdir -p $(bundle_name)/Contents/Resources/data/music
	mkdir -p $(bundle_name)/Contents/Resources/data/music/menu
	cp $(top_builddir)/data/music/menu/*.s3m $(bundle_name)/Contents/Resources/data/music/menu
	cp $(top_builddir)/data/music/menu/*.ogg $(bundle_name)/Contents/Resources/data/music/menu
	mkdir -p $(bundle_name)/Contents/Resources/data/soundsets
	mkdir -p $(bundle_name)/Contents/Resources/data/soundsets/enigma
	cp $(top_builddir)/data/soundsets/enigma/*.wav $(bundle_name)/Contents/Resources/data/soundsets/enigma
	mkdir -p $(bundle_name)/Contents/Resources/data/fonts
	cp $(top_builddir)/data/fonts/*.png $(bundle_name)/Contents/Resources/data/fonts
	cp $(top_builddir)/data/fonts/*.bmf $(bundle_name)/Contents/Resources/data/fonts
	cp $(top_builddir)/data/fonts/*.ttf $(bundle_name)/Contents/Resources/data/fonts
	mkdir -p $(bundle_name)/Contents/Frameworks
	cp -R /Library/Frameworks/SDL.framework $(bundle_name)/Contents/Frameworks/
	cp -R /Library/Frameworks/SDL_image.framework $(bundle_name)/Contents/Frameworks/
	cp -R /Library/Frameworks/SDL_mixer.framework $(bundle_name)/Contents/Frameworks/
	cp -R /Library/Frameworks/SDL_ttf.framework $(bundle_name)/Contents/Frameworks/
	for cat in $(ALL_LINGUAS); do \
	  dir=$(bundle_name)/Contents/Resources/locale/$$cat/LC_MESSAGES; \
	  mkdir -p $$dir; \
	  echo "installing $$cat as $$dir/enigma.mo"; \
	  cp $(top_builddir)/po/$$cat.gmo $$dir/enigma.mo; \
	done

# Special target to copy the binary to Enigma.app/Contents/MacOS.
.PHONY: bundle-fw
bundle-fw: enigma-fw-mosx
	cp enigma-fw $(bundle_name)/Contents/MacOS/enigma
	strip $(bundle_name)/Contents/MacOS/enigma

# Special target to copy the doc files to the MacOS dmg staging area
bundle_doc := $(bundle_root)/Documentation
.PHONY: bundle-doc
bundle-doc:
	-rm -rf $(bundle_doc)
	mkdir -p $(bundle_doc)/images/flags25x15
	mkdir -p $(bundle_doc)/reference/images
	mkdir -p $(bundle_doc)/manual/images
	cp $(top_builddir)/README $(bundle_doc)/
	cp $(top_builddir)/COPYING $(bundle_doc)/
	cp $(top_builddir)/CHANGES $(bundle_doc)/
	cp $(top_builddir)/ACKNOWLEDGEMENTS $(bundle_doc)/
	cp $(top_builddir)/etc/README-SDL.txt $(bundle_doc)/
	cp $(top_builddir)/doc/index.html $(bundle_doc)/
	( cd $(bundle_root) ; ln -s ./Documentation/index.html index.html )
	cp $(top_builddir)/doc/gpl.txt $(bundle_doc)/
	cp $(top_builddir)/doc/lgpl.txt $(bundle_doc)/
	cp $(top_builddir)/doc/images/enigma.css $(bundle_doc)/images/
	cp $(top_builddir)/doc/images/nav_enigma.gif $(bundle_doc)/images/
	cp $(top_builddir)/doc/images/nav_cornerul.gif $(bundle_doc)/images/
	cp $(top_builddir)/doc/images/nav_cornerur.gif $(bundle_doc)/images/
	cp $(top_builddir)/doc/images/favicon.png $(bundle_doc)/images/
	cp $(top_builddir)/doc/images/menu_bg.jpg $(bundle_doc)/images/
	cp $(top_builddir)/doc/images/flags25x15/*.png $(bundle_doc)/images/flags25x15/
	cp $(top_builddir)/doc/reference/ant_lua.txt $(bundle_doc)/reference/
	cp $(top_builddir)/doc/reference/sounds.txt $(bundle_doc)/reference/
	cp $(top_builddir)/doc/reference/soundset.lua $(bundle_doc)/reference/
	cp $(top_builddir)/doc/reference/lua2xml $(bundle_doc)/reference/
	cp $(top_builddir)/doc/reference/xml2lua $(bundle_doc)/reference/
	cp $(top_builddir)/doc/manual/images/*.png $(bundle_doc)/manual/images/
	cp $(top_builddir)/doc/manual/*.html $(bundle_doc)/manual/
	cp $(top_builddir)/doc/reference/images/*.png $(bundle_doc)/reference/images/
	cp $(top_builddir)/doc/reference/*.html $(bundle_doc)/reference/

# Special target to create a binary linked with Mac OS X frameworks
# note this assumes that xercesc and libpng are installed in same directory as the SDL libraries.
static_prefix := `$(SDL_CONFIG) --prefix`
.PHONY: enigma-fw-mosx
enigma-fw-mosx: $(enigma_OBJECTS) $(enigma_DEPENDENCIES)
	$(CXX) $(LDFLAGS) -o enigma-fw $(enigma_OBJECTS) $(enigma_DEPENDENCIES) $(static_prefix)/lib/libintl.a /usr/lib/libiconv.dylib $(static_prefix)/lib/libxerces-c.a -L$(static_prefix)/lib -lsdlmain -lz -framework SDL -framework SDL_image -framework SDL_mixer -framework Cocoa -framework OpenGL -framework SDL_ttf $(static_prefix)/lib/libpng.a 

# Special target to make a disk image of the bundled app
.PHONY: bundle-dmg
bundle-dmg:
	hdiutil create  -ov -srcfolder $(bundle_root) -volname Enigma -imagekey zlib-level=6 $(top_builddir)/etc/enigma.dmg

