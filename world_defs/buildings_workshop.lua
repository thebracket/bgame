-- Workshops

------------------------------------------------------------------------------------------------------------------------
-- Primitive workshops are used for the crafting of intricate, but primitive, items.
------------------------------------------------------------------------------------------------------------------------

buildings["primitive_workshop"] = {
    name = "Primitive Workshop",
    description = "A general construction workshop, used to assemble items from components.",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=15 },
    render_rex = "mechanic.xp",
    vox = voxelId("mechanic")
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

reactions["make_galea"] = {
    name = "Make Galea Helm",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="helm_roman_galea", qty=1 } },
    skill = "Metalworking",
    difficulty = 19,
    automatic = false
};

reactions["make_banded_armor"] = {
    name = "Make Banded Armor",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="banded_armor", qty=1 } },
    skill = "Metalworking",
    difficulty = 20,
    automatic = false
};

reactions["make_scalemail_armor"] = {
    name = "Make Scale Mail Armor",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="scalemail_armor", qty=1 } },
    skill = "Metalworking",
    difficulty = 20,
    automatic = false
};

reactions["make_scalemail_leggings"] = {
    name = "Make Scale Mail Leggings",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="scalemail_leggings", qty=1 } },
    skill = "Metalworking",
    difficulty = 20,
    automatic = false
};

reactions["make_chainmail_armor"] = {
    name = "Make Chain Mail Armor",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="chainmail_shirt", qty=1 } },
    skill = "Metalworking",
    difficulty = 20,
    automatic = false
};

reactions["make_chainmail_leggings"] = {
    name = "Make Chain Mail Leggings",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="chainmail_leggings", qty=1 } },
    skill = "Metalworking",
    difficulty = 20,
    automatic = false
};

reactions["make_chainmail_skirt"] = {
    name = "Make Chain Mail Skirt",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="chainmail_skirt", qty=1 } },
    skill = "Metalworking",
    difficulty = 20,
    automatic = false
};

reactions["make_chainmail_bikini"] = {
    name = "Make Chain Mail Bikini",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="chainmail_bikini", qty=1 } },
    skill = "Metalworking",
    difficulty = 20,
    automatic = false
};

reactions["make_ringmail_leggings"] = {
    name = "Make Armored Leggings",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="ringmail_leggings", qty=1 } },
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

reactions["make_bassinet"] = {
    name = "Make Bassinet Helm",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="helm_bassinet", qty=1 } },
    skill = "Metalworking",
    difficulty = 19,
    automatic = false
};

reactions["make_great_helm"] = {
    name = "Make Great Helm",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=2 }, { item="block", qty=3, mat_type="metal" } },
    outputs = { { item="great_helm", qty=1 } },
    skill = "Metalworking",
    difficulty = 21,
    automatic = false
};

reactions["make_chain_and_plate"] = {
    name = "Make Plate and Chain Torso",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=2 }, { item="block", qty=3, mat_type="metal" } },
    outputs = { { item="plate_and_chain_mail", qty=1 } },
    skill = "Metalworking",
    difficulty = 21,
    automatic = false
};

reactions["make_chain_and_plate_legs"] = {
    name = "Make Plate and Chain Leggings",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=2 }, { item="block", qty=3, mat_type="metal" } },
    outputs = { { item="plate_and_chain_leggings", qty=1 } },
    skill = "Metalworking",
    difficulty = 21,
    automatic = false
};

reactions["make_chain_and_plate_bikini"] = {
    name = "Make Plate and Chain Swimwear",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=1 }, { item="block", qty=1, mat_type="metal" } },
    outputs = { { item="plate_and_chain_swimwear", qty=1 } },
    skill = "Metalworking",
    difficulty = 21,
    automatic = false
};

reactions["make_field_plate"] = {
    name = "Make Field Plate Torso",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=2 }, { item="block", qty=3, mat_type="metal" } },
    outputs = { { item="field_plate_mail", qty=1 } },
    skill = "Metalworking",
    difficulty = 23,
    automatic = false
};

reactions["make_field_plate_legs"] = {
    name = "Make Field Plate Legs",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=2 }, { item="block", qty=3, mat_type="metal" } },
    outputs = { { item="field_plate_mail_legs", qty=1 } },
    skill = "Metalworking",
    difficulty = 23,
    automatic = false
};

reactions["make_full_plate"] = {
    name = "Make Full Plate Torso",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=2 }, { item="block", qty=4, mat_type="metal" } },
    outputs = { { item="full_plate_mail", qty=1 } },
    skill = "Metalworking",
    difficulty = 24,
    automatic = false
};

reactions["make_full_plate_legs"] = {
    name = "Make Full Plate Legs",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=2 }, { item="block", qty=4, mat_type="metal" } },
    outputs = { { item="full_plate_mail_legs", qty=1 } },
    skill = "Metalworking",
    difficulty = 24,
    automatic = false
};

reactions["make_cuirass"] = {
    name = "Make Cuirass",
    workshop = "primitive_workshop",
    inputs = { { item="leather", qty=2 }, { item="block", qty=2, mat_type="metal" } },
    outputs = { { item="cuirass", qty=1 } },
    skill = "Metalworking",
    difficulty = 21,
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
    description = "A more advanced assembly workshop, capable of intricate assembly up to the renaissance era.",
    components = { { item="block", qty=1 }, { item="simple_lathe", qty=1 } },
    skill = { name="Construction", difficulty=15 },
    render_rex = "mechanic.xp",
    vox = voxelId("mechanic")
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
    description = "A modern assembly plant, capable of constructing quite advanced items.",
    components = { { item="block", qty=1 }, { item="simple_lathe", qty=1 }, { item="tool_and_die", qty=1 } },
    skill = { name="Construction", difficulty=15 },
    render_rex = "mechanic.xp",
    vox = voxelId("mechanic")
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
    description = "An automated replication system, capable of fabricating the most advanced devices.",
    components = { { item="block", qty=1 }, { item="smart_fabrication_controller", qty=1 }, { item="tool_and_die", qty=1 } },
    skill = { name="Construction", difficulty=15 },
    render_rex = "mechanic.xp",
    vox = voxelId("mechanic")
};

