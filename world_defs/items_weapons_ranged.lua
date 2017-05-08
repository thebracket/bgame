-- This file defines ranged weapons and ammo
-----------------------------------------------------------------------------------------------------------------

-- Primitive ranged weapons

items["atlatl"] = {
    name = "Atlatl",
    description = "A curved piece of wood used to launch darts at high velocity",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=0, damage_stat="str", range=6, ammo="dart", initiative_penalty = 6,
    stockpile = stockpiles['weapon']
};

items["blowgun"] = {
    name = "Blowgun",
    description = "A hollow piece of wood, for blowing darts",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=0, damage_stat="str", range=4, ammo="dart", initiative_penalty = 6,
    stockpile = stockpiles['weapon']
};

items["dart"] = {
    name = "Dart",
    description = "A simple dart",
    itemtype = {"ammo"},
    glyph = glyphs['comma'],
    glyph_ascii = glyphs['comma'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_n=1, damage_d=4, damage_mod=0, stack_size=20, ammo="dart",
    stockpile = stockpiles['ammo'].id
};

-- Bows

items["shortbow"] = {
    name = "Shortbow",
    description = "A simple shortbow",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=0, damage_stat="dex", range=4, ammo="arrow", initiative_penalty = 4,
    stockpile = stockpiles['weapon'].id
};

items["composite_shortbow"] = {
    name = "Composite Shortbow",
    description = "A composite shortbow, made of bone and wood",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=2, damage_stat="dex", range=6, ammo="arrow", initiative_penalty = 4,
    stockpile = stockpiles['weapon'].id
};

items["arrow"] = {
    name = "Arrow",
    description = "A simple arrow",
    itemtype = {"ammo"},
    glyph = glyphs['greater_than_equal'],
    glyph_ascii = glyphs['greater_than_equal'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_n=1, damage_d=4, damage_mod=0, stack_size=20, ammo="arrow",
    stockpile = stockpiles['ammo'].id
};

items["longbow"] = {
    name = "Longbow",
    description = "A simple longbow",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=4, damage_stat="dex", range=8, ammo="arrow", initiative_penalty = 4,
    stockpile = stockpiles['weapon'].id
};

items["composite_longbow"] = {
    name = "Composite Longbow",
    description = "A composite longbow, made of wood and bone",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=6, damage_stat="dex", range=10, ammo="arrow", initiative_penalty = 4,
    stockpile = stockpiles['weapon'].id
};

-- Crossbows

items["heavy_crossbow"] = {
    name = "Heavy Crossbow",
    description = "A heavy crossbow, with a manual winding crank",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=4, damage_stat="dex", range=10, ammo="quarrel", initiative_penalty = 10,
    stockpile = stockpiles['weapon'].id
};

items["light_crossbow"] = {
    name = "Light Crossbow",
    description = "A light crossbow, with a manual winding crank",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=2, damage_stat="dex", range=8, ammo="quarrel", initiative_penalty = 7,
    stockpile = stockpiles['weapon'].id
};

items["repeating_crossbow"] = {
    name = "Repeating Crossbow",
    description = "A light crossbow, with automatic fire",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=2, damage_stat="dex", range=8, ammo="quarrel", initiative_penalty = 2,
    stockpile = stockpiles['weapon'].id
};

items["quarrel"] = {
    name = "Quarrel",
    description = "A crossbow quarrel",
    itemtype = {"ammo"},
    glyph = glyphs['greater_than_equal'],
    glyph_ascii = glyphs['greater_than_equal'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=0, stack_size=20, ammo="quarrel",
    stockpile = stockpiles['ammo'].id
};

-- Future Ranged Weapons

items["ray_pistol"] = {
    name = "Ray Pistol",
    description = "A tiny, one handed raygun, that looks suspiciously like a toy.",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=0, damage_stat="dex", range=4, ammo="smallcell", initiative_penalty = 4,
    stockpile = stockpiles['weapon'].id
};

items["small_energy_cell"] = {
    name = "Energy Cell",
    description = "A small energy cell, used for powering laser weaponry.",
    itemtype = {"ammo"},
    glyph = glyphs['inset_square'],
    glyph_ascii = glyphs['inset_square'],
    foreground = colors['red'],
    background = colors['black'],
    damage_n=1, damage_d=4, damage_mod=0, stack_size=20, ammo="smallcell",
    stockpile = stockpiles['ammo'].id
};