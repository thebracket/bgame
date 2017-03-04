
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
civilizations = {}

species_sentient = {
    human = {
        name = "Human", male_name = "Man", female_name = "Woman", group_name = "People",
        description = "A bipedal ape-descendant with an unhealthy obsession with money and power",
        stat_mods = {}, -- Humans have no modifiers
        parts = humanoid_parts,
        ethics = { diet="omnivore", alignment="neutral" },
        max_age = 114, infant_age=3, child_age=16,
        glyph = glyphs['at'], worldgen_glyph = glyphs['caveman']
    }
}