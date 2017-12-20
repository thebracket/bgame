-- Scary ant-people who just want to eat the world

species_sentient['emmet3'] = {
    name = "Fire Emmet", male_name = "Drone", female_name = "Queen", group_name = "Emmet",
    description = "An ant-like creature of nightmares. It eats everything, and spreads blight. It spits fire.",
    stat_mods = { con = 3, str = 3, int = -2, wis = -2 },
    parts = humanoid_parts,
    ethics = { diet = "omnivore", blight = true, alignment="devour" },
    max_age = 20, infant_age = 1, child_age = 2,
    glyph = glyphs['ant_fire'], ascii_glyph = glyphs['a'],
    worldgen_glyph = glyphs['ant_fire'],
    composite_render=false, vox = voxelId("ant")
}

emmet_queen3 = {
    n = 1, name = "Fire Emmet Queen", level=11,
    armor_class = 24,
    attacks = {
        bite1 = { type = "bite", hit_bonus = 8, n_dice = 3, die_type = 20, die_mod = 4 },
        sting1 = { type = "sting", hit_bonus = 8, n_dice = 3, die_type = 20, die_mod = 4 },
        spit = { type = "spit", hit_bonus = 8, n_dice = 3, die_type = 20, die_mod = 4, range = 8 }
    },
    hp_n = 10, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_princess3 = {
    n = 6, name = "Fire Emmet Princess", level=7,
    armor_class = 21,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 6, n_dice = 3, die_type = 12, die_mod = 2 },
        sting1 = { type = "sting", hit_bonus = 6, n_dice = 3, die_type = 12, die_mod = 2 },
        spit = { type = "spit", hit_bonus = 6, n_dice = 3, die_type = 12, die_mod = 2, range=8 }
    },
    hp_n = 10, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_soldier3 = {
    n = 25, name = "Fire Emmet Soldier", level=2,
    armor_class = 16,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0 },
        sting1 = { type = "sting", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0 },
        spit = { type = "spit", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0, range=8 }
    },
    hp_n = 2, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_worker3 = {
    n = 25, name = "Fire Emmet Worker", level=1,
    armor_class = 16,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 2, n_dice = 1, die_type = 4, die_mod = 0 }
    },
    hp_n = 2, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

civilizations['emmet3'] = {
    tech_level = 3,
    species_def = 'emmet3',
    ai = 'worldeater',
    name_generator = "ant3",
    can_build = { "ant_mound", "ant_tunnel" },
    units = {
        garrison = {
            bp_per_turn = 1,
            speed = 0,
            name = "Emmet Guardians",
            sentients = {
                queen = emmet_queen3,
                princess = emmet_princess3,
                soldier = emmet_soldier3,
                worker = emmet_worker3
            },
            worldgen_strength = 10
        },
        swarm = {
            bp_per_turn = 0,
            speed = 1,
            name = "Emmet Swarm",
            sentients = {
                soldier = emmet_soldier3,
                worker = emmet_worker3
            },
            worldgen_strength = 20
        }
    },
    evolves_into = { "emmet4" }
}

function civ_name_gen_ant3(n)
    n = n % 6;
    if n == 0 then return "Fire Emmet Swarm";
    elseif n == 1 then return "Fire Emmet Hive";
    elseif n == 2 then return "Fire Emmet Nest";
    elseif n == 3 then return "Fire Emmet Mound";
    elseif n == 4 then return "Fire Emmet Plague";
    elseif n == 5 then return "Fire Emmet Monarchy";
    else return "Fire Emmet Infestation";
    end
end

function leader_name_gen_ant3(n)
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