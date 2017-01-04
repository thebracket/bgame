buildings = {
	cordex = {
		name = "Cordex",
		components = { { item="cordex_core", qty=1} },
		skill = { name="Construction", difficulty=25 },
		render_rex = "cordex.xp"
	},

	solar_panel = {
		name = "Solar Panel",
		components = { { item="solar_panel_kit", qty=1} },
		skill = { name="Construction", difficulty=15 },
		provides = { generator={energy_cost=10} },
		render = {
			width=1, height=1, tiles={
				{glyph=glyphs['solar_panel'], foreground = colors['cyan'], background = colors['white']}
			}
		}
	},

	cryo_bed = {
		name = "Cryogenic Bed",
		components = { { item="cryo_bed_kit", qty=1} },
		skill = { name="Construction", difficulty=10 },
		provides = { sleep={energy_cost=0} },
		render = {
			width=1, height=1, tiles={
				{glyph=glyphs['cryo_bed'], foreground = colors['white'], background = colors['black']}
			}
		}
	},

	battery = {
		name = "A battery bank",
		components = { { item="battery_kit", qty=1} },
		skill = { name="Construction", difficulty=10 },
		provides = { power_storage={energy_cost=20} },
		render = {
			width=1, height=1, tiles={
				{glyph=glyphs['battery'], foreground = colors['white'], background = colors['black']}
			}
		}
	},

	storage_locker = {
		name = "A storage locker",
		components = { { item="storage_locker", qty=1} },
		skill = { name="Construction", difficulty=10 },
		provides = { storage={energy_cost=0} },
		render = {
			width=1, height=1, tiles={
				{glyph=glyphs['storage_locker'], foreground = colors['white'], background = colors['black']}
			}
		}
	},

	small_replicator = {
		name = "A small replicator",
		components = { { item="replicator_unit", qty=1} },
		skill = { name="Construction", difficulty=10 },
		provides = { storage={energy_cost=0} },
		render = {
			width=1, height=1, tiles={
				{glyph=glyphs['replicator_small'], foreground = colors['white'], background = colors['black']}
			}
		}
	},

	rtg = {
		name = "A small nuclear power generator",
		components = { {item="refined_plutonium", qty=1} },
		skill = { name="Construction", difficulty=15 },
		provides = { storage={energy_cost=0} },
		render = {
			width=1, height=1, tiles={
				{glyph=glyphs['rtg'], foreground = colors['white'], background = colors['black']}
			}
		}
	},

	personal_survival_shelter = {
		name = "Tent",
		components = { { item="personal_survival_shelter_kit", qty=1 } },
		skill = { name="Construction", difficulty=5 },
		provides = { sleep={energy_cost=0} },
		render = {
			width=1, height=1, tiles={
				{glyph=glyphs['tent'], foreground = colors['green'], background = colors['black']}
			}
		}
	},

	fake_camp_fire = {
		name = "Nuclear Camp Fire",
		components = { { item="camp_fire_kit", qty=1 } },
		skill = { name="Construction", difficulty=5 },
		provides = { light={radius=5, color = colors['yellow']} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['campfire'], foreground = colors['firelight'], background = colors['yellow']}
			}
		},
        emits_smoke = true
	},

	camp_fire = {
		name = "Camp Fire",
		components = { { item="wood_log", qty=1 } },
		skill = { name="Construction", difficulty=5 },
		provides = { light={radius=5, color = colors['yellow']} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['campfire'], foreground = colors['firelight'], background = colors['yellow']}
			}
		},
        emits_smoke = true
	},

	sawmill = {
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
	},

	still = {
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
	},

	butcher = {
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
	},

	tanner = {
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
	},

	leatherworker = {
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
	},

	bonecarver = {
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
	},

	stonecutter = {
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
	},

	carpenter = {
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
	},

	charcoal_hut = {
		name = "Charcoal Burner",
		components = { { item="block", qty=1, mat_type="rock" } },
		skill = { name="Construction", difficulty=12 },
		render_rex = "charcoal_hut.xp"
	},

	smelter = {
		name = "Smelter",
		components = { { item="block", qty=1, mat_type="rock" } },
		skill = { name="Construction", difficulty=12 },
		render_rex = "smelter.xp"
	},

	glass_furnace = {
		name = "Glass Furnace",
		components = { { item="block", qty=1, mat_type="rock" } },
		skill = { name="Construction", difficulty=12 },
		render_rex = "glass_furnace.xp"
	},

	silicon_refinery = {
		name = "Silicon Refinery",
		components = { { item="block", qty=1, mat_type="rock" }, { item="raw_glass", qty=1 } },
		skill = { name="Construction", difficulty=12 },
		render_rex = "silicon_refinery.xp"
	},

	kiln = {
		name = "Kiln",
		components = { { item="clay", qty=1 } },
		skill = { name="Construction", difficulty=12 },
		render_rex = "kiln.xp"
	},

	primitive_forge = {
		name = "Primitive Forge",
		components = { { item="block", qty=1, mat_type="rock" } },
		skill = { name="Construction", difficulty=15 },
		render_rex = "primitive_forge.xp"
	},

	primitive_workshop = {
		name = "Primitive Workshop",
		components = { { item="block", qty=1 } },
		skill = { name="Construction", difficulty=15 },
		render_rex = "primitive_workshop.xp"
	},

	mason = {
		name = "Masonry",
		components = { { item="block", qty=1 } },
		skill = { name="Construction", difficulty=12 },
		render_rex = "mason.xp"
	},

	table = {
		name = "Table",
		components = { { item="table", qty=1 } },
		skill = { name="Construction", difficulty=5 },
		provides = { table={energy_cost=0} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['table'], foreground = colors['white'], background = colors['black']}
			}
		}
	},

	chair = {
		name = "Chair",
		components = { { item="chair", qty=1 } },
		skill = { name="Construction", difficulty=5 },
		provides = { table={energy_cost=0} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['chair'], foreground = colors['white'], background = colors['black']}
			}
		}
	},

	door = {
		name = "Door",
		components = { { item="door", qty=1 } },
		skill = { name="Construction", difficulty=10 },
		provides = { door={energy_cost=0} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['door'], foreground = colors['white'], background = colors['black']}
			}
		}
	},

	energy_door = {
		name = "Force Door",
		components = { { item="door", qty=1 } },
		skill = { name="Construction", difficulty=10 },
		provides = { door={energy_cost=10} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['door'], foreground = colors['cyan'], background = colors['black']}
			}
		}
	},

	wall = {
		name = "Wall", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 },
		provides = { wall={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['solid_square'], foreground = colors['white'], background = colors['black']}	} }
	},
	floor = {
		name = "Floor", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 },
		provides = { floor={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['comma'], foreground = colors['wood_brown'], background = colors['black']} } }
	},
	stairs_up = {
		name = "Up Stairs", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 },
		provides = { stairs_up={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['less_than'], foreground = colors['wood_brown'], background = colors['black']} } }
	},
	stairs_down = {
		name = "Down Stairs", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 },
		provides = { stairs_down={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['greater_than'], foreground = colors['wood_brown'], background = colors['black']} } }
	},
	stairs_updown = {
		name = "Spiral Stairs", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 },
		provides = { stairs_updown={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['X'], foreground = colors['wood_brown'], background = colors['black']} } }
	},
	ramp = {
		name = "Ramp", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 },
		provides = { ramp={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['ramp_up'], foreground = colors['wood_brown'], background = colors['black']} } }
	}
}