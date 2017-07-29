-- Boneworking

------------------------------------------------------------------------------------------------------------------------
-- Bonecarvers are used to turn bone into something useful.
------------------------------------------------------------------------------------------------------------------------

buildings["bonecarver"] = {
    name = "Bonecarver",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['bonecarver_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['bonecarver_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['bonecarver_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['bonecarver_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['happy_face'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['happy_face'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['vertical_line'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['vertical_line'], foreground = colors['white'], background = colors['black']}
        }
    },
};

