materials = {
    alabaster = { name="Alabaster", type="cluster_rock", parent="gypsum", 
        glyph=glyphs['power_of'], fg=colors['white'], bg=colors['grey'],
        hit_points = 110, mines_to="alabaster_powder" 
    },
    aluminium_rock = { name="Raw Aluminium", type="rock", layer="igneous", 
        glyph=glyphs['power_of'], fg=colors['white'], bg=colors['grey'],
        hit_points = 80, mines_to="ore", ore_materials = {"aluminium"}
    },
    alunite = { name="Alunite", type="rock", layer="igneous", 
        glyph=glyphs['backtick'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    andesite = { name="Andesite", type="rock", layer="igneous", 
        glyph=glyphs['infinity'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    basalt = { name="Basalt", type="rock", layer="igneous", 
        glyph=glyphs['hash'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    bauxite = { name="Bauxite", type="rock", layer="sedimentary", 
        glyph=glyphs['plus'], fg=colors['dark_red'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"aluminium"}
    },
    bismunthine = { name="Bisminthine", type="rock", layer="sedimentary", 
        glyph=glyphs['percent'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials={"bismuth"}
    },
    bithumous_coal = { name="Bithumous Coal", type="rock", layer="sedimentary", 
        glyph=glyphs['sun'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="coal"
    },
    brimstone = { name="Brimstone", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="sulphur"
    },
    cassiterite = { name="Cassiterite", type="rock", layer="sedimentary", 
        glyph=glyphs['ukp'], fg=colors['dark_yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"tin"}
    },
    chalk = { name="Chalk", type="rock", layer="sedimentary", 
        glyph=glyphs['one_quarter_solid'], fg=colors['white'], bg=colors['grey'],
        hit_points = 75, mines_to="stone_boulder"
    },
    chert = { name="Chert", type="rock", layer="sedimentary", 
        glyph=glyphs['equals'], fg=colors['dark_yellow'], bg=colors['grey'],
        hit_points = 75, mines_to="flint", mines_to_also="stone_boulder"
    },
    chromite = { name="Chromite", type="rock", layer="igneous", 
        glyph=glyphs['equals'], fg=colors['white'], bg=colors['grey'],
        hit_points = 75, mines_to="ore", ore_materials = {"chromium"}
    },
    cinnabar = { name="Cinnabar", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['red'], bg=colors['grey'],
        hit_points = 115, mines_to="mercury_ore" -- TODO: Mercury should be treated differently
    },
    claystone = { name="Claystone", type="rock", layer="sedimentary", 
        glyph=glyphs['comma'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 75, mines_to="stone_boulder"
    },
    cobaltite = { name="Cobaltite", type="rock", layer="igneous", 
        glyph=glyphs['urk'], fg=colors['blue'], bg=colors['grey'],
        hit_points = 115, mines_to="stone_boulder"
    },
    conglomerate = { name="Conglomerate", type="rock", layer="sedimentary", 
        glyph=glyphs['infinity'], fg=colors['dark_yellow'], bg=colors['grey'],
        hit_points = 75, mines_to="stone_boulder"
    },
    copper_rock = { name="Native Copper", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['dark_yellow'], bg=colors['grey'],
        hit_points = 75, mines_to="ore", ore_materials = {"copper"}
    },
    dacite = { name="Dacite", type="rock", layer="igneous", 
        glyph=glyphs['period'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    diorite = { name="Diorite", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    dolomite = { name="Dolomite", type="rock", layer="sedimentary", 
        glyph=glyphs['white'], fg=colors['backtick'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    gabbro = { name="Gabbro", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    galena = { name="Galena", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"lead", "silver"}
    },
    garnierite = { name="Garnierite", type="rock", layer="cluster_rock", parent="gabbro",
        glyph=glyphs['ukp'], fg=colors['green'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"lead", "nickel"}
    },
    gneiss = { name="Gneiss", type="rock", layer="igneous", 
        glyph=glyphs['equals'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    gold_rock = { name="Native Gold", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"gold"}
    },
    granite = { name="Granite", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    graphite = { name="Graphite", type="rock", layer="cluster_rock", parent="gneiss",
        glyph=glyphs['circle'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    gypsum = { name="Gypsum", type="rock", layer="sedimentary", 
        glyph=glyphs['hash'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="gypsum_powder"
    },
    hematite_ig = { name="Hematite", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['dark_red'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"iron"}
    },
    hematite_se = { name="Hematite", type="rock", layer="sedimentary", 
        glyph=glyphs['ukp'], fg=colors['dark_red'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"iron"}
    },
    hornblende = { name="Hornblende", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    jet = { name="Jet", type="rock", layer="sedimentary", 
        glyph=glyphs['one_quarter_solid'], fg=colors['black'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    kaolinite = { name="Kaolinite", type="rock", layer="sedimentary", 
        glyph=glyphs['equals'], fg=colors['dark_red'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    lignite = { name="Bithumous Coal", type="rock", layer="sedimentary", 
        glyph=glyphs['sun'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="coal"
    },
    limestone = { name="Limestone", type="rock", layer="sedimentary", 
        glyph=glyphs['one_quarter_solid'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    limonite = { name="Limonite", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"iron"}
    },
    magnetite = { name="Magnetite", type="rock", layer="igneous", 
        glyph=glyphs['tilde'], fg=colors['dark_red'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"iron"}
    },
    malachite = { name="Malachite", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['dark_green'], bg=colors['grey'],
        hit_points = 75, mines_to="ore", ore_materials = {"copper"}
    },
    marble = { name="Marble", type="rock", layer="sedimentary", 
        glyph=glyphs['one_quarter_solid'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    microcline = { name="Microcline", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['cyan'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    mudstone = { name="Marble", type="rock", layer="sedimentary", 
        glyph=glyphs['double_wave'], fg=colors['dark_yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    obsidian = { name="Marble", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['black'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder", damage_bonus=1
    },
    olivine = { name="Olivine", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['dark_green'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    orpiment = { name="Marble", type="rock", layer="igneous", 
        glyph=glyphs['yellow'], fg=colors['tilde'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    orthoclase = { name="Orthoclase", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    phylite = { name="Phylite", type="rock", layer="sedimentary", 
        glyph=glyphs['minus'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    pitchblende = { name="Pitchblende", type="rock", layer="igneous", 
        glyph=glyphs['star'], fg=colors['purple'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"uranium"}
    },
    platinum_rock = { name="Native Platinum", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"platinum"}
    },
    pyrolusite = { name="Pyrolusite", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    quartzite = { name="Quartzite", type="rock", layer="igneous", 
        glyph=glyphs['minus'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    realgar = { name="Realgar", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['red'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    rhyolite = { name="Rhyolite", type="rock", layer="igneous", 
        glyph=glyphs['comma'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    rock_salt = { name="Rock Salt", type="rock", layer="sedimentary", 
        glyph=glyphs['hash'], fg=colors['white'], bg=colors['grey'],
        hit_points = 50, mines_to="stone_boulder"
    },
    rutile = { name="Rutile", type="rock", layer="igneous", 
        glyph=glyphs['backtick'], fg=colors['purple'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"uranium"}
    },
    saltpeter = { name="Saltpeter", type="rock", layer="sedimentary", 
        glyph=glyphs['hash'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 50, mines_to="saltpeter_ore"
    },
    sandstone = { name="Sandstone", type="rock", layer="sedimentary", 
        glyph=glyphs['hash'], fg=colors['dark_yellow'], bg=colors['grey'],
        hit_points = 50, mines_to="sand"
    },
    serpentine = { name="Serpentine", type="rock", layer="igneous", 
        glyph=glyphs['double_wave'], fg=colors['dark_green'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    shale = { name="Shale", type="rock", layer="sedimentary", 
        glyph=glyphs['period'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 50, mines_to="oil"
    },
    siltstone = { name="Shale", type="rock", layer="sedimentary", 
        glyph=glyphs['percent'], fg=colors['dark_yellow'], bg=colors['grey'],
        hit_points = 50, mines_to="oil"
    },
    silver_rock = { name="Native Silver", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"silver"}
    },
    sphalerite_rock = { name="Sphalerite", type="rock", layer="sedimentary", 
        glyph=glyphs['ukp'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"zinc"}
    },
    stibnite = { name="Stibnite", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="stone_boulder"
    },
    tetrahedrite = { name="Tetrahedrite", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="ore", ore_materials = {"copper", "silver"}
    },

    clay_soil = { name="Clay", type="soil",
        glyph = glyphs['solid_square'], fg=colors['dark_red'], bg=colors['black'],
        hit_points = 20, mines_to="clay"
    },
    clay_loam = { name="Clay Loam", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['dark_red'], bg=colors['grey'],
        hit_points = 20, mines_to="clay"
    },
    loam = { name="Loam", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 15, mines_to="topsoil"
    },
    loamy_sand = { name="Loamy Sand", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 15, mines_to="topsoil"
    },
    peat = { name="Peat", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['dark_yellow'], bg=colors['grey'],
        hit_points = 15, mines_to="topsoil"
    },
    sandy_clay = { name="Sandy Clay", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['wood_brown'], bg=colors['dark_red'],
        hit_points = 15, mines_to="clay"
    },
    sandy_clay_loam = { name="Sandy Clay Loam", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['dark_red'], bg=colors['dark_yellow'],
        hit_points = 15, mines_to="topsoil", mines_to_also="clay"
    },
    silt = { name="Silt", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['dark_yellow'], bg=colors['black'],
        hit_points = 15, mines_to="topsoil"
    },
    silty_clay = { name="Silty Clay", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['dark_red'], bg=colors['black'],
        hit_points = 15, mines_to="clay"
    },
    silty_clay_loam = { name="Silty Clay Loam", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['dark_red'], bg=colors['grey'],
        hit_points = 15, mines_to="topsoil"
    },
    silt_loam = { name="Silt Loam", type="soil",
        glyph = glyphs['one_quarter_solid'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 15, mines_to="topsoil"
    },
    sand = { name="Sand", type="sand",
        glyph = glyphs['one_quarter_solid'], fg=colors['yellow'], bg=colors['red'],
        hit_points = 10, mines_to="sand"
    },
    black_sand = { name="Black Sand", type="sand",
        glyph = glyphs['one_quarter_solid'], fg=colors['grey'], bg=colors['black'],
        hit_points = 10, mines_to="sand"
    },
    red_sand = { name="Red Sand", type="sand",
        glyph = glyphs['one_quarter_solid'], fg=colors['red_sand'], bg=colors['black'],
        hit_points = 10, mines_to="sand"
    },
    white_sand = { name="Sand", type="sand",
        glyph = glyphs['one_quarter_solid'], fg=colors['white_sand'], bg=colors['black'],
        hit_points = 10, mines_to="sand"
    },
    yellow_sand = { name="Yellow Sand", type="sand",
        glyph = glyphs['one_quarter_solid'], fg=colors['yellow_sand'], bg=colors['black'],
        hit_points = 10, mines_to="sand"
    },

-- Other materials needed
    wood = { name = "Wood", type="synthetic", 
        glyph = glyphs['one_quarter_solid'], fg=colors['wood_brown'], bg=colors['black'],
        hit_points = 20
    },
    organic = { name = "Organic", type="organic", 
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
        hit_points = 200
    },

-- Metals
    aluminium = { name = "Aluminium", type="metal", 
        glyph = glyphs['one_quarter_solid'], fg=colors['aluminium_blue'], bg=colors['black'],
        hit_points = 200, damage_bonus=-1, ac_bonus=0.5
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