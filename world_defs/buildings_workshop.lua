-- Workshops

buildings["small_replicator"] = {
    name = "A small replicator",
    components = { { item="replicator_unit", qty=1} },
    skill = { name="Construction", difficulty=10 },
    provides = { storage={energy_cost=0} },
    render = {
        width=1, height=1, tiles={
            {glyph=glyphs['replicator_small'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles={
            {glyph=glyphs['square_root'], foreground = colors['cyan'], background = colors['black']}
        }
    }
};

buildings["fake_camp_fire"] = {
    name = "Nuclear Camp Fire",
    components = { { item="camp_fire_kit", qty=1 } },
    skill = { name="Construction", difficulty=5 },
    provides = { light={radius=5, color = colors['yellow']} },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['campfire'], foreground = colors['firelight'], background = colors['yellow']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['sun'], foreground = colors['yellow'], background = colors['yellow']}
        }
    },
    emits_smoke = true
};

buildings["camp_fire"] = {
    name = "Camp Fire",
    components = { { item="wood_log", qty=1 } },
    skill = { name="Construction", difficulty=5 },
    provides = { light={radius=5, color = colors['yellow']} },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['campfire'], foreground = colors['firelight'], background = colors['yellow']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['sun'], foreground = colors['yellow'], background = colors['yellow']}
        }
    },
    emits_smoke = true
};

buildings["sawmill"] = {
    name = "Sawmill",
    components = { { item="wood_log", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['sawmill_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['sawmill_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['sawmill_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['sawmill_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['three_bar'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['three_bar'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['sun'], foreground = colors['cyan'], background = colors['black']}
        }
    },
};

buildings["still"] = {
    name = "Distillery",
    components = { { item="block", qty=2 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['still_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['still_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['still_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['still_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['kit'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['kit'], foreground = colors['wood_brown'], background = colors['black']}
        }
    },
};

buildings["butcher"] = {
    name = "Butcher",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['butcher_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['butcher_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['butcher_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['butcher_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['cabinet'], foreground = colors['red'], background = colors['black']},
            {glyph= glyphs['kit'], foreground = colors['red'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['red'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['red'], background = colors['black']}
        }
    },
};

buildings["tanner"] = {
    name = "Tanner",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['tanner_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['tanner_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['tanner_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['tanner_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['double_wall_nse'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['double_wall_nsw'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['wood_brown'], background = colors['black']}
        }
    },
};

buildings["leatherworker"] = {
    name = "Leatherworker",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['leatherworker_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['leatherworker_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['leatherworker_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['leatherworker_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['chair'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['cabinet'], foreground = colors['wood_brown'], background = colors['black']}
        }
    },
};

buildings["bonecarver"] = {
    name = "Bonecarver",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['bonecarver_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['bonecarver_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['bonecarver_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['bonecarver_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['happy_face'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['happy_face'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['vertical_line'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['vertical_line'], foreground = colors['white'], background = colors['black']}
        }
    },
};

buildings["stonecutter"] = {
    name = "Stonecutters",
    components = { { item="stone_boulder", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['stonecutter_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['stonecutter_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['stonecutter_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['stonecutter_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['paragraph'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['sun'], foreground = colors['white'], background = colors['black']}
        }
    },
};

buildings["carpenter"] = {
    name = "Carpentry Workshop",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['carpenter_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['cabinet'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['cabinet'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['wood_brown'], background = colors['black']}
        }
    },
};

buildings["charcoal_hut"] = {
    name = "Charcoal Burner",
    components = { { item="block", qty=1, mat_type="rock" } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['charcoal_hut'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['arch'], foreground = colors['wood_brown'], background = colors['black']}
        }
    },
};

buildings["smelter"] = {
    name = "Smelter",
    components = { { item="block", qty=1, mat_type="rock" } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['smelter'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['arch'], foreground = colors['red'], background = colors['black']}
        }
    },
};

buildings["glass_furnace"] = {
    name = "Glass Furnace",
    components = { { item="block", qty=1, mat_type="rock" } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['glass_furnace'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['arch'], foreground = colors['light_green'], background = colors['black']}
        }
    },
};

buildings["silicon_refinery"] = {
    name = "Silicon Refinery",
    components = { { item="block", qty=1, mat_type="rock" }, { item="raw_glass", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['silicon_refinery'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['arch'], foreground = colors['cyan'], background = colors['black']}
        }
    },
};

buildings["kiln"] = {
    name = "Kiln",
    components = { { item="clay", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['kiln'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['arch'], foreground = colors['orange'], background = colors['black']}
        }
    },
};

buildings["primitive_forge"] = {
    name = "Primitive Forge",
    components = { { item="block", qty=1, mat_type="rock" } },
    skill = { name="Construction", difficulty=15 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['forge_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['forge_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['forge_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['forge_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['sun'], foreground = colors['cyan'], background = colors['black']},
            {glyph= glyphs['cabinet'], foreground = colors['grey'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['grey'], background = colors['black']},
            {glyph= glyphs['sun'], foreground = colors['yellow'], background = colors['black']}
        }
    },
};

buildings["primitive_workshop"] = {
    name = "Primitive Workshop",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=15 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['workshop_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['workshop_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['workshop_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['workshop_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['cabinet'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['cabinet'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['white'], background = colors['black']}
        }
    },
};

buildings["mason"] = {
    name = "Masonry",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['mason_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mason_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mason_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mason_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']}
        }
    },
};

buildings["mechanic"] = {
    name = "Mechanic Workshop",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['mechanic_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mechanic_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mechanic_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['mechanic_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['star'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['star'], foreground = colors['red'], background = colors['black']}
        }
    },
};
