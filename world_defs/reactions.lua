reactions = {
    cut_wooden_planks = {
        name = "Cut Wooden Planks",
        workshop = "sawmill",
        inputs = { { item="wood_log", qty=1 } },
        outputs = { { item="block", qty=4 }, { item="wood_waste", qty=2} },
        skill = "Carpentry",
        difficulty = 10,
        automatic = true
    },
    charcoal_waste = {
        name = "Make Charcoal from Waste",
        workshop = "charcoal_hut",
        inputs = { { item="wood_waste", qty=1 } },
        outputs = { { item="charcoal", qty=2 }, { item="ash", qty=1} },
        skill = "Furnace Operation",
        difficulty = 10,
        automatic = true,
        emits_smoke = true
    },
    tan_leather = {
        name = "Tan Leather",
        workshop = "tanner",
        inputs = { { item="hide", qty=1 } },
        outputs = { { item="leather", qty=2 } },
        skill = "Tanning",
        difficulty = 10,
        automatic = true
    },
    make_raw_glass = {
        name = "Make Raw Glass",
        workshop = "glass_furnace",
        inputs = { { item="charcoal", qty=1 }, { item="sand", qty=1} },
        outputs = { { item="raw_glass", qty=2 } },
        skill = "Glassmaking",
        difficulty = 10,
        automatic = false,
        emits_smoke = true
    },
    make_raw_silicon = {
        name = "Make Raw Silicon",
        workshop = "silicon_refinery",
        inputs = { { item="raw_glass", qty=1} },
        outputs = { { item="raw_silicon", qty=1 } },
        skill = "Glassmaking",
        difficulty = 10,
        automatic = false
    },
    cut_stone = {
        name = "Cut Stone Blocks",
        workshop = "stonecutter",
        inputs = { { item="stone_boulder", qty=1 } },
        outputs = { { item="block", qty=4 } },
        skill = "Masonry",
        difficulty = 10,
        automatic = true
    },
    make_wooden_table = {
        name = "Make Wooden Table",
        workshop = "carpenter",
        inputs = { { item="block", qty=1 } },
        outputs = { { item="wood_table", qty=1 } },
        skill = "Carpentry",
        difficulty = 10,
        automatic = false
    },
    make_stone_table = {
        name = "Make Stone Table",
        workshop = "mason",
        inputs = { { item="block", qty=1 } },
        outputs = { { item="stone_table", qty=1 } },
        skill = "Masonry",
        difficulty = 10,
        automatic = false
    },
    make_wooden_chair = {
        name = "Make Wooden Chair",
        workshop = "carpenter",
        inputs = { { item="block", qty=1 } },
        outputs = { { item="wood_chair", qty=1 } },
        skill = "Carpentry",
        difficulty = 10,
        automatic = false
    },
    make_stone_chair = {
        name = "Make Stone Chair",
        workshop = "mason",
        inputs = { { item="block", qty=1 } },
        outputs = { { item="stone_chair", qty=1 } },
        skill = "Masonry",
        difficulty = 10,
        automatic = false
    },
    make_wooden_door = {
        name = "Make Wooden Door",
        workshop = "carpenter",
        inputs = { { item="block", qty=1 } },
        outputs = { { item="wood_door", qty=1 } },
        skill = "Carpentry",
        difficulty = 12,
        automatic = false
    },
    make_stone_door = {
        name = "Make Stone Door",
        workshop = "mason",
        inputs = { { item="block", qty=1 } },
        outputs = { { item="stone_door", qty=1 } },
        skill = "Masonry",
        difficulty = 12,
        automatic = false
    },
    smelt_ore = {
        name = "Smelt Ore",
        workshop = "smelter",
        inputs = { { item="ore", qty=1 } },
        outputs = { { item="bar", qty=2 } },
        skill = "Furnace Operation",
        difficulty = 12,
        automatic = true
    },
    make_bronze_ore = {
        name = "Smelt Bronze Alloy (Ore)",
        workshop = "smelter",
        inputs = { { item="tin_ore", qty=1 }, { item="copper_ore", qty=1} },
        outputs = { { item="bronze_bar", qty=1 } },
        skill = "Furnace Operation",
        difficulty = 14,
        automatic = true,
        emits_smoke = true
    },
    make_bronze_bar = {
        name = "Smelt Bronze Alloy (Bar)",
        workshop = "smelter",
        inputs = { { item="tin_bar", qty=1 }, { item="copper_bar", qty=1} },
        outputs = { { item="bronze_bar", qty=2 } },
        skill = "Furnace Operation",
        difficulty = 14,
        automatic = true,
        emits_smoke = true
    },
    make_bismuth_bronze = {
        name = "Smelt Bronze Alloy (Bismuth)",
        workshop = "smelter",
        inputs = { { item="bismuth_ore", qty=1 }, { item="copper_bar", qty=1} },
        outputs = { { item="bronze_bar", qty=2 } },
        skill = "Furnace Operation",
        difficulty = 14,
        automatic = true,
        emits_smoke = true
    },
    replicate_tiny_marshmallow = {
        name = "Replicate Tiny Marshmallow",
        workshop = "fake_camp_fire",
        inputs = {  },
        outputs = { { item="tiny_marshmallow", qty=1 } },
        skill = "Construction",
        difficulty = 5,
        automatic = false,
        power_drain = 10
    },
    replicate_tea_earl_grey_hot = {
        name = "Replicate Cup of Tea",
        workshop = "small_replicator",
        inputs = {  },
        outputs = { { item="tea_replicated", qty=1 } },
        skill = "Construction",
        difficulty = 5,
        automatic = false,
        power_drain = 20
    },
    replicate_sandwich = {
        name = "Replicate Sandwich",
        workshop = "small_replicator",
        inputs = {  },
        outputs = { { item="sandwich_replicated", qty=1 } },
        skill = "Construction",
        difficulty = 5,
        automatic = false,
        power_drain = 20
    },
    replicate_small_energy_cell = {
        name = "Replicate Small Energy Cell",
        workshop = "small_replicator",
        inputs = {  },
        outputs = { { item="small_energy_cell", qty=1 } },
        skill = "Construction",
        difficulty = 5,
        automatic = false,
        power_drain = 30
    },
    make_wooden_club = {
        name = "Make Wooden Club",
        workshop = "carpenter",
        inputs = { { item="block", qty=1 } },
        outputs = { { item="club", qty=1 } },
        skill = "Carpentry",
        difficulty = 10,
        automatic = false
    },
    make_pointy_stick = {
        name = "Make Wooden Pointy Stick",
        workshop = "carpenter",
        inputs = { { item="block", qty=1 } },
        outputs = { { item="pointy_stick", qty=1 } },
        skill = "Carpentry",
        difficulty = 7,
        automatic = false
    },
    make_atlatl = {
        name = "Make Atlatl",
        workshop = "carpenter",
        inputs = { { item="block", qty=1 } },
        outputs = { { item="atlatl", qty=1 } },
        skill = "Carpentry",
        difficulty = 15,
        automatic = false
    },
    make_wood_dart= {
        name = "Make Wooden Dart",
        workshop = "carpenter",
        inputs = { { item="block", qty=1 } },
        outputs = { { item="wood_dart", qty=1 } },
        skill = "Carpentry",
        difficulty = 12,
        automatic = false
    },

    -- Stone Age Support
    make_hide_tunic = {
        name = "Make Hide Tunic",
        workshop = "leatherworker",
        inputs = { { item="leather", qty=1 } },
        outputs = { { item="hide_tunic", qty=1 } },
        skill = "Leatherworking",
        difficulty = 10,
        automatic = false
    },
    make_hide_pants = {
        name = "Make Hide Pants",
        workshop = "leatherworker",
        inputs = { { item="leather", qty=1 } },
        outputs = { { item="hide_pants", qty=1 } },
        skill = "Leatherworking",
        difficulty = 10,
        automatic = false
    },
    make_hide_skirt = {
        name = "Make Hide Skirt",
        workshop = "leatherworker",
        inputs = { { item="leather", qty=1 } },
        outputs = { { item="hide_skirt", qty=1 } },
        skill = "Leatherworking",
        difficulty = 10,
        automatic = false
    },
    make_hide_sandals = {
        name = "Make Hide Sandals",
        workshop = "leatherworker",
        inputs = { { item="leather", qty=1 } },
        outputs = { { item="hide_sandals", qty=1 } },
        skill = "Leatherworking",
        difficulty = 10,
        automatic = false
    },
    make_leather_tunic = {
        name = "Make Leather Tunic",
        workshop = "leatherworker",
        inputs = { { item="leather", qty=1 } },
        outputs = { { item="leather_tunic", qty=1 } },
        skill = "Leatherworking",
        difficulty = 14,
        automatic = false
    },
    make_leather_pants = {
        name = "Make Leather Pants",
        workshop = "leatherworker",
        inputs = { { item="leather", qty=1 } },
        outputs = { { item="leather_pants", qty=1 } },
        skill = "Leatherworking",
        difficulty = 14,
        automatic = false
    },
    make_leather_skirt = {
        name = "Make Leather Skirt",
        workshop = "leatherworker",
        inputs = { { item="leather", qty=1 } },
        outputs = { { item="leather_skirt", qty=1 } },
        skill = "Leatherworking",
        difficulty = 14,
        automatic = false
    },
    make_boiled_leather_tunic = {
        name = "Make Boiled Leather Tunic",
        workshop = "leatherworker",
        inputs = { { item="leather", qty=2 } },
        outputs = { { item="boiled_leather_tunic", qty=1 } },
        skill = "Leatherworking",
        difficulty = 16,
        automatic = false
    },
    make_stone_club = {
        name = "Make Stone Club",
        workshop = "mason",
        inputs = { { item="block", qty=1 } },
        outputs = { { item="club", qty=1 } },
        skill = "Masonry",
        difficulty = 10,
        automatic = false
    },
    make_stone_hammer = {
        name = "Make Stone Hammer",
        workshop = "primitive_workshop",
        inputs = { { item="block", qty=1 }, { item="block", qty=1 } },
        outputs = { { item="stone_hammer", qty=1 } },
        skill = "Metalworking",
        difficulty = 10,
        automatic = false
    },
    make_stone_axe = {
        name = "Make Stone Axe",
        workshop = "primitive_workshop",
        inputs = { { item="block", qty=1 }, { item="block", qty=1 } },
        outputs = { { item="stone_axe", qty=1 } },
        skill = "Metalworking",
        difficulty = 10,
        automatic = false
    },
    make_stone_knife = {
        name = "Make Stone Knife",
        workshop = "primitive_workshop",
        inputs = { { item="block", qty=1 }, { item="block", qty=1 } },
        outputs = { { item="stone_knife", qty=1 } },
        skill = "Metalworking",
        difficulty = 10,
        automatic = false
    },

    -- Bronze Age Support
    make_leather_sandals = {
        name = "Make Leather Sandals",
        workshop = "leatherworker",
        inputs = { { item="leather", qty=1 } },
        outputs = { { item="leather_sandals", qty=1 } },
        skill = "Leatherworking",
        difficulty = 16,
        automatic = false
    },
    make_bronze_hammer = {
        name = "Make Bronze Hammer",
        workshop = "primitive_forge",
        inputs = { { item="bronze_bar", qty=1 }, { item="block", qty=1 } },
        outputs = { { item="bronze_hammer", qty=1 } },
        skill = "Metalworking",
        difficulty = 12,
        automatic = false
    },
    make_bronze_axe = {
        name = "Make Bronze Axe",
        workshop = "primitive_forge",
        inputs = { { item="bronze_bar", qty=1 }, { item="block", qty=1 } },
        outputs = { { item="bronze_axe", qty=1 } },
        skill = "Metalworking",
        difficulty = 12,
        automatic = false
    },
    make_bronze_pickaxe = {
        name = "Make Bronze Pickaxe",
        workshop = "primitive_forge",
        inputs = { { item="bronze_bar", qty=1 }, { item="block", qty=1 } },
        outputs = { { item="bronze_pickaxe", qty=1 } },
        skill = "Metalworking",
        difficulty = 12,
        automatic = false
    },
    make_bronze_dagger = {
        name = "Make Bronze Dagger",
        workshop = "primitive_forge",
        inputs = { { item="bronze_bar", qty=1 }, { item="block", qty=1 } },
        outputs = { { item="bronze_dagger", qty=1 } },
        skill = "Metalworking",
        difficulty = 12,
        automatic = false
    },
    make_bronze_sword = {
        name = "Make Bronze Sword",
        workshop = "primitive_forge",
        inputs = { { item="bronze_bar", qty=1 }, { item="block", qty=1 } },
        outputs = { { item="bronze_sword", qty=1 } },
        skill = "Metalworking",
        difficulty = 14,
        automatic = false
    },
    make_bronze_ringmail = {
        name = "Make Bronze Ringmail",
        workshop = "primitive_workshop",
        inputs = { { item="leather", qty=1 }, { item="bronze_bar", qtr=1 } },
        outputs = { { item="bronze_ringmail", qty=1 } },
        skill = "Metalworking",
        difficulty = 16,
        automatic = false
    },
    make_bronze_breastplate = {
        name = "Make Bronze Breastplate",
        workshop = "primitive_workshop",
        inputs = { { item="leather", qty=1 }, { item="bronze_bar", qtr=1 } },
        outputs = { { item="bronze_breastplate", qty=1 } },
        skill = "Metalworking",
        difficulty = 16,
        automatic = false
    },
    make_bronze_cap = {
        name = "Make Bronze Cap",
        workshop = "primitive_workshop",
        inputs = { { item="leather", qty=1 }, { item="bronze_bar", qtr=1 } },
        outputs = { { item="bronze_cap", qty=1 } },
        skill = "Metalworking",
        difficulty = 16,
        automatic = false
    },
    make_bronze_helm = {
        name = "Make Bronze Helm",
        workshop = "primitive_workshop",
        inputs = { { item="leather", qty=1 }, { item="bronze_bar", qtr=2 } },
        outputs = { { item="bronze_helm", qty=1 } },
        skill = "Metalworking",
        difficulty = 18,
        automatic = false
    },
    make_bronze_leggings = {
        name = "Make Bronze Leggings",
        workshop = "primitive_workshop",
        inputs = { { item="leather", qty=1 }, { item="bronze_bar", qtr=1 } },
        outputs = { { item="bronze_leggings", qty=1 } },
        skill = "Metalworking",
        difficulty = 16,
        automatic = false
    },
    make_bronze_skirt = {
        name = "Make Bronze Skirt",
        workshop = "primitive_workshop",
        inputs = { { item="leather", qty=1 }, { item="bronze_bar", qtr=1 } },
        outputs = { { item="bronze_skirt", qty=1 } },
        skill = "Metalworking",
        difficulty = 16,
        automatic = false
    },
}

