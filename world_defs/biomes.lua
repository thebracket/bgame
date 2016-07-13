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
        occurs = { biome_types["ocean"], biome_types["coast"] },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['white'] }
    },
    icy_ocean = {
        name = "Icy Ocean", min_temp = -10, max_temp = 10, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["ocean"], biome_types["coast"] },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['cyan'] }
    },
    ocean = {
        name = "Ocean", min_temp = 0, max_temp = 30, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["ocean"], biome_types["coast"] },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['blue'] }
    },
    hot_ocean = {
        name = "Tropical Ocean", min_temp = 30, max_temp = 100, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["ocean"], biome_types["coast"] },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['blue'] }
    },

    -- Coastal Zones
    arctic_coastal = {
        name = "Arctic Coast", min_temp = -100, max_temp = -5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["coast"] },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['white'] }
    },
    cold_coastal = {
        name = "Cold Coast", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["coast"] },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['cyan'] }
    },
    temperate_coastal = {
        name = "Temperate Coast", min_temp = 5, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["coast"] },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['green'] }
    },
    tropical_coastal = {
        name = "Tropical Coast", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["coast"] },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['light_green'] }
    },

    -- Salt-marsh Zones
    arctic_salt = {
        name = "Arctic Coast", min_temp = -100, max_temp = -5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["salt_marsh"] },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['white'] }
    },
    cold_salt = {
        name = "Cold Coast", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["salt_marsh"] },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['cyan'] }
    },
    temperate_salt = {
        name = "Temperate Coast", min_temp = 5, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["salt_marsh"] },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['green'] }
    },
    tropical_salt = {
        name = "Tropical Coast", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["salt_marsh"] },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['light_green'] }
    },

    -- Flatlands
    permafrost_plain = {
        name = "Permafrost Plain", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["coast"], biome_types["marsh"] },
        worldgen_render = { glyph=glyphs['one_half_solid'], color=colors['white'] }
    },
    rocky_plain = {
        name = "Rocky Plain", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["coast"], biome_types["marsh"] },
        worldgen_render = { glyph=glyphs['one_half_solid'], color=colors['grey'] }
    },
    grass_plain = {
        name = "Grass Plain", min_temp = 0, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["coast"], biome_types["marsh"] },
        worldgen_render = { glyph=glyphs['one_half_solid'], color=colors['green'] }
    },
    savannah_plain = {
        name = "Savannah Plain", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["coast"], biome_types["marsh"] },
        worldgen_render = { glyph=glyphs['one_half_solid'], color=colors['earth_brown'] }
    },

    -- Hills
    permafrost_hills = {
        name = "Permafrost Hills", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["hills"], biome_types["highlands"] },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['white'] }
    },
    rocky_hills = {
        name = "Rocky Hills", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["hills"], biome_types["highlands"] },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['grey'] }
    },
    grass_hills = {
        name = "Grass Hills", min_temp = 0, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["hills"], biome_types["highlands"] },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['green'] }
    },
    savannah_hills = {
        name = "Savannah Hills", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["hills"], biome_types["highlands"] },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['earth_brown'] }
    },

     -- Plateau
    permafrost_plateau = {
        name = "Permafrost Plateau", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plateau"] },
        worldgen_render = { glyph=glyphs['arch'], color=colors['white'] }
    },
    rocky_plateau = {
        name = "Rocky Plateau", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plateau"] },
        worldgen_render = { glyph=glyphs['arch'], color=colors['grey'] }
    },
    grass_plateau = {
        name = "Grass Plateau", min_temp = 0, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plateau"] },
        worldgen_render = { glyph=glyphs['arch'], color=colors['green'] }
    },
    savannah_plateau = {
        name = "Savannah Plateau", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plateau"] },
        worldgen_render = { glyph=glyphs['arch'], color=colors['earth_brown'] }
    },
    badland_plateau = {
        name = "Badlands", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 20, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plateau"] },
        worldgen_render = { glyph=glyphs['arch'], color=colors['red_sand'] }
    },

    -- Highlands
    permafrost_highlands = {
        name = "Permafrost Highlands", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["highlands"] },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['white'] }
    },
    rocky_highlands = {
        name = "Rocky Highlands", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["highlands"] },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['grey'] }
    },
    grass_highlands = {
        name = "Grass Highlands", min_temp = 0, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["highlands"] },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['green'] }
    },
    savannah_highlands = {
        name = "Savannah Highlands", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["highlands"] },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['earth_brown'] }
    },
    badland_highlands = {
        name = "Badland Highlands", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 20, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["highlands"] },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['red_sand'] }
    },

    -- Mountains
    permafrost_mountains = {
        name = "Permafrost Mountains", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"] },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['white'] }
    },
    rocky_mountains = {
        name = "Rocky Mountains", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"] },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['grey'] }
    },
    grass_mountains = {
        name = "Grass Mountains", min_temp = 0, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"] },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['green'] }
    },
    savannah_mountains = {
        name = "Savannah Mountains", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"] },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['earth_brown'] }
    },
    badland_mountains = {
        name = "Badlands", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 20, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"] },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['red_sand'] }
    },

    -- Low precipitation desert areas
    tundra = {
        name = "Tundra", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 10, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["hills"], biome_types["highlands"], biome_types["plateau"], biome_types["coast"] },
        worldgen_render = { glyph=glyphs['tilde'], color=colors['white'] }
    },
    cold_desert = {
        name = "Cold Desert", min_temp = 1, max_temp = 10, min_rain = 0, max_rain = 10, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["hills"], biome_types["highlands"], biome_types["plateau"], biome_types["coast"] },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['white'] }
    },
    sand_desert = {
        name = "Sand Desert", min_temp = 9, max_temp = 100, min_rain = 0, max_rain = 10, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["hills"], biome_types["coast"] },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['yellow_sand'] }
    },
    rock_desert = {
        name = "Rocky Desert", min_temp = 9, max_temp = 100, min_rain = 0, max_rain = 10, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"], biome_types["plateau"], biome_types["hills"], biome_types["coast"] },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['red_sand'] }
    },

    -- High-precipitation areas
    deciduous_broadleaf = {
        name = "Deciduous Broadleaf", min_temp = 5, max_temp = 34, min_rain = 15, max_rain = 100, min_mutation = 0, max_mutation = 100,        
        occurs = { biome_types["plains"], biome_types["hills"], biome_types["coast"] },
        worldgen_render = { glyph=glyphs['spades'], color=colors['tree_green'] }
    },
    deciduous_needleleaf = {
        name = "Deciduous Needleleaf", min_temp = 5, max_temp = 34, min_rain = 15, max_rain = 100, min_mutation = 0, max_mutation = 100,        
        occurs = { biome_types["highlands"], biome_types["hills"], biome_types["plateau"] },
        worldgen_render = { glyph=glyphs['clubs'], color=colors['tree_green'] }
    },
    evergreen_broadleef = {
        name = "Evergreen Broadleaf", min_temp = -5, max_temp = 20, min_rain = 15, max_rain = 100, min_mutation = 0, max_mutation = 100,        
        occurs = { biome_types["highlands"], biome_types["hills"], biome_types["plateau"] },
        worldgen_render = { glyph=glyphs['spades'], color=colors['tree_green'] }
    },
    evergreen_needleleef = {
        name = "Evergreen Broadleaf", min_temp = -15, max_temp = 30, min_rain = 15, max_rain = 100, min_mutation = 0, max_mutation = 100,        
        occurs = { biome_types["highlands"], biome_types["mountains"], biome_types["plateau"] },
        worldgen_render = { glyph=glyphs['spades'], color=colors['tree_green'] }
    },
    rainforest = {
        name = "Rainforest", min_temp = 20, max_temp = 34, min_rain = 25, max_rain = 100, min_mutation = 0, max_mutation = 100,        
        occurs = { biome_types["plains"], biome_types["hills"], biome_types["coast"] },
        worldgen_render = { glyph=glyphs['spades'], color=colors['tree_green'] }
    }
}