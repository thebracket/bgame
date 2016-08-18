buildings = {
	cordex = {
		name = "Cordex",
		components = { "cordex_core" },
		skill = { name="Construction", difficulty=25 },
		render_rex = "cordex.xp"
	},

	solar_panel = {
		name = "Solar Panel",
		components = { "solar_panel_kit" },
		skill = { name="Construction", difficulty=15 },
		provides = { generator={energy_cost=10} },
		render = {
			width=1, height=1, tiles={
				{glyph=glyphs['one_quarter_solid'], foreground = colors['cyan'], background = colors['white']}
			}
		}
	},

	cryo_bed = {
		name = "Cryogenic Bed",
		components = { "cryo_bed_kit" },
		skill = { name="Construction", difficulty=10 },
		provides = { sleep={energy_cost=0} },
		render = {
			width=1, height=1, tiles={
				{glyph=glyphs['zero'], foreground = colors['white'], background = colors['black']}
			}
		}
	},

	battery = {
		name = "A battery bank",
		components = { "battery_kit" },
		skill = { name="Construction", difficulty=10 },
		provides = { power_storage={energy_cost=20} },
		render = {
			width=1, height=1, tiles={
				{glyph=glyphs['plus_minus'], foreground = colors['white'], background = colors['black']}
			}
		}
	},

	storage_locker = {
		name = "A storage locker",
		components = { "storage_locker" },
		skill = { name="Construction", difficulty=10 },
		provides = { storage={energy_cost=0} },
		render = {
			width=1, height=1, tiles={
				{glyph=glyphs['infinity'], foreground = colors['white'], background = colors['black']}
			}
		}
	},

	small_replicator = {
		name = "A small replicator",
		components = { "replicator_unit" },
		skill = { name="Construction", difficulty=10 },
		provides = { storage={energy_cost=0} },
		render = {
			width=1, height=1, tiles={
				{glyph=glyphs['square_root'], foreground = colors['white'], background = colors['black']}
			}
		}
	},

	rtg = {
		name = "A small nuclear power generator",
		components = { "refined_plutonium" },
		skill = { name="Construction", difficulty=15 },
		provides = { storage={energy_cost=0} },
		render = {
			width=1, height=1, tiles={
				{glyph=glyphs['double_pillar'], foreground = colors['white'], background = colors['black']}
			}
		}
	},

	personal_survival_shelter = {
		name = "Tent",
		components = { "personal_survival_shelter_kit" },
		skill = { name="Construction", difficulty=5 },
		provides = { sleep={energy_cost=0} },
		render = {
			width=1, height=1, tiles={
				{glyph=glyphs['arch'], foreground = colors['green'], background = colors['black']}
			}
		}
	},

	fake_camp_fire = {
		name = "Nuclear Camp Fire",
		components = { "camp_fire_kit" },
		skill = { name="Construction", difficulty=5 },
		provides = { light={radius=5, color = colors['yellow']} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['sun'], foreground = colors['firelight'], background = colors['yellow']}
			}
		}
	},

	sawmill = {
		name = "Sawmill",
		components = { "wood_log" },
		skill = { name="Construction", difficulty=12 },
		render_rex = "sawmill.xp"
	},

	butcher = {
		name = "Butcher",
		components = { "wood_plank" },
		skill = { name="Construction", difficulty=12 },
		render_rex = "butchershop.xp"
	},

	tanner = {
		name = "Tanner",
		components = { "wood_plank" },
		skill = { name="Construction", difficulty=12 },
		render_rex = "tanner.xp"
	},

	leatherworker = {
		name = "Leatherworker",
		components = { "wood_plank" },
		skill = { name="Construction", difficulty=12 },
		render_rex = "leatherworker.xp"
	},

	bonecarver = {
		name = "Bonecarver",
		components = { "wood_plank" },
		skill = { name="Construction", difficulty=12 },
		render_rex = "bonecarver.xp"
	},

	stonecutter = {
		name = "Stonecutters",
		components = { "stone_boulder" },
		skill = { name="Construction", difficulty=12 },
		render_rex = "stonecutter.xp"
	},

	carpenter = {
		name = "Carpentry Workshop",
		components = { "wood_plank" },
		skill = { name="Construction", difficulty=12 },
		render_rex = "carpenter.xp"
	},

	charcoal_hut = {
		name = "Charcoal Burner",
		components = { "wood_plank" },
		skill = { name="Construction", difficulty=12 },
		render_rex = "charcoal_hut.xp"
	},

	smelter = {
		name = "Smelter",
		components = { "stone_block" },
		skill = { name="Construction", difficulty=12 },
		render_rex = "smelter.xp"
	},

	glass_furnace = {
		name = "Glass Furnace",
		components = { "stone_block" },
		skill = { name="Construction", difficulty=12 },
		render_rex = "glass_furnace.xp"
	},

	silicon_refinery = {
		name = "Silicon Refinery",
		components = { "stone_block", "raw_glass" },
		skill = { name="Construction", difficulty=12 },
		render_rex = "silicon_refinery.xp"
	},

	primitive_forge = {
		name = "Primitive Forge",
		components = { "stone_block" },
		skill = { name="Construction", difficulty=15 },
		render_rex = "primitive_forge.xp"
	},

	primitive_workshop = {
		name = "Primitive Workshop",
		components = { "stone_block" },
		skill = { name="Construction", difficulty=15 },
		render_rex = "primitive_workshop.xp"
	},

	mason = {
		name = "Masonry",
		components = { "stone_block" },
		skill = { name="Construction", difficulty=12 },
		render_rex = "mason.xp"
	},

	stone_table = {
		name = "Stone Table",
		components = { "stone_table" },
		skill = { name="Construction", difficulty=5 },
		provides = { table={energy_cost=0} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['table'], foreground = colors['white'], background = colors['black']}
			}
		}
	},

	wood_table = {
		name = "Wooden Table",
		components = { "wood_table" },
		skill = { name="Construction", difficulty=5 },
		provides = { table={energy_cost=0} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']}
			}
		}
	},

	stone_chair = {
		name = "Stone Chair",
		components = { "stone_chair" },
		skill = { name="Construction", difficulty=5 },
		provides = { table={energy_cost=0} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['chair'], foreground = colors['white'], background = colors['black']}
			}
		}
	},

	stone_door = {
		name = "Stone Dooor",
		components = { "stone_door" },
		skill = { name="Construction", difficulty=10 },
		provides = { door={energy_cost=0} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['door'], foreground = colors['white'], background = colors['black']}
			}
		}
	},

	wood_chair = {
		name = "Wooden Chair",
		components = { "wood_chair" },
		skill = { name="Construction", difficulty=5 },
		provides = { table={energy_cost=0} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['chair'], foreground = colors['wood_brown'], background = colors['black']}
			}
		}
	},

	wood_door = {
		name = "Wooden Door",
		components = { "wood_door" },
		skill = { name="Construction", difficulty=10 },
		provides = { door={energy_cost=0} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['door'], foreground = colors['wood_brown'], background = colors['black']}
			}
		}
	},

	wood_wall = {
		name = "Wooden Wall", components = { "wood_plank" }, skill = { name="Construction", difficulty=10 },
		provides = { wall={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['solid_square'], foreground = colors['wood_brown'], background = colors['black']} } }
	},
	wood_floor = {
		name = "Wooden Floor", components = { "wood_plank" }, skill = { name="Construction", difficulty=10 },
		provides = { floor={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['comma'], foreground = colors['wood_brown'], background = colors['black']} } }
	},
	wood_stairs_up = {
		name = "Wooden Up Stairs", components = { "wood_plank" }, skill = { name="Construction", difficulty=10 },
		provides = { stairs_up={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['less_than'], foreground = colors['wood_brown'], background = colors['black']} } }
	},
	wood_stairs_down = {
		name = "Wooden Down Stairs", components = { "wood_plank" }, skill = { name="Construction", difficulty=10 },
		provides = { stairs_down={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['greater_than'], foreground = colors['wood_brown'], background = colors['black']} } }
	},
	wood_stairs_updown = {
		name = "Wooden Spiral Stairs", components = { "wood_plank" }, skill = { name="Construction", difficulty=10 },
		provides = { stairs_updown={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['X'], foreground = colors['wood_brown'], background = colors['black']} } }
	},
	wood_ramp = {
		name = "Wooden Ramp", components = { "wood_plank" }, skill = { name="Construction", difficulty=10 },
		provides = { ramp={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['ramp_up'], foreground = colors['wood_brown'], background = colors['black']} } }
	},

	stone_wall = {
		name = "Stone Wall", components = { "stone_block" }, skill = { name="Construction", difficulty=10 },
		provides = { wall={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['solid_square'], foreground = colors['white'], background = colors['black']}	} }
	},
	stone_floor = {
		name = "Stone Floor", components = { "stone_block" }, skill = { name="Construction", difficulty=10 },
		provides = { floor={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['comma'], foreground = colors['wood_brown'], background = colors['black']} } }
	},
	stone_stairs_up = {
		name = "Stone Up Stairs", components = { "stone_block" }, skill = { name="Construction", difficulty=10 },
		provides = { stairs_up={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['less_than'], foreground = colors['wood_brown'], background = colors['black']} } }
	},
	stone_stairs_down = {
		name = "Stone Down Stairs", components = { "stone_block" }, skill = { name="Construction", difficulty=10 },
		provides = { stairs_down={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['greater_than'], foreground = colors['wood_brown'], background = colors['black']} } }
	},
	stone_stairs_updown = {
		name = "Stone Spiral Stairs", components = { "stone_block" }, skill = { name="Construction", difficulty=10 },
		provides = { stairs_updown={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['X'], foreground = colors['wood_brown'], background = colors['black']} } }
	},
	stone_ramp = {
		name = "Stone Ramp", components = { "stone_block" }, skill = { name="Construction", difficulty=10 },
		provides = { ramp={energy_cost=0} }, render = {	width=1, height=1, tiles= {	{glyph= glyphs['ramp_up'], foreground = colors['wood_brown'], background = colors['black']} } }
	}
}