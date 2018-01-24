-- This file defines melee weapons
-----------------------------------------------------------------------------------------------------------------

-- Clubs - simple items with which one bashes things

items["club"] = {
    name = "Club",
    description = "A simple club",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    damage_n=1, damage_d=4, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("club")
};

items["spiked_club"] = {
    name = "Board With Nail",
    description = "A large board with a nail through it",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    damage_n=1, damage_d=4, damage_mod=2, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("club_spiked")
};

items["greatclub"] = {
    name = "Huge Club",
    description = "A simple club, but REALLY BIG",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("club")
};

-- Maces and flails - basically refined clubs

items["mace_light"] = {
    name = "Light Mace",
    description = "A lightweight mace, designed for bashing things",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    damage_n=1, damage_d=6, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("club")
};

items["mace_heavy"] = {
    name = "Heavy Mace",
    description = "A weighted mace, designed for bashing things",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("club")
};

items["morningstar"] = {
    name = "Morningstar",
    description = "A spiked ball on the end of a handle.",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("club")
};

items["flail_light"] = {
    name = "Light Flail",
    description = "A wooden handle, with balls on chains mounted on it.",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    damage_n=1, damage_d=6, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("club")
};

items["flail_heavy"] = {
    name = "Heavy Flail",
    description = "A metal handle, with balls on chains mounted on it.",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("club")
};

-- Spear types

items["pointy_stick"] = {
    name = "Pointy Stick",
    description = "A sharpened stick",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    damage_n=1, damage_d=4, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("pointystick")
};

items["quarterstaff"] = {
    name = "Quarterstaff",
    description = "A rounded walking stick, useful for hitting things",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    damage_n=1, damage_d=4, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("pointystick")
};

items["short_spear"] = {
    name = "Short Spear",
    description = "A short length of wood with a spike on the end",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    damage_n=1, damage_d=6, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("pointystick")
};

items["long_spear"] = {
    name = "Long Spear",
    description = "A length of wood with a spike on the end",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("pointystick")
};

items["boar_spear"] = {
    name = "Boar Spear",
    description = "A spiked length of wood with a large cross-guard",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=2, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("pointystick")
};

items["trident"] = {
    name = "Trident",
    description = "A three-pronged metal spear",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['white'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("pointystick")
};

-- Hammertime

items["light_warhammer"] = {
    name = "Light Warhammer",
    description = "A light hammer design for bashing people",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=4, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("hammer")
};

items["warhammer"] = {
    name = "Warhammer",
    description = "A heavy hammer design for bashing people",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=6, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("hammer")
};

items["hooked_warhammer"] = {
    name = "Hooked Warhammer",
    description = "A heavy hammer design for bashing people, with extra spikes",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=0, damage_stat="str",
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("hammer")
};

-- Axe variants for chopping in various ways

items["battleaxe"] = {
    name = "Battle Axe",
    description = "A large axe, designed for chopping people rather than trees",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=0, damage_stat="str", initiative_penalty = 6,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("axe")
};

items["greataxe"] = {
    name = "Great Axe",
    description = "A REALLY big axe, designed for chopping people rather than trees",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=10, damage_mod=0, damage_stat="str", initiative_penalty = 6,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("axe")
};

items["double_axe"] = {
    name = "Double-Bladed Axe",
    description = "A REALLY big axe, with two cutting edges, for when you really need to look barbaric",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=10, damage_mod=2, damage_stat="str", initiative_penalty = 6,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("axe")
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
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("axe")
};

-- Swords

items["shortsword"] = {
    name = "Short Sword",
    description = "A short sword, designed for thrusting",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['bismuth_yellow'],
    background = colors['black'],
    damage_n=1, damage_d=6, damage_mod=0, damage_stat="str", initiative_penalty = 5,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("sword")
};

items["sickle"] = {
    name = "Sickle",
    description = "An agricultural implement ideally suited to removing fingers",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['bismuth_yellow'],
    background = colors['black'],
    damage_n=1, damage_d=6, damage_mod=1, damage_stat="str", initiative_penalty = 5,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("sword")
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
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("sword")
};

items["scimitar"] = {
    name = "Scimitar",
    description = "A long sword, surprisingly bendy",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['bismuth_yellow'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=0, damage_stat="str", initiative_penalty = 5,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("sword")
};

items["rapier"] = {
    name = "Rapier",
    description = "A long, thin, sword used for making holes",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['bismuth_yellow'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=0, damage_stat="str", initiative_penalty = 5,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("sword")
};

items["sabre"] = {
    name = "Sabre",
    description = "A large sword optimized for quick slashing",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['bismuth_yellow'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=0, damage_stat="str", initiative_penalty = 2,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("sword")
};

items["cutlass"] = {
    name = "Cutlass",
    description = "A heavy sword used for bashing",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['bismuth_yellow'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=2, damage_stat="str", initiative_penalty = 5,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("sword")
};

items["falchion"] = {
    name = "Falchion",
    description = "A heavy sword used for slashing",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['bismuth_yellow'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=2, damage_stat="str", initiative_penalty = 5,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("sword")
};

items["bastard_sword"] = {
    name = "Bastard Sword",
    description = "A sword that can be used with one or two hands.",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['bismuth_yellow'],
    background = colors['black'],
    damage_n=1, damage_d=8, damage_mod=2, damage_stat="str", initiative_penalty = 5,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("sword")
};

items["greatsword"] = {
    name = "Greatsword",
    description = "A REALLY BIG sword, for when you absolutely have to slash everyone in the room.",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['bismuth_yellow'],
    background = colors['black'],
    damage_n=1, damage_d=10, damage_mod=2, damage_stat="str", initiative_penalty = 7,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("sword")
};

-- Knives and Daggers

items["knife"] = {
    name = "Knife",
    description = "A simple knife",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=3, damage_mod=0, damage_stat="str", initiative_penalty = 2,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("knife")
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
    stockpile = stockpiles['weapon'],
    vox = voxelId("knife")
};

items["punching_dagger"] = {
    name = "Punching Dagger",
    description = "A dagger designed to fit in your hand like brass knuckles, but make your punch really hurt",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=4, damage_mod=2, damage_stat="str", initiative_penalty = 2,
    stockpile = stockpiles['weapon'],
    vox = voxelId("knife")
};

items["kukri"] = {
    name = "Kukri",
    description = "A dagger with an inwardly curving blade",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=4, damage_mod=1, damage_stat="str", initiative_penalty = 2,
    stockpile = stockpiles['weapon'],
    vox = voxelId("knife")
};

-- Polearms

items["glaive"] = {
    name = "Glaive",
    description = "A wicked-looking sword on a stick",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=10, damage_mod=1, damage_stat="str", initiative_penalty = 5,
    stockpile = stockpiles['weapon'],
    vox = voxelId("pointystick")
};

items["guisarm"] = {
    name = "Guisarm",
    description = "A hooked agricultural implement, on a stick",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=2, damage_d=4, damage_mod=0, damage_stat="str", initiative_penalty = 6,
    stockpile = stockpiles['weapon'],
    vox = voxelId("pointystick")
};

items["halberd"] = {
    name = "Halberd",
    description = "A big axe, on a stick",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=10, damage_mod=0, damage_stat="str", initiative_penalty = 6,
    stockpile = stockpiles['weapon'],
    vox = voxelId("pointystick")
};

items["pike"] = {
    name = "Pike",
    description = "A wicked blade, on a stick. Useful for fending off cavalry, and pretending to be a hedgehog.",
    itemtype = {"weapon-melee"},
    glyph = glyphs['axe'],
    glyph_ascii = glyphs['paragraph'],
    foreground = colors['grey'],
    background = colors['black'],
    damage_n=1, damage_d=10, damage_mod=0, damage_stat="str", initiative_penalty = 6,
    stockpile = stockpiles['weapon'],
    vox = voxelId("pointystick")
};
