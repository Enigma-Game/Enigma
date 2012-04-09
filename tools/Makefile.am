# Build font converter only if SDL_ttf library is present
# and developer tools were explicitly requested (cf. configure.ac).

EXTRA_DIST = merge_level_i18n.lua sub_quote_gettext.lua 

if BUILDTTF2BMF
TTFTOOL = ttf2bmf
else
TTFTOOL = 
endif

noinst_PROGRAMS = tolua $(TTFTOOL)

ttf2bmf_SOURCES = ttf2bmf.cc getopt.c getopt1.c getopt.h
ttf2bmf_CXXFLAGS = @SDL_CFLAGS@ $(AM_CXXFLAGS)
ttf2bmf_LDADD = -lSDL_ttf $(AM_LDADD)

tolua_SOURCES = tolua.c toluabind.c toluabind.h dummy.cc 

tolua_CFLAGS = -I$(top_srcdir)/lib-src/lua $(AM_CFLAGS)
tolua_LDADD = $(top_builddir)/lib-src/lua/liblua.a $(AM_LDADD)

if CXXLUA
tolua_CFLAGS += -x c++ $(AM_CXXFLAGS) $(CXXFLAGS)
endif
