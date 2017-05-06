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

items["precision_tools"] = {
    name = "Precision Tools",
    description = "A set of early-Industrial era precision tools",
    itemtype = {"component"},
    glyph = glyphs['less_than_equal'],
    glyph_ascii = glyphs['less_than_equal'],
    foreground = colors['grey'],
    background = colors['black'],
    stockpile = stockpiles['component'].id
};

items["tool_and_die"] = {
    name = "Tool and Die Kit",
    description = "A set of tools and die-casting equipment, suitable for mass-production.",
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

items["reed_paper"] = {
    name="Reed Papyrus",
    description = "Fibers from a reed, suitable for making paper",
    itemtype = { "thread" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['white'], background=colors['black'],
    stockpile=stockpiles['thread']
};
