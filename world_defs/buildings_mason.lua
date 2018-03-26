-- Stone working

------------------------------------------------------------------------------------------------------------------------
-- Masonry shops are used to convert stone blocks into furniture
------------------------------------------------------------------------------------------------------------------------
buildings["mason"] = {
    name = "Masonry",
    description = "A workshop consisting of hammers, chisels, measuring devices - all designed for shaping rock into useful (but uncomfortable) items.",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "mason.xp",
    vox = voxelId("mason")
};

reactions["make_stone_table"] = {
    name = "Make Stone Table",
    workshop = "mason",
    inputs = { { item="block", qty=1, mat_type="rock" } },
    outputs = { { item="table", qty=1 } },
    skill = "Masonry",
    difficulty = 10,
    automatic = false
};

reactions["make_stone_chair"] = {
    name = "Make Stone Chair",
    workshop = "mason",
    inputs = { { item="block", qty=1, mat_type="rock" } },
    outputs = { { item="chair", qty=1 } },
    skill = "Masonry",
    difficulty = 10,
    automatic = false
};

reactions["make_stone_door"] = {
   name = "Make Stone Door",
   workshop = "mason",
   inputs = { { item="block", qty=1, mat_type="rock" } },
   outputs = { { item="door", qty=1 } },
   skill = "Masonry",
   difficulty = 12,
   automatic = false
};

reactions["make_stone_floodgate"] = {
   name = "Make Stone Floodgate",
   workshop = "mason",
   inputs = { { item="block", qty=1, mat_type="rock" } },
   outputs = { { item="floodgate", qty=1 } },
   skill = "Masonry",
   difficulty = 12,
   automatic = false
};

reactions["make_stone_club"] = {
    name = "Make Stone Club",
    workshop = "mason",
    inputs = { { item="block", qty=1, mat_type="rock" } },
    outputs = { { item="club", qty=1 } },
    skill = "Masonry",
    difficulty = 10,
    automatic = false
};

reactions["make_sling_bullet"] = {
    name = "Make Sling Bullet",
    workshop = "mason",
    inputs = { { item="block", qty=1, mat_type="rock" } },
    outputs = { { item="sling_bullet", qty=1 } },
    skill = "Masonry",
    difficulty = 10,
    automatic = false
};
