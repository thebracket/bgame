-- Workshops

------------------------------------------------------------------------------------------------------------------------
-- Cordex starts with a small replicator on board. It's deliberately not very useful, but can get you started.
------------------------------------------------------------------------------------------------------------------------
buildings["small_replicator"] = {
    name = "A small replicator",
    components = { { item="replicator_unit", qty=1} },
    skill = { name="Construction", difficulty=10 },
    provides = { storage={energy_cost=0} },
    render = {
        width=1, height=1, tiles={
            {glyph=glyphs['replicator_small'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles={
            {glyph=glyphs['square_root'], foreground = colors['cyan'], background = colors['black']}
        }
    }
};

reactions["replicate_tea_earl_grey_hot"] = {
    name = "Replicate Cup of Tea",
    workshop = "small_replicator",
    inputs = {  },
    outputs = { { item="tea_replicated", qty=1 } },
    skill = "Construction",
    difficulty = 5,
    automatic = false,
    power_drain = 20
};

reactions["replicate_sandwich"] = {
    name = "Replicate Sandwich",
    workshop = "small_replicator",
    inputs = {  },
    outputs = { { item="sandwich_replicated", qty=1 } },
    skill = "Construction",
    difficulty = 5,
    automatic = false,
    power_drain = 20
};

reactions["replicate_small_energy_cell"] = {
    name = "Replicate Small Energy Cell",
    workshop = "small_replicator",
    inputs = {  },
    outputs = { { item="small_energy_cell", qty=1 } },
    skill = "Construction",
    difficulty = 5,
    automatic = false,
    power_drain = 30
};

------------------------------------------------------------------------------------------------------------------------
-- Fake camp fires provide light and can replicate marshmallows.
------------------------------------------------------------------------------------------------------------------------
buildings["fake_camp_fire"] = {
    name = "Nuclear Camp Fire",
    components = { { item="camp_fire_kit", qty=1 } },
    skill = { name="Construction", difficulty=5 },
    provides = { light={radius=5, color = colors['yellow']} },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['campfire'], foreground = colors['firelight'], background = colors['yellow']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['sun'], foreground = colors['yellow'], background = colors['yellow']}
        }
    },
    emits_smoke = true
};

reactions["replicate_tiny_marshmallow"] = {
    name = "Replicate Tiny Marshmallow",
    workshop = "fake_camp_fire",
    inputs = {  },
    outputs = { { item="tiny_marshmallow", qty=1 } },
    skill = "Construction",
    difficulty = 5,
    automatic = false,
    power_drain = 10
};

reactions["roast_food_on_fire"] = {
    name = "Roast simple meal",
    workshop = "fake_camp_fire",
    inputs = { { item="any", qty=1, mat_type="food" }  },
    outputs = { { item="roast_simple", qty=1, special="cooking" } },
    skill = "Cooking",
    difficulty = 5,
    automatic = false
};

reactions["roast_combined_food_on_fire"] = {
    name = "Roast normal meal",
    workshop = "fake_camp_fire",
    inputs = { { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }  },
    outputs = { { item="roast_simple", qty=1, special="cooking" } },
    skill = "Cooking",
    difficulty = 10,
    automatic = false
};

reactions["roast_lavish_meal_on_fire"] = {
    name = "Roast lavish meal",
    workshop = "fake_camp_fire",
    inputs = { { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="spice" }  },
    outputs = { { item="roast_simple", qty=1, special="cooking" } },
    skill = "Cooking",
    difficulty = 15,
    automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- An actual campfire, useful for cooking.
------------------------------------------------------------------------------------------------------------------------
buildings["camp_fire"] = {
    name = "Camp Fire",
    components = { { item="wood_log", qty=1 } },
    skill = { name="Construction", difficulty=5 },
    provides = { light={radius=5, color = colors['yellow']} },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['campfire'], foreground = colors['firelight'], background = colors['yellow']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['sun'], foreground = colors['yellow'], background = colors['yellow']}
        }
    },
    emits_smoke = true
};

reactions["roast_food_on_real_fire"] = {
    name = "Roast simple meal",
    workshop = "camp_fire",
    inputs = { { item="any", qty=1, mat_type="food" }  },
    outputs = { { item="roast_simple", qty=1, special="cooking" } },
    skill = "Cooking",
    difficulty = 5,
    automatic = false
};

reactions["roast_combined_food_on_real_fire"] = {
    name = "Roast normal meal",
    workshop = "camp_fire",
    inputs = { { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }  },
    outputs = { { item="roast_simple", qty=1, special="cooking" } },
    skill = "Cooking",
    difficulty = 10,
    automatic = false
};

reactions["roast_lavish_meal_on_real_fire"] = {
    name = "Roast lavish meal",
    workshop = "camp_fire",
    inputs = { { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="spice" }  },
    outputs = { { item="roast_simple", qty=1, special="cooking" } },
    skill = "Cooking",
    difficulty = 15,
    automatic = false
};

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
-- Distilleries make booze.
------------------------------------------------------------------------------------------------------------------------

buildings["still"] = {
    name = "Distillery",
    components = { { item="block", qty=2 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['still_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['still_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['still_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['still_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['kit'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['kit'], foreground = colors['wood_brown'], background = colors['black']}
        }
    },
};

reactions["make_booze"] = {
    name = "Ferment Alcoholic Beverage", workshop = "still",
    inputs = { { item="any", qty=1, mat_type="food" } },
    outputs = { { item="wine", qty=1, special="cooking" } },
    skill = "Brewing", difficulty = 16, automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Butchers convert dead animals into useful parts. Not controlled by reactions, it's in the C++. Sorry.
------------------------------------------------------------------------------------------------------------------------

buildings["butcher"] = {
    name = "Butcher",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['butcher_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['butcher_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['butcher_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['butcher_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['cabinet'], foreground = colors['red'], background = colors['black']},
            {glyph= glyphs['kit'], foreground = colors['red'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['red'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['red'], background = colors['black']}
        }
    },
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
-- Leatherworker shops are used to sew leather into useful shapes.
------------------------------------------------------------------------------------------------------------------------

buildings["leatherworker"] = {
    name = "Leatherworker",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['leatherworker_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['leatherworker_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['leatherworker_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['leatherworker_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['chair'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['cabinet'], foreground = colors['wood_brown'], background = colors['black']}
        }
    },
};

reactions["make_hide_tunic"] = {
    name = "Make Leather Tunic",
    workshop = "leatherworker",
    inputs = { { item="leather", qty=1 } },
    outputs = { { item="tunic", qty=1 } },
    skill = "Leatherworking",
    difficulty = 10,
    automatic = false
};

reactions["make_hide_pants"] = {
    name = "Make Leather Britches",
    workshop = "leatherworker",
    inputs = { { item="leather", qty=1 } },
    outputs = { { item="britches", qty=1 } },
    skill = "Leatherworking",
    difficulty = 10,
    automatic = false
};

reactions["make_hide_skirt"] = {
    name = "Make Leather Skirt",
    workshop = "leatherworker",
    inputs = { { item="leather", qty=1 } },
    outputs = { { item="skirt_simple", qty=1 } },
    skill = "Leatherworking",
    difficulty = 10,
    automatic = false
};

reactions["make_hide_sandals"] = {
    name = "Make Leather Sandals",
    workshop = "leatherworker",
    inputs = { { item="leather", qty=1 } },
    outputs = { { item="sandals", qty=1 } },
    skill = "Leatherworking",
    difficulty = 10,
    automatic = false
};

reactions["make_leather_sandals"] = {
    name = "Make Sandals",
    workshop = "leatherworker",
    inputs = { { item="leather", qty=1 } },
    outputs = { { item="sandals", qty=1 } },
    skill = "Leatherworking",
    difficulty = 16,
    automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Bonecarvers are used to turn bone into something useful.
------------------------------------------------------------------------------------------------------------------------

buildings["bonecarver"] = {
    name = "Bonecarver",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['bonecarver_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['bonecarver_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['bonecarver_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['bonecarver_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['happy_face'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['happy_face'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['vertical_line'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['vertical_line'], foreground = colors['white'], background = colors['black']}
        }
    },
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
-- Carpentry workshops are where woodwork is performed.
------------------------------------------------------------------------------------------------------------------------
buildings["carpenter"] = {
    name = "Carpentry Workshop",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['carpenter_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['cabinet'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['cabinet'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['wood_brown'], background = colors['black']}
        }
    },
};

reactions["make_wooden_table"] = {
    name = "Make Wooden Table",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="table", qty=1 } },
    skill = "Carpentry",
    difficulty = 10,
    automatic = false
};

reactions["make_wooden_chair"] = {
    name = "Make Wooden Chair",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="chair", qty=1 } },
    skill = "Carpentry",
    difficulty = 10,
    automatic = false
};

reactions["make_wooden_door"] = {
    name = "Make Wooden Door",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="door", qty=1 } },
    skill = "Carpentry",
    difficulty = 12,
    automatic = false
};

reactions["make_simple_bed"] = {
    name = "Make Simple Bed",
    workshop = "carpenter",
    inputs = { { item="block", material="wood", qty=1} },
    outputs = { { item="bed_item", qty=1 } },
    skill = "Carpentry",
    difficulty = 10,
    automatic = false
};

reactions["make_cage"] = {
    name = "Make Cage",
    workshop = "carpenter",
    inputs = { { item="block", qty=1} },
    outputs = { { item="cage", qty=1 } },
    skill = "Mechanics",
    difficulty = 10,
    automatic = false
};

reactions["make_wooden_club"] = {
    name = "Make Wooden Club",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="club", qty=1 } },
    skill = "Carpentry",
    difficulty = 10,
    automatic = false
};

reactions["make_pointy_stick"] = {
    name = "Make Wooden Pointy Stick",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="pointy_stick", qty=1 } },
    skill = "Carpentry",
    difficulty = 7,
    automatic = false
};

reactions["make_atlatl"] = {
    name = "Make Atlatl",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="atlatl", qty=1 } },
    skill = "Carpentry",
    difficulty = 15,
    automatic = false
};

reactions["make_wood_dart"] = {
    name = "Make Wooden Dart",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="dart", qty=1 } },
    skill = "Carpentry",
    difficulty = 12,
    automatic = false
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

------------------------------------------------------------------------------------------------------------------------
-- Primitive forges cover up to medieval forge technology, before metallurgy really took flight.
------------------------------------------------------------------------------------------------------------------------

buildings["primitive_forge"] = {
    name = "Primitive Forge",
    components = { { item="block", qty=1, mat_type="rock" } },
    skill = { name="Construction", difficulty=15 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['forge_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['forge_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['forge_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['forge_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['sun'], foreground = colors['cyan'], background = colors['black']},
            {glyph= glyphs['cabinet'], foreground = colors['grey'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['grey'], background = colors['black']},
            {glyph= glyphs['sun'], foreground = colors['yellow'], background = colors['black']}
        }
    },
};

reactions["make_metal_table"] = {
    name = "Make Metal Table",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="table", qty=1 } },
    skill = "Metalworking",
    difficulty = 10,
    automatic = false
};

reactions["make_metal_chair"] = {
    name = "Make Metal Chair",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="chair", qty=1 } },
    skill = "Metalworking",
    difficulty = 10,
    automatic = false
};

reactions["make_metal_door"] = {
    name = "Make Metal Door",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="door", qty=1 } },
    skill = "Metalworking",
    difficulty = 12,
    automatic = false
};

reactions["make_trap_blade"] = {
    name = "Make Trap Blades",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="trap_blade", qty=1 } },
    skill = "Metalworking",
    difficulty = 12,
    automatic = false
};

reactions["make_retractable_spikes"] = {
    name = "Make Retractable Spikes",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="retractable_spike", qty=1 } },
    skill = "Metalworking",
    difficulty = 15,
    automatic = false
};

reactions["make_metal_hammer"] = {
    name = "Make Metal Warhammer",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="warhammer", qty=1 } },
    skill = "Metalworking",
    difficulty = 12,
    automatic = false
};

reactions["make_metal_battleaxe"] = {
    name = "Make Metal Battleaxe",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="battleaxe", qty=1 } },
    skill = "Metalworking",
    difficulty = 12,
    automatic = false
};

reactions["make_metal_handaxe"] = {
    name = "Make Metal Hand-axe",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="handaxe", qty=1 } },
    skill = "Metalworking",
    difficulty = 12,
    automatic = false
};

reactions["make_metal_pickaxe"] = {
    name = "Make Metal Pickaxe",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="pickaxe", qty=1 } },
    skill = "Metalworking",
    difficulty = 12,
    automatic = false
};

reactions["make_metal_dagger"] = {
    name = "Make Metal Dagger",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="dagger", qty=1 } },
    skill = "Metalworking",
    difficulty = 12,
    automatic = false
};

reactions["make_metal_knife"] = {
    name = "Make Metal Knife",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="knife", qty=1 } },
    skill = "Metalworking",
    difficulty = 12,
    automatic = false
};

reactions["make_metal_shortsword"] = {
    name = "Make Short Sword",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="shortsword", qty=1 } },
    skill = "Metalworking",
    difficulty = 14,
    automatic = false
};

reactions["make_metal_longsword"] = {
    name = "Make Longsword",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="longsword", qty=1 } },
    skill = "Metalworking",
    difficulty = 16,
    automatic = false
};


------------------------------------------------------------------------------------------------------------------------
-- Primitive workshops are used for the crafting of intricate, but primitive, items.
------------------------------------------------------------------------------------------------------------------------

buildings["primitive_workshop"] = {
    name = "Primitive Workshop",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=15 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['workshop_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['workshop_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['workshop_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['workshop_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['cabinet'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['cabinet'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['white'], background = colors['black']}
        }
    },
};

reactions["make_stone_hammer"] = {
    name = "Make Stone Warhammer",
    workshop = "primitive_workshop",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="rock" } },
    outputs = { { item="warhammer", qty=1 } },
    skill = "Masonry",
    difficulty = 10,
    automatic = false
};

reactions["make_stone_battleaxe"] = {
    name = "Make Stone Battlaxe",
    workshop = "primitive_workshop",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="rock" } },
    outputs = { { item="battleaxe", qty=1 } },
    skill = "Masonry",
    difficulty = 10,
    automatic = false
};

reactions["make_stone_handaxe"] = {
    name = "Make Stone Handaxe",
    workshop = "primitive_workshop",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="rock" } },
    outputs = { { item="handaxe", qty=1 } },
    skill = "Masonry",
    difficulty = 10,
    automatic = false
};

reactions["make_stone_knife"] = {
    name = "Make Stone Knife",
    workshop = "primitive_workshop",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="rock" } },
    outputs = { { item="knife", qty=1 } },
    skill = "Masonry",
    difficulty = 10,
    automatic = false
};

reactions["make_ringmail"] = {
    name = "Make Ringmail",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="ringmail", qty=1 } },
    skill = "Metalworking",
    difficulty = 16,
    automatic = false
};

reactions["make_breastplate"] = {
    name = "Make Breastplate",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="breastplate", qty=1 } },
    skill = "Metalworking",
    difficulty = 16,
    automatic = false
};

reactions["make_cap"] = {
    name = "Make Metal Cap",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="cap", qty=1 } },
    skill = "Metalworking",
    difficulty = 16,
    automatic = false
};

reactions["make_helm"] = {
    name = "Make Helm",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="helm", qty=1 } },
    skill = "Metalworking",
    difficulty = 18,
    automatic = false
};

reactions["make_leggings"] = {
    name = "Make Armored Leggings",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="leggings", qty=1 } },
    skill = "Metalworking",
    difficulty = 16,
    automatic = false
};

reactions["make_bronze_skirt"] = {
    name = "Make Metal Skirt",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="skirt_simple", qty=1 } },
    skill = "Metalworking",
    difficulty = 16,
    automatic = false
};


------------------------------------------------------------------------------------------------------------------------
-- Masonry shops are used to convert stone blocks into furniture
------------------------------------------------------------------------------------------------------------------------
buildings["mason"] = {
    name = "Masonry",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['mason_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mason_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mason_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mason_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']}
        }
    },
};

reactions["make_stone_table"] = {
    name = "Make Stone Table",
    workshop = "mason",
    inputs = { { item="block", qty=1, mat_type="rock" } },
    outputs = { { item="table", qty=1 } },
    skill = "Masonry",
    difficulty = 10,
    automatic = false
};

reactions["make_stone_chair"] = {
    name = "Make Stone Chair",
    workshop = "mason",
    inputs = { { item="block", qty=1, mat_type="rock" } },
    outputs = { { item="chair", qty=1 } },
    skill = "Masonry",
    difficulty = 10,
    automatic = false
};

reactions["make_stone_door"] = {
   name = "Make Stone Door",
   workshop = "mason",
   inputs = { { item="block", qty=1, mat_type="rock" } },
   outputs = { { item="door", qty=1 } },
   skill = "Masonry",
   difficulty = 12,
   automatic = false
};

reactions["make_stone_club"] = {
    name = "Make Stone Club",
    workshop = "mason",
    inputs = { { item="block", qty=1, mat_type="rock" } },
    outputs = { { item="club", qty=1 } },
    skill = "Masonry",
    difficulty = 10,
    automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Mechanic shops are used to make mechanisms
------------------------------------------------------------------------------------------------------------------------
buildings["mechanic"] = {
    name = "Mechanic Workshop",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['mechanic_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mechanic_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mechanic_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mechanic_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['star'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['star'], foreground = colors['red'], background = colors['black']}
        }
    },
};

reactions["make_simple_mechanism"] = {
    name = "Make Simple Mechanism",
    workshop = "mechanic",
    inputs = { { item="block", qty=1} },
    outputs = { { item="mechanism", qty=1 } },
    skill = "Mechanics",
    difficulty = 10,
    automatic = false
};

reactions["make_simple_circuit"] = {
    name = "Make Simple Circuit",
    workshop = "mechanic",
    inputs = { { item="raw_silicon", qty=1}, { item="block", mat_type="metal", qty=1 } },
    outputs = { { item="mechanism", qty=1 } },
    skill = "Mechanics",
    difficulty = 10,
    automatic = false
};
