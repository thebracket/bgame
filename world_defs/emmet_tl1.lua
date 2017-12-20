-- Scary ant-people who just want to eat the world

species_sentient['emmet1'] = {
    name = "Bronze-Lined Emmet", male_name = "Drone", female_name = "Queen", group_name = "Emmet",
    description = "An ant-like creature of nightmares. It eats everything, and spreads blight. It was worked bronze armor into its chitin.",
    stat_mods = { con = 3, str = 3, int = -2, wis = -2 },
    parts = humanoid_parts,
    ethics = { diet = "omnivore", blight = true, alignment="devour" },
    max_age = 20, infant_age = 1, child_age = 2,
    glyph = glyphs['ant_bronze'], ascii_glyph = glyphs['a'],
    worldgen_glyph = glyphs['ant_bronze'],
    composite_render=false, vox = voxelId("ant")
}

emmet_queen1 = {
    n = 1, name = "Bronze Emmet Queen", level=11,
    armor_class = 22,
    attacks = {
        bite1 = { type = "bite", hit_bonus = 8, n_dice = 3, die_type = 20, die_mod = 4 },
        sting1 = { type = "sting", hit_bonus = 8, n_dice = 3, die_type = 20, die_mod = 4 }
    },
    hp_n = 10, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_princess1 = {
    n = 6, name = "Bronze Emmet Princess", level=7,
    armor_class = 19,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 6, n_dice = 3, die_type = 12, die_mod = 2 },
        sting1 = { type = "sting", hit_bonus = 6, n_dice = 3, die_type = 12, die_mod = 2 }
    },
    hp_n = 10, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_soldier1 = {
    n = 25, name = "Bronze Emmet Soldier", level=2,
    armor_class = 16,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0 },
        sting1 = { type = "sting", hit_bonus = 2, n_dice = 1, die_type = 12, die_mod = 0 }
    },
    hp_n = 2, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

emmet_worker1 = {
    n = 25, name = "Bronze Emmet Worker", level=1,
    armor_class = 14,
    natural_attacks = {
        bite1 = { type = "bite", hit_bonus = 2, n_dice = 1, die_type = 4, die_mod = 0 }
    },
    hp_n = 2, hp_dice = 6, hp_mod = 1,
    gender = "female"
}

civilizations['emmet1'] = {
    tech_level = 1,
    species_def = 'emmet1',
    ai = 'worldeater',
    name_generator = "ant1",
    can_build = { "ant_mound", "ant_tunnel" },
    units = {
        garrison = {
            bp_per_turn = 1,
            speed = 0,
            name = "Emmet Guardians",
            sentients = {
                queen = emmet_queen1,
                princess = emmet_princess1,
                soldier = emmet_soldier1,
                worker = emmet_worker1
            },
            worldgen_strength = 3
        },
        swarm = {
            bp_per_turn = 0,
            speed = 1,
            name = "Emmet Swarm",
            sentients = {
                soldier = emmet_soldier1,
                worker = emmet_worker1
            },
            worldgen_strength = 5
        }
    },
    evolves_into = { "emmet2" }
}

function civ_name_gen_ant1(n)
    n = n % 6;
    if n == 0 then return "Bronze Emmet Swarm";
    elseif n == 1 then return "Bronze Emmet Hive";
    elseif n == 2 then return "Bronze Emmet Nest";
    elseif n == 3 then return "Bronze Emmet Mound";
    elseif n == 4 then return "Bronze Emmet Plague";
    elseif n == 5 then return "Bronze Emmet Monarchy";
    else return "Bronze Emmet Infestation";
    end
end

function leader_name_gen_ant1(n)
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