-- Science fiction buildings

------------------------------------------------------------------------------------------------------------------------
-- Cordex starts with a small replicator on board. It's deliberately not very useful, but can get you started.
------------------------------------------------------------------------------------------------------------------------
buildings["small_replicator"] = {
    name = "Small replicator",
    description = "A portable device that can convert energy into matter, according to pre-programmed recipes. This model is designed for field-trips and can make tea and sandwiches.",
    components = { { item="replicator_unit", qty=1} },
    skill = { name="Construction", difficulty=10 },
    provides = { storage={energy_cost=0} },
    render_rex = "smallreplicator.xp",
    vox=voxelId("replicator_small")
};

reactions["replicate_tea_earl_grey_hot"] = {
    name = "Replicate Cup of Tea",
    workshop = "small_replicator",
    inputs = {  },
    outputs = { { item="tea_replicated", qty=1 } },
    skill = "Construction",
    difficulty = 5,
    automatic = false,
    power_drain = 20
};

reactions["replicate_sandwich"] = {
    name = "Replicate Sandwich",
    workshop = "small_replicator",
    inputs = {  },
    outputs = { { item="sandwich_replicated", qty=1 } },
    skill = "Construction",
    difficulty = 5,
    automatic = false,
    power_drain = 20
};

reactions["replicate_small_energy_cell"] = {
    name = "Replicate Small Energy Cell",
    workshop = "small_replicator",
    inputs = {  },
    outputs = { { item="small_energy_cell", qty=1 } },
    skill = "Construction",
    difficulty = 5,
    automatic = false,
    power_drain = 30
};

------------------------------------------------------------------------------------------------------------------------
-- Fake camp fires provide light and can replicate marshmallows.
------------------------------------------------------------------------------------------------------------------------
buildings["fake_camp_fire"] = {
    name = "Nuclear Camp Fire",
    description = "A portable camp fire, for the lazy camper. Combining a nuclear battery, fake fire effects and a marshmallow dispenser it is ideal for a camping trip.",
    components = { { item="camp_fire_kit", qty=1 } },
    skill = { name="Construction", difficulty=5 },
    provides = { light={radius=5, color = colors['yellow']} },
    render_rex = "campfire.xp",
    emits_smoke = true,
    vox = voxelId("fakefire")
};

reactions["replicate_tiny_marshmallow"] = {
    name = "Replicate Tiny Marshmallow",
    workshop = "fake_camp_fire",
    inputs = {  },
    outputs = { { item="tiny_marshmallow", qty=1 } },
    skill = "Construction",
    difficulty = 5,
    automatic = false,
    power_drain = 10
};

reactions["roast_food_on_fire"] = {
    name = "Roast simple meal",
    workshop = "fake_camp_fire",
    inputs = { { item="any", qty=1, mat_type="food" }  },
    outputs = { { item="roast_simple", qty=1, special="cooking" } },
    skill = "Cooking",
    difficulty = 5,
    automatic = false
};

reactions["roast_combined_food_on_fire"] = {
    name = "Roast normal meal",
    workshop = "fake_camp_fire",
    inputs = { { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }  },
    outputs = { { item="roast_simple", qty=1, special="cooking" } },
    skill = "Cooking",
    difficulty = 10,
    automatic = false
};

reactions["roast_lavish_meal_on_fire"] = {
    name = "Roast lavish meal",
    workshop = "fake_camp_fire",
    inputs = { { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="food" }, { item="any", qty=1, mat_type="spice" }  },
    outputs = { { item="roast_simple", qty=1, special="cooking" } },
    skill = "Cooking",
    difficulty = 15,
    automatic = false
};

