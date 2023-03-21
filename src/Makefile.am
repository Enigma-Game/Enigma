#
# Makefile prototype for Enigma.
#
AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = enigma

EXTRA_DIST = \
	enigma.ico \
	enigma.rc

CXXFLAGS = @CXXFLAGS@\
	-DSYSTEM_DATA_DIR=\"$(datadir)/enigma\" \
	-DDOCDIR=\"$(docdir)\" \
	-I$(top_srcdir)/lib-src/zipios++  \
	-I$(top_builddir)/lib-src/zipios++ \
	-I$(top_srcdir)/lib-src/lua \
	-I$(top_srcdir)/lib-src/tinygettext \
	-I$(top_srcdir)/lib-src/enigma-core

if SYSTEM_ENET
CXXFLAGS += $(LIBENET_CFLAGS)
else
CXXFLAGS += -I$(top_srcdir)/lib-src/enet/include
endif

if MINGW32
CXXFLAGS += -static-libgcc -static-libstdc++ -DWIN32
endif

CXXFLAGS += -I$(top_srcdir)/lib-src @SDL_CFLAGS@


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
	d_follower.cc		\
	d_follower.hh		\
	d_models.cc 		\
	d_models.hh		\
	display.hh 		\
	display.cc 		\
	display_internal.hh	\
	d_engine.hh		\
	enigma.cc 		\
	enigma.hh		\
	enet_ver.hh		\
	errors.hh		\
	file.cc 		\
	file.hh 		\
	file_zip.cc 		\
	floors.cc 		\
	floors.hh		\
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
	MouseCursor.cc		\
	MouseCursor.hh		\
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
	resource_cache.cc	\
	resource_cache.hh	\
	server.cc 		\
	server.hh		\
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
	timer.cc 		\
	timer.hh		\
	Utf8ToXML.cc		\
	Utf8ToXML.hh		\
	utilXML.cc		\
	utilXML.hh		\
	Value.cc		\
	Value.hh		\
	video.cc 		\
	video.hh		\
	video_effects.cc	\
	video_effects.hh	\
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
	floors/ForwardFloor.cc	\
	floors/ForwardFloor.hh	\
	floors/ScalesFloor.cc	\
	floors/ScalesFloor.hh	\
	floors/SimpleFloors.cc	\
	floors/SimpleFloors.hh	\
	floors/SlopeFloor.cc	\
	floors/SlopeFloor.hh	\
	floors/StandardFloors.cc	\
	floors/StandardFloors.hh	\
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
	gui/LanguageMenu.cc	\
	gui/LanguageMenu.hh	\
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
	lev/SearchCombination.cc    \
	lev/SearchCombination.hh    \
	lev/SubProxy.cc		\
	lev/SubProxy.hh		\
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
	stones/BarrierStone.cc   \
	stones/BarrierStone.hh   \
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
	stones/InkwellStone.cc	\
	stones/InkwellStone.hh	\
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
	stones/SpitterStone.cc	\
	stones/SpitterStone.hh	\
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

  # mingw_ldadd := enigma_ico.o -lws2_32
  mingw_ldadd := enigma_ico.o -lws2_32 -lSDL2_image -ltiff -llzma -ljpeg -lpng16 -lSDL2_mixer -lmikmod -lsmpeg2 -lvorbisfile -lvorbis -logg -lSDL2_ttf -lcurl -lidn -lwldap32 -lgnutls -lhogweed -lgmp -lz -lnettle -lssh2 -lgcrypt -lgpg-error -lws2_32 -lfreetype -lintl -liconv
  enigma_LDFLAGS := -mwindows -pthread
  enigma_ico.o: enigma.rc enigma.ico
	$(WINDRES) $(srcdir)/enigma.rc --include-dir=$(srcdir) -O coff -o enigma_ico.o

else # not MINGW32

  mingw_ldadd =

endif

if MACOSX

  macosx_ldadd := -liconv

else # not MACOSX

  macosx_ldadd =

endif

enigma_LDADD = \
	$(top_builddir)/lib-src/enigma-core/libecl.a \
	$(top_builddir)/lib-src/lua/liblua.a

if SYSTEM_ENET
enigma_LDADD += $(LIBENET_LIBS)
else
enigma_LDADD += $(top_builddir)/lib-src/enet/libenet.a
endif

enigma_LDADD += $(top_builddir)/lib-src/oxydlib/liboxyd.a \
	$(top_builddir)/lib-src/zipios++/src/libzipios.a \
	$(top_builddir)/lib-src/tinygettext/libtinygettext.a \
	-lz \
	$(mingw_ldadd) \
	$(macosx_ldadd) \
	@SDL_LIBS@


# Rule for creating Lua interfaces from package descriptions using 'tolua'
TOLUA=@TOLUA@
.PHONY: tolua
tolua:
	@if [ "x${TOLUA}" != "x" ]; then \
		echo "Running ${TOLUA}..."; \
		${TOLUA} -H lua-display.hh -n display -o lua-display.cc display-lua.pkg; \
		${TOLUA} -H lua-enigma.hh -n enigma -o lua-enigma.cc enigma-lua.pkg; \
		${TOLUA} -H lua-global.hh -n global -o lua-global.cc global-lua.pkg; \
		${TOLUA} -H lua-ecl.hh -n px -o lua-ecl.cc ecl-lua.pkg; \
	else \
		echo "$<:0: Warning: Can't update lua interface - tolua not installed "; \
	fi

