-- Chemistry facilities

------------------------------------------------------------------------------------------------------------------------
-- Simple chemists handle early chemistry tasks
------------------------------------------------------------------------------------------------------------------------
buildings["simple_chemist"] = {
    name = "Simple Chemistry Workshop",
    components = { { item="block", qty=1 }, { item="raw_glass", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['carpenter_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['cabinet'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['cabinet'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['wood_brown'], background = colors['black']}
        }
    },
    vox=voxelId("chemist")
};

reactions["make_black_powder"] = {
    name = "Make Black Powder",
    workshop = "simple_chemist",
    inputs = { { item="sulphur", qty=1 }, { item="saltpeter_ore", qty=1 } },
    outputs = { { item="black_powder", qty=1 } },
    skill = "Chemistry",
    difficulty = 15,
    automatic = false
};

reactions["make_nitric_acid"] = {
    name = "Make Nitric Acid",
    workshop = "simple_chemist",
    inputs = { { item="sulphur", qty=1 }, { item="saltpeter_ore", qty=1 } },
    outputs = { { item="nitric_acid", qty=1 } },
    skill = "Chemistry",
    difficulty = 15,
    automatic = false
};

reactions["refine_dung_acid"] = {
    name = "Refine Dung into Nitric Acid",
    workshop = "simple_chemist",
    inputs = { { item="dung", qty=1 } },
    outputs = { { item="nitric_acid", qty=1 } },
    skill = "Chemistry",
    difficulty = 15,
    automatic = false
};

reactions["refine_dung_saltpeter"] = {
    name = "Refine Dung into Saltpeter",
    workshop = "simple_chemist",
    inputs = { { item="dung", qty=1 } },
    outputs = { { item="saltpeter_ore", qty=1 } },
    skill = "Chemistry",
    difficulty = 15,
    automatic = false
};

reactions["make_raw_dynamite"] = {
    name = "Make Raw Dynamite",
    workshop = "simple_chemist",
    inputs = { { item="sulphur", qty=1 }, { item="nitric_acid", qty=1 } },
    outputs = { { item="raw_dynamite", qty=1 } },
    skill = "Chemistry",
    difficulty = 15,
    automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Implements the Birkeland-Eyde process (https://en.wikipedia.org/wiki/Birkeland%E2%80%93Eyde_process) to make
-- nitric acid out of the air.
------------------------------------------------------------------------------------------------------------------------
buildings["nitrogen_extractor"] = {
    name = "Nitrogen Extractor",
    components = { { item="block", qty=2 }, { item="circuit", qty=1 } },
    skill = { name="Construction", difficulty=17 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['carpenter_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['cabinet'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['cabinet'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['wood_brown'], background = colors['black']}
        }
    },
};

reactions["extract_atmospheric_nitrogen"] = {
    name = "Refine Air into Nitric Acid",
    workshop = "nitrogen_extractor",
    inputs = {  },
    outputs = { { item="nitric_acid", qty=1 } },
    skill = "Construction",
    difficulty = 5,
    automatic = false,
    power_drain = 75
};
