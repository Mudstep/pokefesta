MENUGFXDIR := graphics/interface
INTROGFXDIR := graphics/intro
PSSGFXDIR := graphics/pokemon_storage

$(MENUGFXDIR)/menu.gbapal: $(MENUGFXDIR)/menu_0.gbapal $(MENUGFXDIR)/menu_1.gbapal

	@cat $(MENUGFXDIR)/menu_0.gbapal $(MENUGFXDIR)/menu_1.gbapal >$@
	
$(INTROGFXDIR)/intro.4bpp: $(INTROGFXDIR)/intro1_waterdrops.4bpp $(INTROGFXDIR)/intro1_gamefreak.4bpp

	@cat $(INTROGFXDIR)/intro1_waterdrops.4bpp $(INTROGFXDIR)/intro1_gamefreak.4bpp >$@
	
$(INTROGFXDIR)/introgfx.4bpp: $(INTROGFXDIR)/intro1_bg.4bpp $(INTROGFXDIR)/intro1_leaves.4bpp

	@cat $(INTROGFXDIR)/intro1_bg.4bpp $(INTROGFXDIR)/intro1_leaves.4bpp >$@
	
$(MENUGFXDIR)/map.8bpp: $(MENUGFXDIR)/map.png
	$(GFX) $< $@ -num_tiles 233
	
$(PSSGFXDIR)/forest_frame.4bpp: $(PSSGFXDIR)/forest_frame.png
	$(GFX) $< $@ -num_tiles 49

$(PSSGFXDIR)/forest.4bpp: $(PSSGFXDIR)/forest_frame.4bpp $(PSSGFXDIR)/forest_bg.4bpp
	@cat $(PSSGFXDIR)/forest_frame.4bpp $(PSSGFXDIR)/forest_bg.4bpp >$@
	
$(PSSGFXDIR)/city_frame.4bpp: $(PSSGFXDIR)/city_frame.png
	$(GFX) $< $@ -num_tiles 37

$(PSSGFXDIR)/city.4bpp: $(PSSGFXDIR)/city_frame.4bpp $(PSSGFXDIR)/city_bg.4bpp
	@cat $(PSSGFXDIR)/city_frame.4bpp $(PSSGFXDIR)/city_bg.4bpp >$@
	
$(PSSGFXDIR)/desert_frame.4bpp: $(PSSGFXDIR)/desert_frame.png
	$(GFX) $< $@ -num_tiles 48

$(PSSGFXDIR)/desert.4bpp: $(PSSGFXDIR)/desert_frame.4bpp $(PSSGFXDIR)/desert_bg.4bpp
	@cat $(PSSGFXDIR)/desert_frame.4bpp $(PSSGFXDIR)/desert_bg.4bpp >$@
	
$(PSSGFXDIR)/savanna_frame.4bpp: $(PSSGFXDIR)/savanna_frame.png
	$(GFX) $< $@ -num_tiles 40

$(PSSGFXDIR)/savanna.4bpp: $(PSSGFXDIR)/savanna_frame.4bpp $(PSSGFXDIR)/savanna_bg.4bpp
	@cat $(PSSGFXDIR)/savanna_frame.4bpp $(PSSGFXDIR)/savanna_bg.4bpp >$@
	
$(PSSGFXDIR)/crag_frame.4bpp: $(PSSGFXDIR)/crag_frame.png
	$(GFX) $< $@ -num_tiles 46

$(PSSGFXDIR)/crag.4bpp: $(PSSGFXDIR)/crag_frame.4bpp $(PSSGFXDIR)/crag_bg.4bpp
	@cat $(PSSGFXDIR)/crag_frame.4bpp $(PSSGFXDIR)/crag_bg.4bpp >$@
	
$(PSSGFXDIR)/volcano_frame.4bpp: $(PSSGFXDIR)/volcano_frame.png
	$(GFX) $< $@ -num_tiles 49

$(PSSGFXDIR)/volcano.4bpp: $(PSSGFXDIR)/volcano_frame.4bpp $(PSSGFXDIR)/volcano_bg.4bpp
	@cat $(PSSGFXDIR)/volcano_frame.4bpp $(PSSGFXDIR)/volcano_bg.4bpp >$@
	
$(PSSGFXDIR)/snow_frame.4bpp: $(PSSGFXDIR)/snow_frame.png
	$(GFX) $< $@ -num_tiles 46

$(PSSGFXDIR)/snow.4bpp: $(PSSGFXDIR)/snow_frame.4bpp $(PSSGFXDIR)/snow_bg.4bpp
	@cat $(PSSGFXDIR)/snow_frame.4bpp $(PSSGFXDIR)/snow_bg.4bpp >$@
	
$(PSSGFXDIR)/cave_frame.4bpp: $(PSSGFXDIR)/cave_frame.png
	$(GFX) $< $@ -num_tiles 57

$(PSSGFXDIR)/cave.4bpp: $(PSSGFXDIR)/cave_frame.4bpp $(PSSGFXDIR)/cave_bg.4bpp
	@cat $(PSSGFXDIR)/cave_frame.4bpp $(PSSGFXDIR)/cave_bg.4bpp >$@

$(PSSGFXDIR)/beach_frame.4bpp: $(PSSGFXDIR)/beach_frame.png
	$(GFX) $< $@ -num_tiles 48

$(PSSGFXDIR)/beach.4bpp: $(PSSGFXDIR)/beach_frame.4bpp $(PSSGFXDIR)/beach_bg.4bpp
	@cat $(PSSGFXDIR)/beach_frame.4bpp $(PSSGFXDIR)/beach_bg.4bpp >$@

$(PSSGFXDIR)/seafloor_frame.4bpp: $(PSSGFXDIR)/seafloor_frame.png
	$(GFX) $< $@ -num_tiles 45

$(PSSGFXDIR)/seafloor.4bpp: $(PSSGFXDIR)/seafloor_frame.4bpp $(PSSGFXDIR)/seafloor_bg.4bpp
	@cat $(PSSGFXDIR)/seafloor_frame.4bpp $(PSSGFXDIR)/seafloor_bg.4bpp >$@

$(PSSGFXDIR)/river_frame.4bpp: $(PSSGFXDIR)/river_frame.png
	$(GFX) $< $@ -num_tiles 57

$(PSSGFXDIR)/river.4bpp: $(PSSGFXDIR)/river_frame.4bpp $(PSSGFXDIR)/river_bg.4bpp
	@cat $(PSSGFXDIR)/river_frame.4bpp $(PSSGFXDIR)/river_bg.4bpp >$@

$(PSSGFXDIR)/sky_frame.4bpp: $(PSSGFXDIR)/sky_frame.png
	$(GFX) $< $@ -num_tiles 45

$(PSSGFXDIR)/sky.4bpp: $(PSSGFXDIR)/sky_frame.4bpp $(PSSGFXDIR)/sky_bg.4bpp
	@cat $(PSSGFXDIR)/sky_frame.4bpp $(PSSGFXDIR)/sky_bg.4bpp >$@

$(PSSGFXDIR)/polkadot_frame.4bpp: $(PSSGFXDIR)/polkadot_frame.png
	$(GFX) $< $@ -num_tiles 34

$(PSSGFXDIR)/polkadot.4bpp: $(PSSGFXDIR)/polkadot_frame.4bpp $(PSSGFXDIR)/polkadot_bg.4bpp
	@cat $(PSSGFXDIR)/polkadot_frame.4bpp $(PSSGFXDIR)/polkadot_bg.4bpp >$@

$(PSSGFXDIR)/pokecenter_bg.4bpp: $(PSSGFXDIR)/pokecenter_bg.png
	$(GFX) $< $@ -num_tiles 38

$(PSSGFXDIR)/pokecenter.4bpp: $(PSSGFXDIR)/pokecenter_frame.4bpp $(PSSGFXDIR)/pokecenter_bg.4bpp
	@cat $(PSSGFXDIR)/pokecenter_frame.4bpp $(PSSGFXDIR)/pokecenter_bg.4bpp >$@

$(PSSGFXDIR)/machine_frame.4bpp: $(PSSGFXDIR)/machine_frame.png
	$(GFX) $< $@ -num_tiles 46

$(PSSGFXDIR)/machine.4bpp: $(PSSGFXDIR)/machine_frame.4bpp $(PSSGFXDIR)/machine_bg.4bpp
	@cat $(PSSGFXDIR)/machine_frame.4bpp $(PSSGFXDIR)/machine_bg.4bpp >$@

$(PSSGFXDIR)/plain_frame.4bpp: $(PSSGFXDIR)/plain_frame.png
	$(GFX) $< $@ -num_tiles 18

$(PSSGFXDIR)/plain.4bpp: $(PSSGFXDIR)/plain_frame.4bpp $(PSSGFXDIR)/plain_bg.4bpp
	@cat $(PSSGFXDIR)/plain_frame.4bpp $(PSSGFXDIR)/plain_bg.4bpp >$@
