reactions = {
    cut_wooden_planks = {
        name = "Cut Wooden Planks",
        workshop = "sawmill",
        inputs = { { item="wood_log", qty=1 } },
        outputs = { { item="wood_plank", qty=4 } },
        skill = "Carpentry",
        difficulty = 10,
        automatic = true
    },
    cut_stone = {
        name = "Cut Stone Blocks",
        workshop = "stonecutter",
        inputs = { { item="stone_boulder", qty=1 } },
        outputs = { { item="stone_block", qty=4 } },
        skill = "Masonry",
        difficulty = 10,
        automatic = true
    }
}

