-- Scary ant-people who just want to eat the world

species_sentient['emmet2'] = {
    name = "Iron-Lined Emmet", male_name = "Drone", female_name = "Queen", group_name = "Emmet",
    description = "An ant-like creature of nightmares. It eats everything, and spreads blight. It was worked iron armor into its chitin.",
    stat_mods = { con = 3, str = 3, int = -2, wis = -2 },
    parts = humanoid_parts,
    ethics = { diet = "omnivore", blight = true, alignment="devour" },
    max_age = 20, infant_age = 1, child_age = 2,
    glyph = glyphs['ant_iron'], ascii_glyph = glyphs['a'],
    worldgen_glyph = glyphs['ant_iron'],
    composite_render=false, vox = voxelId("ant")
}

emmet_queen2 = {
    n = 1, name = "Iron Emmet Queen", level=11,
    armor_class = 24,
    attacks = {
        bite1 = { type = "bite", hit_bonus = 8, n_dice = 3, die_type = 20, die_mod = 4 },
        sting1 = { type = "sting", hit_bonus = 8, n_dice = 3, die_type = 20, die_mod = 4 }
    },
    hp_n = 10, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_princess2 = {
    n = 6, name = "Iron Emmet Princess", level=7,
    armor_class = 21,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 6, n_dice = 3, die_type = 12, die_mod = 2 },
        sting1 = { type = "sting", hit_bonus = 6, n_dice = 3, die_type = 12, die_mod = 2 }
    },
    hp_n = 10, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_soldier2 = {
    n = 25, name = "Iron Emmet Soldier", level=2,
    armor_class = 16,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0 },
        sting1 = { type = "sting", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0 }
    },
    hp_n = 2, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_worker2 = {
    n = 25, name = "Iron Emmet Worker", level=1,
    armor_class = 16,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 2, n_dice = 1, die_type = 4, die_mod = 0 }
    },
    hp_n = 2, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

civilizations['emmet2'] = {
    tech_level = 2,
    species_def = 'emmet2',
    ai = 'worldeater',
    name_generator = "ant2",
    can_build = { "ant_mound", "ant_tunnel" },
    units = {
        garrison = {
            bp_per_turn = 1,
            speed = 0,
            name = "Emmet Guardians",
            sentients = {
                queen = emmet_queen2,
                princess = emmet_princess2,
                soldier = emmet_soldier2,
                worker = emmet_worker2
            },
            worldgen_strength = 5
        },
        swarm = {
            bp_per_turn = 0,
            speed = 1,
            name = "Emmet Swarm",
            sentients = {
                soldier = emmet_soldier2,
                worker = emmet_worker2
            },
            worldgen_strength = 10
        }
    },
    evolves_into = { "emmet3" }
}

function civ_name_gen_ant2(n)
    n = n % 6;
    if n == 0 then return "Iron Emmet Swarm";
    elseif n == 1 then return "Iron Emmet Hive";
    elseif n == 2 then return "Iron Emmet Nest";
    elseif n == 3 then return "Iron Emmet Mound";
    elseif n == 4 then return "Iron Emmet Plague";
    elseif n == 5 then return "Iron Emmet Monarchy";
    else return "Iron Emmet Infestation";
    end
end

function leader_name_gen_ant2(n)
    n = n % 8;
    if n == 0 then return "Queen Click-Click";
    elseif n == 1 then return "Queen Devourer";
    elseif n == 2 then return "Queen Gut-muncher";
    elseif n == 3 then return "Queen Butcher";
    elseif n == 4 then return "Queen Marrow-Gnawer";
    elseif n == 5 then return "Queen Bone-cruncher";
    elseif n == 6 then return "Queen Hatred";
    elseif n == 7 then return "Queen Murder";
    elseif n == 8 then return "Queen Pillage";
    elseif n == 9 then return "Queen Intestinal Fungus";
    elseif n == 10 then return "Queen of the Blight";
    else return "Queen Slaughter";
    end
end