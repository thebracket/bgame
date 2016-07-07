buildings = {
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
		provides = { food={energy_cost=5} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['sun'], foreground = colors['orange'], background = colors['yellow']}
			}
		}
	},

	sawmill = {
		name = "Sawmill",
		components = { "wood_log" },
		skill = { name="Construction", difficulty=12 },
		render = {
			width=3, height=3, tiles= {
				{glyph= glyphs['one_quarter_solid'], foreground = colors['wood_brown'], background = colors['black']},
				{glyph= glyphs['three_bar'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['one_quarter_solid'], foreground = colors['wood_brown'], background = colors['black']},

				{glyph= glyphs['three_bar'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['sun'], foreground = colors['icy_blue'], background = colors['black']},
				{glyph= glyphs['three_bar'], foreground = colors['grey'], background = colors['black']},

				{glyph= glyphs['one_quarter_solid'], foreground = colors['wood_brown'], background = colors['black']},
				{glyph= glyphs['three_bar'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['one_quarter_solid'], foreground = colors['wood_brown'], background = colors['black']}
			}
		}
	},

	stonecutter = {
		name = "Stonecutters",
		components = { "stone_boulder" },
		skill = { name="Construction", difficulty=12 },
		render = {
			width=3, height=3, tiles= {
				{glyph= glyphs['three_bar'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['one_quarter_solid'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['three_bar'], foreground = colors['grey'], background = colors['black']},

				{glyph= glyphs['one_quarter_solid'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['sun'], foreground = colors['icy_blue'], background = colors['black']},
				{glyph= glyphs['one_quarter_solid'], foreground = colors['grey'], background = colors['black']},

				{glyph= glyphs['three_bar'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['one_quarter_solid'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['three_bar'], foreground = colors['grey'], background = colors['black']}
			}
		}
	},

	carpenter = {
		name = "Carpentry Workshop",
		components = { "wood_plank" },
		skill = { name="Construction", difficulty=12 },
		render = {
			width=3, height=3, tiles= {
				{glyph= glyphs['one_quarter_solid'], foreground = colors['wood_brown'], background = colors['black']},
				{glyph= glyphs['three_bar'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['solid_square'], foreground = colors['wood_brown'], background = colors['black']},

				{glyph= glyphs['three_bar'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['sun'], foreground = colors['icy_blue'], background = colors['black']},
				{glyph= glyphs['three_bar'], foreground = colors['grey'], background = colors['black']},

				{glyph= glyphs['solid_square'], foreground = colors['wood_brown'], background = colors['black']},
				{glyph= glyphs['three_bar'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['solid_square'], foreground = colors['wood_brown'], background = colors['black']}
			}
		}
	},

	mason = {
		name = "Masonry",
		components = { "stone_block" },
		skill = { name="Construction", difficulty=12 },
		render = {
			width=3, height=3, tiles= {
				{glyph= glyphs['three_bar'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['solid_square'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['three_bar'], foreground = colors['grey'], background = colors['black']},

				{glyph= glyphs['solid_square'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['sun'], foreground = colors['white'], background = colors['black']},
				{glyph= glyphs['solid_square'], foreground = colors['grey'], background = colors['black']},

				{glyph= glyphs['three_bar'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['solid_square'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['three_bar'], foreground = colors['grey'], background = colors['black']}
			}
		}
	},

	stone_table = {
		name = "Stone Table",
		components = { "stone_table" },
		skill = { name="Construction", difficulty=5 },
		provides = { table={energy_cost=0} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['table'], foreground = colors['grey'], background = colors['black']}
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
	}

	,

	stone_chair = {
		name = "Stone Chair",
		components = { "stone_chair" },
		skill = { name="Construction", difficulty=5 },
		provides = { table={energy_cost=0} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['chair'], foreground = colors['grey'], background = colors['black']}
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
	}
}