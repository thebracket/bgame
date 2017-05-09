-- Workshops

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

reactions["make_light_stone_hammer"] = {
    name = "Make Light Stone Warhammer",
    workshop = "primitive_workshop",
    inputs = { { item="block", qty=1, mat_type="rock" } },
    outputs = { { item="light_warhammer", qty=1 } },
    skill = "Masonry",
    difficulty = 10,
    automatic = false
};

reactions["make_sling"] = {
    name = "Make Cloth Sling",
    workshop = "primitive_workshop",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="sling", qty=1 } },
    skill = "Tailoring",
    difficulty = 10,
    automatic = false
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

reactions["make_shortbow"] = {
    name = "Make Shortbow",
    workshop = "primitive_workshop",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="shortbow", qty=1 } },
    skill = "Carpentry",
    difficulty = 15,
    automatic = false
};

reactions["make_composite_shortbow"] = {
    name = "Make Composite Shortbow",
    workshop = "primitive_workshop",
    inputs = { { item="block", qty=1, material="wood" }, { item="bone", qty=1 } },
    outputs = { { item="composite_shortbow", qty=1 } },
    skill = "Carpentry",
    difficulty = 17,
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

reactions["make_heavy_crossbow"] = {
    name = "Make Heavy Crossbow",
    workshop = "intermediate_workshop",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="heavy_crossbow", qty=1 } },
    skill = "Carpentry",
    difficulty = 19,
    automatic = false
};

reactions["make_light_crossbow"] = {
    name = "Make Light Crossbow",
    workshop = "intermediate_workshop",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="light_crossbow", qty=1 } },
    skill = "Carpentry",
    difficulty = 17,
    automatic = false
};

reactions["make_repeating_crossbow"] = {
    name = "Make Repeating Crossbow",
    workshop = "intermediate_workshop",
    inputs = { { item="block", qty=1, material="wood" }, { item="mechanism", qty=1 } },
    outputs = { { item="repeating_crossbow", qty=1 } },
    skill = "Carpentry",
    difficulty = 17,
    automatic = false
};

reactions["make_longbow"] = {
    name = "Make Longbow",
    workshop = "intermediate_workshop",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="longbow", qty=1 } },
    skill = "Carpentry",
    difficulty = 17,
    automatic = false
};

reactions["make_composite_longbow"] = {
    name = "Make Composite Longbow",
    workshop = "intermediate_workshop",
    inputs = { { item="block", qty=1, material="wood" }, { item="bone", qty=1 } },
    outputs = { { item="composite_longbow", qty=1 } },
    skill = "Carpentry",
    difficulty = 19,
    automatic = false
};

reactions["make_quarrel"] = {
    name = "Make Quarrel",
    workshop = "intermediate_workshop",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="quarrel", qty=1 } },
    skill = "Carpentry",
    difficulty = 12,
    automatic = false
};

reactions["make_firelance_tube"] = {
    name = "Make Firelance Tube",
    workshop = "intermediate_workshop",
    inputs = { { item="block", qty=1, material="metal" }, { item="mechanism", qty=1 } },
    outputs = { { item="firelance_tube", qty=1 } },
    skill = "Metalworking",
    difficulty = 17,
    automatic = false
};

reactions["make_firelance_spike"] = {
    name = "Make Firelance Spike",
    workshop = "intermediate_workshop",
    inputs = { { item="block", qty=1, mat_type="metal" }, { item="black_powder", qty=1 }, { item="flint", qty=1 } },
    outputs = { { item="firelance_spike", qty=1 } },
    skill = "Metalworking",
    difficulty = 16,
    automatic = false
};

reactions["make_handcannon"] = {
    name = "Make Hand Cannon",
    workshop = "intermediate_workshop",
    inputs = { { item="block", qty=1, material="metal" }, { item="mechanism", qty=1 }, { item="flint", qty=1 } },
    outputs = { { item="handcannon", qty=1 } },
    skill = "Metalworking",
    difficulty = 17,
    automatic = false
};

reactions["make_musket"] = {
    name = "Make Musket",
    workshop = "intermediate_workshop",
    inputs = { { item="block", qty=1, material="metal" }, { item="musket_cast", qty=1 }, { item="flint", qty=1 } },
    outputs = { { item="musket", qty=2 }, { item="musket_cast", qty=1 } },
    skill = "Metalworking",
    difficulty = 21,
    automatic = false
};

reactions["make_flintlock_pistol"] = {
    name = "Make Flintlock Pistol",
    workshop = "intermediate_workshop",
    inputs = { { item="block", qty=1, material="metal" }, { item="mechanism", qty=1 }, { item="flint", qty=1 } },
    outputs = { { item="flintlock_pistol", qty=1 } },
    skill = "Metalworking",
    difficulty = 17,
    automatic = false
};

reactions["make_wheelock_pistol"] = {
    name = "Make Wheelock Pistol",
    workshop = "intermediate_workshop",
    inputs = { { item="block", qty=1, material="metal" }, { item="mechanism", qty=2 }, { item="flint", qty=2 } },
    outputs = { { item="wheel_lock_pistol", qty=1 } },
    skill = "Metalworking",
    difficulty = 21,
    automatic = false
};

reactions["make_flintlock_shotgun"] = {
    name = "Make Flintlock Shotgun",
    workshop = "intermediate_workshop",
    inputs = { { item="block", qty=1, material="metal" }, { item="mechanism", qty=1 }, { item="flint", qty=1 } },
    outputs = { { item="flintlock_shotgun", qty=1 } },
    skill = "Metalworking",
    difficulty = 21,
    automatic = false
};

reactions["make_blunderbuss"] = {
    name = "Make Blunderbuss",
    workshop = "intermediate_workshop",
    inputs = { { item="block", qty=1, material="metal" }, { item="mechanism", qty=1 } },
    outputs = { { item="flintlock_shotgun", qty=1 } },
    skill = "Metalworking",
    difficulty = 21,
    automatic = false
};

reactions["make_shot_wadding"] = {
    name = "Make Shot and Wadding",
    workshop = "intermediate_workshop",
    inputs = { { item="block", qty=1, mat_type="metal" }, { item="black_powder", qty=1 }, { item="cloth", qty=1 } },
    outputs = { { item="shot_wadding", qty=2 } },
    skill = "Metalworking",
    difficulty = 16,
    automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Factories are used for the crafting of modern items
------------------------------------------------------------------------------------------------------------------------

buildings["factory"] = {
    name = "Factory",
    components = { { item="block", qty=1 }, { item="simple_lathe", qty=1 }, { item="tool_and_die", qty=1 } },
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

reactions["make_fabrication_controller"] = {
    name = "Make Smart Fabrication Controller",
    workshop = "factory",
    inputs = { { item="block", qty=1, mat_type="metal" }, { item="mechanism", qty=1 }, { item="circuit", qty=1 } },
    outputs = { { item="smart_fabrication_controller", qty=1 } },
    skill = "Electronics",
    difficulty = 19,
    automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Fabricators are used for ultra-modern to futuristic construction
------------------------------------------------------------------------------------------------------------------------

buildings["fabricator"] = {
    name = "Fabrication Plant",
    components = { { item="block", qty=1 }, { item="smart_fabrication_controller", qty=1 }, { item="tool_and_die", qty=1 } },
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