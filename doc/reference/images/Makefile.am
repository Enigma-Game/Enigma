
PNG = first_level.png

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
	it_banana.png \
	it_blocker.png \
	it_bomb_black.png \
	it_bomb_white.png \
	it_bottle_idle.png \
	it_bottle_broken.png \
	it_brush.png \
	it_burnable_ash.png \
	it_burnable_burning.png \
	it_burnable_fireproof.png \
	it_burnable_oil.png \
	it_cherry.png \
	it_coffee.png \
	it_coin_s.png \
	it_coin_m.png \
	it_coin_l.png \
	it_crack_abyss.png \
	it_crack_water.png \
	it_cross.png \
	it_death.png \
	it_document.png \
	it_drop.png \
	it_dynamite.png \
	it_explosion.png \
	it_extinguisher.png \
	it_extralife.png \
	it_flag_black.png \
	it_flag_white.png \
	it_floppy.png \
	it_glasses.png \
	it_glasses_broken.png \
	it_hammer.png \
	it_key.png \
	it_landmine.png \
	it_laser.png \
	it_magicwand.png \
	it_magnet_off.png \
	it_magnet_on.png \
	it_meditation_bump.png \
	it_meditation_caldera.png \
	it_meditation_dent.png \
	it_meditation_hill.png \
	it_meditation_hollow.png \
	it_meditation_volcano.png \
	it_pencil.png \
	it_pin.png \
	it_pipe.png \
	it_puller.png \
	it_ring.png \
	it_rubberband.png \
	it_seed.png \
	it_sensor.png \
	it_sensor_exit.png \
	it_shogun_m.png \
	it_shogun_l.png \
	it_shogun_s.png \
	it_spade.png \
	it_spoon.png \
	it_springboard.png \
	it_spring_drop.png \
	it_spring_keep.png \
	it_squashed.png \
	it_strip.png \
	it_surprise.png \
	it_sword.png \
	it_trap.png \
	it_trigger.png \
	it_umbrella.png \
	it_vortex.png \
	it_weight.png \
	it_wormhole.png \
	it_wrench.png \
	it_yinyang.png \
	st_ancient.png \
	st_bluegray.png \
	st_brake.png \
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

pngdir = @datadir@/doc/enigma/reference/images
png_DATA = $(PNG) $(PNGLIST)

.SUFFIXES = .png

all: $(PNGLIST) $(PDFLIST) first_level.pdf

clean:
	$(RM) $(PNGLIST)
	$(RM) $(PDFLIST)
	$(RM) first_level.pdf

first_level.pdf: first_level.png
	convert -density 112x112 $< $@

%.png: $(GFXDIR)/%.png
	convert -density 112x112 -units PixelsPerCentimeter -crop 48x48+0+0 $< $@

%.pdf: $(GFXDIR)/%.png
	convert -density 112x112 -units PixelsPerCentimeter -crop 48x48+0+0 $< $@

EXTRA_DIST = $(PNG)
