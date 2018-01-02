
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

human_hair_colors = {
    Black = { r=9, g=8, b=6 },
    OffBlack = { r=44, g=34, b=43 },
    DarkestBrown = { r=59, g=49, b=38 },
    Mocha = { r=78, g=87, b=83 },
    Chestnut = { r=90, g=68, b=68 },
    LightChestnut = { r=106, g=78, b=66 },
    DarkGoldenBrown = { r=05, g=72, b=56 },
    LightGoldenBrown = { r=167, g=133, b=106 },
    DarkHoneyBlonde = { r=194, g=151, b=123 },
    BleachedBlonde = { r=220, g=209, b=196 },
    LightAshBlonde = { r=222, g=168, b=153 },
    LightAshBrown = { r=151, g=121, b=97 },
    LightestBlonde = { r=230, g=206, b=168 },
    PaleGoldenBlonde = { r=229, g=200, b=168 },
    StrawberryBlonde = { r=165, g=137, b=70 },
    LightAuburn = { r=146, g=85, b=61 },
    DarkAuburn = { r=83, g=61, b=53 },
    DarkestGrey = { r=113, g=99, b=93 },
    MediumGrey = { r=183, g=166, b=158 },
    LightGrey = { r=214, g=196, b=194 },
    WhiteBlonde = { r=255, g=245, b=225 },
    PlatinumBlonde = { r=202, g=191, b=177 },
    RussetRed = { r=141, g=74, b=67 },
    Terracotta = { r=181, g=82, b=57 }
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
        skin_colors = human_skin_tones, hair_colors = human_hair_colors,
        base_male_glyph = 352, base_female_glyph = 353
    }
}