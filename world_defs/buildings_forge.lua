-- Metalworking

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
-- Intermediate forges cover up to medieval forge technology up to the renaissance.
------------------------------------------------------------------------------------------------------------------------

buildings["advanced_forge"] = {
    name = "Advanced Forge",
    components = { { item="block", qty=4, mat_type="rock" }, { item="precision_tools", qty=1 } },
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
-- Intermediate forges cover up to medieval forge technology up to the renaissance.
------------------------------------------------------------------------------------------------------------------------

buildings["foundry"] = {
    name = "Foundry",
    components = { { item="block", qty=4, mat_type="rock" }, { item="tool_and_die", qty=1 } },
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
