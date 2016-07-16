materials = {
    alabaster = { name="Alabaster", type="cluster_rock", parent="gypsum", 
        glyph=glyphs['power_of'], fg=colors['white'], bg=colors['grey'],
        hit_points = 110, mines_to="alabaster_powder" 
    },
    aluminium_rock = { name="Raw Aluminium", type="rock", layer="igneous", 
        glyph=glyphs['power_of'], fg=colors['white'], bg=colors['grey'],
        hit_points = 80, mines_to="aluminium_ore"
    },
    alunite = { name="Alunite", type="rock", layer="igneous", 
        glyph=glyphs['backtick'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    andesite = { name="Andesite", type="rock", layer="igneous", 
        glyph=glyphs['infinity'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    basalt = { name="Basalt", type="rock", layer="igneous", 
        glyph=glyphs['hash'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    bauxite = { name="Bauxite", type="rock", layer="sedimentary", 
        glyph=glyphs['plus'], fg=colors['dark_red'], bg=colors['grey'],
        hit_points = 100, mines_to="aluminium_ore"
    },
    bismunthine = { name="Bisminthine", type="rock", layer="sedimentary", 
        glyph=glyphs['percent'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="bismuth_ore"
    },
    bithumous_coal = { name="Bithumous Coal", type="rock", layer="sedimentary", 
        glyph=glyphs['sun'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="coal"
    },
    brimstone = { name="Bithumous Coal", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="sulphur"
    },
    cassiterite = { name="Cassiterite", type="rock", layer="sedimentary", 
        glyph=glyphs['ukp'], fg=colors['dark_yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="tin_ore"
    },
    chalk = { name="Chalk", type="rock", layer="sedimentary", 
        glyph=glyphs['one_quarter_solid'], fg=colors['white'], bg=colors['grey'],
        hit_points = 75, mines_to="boulder"
    },
    chert = { name="Chert", type="rock", layer="sedimentary", 
        glyph=glyphs['equals'], fg=colors['dark_yellow'], bg=colors['grey'],
        hit_points = 75, mines_to="flint"
    },
    chromite = { name="Chromite", type="rock", layer="igneous", 
        glyph=glyphs['equals'], fg=colors['white'], bg=colors['grey'],
        hit_points = 75, mines_to="chromium_ore"
    },
    cinnabar = { name="Cinnabar", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['red'], bg=colors['grey'],
        hit_points = 115, mines_to="mercury_ore"
    },
    claystone = { name="Claystone", type="rock", layer="sedimentary", 
        glyph=glyphs['comma'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 75, mines_to="boulder"
    },
    cobaltite = { name="Cobaltite", type="rock", layer="igneous", 
        glyph=glyphs['urk'], fg=colors['blue'], bg=colors['grey'],
        hit_points = 115, mines_to="boulder"
    },
    conglomerate = { name="Conglomerate", type="rock", layer="sedimentary", 
        glyph=glyphs['infinity'], fg=colors['dark_yellow'], bg=colors['grey'],
        hit_points = 75, mines_to="boulder"
    },
    copper_rock = { name="Native Copper", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['dark_yellow'], bg=colors['grey'],
        hit_points = 75, mines_to="copper_ore"
    },
    dacite = { name="Dacite", type="rock", layer="igneous", 
        glyph=glyphs['period'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    diorite = { name="Diorite", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    dolomite = { name="Dolomite", type="rock", layer="sedimentary", 
        glyph=glyphs['white'], fg=colors['backtick'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    gabbro = { name="Gabbro", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    galena = { name="Galena", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="lead_ore", mines_to_also="silver_ore"
    },
    garnierite = { name="Garnierite", type="rock", layer="cluster_rock", parent="gabbro" 
        glyph=glyphs['ukp'], fg=colors['green'], bg=colors['grey'],
        hit_points = 100, mines_to="lead", mines_to_also="nickel_ore"
    },
    gneiss = { name="Gabbro", type="rock", layer="igneous", 
        glyph=glyphs['equals'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    gold_rock = { name="Native Gold", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="gold_ore"
    },
    granite = { name="Granite", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    graphite = { name="Gabbro", type="rock", layer="cluster_rock", parent="gneiss" 
        glyph=glyphs['circle'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    gypsum = { name="Gabbro", type="rock", layer="sedimentary", 
        glyph=glyphs['hash'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="gypsum_powder"
    },
    hematite_ig = { name="Hematite", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['dark_red'], bg=colors['grey'],
        hit_points = 100, mines_to="iron_ore"
    },
    hematite_se = { name="Hematite", type="rock", layer="sedimentary", 
        glyph=glyphs['ukp'], fg=colors['dark_red'], bg=colors['grey'],
        hit_points = 100, mines_to="iron_ore"
    },
    hornblende = { name="Hornblende", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    jet = { name="Jet", type="rock", layer="sedimentary", 
        glyph=glyphs['one_quarter_solid'], fg=colors['black'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    kaolinite = { name="Kaolinite", type="rock", layer="sedimentary", 
        glyph=glyphs['equals'], fg=colors['dark_red'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    lignite = { name="Bithumous Coal", type="rock", layer="sedimentary", 
        glyph=glyphs['sun'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="coal"
    },
    limestone = { name="Limestone", type="rock", layer="sedimentary", 
        glyph=glyphs['one_quarter_solid'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    limonite = { name="Limonite", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="iron_ore"
    },
    magnetite = { name="Magnetite", type="rock", layer="igneous", 
        glyph=glyphs['tilde'], fg=colors['dark_red'], bg=colors['grey'],
        hit_points = 100, mines_to="iron_ore"
    },
    malachite = { name="Malachite", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['dark_green'], bg=colors['grey'],
        hit_points = 75, mines_to="copper_ore"
    },
    marble = { name="Marble", type="rock", layer="sedimentary", 
        glyph=glyphs['one_quarter_solid'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    microcline = { name="Microcline", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['cyan'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    mudstone = { name="Marble", type="rock", layer="sedimentary", 
        glyph=glyphs['double_wave'], fg=colors['dark_yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    obsidian = { name="Marble", type="rock", layer="igneous", 
        glyph=glyphs['one_quarter_solid'], fg=colors['black'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    olivine = { name="Olivine", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['dark_green'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    orpiment = { name="Marble", type="rock", layer="igneous", 
        glyph=glyphs['yellow'], fg=colors['tilde'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    orthoclase = { name="Orthoclase", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    phylite = { name="Phylite", type="rock", layer="sedimentary", 
        glyph=glyphs['minus'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    pitchblende = { name="Pitchblende", type="rock", layer="igneous", 
        glyph=glyphs['star'], fg=colors['purple'], bg=colors['grey'],
        hit_points = 100, mines_to="uranium_ore"
    },
    platinum_rock = { name="Native Platinum", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="platinum_ore"
    },
    pyrolusite = { name="Pyrolusite", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    quartzite = { name="Quartzite", type="rock", layer="igneous", 
        glyph=glyphs['minus'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    realgar = { name="Realgar", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['red'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    rhyolite = { name="Rhyolite", type="rock", layer="igneous", 
        glyph=glyphs['comma'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    rock_salt = { name="Rock Salt", type="rock", layer="sedimentary", 
        glyph=glyphs['hash'], fg=colors['white'], bg=colors['grey'],
        hit_points = 50, mines_to="boulder"
    },
    rutile = { name="Rutile", type="rock", layer="igneous", 
        glyph=glyphs['backtick'], fg=colors['purple'], bg=colors['grey'],
        hit_points = 100, mines_to="uranium_ore"
    },
    saltpeter = { name="Saltpeter", type="rock", layer="sedimentary", 
        glyph=glyphs['hash'], fg=colors['yellow'], bg=colors['grey'],
        hit_points = 50, mines_to="saltpeter_ore"
    },
    sandstone = { name="Sandstone", type="rock", layer="sedimentary", 
        glyph=glyphs['hash'], fg=colors['dark_yellow'], bg=colors['grey'],
        hit_points = 50, mines_to="yellow_sand"
    },
    serpentine = { name="Serpentine", type="rock", layer="igneous", 
        glyph=glyphs['double_wave'], fg=colors['dark_green'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
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
        hit_points = 100, mines_to="silver_ore"
    },
    sphalerite_rock = { name="Sphalerite", type="rock", layer="sedimentary", 
        glyph=glyphs['ukp'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="zinc_ore"
    },
    stibnite = { name="Stibnite", type="rock", layer="igneous", 
        glyph=glyphs['percent'], fg=colors['white'], bg=colors['grey'],
        hit_points = 100, mines_to="boulder"
    },
    tetrahedrite = { name="Tetrahedrite", type="rock", layer="igneous", 
        glyph=glyphs['ukp'], fg=colors['dark_grey'], bg=colors['grey'],
        hit_points = 100, mines_to="copper_ore", mines_to_also="silver_ore"
    }
}