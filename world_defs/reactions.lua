reactions = {
    cut_wooden_planks = {
        name = "Cut Wooden Planks",
        workshop = "sawmill",
        inputs = { { item="wood_log", qty=1 } },
        outputs = { { item="wood_plank", qty=4 }, { item="wood_waste", qty=2} },
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
        automatic = true
    },
    make_raw_glass = {
        name = "Make Raw Glass",
        workshop = "glass_furnace",
        inputs = { { item="charcoal", qty=1 }, { item="sand", qty=1} },
        outputs = { { item="raw_glass", qty=2 } },
        skill = "Glassmaking",
        difficulty = 10,
        automatic = false
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
        outputs = { { item="stone_block", qty=4 } },
        skill = "Masonry",
        difficulty = 10,
        automatic = true
    },
    make_wooden_table = {
        name = "Make Wooden Table",
        workshop = "carpenter",
        inputs = { { item="wood_plank", qty=1 } },
        outputs = { { item="wood_table", qty=1 } },
        skill = "Carpentry",
        difficulty = 10,
        automatic = false
    },
    make_stone_table = {
        name = "Make Stone Table",
        workshop = "mason",
        inputs = { { item="stone_block", qty=1 } },
        outputs = { { item="stone_table", qty=1 } },
        skill = "Masonry",
        difficulty = 10,
        automatic = false
    },
    make_wooden_chair = {
        name = "Make Wooden Chair",
        workshop = "carpenter",
        inputs = { { item="wood_plank", qty=1 } },
        outputs = { { item="wood_chair", qty=1 } },
        skill = "Carpentry",
        difficulty = 10,
        automatic = false
    },
    make_axe_haft = {
        name = "Make Axe Haft",
        workshop = "carpenter",
        inputs = { { item="wood_plank", qty=1 } },
        outputs = { { item="axe_haft", qty=1 } },
        skill = "Carpentry",
        difficulty = 10,
        automatic = false
    },
    make_stone_chair = {
        name = "Make Stone Chair",
        workshop = "mason",
        inputs = { { item="stone_block", qty=1 } },
        outputs = { { item="stone_chair", qty=1 } },
        skill = "Masonry",
        difficulty = 10,
        automatic = false
    },
    make_wooden_door = {
        name = "Make Wooden Door",
        workshop = "carpenter",
        inputs = { { item="wood_plank", qty=1 } },
        outputs = { { item="wood_door", qty=1 } },
        skill = "Carpentry",
        difficulty = 12,
        automatic = false
    },
    make_stone_door = {
        name = "Make Stone Door",
        workshop = "mason",
        inputs = { { item="stone_block", qty=1 } },
        outputs = { { item="stone_door", qty=1 } },
        skill = "Masonry",
        difficulty = 12,
        automatic = false
    },
    smelt_aluminium = {
        name = "Smelt Aluminium",
        workshop = "smelter",
        inputs = { { item="aluminium_ore", qty=1 } },
        outputs = { { item="aluminium_bar", qty=2 } },
        skill = "Furnace Operation",
        difficulty = 12,
        automatic = true
    },
    smelt_tin = {
        name = "Smelt Tin",
        workshop = "smelter",
        inputs = { { item="tin_ore", qty=1 } },
        outputs = { { item="tin_bar", qty=2 } },
        skill = "Furnace Operation",
        difficulty = 12,
        automatic = true
    },
    smelt_chromium = {
        name = "Smelt Chromium",
        workshop = "smelter",
        inputs = { { item="chromium_ore", qty=1 } },
        outputs = { { item="chromium_bar", qty=2 } },
        skill = "Furnace Operation",
        difficulty = 12,
        automatic = true
    },
    smelt_tin = {
        name = "Smelt Copper",
        workshop = "smelter",
        inputs = { { item="copper_ore", qty=1 } },
        outputs = { { item="copper_bar", qty=2 } },
        skill = "Furnace Operation",
        difficulty = 12,
        automatic = true
    },
    smelt_lead = {
        name = "Smelt Lead",
        workshop = "smelter",
        inputs = { { item="lead_ore", qty=1 } },
        outputs = { { item="lead_bar", qty=2 } },
        skill = "Masonry",
        difficulty = 12,
        automatic = true
    },
    smelt_gold = {
        name = "Smelt Gold",
        workshop = "smelter",
        inputs = { { item="gold_ore", qty=1 } },
        outputs = { { item="gold_bar", qty=2 } },
        skill = "Furnace Operation",
        difficulty = 12,
        automatic = true
    },
    smelt_platinum = {
        name = "Smelt Platinum",
        workshop = "smelter",
        inputs = { { item="platinum_ore", qty=1 } },
        outputs = { { item="platinum_bar", qty=2 } },
        skill = "Furnace Operation",
        difficulty = 12,
        automatic = true
    },
    smelt_silver = {
        name = "Smelt Silver",
        workshop = "smelter",
        inputs = { { item="silver_ore", qty=1 } },
        outputs = { { item="silver_bar", qty=2 } },
        skill = "Furnace Operation",
        difficulty = 12,
        automatic = true
    },
    smelt_zinc = {
        name = "Smelt Zinc",
        workshop = "smelter",
        inputs = { { item="zinc_ore", qty=1 } },
        outputs = { { item="zinc_bar", qty=2 } },
        skill = "Furnace Operation",
        difficulty = 12,
        automatic = true
    },
    smelt_iron = {
        name = "Smelt Iron",
        workshop = "smelter",
        inputs = { { item="iron_ore", qty=1 } },
        outputs = { { item="iron_bar", qty=2 } },
        skill = "Furnace Operation",
        difficulty = 12,
        automatic = true
    },
    make_bismuth_bronze = {
        name = "Smelt Bronze Alloy",
        workshop = "smelter",
        inputs = { { item="bismuth_ore", qty=1 }, { item="copper_bar", qty=1} },
        outputs = { { item="bronze_bar", qty=1 } },
        skill = "Furnace Operation",
        difficulty = 14,
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
    make_wooden_club = {
        name = "Make Wooden Club",
        workshop = "carpenter",
        inputs = { { item="wood_plank", qty=1 } },
        outputs = { { item="club", qty=1 } },
        skill = "Carpentry",
        difficulty = 10,
        automatic = false
    },
    make_stone_club = {
        name = "Make Stone Club",
        workshop = "mason",
        inputs = { { item="stone_block", qty=1 } },
        outputs = { { item="club", qty=1 } },
        skill = "Masonry",
        difficulty = 10,
        automatic = false
    },
    make_battleaxe = {
        name = "Make Battleaxe",
        workshop = "primitive_workshop",
        inputs = { { item="axe_blade", qty=1 }, { item="axe_haft", qty=1 } },
        outputs = { { item="battleaxe", qty=1 } },
        skill = "Metalworking",
        difficulty = 10,
        automatic = false
    },
    make_battleaxe = {
        name = "Make Pickaxe",
        workshop = "primitive_workshop",
        inputs = { { item="pickaxe_blade", qty=1 }, { item="axe_haft", qty=1 } },
        outputs = { { item="pickaxe", qty=1 } },
        skill = "Metalworking",
        difficulty = 10,
        automatic = false
    },
    make_axe_blade_aluminium = { name="Make Aluminium Axe Blade", inputs={{item="aluminium_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="axe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_axe_blade_tin = { name="Make Tin Axe Blade", inputs={{item="tin_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="axe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_axe_blade_chromium = { name="Make Chrome Axe Blade", inputs={{item="chromium_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="axe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_axe_blade_copper = { name="Make Copper Axe Blade", inputs={{item="copper_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="axe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_axe_blade_lead = { name="Make Lead Axe Blade", inputs={{item="lead_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="axe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_axe_blade_gold = { name="Make Gold Axe Blade", inputs={{item="gold_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="axe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_axe_blade_iron = { name="Make Iron Axe Blade", inputs={{item="iron_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="axe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_axe_blade_platinum = { name="Make Platinum Axe Blade", inputs={{item="platinum_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="axe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_axe_blade_silver = { name="Make Silver Axe Blade", inputs={{item="silver_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="axe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_axe_blade_zinc = { name="Make Zinc Axe Blade", inputs={{item="zinc_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="axe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_axe_blade_bronze = { name="Make Bronze Axe Blade", inputs={{item="bronze_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="axe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},

    make_pickaxe_blade_aluminium = { name="Make Aluminium Pickaxe Blade", inputs={{item="aluminium_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="pickaxe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_pickaxe_blade_tin = { name="Make Tin Pickaxe Blade", inputs={{item="tin_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="pickaxe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_pickaxe_blade_chromium = { name="Make Chrome Pickaxe Blade", inputs={{item="chromium_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="pickaxe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_pickaxe_blade_copper = { name="Make Copper Pickaxe Blade", inputs={{item="copper_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="pickaxe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_pickaxe_blade_lead = { name="Make Lead Pickaxe Blade", inputs={{item="lead_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="pickaxe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_pickaxe_blade_gold = { name="Make Gold Pickaxe Blade", inputs={{item="gold_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="pickaxe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_pickaxe_blade_iron = { name="Make Iron Pickaxe Blade", inputs={{item="iron_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="pickaxe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_pickaxe_blade_platinum = { name="Make Platinum Pickaxe Blade", inputs={{item="platinum_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="pickaxe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_pickaxe_blade_silver = { name="Make Silver Pickaxe Blade", inputs={{item="silver_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="pickaxe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_pickaxe_blade_zinc = { name="Make Zinc Pickaxe Blade", inputs={{item="zinc_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="pickaxe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
    make_pickaxe_blade_bronze = { name="Make Bronze Pickaxe Blade", inputs={{item="bronze_bar", qty=1}}, workshop="primitive_forge", outputs = {{item="pickaxe_blade",qty=1}}, skill="Metalworking", difficulty=12, automatic=false},
}

