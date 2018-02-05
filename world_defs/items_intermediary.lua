-- This file defines items that are part of other items.
-----------------------------------------------------------------------------------------------------------------

items["axe_blade"] = {
    name = "Axe Blade",
    description = "An axe blade",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['white'],
    background = colors['black'],
    stockpile = stockpiles['component'].id,
    vox = voxelId("axeblade")
};

items["simple_lathe"] = {
    name = "Simple Lathe",
    description = "A simple lathe, required for an intermediary-level forge or carpentry workshop",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['component'].id,
    vox = voxelId("lathe")
};

items["precision_tools"] = {
    name = "Precision Tools",
    description = "A set of early-Industrial era precision tools",
    name = "Tool and Die Template",
    description = "A tool-and-die template required for the construction of advanced forges and workshops.",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['component'].id,
    vox = voxelId("lathe")
};


items["smart_fabrication_controller"] = {
    name = "Smart Fabrication Controller",
    description = "A small AI unit, capable of running a fabrication facility.",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['component'].id,
    vox = voxelId("lathe")
};

items["oil_separation_tank"] = {
    name = "Oil Separation Tank",
    description = "A large metal tank stratified with extraction systems at various levels.",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['component'].id,
    vox = voxelId("liquid")
};

items["hay"] = {
    name="Hay",
    description = "Dried grass, suitable for animal fodder",
    itemtype = { "feed" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['yellow'], background=colors['black'],
    stockpile=stockpiles['fodder'],
    vox = voxelId("hay")
};

items["reed_thread"] = {
    name="Reed Fiber",
    description = "Fibers from a reed, suitable for use in cloth or wicker",
    itemtype = { "thread" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['white'], background=colors['black'],
    stockpile=stockpiles['thread'],
    vox = voxelId("thread")
};

items["burlap_thread"] = {
    name="Burlap Fiber",
    description = "Fibers from a jute plant, often used to make sacks",
    itemtype = { "thread" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['white'], background=colors['black'],
    stockpile=stockpiles['thread'],
    vox = voxelId("thread")
};

items["hemp_thread"] = {
    name="Hemp Thread",
    description = "Fibers from a hemp plant, useful for a variety of crafts",
    itemtype = { "thread" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['white'], background=colors['black'],
    stockpile=stockpiles['thread'],
    vox = voxelId("thread")
};

items["ramie_thread"] = {
    name="Ramie Thread",
    description = "Fibers from a ramie plant, useful for a variety of crafts",
    itemtype = { "thread" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['white'], background=colors['black'],
    stockpile=stockpiles['thread'],
    vox = voxelId("thread")
};

items["kenaf_thread"] = {
    name="Ramie Thread",
    description = "Fibers from a kenaf plant, useful for a variety of crafts",
    itemtype = { "thread" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['white'], background=colors['black'],
    stockpile=stockpiles['thread'],
    vox = voxelId("thread")
};

items["cloth"] = {
    name="Cloth",
    description = "A skein of cloth",
    itemtype = { "cloth" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['white'], background=colors['black'],
    stockpile=stockpiles['thread'],
    vox = voxelId("cloth")
};

items["reed_paper"] = {
    name="Reed Papyrus",
    description = "Fibers from a reed, suitable for making paper",
    itemtype = { "thread" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['white'], background=colors['black'],
    stockpile=stockpiles['thread'],
    vox = voxelId("paper")
};

items["liquified_petroleum_gas"] = {
    name = "LPG",
    description = "Liquified Petroleum Gas.",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['component'].id,
    vox = voxelId("liquid")
};

items["gasolene_fuel"] = {
    name = "Gasolene",
    description = "Gasolene Fuel",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['component'].id,
    vox = voxelId("liquid")
};

items["naptha"] = {
    name = "Naptha",
    description = "Highly flammable hydrocarbon liquid",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['component'].id,
    vox = voxelId("liquid")
};

items["lubricating_oil"] = {
    name = "Lubricating Oil",
    description = "Oil, suitable for keeping machines running",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['component'].id,
    vox = voxelId("liquid")
};

items["paraffin_wax"] = {
    name = "Paraffin Wax",
    description = "A plastic wax (flammable), a precursor to most plastics.",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['component'].id,
    vox = voxelId("liquid")
};

items["tar"] = {
    name = "Tar",
    description = "Thick, oily tar. Ideal for burning invaders, or building roads.",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['component'].id,
    vox = voxelId("liquid")
};

items["black_powder"] = {
    name = "Black Powder",
    description = "An oxidizing powder, useful for making things explode.",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['component'].id,
    vox = voxelId("powder")
};

items["nitric_acid"] = {
    name = "Nitric Acid",
    description = "A corrosive liquid, often used in the creation of explosives and fertilizers.",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['component'].id,
    vox = voxelId("liquid")
};

items["dynamite_raw"] = {
    name = "Raw Dynamite",
    description = "Dynamite clay, ready to be shaped into an explosive.",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['component'].id,
    vox = voxelId("liquid")
};

items["musket_cast"] = {
    name = "Musket Cast",
    description = "A cast for mass-production of muskets",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['component'].id,
    vox = voxelId("lathe")
};

items["dung"] = {
    name = "Dung",
    description = "Smelly animal excrement, that can be used in various chemical processes. It smells terrible.",
    itemtype = {"component"},
    glyph = glyphs['circle'],
    glyph_ascii = glyphs['circle'],
    foreground = colors['earth_brown'],
    background = colors['black'],
    stockpile = stockpiles['manure'].id,
    vox = voxelId("dung")
};

items["manure"] = {
    name = "Manure",
    description = "Smelly animal excrement and grass, aged to perfection. It smells terrible, but is good for plant growth.",
    itemtype = {"fertilizer"},
    glyph = glyphs['circle'],
    glyph_ascii = glyphs['circle'],
    foreground = colors['earth_brown'],
    background = colors['black'],
    stockpile = stockpiles['manure'].id,
    vox = voxelId("dung")
};
