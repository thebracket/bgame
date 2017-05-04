
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

human_skin_tones = {
   Pale = { r=237, g=232, b=226 },
   Peach = { r=255, g=216, b=173 },
   Tanned = { r=255, g=205, b=148 },
   Wan = { r=234, g=192, b=134},
   Yellow = {r=255, g=227, b=159},
   Red = {r=113, g=2, b=0},
   Dark = { r=198, g=134, b=66 },
   Darker = { r=141, g=85, b=36 },
   Black = { r=56, g=0, b=0 }
}

species_sentient = {
    human = {
        name = "Human", male_name = "Man", female_name = "Woman", group_name = "People",
        description = "A bipedal ape-descendant with an unhealthy obsession with money and power",
        stat_mods = {}, -- Humans have no modifiers
        parts = humanoid_parts,
        ethics = { diet="omnivore", alignment="neutral" },
        max_age = 114, infant_age=3, child_age=16,
        glyph = glyphs['at'], worldgen_glyph = glyphs['caveman'],
        skin_colors = human_skin_tones
    }
}