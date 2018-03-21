-- This file defines items that one can eat or drink.
-----------------------------------------------------------------------------------------------------------------

items["tiny_marshmallow"] = {
    name = "Tiny Marshmallow",
    description = "A tiny lump of congealed sugar",
    itemtype = {"food-prepared"},
    glyph = glyphs['squared'],
    glyph_ascii = glyphs['squared'],
    foreground = colors['magenta'],
    background = colors['black'],
    stockpile = stockpiles['food'].id
};

items["tea_replicated"] = {
    name = "Tea, Earl-Grey Lukewarm",
    description = "A replicated substance that tastes somewhat like tea",
    itemtype = {"drink"},
    glyph = glyphs['squared'],
    glyph_ascii = glyphs['squared'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    stockpile = stockpiles['food'].id
};

items["sandwich_replicated"] = {
    name = "Stale-tasting sandwich",
    description = "A replicated sandwich that sort-of tastes like chicken",
    itemtype = {"food-prepared"},
    glyph = glyphs['squared'],
    glyph_ascii = glyphs['squared'],
    foreground = colors['wood_brown'],
    background = colors['black'],
    stockpile = stockpiles['food'].id
};

-- Raw meat, turns into a food-stuff
items["meat"] = {
    name = "Meat",
    description = "Raw Meat",
    itemtype = {"food"},
    glyph = glyphs['double_pillar'],
    glyph_ascii = glyphs['double_pillar'],
    foreground = colors['red'],
    background = colors['black'],
    stockpile = stockpiles['food'].id,
    vox = voxelId("meat")
};

-- Cooked Food
items["roast_simple"] = {
    name="Roasted",
    description = "Roasted food",
    itemtype = { "food-prepared" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['brown'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("foodplate")
};

items["wine"] = {
    name="Alcoholic Beverage",
    description = "Fermented food",
    itemtype = { "drink" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("liquid")
};

------------- Raw herb and veg

items["artichoke_veg"] = {
    name="Raw Artichoke",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["asparagus_veg"] = {
    name="Raw Asparagus",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["string_bean_veg"] = {
    name="Raw String Bean",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["broad_bean_veg"] = {
    name="Raw Broad Bean",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["beetroot_veg"] = {
    name="Raw Beetroot",
    description = "A root vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['purple'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["goya_veg"] = {
    name="Raw Goya",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["cabbage_veg"] = {
    name="Raw Cabbage",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["caper_veg"] = {
    name="Raw Caper",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["carrot_veg"] = {
    name="Raw Carrot",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['orange'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["cassava_veg"] = {
    name="Raw Cassava",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["celery_veg"] = {
    name="Raw Celery",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["chickpea_veg"] = {
    name="Raw Chickpea",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["cowpea_veg"] = {
    name="Raw Cowpea",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["cucumber_veg"] = {
    name="Raw Cucumber",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["eggplant_veg"] = {
    name="Raw Eggplant",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["cress_veg"] = {
    name="Raw Cress",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["melon_veg"] = {
    name="Raw Melon",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["leek_veg"] = {
    name="Raw Leek",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["lentil_veg"] = {
    name="Raw Lentils",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["lettuce_veg"] = {
    name="Raw Lettuce",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["mung_bean_veg"] = {
    name="Raw Mung Bean",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["onion_veg"] = {
    name="Raw Onion",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["parsnip_veg"] = {
    name="Raw Parsnip",
    description = "A root vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["pea_veg"] = {
    name="Raw Peas",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["pepper_veg"] = {
    name="Raw Bell Pepper",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["potato_veg"] = {
    name="Raw Potato",
    description = "A root vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["radish_veg"] = {
    name="Raw Radish",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["red_bean_veg"] = {
    name="Raw Red Beans",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["spinach_veg"] = {
    name="Raw Spinach",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["soybean_veg"] = {
    name="Raw Soy",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["squash_veg"] = {
    name="Raw Squash",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["sweet_potato_veg"] = {
    name="Raw Sweet Potato",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['orange'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["tomato_veg"] = {
    name="Raw Tomato",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['red'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["tomatillo_veg"] = {
    name="Raw Tomatillo",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["turnip_veg"] = {
    name="Raw Turnip",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["watermelon_veg"] = {
    name="Raw Watermelon",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["yam_veg"] = {
    name="Raw Yam",
    description = "A green vegetable",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['yellow'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["strawberry_fruit"] = {
    name="Raw Strawberry",
    description = "A red fruit",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['red'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["bambara_nut"] = {
    name="Bambara Nut",
    description = "A tasty nut",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['red'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["peanut_nut"] = {
    name="Peanut",
    description = "A tasty nut",
    itemtype = { "food" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['brown'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["chickory_herb"] = {
    name="Raw Chickory",
    description = "A tasty herb",
    itemtype = { "spice" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['brown'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["garlic_herb"] = {
    name="Raw Garlic",
    description = "A tasty herb",
    itemtype = { "spice" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['brown'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["rhubarb_herb"] = {
    name="Raw Rhubarb",
    description = "A tasty herb",
    itemtype = { "spice" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['brown'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["lavendar_herb"] = {
    name="Raw Lavendar",
    description = "A tasty herb",
    itemtype = { "spice" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['purple'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["sage_herb"] = {
    name="Raw Sage",
    description = "A tasty herb",
    itemtype = { "spice" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['inv_exclaim'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['food'],
    vox = voxelId("veggie")
};

items["seed"] = {
    name="Seeds",
    description = "Seeds that when tended properly will grow into a lovely plant",
    itemtype = { "seed" }, glyph = glyphs["inv_exclaim"],
    glyph_ascii = glyphs['colon'],
    foreground=colors['green'], background=colors['black'],
    stockpile=stockpiles['seeds'],
    vox = voxelId("seed")
};
