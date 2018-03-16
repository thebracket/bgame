function tfn(name) return "game_assets/terrain/" .. name end
next_texture_id = 0
function tid()
    tmp = next_texture_id
    next_texture_id = next_texture_id + 3
    return tmp
end

terrain_textures = {}

function registerTerrainTexture(name, filename)
    if terrain_textures[name] ~= nil then
        print("WARNING: Duplicate texture name mapping: " .. name)
    else
        terrain_textures[name] = { index=tid(), texture=tfn(filename) }
        --print("Texture: " .. name .. ", index " .. terrain_textures[name].index .. ", filename: " .. terrain_textures[name].texture)
    end
end

-- These textures have hard-coded ID numbers. Be careful.
registerTerrainTexture("grass", "grass")
registerTerrainTexture("red_plastic", "redplastic")
registerTerrainTexture("bark", "bark")
registerTerrainTexture("leaf", "leaf")
registerTerrainTexture("water", "water")
registerTerrainTexture("window", "window")
registerTerrainTexture("germinating", "germinating")
registerTerrainTexture("sprouting", "sprouting")
registerTerrainTexture("flowering", "flowering")

--[ These textures are all references by the materials system, and are dynamically assigned.
--  They get baked into save games, so if you change them - generate a new world.
--]

-- Rock types
registerTerrainTexture("sandstone", "sandstone")
registerTerrainTexture("copper_rock", "copperrock")
registerTerrainTexture("red_rock", "redrock")
registerTerrainTexture("limestone", "limestone")
registerTerrainTexture("granite", "granite")
registerTerrainTexture("greypocked", "greypocked")
registerTerrainTexture("yellowpocked", "yellowpocked")
registerTerrainTexture("bluepocked", "bluepocked")
registerTerrainTexture("greenpocked", "greenpocked")
registerTerrainTexture("slate", "slate")
registerTerrainTexture("blackrock", "blackrock")

-- Constructed types
registerTerrainTexture("blocks_rough", "blocksrough")
registerTerrainTexture("cobbles", "cobbles")

-- Soils
registerTerrainTexture("sandy_soil", "sandsoil")
registerTerrainTexture("dry_dirt", "drydirt")
registerTerrainTexture("dry_brown_dirt", "drybrowndirt")

-- Synthetic
registerTerrainTexture("rubber", "rubber")
registerTerrainTexture("titanium", "titanium")
registerTerrainTexture("wood", "bamboo")

-- Floor Types
registerTerrainTexture("slicedrock", "slicedrock")
registerTerrainTexture("streakedmarble", "streakedmarble")
registerTerrainTexture("cavefloor", "cavefloor")
registerTerrainTexture("cavefloor_red", "cavefloorred")
registerTerrainTexture("streakedmarble_red", "streakedmarblered")
registerTerrainTexture("streakedmarble_blue", "streakedmarbleblue")
registerTerrainTexture("slipperystone", "slipperystone")
registerTerrainTexture("slicedrock_black", "slicedrockblack")
registerTerrainTexture("slicedrock_yellow", "slicedrockyellow")
registerTerrainTexture("slicedrock_blue", "slicedrockblue")
registerTerrainTexture("slipperystone_yellow", "slipperystoneyellow")
registerTerrainTexture("cement", "cement")
registerTerrainTexture("oakfloor", "oakfloor")
registerTerrainTexture("plasteelfloor", "plasteelfloor")
registerTerrainTexture("ceramictile", "ceramictile")
registerTerrainTexture("plasteeltile", "plasteeltile")
registerTerrainTexture("metalfloor", "metalfloor")

-- Blight
registerTerrainTexture("blight", "blight")

function chunkTex(name)
    if terrain_textures[name] ~= nil then
        texidx = terrain_textures[name].index
        --print("chunkTex mapping: " .. name .. " : " .. texidx)
        return texidx
    else
        print("Unknown texture: "..name)
    end
    return 0
end

-- Rock Types

materials = {
    alabaster = { name="Alabaster", type="cluster_rock", parent="gypsum", 
        glyph=glyphs['power_of'], fg=colors['white'], bg=colors['grey'],
        hit_points = 110, mines_to="alabaster_powder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed = chunkTex("streakedmarble"),
        description = "A soft rock that can be ground into plaster, or easily carved."
    },
    aluminium_rock = { name="Raw Aluminium", type="rock", layer="igneous", 
        glyph=glyphs['power_of'], fg=colors['white'], bg=colors['grey'],
        hit_points = 80, mines_to="ore", ore_materials = {"aluminium"},
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("cavefloor_red"), floor_constructed="streakedmarble_red",
        description = "An aluminium ore, that can be refined into useful metal."
    },
    alunite = { name="Alunite", type="rock", layer="igneous", 
        glyph=glyphs['backtick'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("cavefloor"), floor_constructed=chunkTex("slipperystone"),
        description = "A hydrated aluminium potassium sulfate rock."
    },
    andesite = { name="Andesite", type="rock", layer="igneous", 
        glyph=glyphs['infinity'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("cavefloor"), floor_constructed=chunkTex("slipperystone"),
        description = "A common, extrusive igneous stone."
    },
    basalt = { name="Basalt", type="rock", layer="igneous", 
        glyph=glyphs['hash'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("cavefloor"), floor_constructed=chunkTex("slipperystone"),
        description = "A common, extrusive igneous stone."
    },
    bauxite = { name="Bauxite", type="rock", layer="sedimentary", 
        glyph=glyphs['plus'], fg=colors['red'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"aluminium"},
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("cavefloor_red"), floor_constructed="streakedmarble_red",
        description = "An aluminium ore, that can be refined into useful metal."
    },
    bismunthine = { name="Bisminthine", type="rock", layer="sedimentary", 
        glyph=glyphs['percent'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials={"bismuth"},
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles"),
        floor = chunkTex("cavefloor"), floor_constructed=chunkTex("slipperystone"),
        description = "A crystalline rock, similar to ammonia. Refines into Bismuth, used in bronzeworking."
    },
    bithumous_coal = { name="Bithumous Coal", type="rock", layer="sedimentary", 
        glyph=glyphs['sun'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="coal",
        texture = chunkTex("blackrock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock_black"), floor_constructed=chunkTex("slipperystone"),
        description = "Sticky black rock from which one can extract coal and tar."
    },
    brimstone = { name="Brimstone", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="sulphur",
        texture = chunkTex("yellowpocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock_yellow"), floor_constructed=chunkTex("slipperystone_yellow"),
        description = "A smelly, sulphorous rock."
    },
    cassiterite = { name="Cassiterite", type="rock", layer="sedimentary", 
        glyph=glyphs['ukp'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"tin"},
        texture = chunkTex("blackrock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock_black"), floor_constructed=chunkTex("slipperystone"),
        description = "A dark crystalline rock."
    },
    chalk = { name="Chalk", type="rock", layer="sedimentary", 
        glyph=glyphs['one_quarter_solid'], fg=colors['white'], bg=colors['grey'],
        hit_points = 75, mines_to="stone_boulder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A crumbly rock, commonly used for writing on walls."
    },
    chert = { name="Chert", type="rock", layer="sedimentary", 
        glyph=glyphs['equals'], fg=colors['purple'], bg=colors['grey'],
        hit_points = 75, mines_to="flint", mines_to_also="stone_boulder",
        texture = chunkTex("slate"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A microcrystalline quartz rock."
    },
    chromite = { name="Chromite", type="rock", layer="igneous", 
        glyph=glyphs['equals'], fg=colors['white'], bg=colors['grey'],
        hit_points = 75, mines_to="ore", ore_materials = {"chromium"},
        texture = chunkTex("granite"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A metamorphic rock containing chrome ore."
    },
    cinnabar = { name="Cinnabar", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['red'], bg=colors['grey'],
        hit_points = 115, mines_to="mercury_ore", -- TODO: Mercury should be treated differently,
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("cavefloor_red"), floor_constructed="streakedmarble_red",
        description = "A red, crystalline rock."
    },
    claystone = { name="Claystone", type="rock", layer="sedimentary", 
        glyph=glyphs['comma'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 75, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A sedimentary rock formed largely from mud."
    },
    cobaltite = { name="Cobaltite", type="rock", layer="igneous", 
        glyph=glyphs['urk'], fg=colors['blue'], bg=colors['grey'],
        hit_points = 115, mines_to="stone_boulder",
        texture = chunkTex("bluepocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock_blue"), floor_constructed=chunkTex("streakedmarble_blue"),
        description = "Cobalt, arsenic and sulphur. Not tasty. Kobolds were named after this rock."
    },
    conglomerate = { name="Conglomerate", type="rock", layer="sedimentary", 
        glyph=glyphs['infinity'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 75, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A sedimentary rock formed largely from just about everything else."
    },
    copper_rock = { name="Native Copper", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['copper'], bg=colors['grey'],
        hit_points = 75, mines_to="ore", ore_materials = {"copper"},
        texture = chunkTex("copper_rock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "Rock that contains streaks of copper."
    },
    dacite = { name="Dacite", type="rock", layer="igneous", 
        glyph=glyphs['period'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A hard rock containing lots of quartz and feltspar."
    },
    diorite = { name="Diorite", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("granite"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A hard rock containing lots of quartz."
    },
    dolomite = { name="Dolomite", type="rock", layer="sedimentary", 
        glyph=glyphs['backtick'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A readily dissolved rock, frequently found in aquifers."
    },
    gabbro = { name="Gabbro", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greenpocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A dense rock, sometimes containing Olivine."
    },
    galena = { name="Galena", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"lead", "silver"},
        texture = chunkTex("blackrock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock_black"), floor_constructed=chunkTex("slipperystone"),
        description = "A rock made up lead and sulphur. Sure to give you brain damage if consumed!"
    },
    garnierite = { name="Garnierite", type="rock", layer="cluster_rock", parent="gabbro",
        glyph=glyphs['ukp'], fg=colors['green'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"lead", "nickel"},
        texture = chunkTex("greenpocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A nickel bearing green rock."
    },
    gneiss = { name="Gneiss", type="rock", layer="igneous", 
        glyph=glyphs['equals'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A dense rock, also the subject of numerous puns amongst geologists."
    },
    gold_rock = { name="Native Gold", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"gold"},
        texture = chunkTex("copper_rock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A dense rock, streaked with veins of gold. Dwarves love it."
    },
    granite = { name="Granite", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("granite"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A dense rock with lots of quartz."
    },
    graphite = { name="Graphite", type="rock", layer="cluster_rock", parent="gneiss",
        glyph=glyphs['circle'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("blackrock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock_black"), floor_constructed=chunkTex("slipperystone"),
        description = "A cyrstalline black rock that generally ends up in pencils."
    },
    gypsum = { name="Gypsum", type="rock", layer="sedimentary", 
        glyph=glyphs['hash'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="gypsum_powder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A white rock frequently ground into plaster."
    },
    hematite_ig = { name="Hematite", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['red_sand'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"iron"},
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("cavefloor_red"), floor_constructed="streakedmarble_red",
        description = "An iron-bearing rock, reddish in hue from the rust."
    },
    hematite_se = { name="Hematite", type="rock", layer="sedimentary", 
        glyph=glyphs['ukp'], fg=colors['red_sand'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"iron"},
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("cavefloor_red"), floor_constructed="streakedmarble_red",
        description = "An iron-bearing rock, reddish in hue from the rust."
    },
    hornblende = { name="Hornblende", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("blackrock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock_black"), floor_constructed=chunkTex("slipperystone"),
        description = "A mixture of minerals clumped together."
    },
    jet = { name="Jet", type="rock", layer="sedimentary", 
        glyph=glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("blackrock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock_black"), floor_constructed=chunkTex("slipperystone"),
        description = "A hard, black rock."
    },
    kaolinite = { name="Kaolinite", type="rock", layer="sedimentary", 
        glyph=glyphs['equals'], fg=colors['red'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A white rock made up mostly of clay."
    },
    lignite = { name="Lignite", type="rock", layer="sedimentary", 
        glyph=glyphs['sun'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="coal",
        texture = chunkTex("blackrock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock_black"), floor_constructed=chunkTex("slipperystone"),
        description = "Sticky brown rock from which one can extract coal and tar."
    },
    limestone = { name="Limestone", type="rock", layer="sedimentary", 
        glyph=glyphs['one_quarter_solid'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A soft, light-colored rock."
    },
    limonite = { name="Limonite", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"iron"},
        texture = chunkTex("yellowpocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock_yellow"), floor_constructed=chunkTex("slipperystone_yellow"),
        description = "An iron bearing rock, containing lots of sulphur."
    },
    magnetite = { name="Magnetite", type="rock", layer="igneous", 
        glyph=glyphs['tilde'], fg=colors['red'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"iron"},
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("cavefloor_red"), floor_constructed="streakedmarble_red",
        description = "An iron-bearing rock, reddish in hue from the rust."
    },
    malachite = { name="Malachite", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['green'], bg=colors['grey'],
        hit_points = 75, mines_to="ore", ore_materials = {"copper"},
        texture = chunkTex("greenpocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A green rock, frequently found in stalagtites."
    },
    marble = { name="Marble", type="rock", layer="sedimentary", 
        glyph=glyphs['one_quarter_solid'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A soft, light-colored rock popular in architecture."
    },
    microcline = { name="Microcline", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['cyan'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("bluepocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock_blue"), floor_constructed=chunkTex("streakedmarble_blue"),
        description = "A hard rock with a pleasant blue color."
    },
    mudstone = { name="Marble", type="rock", layer="sedimentary", 
        glyph=glyphs['double_wave'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A sedimentary rock formed largely from just about everything else."
    },
    obsidian = { name="Marble", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder", damage_bonus=1,
        texture = chunkTex("blackrock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock_black"), floor_constructed=chunkTex("slipperystone"),
        description = "Volcanic glass rock."
    },
    olivine = { name="Olivine", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['green'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greenpocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A green crystalline rock."
    },
    orpiment = { name="Orpiment", type="rock", layer="igneous",
        glyph=glyphs['yellow'], fg=colors['tilde'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("yellowpocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock_yellow"), floor_constructed=chunkTex("slipperystone_yellow"),
        description = "An igneous rock, yellow from sulphur."
    },
    orthoclase = { name="Orthoclase", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("granite"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A dense rock with lots of quartz."
    },
    phylite = { name="Phylite", type="rock", layer="sedimentary", 
        glyph=glyphs['minus'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("granite"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A dense rock with lots of quartz."
    },
    pitchblende = { name="Pitchblende", type="rock", layer="igneous", 
        glyph=glyphs['star'], fg=colors['purple'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"uranium"},
        texture = chunkTex("greenpocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A green rock containing Uranium. People used to grind this up for paint."
    },
    platinum_rock = { name="Native Platinum", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"platinum"},
        texture = chunkTex("copper_rock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A dense rock, streaked with veins of platinum. Dwarves love it."
    },
    pyrolusite = { name="Pyrolusite", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("granite"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A dense rock with lots of quartz."
    },
    quartzite = { name="Quartzite", type="rock", layer="igneous", 
        glyph=glyphs['minus'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("granite"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A dense rock with lots of quartz."
    },
    realgar = { name="Realgar", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['red'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("granite"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A dense rock with lots of quartz."
    },
    rhyolite = { name="Rhyolite", type="rock", layer="igneous", 
        glyph=glyphs['comma'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("granite"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A dense rock with lots of quartz."
    },
    rock_salt = { name="Rock Salt", type="rock", layer="sedimentary", 
        glyph=glyphs['hash'], fg=colors['white'], bg=colors['grey'],
        hit_points = 50, mines_to="stone_boulder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "Salt crystals, crammed together into a rock."
    },
    rutile = { name="Rutile", type="rock", layer="igneous", 
        glyph=glyphs['backtick'], fg=colors['purple'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"uranium"},
        texture = chunkTex("blackrock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock_black"), floor_constructed=chunkTex("slipperystone"),
        description = "A hard, black rock made from titantium oxide."
    },
    saltpeter = { name="Saltpeter", type="rock", layer="sedimentary", 
        glyph=glyphs['hash'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 50, mines_to="saltpeter_ore",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A primary component of gunpowder."
    },
    sandstone = { name="Sandstone", type="rock", layer="sedimentary", 
        glyph=glyphs['hash'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 50, mines_to="sand",
        texture = chunkTex("sandstone"), constructed = chunkTex("blocks_rough"),
        floor = chunkTex("slicedrock_yellow"), floor_constructed=chunkTex("slipperystone_yellow"),
        description = "A sandy stone."
    },
    serpentine = { name="Serpentine", type="rock", layer="igneous", 
        glyph=glyphs['double_wave'], fg=colors['green'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greenpocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A green crystalline rock."
    },
    shale = { name="Shale", type="rock", layer="sedimentary", 
        glyph=glyphs['period'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 50, mines_to="oil",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A soft, light-colored rock often containing oil."
    },
    siltstone = { name="Siltstone", type="rock", layer="sedimentary",
        glyph=glyphs['percent'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 50, mines_to="oil",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A soft, light-colored rock."
    },
    silver_rock = { name="Native Silver", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"silver"},
        texture = chunkTex("copper_rock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A dense rock, streaked with veins of silver. Dwarves kinda like it."
    },
    sphalerite_rock = { name="Sphalerite", type="rock", layer="sedimentary", 
        glyph=glyphs['ukp'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"zinc"},
        texture = chunkTex("copper_rock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A zinc-bearing rock."
    },
    stibnite = { name="Stibnite", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A crystalline rock."
    },
    tetrahedrite = { name="Tetrahedrite", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['copper'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"copper", "silver"},
        texture = chunkTex("copper_rock"), constructed = chunkTex("cobbles"),
        floor = chunkTex("slicedrock"), floor_constructed=chunkTex("streakedmarble"),
        description = "A dense rock, streaked with veins of silver and copper. Rated 9/10 by most Dwarves."
    },

    -- Soils

    clay_soil = { name="Clay", type="soil",
        glyph = glyphs['solid_square'], fg=colors['red'], bg=colors['black'],
        hit_points = 20, mines_to="clay",
        texture = chunkTex("dry_dirt"), constructed = chunkTex("dry_dirt"),
        floor = chunkTex("dry_dirt"), floor_constructed = chunkTex("cobbles"),
        description = ""
    },
    clay_loam = { name="Clay Loam", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['red'], bg=colors['grey'],
        hit_points = 20, mines_to="clay",
        texture = chunkTex("dry_brown_dirt"), constructed = chunkTex("dry_brown_dirt"),
        floor = chunkTex("dry_brown_dirt"), floor_constructed = chunkTex("cobbles"),
        description = ""
    },
    loam = { name="Loam", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 15, mines_to="topsoil",
        texture = chunkTex("dry_brown_dirt"), constructed = chunkTex("dry_brown_dirt"),
        floor = chunkTex("dry_brown_dirt"), floor_constructed = chunkTex("cobbles"),
        description = ""
    },
    loamy_sand = { name="Loamy Sand", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 15, mines_to="topsoil",
        texture = chunkTex("dry_brown_dirt"), constructed = chunkTex("dry_brown_dirt"),
        floor = chunkTex("dry_brown_dirt"), floor_constructed = chunkTex("cobbles"),
        description = ""
    },
    peat = { name="Peat", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 15, mines_to="topsoil",
        texture = chunkTex("dry_dirt"), constructed = chunkTex("dry_dirt"),
        floor = chunkTex("blight"), floor_constructed = chunkTex("cobbles"),
        description = ""
    },
    sandy_clay = { name="Sandy Clay", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['wood_brown'], bg=colors['red'],
        hit_points = 15, mines_to="clay",
        texture = chunkTex("dry_brown_dirt"), constructed = chunkTex("dry_brown_dirt"),
        floor = chunkTex("dry_brown_dirt"), floor_constructed = chunkTex("cobbles"),
        description = ""
    },
    sandy_clay_loam = { name="Sandy Clay Loam", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['red'], bg=colors['yellow'],
        hit_points = 15, mines_to="topsoil", mines_to_also="clay",
        texture = chunkTex("dry_brown_dirt"), constructed = chunkTex("dry_brown_dirt"),
        floor = chunkTex("dry_brown_dirt"), floor_constructed = chunkTex("cobbles"),
        description = ""
    },
    silt = { name="Silt", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['yellow'], bg=colors['black'],
        hit_points = 15, mines_to="topsoil",
        texture = chunkTex("dry_brown_dirt"), constructed = chunkTex("dry_brown_dirt"),
        floor = chunkTex("dry_brown_dirt"), floor_constructed = chunkTex("cobbles"),
        description = ""
    },
    silty_clay = { name="Silty Clay", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['red'], bg=colors['black'],
        hit_points = 15, mines_to="clay",
        texture = chunkTex("dry_dirt"), constructed = chunkTex("dry_dirt"),
        floor = chunkTex("dry_dirt"), floor_constructed = chunkTex("cobbles"),
        description = ""
    },
    silty_clay_loam = { name="Silty Clay Loam", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['red'], bg=colors['grey'],
        hit_points = 15, mines_to="topsoil",
        texture = chunkTex("dry_brown_dirt"), constructed = chunkTex("dry_brown_dirt"),
        floor = chunkTex("dry_brown_dirt"), floor_constructed = chunkTex("cobbles"),
        description = ""
    },
    silt_loam = { name="Silt Loam", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 15, mines_to="topsoil",
        texture = chunkTex("dry_dirt"), constructed = chunkTex("dry_dirt"),
        floor = chunkTex("dry_dirt"), floor_constructed = chunkTex("cobbles"),
        description = ""
    },
    sand = { name="Sand", type="sand",
        glyph = glyphs['one_quarter_solid'], fg=colors['yellow'], bg=colors['red'],
        hit_points = 10, mines_to="sand",
        texture = chunkTex("sandy_soil"), constructed = chunkTex("sandy_soil"),
        floor = chunkTex("sandy_soil"), floor_constructed = chunkTex("cobbles"),
        description = ""
    },
    black_sand = { name="Black Sand", type="sand",
        glyph = glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['black'],
        hit_points = 10, mines_to="sand",
        texture = chunkTex("sandy_soil"), constructed = chunkTex("sandy_soil"),
        floor = chunkTex("sandy_soil"), floor_constructed = chunkTex("cobbles"),
        description = ""
    },
    red_sand = { name="Red Sand", type="sand",
        glyph = glyphs['one_quarter_solid'], fg=colors['red_sand'], bg=colors['black'],
        hit_points = 10, mines_to="sand",
        texture = chunkTex("sandy_soil"), constructed = chunkTex("sandy_soil"),
        floor = chunkTex("sandy_soil"), floor_constructed = chunkTex("cobbles"),
        description = ""
    },
    white_sand = { name="Sand", type="sand",
        glyph = glyphs['one_quarter_solid'], fg=colors['white_sand'], bg=colors['black'],
        hit_points = 10, mines_to="sand",
        texture = chunkTex("sandy_soil"), constructed = chunkTex("sandy_soil"),
        floor = chunkTex("sandy_soil"), floor_constructed = chunkTex("cobbles"),
        description = ""
    },
    yellow_sand = { name="Yellow Sand", type="sand",
        glyph = glyphs['one_quarter_solid'], fg=colors['yellow_sand'], bg=colors['black'],
        hit_points = 10, mines_to="sand",
        texture = chunkTex("sandy_soil"), constructed = chunkTex("sandy_soil"),
        floor = chunkTex("sandy_soil"), floor_constructed = chunkTex("cobbles"),
        description = ""
    },

-- Other materials needed
    wood = { name = "Wood", type="synthetic", 
        glyph = glyphs['one_quarter_solid'], fg=colors['wood_brown'], bg=colors['black'],
        hit_points = 20,
        texture = chunkTex("bark"), constructed = chunkTex("wood"),
        floor = chunkTex("oakfloor"), floor_constructed = chunkTex("oakfloor")
    },
    brick = { name = "Brick", type="synthetic", 
        glyph = glyphs['one_quarter_solid'], fg=colors['wood_brown'], bg=colors['black'],
        hit_points = 100,
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles"),
        floor = chunkTex("cavefloor"), floor_constructed=chunkTex("cobbles"),
    },
    organic = { name = "Organic", type="organic", 
        glyph = glyphs['one_quarter_solid'], fg=colors['red'], bg=colors['black'],
        hit_points = 10
    },
    blight = { name = "Blight", type="blight",
        glyph = glyphs['blight'], fg=colors['brown'], bg=colors['black'],
        hit_points = 10,
        texture = chunkTex("blight"), constructed = chunkTex("blight"),
        floor = chunkTex("blight"), floor_constructed = chunkTex("blight")
    },
    food = { name = "Food:", type="food", 
        glyph = glyphs['one_quarter_solid'], fg=colors['red'], bg=colors['black'],
        hit_points = 10
    },
    spice = { name = "Spice:", type="spice", 
        glyph = glyphs['one_quarter_solid'], fg=colors['red'], bg=colors['black'],
        hit_points = 10
    },
    bone = { name = "Organic", type="organic", 
        glyph = glyphs['one_quarter_solid'], fg=colors['white'], bg=colors['black'],
        hit_points = 10, damage_bonus=1
    },
    hide = { name = "Hide", type="leather", 
        glyph = glyphs['one_quarter_solid'], fg=colors['red'], bg=colors['black'],
        hit_points = 10, ac_bonus = 0.5
    },
    leather = { name = "Leather", type="leather", 
        glyph = glyphs['one_quarter_solid'], fg=colors['red'], bg=colors['black'],
        hit_points = 10, ac_bonus = 0.75
    },
    boiled_leather = { name = "Boiled Leather", type="leather", 
        glyph = glyphs['one_quarter_solid'], fg=colors['red'], bg=colors['black'],
        hit_points = 10, ac_bonus = 1.0
    },

-- Synthetic Materials - only occur after refining
    plasteel = { name = "Plasteel", type="synthetic", 
        glyph = glyphs['one_quarter_solid'], fg=colors['white'], bg=colors['black'],
        hit_points = 200,
        texture = chunkTex("metalfloor"), constructed = chunkTex("metalfloor"),
        floor = chunkTex("titanium"), floor_constructed=chunkTex("plasteeltile")
    },
    cloth = { name = "Cloth", type="organic", 
        glyph = glyphs['one_quarter_solid'], fg=colors['red'], bg=colors['black'],
        hit_points = 10, ac_bonus = 0.0
    },

-- Metals
    aluminium = { name = "Aluminium", type="metal", 
        glyph = glyphs['one_quarter_solid'], fg=colors['aluminium_blue'], bg=colors['black'],
        hit_points = 200, damage_bonus=-1, ac_bonus=0.5
    },
    bismuth = { name = "Bismuth", type="metal", 
        glyph = glyphs['one_quarter_solid'], fg=colors['bronze'], bg=colors['black'],
        hit_points = 200, damage_bonus=1, ac_bonus=0.5
    },
    bronze = { name = "Bronze", type="metal", 
        glyph = glyphs['one_quarter_solid'], fg=colors['bronze'], bg=colors['black'],
        hit_points = 200, damage_bonus=2, ac_bonus=1.0
    },
    copper = { name = "Copper", type="metal", 
        glyph = glyphs['one_quarter_solid'], fg=colors['copper'], bg=colors['black'],
        hit_points = 200, damage_bonus=1, ac_bonus=0.5
    },
    chromium = { name = "Chromium", type="metal", 
        glyph = glyphs['one_quarter_solid'], fg=colors['chromium_blue'], bg=colors['black'],
        hit_points = 200, damage_bonus=3, ac_bonus=1.0
    },
    gold = { name = "Gold", type="metal", 
        glyph = glyphs['one_quarter_solid'], fg=colors['gold'], bg=colors['black'],
        hit_points = 200, damage_bonus=1, ac_bonus=0.5
    },
    iron = { name = "Iron", type="metal", 
        glyph = glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['black'],
        hit_points = 200, damage_bonus=3, ac_bonus=2.0
    },
    steel = { name = "Steel", type="metal", 
        glyph = glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['black'],
        hit_points = 200, damage_bonus=4, ac_bonus=3.0
    },
    lead = { name = "Lead", type="metal", 
        glyph = glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['black'],
        hit_points = 200, damage_bonus=-1, ac_bonus=0.5
    },
    nickel = { name = "Nickel", type="metal", 
        glyph = glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['black'],
        hit_points = 200, damage_bonus=-1, ac_bonus=0.5
    },
    platinum = { name = "Lead", type="metal", 
        glyph = glyphs['one_quarter_solid'], fg=colors['silver'], bg=colors['black'],
        hit_points = 200, damage_bonus=1, ac_bonus=1.0
    },
    silver = { name = "Silver", type="metal", 
        glyph = glyphs['one_quarter_solid'], fg=colors['silver'], bg=colors['black'],
        hit_points = 200, damage_bonus=1, ac_bonus=1.0
    },
    tin = { name = "Tin", type="metal", 
        glyph = glyphs['one_quarter_solid'], fg=colors['tin_blue'], bg=colors['black'],
        hit_points = 200, damage_bonus=-1, ac_bonus=0.5
    },
    uranium = { name = "Uranium", type="metal", 
        glyph = glyphs['one_quarter_solid'], fg=colors['green'], bg=colors['black'],
        hit_points = 200, damage_bonus=4, ac_bonus=4.0
    },
    zinc = { name = "Zinc", type="metal", 
        glyph = glyphs['one_quarter_solid'], fg=colors['tin_blue'], bg=colors['black'],
        hit_points = 200, damage_bonus=-1, ac_bonus=0.5
    },
}
