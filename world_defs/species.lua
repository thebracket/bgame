species_sentient = {
    human = {
        name = "Human", male_name = "Man", female_name = "Woman", group_name = "People",
        description = "A bipedal ape-descendant with an unhealthy obsession with money and power",
        stat_mods = {}, -- Humans have no modifiers
        parts = { 
            head = { qty=1, size = 10 }, 
            torso = { qty = 1, size = 50 }, 
            arms = { qty=2, size = 10 }, 
            legs = { qty=2, size = 10 } 
        },
        ethics = { diet="omnivore", alignment="neutral" },
        max_age = 114, infant_age=3, child_age=16,
        glyph = glyphs['at']
    },
    dwarf = {
        name = "Dwarf", male_name = "Dwarf", female_name = "Dwarf", group_name = "Dwarves",
        description = "Short, sturdy folk obsessed with industry and hard work",
        stat_mods = {con=1,str=1,dex=-2},
        parts = { 
            head = { qty=1, size = 10 }, 
            torso = { qty = 1, size = 50 }, 
            arms = { qty=2, size = 10 }, 
            legs = { qty=2, size = 10 } 
        },
        ethics = { diet="omnivore", alignment="good" },
        max_age = 200, infant_age=3, child_age=30,
        glyph = glyphs['d']
    },
    gnome = {
        name = "Gnome", male_name = "Nebli", female_name = "Neebli", group_name = "Gnomes",
        description = "Short, sturdy folk with a bad habit of inventing explosives",
        stat_mods = {dex=1,int=1,con=-1},
        parts = { 
            head = { qty=1, size = 10 }, 
            torso = { qty = 1, size = 50 }, 
            arms = { qty=2, size = 10 }, 
            legs = { qty=2, size = 10 } 
        },
        ethics = { diet="omnivore", alignment="neutral" },
        max_age = 125, infant_age=3, child_age=25,
        glyph = glyphs['g']
    },
    vogon = {
        name = "Vogon", male_name = "Poet", female_name = "Writer", group_name = "Vogons",
        description = "A large, bipedal creature with green skin and awful poetry",
        stat_mods = { str=2, con=2, int=-2 },
        parts = { 
            head = { qty=1, size = 10 }, 
            torso = { qty = 1, size = 50 }, 
            arms = { qty=2, size = 10 }, 
            legs = { qty=2, size = 10 } 
        },
        ethics = { diet="omnivore", alignment="evil" },
        max_age = 40, infant_age=1, child_age=3,
        glyph = glyphs['v']
    },
    felinus = {
        name = "Felinus", male_name = "Tom", female_name = "Queen", group_name = "Clowder",
        description = "A bipedal cat person with an unusual sense of self-worth",
        stat_mods = { dex=2,com=1,con=-2 },
        parts = { 
            head = { qty=1, size = 10 }, 
            torso = { qty = 1, size = 40 }, 
            arms = { qty=2, size = 10 }, 
            legs = { qty=2, size = 10 },
            tail_prehensile = { qty = 1, size = 10 }
        },
        ethics = { diet="carnivore", alignment="neutral" },
        max_age = 75, infant_age = 2, child_age = 14,
        glyph = glyphs['C']
    },
    rhinoran = {
        name = "Rhinoran", male_name = "Tom", female_name = "Queen", group_name = "Crash",
        description = "A large biped with a rhino horn",
        stat_mods = { str=2, con=2, int=-1 },
        parts = { 
            head = { qty=1, size = 10 }, 
            torso = { qty = 1, size = 40 }, 
            arms = { qty=2, size = 10 }, 
            legs = { qty=2, size = 10 },
            horn_weapon = { qty = 1, size = 10 }
        },
        ethics = { diet="herbivore", alignment="neutral" },
        max_age = 60, infant_age = 2, child_age = 14
    },
    elder = {
        name = "Elder", male_name = "Shi", female_name = "Sha", group_name = "Wise Ones",
        description = "A tall, slender humanoid ",
        stat_mods = { int=2,wis=2,str=-1 }, 
        parts = { 
            head = { qty=1, size = 10 }, 
            torso = { qty = 1, size = 50 }, 
            arms = { qty=2, size = 10 }, 
            legs = { qty=2, size = 10 },
        },
        ethics = { diet="herbivore", alignment="neutral" },
        max_age = 1000, infant_age = 2, child_age = 30,
        glyph = glyphs['e']
    },
    fae = {
        name = "Fae", male_name = "Unseelie", female_name = "Seelie", group_name = "Sidhe",
        description = "A small humanoid with gossamer wings ",
        stat_mods = { str=-2, con=-1, int=2, cha=2 },
        parts = { 
            head = { qty=1, size = 10 }, 
            torso = { qty = 1, size = 40 }, 
            arms = { qty=2, size = 10 }, 
            legs = { qty=2, size = 10 },
            wings = { qty=2, size = 10 }
        },
        ethics = { diet="herbivore", alignment="neutral" },
        max_age = 1000, infant_age = 2, child_age = 30,
        glyph = glyphs['f']
    }
}