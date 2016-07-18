biome_types = {
    ocean=1,
    plains=2,
    hills=3,
    mountains=4,
    marsh=5,
    plateau=6,
    highlands=7,
    coast=8,
    salt_marsh=9
}

biomes = {
    -- Ocean areas
    frozen_ocean = {
        name = "Frozen Ocean", min_temp = -100, max_temp = -5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["ocean"], biome_types["coast"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['white'] },
        plants = { none=20, grass=10, grass_long=5, grass_short=5, lavendar=1, daisy=1, reeds=2 }
    },
    icy_ocean = {
        name = "Icy Ocean", min_temp = -10, max_temp = 10, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["ocean"], biome_types["coast"], soils = { soil=50, sand=50 } },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['cyan'] },
        plants = { none=10, grass=10, grass_long=5, grass_short=5, lavendar=1, daisy=1, reeds=2 }
    },
    ocean = {
        name = "Ocean", min_temp = 0, max_temp = 30, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["ocean"], biome_types["coast"], soils = { soil=50, sand=50 } },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['blue'] },
        plants = { none=5, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    hot_ocean = {
        name = "Tropical Ocean", min_temp = 30, max_temp = 100, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["ocean"], biome_types["coast"], soils = { soil=50, sand=50 } },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['blue'] },
        plants = { none=5, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },

    -- Coastal Zones
    arctic_coastal = {
        name = "Arctic Coast", min_temp = -100, max_temp = -5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["coast"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['white'] },
        plants = { none=20, grass=10, grass_long=5, grass_short=5, lavendar=1, daisy=1, reeds=2, heather=2 }
    },
    cold_coastal = {
        name = "Cold Coast", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["coast"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['cyan'] },
        plants = { none=10, grass=10, grass_long=5, grass_short=5, lavendar=1, daisy=1, reeds=2, heather=2 }
    },
    temperate_coastal = {
        name = "Temperate Coast", min_temp = 5, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["coast"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['green'] },
        plants = { none=5, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    tropical_coastal = {
        name = "Tropical Coast", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["coast"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['light_green'] },
        plants = { none=5, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },

    -- Salt-marsh Zones
    arctic_salt = {
        name = "Arctic Coast", min_temp = -100, max_temp = -5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["salt_marsh"] }, soils = { soil=70, sand=30 },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['white'] },
        plants = { none=20, grass=10, grass_long=5, grass_short=5, lavendar=1, daisy=1, reeds=2 }
    },
    cold_salt = {
        name = "Cold Coast", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["salt_marsh"] }, soils = { soil=70, sand=30 },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['cyan'] },
        plants = { none=10, grass=10, grass_long=5, grass_short=5, lavendar=1, daisy=1, reeds=2 }
    },
    temperate_salt = {
        name = "Temperate Coast", min_temp = 5, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["salt_marsh"] }, soils = { soil=70, sand=30 },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['green'] }
    },
    tropical_salt = {
        name = "Tropical Coast", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["salt_marsh"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['light_green'] }
    },

    -- Flatlands
    permafrost_plain = {
        name = "Permafrost Plain", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["coast"], biome_types["marsh"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_half_solid'], color=colors['white'] },
        plants = { none=20, grass=10, grass_long=5, grass_short=5, lavendar=1, daisy=1, reeds=2, heather=2 }
    },
    rocky_plain = {
        name = "Rocky Plain", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["coast"], biome_types["marsh"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_half_solid'], color=colors['grey'] },
        plants = { none=25, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    grass_plain = {
        name = "Grass Plain", min_temp = 0, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["coast"], biome_types["marsh"] }, soils = { soil=75, sand=25 },
        worldgen_render = { glyph=glyphs['one_half_solid'], color=colors['green'] },
        plants = { none=3, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    savannah_plain = {
        name = "Savannah Plain", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["coast"], biome_types["marsh"] }, soils = { soil=25, sand=75 },
        worldgen_render = { glyph=glyphs['one_half_solid'], color=colors['earth_brown'] },
        plants = { none=3, grass=10, grass_long=10, grass_short=5, sage=1, daisy=1, reeds=2 }
    },

    -- Hills
    permafrost_hills = {
        name = "Permafrost Hills", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["hills"], biome_types["highlands"] }, soils = { soil=80, sand=20 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['white'] },
        plants = { none=20, grass=10, grass_long=5, grass_short=5, lavendar=1, daisy=1, reeds=2, heather=2 }
    },
    rocky_hills = {
        name = "Rocky Hills", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["hills"], biome_types["highlands"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['grey'] },
        plants = { none=30, grass=10, grass_long=5, grass_short=5, lavendar=1, daisy=1, reeds=2, heather=2 }
    },
    grass_hills = {
        name = "Grass Hills", min_temp = 0, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["hills"], biome_types["highlands"] }, soils = { soil=75, sand=25 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['green'] },
        plants = { none=5, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    savannah_hills = {
        name = "Savannah Hills", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["hills"], biome_types["highlands"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['earth_brown'] },
        plants = { none=5, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },

     -- Plateau
    permafrost_plateau = {
        name = "Permafrost Plateau", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plateau"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['arch'], color=colors['white'] },
        plants = { none=20, grass=10, grass_long=5, grass_short=5, lavendar=1, daisy=1, reeds=2, heather=2 }
    },
    rocky_plateau = {
        name = "Rocky Plateau", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plateau"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['arch'], color=colors['grey'] },
        plants = { none=15, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    grass_plateau = {
        name = "Grass Plateau", min_temp = 0, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plateau"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['arch'], color=colors['green'] },
        plants = { none=5, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    savannah_plateau = {
        name = "Savannah Plateau", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plateau"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['arch'], color=colors['earth_brown'] },
        plants = { none=2, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    badland_plateau = {
        name = "Badlands", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 20, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plateau"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['arch'], color=colors['red_sand'] }
    },

    -- Highlands
    permafrost_highlands = {
        name = "Permafrost Highlands", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["highlands"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['white'] },
        plants = { none=20, grass=10, grass_long=5, grass_short=5, lavendar=1, daisy=1, reeds=2, heather=2 }
    },
    rocky_highlands = {
        name = "Rocky Highlands", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["highlands"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['grey'] },
        plants = { none=25, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    grass_highlands = {
        name = "Grass Highlands", min_temp = 0, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["highlands"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['green'] },
        plants = { none=5, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    savannah_highlands = {
        name = "Savannah Highlands", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["highlands"] }, soils = { soil=25, sand=75 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['earth_brown'] },
        plants = { none=2, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    badland_highlands = {
        name = "Badland Highlands", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 20, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["highlands"] }, soils = { soil=10, sand=90 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['red_sand'] }
    },

    -- Mountains
    permafrost_mountains = {
        name = "Permafrost Mountains", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['white'] },
        plants = { none=40, grass=10, grass_long=5, grass_short=5, lavendar=1, daisy=1, reeds=2, heather=2 }
    },
    rocky_mountains = {
        name = "Rocky Mountains", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['grey'] },
        plants = { none=25, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    grass_mountains = {
        name = "Grass Mountains", min_temp = 0, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['green'] },
        plants = { none=5, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    savannah_mountains = {
        name = "Savannah Mountains", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"] }, soils = { soil=25, sand=75 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['earth_brown'] },
        plants = { none=2, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    badland_mountains = {
        name = "Badlands", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 20, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"] }, soils = { soil=10, sand=90 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['red_sand'] }
    },

    -- Low precipitation desert areas
    tundra = {
        name = "Tundra", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 10, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["hills"], biome_types["highlands"], biome_types["plateau"], biome_types["coast"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['tilde'], color=colors['white'] },
        plants = { none=40, grass=10, grass_long=5, grass_short=5, lavendar=1, daisy=1, reeds=2, heather=2 }
    },
    cold_desert = {
        name = "Cold Desert", min_temp = 1, max_temp = 10, min_rain = 0, max_rain = 10, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["hills"], biome_types["highlands"], biome_types["plateau"], biome_types["coast"] }, soils = { soil=15, sand=85 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['white'] },
        plants = { none=40, grass=10, grass_long=5, grass_short=5, lavendar=1, daisy=1, reeds=2, heather=2 }
    },
    sand_desert = {
        name = "Sand Desert", min_temp = 9, max_temp = 100, min_rain = 0, max_rain = 10, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["hills"], biome_types["coast"] }, soils = { soil=5, sand=95 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['yellow_sand'] }
    },
    rock_desert = {
        name = "Rocky Desert", min_temp = 9, max_temp = 100, min_rain = 0, max_rain = 10, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"], biome_types["plateau"], biome_types["hills"], biome_types["coast"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['red_sand'] }
    },

    -- High-precipitation areas
    deciduous_broadleaf = {
        name = "Deciduous Broadleaf", min_temp = 5, max_temp = 34, min_rain = 15, max_rain = 100, min_mutation = 0, max_mutation = 100,        
        occurs = { biome_types["plains"], biome_types["hills"], biome_types["coast"] }, soils = { soil=80, sand=20 },
        worldgen_render = { glyph=glyphs['spades'], color=colors['tree_green'] },
        plants = { none=5, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    deciduous_needleleaf = {
        name = "Deciduous Needleleaf", min_temp = 5, max_temp = 34, min_rain = 15, max_rain = 100, min_mutation = 0, max_mutation = 100,        
        occurs = { biome_types["highlands"], biome_types["hills"], biome_types["plateau"] }, soils = { soil=80, sand=20 },
        worldgen_render = { glyph=glyphs['clubs'], color=colors['tree_green'] },
        plants = { none=5, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    evergreen_broadleef = {
        name = "Evergreen Broadleaf", min_temp = -5, max_temp = 20, min_rain = 15, max_rain = 100, min_mutation = 0, max_mutation = 100,        
        occurs = { biome_types["highlands"], biome_types["hills"], biome_types["plateau"] }, soils = { soil=80, sand=20 },
        worldgen_render = { glyph=glyphs['spades'], color=colors['tree_green'] },
        plants = { none=5, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    evergreen_needleleef = {
        name = "Evergreen Broadleaf", min_temp = -15, max_temp = 30, min_rain = 15, max_rain = 100, min_mutation = 0, max_mutation = 100,        
        occurs = { biome_types["highlands"], biome_types["mountains"], biome_types["plateau"] }, soils = { soil=80, sand=20 },
        worldgen_render = { glyph=glyphs['spades'], color=colors['tree_green'] },
        plants = { none=5, grass=10, grass_long=5, grass_short=5, sage=1, daisy=1, reeds=2 }
    },
    rainforest = {
        name = "Rainforest", min_temp = 20, max_temp = 34, min_rain = 25, max_rain = 100, min_mutation = 0, max_mutation = 100,        
        occurs = { biome_types["plains"], biome_types["hills"], biome_types["coast"] }, soils = { soil=90, sand=10 },
        worldgen_render = { glyph=glyphs['spades'], color=colors['tree_green'] }
    }
}