
humanoid_parts = {
    head = { qty=1, size = 10 },
    neck = { qty=1, size = 1 },
    torso = { qty = 1, size = 50 }, 
    left_arm = { qty=1, size = 10 },
    left_hand = { qty=1, size = 2 },
    right_arm = { qty=1, size = 10 },
    right_hand = { qty=1, size = 2 }, 
    left_leg = { qty=1, size = 10 },
    left_foot = { qty=1, size = 2 },
    right_leg = { qty=1, size = 10 },
    right_foot = { qty=1, size = 2 } 
}

-- This is a prototype of what the new species system should look like.
civilized_species = {
    emmet = {
        name = "Emmet", male_name = "Emmet", female_name = "Emmet", group_name = "Emmet", baby_name = "Hatchling",
        child_name = "Young",
        description = "An ant-like creature of nightmares. It eats everything, and spreads blight.",
        stat_mods = {con=2,str=2,int=-2,wis=-2},
        parts = humanoid_parts,
        ethics = { diet="omnivore", mentality="hive", blight=true, behavior="eat_world" },
        max_age = 20, infant_age=1, child_age=2,
        glyph = glyphs['ant'],
        breed_type = "hatch",
        clutch_size_min = 100,
        clutch_size_max = 200,
        clutch_frequency = 1,
        gains_tech_by_eating = true,
        lisp = true,
        never_negotiates = true,

        castes = {
            queen = { max_per_region=1, min_per_occupied_region=1, starting_level = 10, name = "Hive Queen",
                fertile=true,
                berserk=false,
                combat = {
                    armor_class = 20,
                    attacks = {
                        bite1 = { type="bite", hit_bonus=8, n_dice=2, die_type=20, die_mod=4 },
                        sting1 = { type="sting", hit_bonus=8, n_dice=2, die_type=20, die_mod=4 }
                     },
                    guard_only = true
                },
                hp_n = 10, hp_dice = 6, hp_mod = 1
            },
            prince = { max_per_region=16, min_per_occupied_region=1, starting_level = 10, name = "Hive Prince",
                fertile=true,
                berserk=false,
                combat = {
                    armor_class = 18,
                    attacks = {
                        bite1 = { type="bite", hit_bonus=6, n_dice=1, die_type=20, die_mod=4 },
                        sting1 = { type="sting", hit_bonus=6, n_dice=1, die_type=20, die_mod=4 }
                    },
                    guard_only = true
                },
                hp_n = 10, hp_dice = 6, hp_mod = 1
            },
            soldier = { probability=1, starting_level = 10, name = "Hive Soldier",
                fertile = false,
                berserk=true,
                combat = {
                    armor_class = 16,
                    attacks = {
                        bite1 = { type="bite", hit_bonus=2, n_dice=1, die_type=8, die_mod=4 },
                        sting1 = { type="sting", hit_bonus=2, n_dice=1, die_type=8, die_mod=4 }
                    }
                },
                hp_n = 10, hp_dice = 6, hp_mod = 1
            },
            worker = { probability=3, starting_level = 10, name = "Hive Worker",
                fertile = false,
                berserk=false,
                combat = {
                    armor_class = 16,
                    attacks = {
                        bite1 = { type="bite", hit_bonus=0, n_dice=1, die_type=4, die_mod=4 },
                        sting1 = { type="sting", hit_bonus=0, n_dice=1, die_type=4, die_mod=4 }
                    }
                },
                spreads_blight = true,
                destroys_everything = true,
                hp_n = 10, hp_dice = 6, hp_mod = 1
            },
        }
    },
}

species_sentient = {
    human = {
        name = "Human", male_name = "Man", female_name = "Woman", group_name = "People",
        description = "A bipedal ape-descendant with an unhealthy obsession with money and power",
        stat_mods = {}, -- Humans have no modifiers
        parts = humanoid_parts,
        ethics = { diet="omnivore", alignment="neutral" },
        max_age = 114, infant_age=3, child_age=16,
        glyph = glyphs['at']
    }
}