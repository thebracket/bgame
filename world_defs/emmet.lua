-- Scary ant-people who just want to eat the world

species_sentient['emmet'] = {
    name = "Emmet", male_name = "Drone", female_name = "Queen", group_name = "Emmet",
    description = "An ant-like creature of nightmares. It eats everything, and spreads blight.",
    stat_mods = { con = 2, str = 2, int = -2, wis = -2 },
    parts = humanoid_parts,
    ethics = { diet = "omnivore", blight = true, alignment="devour" },
    max_age = 20, infant_age = 1, child_age = 2,
    glyph = glyphs['ant'], ascii_glyph = glyphs['a'],
    worldgen_glyph = glyphs['ant'],
    composite_render=false, vox = voxelId("ant")
}

emmet_queen = {
    n = 1, name = "Emmet Queen", level=10,
    armor_class = 20,
    attacks = {
        bite1 = { type = "bite", hit_bonus = 8, n_dice = 2, die_type = 20, die_mod = 4 },
        sting1 = { type = "sting", hit_bonus = 8, n_dice = 2, die_type = 20, die_mod = 4 }
    },
    hp_n = 10, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_princess = {
    n = 6, name = "Emmet Princess", level=7,
    armor_class = 17,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 6, n_dice = 2, die_type = 12, die_mod = 2 },
        sting1 = { type = "sting", hit_bonus = 6, n_dice = 2, die_type = 12, die_mod = 2 }
    },
    hp_n = 10, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_soldier = {
    n = 25, name = "Emmet Soldier", level=2,
    armor_class = 14,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0 },
        sting1 = { type = "sting", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0 }
    },
    hp_n = 2, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_worker = {
    n = 25, name = "Emmet Worker", level=1,
    armor_class = 12,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 2, n_dice = 1, die_type = 4, die_mod = 0 }
    },
    hp_n = 2, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

civilizations['emmet'] = {
    tech_level = 0,
    species_def = 'emmet',
    ai = 'worldeater',
    name_generator = "ant",
    can_build = { "ant_mound", "ant_tunnel" },
    units = {
        garrison = {
            bp_per_turn = 1,
            speed = 0,
            name = "Emmet Guardians",
            sentients = {
                queen = emmet_queen,
                princess = emmet_princess,
                soldier = emmet_soldier,
                worker = emmet_worker
            },
            worldgen_strength = 2
        },
        swarm = {
            bp_per_turn = 0,
            speed = 1,
            name = "Emmet Swarm",
            sentients = {
                soldier = emmet_soldier,
                worker = emmet_worker
            },
            worldgen_strength = 2
        }
    },
    evolves_into = { "emmet1" }
}

function civ_name_gen_ant(n)
    n = n % 6;
    if n == 0 then return "Emmet Swarm";
    elseif n == 1 then return "Emmet Hive";
    elseif n == 2 then return "Emmet Nest";
    elseif n == 3 then return "Emmet Mound";
    elseif n == 4 then return "Emmet Plague";
    elseif n == 5 then return "Emmet Monarchy";
    else return "Emmet Infestation";
    end
end

function leader_name_gen_ant(n)
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