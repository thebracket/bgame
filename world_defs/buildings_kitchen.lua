-- Cooking-related workshops

------------------------------------------------------------------------------------------------------------------------
-- An actual campfire, useful for cooking.
------------------------------------------------------------------------------------------------------------------------
buildings["camp_fire"] = {
    name = "Camp Fire",
    description = "Who doesn't like telling stories around a campfire? This is basically some wood, on fire. Surprisingly useful.",
    components = { { item="wood_log", qty=1 } },
    skill = { name="Construction", difficulty=5 },
    provides = { light={radius=5, color = colors['yellow']} },
    render_rex = "campfire.xp",
    vox = voxelId("fakefire"),
    emits_smoke = true
};

reactions["roast_food_on_real_fire"] = {
    name = "Roast simple meal",
    workshop = "camp_fire",
    inputs = { { item="any", qty=1, mat_type="food" }  },
    outputs = { { item="roast_simple", qty=1, special="cooking" } },
    skill = "Cooking",
    difficulty = 5,
    automatic = false
};

reactions["roast_combined_food_on_real_fire"] = {
    name = "Roast normal meal",
    workshop = "camp_fire",
    inputs = { { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }  },
    outputs = { { item="roast_simple", qty=1, special="cooking" } },
    skill = "Cooking",
    difficulty = 10,
    automatic = false
};

reactions["roast_lavish_meal_on_real_fire"] = {
    name = "Roast lavish meal",
    workshop = "camp_fire",
    inputs = { { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="spice" }  },
    outputs = { { item="roast_simple", qty=1, special="cooking" } },
    skill = "Cooking",
    difficulty = 15,
    automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Distilleries make booze.
------------------------------------------------------------------------------------------------------------------------

buildings["still"] = {
    name = "Distillery",
    description = "A set of tanks and water, designed to ferment plants into alcoholic beverages.",
    components = { { item="block", qty=2 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "still.xp",
    vox = voxelId("still")
};

reactions["make_booze"] = {
    name = "Ferment Alcoholic Beverage", workshop = "still",
    inputs = { { item="any", qty=1, mat_type="food" } },
    outputs = { { item="wine", qty=1, special="cooking" } },
    skill = "Brewing", difficulty = 16, automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Butchers convert dead animals into useful parts. Not controlled by reactions, it's in the C++. Sorry.
------------------------------------------------------------------------------------------------------------------------

buildings["butcher"] = {
    name = "Butcher",
    description = "A workshop dedicated to cutting up animals into bone, meat, hide and fat. Quite disgusting. Note that building this enables hunting.",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "butcher.xp",
    vox=voxelId("butcher")
};
