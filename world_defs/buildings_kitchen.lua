-- Cooking-related workshops

------------------------------------------------------------------------------------------------------------------------
-- An actual campfire, useful for cooking.
------------------------------------------------------------------------------------------------------------------------
buildings["camp_fire"] = {
    name = "Camp Fire",
    components = { { item="wood_log", qty=1 } },
    skill = { name="Construction", difficulty=5 },
    provides = { light={radius=5, color = colors['yellow']} },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['campfire'], foreground = colors['firelight'], background = colors['yellow']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['sun'], foreground = colors['yellow'], background = colors['yellow']}
        }
    },
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
    components = { { item="block", qty=2 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['still_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['still_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['still_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['still_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['kit'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['kit'], foreground = colors['wood_brown'], background = colors['black']}
        }
    },
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
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=3, height=3, tiles= {
            {glyph= glyphs['butcher_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['butcher_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['butcher_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['butcher_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['butcher_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['butcher_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['butcher_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['butcher_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['cabinet'], foreground = colors['red'], background = colors['black']},
            {glyph= glyphs['kit'], foreground = colors['red'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['red'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['red'], background = colors['black']}
        }
    },
    vox=voxelId("butcher")
};
