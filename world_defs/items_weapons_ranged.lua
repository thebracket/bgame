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
    stockpile = stockpiles['weapon'],
    vox = voxelId("atlatl")
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
    stockpile = stockpiles['weapon'],
    vox = voxelId("pointystick")
};

items["sling"] = {
    name = "Sling",
    description = "Cloth, arranged to throw sling bullets at an enemy.",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=0, damage_stat="str", range=4, ammo="dart", initiative_penalty = 6,
    stockpile = stockpiles['weapon'],
    vox = voxelId("pointystick")
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
    stockpile = stockpiles['ammo'].id,
    vox = voxelId("arrow")
};

items["sling_bullet"] = {
    name = "Sling Bullet",
    description = "A simple rounded rock",
    itemtype = {"ammo"},
    glyph = glyphs['comma'],
    glyph_ascii = glyphs['comma'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_n=1, damage_d=4, damage_mod=0, stack_size=20, ammo="sling_bullet",
    stockpile = stockpiles['ammo'].id,
    vox = voxelId("pointystick")
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
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("bow")
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
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("bow")
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
    stockpile = stockpiles['ammo'].id,
    vox = voxelId("arrow")
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
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("bow")
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
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("bow")
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
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("crossbow")
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
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("crossbow")
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
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("crossbow")
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
    stockpile = stockpiles['ammo'].id,
    vox = voxelId("arrow")
};

-- Primitive Firearms

items["firelance_tube"] = {
    name = "Firelance",
    description = "A metal tube, into which one places a firelance spike, lights the fuse, and hopes for the best",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=2, damage_stat="dex", range=8, ammo="firelance_spike", initiative_penalty = 20,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("pointystick")
};

items["firelance_spike"] = {
    name = "Firelance Spike",
    description = "A serrated spike, designed to be launched from a firelance",
    itemtype = {"ammo"},
    glyph = glyphs['greater_than_equal'],
    glyph_ascii = glyphs['greater_than_equal'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_n=2, damage_d=8, damage_mod=0, stack_size=20, ammo="firelance_spike",
    stockpile = stockpiles['ammo'].id,
    vox = voxelId("pointystick")
};

items["handcannon"] = {
    name = "Hand Cannon",
    description = "A metal tube, into which one places shot and wadding, aim and hope for the best",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=2, damage_stat="dex", range=8, ammo="shot_wadding", initiative_penalty = 20,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("pointystick")
};

items["musket"] = {
    name = "Musket",
    description = "A muzzle-loaded, flintlock musket",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=4, damage_stat="dex", range=8, ammo="shot_wadding", initiative_penalty = 10,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("pointystick")
};

items["flintlock_pistol"] = {
    name = "Flintlock Pistol",
    description = "A flintlock pistol",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=2, damage_stat="dex", range=6, ammo="shot_wadding", initiative_penalty = 8,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("pointystick")
};

items["wheel_lock_pistol"] = {
    name = "Wheelock Pistol",
    description = "A wheelock pistol",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=2, damage_stat="dex", range=6, ammo="shot_wadding", initiative_penalty = 4,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("pointystick")
};

items["flintlock_shotgun"] = {
    name = "Flintlock Shotgun",
    description = "A flintlock shotgun",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=20, damage_stat="dex", range=4, ammo="shot_wadding", initiative_penalty = 8,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("pointystick")
};

items["blunderbuss"] = {
    name = "Blunderbuss",
    description = "A blunderbuss",
    itemtype = {"weapon-ranged"},
    glyph = glyphs['curly_right'],
    glyph_ascii = glyphs['curly_right'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_mod=20, damage_stat="dex", range=3, ammo="shot_wadding", initiative_penalty = 8,
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("pointystick")
};

items["shot_wadding"] = {
    name = "Shot and Wadding",
    description = "A metal ball, piece of cloth, and accompanying load of black powder",
    itemtype = {"ammo"},
    glyph = glyphs['greater_than_equal'],
    glyph_ascii = glyphs['greater_than_equal'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    damage_n=2, damage_d=8, damage_mod=0, stack_size=20, ammo="shot_wadding",
    stockpile = stockpiles['ammo'].id,
    vox = voxelId("pointystick")
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
    stockpile = stockpiles['weapon'].id,
    vox = voxelId("pointystick")
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
    stockpile = stockpiles['ammo'].id,
    vox = voxelId("pointystick")
};