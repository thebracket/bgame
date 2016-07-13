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
    rainforest = {
        name = "Rainforest", min_temp = 20, max_temp = 34, min_rain = 25, max_rain = 100, min_mutation = 0, max_mutation = 100,        
        occurs = { biome_types["plains"], biome_types["hills"], biome_types["coast"] },
        worldgen_render = { glyph=glyphs['spades'], color=colors['tree_green'] }
    }
}