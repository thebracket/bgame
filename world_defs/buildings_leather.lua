-- Leatherworking

------------------------------------------------------------------------------------------------------------------------
-- Leatherworker shops are used to sew leather into useful shapes.
------------------------------------------------------------------------------------------------------------------------

buildings["leatherworker"] = {
    name = "Leatherworker",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['leatherworker_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['leatherworker_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['leatherworker_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['leatherworker_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['chair'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['cabinet'], foreground = colors['wood_brown'], background = colors['black']}
        }
    },
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
