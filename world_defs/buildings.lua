buildings = {
	personal_survival_shelter = {
		name = "Tent",
		components = { "personal_survival_shelter_kit" },
		skill = { name="construction", difficulty=5 },
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
		skill = { name="construction", difficulty=5 },
		provides = { food={energy_cost=5} },
		render = {
			width=1, height=1, tiles= {
				{glyph= glyphs['sun'], foreground = colors['orange'], background = colors['yellow']}
			}
		}
	}
}