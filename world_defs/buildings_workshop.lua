-- Workshops

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
-- Intermediate Carpentry workshops are where woodwork is performed, requiring a lathe or other advancement.
------------------------------------------------------------------------------------------------------------------------
buildings["intermediate_carpenter"] = {
    name = "Intermediate Carpentry Workshop",
    components = { { item="block", qty=1 }, {item="simple_lathe", qty=1 } },
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
-- Intermediate forges cover up to medieval forge technology up to the renaissance.
------------------------------------------------------------------------------------------------------------------------

buildings["intermediate_forge"] = {
    name = "Intermediate Forge",
    components = { { item="block", qty=1, mat_type="rock" }, { item="simple_lathe", qty=1 } },
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

reactions["make_simple_lathe"] = {
    name = "Make Simple Lathe",
    workshop = "primitive_workshop",
    inputs = { { item="block", material="wood", qty=1 }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="simple_lathe", qty=1 } },
    skill = "Metalworking",
    difficulty = 16,
    automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Intermediate workshops are used for the crafting of intricate items, up to the renaissance era.
------------------------------------------------------------------------------------------------------------------------

buildings["intermediate_workshop"] = {
    name = "Intermediate Workshop",
    components = { { item="block", qty=1 }, { item="simple_lathe", qty=1 } },
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
