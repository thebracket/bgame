-- Metalworking

------------------------------------------------------------------------------------------------------------------------
-- Primitive forges cover up to medieval forge technology, before metallurgy really took flight.
------------------------------------------------------------------------------------------------------------------------

buildings["primitive_forge"] = {
    name = "Primitive Forge",
    description = "A simple forge - hammer, anvil, heat source and water. Everything you need for primitive metal equipment.",
    components = { { item="block", qty=1, mat_type="rock" } },
    skill = { name="Construction", difficulty=15 },
    render_rex = "forge.xp",
    vox=voxelId("forge")
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

reactions["make_metal_floodgate"] = {
    name = "Make Metal Floodgate",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="floodgate", qty=1 } },
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

reactions["make_light_metal_hammer"] = {
    name = "Make Light Metal Warhammer",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="light_warhammer", qty=1 } },
    skill = "Metalworking",
    difficulty = 10,
    automatic = false
};

reactions["make_hooked_warhammer"] = {
    name = "Make Hooked Warhammer",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="hooked_warhammer", qty=1 } },
    skill = "Metalworking",
    difficulty = 18,
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

reactions["make_metal_greataxe"] = {
    name = "Make Metal Greataxe",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="greataxe", qty=1 } },
    skill = "Metalworking",
    difficulty = 17,
    automatic = false
};

reactions["make_metal_doubleaxe"] = {
    name = "Make Metal Double-Bladed Axe",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="double_axe", qty=1 } },
    skill = "Metalworking",
    difficulty = 19,
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

reactions["make_hoe"] = {
    name = "Make Hoe",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="hoe", qty=1 } },
    skill = "Metalworking",
    difficulty = 10,
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

reactions["make_metal_kukri"] = {
    name = "Make Metal Kukri",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="kukri", qty=1 } },
    skill = "Metalworking",
    difficulty = 14,
    automatic = false
};

reactions["make_metal_punching_dagger"] = {
    name = "Make Metal Punching Dagger",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="punching_dagger", qty=1 } },
    skill = "Metalworking",
    difficulty = 14,
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

reactions["make_metal_sickle"] = {
    name = "Make Sickle",
    workshop = "primitive_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="sickle", qty=1 } },
    skill = "Metalworking",
    difficulty = 16,
    automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Intermediate forges cover up to medieval forge technology up to the renaissance.
------------------------------------------------------------------------------------------------------------------------

buildings["intermediate_forge"] = {
    name = "Intermediate Forge",
    description = "A more advanced forge, suitable for making early renaissance equipment. It includes advanced metal shaping machinery.",
    components = { { item="block", qty=1, mat_type="rock" }, { item="simple_lathe", qty=1 } },
    skill = { name="Construction", difficulty=15 },
    render_rex = "forge.xp",
    vox = voxelId("forge")
};

------------------------------------------------------------------------------------------------------------------------
-- Intermediate forges cover up to medieval forge technology up to the renaissance.
------------------------------------------------------------------------------------------------------------------------

buildings["advanced_forge"] = {
    name = "Advanced Forge",
    description = "A renaissance-era forge, capable of milling precise tools.",
    components = { { item="block", qty=4, mat_type="rock" }, { item="precision_tools", qty=1 } },
    skill = { name="Construction", difficulty=15 },
    render_rex = "forge.xp",
    vox = voxelId("forge")
};

------------------------------------------------------------------------------------------------------------------------
-- Intermediate forges cover up to medieval forge technology up to the renaissance.
reactions["make_light_mace"] = {
    name = "Make Light Mace",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="mace_light", qty=1 } },
    skill = "Metalworking",
    difficulty = 16,
    automatic = false
};

reactions["make_heavy_mace"] = {
    name = "Make Heavy Mace",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=3, mat_type="metal" } },
    outputs = { { item="mace_heavy", qty=1 } },
    skill = "Metalworking",
    difficulty = 16,
    automatic = false
};

reactions["make_morningstar"] = {
    name = "Make Morningstar",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=3, mat_type="metal" } },
    outputs = { { item="morningstar", qty=1 } },
    skill = "Metalworking",
    difficulty = 18,
    automatic = false
};

reactions["make_light_flail"] = {
    name = "Make Light Flail",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="morningstar", qty=1 } },
    skill = "Metalworking",
    difficulty = 18,
    automatic = false
};

reactions["make_heavy_flail"] = {
    name = "Make Heavy Flail",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=3, mat_type="metal" } },
    outputs = { { item="flail_heavy", qty=1 } },
    skill = "Metalworking",
    difficulty = 18,
    automatic = false
};

reactions["make_short_spear"] = {
    name = "Make Short Spear",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="short_spear", qty=1 } },
    skill = "Metalworking",
    difficulty = 12,
    automatic = false
};

reactions["make_long_spear"] = {
    name = "Make Long Spear",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="long_spear", qty=1 } },
    skill = "Metalworking",
    difficulty = 14,
    automatic = false
};

reactions["make_boar_spear"] = {
    name = "Make Boar Spear",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="boar_spear", qty=1 } },
    skill = "Metalworking",
    difficulty = 17,
    automatic = false
};

reactions["make_trident"] = {
    name = "Make Trident",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="trident", qty=1 } },
    skill = "Metalworking",
    difficulty = 17,
    automatic = false
};

reactions["make_metal_longsword"] = {
    name = "Make Longsword",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="longsword", qty=1 } },
    skill = "Metalworking",
    difficulty = 16,
    automatic = false
};

reactions["make_metal_scimitar"] = {
    name = "Make Scimitar",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="scimitar", qty=1 } },
    skill = "Metalworking",
    difficulty = 18,
    automatic = false
};

reactions["make_metal_rapier"] = {
    name = "Make Rapier",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="rapier", qty=1 } },
    skill = "Metalworking",
    difficulty = 18,
    automatic = false
};

reactions["make_metal_sabre"] = {
    name = "Make Sabre",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="sabre", qty=1 } },
    skill = "Metalworking",
    difficulty = 18,
    automatic = false
};

reactions["make_metal_cutlass"] = {
    name = "Make Cutlass",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="cutlass", qty=1 } },
    skill = "Metalworking",
    difficulty = 18,
    automatic = false
};

reactions["make_metal_falchion"] = {
    name = "Make Falchion",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="falchion", qty=1 } },
    skill = "Metalworking",
    difficulty = 18,
    automatic = false
};

reactions["make_metal_greatsword"] = {
    name = "Make Greatsword",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="greatsword", qty=1 } },
    skill = "Metalworking",
    difficulty = 18,
    automatic = false
};

reactions["make_metal_bastard_sword"] = {
    name = "Make Bastard Sword",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="bastard_sword", qty=1 } },
    skill = "Metalworking",
    difficulty = 16,
    automatic = false
};

reactions["make_metal_glaive"] = {
    name = "Make Glaive",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="glaive", qty=1 } },
    skill = "Metalworking",
    difficulty = 16,
    automatic = false
};

reactions["make_metal_guisarm"] = {
    name = "Make Guisarm",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="guisarm", qty=1 } },
    skill = "Metalworking",
    difficulty = 14,
    automatic = false
};

reactions["make_metal_halberd"] = {
    name = "Make Halberd",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="halberd", qty=1 } },
    skill = "Metalworking",
    difficulty = 19,
    automatic = false
};

reactions["make_metal_pike"] = {
    name = "Make Pike",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="pike", qty=1 } },
    skill = "Metalworking",
    difficulty = 17,
    automatic = false
};

reactions["make_musket_cast"] = {
    name = "Make Musket Cast",
    workshop = "intermediate_forge",
    inputs = { { item="block", qty=1, material="metal" }  },
    outputs = { { item="musket_cast", qty=1 } },
    skill = "Metalworking",
    difficulty = 17,
    automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Foundries are used for the construction of modern metal items
------------------------------------------------------------------------------------------------------------------------

buildings["foundry"] = {
    name = "Foundry",
    description = "A semi-automated forge, designed for mass production of goods from templates. It can produce items up to the early-modern era.",
    components = { { item="block", qty=1, mat_type="rock" }, { item="simple_lathe", qty=1 }, { item="tool_and_die", qty=1 } },
    skill = { name="Construction", difficulty=15 },
    render_rex = "forge.xp",
    vox = voxelId("forge")
};

reactions["make_oil_separation_tank"] = {
    name = "Make Oil Separation Tank",
    workshop = "foundry",
    inputs = { { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="oil_separation_tank", qty=1 } },
    skill = "Metalworking",
    difficulty = 21,
    automatic = false
};

