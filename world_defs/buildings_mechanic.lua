-- Mechanical Workshops

------------------------------------------------------------------------------------------------------------------------
-- Mechanic shops are used to make mechanisms
------------------------------------------------------------------------------------------------------------------------
buildings["mechanic"] = {
    name = "Mechanic Workshop",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['mechanic_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mechanic_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mechanic_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mechanic_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['star'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['star'], foreground = colors['red'], background = colors['black']}
        }
    },
};

reactions["make_simple_mechanism"] = {
    name = "Make Simple Mechanism",
    workshop = "mechanic",
    inputs = { { item="block", qty=1} },
    outputs = { { item="mechanism", qty=1 } },
    skill = "Mechanics",
    difficulty = 10,
    automatic = false
};

reactions["make_simple_circuit"] = {
    name = "Make Simple Circuit",
    workshop = "mechanic",
    inputs = { { item="raw_silicon", qty=1}, { item="block", mat_type="metal", qty=1 } },
    outputs = { { item="circuit", qty=1 } },
    skill = "Mechanics",
    difficulty = 10,
    automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Machine shops are used to create advanced mechanical devices
------------------------------------------------------------------------------------------------------------------------
buildings["machine_shop"] = {
    name = "Machine Shop",
    components = { { item="block", qty=1 }, { item="simple_lathe", qty=1 }, { item="tool_and_die", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['mechanic_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mechanic_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mechanic_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mechanic_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['star'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['star'], foreground = colors['red'], background = colors['black']}
        }
    },
};