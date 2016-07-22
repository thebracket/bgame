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
        outputs = { { item="charcoal", qty=2 } },
        skill = "Furnace Operation",
        difficulty = 10,
        automatic = true
    },
    make_raw_glass = {
        name = "Make Raw Glass",
        workshop = "glass_furnace",
        inputs = { { item="charcoal", qty=1 }, { item="sand", qty=1} },
        outputs = { { item="raw_glass", qty=2 } },
        skill = "Carpentry",
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
        skill = "Furnace Operation",
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
    }
}

