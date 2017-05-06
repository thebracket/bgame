-- Workshops that mostly just convert raw materials into other materials

------------------------------------------------------------------------------------------------------------------------
-- Sawmills are used for converting wood into blocks, and offcuts.
------------------------------------------------------------------------------------------------------------------------

buildings["sawmill"] = {
    name = "Sawmill",
    components = { { item="wood_log", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['sawmill_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['sawmill_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['sawmill_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['sawmill_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['three_bar'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['three_bar'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['sun'], foreground = colors['cyan'], background = colors['black']}
        }
    },
};

reactions["cut_wooden_planks"] = {
   name = "Cut Wooden Logs into Blocks",
   workshop = "sawmill",
   inputs = { { item="wood_log", qty=1 } },
   outputs = { { item="block", qty=4 }, { item="wood_waste", qty=2} },
   skill = "Carpentry",
   difficulty = 10,
   automatic = true
};

------------------------------------------------------------------------------------------------------------------------
-- Tanneries combine hide with dung, and use it to make leather.
------------------------------------------------------------------------------------------------------------------------

buildings["tanner"] = {
    name = "Tanner",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['tanner_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['tanner_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['tanner_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['tanner_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['double_wall_nse'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['double_wall_nsw'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['wood_brown'], background = colors['black']}
        }
    },
};

reactions["tan_leather"] = {
   name = "Tan Leather",
   workshop = "tanner",
   inputs = { { item="hide", qty=1 } },
   outputs = { { item="leather", qty=2, special="tanning" } },
   skill = "Tanning",
   difficulty = 10,
   automatic = true
};

------------------------------------------------------------------------------------------------------------------------
-- Stonecutters are where one cuts stone. Kinda obvious, I guess.
------------------------------------------------------------------------------------------------------------------------
buildings["stonecutter"] = {
    name = "Stonecutters",
    components = { { item="stone_boulder", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['stonecutter_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['stonecutter_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['stonecutter_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['stonecutter_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['paragraph'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['sun'], foreground = colors['white'], background = colors['black']}
        }
    },
};

reactions["cut_stone"] = {
    name = "Cut Stone Blocks",
    workshop = "stonecutter",
    inputs = { { item="stone_boulder", qty=1 } },
    outputs = { { item="block", qty=4 } },
    skill = "Masonry",
    difficulty = 10,
    automatic = true
};

------------------------------------------------------------------------------------------------------------------------
-- Charcoal huts slow-cook wood to make a super-fuel. They can also be used to refine coal.
------------------------------------------------------------------------------------------------------------------------

buildings["charcoal_hut"] = {
    name = "Charcoal Burner",
    components = { { item="block", qty=1, mat_type="rock" } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['charcoal_hut'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['arch'], foreground = colors['wood_brown'], background = colors['black']}
        }
    },
};

reactions["charcoal_waste"] = {
    name = "Make Charcoal from Waste",
    workshop = "charcoal_hut",
    inputs = { { item="wood_waste", qty=1 } },
    outputs = { { item="charcoal", qty=2 }, { item="ash", qty=1} },
    skill = "Furnace Operation",
    difficulty = 10,
    automatic = true,
    emits_smoke = true
};

------------------------------------------------------------------------------------------------------------------------
-- Smelters extract metal from ore.
------------------------------------------------------------------------------------------------------------------------

buildings["smelter"] = {
    name = "Smelter",
    components = { { item="block", qty=1, mat_type="rock" } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['smelter'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['arch'], foreground = colors['red'], background = colors['black']}
        }
    },
};

reactions["smelt_ore"] = {
   name = "Smelt Ore",
   workshop = "smelter",
   inputs = { { item="ore", qty=1 } },
   outputs = { { item="block", qty=2 } },
   skill = "Furnace Operation",
   difficulty = 12,
   automatic = true
};

------------------------------------------------------------------------------------------------------------------------
-- Glass furnaces heat up sand to make raw glass.
------------------------------------------------------------------------------------------------------------------------
buildings["glass_furnace"] = {
    name = "Glass Furnace",
    components = { { item="block", qty=1, mat_type="rock" } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['glass_furnace'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['arch'], foreground = colors['light_green'], background = colors['black']}
        }
    },
};

reactions["make_raw_glass"] = {
    name = "Make Raw Glass",
    workshop = "glass_furnace",
    inputs = { { item="charcoal", qty=1 }, { item="sand", qty=1} },
    outputs = { { item="raw_glass", qty=2 } },
    skill = "Glassmaking",
    difficulty = 10,
    automatic = false,
    emits_smoke = true
};

------------------------------------------------------------------------------------------------------------------------
-- Silicon refineries convert glass into raw silicon.
------------------------------------------------------------------------------------------------------------------------
buildings["silicon_refinery"] = {
    name = "Silicon Refinery",
    components = { { item="block", qty=1, mat_type="rock" }, { item="raw_glass", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['silicon_refinery'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['arch'], foreground = colors['cyan'], background = colors['black']}
        }
    },
};

reactions["make_raw_silicon"] = {
   name = "Make Raw Silicon",
   workshop = "silicon_refinery",
   inputs = { { item="raw_glass", qty=1} },
   outputs = { { item="raw_silicon", qty=1 } },
   skill = "Glassmaking",
   difficulty = 10,
   automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Kilns are used for baking clay into bricks. They should also be used for pottery.
------------------------------------------------------------------------------------------------------------------------

buildings["kiln"] = {
    name = "Kiln",
    components = { { item="clay", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['kiln'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['arch'], foreground = colors['orange'], background = colors['black']}
        }
    },
};

reactions["bake_bricks"] = {
    name = "Bake Clay into Bricks",
    workshop = "kiln",
    inputs = { { item="charcoal", qty=1 }, { item="clay", qty=1 } },
    outputs = { { item="block", qty=4 } },
    skill = "Carpentry",
    difficulty = 10,
    automatic = true
};

