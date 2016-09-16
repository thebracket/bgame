stone_age_default = {
    both = { torso="hide_tunic", shoes="hide_sandals" },
    male = { legs="hide_pants" },
    female = { legs="hide_skirt" }
}
stone_age_leather = {
    both = { torso="leather_tunic", shoes="hide_sandals", head="leather_cap" },
    male = { legs="leather_pants" },
    female = { legs="leather_skirt" }
}
stone_age_boiled_leather = {
    both = { torso="boiled_leather_tunic", shoes="hide_sandals", head="leather_cap" },
    male = { legs="leather_pants" },
    female = { legs="leather_skirt" }
}

bronze_age_default = {
    both = { torso="cloth_tunic", shoes="leather_sandals" },
    male = { legs="cloth_pants" },
    female = { legs="cloth_skirt" }
}
bronze_age_light = {
    both = { torso="bronze_ringmail", shoes="leather_sandals", head="leather_cap" },
    male = { legs="cloth_pants" },
    female = { legs="cloth_skirt" }
}
bronze_age_medium = {
    both = { torso="bronze_ringmail", shoes="leather_sandals", head="bronze_cap" },
    male = { legs="leather_pants" },
    female = { legs="leather_skirt" }
}
bronze_age_heavy = {
    both = { torso="bronze_breastplate", shoes="leather_sandals", head="bronze_helm" },
    male = { legs="bronze_leggings" },
    female = { legs="bronze_skirt" }
}

native_populations = {
    -- The Stone Age
    laborer_1 = {
        {
            title = "Builder", aggression=2, modifiers = { str = 2, int = -1 },
            clothing = stone_age_default,
            melee = "stone_hammer"
        },
        {
            title = "Woodworker", aggression=3, modifiers = { str = 2, int = -1 },
            clothing = stone_age_default,
            melee = "stone_axe"
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
            melee = "stone_hammer"
        }
    },
    stoneworker_1 = {
        {
            title = "Crafter", aggression=1, modifiers = { dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "stone_hammer"
        }
    },
    woodworker_1 = {
        {
            title = "Woodcrafter", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "stone_axe"
        }
    },
    cook_1 = {
        {
            title = "Cook", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "stone_knife"
        }
    },
    hunter_1 = {
        {
            title = "Bow Hunter", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "stone_knife", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Atlatl Hunter", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "stone_knife", ranged = "atlatl", ammo = "wood_dart"
        }
    },
    skirmisher_1 = {
        {
            title = "Bow Hunter", aggression=4, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "stone_knife", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Atlatl Hunter", aggression=4, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "stone_knife", ranged = "atlatl", ammo = "wood_dart"
        }
    },
    lightinfantry_1 = {
        {
            title = "Archer", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "stone_knife", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "stone_axe"
        },
        {
            title = "Hammerman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "stone_hammer"
        }
    },
    infantry_1 = {
        {
            title = "Archer", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "stone_knife", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "stone_axe"
        },
        {
            title = "Hammerman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "stone_hammer"
        }
    },
    heavyinfantry_1 = {
        {
            title = "Archer", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "stone_knife", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "stone_axe"
        },
        {
            title = "Hammerman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "stone_hammer"
        }
    },
    lightcavalry_1 = {
        {
            title = "Archer", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "stone_knife", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "stone_axe"
        },
        {
            title = "Hammerman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_default,
            melee = "stone_hammer"
        }
    },
    mediumcavalry_1 = {
        {
            title = "Archer", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "stone_knife", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "stone_axe"
        },
        {
            title = "Hammerman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_leather,
            melee = "stone_hammer"
        }
    },
    heavycavalry_1 = {
        {
            title = "Archer", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "stone_knife", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "stone_axe"
        },
        {
            title = "Hammerman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = stone_age_boiled_leather,
            melee = "stone_hammer"
        }
    },

    -- The Bronze Age
    laborer_2 = {
        {
            title = "Builder", aggression=2, modifiers = { str = 2, int = -1 },
            clothing = bronze_age_default,
            melee = "bronze_hammer"
        },
        {
            title = "Woodworker", aggression=3, modifiers = { str = 2, int = -1 },
            clothing = bronze_age_default,
            melee = "bronze_axe"
        },
        {
            title = "Miner", aggression=4, modifiers = { str = 2, int = -1 },
            clothing = bronze_age_default,
            melee = "bronze_pickaxe"
        }
    },
    farmer_2 = {
        {
            title = "Gatherer", aggression=1, modifiers = { con = 1, int = -1 },
            clothing = bronze_age_default,
            melee = "bronze_dagger"
        }
    },
    herbalist_2 = {
        {
            title = "Herbalist", aggression=1, modifiers = { wis = 1, int = -1 },
            clothing = bronze_age_default,
            melee = "bronze_dagger"
        }
    },
    metalworker_2 = {
        {
            title = "Smith", aggression=1, modifiers = { str = 2, int = 1 },
            clothing = bronze_age_default,
            melee = "bronze_hammer"
        }
    },
    stoneworker_2 = {
        {
            title = "Mason", aggression=1, modifiers = { dex = 1, str = 1 },
            clothing = bronze_age_default,
            melee = "bronze_hammer"
        }
    },
    woodworker_2 = {
        {
            title = "Woodcrafter", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_default,
            melee = "bronze_axe"
        }
    },
    cook_2 = {
        {
            title = "Cook", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_default,
            melee = "bronze_knife"
        }
    },
    hunter_2 = {
        {
            title = "Bow Hunter", aggression=1, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_default,
            melee = "bronze_dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        }
    },
    skirmisher_2 = {
        {
            title = "Scout", aggression=4, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "bronze_dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        }
    },
    lightinfantry_2 = {
        {
            title = "Archer", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "bronze_dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "bronze_axe"
        },
        {
            title = "Hammerman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "bronze_hammer"
        },
        {
            title = "Swordsman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "bronze_sword"
        }
    },
    infantry_2 = {
        {
            title = "Archer", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "stone_knife", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "bronze_axe"
        },
        {
            title = "Hammerman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "bronze_hammer"
        },
        {
            title = "Swordsman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "bronze_sword"
        }
    },
    heavyinfantry_2 = {
        {
            title = "Archer", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "bronze_dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "bronze_axe"
        },
        {
            title = "Hammerman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "bronze_hammer"
        },
        {
            title = "Swordsman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "bronze_sword"
        }
    },
    lightcavalry_2 = {
        {
            title = "Archer", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "bronze_dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "bronze_axe"
        },
        {
            title = "Hammerman", aggression=5, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_light,
            melee = "bronze_hammer"
        }
    },
    mediumcavalry_2 = {
        {
            title = "Archer", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "bronze_dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "bronze_axe"
        },
        {
            title = "Hammerman", aggression=6, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_medium,
            melee = "bronze_hammer"
        }
    },
    heavycavalry_2 = {
        {
            title = "Archer", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "bronze_dagger", ranged = "wood_shortbow", ammo = "wood_arrow"
        },
        {
            title = "Axeman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "bronze_axe"
        },
        {
            title = "Hammerman", aggression=7, modifiers = { str = 1, dex = 1, int = 1 },
            clothing = bronze_age_heavy,
            melee = "bronze_hammer"
        }
    },
}