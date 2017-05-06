-- This file defines raw material items, that can be extracted from the environment
-----------------------------------------------------------------------------------------------------------------

-- Wood logs are the by-product of chopping down trees.
items["wood_log"] = {
    name = "Log",
    description = "Wood from a felled tree",
    itemtype = {"component"},
    glyph = glyphs['wood_log'],
    glyph_ascii = glyphs['single_line'],
    foreground = colors['white'],
    background = colors['black'],
    stockpile = stockpiles['wood'].id
};

-- Stone boulders are the by-product of cutting non-economic rock. They can also spawn in region-gen.
items["stone_boulder"] = {
    name = "Boulder",	description = "A large stone boulder",
    itemtype = {"component"}, glyph = glyphs['boulder'], foreground = colors['white'], background = colors['black'],
    glyph_ascii = glyphs['small_circle'],
    stockpile = stockpiles['stone'].id
};

-- Ore is a generic category for unrefined metals. They can be smelted to make metals, and are created by mining.
items["ore"] = {
    name = "Ore", description = "A lump of unrefined metal",
    itemtype = {"component"}, glyph = glyphs['ore'], foreground = colors['white'], background = colors['black'],
    glyph_ascii = glyphs['star'],
    stockpile = stockpiles['ore'].id
};

-- Saltpeter is handled separately because it isn't smelted. It goes into gunpowder.
items["saltpeter_ore"] = {
    name = "Saltpeter Ore", description = "A lump of unrefined saltpeter",
    itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['white'], background = colors['black'],
    glyph_ascii = glyphs['star'],
    stockpile = stockpiles['ore'].id
};

-- Alabaster powder is used for making plaster, casts and similar.
items["alabaster_powder"] = {
    name = "Alabaster Powder", description = "A lump of unrefined alabaster powder",
    itemtype = {"component"}, glyph = glyphs['inset_square'], foreground = colors['white'], background = colors['black'],
    glyph_ascii = glyphs['star'],
    stockpile = stockpiles['powder'].id
};

-- Gypsum is used in cement
items["gypsum_powder"] = {
    name = "Gypsum Powder", description = "A lump of unrefined gypsum powder",
    itemtype = {"component"}, glyph = glyphs['inset_square'], foreground = colors['white'], background = colors['black'],
    glyph_ascii = glyphs['star'],
    stockpile = stockpiles['powder'].id
};

-- Coal is a burnable substance, used for fuel in various reactions.
items["coal"] = {
    name = "Coal", description = "A lump of unrefined coal",
    itemtype = {"component"}, glyph = glyphs['ore'], foreground = colors['dark_grey'], background = colors['black'],
    glyph_ascii = glyphs['star'],
    stockpile = stockpiles['stone'].id
};

-- Sulphur is a separate powder that will eventually be used in a lot of things.
items["sulphur"] = {
    name = "Sulphur", description = "A lump of sulphur",
    itemtype = {"component"}, glyph = glyphs['inset_square'], foreground = colors['yellow'], background = colors['black'],
    glyph_ascii = glyphs['star'],
    stockpile = stockpiles['stone'].id
};

-- Flint can be knapped into weaponry
items["flint"] = {
    name = "Flint", description = "A piece of flint",
    itemtype = {"component"}, glyph = glyphs['inset_square'], foreground = colors['brown'], background = colors['black'],
    glyph_ascii = glyphs['star'],
    stockpile = stockpiles['stone'].id
};

-- Sand is the basis of the glass and silicon industries
items["sand"] = {
    name = "Sand", description = "A lump of raw sand",
    itemtype = {"component"}, glyph = glyphs['double_wave'], foreground = colors['yellow'], background = colors['black'],
    glyph_ascii = glyphs['double_wave'],
    stockpile = stockpiles['sand'].id
};

-- Oil will be a fuel and go into plastics
items["oil"] = {
    name = "Oil", description = "A blob of viscous oil",
    itemtype = {"component"}, glyph = glyphs['oil'], foreground = colors['white'], background = colors['black'],
    glyph_ascii = glyphs['double_wave'],
    stockpile = stockpiles['oil'].id
};

-- Clay is baked into bricks
items["clay"] = {
    name = "Clay", description = "A blob of clay",
    itemtype = {"component"}, glyph = glyphs['inset_square'], foreground = colors['dark_red'], background = colors['black'],
    glyph_ascii = glyphs['inset_square'],
    stockpile = stockpiles['stone'].id
};

-- Topsoil will be used in farming
items["topsoil"] = {
    name = "Topsoil", description = "A lump of rich topsoil",
    itemtype = {"component"}, glyph = glyphs['double_wave'], foreground = colors['brown'], background = colors['black'],
    glyph_ascii = glyphs['double_wave'],
    stockpile = stockpiles['soil'].id
};

-- Charcoal is the primary burnable fuel. It is made from wooden offcuts, or coal.
items["charcoal"] = {
    name = "Charcoal",
    description = "Wood, processed to burn at high temperature",
    itemtype = {"component"},
    glyph = glyphs['inset_square'],
    glyph_ascii = glyphs['inset_square'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    stockpile = stockpiles['fuel'].id
};

-- Ash is a byproduct of burning wood. It will be used to make lye.
items["ash"] = {
    name = "Ash",
    description = "Ash, remnants of burned wood",
    itemtype = {"component"},
    glyph = glyphs['one_quarter_solid'],
    glyph_ascii = glyphs['one_quarter_solid'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['powder'].id
};

-- Raw class is made by heating sand.
items["raw_glass"] = {
    name = "Raw Glass",
    description = "Raw glass, ready to turn into something useful",
    itemtype = {"component"},
    glyph = glyphs['inset_square'],
    glyph_ascii = glyphs['inset_square'],
    foreground = colors['green'],
    background = colors['black'],
    stockpile = stockpiles['glass'].id
};

-- Raw silicon is also made from sand.
items["raw_silicon"] = {
    name = "Raw Silicon",
    description = "Silicon, suitable for use in circuitry and chips",
    itemtype = {"component"},
    glyph = glyphs['inset_square'],
    glyph_ascii = glyphs['inset_square'],
    foreground = colors['green'],
    background = colors['black'],
    stockpile = stockpiles['silicon'].id
};

-- Wood waste is a byproduct of cutting wood into blocks, and is used to make fuel.
items["wood_waste"] = {
    name = "Offcuts",
    description = "Wood off-cuts",
    itemtype = {"component"},
    glyph = glyphs['three_bar'],
    glyph_ascii = glyphs['three_bar'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    stockpile = stockpiles['waste'].id
};

-- Blocks are the base building item for a LOT of things.
items["block"] = {
    name = "Block",
    description = "A convenient building block",
    itemtype = {"component"},
    glyph = glyphs['block'],
    glyph_ascii = glyphs['inset_square'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['block'].id
};

-- Animal bones
items["bone"] = {
    name = "Bone",
    description = "Bones",
    itemtype = {"bone"},
    glyph = glyphs['single_wall_ew'],
    glyph_ascii = glyphs['single_wall_ew'],
    foreground = colors['white'],
    background = colors['black'],
    stockpile = stockpiles['bone'].id
};

-- Animal hides
items["hide"] = {
    name = "Hide",
    description = "Untanned Hide",
    itemtype = {"hide"},
    glyph = glyphs['single_wall_ew'],
    glyph_ascii = glyphs['single_wall_ew'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    stockpile = stockpiles['leather'].id
};

-- Animal skulls
items["skull"] = {
    name = "Skull",
    description = "A skull",
    itemtype = {"skull"},
    glyph = glyphs['happy_face'],
    glyph_ascii = glyphs['happy_face'],
    foreground = colors['white'],
    background = colors['black'],
    stockpile = stockpiles['bone'].id
};

-- Tanned leather
items["leather"] = {
    name = "Leather",
    description = "Tanned leather, ready to make into something useful",
    itemtype = {"leather"},
    glyph = glyphs['single_wall_ew'],
    glyph_ascii = glyphs['single_wall_ew'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    stockpile = stockpiles['leather'].id
};
