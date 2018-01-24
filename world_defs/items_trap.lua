-- This file defines items used in traps
-----------------------------------------------------------------------------------------------------------------

-- Cages, used in cage traps
items["cage"] = {
    name = "Simple Cage",
    description = "A cage, designed to hold a normal humanoid sized prisoner",
    itemtype = {"component"},
    glyph = glyphs['cage'],
    glyph_ascii = glyphs['cage_ascii'],
    foreground = colors['white'],
    background = colors['black'],
    stockpile = stockpiles['furniture'].id,
    vox = voxelId("cage")
};

-- Trap blades - used for blade traps, oddly enough.
items["trap_blade"] = {
    name = "Trap Blades",
    description = "A set of blades designed for integration into a weapon trap",
    itemtype = {"component"},
    glyph = glyphs['clubs'],
    glyph_ascii = glyphs['clubs'],
    foreground = colors['white'],
    background = colors['black'],
    stockpile = stockpiles['furniture'].id,
    vox = voxelId("axe_blade")
};

-- Spikes, used for everyone's favorite mincing machines
items["retractable_spike"] = {
    name = "Retractable Trap Spikes",
    description = "A set of retractable spikes designed for integration into a weapon trap",
    itemtype = {"component"},
    glyph = glyphs['three_bar'],
    glyph_ascii = glyphs['three_bar'],
    foreground = colors['white'],
    background = colors['black'],
    stockpile = stockpiles['furniture'].id,
    vox = voxelId("axe_blade")
};
