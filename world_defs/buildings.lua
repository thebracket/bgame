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
				{glyph= glyphs['one_quarter_solid'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['one_quarter_solid'], foreground = colors['wood_brown'], background = colors['black']},

				{glyph= glyphs['one_quarter_solid'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['sun'], foreground = colors['icy_blue'], background = colors['black']},
				{glyph= glyphs['one_quarter_solid'], foreground = colors['grey'], background = colors['black']},

				{glyph= glyphs['one_quarter_solid'], foreground = colors['wood_brown'], background = colors['black']},
				{glyph= glyphs['one_quarter_solid'], foreground = colors['grey'], background = colors['black']},
				{glyph= glyphs['one_quarter_solid'], foreground = colors['wood_brown'], background = colors['black']}
			}
		}
	}
}