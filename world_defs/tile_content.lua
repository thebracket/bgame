tile_content = {
	nothing = { index = 0 },

	permafrost_white = { index = 1, glyph = glyphs['three_quarters_solid'], foreground = colors['white'], background = colors['black'], name="White Permafrost" },
	permafrost_blue = { index = 2, glyph = glyphs['three_quarters_solid'], foreground = colors['icy_blue'], background = colors['black'], name="Blue Permafrost" },
	permafrost_cracked = { index = 3, glyph = glyphs['one_quarter_solid'], foreground = colors['icy_blue'], background = colors['black'], name="Cracked Permafrost" },
	permafrost_dirty = { index = 4, glyph = glyphs['three_quarters_solid'], foreground = colors['grey'], background = colors['black'], name="Dirty Permafrost" },

	yellow_sand = { index = 5, glyph = glyphs['one_quarter_solid'], foreground = colors['yellow_sand'], background = colors['black'], name="Yellow Sand" },
	yellow_sand_cracked = { index = 6, glyph = glyphs['percent'], foreground = colors['yellow_sand'], background = colors['black'], name="Cracked Yellow Sand" },
	yellow_sand_dirty = { index = 7, glyph = glyphs['one_half_solid'], foreground = colors['yellow_sand'], background = colors['black'], name="Dirty Yellow Sand" },
	yellow_sand_ripples = { index = 8, glyph = glyphs['double_wave'], foreground = colors['yellow_sand'], background = colors['black'], name="Rippled Yellow Sand" },

	red_sand = { index = 9, glyph = glyphs['one_quarter_solid'], foreground = colors['red_sand'], background = colors['black'], name="Red Sand" },
	red_sand_cracked = { index = 10, glyph = glyphs['percent'], foreground = colors['red_sand'], background = colors['black'], name="Cracked Red Sand" },
	red_sand_dirty = { index = 11, glyph = glyphs['one_half_solid'], foreground = colors['red_sand'], background = colors['black'], name="Dirty Red Sand" },
	red_sand_ripples = { index = 12, glyph = glyphs['double_wave'], foreground = colors['red_sand'], background = colors['black'], name="Rippled Red Sand" },

	white_sand = { index = 13, glyph = glyphs['one_quarter_solid'], foreground = colors['white_sand'], background = colors['black'], name="White Sand" },
	white_sand_cracked = { index = 14, glyph = glyphs['percent'], foreground = colors['white_sand'], background = colors['black'], name="Cracked White Sand" },
	white_sand_dirty = { index = 15, glyph = glyphs['one_half_solid'], foreground = colors['white_sand'], background = colors['black'], name="Dirty White Sand" },
	white_sand_ripples = { index = 16, glyph = glyphs['double_wave'], foreground = colors['white_sand'], background = colors['black'], name="Rippled White Sand" },

	pebbles = { index = 17, glyph = glyphs['one_quarter_solid'], foreground = colors['grey'], background = colors['black'], name="Pebbles" },
	gravel = { index = 18, glyph = glyphs['one_half_solid'], foreground = colors['grey'], background = colors['black'], name="Gravel" },
	mud = { index = 19, glyph = glyphs['three_quarters_solid'], foreground = colors['earth_brown'], background = colors['black'], name="Mud" },

	lychen = { index = 1024, glyph = glyphs['percent'], foreground = colors['lychen'], background = colors['black'], name="Lychen" },
	moss = { index = 1025, glyph = glyphs['three_quarters_solid'], foreground = colors['moss'], background = colors['black'], name="Moss" },
	cactus = { index = 1026, glyph = glyphs['sun'], foreground = colors['cactus'], background = colors['black'], name="Cactus" },
	grass = { index = 1027, glyph = glyphs['one_half_solid'], foreground = colors['grass_green'], background = colors['black'], name="Grass" },
	grass_sparse = { index = 1028, glyph = glyphs['one_quarter_solid'], foreground = colors['grass_green'], background = colors['black'], name="Sparse Grass" },
	reeds = { index = 1029, glyph = glyphs['percent'], foreground = colors['grass_green'], background = colors['black'], name="Reeds" },
	gorse = { index = 1030, glyph = glyphs['quote'], foreground = colors['grass_green'], background = colors['black'], name="Gorse" },
	wildflower = { index = 1031, glyph = glyphs['quote'], foreground = colors['yellow'], background = colors['black'], name="Wild Flowers" },
	thistle = { index = 1032, glyph = glyphs['quote'], foreground = colors['grass_green'], background = colors['black'], name="Thistle" },
	shrub = { index = 1033, glyph = glyphs['sun'], foreground = colors['grass_green'], background = colors['black'], name="Shrub" },
	heather = { index = 1034, glyph = glyphs['sun'], foreground = colors['heather'], background = colors['black'], name="Heather" },

	cordex = { index = 4096, glyph = glyphs['sun'], foreground = colors['cyan'], background = colors['black'], name="Cordex" },
	scanner_console = { index = 4097, glyph = glyphs['ramp_left'], foreground = colors['cyan'], background = colors['black'], name="Scanner Console" },
	defense_console = { index = 4098, glyph = glyphs['ramp_right'], foreground = colors['cyan'], background = colors['black'], name="Defense Console" },
	education_console = { index = 4099, glyph = glyphs['ramp_up'], foreground = colors['cyan'], background = colors['black'], name = "Education Console" },
	communications_console = { index = 4100, glyph = glyphs['ramp_down'], foreground = colors['cyan'], background = colors['black'], name="Comms Console" },
	ship_floor = { index = 4101, glyph = glyphs['one_quarter_solid'], foreground = colors['grey'], background = colors['dark_grey'], name="Ship Floor" },
	ship_wall_ew = { index = 4102, glyph = glyphs['double_wall_ew'], foreground = colors['white'], background = colors['black'], name="Ship Wall" },
	ship_wall_ns = { index = 4103, glyph = glyphs['double_wall_ns'], foreground = colors['white'], background = colors['black'], name="Ship Wall" },
	ship_wall_se = { index = 4104, glyph = glyphs['double_wall_se'], foreground = colors['white'], background = colors['black'], name="Ship Wall" },
	ship_wall_ne = { index = 4105, glyph = glyphs['double_wall_ne'], foreground = colors['white'], background = colors['black'], name="Ship Wall" },
	ship_wall_nw = { index = 4106, glyph = glyphs['double_wall_nw'], foreground = colors['white'], background = colors['black'], name="Ship Wall" },
	ship_wall_sw = { index = 4107, glyph = glyphs['double_wall_sw'], foreground = colors['white'], background = colors['black'], name="Ship Wall" },
	ship_wall = { index = 4108, glyph = glyphs['solid_square'], foreground = colors['white'], background = colors['black'], name="Ship Wall" },

	ship_updown = { index = 4109, glyph = glyphs['X'], foreground = colors['white'], background = colors['black'], stairs='updown', name="Up/Down Stairs"},
	ship_down = { index = 4110, glyph = glyphs['greater_than'], foreground = colors['white'], background = colors['black'], stairs='down', name="Down Stairs" },
	ship_up = { index = 4111, glyph = glyphs['less_than'], foreground = colors['white'], background = colors['black'], stairs='up', name='Up Stairs' },
	ship_ramp = { index = 4118, glyph = glyphs['ramp_up'], foreground = colors['white'], background = colors['black'], stairs='up', name='Up Ramp' },

	solar_panel = { index = 4112, glyph = glyphs['one_quarter_solid'], foreground = colors['yellow'], background = colors['cyan'], name="Solar Panel" },
	cryo_bed = { index = 4113, glyph = glyphs['zero'], foreground = colors['white'], background = colors['black'], name="Cryo Bed" },
	storage_locker = { index = 4114, glyph = glyphs['infinity'], foreground = colors['white'], background = colors['black'], name="Storage Locker" },
	battery = { index = 4115, glyph = glyphs['plus_minus'], foreground = colors['white'], background = colors['black'], name="Battery Bank" },
	rtg = { index = 4116, glyph = glyphs['double_pillar'], foreground = colors['orange'], background = colors['black'], name="RTG" },
	replicator = { index = 4117, glyph = glyphs['sigma'], foreground = colors['magenta'], background = colors['black'], name="Replicator" }

}
