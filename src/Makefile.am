#
# Makefile prototype for Enigma.
#
bin_PROGRAMS = enigma


EXTRA_DIST = \
	enigma.ico \
	enigma.rc \
	enigma.doxygen


CXXFLAGS = @CXXFLAGS@\
	-DLOCALEDIR=\"$(localedir)\" \
	-I$(top_srcdir)/lib-src/zipios++  \
	-I$(top_builddir)/lib-src/zipios++ \
	-I$(top_srcdir)/lib-src/lua \
	-I$(top_srcdir)/lib-src/enigma-core \
	-I$(top_srcdir)/lib-src/enet/include \
	-I$(top_srcdir)/lib-src \
	-I$(top_builddir)/intl


enigma_SOURCES = 		\
	actors.hh 		\
	actors.cc 		\
	actors_internal.hh	\
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
	file.cc 		\
	file.hh 		\
	file_zip.cc 		\
	floors.cc 		\
	floors.hh		\
	fwd.hh			\
	game.cc			\
	game.hh			\
	Inventory.cc		\
	Inventory.hh		\
	ItemHolder.hh		\
	items.cc 		\
	items.hh		\
	laser.cc 		\
	laser.hh		\
	levelpack.cc		\
	levels.cc 		\
	levels.hh		\
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
	netgame.cc 		\
	netgame.hh 		\
	network.cc		\
	network.hh		\
	nls.cc 			\
	nls.hh			\
	objects.cc 		\
	objects.hh 		\
	objects_decl.hh 	\
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
	server.cc 		\
	server.hh		\
	sound.cc 		\
	sound.hh 		\
	sound_internal.hh	\
	stones.cc 		\
	stones.hh 		\
	stones_internal.hh 	\
	stones_simple.cc 	\
	stones_complex.cc  	\
	st_switches.cc		\
	Utf8ToXML.cc		\
	Utf8ToXML.hh		\
	util.cc 		\
	util.hh			\
	video.cc 		\
	video.hh		\
	world.cc 		\
	world.hh 		\
	world_internal.hh	\
	XMLtoLocal.cc		\
	XMLtoLocal.hh		\
	XMLtoUtf8.cc		\
	XMLtoUtf8.hh		\
	gui/HelpMenu.cc		\
	gui/HelpMenu.hh		\
	gui/LevelInspector.cc	\
	gui/LevelInspector.hh	\
	gui/menus.cc		\
	gui/menus.hh		\
	gui/menus_internal.hh	\
	gui/MonospacedLabel.cc	\
	gui/MonospacedLabel.hh	\
	gui/ScreenshotViewer.cc	\
	gui/ScreenshotViewer.hh	\
	gui/TextField.cc	\
	gui/TextField.hh	\
	gui/widgets.cc		\
	gui/widgets.hh		\
	lev/RatingManager.cc	\
	lev/RatingManager.hh	\
	lev/Proxy.cc		\
	lev/Proxy.hh

datadir = @datadir@

if MINGW32

  localedir := data/locale
  mingw_ldadd := enigma_ico.o -lws2_32
  enigma_ico.o: enigma.rc enigma.ico
	$(WINDRES) $(srcdir)/enigma.rc --include-dir=$(srcdir) -O coff -o enigma_ico.o

else # not MINGW32

  localedir = $(datadir)/locale
  mingw_ldadd =

endif


enigma_LIBS = \
	@SDL_LIBS@


enigma_LDADD = \
	@LIBINTL@ \
	$(top_builddir)/lib-src/enigma-core/libecl.a \
	$(top_builddir)/lib-src/lua/liblua.a \
	$(top_builddir)/lib-src/enet/libenet.a \
	$(top_builddir)/lib-src/oxydlib/liboxyd.a \
	$(top_builddir)/lib-src/zipios++/src/libzipios.a \
	-lz \
	$(mingw_ldadd)

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

bundle_name := ../etc/Enigma.app
# Special target to copy all needed game files to Enigma.app/Contents/Resources.
# For Mac OS X bundle.
.PHONY: bundle
bundle:
	-rm -rf $(bundle_name)/Contents
	mkdir -p $(bundle_name)/Contents/MacOS
	mkdir -p $(bundle_name)/Contents/Resources
	mkdir -p $(bundle_name)/Contents/Resources/data/
	echo "APPL????" > $(bundle_name)/Contents/PkgInfo
	cp $(top_builddir)/etc/Info.plist $(bundle_name)/Contents/
	cp $(top_builddir)/etc/enigma.icns $(bundle_name)/Contents/Resources/
	tar --exclude .svn -c -f - -C $(top_builddir)/data/levels . | tar xpf - -C $(bundle_name)/Contents/Resources/data
	cp $(top_builddir)/data/*.lua $(bundle_name)/Contents/Resources/data/
	cp $(top_builddir)/data/*.xml $(bundle_name)/Contents/Resources/data/
	mkdir -p $(bundle_name)/Contents/Resources/data/gfx
	cp $(top_builddir)/data/gfx/*.png $(bundle_name)/Contents/Resources/data/gfx
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
	mkdir -p $(bundle_name)/Contents/Resources/data/sound
	cp $(top_builddir)/data/sound/*.wav $(bundle_name)/Contents/Resources/data/sound
	cp $(top_builddir)/data/sound/*.s3m $(bundle_name)/Contents/Resources/data/sound
	mkdir -p $(bundle_name)/Contents/Resources/data/fonts
	cp $(top_builddir)/data/fonts/*.png $(bundle_name)/Contents/Resources/data/fonts
	cp $(top_builddir)/data/fonts/*.bmf $(bundle_name)/Contents/Resources/data/fonts
	cp $(top_builddir)/data/fonts/*.ttf $(bundle_name)/Contents/Resources/data/fonts
	mkdir -p $(bundle_name)/Contents/Frameworks
	cp -R /Library/Frameworks/SDL.framework $(bundle_name)/Contents/Frameworks/
	cp -R /Library/Frameworks/SDL_image.framework $(bundle_name)/Contents/Frameworks/
	cp -R /Library/Frameworks/SDL_mixer.framework $(bundle_name)/Contents/Frameworks/
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

# Special target to create a binary linked with Mac OS X frameworks
static_prefix := `$(SDL_CONFIG) --prefix`
.PHONY: enigma-fw-mosx
enigma-fw-mosx: $(enigma_OBJECTS) $(enigma_DEPENDENCIES)
	$(CXX) $(LDFLAGS) -o enigma-fw $(enigma_OBJECTS) $(enigma_DEPENDENCIES) $(static_prefix)/lib/libintl.a $(static_prefix)/lib/libxerces-c.a /usr/lib/libiconv.dylib -L$(static_prefix)/lib -lsdlmain -lz -framework SDL -framework SDL_image -framework SDL_mixer -framework Cocoa -framework OpenGL -framework SDL_ttf $(static_prefix)/lib/libpng.a 
