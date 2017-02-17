-- Scary ant-people who just want to eat the world
civilized_species['emmet'] = {
    name = "Emmet",
    male_name = "Emmet",
    female_name = "Emmet",
    group_name = "Emmet",
    baby_name = "Hatchling",
    child_name = "Young",
    description = "An ant-like creature of nightmares. It eats everything, and spreads blight.",
    stat_mods = { con = 2, str = 2, int = -2, wis = -2 },
    parts = humanoid_parts,
    ethics = { diet = "omnivore", mentality = "hive", blight = true, behavior = "eat_world" },
    max_age = 20,
    infant_age = 1,
    child_age = 1,
    glyph = glyphs['ant'],
    breed_type = "hatch",
    clutch_size_min = 10,
    clutch_size_max = 50,
    clutch_frequency = 1,
    gains_tech_by_eating = true,
    lisp = true,
    never_negotiates = true,
    tech_level = 0,
    evolves_into = {},
    castes = {
        queen = {
            max_per_region = 1,
            min_per_occupied_region = 1,
            starting_level = 10,
            name = "Hive Queen",
            fertile = true,
            berserk = false,
            combat = {
                armor_class = 20,
                attacks = {
                    bite1 = { type = "bite", hit_bonus = 8, n_dice = 2, die_type = 20, die_mod = 4 },
                    sting1 = { type = "sting", hit_bonus = 8, n_dice = 2, die_type = 20, die_mod = 4 }
                },
                guard_only = true
            },
            hp_n = 10,
            hp_dice = 6,
            hp_mod = 1
        },
        prince = {
            max_per_region = 16,
            min_per_occupied_region = 1,
            starting_level = 10,
            name = "Hive Prince",
            fertile = true,
            berserk = false,
            combat = {
                armor_class = 18,
                attacks = {
                    bite1 = { type = "bite", hit_bonus = 6, n_dice = 1, die_type = 20, die_mod = 4 },
                    sting1 = { type = "sting", hit_bonus = 6, n_dice = 1, die_type = 20, die_mod = 4 }
                },
                guard_only = true
            },
            hp_n = 10,
            hp_dice = 6,
            hp_mod = 1
        },
        soldier = {
            probability = 1,
            starting_level = 1,
            name = "Hive Soldier",
            fertile = false,
            berserk = true,
            combat = {
                armor_class = 16,
                attacks = {
                    bite1 = { type = "bite", hit_bonus = 2, n_dice = 1, die_type = 8, die_mod = 4 },
                    sting1 = { type = "sting", hit_bonus = 2, n_dice = 1, die_type = 8, die_mod = 4 }
                }
            },
            hp_n = 10,
            hp_dice = 6,
            hp_mod = 1
        },
        worker = {
            probability = 3,
            starting_level = 1,
            name = "Hive Worker",
            fertile = false,
            berserk = false,
            combat = {
                armor_class = 16,
                attacks = {
                    bite1 = { type = "bite", hit_bonus = 0, n_dice = 1, die_type = 4, die_mod = 4 },
                    sting1 = { type = "sting", hit_bonus = 0, n_dice = 1, die_type = 4, die_mod = 4 }
                }
            },
            spreads_blight = true,
            destroys_everything = true,
            hp_n = 10,
            hp_dice = 6,
            hp_mod = 1,
            builds = { mound = { type = "ant_mound", max=1} }
        },
    }
}

