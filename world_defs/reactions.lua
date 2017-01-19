reactions = {
    cut_wooden_planks = {
        name = "Cut Wooden Logs into Blocks",
        workshop = "sawmill",
        inputs = { { item="wood_log", qty=1 } },
        outputs = { { item="block", qty=4 }, { item="wood_waste", qty=2} },
        skill = "Carpentry",
        difficulty = 10,
        automatic = true
    },
    bake_bricks = {
        name = "Bake Clay into Bricks",
        workshop = "kiln",
        inputs = { { item="charcoal", qty=1 }, { item="clay", qty=1 } },
        outputs = { { item="block", qty=4 } },
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
        outputs = { { item="leather", qty=2, special="tanning" } },
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
        inputs = { { item="block", qty=1, material="wood" } },
        outputs = { { item="table", qty=1 } },
        skill = "Carpentry",
        difficulty = 10,
        automatic = false
    },
    make_stone_table = {
        name = "Make Stone Table",
        workshop = "mason",
        inputs = { { item="block", qty=1, mat_type="rock" } },
        outputs = { { item="table", qty=1 } },
        skill = "Masonry",
        difficulty = 10,
        automatic = false
    },
    make_metal_table = {
        name = "Make Metal Table",
        workshop = "mason",
        inputs = { { item="block", qty=1, mat_type="metal" } },
        outputs = { { item="table", qty=1 } },
        skill = "Metalworking",
        difficulty = 10,
        automatic = false
    },
    make_wooden_chair = {
        name = "Make Wooden Chair",
        workshop = "carpenter",
        inputs = { { item="block", qty=1, material="wood" } },
        outputs = { { item="chair", qty=1 } },
        skill = "Carpentry",
        difficulty = 10,
        automatic = false
    },
    make_stone_chair = {
        name = "Make Stone Chair",
        workshop = "mason",
        inputs = { { item="block", qty=1, mat_type="rock" } },
        outputs = { { item="chair", qty=1 } },
        skill = "Masonry",
        difficulty = 10,
        automatic = false
    },
    make_metal_chair = {
        name = "Make Metal Chair",
        workshop = "primitive_forge",
        inputs = { { item="block", qty=1, mat_type="metal" } },
        outputs = { { item="chair", qty=1 } },
        skill = "Metalworking",
        difficulty = 10,
        automatic = false
    },
    make_wooden_door = {
        name = "Make Wooden Door",
        workshop = "carpenter",
        inputs = { { item="block", qty=1, material="wood" } },
        outputs = { { item="door", qty=1 } },
        skill = "Carpentry",
        difficulty = 12,
        automatic = false
    },
    make_stone_door = {
        name = "Make Stone Door",
        workshop = "mason",
        inputs = { { item="block", qty=1, mat_type="rock" } },
        outputs = { { item="door", qty=1 } },
        skill = "Masonry",
        difficulty = 12,
        automatic = false
    },
    make_metal_door = {
        name = "Make Metal Door",
        workshop = "primitive_forge",
        inputs = { { item="block", qty=1, mat_type="metal" } },
        outputs = { { item="door", qty=1 } },
        skill = "Metalworking",
        difficulty = 12,
        automatic = false
    },
    make_simple_bed = {
        name = "Make Simple Bed",
        workshop = "carpenter",
        inputs = { { item="block", material="wood", qty=1} },
        outputs = { { item="bed_item", qty=1 } },
        skill = "Carpentry",
        difficulty = 10,
        automatic = false
    },
    make_simple_mechanism = {
        name = "Make Simple Mechanism",
        workshop = "mechanic",
        inputs = { { item="block", qty=1} },
        outputs = { { item="mechanism", qty=1 } },
        skill = "Mechanics",
        difficulty = 10,
        automatic = false
    },
    make_cage = {
        name = "Make Cage",
        workshop = "carpenter",
        inputs = { { item="block", qty=1} },
        outputs = { { item="cage", qty=1 } },
        skill = "Mechanics",
        difficulty = 10,
        automatic = false
    },
    make_trap_blade = {
        name = "Make Trap Blades",
        workshop = "primitive_forge",
        inputs = { { item="block", qty=1, mat_type="metal" } },
        outputs = { { item="trap_blade", qty=1 } },
        skill = "Metalworking",
        difficulty = 10,
        automatic = false
    },
    make_simple_circuit = {
        name = "Make Simple Circuit",
        workshop = "mechanic",
        inputs = { { item="raw_silicon", qty=1}, { item="block", mat_type="metal", qty=1 } },
        outputs = { { item="mechanism", qty=1 } },
        skill = "Mechanics",
        difficulty = 10,
        automatic = false
    },
    smelt_ore = {
        name = "Smelt Ore",
        workshop = "smelter",
        inputs = { { item="ore", qty=1 } },
        outputs = { { item="block", qty=2 } },
        skill = "Furnace Operation",
        difficulty = 12,
        automatic = true
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
        inputs = { { item="block", qty=1, material="wood" } },
        outputs = { { item="club", qty=1 } },
        skill = "Carpentry",
        difficulty = 10,
        automatic = false
    },
    make_pointy_stick = {
        name = "Make Wooden Pointy Stick",
        workshop = "carpenter",
        inputs = { { item="block", qty=1, material="wood" } },
        outputs = { { item="pointy_stick", qty=1 } },
        skill = "Carpentry",
        difficulty = 7,
        automatic = false
    },
    make_atlatl = {
        name = "Make Atlatl",
        workshop = "carpenter",
        inputs = { { item="block", qty=1, material="wood" } },
        outputs = { { item="atlatl", qty=1 } },
        skill = "Carpentry",
        difficulty = 15,
        automatic = false
    },
    make_wood_dart= {
        name = "Make Wooden Dart",
        workshop = "carpenter",
        inputs = { { item="block", qty=1, material="wood" } },
        outputs = { { item="dart", qty=1 } },
        skill = "Carpentry",
        difficulty = 12,
        automatic = false
    },

    -- Stone Age Support
    make_hide_tunic = {
        name = "Make Leather Tunic",
        workshop = "leatherworker",
        inputs = { { item="leather", qty=1 } },
        outputs = { { item="tunic", qty=1 } },
        skill = "Leatherworking",
        difficulty = 10,
        automatic = false
    },
    make_hide_pants = {
        name = "Make Leather Britches",
        workshop = "leatherworker",
        inputs = { { item="leather", qty=1 } },
        outputs = { { item="britches", qty=1 } },
        skill = "Leatherworking",
        difficulty = 10,
        automatic = false
    },
    make_hide_skirt = {
        name = "Make Leather Skirt",
        workshop = "leatherworker",
        inputs = { { item="leather", qty=1 } },
        outputs = { { item="skirt_simple", qty=1 } },
        skill = "Leatherworking",
        difficulty = 10,
        automatic = false
    },
    make_hide_sandals = {
        name = "Make Leather Sandals",
        workshop = "leatherworker",
        inputs = { { item="leather", qty=1 } },
        outputs = { { item="sandals", qty=1 } },
        skill = "Leatherworking",
        difficulty = 10,
        automatic = false
    },
    make_stone_club = {
        name = "Make Stone Club",
        workshop = "mason",
        inputs = { { item="block", qty=1, mat_type="rock" } },
        outputs = { { item="club", qty=1 } },
        skill = "Masonry",
        difficulty = 10,
        automatic = false
    },
    make_stone_hammer = {
        name = "Make Stone Warhammer",
        workshop = "primitive_workshop",
        inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="rock" } },
        outputs = { { item="warhammer", qty=1 } },
        skill = "Metalworking",
        difficulty = 10,
        automatic = false
    },
    make_stone_battleaxe = {
        name = "Make Stone Battlaxe",
        workshop = "primitive_workshop",
        inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="rock" } },
        outputs = { { item="battleaxe", qty=1 } },
        skill = "Metalworking",
        difficulty = 10,
        automatic = false
    },
    make_stone_handaxe = {
        name = "Make Stone Handaxe",
        workshop = "primitive_workshop",
        inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="rock" } },
        outputs = { { item="handaxe", qty=1 } },
        skill = "Metalworking",
        difficulty = 10,
        automatic = false
    },
    make_stone_knife = {
        name = "Make Stone Knife",
        workshop = "primitive_workshop",
        inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="rock" } },
        outputs = { { item="knife", qty=1 } },
        skill = "Metalworking",
        difficulty = 10,
        automatic = false
    },

    -- Bronze Age Support
    make_leather_sandals = {
        name = "Make Sandals",
        workshop = "leatherworker",
        inputs = { { item="leather", qty=1 } },
        outputs = { { item="sandals", qty=1 } },
        skill = "Leatherworking",
        difficulty = 16,
        automatic = false
    },
    make_metal_hammer = {
        name = "Make Metal Warhammer",
        workshop = "primitive_forge",
        inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
        outputs = { { item="warhammer", qty=1 } },
        skill = "Metalworking",
        difficulty = 12,
        automatic = false
    },
    make_metal_battleaxe = {
        name = "Make Metal Battleaxe",
        workshop = "primitive_forge",
        inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
        outputs = { { item="battleaxe", qty=1 } },
        skill = "Metalworking",
        difficulty = 12,
        automatic = false
    },
    make_metal_handaxe = {
        name = "Make Metal Hand-axe",
        workshop = "primitive_forge",
        inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
        outputs = { { item="handaxe", qty=1 } },
        skill = "Metalworking",
        difficulty = 12,
        automatic = false
    },
    make_metal_pickaxe = {
        name = "Make Metal Pickaxe",
        workshop = "primitive_forge",
        inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
        outputs = { { item="pickaxe", qty=1 } },
        skill = "Metalworking",
        difficulty = 12,
        automatic = false
    },
    make_metal_dagger = {
        name = "Make Metal Dagger",
        workshop = "primitive_forge",
        inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
        outputs = { { item="dagger", qty=1 } },
        skill = "Metalworking",
        difficulty = 12,
        automatic = false
    },
    make_metal_knife = {
        name = "Make Metal Knife",
        workshop = "primitive_forge",
        inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
        outputs = { { item="knife", qty=1 } },
        skill = "Metalworking",
        difficulty = 12,
        automatic = false
    },
    make_metal_shortsword = {
        name = "Make Short Sword",
        workshop = "primitive_forge",
        inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, mat_type="metal" } },
        outputs = { { item="shortsword", qty=1 } },
        skill = "Metalworking",
        difficulty = 14,
        automatic = false
    },
    make_metal_longsword = {
        name = "Make Longsword",
        workshop = "primitive_forge",
        inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=2, mat_type="metal" } },
        outputs = { { item="longsword", qty=1 } },
        skill = "Metalworking",
        difficulty = 16,
        automatic = false
    },
    make_ringmail = {
        name = "Make Ringmail",
        workshop = "primitive_workshop",
        inputs = { { item="leather", qty=1 }, { item="block", qty=1, mat_type="metal" } },
        outputs = { { item="ringmail", qty=1 } },
        skill = "Metalworking",
        difficulty = 16,
        automatic = false
    },
    make_breastplate = {
        name = "Make Breastplate",
        workshop = "primitive_workshop",
        inputs = { { item="leather", qty=1 }, { item="block", qty=1, mat_type="metal" } },
        outputs = { { item="breastplate", qty=1 } },
        skill = "Metalworking",
        difficulty = 16,
        automatic = false
    },
    make_cap = {
        name = "Make Metal Cap",
        workshop = "primitive_workshop",
        inputs = { { item="leather", qty=1 }, { item="block", qty=1, mat_type="metal" } },
        outputs = { { item="cap", qty=1 } },
        skill = "Metalworking",
        difficulty = 16,
        automatic = false
    },
    make_helm = {
        name = "Make Helm",
        workshop = "primitive_workshop",
        inputs = { { item="leather", qty=1 }, { item="block", qty=2, mat_type="metal" } },
        outputs = { { item="helm", qty=1 } },
        skill = "Metalworking",
        difficulty = 18,
        automatic = false
    },
    make_leggings = {
        name = "Make Armored Leggings",
        workshop = "primitive_workshop",
        inputs = { { item="leather", qty=1 }, { item="block", qty=1, mat_type="metal" } },
        outputs = { { item="leggings", qty=1 } },
        skill = "Metalworking",
        difficulty = 16,
        automatic = false
    },
    make_bronze_skirt = {
        name = "Make Metal Skirt",
        workshop = "primitive_workshop",
        inputs = { { item="leather", qty=1 }, { item="block", qty=1, mat_type="metal" } },
        outputs = { { item="skirt_simple", qty=1 } },
        skill = "Metalworking",
        difficulty = 16,
        automatic = false
    },

    -- Cooking
    roast_food_on_fire = {
        name = "Roast simple meal",
        workshop = "fake_camp_fire",
        inputs = { { item="any", qty=1, mat_type="food" }  },
        outputs = { { item="roast_simple", qty=1, special="cooking" } },
        skill = "Cooking",
        difficulty = 5,
        automatic = false
    },
    roast_combined_food_on_fire = {
        name = "Roast normal meal",
        workshop = "fake_camp_fire",
        inputs = { { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }  },
        outputs = { { item="roast_simple", qty=1, special="cooking" } },
        skill = "Cooking",
        difficulty = 10,
        automatic = false
    },
    roast_lavish_meal_on_fire = {
        name = "Roast lavish meal",
        workshop = "fake_camp_fire",
        inputs = { { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="spice" }  },
        outputs = { { item="roast_simple", qty=1, special="cooking" } },
        skill = "Cooking",
        difficulty = 15,
        automatic = false
    },

    -- Booze!
    make_booze = {
        name = "Ferment Alcoholic Beverage", workshop = "still", 
        inputs = { { item="any", qty=1, mat_type="food" } }, 
        outputs = { { item="wine", qty=1, special="cooking" } },
        skill = "Brewing", difficulty = 16, automatic = false
    },

}

