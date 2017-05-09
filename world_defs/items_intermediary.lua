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
    stockpile = stockpiles['component'].id
};

items["simple_lathe"] = {
    name = "Simple Lathe",
    description = "A simple lathe, required for an intermediary-level forge or carpentry workshop",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['component'].id
};

items["hay"] = {
    name="Hay",
    description = "Dried grass, suitable for animal fodder",
    itemtype = { "feed" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['yellow'], background=colors['black'],
    stockpile=stockpiles['fodder']
};

items["reed_thread"] = {
    name="Reed Fiber",
    description = "Fibers from a reed, suitable for use in cloth or wicker",
    itemtype = { "thread" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['white'], background=colors['black'],
    stockpile=stockpiles['thread']
};

items["burlap_thread"] = {
    name="Burlap Fiber",
    description = "Fibers from a jute plant, often used to make sacks",
    itemtype = { "thread" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['white'], background=colors['black'],
    stockpile=stockpiles['thread']
};

items["hemp_thread"] = {
    name="Hemp Thread",
    description = "Fibers from a hemp plant, useful for a variety of crafts",
    itemtype = { "thread" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['white'], background=colors['black'],
    stockpile=stockpiles['thread']
};

items["ramie_thread"] = {
    name="Ramie Thread",
    description = "Fibers from a ramie plant, useful for a variety of crafts",
    itemtype = { "thread" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['white'], background=colors['black'],
    stockpile=stockpiles['thread']
};

items["kenaf_thread"] = {
    name="Ramie Thread",
    description = "Fibers from a kenaf plant, useful for a variety of crafts",
    itemtype = { "thread" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['white'], background=colors['black'],
    stockpile=stockpiles['thread']
};

items["cloth"] = {
    name="Cloth",
    description = "A skein of cloth",
    itemtype = { "cloth" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['white'], background=colors['black'],
    stockpile=stockpiles['thread']
};

items["reed_paper"] = {
    name="Reed Papyrus",
    description = "Fibers from a reed, suitable for making paper",
    itemtype = { "thread" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['white'], background=colors['black'],
    stockpile=stockpiles['thread']
};
