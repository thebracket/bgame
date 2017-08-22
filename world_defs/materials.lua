function tfn(name) return "game_assets/terrain/" .. name end
next_texture_id = 12
function tid()
    tmp = next_texture_id
    next_texture_id = next_texture_id + 3
    return tmp
end

terrain_textures = {
    -- Hard coded for engine use
    grass = { index=0, texture=tfn("grass") },
    red_plastic = { index=3, texture=tfn("redplastic") }, -- This is used to indicate a missing texture
    bark = { index=6, texture=tfn("bark") }, -- This is used to indicate a missing texture
    leaf = { index=9, texture=tfn("leaf") }, -- This is used to indicate a missing texture

    -- Rock Types
    sandstone = { index=tid(), texture=tfn("sandstone") },
    copper_rock = { index=tid(), texture=tfn("copperrock") },
    red_rock = { index=tid(), texture=tfn("redrock") },
    limestone = { index=tid(), texture=tfn("limestone") },
    granite = { index=tid(), texture=tfn("granite") },
    greypocked = { index=tid(), texture=tfn("greypocked") },
    granite = { index=tid(), texture=tfn("granite") },
    slate = { index=tid(), texture=tfn("slate") },
    blackrock = { index=tid(), texture=tfn("blackrock") },

    -- Constructed Types
    blocks_rough = { index=tid(), texture=tfn("blocksrough") },
    cobbles = { index=tid(), texture=tfn("cobbles") },

    -- Soils
    sandy_soil = { index=tid(), texture=tfn("sandsoil") },
    dry_dirt = { index=tid(), texture=tfn("drydirt") },
    dry_brown_dirt = { index=tid(), texture=tfn("drybrowndirt") },

    -- Synthetic
    rubber = { index=tid(), texture=tfn("rubber") },
    titanium = { index=tid(), texture=tfn("titanium") }
}

function chunkTex(name)
    if terrain_textures[name] ~= nil then
        return terrain_textures[name].index
    else
        print("Unknown texture: "..name)
    end
    return 0
end

materials = {
    alabaster = { name="Alabaster", type="cluster_rock", parent="gypsum", 
        glyph=glyphs['power_of'], fg=colors['white'], bg=colors['grey'],
        hit_points = 110, mines_to="alabaster_powder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles")
    },
    aluminium_rock = { name="Raw Aluminium", type="rock", layer="igneous", 
        glyph=glyphs['power_of'], fg=colors['white'], bg=colors['grey'],
        hit_points = 80, mines_to="ore", ore_materials = {"aluminium"},
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles")
    },
    alunite = { name="Alunite", type="rock", layer="igneous", 
        glyph=glyphs['backtick'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    andesite = { name="Andesite", type="rock", layer="igneous", 
        glyph=glyphs['infinity'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    basalt = { name="Basalt", type="rock", layer="igneous", 
        glyph=glyphs['hash'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    bauxite = { name="Bauxite", type="rock", layer="sedimentary", 
        glyph=glyphs['plus'], fg=colors['red'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"aluminium"},
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles")
    },
    bismunthine = { name="Bisminthine", type="rock", layer="sedimentary", 
        glyph=glyphs['percent'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials={"bismuth"},
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles")
    },
    bithumous_coal = { name="Bithumous Coal", type="rock", layer="sedimentary", 
        glyph=glyphs['sun'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="coal",
        texture = chunkTex("blackrock"), constructed = chunkTex("cobbles")
    },
    brimstone = { name="Brimstone", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="sulphur",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    cassiterite = { name="Cassiterite", type="rock", layer="sedimentary", 
        glyph=glyphs['ukp'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"tin"},
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    chalk = { name="Chalk", type="rock", layer="sedimentary", 
        glyph=glyphs['one_quarter_solid'], fg=colors['white'], bg=colors['grey'],
        hit_points = 75, mines_to="stone_boulder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles")
    },
    chert = { name="Chert", type="rock", layer="sedimentary", 
        glyph=glyphs['equals'], fg=colors['purple'], bg=colors['grey'],
        hit_points = 75, mines_to="flint", mines_to_also="stone_boulder",
        texture = chunkTex("slate"), constructed = chunkTex("cobbles")
    },
    chromite = { name="Chromite", type="rock", layer="igneous", 
        glyph=glyphs['equals'], fg=colors['white'], bg=colors['grey'],
        hit_points = 75, mines_to="ore", ore_materials = {"chromium"},
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles")
    },
    cinnabar = { name="Cinnabar", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['red'], bg=colors['grey'],
        hit_points = 115, mines_to="mercury_ore", -- TODO: Mercury should be treated differently,
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles")
    },
    claystone = { name="Claystone", type="rock", layer="sedimentary", 
        glyph=glyphs['comma'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 75, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    cobaltite = { name="Cobaltite", type="rock", layer="igneous", 
        glyph=glyphs['urk'], fg=colors['blue'], bg=colors['grey'],
        hit_points = 115, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    conglomerate = { name="Conglomerate", type="rock", layer="sedimentary", 
        glyph=glyphs['infinity'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 75, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    copper_rock = { name="Native Copper", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['copper'], bg=colors['grey'],
        hit_points = 75, mines_to="ore", ore_materials = {"copper"},
        texture = chunkTex("copper_rock"), constructed = chunkTex("cobbles")
    },
    dacite = { name="Dacite", type="rock", layer="igneous", 
        glyph=glyphs['period'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    diorite = { name="Diorite", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("granite"), constructed = chunkTex("cobbles")
    },
    dolomite = { name="Dolomite", type="rock", layer="sedimentary", 
        glyph=glyphs['backtick'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("granite"), constructed = chunkTex("cobbles")
    },
    gabbro = { name="Gabbro", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    galena = { name="Galena", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"lead", "silver"},
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles")
    },
    garnierite = { name="Garnierite", type="rock", layer="cluster_rock", parent="gabbro",
        glyph=glyphs['ukp'], fg=colors['green'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"lead", "nickel"},
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles")
    },
    gneiss = { name="Gneiss", type="rock", layer="igneous", 
        glyph=glyphs['equals'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    gold_rock = { name="Native Gold", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"gold"},
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles")
    },
    granite = { name="Granite", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("granite"), constructed = chunkTex("cobbles")
    },
    graphite = { name="Graphite", type="rock", layer="cluster_rock", parent="gneiss",
        glyph=glyphs['circle'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("blackrock"), constructed = chunkTex("cobbles")
    },
    gypsum = { name="Gypsum", type="rock", layer="sedimentary", 
        glyph=glyphs['hash'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="gypsum_powder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles")
    },
    hematite_ig = { name="Hematite", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['red_sand'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"iron"},
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles")
    },
    hematite_se = { name="Hematite", type="rock", layer="sedimentary", 
        glyph=glyphs['ukp'], fg=colors['red_sand'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"iron"},
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles")
    },
    hornblende = { name="Hornblende", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    jet = { name="Jet", type="rock", layer="sedimentary", 
        glyph=glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("blackrock"), constructed = chunkTex("cobbles")
    },
    kaolinite = { name="Kaolinite", type="rock", layer="sedimentary", 
        glyph=glyphs['equals'], fg=colors['red'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles")
    },
    lignite = { name="Bithumous Coal", type="rock", layer="sedimentary", 
        glyph=glyphs['sun'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="coal",
        texture = chunkTex("blackrock"), constructed = chunkTex("cobbles")
    },
    limestone = { name="Limestone", type="rock", layer="sedimentary", 
        glyph=glyphs['one_quarter_solid'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles")
    },
    limonite = { name="Limonite", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"iron"},
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles")
    },
    magnetite = { name="Magnetite", type="rock", layer="igneous", 
        glyph=glyphs['tilde'], fg=colors['red'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"iron"},
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles")
    },
    malachite = { name="Malachite", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['green'], bg=colors['grey'],
        hit_points = 75, mines_to="ore", ore_materials = {"copper"},
        texture = chunkTex("copper_rock"), constructed = chunkTex("cobbles")
    },
    marble = { name="Marble", type="rock", layer="sedimentary", 
        glyph=glyphs['one_quarter_solid'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles")
    },
    microcline = { name="Microcline", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['cyan'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("granite"), constructed = chunkTex("cobbles")
    },
    mudstone = { name="Marble", type="rock", layer="sedimentary", 
        glyph=glyphs['double_wave'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    obsidian = { name="Marble", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder", damage_bonus=1,
        texture = chunkTex("blackrock"), constructed = chunkTex("cobbles")
    },
    olivine = { name="Olivine", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['green'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("granite"), constructed = chunkTex("cobbles")
    },
    orpiment = { name="Orpiment", type="rock", layer="igneous",
        glyph=glyphs['yellow'], fg=colors['tilde'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles")
    },
    orthoclase = { name="Orthoclase", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles")
    },
    phylite = { name="Phylite", type="rock", layer="sedimentary", 
        glyph=glyphs['minus'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles")
    },
    pitchblende = { name="Pitchblende", type="rock", layer="igneous", 
        glyph=glyphs['star'], fg=colors['purple'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"uranium"},
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    platinum_rock = { name="Native Platinum", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"platinum"},
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles")
    },
    pyrolusite = { name="Pyrolusite", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    quartzite = { name="Quartzite", type="rock", layer="igneous", 
        glyph=glyphs['minus'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles")
    },
    realgar = { name="Realgar", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['red'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("red_rock"), constructed = chunkTex("cobbles")
    },
    rhyolite = { name="Rhyolite", type="rock", layer="igneous", 
        glyph=glyphs['comma'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    rock_salt = { name="Rock Salt", type="rock", layer="sedimentary", 
        glyph=glyphs['hash'], fg=colors['white'], bg=colors['grey'],
        hit_points = 50, mines_to="stone_boulder",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles")
    },
    rutile = { name="Rutile", type="rock", layer="igneous", 
        glyph=glyphs['backtick'], fg=colors['purple'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"uranium"},
        texture = chunkTex("granite"), constructed = chunkTex("cobbles")
    },
    saltpeter = { name="Saltpeter", type="rock", layer="sedimentary", 
        glyph=glyphs['hash'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 50, mines_to="saltpeter_ore",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles")
    },
    sandstone = { name="Sandstone", type="rock", layer="sedimentary", 
        glyph=glyphs['hash'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 50, mines_to="sand",
        texture = chunkTex("sandstone"), constructed = chunkTex("blocks_rough")
    },
    serpentine = { name="Serpentine", type="rock", layer="igneous", 
        glyph=glyphs['double_wave'], fg=colors['green'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("granite"), constructed = chunkTex("cobbles")
    },
    shale = { name="Shale", type="rock", layer="sedimentary", 
        glyph=glyphs['period'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 50, mines_to="oil",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles")
    },
    siltstone = { name="Siltstone", type="rock", layer="sedimentary",
        glyph=glyphs['percent'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 50, mines_to="oil",
        texture = chunkTex("limestone"), constructed = chunkTex("cobbles")
    },
    silver_rock = { name="Native Silver", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"silver"},
        texture = chunkTex("copper_rock"), constructed = chunkTex("cobbles")
    },
    sphalerite_rock = { name="Sphalerite", type="rock", layer="sedimentary", 
        glyph=glyphs['ukp'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"zinc"},
        texture = chunkTex("copper_rock"), constructed = chunkTex("cobbles")
    },
    stibnite = { name="Stibnite", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder",
        texture = chunkTex("greypocked"), constructed = chunkTex("cobbles")
    },
    tetrahedrite = { name="Tetrahedrite", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['copper'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"copper", "silver"},
        texture = chunkTex("copper_rock"), constructed = chunkTex("cobbles")
    },

    -- Soils

    clay_soil = { name="Clay", type="soil",
        glyph = glyphs['solid_square'], fg=colors['red'], bg=colors['black'],
        hit_points = 20, mines_to="clay",
        texture = chunkTex("dry_dirt"), constructed = chunkTex("dry_dirt")
    },
    clay_loam = { name="Clay Loam", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['red'], bg=colors['grey'],
        hit_points = 20, mines_to="clay",
        texture = chunkTex("dry_brown_dirt"), constructed = chunkTex("dry_brown_dirt")
    },
    loam = { name="Loam", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 15, mines_to="topsoil",
        texture = chunkTex("dry_brown_dirt"), constructed = chunkTex("dry_brown_dirt")
    },
    loamy_sand = { name="Loamy Sand", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 15, mines_to="topsoil",
        texture = chunkTex("dry_brown_dirt"), constructed = chunkTex("dry_brown_dirt")
    },
    peat = { name="Peat", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 15, mines_to="topsoil",
        texture = chunkTex("dry_dirt"), constructed = chunkTex("dry_dirt")
    },
    sandy_clay = { name="Sandy Clay", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['wood_brown'], bg=colors['red'],
        hit_points = 15, mines_to="clay",
        texture = chunkTex("dry_brown_dirt"), constructed = chunkTex("dry_brown_dirt")
    },
    sandy_clay_loam = { name="Sandy Clay Loam", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['red'], bg=colors['yellow'],
        hit_points = 15, mines_to="topsoil", mines_to_also="clay",
        texture = chunkTex("dry_brown_dirt"), constructed = chunkTex("dry_brown_dirt")
    },
    silt = { name="Silt", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['yellow'], bg=colors['black'],
        hit_points = 15, mines_to="topsoil",
        texture = chunkTex("dry_brown_dirt"), constructed = chunkTex("dry_brown_dirt")
    },
    silty_clay = { name="Silty Clay", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['red'], bg=colors['black'],
        hit_points = 15, mines_to="clay",
        texture = chunkTex("dry_dirt"), constructed = chunkTex("dry_dirt")
    },
    silty_clay_loam = { name="Silty Clay Loam", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['red'], bg=colors['grey'],
        hit_points = 15, mines_to="topsoil",
        texture = chunkTex("dry_brown_dirt"), constructed = chunkTex("dry_brown_dirt")
    },
    silt_loam = { name="Silt Loam", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['grey'],
        hit_points = 15, mines_to="topsoil",
        texture = chunkTex("dry_dirt"), constructed = chunkTex("dry_dirt")
    },
    sand = { name="Sand", type="sand",
        glyph = glyphs['one_quarter_solid'], fg=colors['yellow'], bg=colors['red'],
        hit_points = 10, mines_to="sand",
        texture = chunkTex("sandy_soil"), constructed = chunkTex("sandy_soil")
    },
    black_sand = { name="Black Sand", type="sand",
        glyph = glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['black'],
        hit_points = 10, mines_to="sand",
        texture = chunkTex("sandy_soil"), constructed = chunkTex("sandy_soil")
    },
    red_sand = { name="Red Sand", type="sand",
        glyph = glyphs['one_quarter_solid'], fg=colors['red_sand'], bg=colors['black'],
        hit_points = 10, mines_to="sand",
        texture = chunkTex("sandy_soil"), constructed = chunkTex("sandy_soil")
    },
    white_sand = { name="Sand", type="sand",
        glyph = glyphs['one_quarter_solid'], fg=colors['white_sand'], bg=colors['black'],
        hit_points = 10, mines_to="sand",
        texture = chunkTex("sandy_soil"), constructed = chunkTex("sandy_soil")
    },
    yellow_sand = { name="Yellow Sand", type="sand",
        glyph = glyphs['one_quarter_solid'], fg=colors['yellow_sand'], bg=colors['black'],
        hit_points = 10, mines_to="sand",
        texture = chunkTex("sandy_soil"), constructed = chunkTex("sandy_soil")
    },

-- Other materials needed
    wood = { name = "Wood", type="synthetic", 
        glyph = glyphs['one_quarter_solid'], fg=colors['wood_brown'], bg=colors['black'],
        hit_points = 20
    },
    brick = { name = "Brick", type="synthetic", 
        glyph = glyphs['one_quarter_solid'], fg=colors['wood_brown'], bg=colors['black'],
        hit_points = 100
    },
    organic = { name = "Organic", type="organic", 
        glyph = glyphs['one_quarter_solid'], fg=colors['red'], bg=colors['black'],
        hit_points = 10
    },
    blight = { name = "Blight", type="blight",
        glyph = glyphs['blight'], fg=colors['brown'], bg=colors['black'],
        hit_points = 10
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
        texture = chunkTex("rubber"), constructed = chunkTex("rubber")
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