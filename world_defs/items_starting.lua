-- This file defines items the settlers start with on board the ship. They are currently spawned in the C++ code.
-----------------------------------------------------------------------------------------------------------------

-- Personal Survival Shelter; tent.
items["personal_survival_shelter_kit"] = {
    name = "Personal Survival Shelter Kit",
    description = "A folded-up personal shelter kit, known in non-corporate parlance as a camping set: a tent, bedroll and sleeping bag.",
    itemtype = {"component"},
    glyph = glyphs['tent_kit'],
    glyph_ascii = glyphs['kit'],
    foreground = colors['green'],
    background = colors['black'],
    stockpile = stockpiles['kit'].id,
    vox = voxelId("tent_kit")
};

-- Easy camp fire
items["camp_fire_kit"] = {
    name = "Camping Fire Kit",
    description = "A plastic campfire, powered by a tiny nuclear reactor. Ideal for your off-world vacation.",
    itemtype = {"component"},
    glyph = glyphs['campfire_kit'],
    glyph_ascii = glyphs['kit'],
    foreground = colors['red'],
    background = colors['black'],
    stockpile = stockpiles['kit'].id,
    vox = voxelId("campfire_kit")
};

-- Fire axe (to allow tree chopping at the start)
items["fire_axe"] = {
    name = "Fire Axe",
    description = "A simple axe. It has the word FIRE burned into it.",
    itemtype = {"tool-chopping"},
    glyph = glyphs['axe_chopping'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    stockpile = stockpiles['tool'].id,
    vox = voxelId("axe"),
    clothing_layer = voxelId("axe_held")
};

-- Pickaxe (to allow digging at the start)
items["pickaxe"] = {
    name = "Pickaxe",
    description = "A simple pickaxe, ideal for digging holes.",
    itemtype = {"tool-digging"},
    glyph = glyphs['axe_pickaxe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    stockpile = stockpiles['tool'].id,
    vox = voxelId("pick"),
    clothing_layer = voxelId("pick_held")
};

-- Hoe (to allow farming)
items["hoe"] = {
    name = "Hoe",
    description = "A garden hoe, used for clearing soil ready for planting.",
    itemtype = {"tool-farming"},
    glyph = glyphs['axe_pickaxe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    stockpile = stockpiles['tool'].id,
    vox = voxelId("hoe"),
    clothing_layer = voxelId("hoe_held")
};
