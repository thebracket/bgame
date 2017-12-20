-- Scary ant-people who just want to eat the world

species_sentient['emmet5'] = {
    name = "Cyber-Emmet", male_name = "Drone", female_name = "Queen", group_name = "Emmet",
    description = "A cybernetic ant-like creature of nightmares. It eats everything, spreads blight, spits fire and has evolved into a fully sentient race.",
    stat_mods = { con = 4, str = 4, int = -2, wis = -2 },
    parts = humanoid_parts,
    ethics = { diet = "omnivore", blight = true, alignment="devour" },
    max_age = 20, infant_age = 1, child_age = 2,
    glyph = glyphs['ant_cyber'], ascii_glyph = glyphs['a'],
    worldgen_glyph = glyphs['ant_cyber'],
    composite_render=false, vox = voxelId("ant")
}

emmet_queen5 = {
    n = 1, name = "Cyber-Emmet Queen", level=11,
    armor_class = 25,
    attacks = {
        bite1 = { type = "bite", hit_bonus = 8, n_dice = 3, die_type = 20, die_mod = 4 },
        disintegrator = { type = "disintegrator", hit_bonus = 8, n_dice = 3, die_type = 30, die_mod = 4 },
        spit = { type = "spit", hit_bonus = 8, n_dice = 3, die_type = 20, die_mod = 4, range = 8 }
    },
    hp_n = 10, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_princess5 = {
    n = 6, name = "Cyber-Emmet Princess", level=7,
    armor_class = 22,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 6, n_dice = 3, die_type = 12, die_mod = 2 },
        disintegrator = { type = "sting", hit_bonus = 6, n_dice = 3, die_type = 20, die_mod = 2 },
        spit = { type = "spit", hit_bonus = 6, n_dice = 3, die_type = 12, die_mod = 2, range=8 }
    },
    hp_n = 10, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_soldier5 = {
    n = 25, name = "Cyber-Emmet Soldier", level=2,
    armor_class = 17,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0 },
        disintegrator = { type = "disintegrator", hit_bonus = 2, n_dice = 1, die_type = 14, die_mod = 0 },
        spit = { type = "spit", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0, range=8 }
    },
    hp_n = 2, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_raider5 = {
    n = 50, name = "Cyber-Emmet Raider", level=2,
    armor_class = 17,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0 },
        disintegrator = { type = "disintegrator", hit_bonus = 2, n_dice = 1, die_type = 14, die_mod = 0 },
        spit = { type = "spit", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0, range=8 }
    },
    hp_n = 2, hp_dice = 6, hp_mod = 1,
    gender = "female",
    flies = true
}

emmet_worker5 = {
    n = 25, name = "Cyber-Emmet Worker", level=1,
    armor_class = 17,
    natural_attacks = {
        disintegrator = { type = "disintegrator", hit_bonus = 2, n_dice = 1, die_type = 8, die_mod = 0 },
        spit = { type = "spit", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0, range=8 }
    },
    hp_n = 2, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

civilizations['emmet5'] = {
    tech_level = 5,
    species_def = 'emmet5',
    ai = 'worldeater',
    name_generator = "ant5",
    can_build = { "ant_mound", "ant_tunnel" },
    units = {
        garrison = {
            bp_per_turn = 1,
            speed = 0,
            name = "Emmet Guardians",
            sentients = {
                queen = emmet_queen5,
                princess = emmet_princess5,
                soldier = emmet_soldier5,
                worker = emmet_worker5
            },
            worldgen_strength = 15
        },
        swarm = {
            bp_per_turn = 0,
            speed = 1,
            name = "Emmet Swarm",
            sentients = {
                soldier = emmet_soldier5,
                worker = emmet_worker5
            },
            worldgen_strength = 40
        },
        raid = {
            bp_per_turn = 0,
            speed = 2,
            name = "Emmet Raid",
            sentients = {
                raider = emmet_raider5,
                soldier = emmet_soldier5
            },
            worldgen_strength = 60
        }
    },
    evolves_into = { }
}

function civ_name_gen_ant5(n)
    n = n % 6;
    if n == 0 then return "Cyber-Emmet Swarm";
    elseif n == 1 then return "Cyber-Emmet Hive";
    elseif n == 2 then return "Cyber-Emmet Nest";
    elseif n == 3 then return "Cyber-Emmet Mound";
    elseif n == 4 then return "Cyber-Emmet Plague";
    elseif n == 5 then return "Cyber-Emmet Monarchy";
    else return "Cyber-Emmet Infestation";
    end
end

function leader_name_gen_ant5(n)
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