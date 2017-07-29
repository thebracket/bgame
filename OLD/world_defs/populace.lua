stone_age_default = {
    both = { torso="tunic/hide", shoes="sandals/hide" },
    male = { legs="britches/hide" },
    female = { legs="skirt_simple/hide" }
}
stone_age_leather = {
    both = { torso="tunic/leather", shoes="sandals/leather", head="cap/leather" },
    male = { legs="britches/leather" },
    female = { legs="skirt_simple/leather" }
}
stone_age_boiled_leather = {
    both = { torso="tunic/boiled_leather", shoes="sandals/boiled_leather", head="cap/boiled_leather" },
    male = { legs="britches/boiled_leather" },
    female = { legs="skirt_simple/boiled_leather" }
}

bronze_age_default = {
    both = { torso="tunic/leather", shoes="sandals/leather" },
    male = { legs="britches/leather" },
    female = { legs="skirt_simple/leather" }
}
bronze_age_light = {
    both = { torso="ringmail/bronze", shoes="sandals/leather", head="cap/leather" },
    male = { legs="britches/leather" },
    female = { legs="skirt_simple/leather" }
}
bronze_age_medium = {
    both = { torso="ringmail/bronze", shoes="sandals/leather", head="cap/leather" },
    male = { legs="britches/leather" },
    female = { legs="skirt_simple/leather" }
}
bronze_age_heavy = {
    both = { torso="breastplate/bronze", shoes="sandals/bronze", head="helm/bronze" },
    male = { legs="leggings/bronze" },
    female = { legs="skirt_simple/bone" }
}

native_populations = {
    -- The Stone Age
    laborer_1 = {
        {
            title = "Builder", aggression=2, modifiers = { str = 2, int = -1 },
            clothing = stone_age_default,
            melee = "warhammer/granite"
        },
        {
            title = "Woodworker", aggression=3, modifiers = { str = 2, int = -1 },
            clothing = stone_age_default,
            melee = "handaxe/granite"
        }
    },
    farmer_1 = {
        {
            title = "Gatherer", aggression=1, modifiers = { con = 1, int = -1 },
            clothing = stone_age_default,
            melee = "pointy_stick/wood"
        }
    },
    herbalist_1 = {
        {
            title = "Gatherer", aggression=1, modifiers = { con = 1, int = -1 },
            clothing = stone_age_default,
            melee = "pointy_stick/wood"
        }
    },
    metalworker_1 = {
        {
            title = "Crafter", aggression=1, modifiers = { dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "warhammer/granite"
        }
    },
    stoneworker_1 = {
        {
            title = "Crafter", aggression=1, modifiers = { dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "warhammer/granite"
        }
    },
    woodworker_1 = {
        {
            title = "Woodcrafter", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "battleaxe/granite"
        }
    },
    cook_1 = {
        {
            title = "Cook", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "knife/granite"
        }
    },
    hunter_1 = {
        {
            title = "Bow Hunter", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "dagger/granite", ranged = "shortbow/wood", ammo = "arrow/wood"
        },
        {
            title = "Atlatl Hunter", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "dagger/granite", ranged = "atlatl/wood", ammo = "dart/wood"
        }
    },
    skirmisher_1 = {
        {
            title = "Bow Hunter", aggression=4, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "dagger/granite", ranged = "shortbow/wood", ammo = "arrow/wood"
        },
        {
            title = "Atlatl Hunter", aggression=4, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "dagger/granite", ranged = "atlatl/wood", ammo = "dart/wood"
        }
    },
    lightinfantry_1 = {
        {
            title = "Archer", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "dagger/granite", ranged = "shortbow/wood", ammo = "arrow/wood"
        },
        {
            title = "Axeman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "battleaxe/granite"
        },
        {
            title = "Hammerman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "warhammer/granite"
        }
    },
    infantry_1 = {
        {
            title = "Archer", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "dagger/granite", ranged = "shortbow/wood", ammo = "arrow/wood"
        },
        {
            title = "Axeman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "battleaxe/granite"
        },
        {
            title = "Hammerman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "warhammer/granite"
        }
    },
    heavyinfantry_1 = {
        {
            title = "Archer", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "dagger/obsidian", ranged = "shortbow/bone", ammo = "arrow/bone"
        },
        {
            title = "Axeman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "battleaxe/obsidian"
        },
        {
            title = "Hammerman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "warhammer/obsidian"
        }
    },
    lightcavalry_1 = {
        {
            title = "Archer", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "dagger/granite", ranged = "shortbow/wood", ammo = "arrow/wood"
        },
        {
            title = "Axeman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "battleaxe/granite"
        },
        {
            title = "Hammerman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "warhammer/granite"
        }
    },
    mediumcavalry_1 = {
        {
            title = "Archer", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "dagger/granite", ranged = "shortbow/wood", ammo = "arrow/wood"
        },
        {
            title = "Axeman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "battleaxe/granite"
        },
        {
            title = "Hammerman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "warhammer/granite"
        }
    },
    heavycavalry_1 = {
        {
            title = "Archer", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "dagger/obsidian", ranged = "shortbow/bone", ammo = "arrow/bone"
        },
        {
            title = "Axeman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "battleaxe/obsidian"
        },
        {
            title = "Hammerman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "warhammer/obsidian"
        }
    },

    -- The Bronze Age
    laborer_2 = {
        {
            title = "Builder", aggression=2, modifiers = { str = 2, int = -1 },
            clothing = bronze_age_default,
            melee = "warhammer/bronze"
        },
        {
            title = "Woodworker", aggression=3, modifiers = { str = 2, int = -1 },
            clothing = bronze_age_default,
            melee = "handaxe/bronze"
        },
        {
            title = "Miner", aggression=4, modifiers = { str = 2, int = -1 },
            clothing = bronze_age_default,
            melee = "pickaxe/bronze"
        }
    },
    farmer_2 = {
        {
            title = "Gatherer", aggression=1, modifiers = { con = 1, int = -1 },
            clothing = bronze_age_default,
            melee = "knife/bronze"
        }
    },
    herbalist_2 = {
        {
            title = "Herbalist", aggression=1, modifiers = { wis = 1, int = -1 },
            clothing = bronze_age_default,
            melee = "dagger/bronze"
        }
    },
    metalworker_2 = {
        {
            title = "Smith", aggression=1, modifiers = { str = 2, int = 1 },
            clothing = bronze_age_default,
            melee = "warhammer/bronze"
        }
    },
    stoneworker_2 = {
        {
            title = "Mason", aggression=1, modifiers = { dex = 1, str = 1 },
            clothing = bronze_age_default,
            melee = "warhammer/bronze"
        }
    },
    woodworker_2 = {
        {
            title = "Woodcrafter", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_default,
            melee = "handaxe/bronze"
        }
    },
    cook_2 = {
        {
            title = "Cook", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_default,
            melee = "knife/bronze"
        }
    },
    hunter_2 = {
        {
            title = "Bow Hunter", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_default,
            melee = "dagger/bronze", ranged = "shortbow/wood", ammo = "arrow/wood"
        }
    },
    skirmisher_2 = {
        {
            title = "Scout", aggression=4, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "dagger/bronze", ranged = "shortbow/wood", ammo = "arrow/wood"
        }
    },
    lightinfantry_2 = {
        {
            title = "Archer", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "dagger/bronze", ranged = "shortbow/wood", ammo = "arrow/wood"
        },
        {
            title = "Axeman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "battleaxe/bronze"
        },
        {
            title = "Hammerman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "warhammer/bronze"
        },
        {
            title = "Swordsman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "shortsword/bronze"
        }
    },
    infantry_2 = {
        {
            title = "Archer", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "dagger/bronze", ranged = "shortbow/wood", ammo = "arrow/wood"
        },
        {
            title = "Axeman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "battleaxe/bronze"
        },
        {
            title = "Hammerman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "warhammer/bronze"
        },
        {
            title = "Swordsman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "longsword/bronze"
        }
    },
    heavyinfantry_2 = {
        {
            title = "Archer", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "dagger/bronze", ranged = "shortbow/wood", ammo = "arrow/wood"
        },
        {
            title = "Axeman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "battleaxe/bronze"
        },
        {
            title = "Hammerman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "warhammer/bronze"
        },
        {
            title = "Swordsman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "longsword/bronze"
        }
    },
    lightcavalry_2 = {
        {
            title = "Archer", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "dagger/bronze", ranged = "shortbow/wood", ammo = "arrow/wood"
        },
        {
            title = "Axeman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "battleaxe/bronze"
        },
        {
            title = "Hammerman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "warhammer/bronze"
        }
    },
    mediumcavalry_2 = {
        {
            title = "Archer", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "dagger/bronze", ranged = "shortbow/wood", ammo = "arrow/wood"
        },
        {
            title = "Axeman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "battleaxe/bronze"
        },
        {
            title = "Hammerman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "warhammer/bronze"
        }
    },
    heavycavalry_2 = {
        {
            title = "Archer", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "dagger/bronze", ranged = "shortbow/wood", ammo = "arrow/wood"
        },
        {
            title = "Axeman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "battleaxe/bronze"
        },
        {
            title = "Hammerman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "warhammer/bronze"
        }
    },
}