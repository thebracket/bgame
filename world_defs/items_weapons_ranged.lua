-- This file defines ranged weapons and ammo
-----------------------------------------------------------------------------------------------------------------

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

