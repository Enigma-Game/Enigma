#
# Makefile prototype for Enigma.
#
bin_PROGRAMS = enigma


EXTRA_DIST = \
	enigma.ico \
	enigma.rc \
	enigma.doxygen


CXXFLAGS = \
	@SDL_CFLAGS@ \
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
	file_internal.hh	\
	floors.cc 		\
	floors.hh		\
	fwd.hh			\
	game.cc			\
	game.hh			\
	gui.cc 			\
	gui.hh			\
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
	help.cc 		\
	help.hh			\
	main.cc 		\
	main.hh			\
	menus.cc 		\
	menus.hh 		\
	menus_internal.hh	\
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
	gui/TextField.cc	\
	gui/TextField.hh

datadir = @datadir@

if MINGW32

localedir = data/locale
mingw_ldadd = enigma_ico.o -lws2_32
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
#TOLUA=@TOLUA@
#%-lua.cc %-lua.hh: %-lua.pkg
#	@echo "XXX: ${TOLUA}"
#	@if [ "x${TOLUA}" != "x" ]; then \
#	  echo "Running ${TOLUA}..."; \
#	  ${TOLUA} -H $*-lua.hh -n $(notdir $*) -o $@ $<; \
#	else \
#	  echo "$<:0: Warning: Can't update lua interface - tolua not installed "; \
#	fi

#------------------ Packaging rules for Mac OSX ------------------------

bundle_name = ../Enigma.app
bundle_locales = de.gmo
# Special target to copy all needed game files to Enigma.app/Contents/Resources.
# For Mac OS X bundle.
bundle:
	mkdir -p $(bundle_name)/Contents/MacOS
	mkdir -p $(bundle_name)/Contents/Resources
	mkdir -p $(bundle_name)/Contents/Resources/data/
	echo "APPL????" > $(bundle_name)/Contents/PkgInfo
	cp ../$(top_builddir)/etc/Info.plist $(bundle_name)/Contents/
	cp ../$(top_builddir)/etc/enigma.icns $(bundle_name)/Contents/Resources/
	cp -R ../$(top_builddir)/data/levels $(bundle_name)/Contents/Resources/data/
	cp ../$(top_builddir)/data/*.lua $(bundle_name)/Contents/Resources/data/
	mkdir -p $(bundle_name)/Contents/Resources/data/gfx
	cp -R ../$(top_builddir)/data/gfx/*.png $(bundle_name)/Contents/Resources/data/gfx
	mkdir -p $(bundle_name)/Contents/Resources/data/gfx32
	cp -R ../$(top_builddir)/data/gfx32/*.png $(bundle_name)/Contents/Resources/data/gfx32
	cp -R ../$(top_builddir)/data/gfx32/*.jpg $(bundle_name)/Contents/Resources/data/gfx32
	mkdir -p $(bundle_name)/Contents/Resources/data/gfx40
	cp -R ../$(top_builddir)/data/gfx40/*.png $(bundle_name)/Contents/Resources/data/gfx40
	cp -R ../$(top_builddir)/data/gfx40/*.jpg $(bundle_name)/Contents/Resources/data/gfx40
	mkdir -p $(bundle_name)/Contents/Resources/data/gfx48
	cp -R ../$(top_builddir)/data/gfx48/*.png $(bundle_name)/Contents/Resources/data/gfx48
	cp -R ../$(top_builddir)/data/gfx48/*.jpg $(bundle_name)/Contents/Resources/data/gfx48
	mkdir -p $(bundle_name)/Contents/Resources/data/sound
	cp -R ../$(top_builddir)/data/sound/*.wav $(bundle_name)/Contents/Resources/data/sound
	cp -R ../$(top_builddir)/data/sound/*.s3m $(bundle_name)/Contents/Resources/data/sound
	mkdir -p $(bundle_name)/Contents/Resources/data/fonts
	cp ../$(top_builddir)/data/fonts/*.png $(bundle_name)/Contents/Resources/data/fonts
	cp ../$(top_builddir)/data/fonts/*.bmf $(bundle_name)/Contents/Resources/data/fonts
	cp ../$(top_builddir)/data/fonts/*.ttf $(bundle_name)/Contents/Resources/data/fonts
	mkdir -p $(bundle_name)/Contents/Frameworks
	cp -r /Library/Frameworks/SDL.framework $(bundle_name)/Contents/Frameworks/
	cp -r /Library/Frameworks/SDL_image.framework $(bundle_name)/Contents/Frameworks/
	cp -r /Library/Frameworks/SDL_mixer.framework $(bundle_name)/Contents/Frameworks/
	@catalogs='$(bundle_locales)'; \
	for cat in $$catalogs; do \
	  lang=`echo $$cat | sed 's/\.gmo$$//'`; \
	  dir=$(bundle_name)/Contents/Resources/locale/$$lang/LC_MESSAGES; \
	  mkdir -p $$dir; \
	  echo "installing $$cat as $$dir/enigma.mo"; \
	  cp -r ../$(top_builddir)/po/$$cat $$dir/enigma.mo; \
	done

# Special target to copy the binary to Enigma.app/Contents/MacOS.
bundle-fw: enigma-fw-mosx
	cp enigma-fw $(bundle_name)/Contents/MacOS/enigma
	strip $(bundle_name)/Contents/MacOS/enigma

# Special target to create a binary linked with Mac OS X frameworks
enigma-fw-mosx: $(enigma_OBJECTS) $(enigma_DEPENDENCIES)
	$(CXX) $(LDFLAGS) -o enigma-fw $(enigma_OBJECTS) $(enigma_DEPENDENCIES) /usr/fink/lib/libintl.a /usr/lib/libiconv.dylib -lsdlmain -lz -framework SDL -framework SDL_image -framework SDL_mixer -framework IOKit -framework Cocoa -framework OpenGL -framework Carbon -framework QuickTime -framework SDL_ttf /usr/fink/lib/libpng.a
