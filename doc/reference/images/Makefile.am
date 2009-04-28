
PNG = $(wildcard $(srcdir)/*.png)

GFXDIR = ../../../data/gfx48

# List of files to copy from GFXDIR
PNGLIST = \
	fl_abyss.png \
	fl_adhesionless.png \
	fl_aquamarine.png \
	fl_bast.png \
	fl_bluegray.png \
	fl_bluegreen.png \
	fl_blueslab.png \
	fl_brick.png \
	fl_bright.png \
	fl_concrete.png \
	fl_dark.png \
	fl_darkgray.png \
	fl_dunes.png \
	fl_fake_trigger.png \
	fl_gravel.png \
	fl_gray.png \
	fl_hay.png \
	fl_himalaya.png \
	fl_ice.png \
	fl_ice_heating.png \
	fl_inverse_gray.png \
	fl_inverse_white.png \
	fl_ivory.png \
	fl_lawn.png \
	fl_marble.png \
	fl_metal.png \
	fl_mortar.png \
	fl_pinkbumps.png \
	fl_plank.png \
	fl_platinum.png \
	fl_red.png \
	fl_redslab.png \
	fl_rock.png \
	fl_rough.png \
	fl_sahara.png \
	fl_samba.png \
	fl_sand.png \
	fl_space.png \
	fl_stone.png \
	fl_swamp.png \
	fl_swamp_heating.png \
	fl_tigris.png \
	fl_water.png \
	fl_water_heating.png \
	fl_white.png \
	fl_woven.png \
	fl_yinyang_yang.png \
	fl_yinyang_yin.png \
	it_bag.png \
	it_pin.png \
	it_puller.png \
	it_sensor.png \
	it_springboard.png \
	it_spring_drop.png \
	it_spring_keep.png \
	it_strip.png \
	it_surprise.png \
	it_trap.png \
	it_weight.png \
	it_yinyang.png \
	st_ancient.png \
	st_bluegray.png \
	st_brownie.png \
	st_brownmarble.png \
	st_camouflage.png \
	st_concrete.png \
	st_darkgray.png \
	st_granite.png \
	st_greengray.png \
	st_metal.png \
	st_pinkbumps.png \
	st_purplegray.png \
	st_purplemarble.png \
	st_redbrown.png \
	st_redmarble.png \
	st_redrock.png \
	st_tigris.png \
	st_woven.png \
	st_yellow.png

PNGSOURCE = $(addprefix $(GFXDIR), $(PNGLIST))

# PDFLIST is deprecated (on elder systems pdflatex needs pdf images)
PDFLIST = $(addsuffix .pdf, $(basename $(PNGLIST)))

.SUFFIXES = .png

all: $(PNGLIST) $(PDFLIST)

clean:
	$(RM) $(PNGLIST)
	$(RM) $(PDFLIST)


%.png: $(GFXDIR)/%.png
	convert -density 112x112 -units PixelsPerCentimeter -crop 48x48+0+0 $< $@

%.pdf: $(GFXDIR)/%.png
	convert -density 112x112 -units PixelsPerCentimeter -crop 48x48+0+0 $< $@

EXTRA_DIST = $(PNG) $(PNGLIST)
