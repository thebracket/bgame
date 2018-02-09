-- Clothesmaking facilities

------------------------------------------------------------------------------------------------------------------------
-- Carpentry workshops are where woodwork is performed.
------------------------------------------------------------------------------------------------------------------------
buildings["tailor"] = {
    name = "Tailoring Workshop",
    description = "A simple tailoring workshop, with everything you need to make basic clothing. Includes band-aids for needle accidents.",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "tailor.xp",
    vox=voxelId("tailor")
};

reactions["make_cloth_tunic"] = {
    name = "Make Cloth Tunic",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="tunic", qty=1 } },
    skill = "Tailoring",
    difficulty = 10,
    automatic = false
};

reactions["make_cloth_pants"] = {
    name = "Make Cloth Britches",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="britches", qty=1 } },
    skill = "Tailoring",
    difficulty = 10,
    automatic = false
};

reactions["make_cloth_skirt"] = {
    name = "Make Cloth Skirt",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="skirt_simple", qty=1 } },
    skill = "Tailoring",
    difficulty = 10,
    automatic = false
};

reactions["make_batman_hat"] = {
    name = "Make Batman Hat",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="batman_hat", qty=1 } },
    skill = "Tailoring",
    difficulty = 15,
    automatic = false
};

reactions["make_police_hat"] = {
    name = "Make Batman Hat",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="police_hat", qty=1 } },
    skill = "Tailoring",
    difficulty = 15,
    automatic = false
};

reactions["make_cat_hat"] = {
    name = "Make Cat Ear Hat",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="cat_ears", qty=1 } },
    skill = "Tailoring",
    difficulty = 15,
    automatic = false
};

reactions["make_formal_shirt"] = {
    name = "Make Formal Shirt",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="formal_shirt", qty=1 } },
    skill = "Tailoring",
    difficulty = 15,
    automatic = false
};

reactions["make_formal_blouse"] = {
    name = "Make Formal Blouse",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="formal_blouse", qty=1 } },
    skill = "Tailoring",
    difficulty = 15,
    automatic = false
};

reactions["make_bathrobe"] = {
    name = "Make Bathrobe",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="bathrobe", qty=1 } },
    skill = "Tailoring",
    difficulty = 15,
    automatic = false
};

reactions["make_sports_shirt"] = {
    name = "Make Sports Shirt",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="sports_shirt", qty=1 } },
    skill = "Tailoring",
    difficulty = 15,
    automatic = false
};

reactions["make_t_shirt"] = {
    name = "Make T-Shirt",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="t_shirt", qty=1 } },
    skill = "Tailoring",
    difficulty = 15,
    automatic = false
};

reactions["make_cargo_pants"] = {
    name = "Make T-Shirt",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="cargo_pants", qty=1 } },
    skill = "Tailoring",
    difficulty = 15,
    automatic = false
};

reactions["make_long_skirt"] = {
    name = "Make Long Skirt",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="long_skirt", qty=1 } },
    skill = "Tailoring",
    difficulty = 15,
    automatic = false
};

reactions["make_hawaiian_shorts"] = {
    name = "Make Hawaiian Shorts",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="hawaiian_shorts", qty=1 } },
    skill = "Tailoring",
    difficulty = 15,
    automatic = false
};

reactions["make_sports_shorts"] = {
    name = "Make Sports Shorts",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="sports_shorts", qty=1 } },
    skill = "Tailoring",
    difficulty = 15,
    automatic = false
};

reactions["make_miniskirt"] = {
    name = "Make Miniskirt",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="miniskirt", qty=1 } },
    skill = "Tailoring",
    difficulty = 15,
    automatic = false
};

reactions["make_toga"] = {
    name = "Make Toga",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="toga", qty=1 } },
    skill = "Tailoring",
    difficulty = 10,
    automatic = false
};

reactions["make_leg_wraps"] = {
    name = "Make Leg Wraps",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="leg_wraps", qty=1 } },
    skill = "Tailoring",
    difficulty = 10,
    automatic = false
};

reactions["make_surcoat"] = {
    name = "Make Surcoat",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="surcoat", qty=1 } },
    skill = "Tailoring",
    difficulty = 10,
    automatic = false
};

reactions["make_nightcap"] = {
    name = "Make Nightcap",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="nightcap", qty=1 } },
    skill = "Tailoring",
    difficulty = 10,
    automatic = false
};

reactions["make_brocade_shirt"] = {
    name = "Make Brocade Shirt",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="brocade_shirt", qty=1 } },
    skill = "Tailoring",
    difficulty = 10,
    automatic = false
};

reactions["make_brocade_blouse"] = {
    name = "Make Brocade Blouse",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="brocade_blouse", qty=1 } },
    skill = "Tailoring",
    difficulty = 10,
    automatic = false
};

reactions["make_peasant_shirt"] = {
    name = "Make Peasant Shirt",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="peasant_shirt", qty=1 } },
    skill = "Tailoring",
    difficulty = 10,
    automatic = false
};

reactions["make_peasant_blouse"] = {
    name = "Make Peasant Blouse",
    workshop = "tailor",
    inputs = { { item="cloth", qty=1 } },
    outputs = { { item="peasant_blouse", qty=1 } },
    skill = "Tailoring",
    difficulty = 10,
    automatic = false
};

