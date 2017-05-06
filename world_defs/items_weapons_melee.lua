-- This file defines melee weapons
-----------------------------------------------------------------------------------------------------------------

items["club"] = {
    name = "Club",
    description = "A simple club",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    damage_n=1, damage_d=4, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id
};

items["pointy_stick"] = {
    name = "Pointy Stick",
    description = "A sharpened stick",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    damage_n=1, damage_d=4, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id
};

items["warhammer"] = {
    name = "Warhammer",
    description = "A heavy hammer design for bashing people",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=4, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id
};

items["battleaxe"] = {
    name = "Battle Axe",
    description = "A large axe, designed for chopping people rather than trees",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=0, damage_stat="str", initiative_penalty = 6,
    stockpile = stockpiles['weapon'].id
};

items["handaxe"] = {
    name = "Hand Axe",
    description = "A small axe, optimized for hitting people quickly",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=6, damage_mod=0, damage_stat="str", initiative_penalty = 6,
    stockpile = stockpiles['weapon'].id
};

items["shortsword"] = {
    name = "Short Sword",
    description = "A short sword, designed for thrusting",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['bismuth_yellow'],
    background = colors['black'],
    damage_n=1, damage_d=6, damage_mod=0, damage_stat="str", initiative_penalty = 5,
    stockpile = stockpiles['weapon'].id
};

items["longsword"] = {
    name = "Long Sword",
    description = "A long sword, designed for slashing",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['bismuth_yellow'],
    background = colors['black'],
    damage_n=1, damage_d=6, damage_mod=0, damage_stat="str", initiative_penalty = 5,
    stockpile = stockpiles['weapon'].id
};

items["knife"] = {
    name = "Knife",
    description = "A simple knife",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=3, damage_mod=0, damage_stat="str", initiative_penalty = 2,
    stockpile = stockpiles['weapon'].id
};

items["dagger"] = {
    name = "Dagger",
    description = "A simple dagger",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=4, damage_mod=0, damage_stat="str", initiative_penalty = 2,
    stockpile = stockpiles['weapon']
};
