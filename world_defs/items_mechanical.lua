-- This file defines mechanical items.
-----------------------------------------------------------------------------------------------------------------

-- Mechanisms are an abstract item used for traps, door/bridge controls and similar.
items["mechanism"] = {
    name = "Simple Mechanism",
    description = "A collection of gears and connectors",
    itemtype = {"component"},
    glyph = glyphs['mechanism_simple'],
    glyph_ascii = glyphs['sigma'],
    foreground = colors['green'],
    background = colors['black'],
    stockpile = stockpiles['furniture'].id,
    vox = voxelId("mechanism")
};

-- Circuits are the basis of a lot of electronic items.
items["circuit"] = {
    name = "Simple Circuit",
    description = "A simple cooper/silicon circuit",
    itemtype = {"component"},
    glyph = glyphs['circuit_simple'],
    glyph_ascii = glyphs['sigma'],
    foreground = colors['yellow'],
    background = colors['black'],
    stockpile = stockpiles['furniture'].id,
    vox = voxelId("circuit")
};
