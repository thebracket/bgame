buildings = {
	cordex = {
		name = "Cordex",
		description = "This is you, the ship-borne AI trying to keep the motley crew of meat-bag morons from your ship alive.",
		components = { { item="cordex_core", qty=1} },
		skill = { name="Construction", difficulty=25 },
		render_rex = "cordex.xp",
        vox=voxelId("cordex")
	},
	lamp = {
		name = "Lamp",
		description = "A simple lamp, designed to provide light.",
		components = { { item="lamp_kit", qty=1} },
		skill = { name="Construction", difficulty=25 },
		render = { width=1, height=1, tiles={ {glyph=glyphs['sun'], foreground = colors['yellow'], background = colors['black']} } },
		render_ascii = { width=1, height=1, tiles={ {glyph=glyphs['sun'], foreground = colors['yellow'], background = colors['black'] } } },
		provides = { light={radius=5, color = colors['white']} },
        vox=voxelId("floorlight")
	},
	ship_defense_turret = {
		name = "Ship Defense Turret",
		description = "An automated ship defense turret, designed to deter critters and blackguards from entering the ship without permission. It has an angry personality.",
	    components = { { item="ship_turret_kit", qty=1 } },
	    skill = { name="Construction", difficulty=25 },
	    render = { width=1, height=1, tiles={ {glyph=glyphs['greater_than_equal'], foreground = colors['red'], background = colors['black']} } },
		render_ascii = { width=1, height=1, tiles={ {glyph=glyphs['greater_than_equal'], foreground = colors['red'], background = colors['black'] } } },
		vox=voxelId("turret")
	},
	wall = {
		name = "Wall", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 }, structure=true,
		provides = { wall={energy_cost=0} },
        render = {	width=1, height=1, tiles= {	{glyph= glyphs['solid_square'], foreground = colors['white'], background = colors['black']}	} },
        render_ascii = {	width=1, height=1, tiles= {	{glyph= glyphs['solid_square'], foreground = colors['white'], background = colors['black']}	} }
	},
	floor = {
		name = "Floor", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 }, structure=true,
		provides = { floor={energy_cost=0} },
        render = {	width=1, height=1, tiles= {	{glyph= glyphs['comma'], foreground = colors['wood_brown'], background = colors['black']} } },
        render_ascii = {	width=1, height=1, tiles= {	{glyph= glyphs['comma'], foreground = colors['wood_brown'], background = colors['black']} } }
	},
	stairs_up = {
		name = "Up Stairs", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 },
		provides = { stairs_up={energy_cost=0} },
        render = {	width=1, height=1, tiles= {	{glyph= glyphs['less_than'], foreground = colors['wood_brown'], background = colors['black']} } },
		render_ascii = {	width=1, height=1, tiles= {	{glyph= glyphs['less_than'], foreground = colors['wood_brown'], background = colors['black']} } },
		vox=voxelId("stairs_up")		
	},
	stairs_down = {
		name = "Down Stairs", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 },
		provides = { stairs_down={energy_cost=0} },
        render = {	width=1, height=1, tiles= {	{glyph= glyphs['greater_than'], foreground = colors['wood_brown'], background = colors['black']} } },
		render_ascii = {	width=1, height=1, tiles= {	{glyph= glyphs['greater_than'], foreground = colors['wood_brown'], background = colors['black']} } },
		vox=voxelId("stairs_down")
	},
	stairs_updown = {
		name = "Spiral Stairs", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 },
		provides = { stairs_updown={energy_cost=0} },
        render = {	width=1, height=1, tiles= {	{glyph= glyphs['X'], foreground = colors['wood_brown'], background = colors['black']} } },
		render_ascii = {	width=1, height=1, tiles= {	{glyph= glyphs['X'], foreground = colors['wood_brown'], background = colors['black']} } },
		vox=voxelId("stairs_updown")
	},
	ramp = {
		name = "Ramp", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 }, structure=true,
		provides = { ramp={energy_cost=0} },
        render = {	width=1, height=1, tiles= {	{glyph= glyphs['ramp_up'], foreground = colors['wood_brown'], background = colors['black']} } },
        render_ascii = {	width=1, height=1, tiles= {	{glyph= glyphs['ramp_up'], foreground = colors['wood_brown'], background = colors['black']} } }
	},
	bridge = {
		name = "Bridge", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 }, structure=true,
		provides = { ramp={energy_cost=0} },
        render = {	width=1, height=1, tiles= {	{glyph= glyphs['comma'], foreground = colors['wood_brown'], background = colors['black']} } },
        render_ascii = {	width=1, height=1, tiles= {	{glyph= glyphs['comma'], foreground = colors['wood_brown'], background = colors['black']} } }
	}
}