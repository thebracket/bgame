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
        hunting_yield = { meat=4, hide=4 },
        ai = "grazer",
        glyph = glyphs['d'],
        hp_n = 1, hp_dice = 10, hp_mod = 0
    }
}
