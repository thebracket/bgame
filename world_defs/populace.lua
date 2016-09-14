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
    }
}