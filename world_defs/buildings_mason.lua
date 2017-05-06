-- Stone working

------------------------------------------------------------------------------------------------------------------------
-- Masonry shops are used to convert stone blocks into furniture
------------------------------------------------------------------------------------------------------------------------
buildings["mason"] = {
    name = "Masonry",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['mason_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mason_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mason_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mason_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']}
        }
    },
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

reactions["make_stone_club"] = {
    name = "Make Stone Club",
    workshop = "mason",
    inputs = { { item="block", qty=1, mat_type="rock" } },
    outputs = { { item="club", qty=1 } },
    skill = "Masonry",
    difficulty = 10,
    automatic = false
};
