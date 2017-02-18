-- Primitive humans, with early settlements and structure
civilized_species['neolithic_human'] = {
    name = "Neolithic Human",
    male_name = "Man",
    female_name = "Woman",
    group_name = "Tribe",
    baby_name = "Baby",
    child_name = "Child",
    description = "A bipedal ape-descendant capable of just about anything",
    stat_mods = {  },
    parts = humanoid_parts,
    ethics = { diet = "omnivore", mentality = "tribal", blight = false, behavior = "tribal" },
    max_age = 70,
    infant_age = 3,
    child_age = 10,
    glyph = glyphs['at'],
    worldgen_glyph = glyphs['caveman'],
    breed_type = "marriage",
    min_guard_settlement = 3,
    tech_level = 0,
    evolves_into = { "bronze_age_human_monarchy" },
    castes = {
        tribal_leader = {
            max_per_region = 2,
            min_per_occupied_region = 2,
            starting_level = 5,
            name = "Tribal Leader",
            fertile = true,
            berserk = false,
            world_block_size=1,
            combat = {
                armor_class = 12,
                attacks = {
                    bite1 = { type = "fists", hit_bonus = 1, n_dice = 1, die_type = 6, die_mod = 0 }
                },
                guard_only = true
            },
            clothing = {
                both = { torso="tunic/hide", shoes="sandals/hide" },
                male = { legs="britches/hide" },
                female = { legs="skirt_simple/hide" }
            },
            weapons = {
                melee = "warhammer/granite"
            },
            hp_n = 2,
            hp_dice = 10,
            hp_mod = 0,
            builds = { longhall = { type = "longhall-wood", max=1} }
        },
        shaman = {
            max_per_region = 5,
            min_per_occupied_region = 1,
            starting_level = 3,
            name = "Tribal Shaman",
            fertile = true,
            berserk = false,
            researcher = true,
            world_block_size=1,
            combat = {
                armor_class = 12,
                attacks = {
                    bite1 = { type = "fists", hit_bonus = 1, n_dice = 1, die_type = 6, die_mod = 0 }
                },
                guard_only = true
            },
            clothing = {
                both = { torso="tunic/hide", shoes="sandals/hide" },
                male = { legs="britches/hide" },
                female = { legs="skirt_simple/hide" }
            },
            weapons = {
                melee = "dagger/granite"
            },
            hp_n = 2,
            hp_dice = 10,
            hp_mod = 0,
            builds = { henge = { type = "henge-wood", max=1} }
        },
        laborer = {
            starting_level = 1,
            name = "Tribal Laborer",
            fertile = true,
            berserk = false,
            combat = {
                armor_class = 12,
                attacks = {
                    bite1 = { type = "fists", hit_bonus = 1, n_dice = 1, die_type = 6, die_mod = 0 }
                }
            },
            clothing = {
                both = { torso="tunic/hide", shoes="sandals/hide" },
                male = { legs="britches/hide" },
                female = { legs="skirt_simple/hide" }
            },
            weapons = {
                melee = "warhammer/granite"
            },
            hp_n = 2,
            hp_dice = 10,
            hp_mod = 0,
            builds = {
                well = { type = "well", max=1},
                hut = { type = "hovel-wood", max=10},
                palisade = { type="wood-palisade", max=1 },
                wood_tower = { type="wood-tower", max=1 },
                earth_works = {type="earthworks", max=1 }
            }
        },
        woodsman = {
            starting_level = 1,
            name = "Tribal Woodsman",
            fertile = true,
            berserk = false,
            combat = {
                armor_class = 12,
                attacks = {
                    bite1 = { type = "fists", hit_bonus = 1, n_dice = 1, die_type = 6, die_mod = 0 }
                }
            },
            clothing = {
                both = { torso="tunic/hide", shoes="sandals/hide" },
                male = { legs="britches/hide" },
                female = { legs="skirt_simple/hide" }
            },
            weapons = {
                melee = "battleaxe/granite"
            },
            hp_n = 2,
            hp_dice = 10,
            hp_mod = 0,
            builds = { hut = { type = "hovel-wood", max=2}, sawmill = { type = "sawmill", max=1 }, carpenter = { type="carpenter", max=1} }
        },
        gatherer = {
            starting_level = 1,
            name = "Tribal Gatherer",
            fertile = true,
            berserk = false,
            combat = {
                armor_class = 12,
                attacks = {
                    bite1 = { type = "fists", hit_bonus = 1, n_dice = 1, die_type = 6, die_mod = 0 }
                }
            },
            clothing = {
                both = { torso="tunic/hide", shoes="sandals/hide" },
                male = { legs="britches/hide" },
                female = { legs="skirt_simple/hide" }
            },
            weapons = {
                melee = "pointy_stick/wood"
            },
            hp_n = 2,
            hp_dice = 10,
            hp_mod = 0
        },
        bow_hunter = {
            starting_level = 1,
            name = "Tribal Bow Hunter",
            fertile = true,
            berserk = false,
            combat = {
                armor_class = 12,
                attacks = {
                    bite1 = { type = "fists", hit_bonus = 1, n_dice = 1, die_type = 6, die_mod = 0 }
                }
            },
            clothing = {
                both = { torso="tunic/hide", shoes="sandals/hide" },
                male = { legs="britches/hide" },
                female = { legs="skirt_simple/hide" }
            },
            weapons = {
                melee = "dagger/granite", ranged = "shortbow/wood", ammo = "arrow/wood"
            },
            hp_n = 2,
            hp_dice = 10,
            hp_mod = 0
        },
        trader = {
            starting_level = 1,
            name = "Tribal Trader",
            fertile = true,
            berserk = false,
            combat = {
                armor_class = 12,
                attacks = {
                    bite1 = { type = "fists", hit_bonus = 1, n_dice = 1, die_type = 6, die_mod = 0 }
                }
            },
            clothing = {
                both = { torso="tunic/hide", shoes="sandals/hide" },
                male = { legs="britches/hide" },
                female = { legs="skirt_simple/hide" }
            },
            weapons = {
                melee = "pointy_stick/wood"
            },
            hp_n = 2,
            hp_dice = 10,
            hp_mod = 0
        },
    }
}

