-- Scary ant-people who just want to eat the world

species_sentient['emmet4'] = {
    name = "Dark Emmet", male_name = "Drone", female_name = "Queen", group_name = "Emmet",
    description = "An ant-like creature of nightmares. It eats everything, spreads blight, spits fire and is evolving into a fully sentient race.",
    stat_mods = { con = 4, str = 4, int = -2, wis = -2 },
    parts = humanoid_parts,
    ethics = { diet = "omnivore", blight = true, alignment="devour" },
    max_age = 20, infant_age = 1, child_age = 2,
    glyph = glyphs['ant_dark'], ascii_glyph = glyphs['a'],
    worldgen_glyph = glyphs['ant_dark'],
    composite_render=false, vox = voxelId("ant")
}

emmet_queen4 = {
    n = 1, name = "Dark Emmet Queen", level=11,
    armor_class = 25,
    attacks = {
        bite1 = { type = "bite", hit_bonus = 8, n_dice = 3, die_type = 20, die_mod = 4 },
        sting1 = { type = "sting", hit_bonus = 8, n_dice = 3, die_type = 20, die_mod = 4 },
        spit = { type = "spit", hit_bonus = 8, n_dice = 3, die_type = 20, die_mod = 4, range = 8 }
    },
    hp_n = 10, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_princess4 = {
    n = 6, name = "Dark Emmet Princess", level=7,
    armor_class = 22,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 6, n_dice = 3, die_type = 12, die_mod = 2 },
        sting1 = { type = "sting", hit_bonus = 6, n_dice = 3, die_type = 12, die_mod = 2 },
        spit = { type = "spit", hit_bonus = 6, n_dice = 3, die_type = 12, die_mod = 2, range=8 }
    },
    hp_n = 10, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_soldier4 = {
    n = 25, name = "Dark Emmet Soldier", level=2,
    armor_class = 17,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0 },
        sting1 = { type = "sting", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0 },
        spit = { type = "spit", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0, range=8 }
    },
    hp_n = 2, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_raider4 = {
    n = 50, name = "Dark Emmet Raider", level=2,
    armor_class = 17,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0 },
        sting1 = { type = "sting", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0 },
        spit = { type = "spit", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0, range=8 }
    },
    hp_n = 2, hp_dice = 6, hp_mod = 1,
    gender = "female",
    flies = true
}

emmet_worker4 = {
    n = 25, name = "Dark Emmet Worker", level=1,
    armor_class = 17,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 2, n_dice = 1, die_type = 4, die_mod = 0 },
        spit = { type = "spit", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0, range=8 }
    },
    hp_n = 2, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

civilizations['emmet4'] = {
    tech_level = 4,
    species_def = 'emmet4',
    ai = 'worldeater',
    name_generator = "ant4",
    can_build = { "ant_mound", "ant_tunnel" },
    units = {
        garrison = {
            bp_per_turn = 1,
            speed = 0,
            name = "Emmet Guardians",
            sentients = {
                queen = emmet_queen4,
                princess = emmet_princess4,
                soldier = emmet_soldier4,
                worker = emmet_worker4
            },
            worldgen_strength = 15
        },
        swarm = {
            bp_per_turn = 0,
            speed = 1,
            name = "Emmet Swarm",
            sentients = {
                soldier = emmet_soldier4,
                worker = emmet_worker4
            },
            worldgen_strength = 25
        },
        raid = {
            bp_per_turn = 0,
            speed = 2,
            name = "Emmet Raid",
            sentients = {
                raider = emmet_raider4,
                soldier = emmet_soldier4
            },
            worldgen_strength = 50
        }
    },
    evolves_into = { "emmet5" }
}

function civ_name_gen_ant4(n)
    n = n % 6;
    if n == 0 then return "Dark Emmet Swarm";
    elseif n == 1 then return "Dark Emmet Hive";
    elseif n == 2 then return "Dark Emmet Nest";
    elseif n == 3 then return "Dark Emmet Mound";
    elseif n == 4 then return "Dark Emmet Plague";
    elseif n == 5 then return "Dark Emmet Monarchy";
    else return "Dark Emmet Infestation";
    end
end

function leader_name_gen_ant4(n)
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