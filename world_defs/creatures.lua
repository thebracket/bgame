creatures = {
    deer = {
        name = "Deer", male_name = "Buck", female_name = "Doe", group_name = "Deer",
        description = "A sleek quadruped that primarily grazes on brush and trees.",
        stats = { str=11, dex=16, con=11, int=2, wis=14, cha=5, hp=8 },
        parts = { 
            head = { qty=1, size = 15 }, 
            torso = { qty = 1, size = 50 }, 
            legs = { qty=4, size = 10 } 
        },
        combat = {
            armor_class = 13,
            attacks = { bite1 = { type="bite", hit_bonus=2, n_dice=1, die_type=4, die_mod=0 } }
        },
        hunting_yield = { meat=4, hide=4, bone=2, skull=1 },
        ai = "grazer",
        glyph = glyphs['deer'], color=colors['wood_brown'],
        glyph_ascii = glyphs['d'],
        hp_n = 1, hp_dice = 10, hp_mod = 0,
        group_size_n_dice = 1, group_size_dice = 4, group_size_mod = 0,
        vox = voxelId("deer")
    },
    horse = {
        name = "Horse", male_name = "Stallion", female_name = "Mare", group_name = "Horses",
        description = "A sleek quadruped, often used for riding",
        stats = { str=16, dex=14, con=17, int=2, wis=13, cha=7, hp=15 },
        parts = {
            head = { qty=1, size = 15 },
            torso = { qty = 1, size = 50 },
            legs = { qty=4, size = 10 }
        },
        combat = {
            armor_class = 11,
            attacks = {
                bite1 = { type="bite", hit_bonus=2, n_dice=1, die_type=4, die_mod=0 },
                kick1 = { type="kick", hit_bonus=2, n_dice=1, die_type=4, die_mod=1 },
            }
        },
        hunting_yield = { meat=4, hide=4, bone=2, skull=1 },
        ai = "grazer",
        glyph = glyphs['H'], color=colors['wood_brown'],
        glyph_ascii = glyphs['d'],
        hp_n = 1, hp_dice = 10, hp_mod = 0,
        group_size_n_dice = 1, group_size_dice = 4, group_size_mod = 0,
        vox = voxelId("horse")
    },
    armadillo = {
        name = "Armadillo", male_name = "Male", female_name = "Female", group_name = "Armadillos",
        description = "A squat-bodied mammal with a distinctive leathery hide.",
        stats = { str=4, dex=15, con=11, int=2, wis=12, cha=9, hp=4 },
        parts = { 
            head = { qty=1, size = 15 }, 
            torso = { qty = 1, size = 50 }, 
            legs = { qty=4, size = 10 } 
        },
        combat = {
            armor_class = 16,
            attacks = { bite1 = { type="bite", hit_bonus=0, n_dice=1, die_type=2, die_mod=0 } }
        },
        hunting_yield = { meat=2, hide=2, bone=1, skull=1 },
        ai = "grazer",
        glyph = glyphs['armadillo'], color=colors['wood_brown'],
        glyph_ascii = glyphs['a'],
        hp_n = 1, hp_dice = 4, hp_mod = 0,
        group_size_n_dice = 1, group_size_dice = 8, group_size_mod = 0,
        vox = voxelId("armadillo")
    },
    badger = {
        name = "Badger", male_name = "Male", female_name = "Female", group_name = "Badgers",
        description = "A black and white foraging mammal.",
        stats = { str=10, dex=13, con=15, int=2, wis=12, cha=6, hp=9 },
        parts = { 
            head = { qty=1, size = 15 }, 
            torso = { qty = 1, size = 50 }, 
            legs = { qty=4, size = 10 } 
        },
        combat = {
            armor_class = 13,
            attacks = { bite1 = { type="bite", hit_bonus=0, n_dice=1, die_type=6, die_mod=0 } }
        },
        hunting_yield = { meat=1, hide=1, bone=1, skull=1 },
        ai = "grazer",
        glyph = glyphs['badger'], color=colors['wood_brown'],
        glyph_ascii = glyphs['b'],
        hp_n = 1, hp_dice = 8, hp_mod = 5,
        group_size_n_dice = 1, group_size_dice = 8, group_size_mod = 0,
        vox = voxelId("badger")
    },
    boar = {
        name = "Boar", male_name = "Male", female_name = "Female", group_name = "Boars",
        description = "An angry wild pig with the evolutionary downside of being delicious.",
        stats = { str=17, dex=10, con=17, int=2, wis=13, cha=4, hp=18 },
        parts = { 
            head = { qty=1, size = 15 }, 
            torso = { qty = 1, size = 50 }, 
            legs = { qty=4, size = 10 } 
        },
        combat = {
            armor_class = 14,
            attacks = { bite1 = { type="gore", hit_bonus=0, n_dice=1, die_type=8, die_mod=4 } }
        },
        hunting_yield = { meat=4, hide=4, bone=4, skull=1 },
        ai = "grazer",
        glyph = glyphs['boar'], color=colors['wood_brown'],
        glyph_ascii = glyphs['b'],
        hp_n = 1, hp_dice = 8, hp_mod = 5,
        group_size_n_dice = 1, group_size_dice = 8, group_size_mod = 0
    },
    elephant = {
        name = "Elephant", male_name = "Bull", female_name = "Female", group_name = "Elephants",
        description = "Enormous mammals with tusks and a trunk.",
        stats = { str=30, dex=10, con=19, int=2, wis=13, cha=7, hp=93 },
        parts = { 
            head = { qty=1, size = 15 },
            trunk = { qty=1, size=10 },
            torso = { qty = 1, size = 50 }, 
            legs = { qty=4, size = 10 } 
        },
        combat = {
            armor_class = 17,
            attacks = { bite1 = { type="gore", hit_bonus=8, n_dice=2, die_type=8, die_mod=10 } }
        },
        hunting_yield = { meat=16, hide=16, bone=8, skull=1 },
        ai = "grazer",
        glyph = glyphs['E'], color=colors['wood_brown'],
        glyph_ascii = glyphs['E'],
        hp_n = 13, hp_dice = 8, hp_mod = 78,
        group_size_n_dice = 1, group_size_dice = 4, group_size_mod = 0
    },
    mammoth = {
        name = "Mammoth", male_name = "Bull", female_name = "Female", group_name = "Elephants",
        description = "Enormous hairy mammals with tusks and a trunk.",
        stats = { str=30, dex=10, con=19, int=2, wis=13, cha=7, hp=93 },
        parts = { 
            head = { qty=1, size = 15 },
            trunk = { qty=1, size=10 },
            torso = { qty = 1, size = 50 }, 
            legs = { qty=4, size = 10 } 
        },
        combat = {
            armor_class = 17,
            attacks = { bite1 = { type="gore", hit_bonus=8, n_dice=2, die_type=8, die_mod=10 } }
        },
        hunting_yield = { meat=16, hide=16, bone=8, skull=1 },
        ai = "grazer",
        glyph = glyphs['E'], color=colors['wood_brown'],
        glyph_ascii = glyphs['E'],
        hp_n = 13, hp_dice = 8, hp_mod = 78,
        group_size_n_dice = 1, group_size_dice = 4, group_size_mod = 0
    },
    hedgehog = {
        name = "Hedgehog", male_name = "Male", female_name = "Female", group_name = "Boars",
        description = "A dimunitive, spiny mammal with a cute nose.",
        stats = { str=1, dex=16, con=6, int=2, wis=12, cha=7, hp=2 },
        parts = { 
            head = { qty=1, size = 15 }, 
            torso = { qty = 1, size = 50 }, 
            legs = { qty=4, size = 10 } 
        },
        combat = {
            armor_class = 18,
            attacks = { bite1 = { type="gore", hit_bonus=0, n_dice=1, die_type=8, die_mod=0 } }
        },
        hunting_yield = { meat=4, hide=4, bone=4, skull=1 },
        ai = "grazer",
        glyph = glyphs['h'], color=colors['wood_brown'],
        glyph_ascii = glyphs['h'],
        hp_n = 1, hp_dice = 4, hp_mod = 0,
        group_size_n_dice = 1, group_size_dice = 8, group_size_mod = 0
    },
    antelope = {
        name = "Antelope", male_name = "Buck", female_name = "Doe", group_name = "Deer",
        description = "A graceful, horned mammal.",
        stats = { str=10, dex=17, con=14, int=2, wis=13, cha=7, hp=8 },
        parts = { 
            head = { qty=1, size = 15 }, 
            torso = { qty = 1, size = 50 }, 
            legs = { qty=4, size = 10 } 
        },
        combat = {
            armor_class = 14,
            attacks = { bite1 = { type="bite", hit_bonus=0, n_dice=1, die_type=4, die_mod=1 } }
        },
        hunting_yield = { meat=4, hide=4, bone=2, skull=1 },
        ai = "grazer",
        glyph = glyphs['d'], color=colors['wood_brown'],
        glyph_ascii = glyphs['d'],
        hp_n = 1, hp_dice = 8, hp_mod = 2,
        group_size_n_dice = 1, group_size_dice = 4, group_size_mod = 0,
        vox = voxelId("deer")
    }
}
