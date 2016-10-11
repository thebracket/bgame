stone_age_default = {
    both = { torso="tunic", shoes="sandals" },
    male = { legs="britches" },
    female = { legs="skirt_simple" }
}
stone_age_leather = {
    both = { torso="tunic", shoes="sandals", head="cap" },
    male = { legs="britches" },
    female = { legs="skirt_simple" }
}
stone_age_boiled_leather = {
    both = { torso="tunic", shoes="sandals", head="cap" },
    male = { legs="britches" },
    female = { legs="skirt_simple" }
}

bronze_age_default = {
    both = { torso="tunic", shoes="sandals" },
    male = { legs="britches" },
    female = { legs="skirt_simple" }
}
bronze_age_light = {
    both = { torso="ringmail", shoes="sandals", head="cap" },
    male = { legs="britches" },
    female = { legs="skirt_simple" }
}
bronze_age_medium = {
    both = { torso="ringmail", shoes="sandals", head="cap" },
    male = { legs="britches" },
    female = { legs="skirt_simple" }
}
bronze_age_heavy = {
    both = { torso="breastplate", shoes="sandals", head="helm" },
    male = { legs="leggings" },
    female = { legs="skirt_simple" }
}

native_populations = {
    -- The Stone Age
    laborer_1 = {
        {
            title = "Builder", aggression=2, modifiers = { str = 2, int = -1 },
            clothing = stone_age_default,
            melee = "warhammer"
        },
        {
            title = "Woodworker", aggression=3, modifiers = { str = 2, int = -1 },
            clothing = stone_age_default,
            melee = "handaxe"
        }
    },
    farmer_1 = {
        {
            title = "Gatherer", aggression=1, modifiers = { con = 1, int = -1 },
            clothing = stone_age_default,
            melee = "pointy_stick"
        }
    },
    herbalist_1 = {
        {
            title = "Gatherer", aggression=1, modifiers = { con = 1, int = -1 },
            clothing = stone_age_default,
            melee = "pointy_stick"
        }
    },
    metalworker_1 = {
        {
            title = "Crafter", aggression=1, modifiers = { dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "warhammer"
        }
    },
    stoneworker_1 = {
        {
            title = "Crafter", aggression=1, modifiers = { dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "warhammer"
        }
    },
    woodworker_1 = {
        {
            title = "Woodcrafter", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "battleaxe"
        }
    },
    cook_1 = {
        {
            title = "Cook", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "knife"
        }
    },
    hunter_1 = {
        {
            title = "Bow Hunter", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Atlatl Hunter", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "dagger", ranged = "atlatl", ammo = "wood_dart"
        }
    },
    skirmisher_1 = {
        {
            title = "Bow Hunter", aggression=4, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Atlatl Hunter", aggression=4, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "dagger", ranged = "atlatl", ammo = "wood_dart"
        }
    },
    lightinfantry_1 = {
        {
            title = "Archer", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "battleaxe"
        },
        {
            title = "Hammerman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "warhammer"
        }
    },
    infantry_1 = {
        {
            title = "Archer", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "battleaxe"
        },
        {
            title = "Hammerman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "warhammer"
        }
    },
    heavyinfantry_1 = {
        {
            title = "Archer", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "battleaxe"
        },
        {
            title = "Hammerman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "warhammer"
        }
    },
    lightcavalry_1 = {
        {
            title = "Archer", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "battleaxe"
        },
        {
            title = "Hammerman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "warhammer"
        }
    },
    mediumcavalry_1 = {
        {
            title = "Archer", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "battleaxe"
        },
        {
            title = "Hammerman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "warhammer"
        }
    },
    heavycavalry_1 = {
        {
            title = "Archer", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "battleaxe"
        },
        {
            title = "Hammerman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "warhammer"
        }
    },

    -- The Bronze Age
    laborer_2 = {
        {
            title = "Builder", aggression=2, modifiers = { str = 2, int = -1 },
            clothing = bronze_age_default,
            melee = "warhammer"
        },
        {
            title = "Woodworker", aggression=3, modifiers = { str = 2, int = -1 },
            clothing = bronze_age_default,
            melee = "handaxe"
        },
        {
            title = "Miner", aggression=4, modifiers = { str = 2, int = -1 },
            clothing = bronze_age_default,
            melee = "pickaxe"
        }
    },
    farmer_2 = {
        {
            title = "Gatherer", aggression=1, modifiers = { con = 1, int = -1 },
            clothing = bronze_age_default,
            melee = "knife"
        }
    },
    herbalist_2 = {
        {
            title = "Herbalist", aggression=1, modifiers = { wis = 1, int = -1 },
            clothing = bronze_age_default,
            melee = "dagger"
        }
    },
    metalworker_2 = {
        {
            title = "Smith", aggression=1, modifiers = { str = 2, int = 1 },
            clothing = bronze_age_default,
            melee = "warhammer"
        }
    },
    stoneworker_2 = {
        {
            title = "Mason", aggression=1, modifiers = { dex = 1, str = 1 },
            clothing = bronze_age_default,
            melee = "warhammer"
        }
    },
    woodworker_2 = {
        {
            title = "Woodcrafter", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_default,
            melee = "handaxe"
        }
    },
    cook_2 = {
        {
            title = "Cook", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_default,
            melee = "knife"
        }
    },
    hunter_2 = {
        {
            title = "Bow Hunter", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_default,
            melee = "dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        }
    },
    skirmisher_2 = {
        {
            title = "Scout", aggression=4, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        }
    },
    lightinfantry_2 = {
        {
            title = "Archer", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "battleaxe"
        },
        {
            title = "Hammerman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "warhammer"
        },
        {
            title = "Swordsman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "shortsword"
        }
    },
    infantry_2 = {
        {
            title = "Archer", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "battleaxe"
        },
        {
            title = "Hammerman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "warhammer"
        },
        {
            title = "Swordsman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "longsword"
        }
    },
    heavyinfantry_2 = {
        {
            title = "Archer", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "battleaxe"
        },
        {
            title = "Hammerman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "warhammer"
        },
        {
            title = "Swordsman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "longsword"
        }
    },
    lightcavalry_2 = {
        {
            title = "Archer", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "battleaxe"
        },
        {
            title = "Hammerman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "warhammer"
        }
    },
    mediumcavalry_2 = {
        {
            title = "Archer", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "battleaxe"
        },
        {
            title = "Hammerman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "warhammer"
        }
    },
    heavycavalry_2 = {
        {
            title = "Archer", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "battleaxe"
        },
        {
            title = "Hammerman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "warhammer"
        }
    },
}