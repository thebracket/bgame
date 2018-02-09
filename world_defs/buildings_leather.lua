-- Leatherworking

------------------------------------------------------------------------------------------------------------------------
-- Leatherworker shops are used to sew leather into useful shapes.
------------------------------------------------------------------------------------------------------------------------

buildings["leatherworker"] = {
    name = "Leatherworker",
    description = "A workshop with all you need to create leather goods, whether practical or kinky.",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "leatherworker.xp",
    vox = voxelId("leatherworker")
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

reactions["make_combat_boots"] = {
    name = "Make Sandals",
    workshop = "leatherworker",
    inputs = { { item="leather", qty=1 } },
    outputs = { { item="combat_boots", qty=1 } },
    skill = "Leatherworking",
    difficulty = 16,
    automatic = false
};

reactions["make_high_boots"] = {
    name = "Make High Boots",
    workshop = "leatherworker",
    inputs = { { item="leather", qty=1 } },
    outputs = { { item="high_boots", qty=1 } },
    skill = "Leatherworking",
    difficulty = 16,
    automatic = false
};
