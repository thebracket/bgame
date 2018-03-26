-- This file defines items used as furniture
-----------------------------------------------------------------------------------------------------------------

-- Basic table
items["table"] = {
    name = "Table",
    description = "A table",
    itemtype = {"furniture"},
    glyph = glyphs['table'],
    glyph_ascii = glyphs['table'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    stockpile = stockpiles['furniture'].id,
    vox = voxelId("table")
};

-- Basic chair
items["chair"] = {
    name = "Chair",
    description = "A chair",
    itemtype = {"furniture"},
    glyph = glyphs['chair'],
    glyph_ascii = glyphs['chair'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    stockpile = stockpiles['furniture'].id,
    vox = voxelId("chair")
};

-- Basic door
items["door"] = {
    name = "Door",
    description = "A simple door",
    itemtype = {"component"},
    glyph = glyphs['door'],
    glyph_ascii = glyphs['door'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    stockpile = stockpiles['furniture'].id,
    vox = voxelId("door_ground")
};

items["floodgate"] = {
    name = "Floodate",
    description = "A heavy floodgate, designed to keep water out.",
    itemtype = {"component"},
    glyph = glyphs['door'],
    glyph_ascii = glyphs['door'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    stockpile = stockpiles['furniture'].id,
    vox = voxelId("floodgate_ground")
};

-- Basic bed
items["bed_item"] = {
    name = "Bed",
    description = "A simple bed",
    itemtype = {"component"},
    glyph = glyphs['cryo_bed'],
    glyph_ascii = glyphs['eight'],
    foreground = colors['white'],
    background = colors['black'],
    stockpile = stockpiles['furniture'].id,
    vox = voxelId("bed")
};
