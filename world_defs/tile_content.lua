print('Setting up tile contents')
tile_content = {
	nothing = { index = 0 },

	permafrost_white = { index = 1, glyph = glyphs['three_quarters_solid'], foreground = colors['white'], background = colors['black'] },
	permafrost_blue = { index = 2, glyph = glyphs['three_quarters_solid'], foreground = colors['icy_blue'], background = colors['black'] },
	permafrost_cracked = { index = 3, glyph = glyphs['one_quarter_solid'], foreground = colors['icy_blue'], background = colors['black'] },
	permafrost_dirty = { index = 4, glyph = glyphs['three_quarters_solid'], foreground = colors['grey'], background = colors['black'] },

	yellow_sand = { index = 5, glyph = glyphs['one_quarter_solid'], foreground = colors['yellow_sand'], background = colors['black'] },
	yellow_sand_cracked = { index = 6, glyph = glyphs['percent'], foreground = colors['yellow_sand'], background = colors['black'] },
	yellow_sand_dirty = { index = 7, glyph = glyphs['one_half_solid'], foreground = colors['yellow_sand'], background = colors['black'] },
	yellow_sand_ripples = { index = 8, glyph = glyphs['double_wave'], foreground = colors['yellow_sand'], background = colors['black'] },

	red_sand = { index = 9, glyph = glyphs['one_quarter_solid'], foreground = colors['red_sand'], background = colors['black'] },
	red_sand_cracked = { index = 10, glyph = glyphs['percent'], foreground = colors['red_sand'], background = colors['black'] },
	red_sand_dirty = { index = 11, glyph = glyphs['one_half_solid'], foreground = colors['red_sand'], background = colors['black'] },
	red_sand_ripples = { index = 12, glyph = glyphs['double_wave'], foreground = colors['red_sand'], background = colors['black'] },

	white_sand = { index = 13, glyph = glyphs['one_quarter_solid'], foreground = colors['white_sand'], background = colors['black'] },
	white_sand_cracked = { index = 14, glyph = glyphs['percent'], foreground = colors['white_sand'], background = colors['black'] },
	white_sand_dirty = { index = 15, glyph = glyphs['one_half_solid'], foreground = colors['white_sand'], background = colors['black'] },
	white_sand_ripples = { index = 16, glyph = glyphs['double_wave'], foreground = colors['white_sand'], background = colors['black'] },

	pebbles = { index = 17, glyph = glyphs['one_quarter_solid'], foreground = colors['grey'], background = colors['black'] },
	gravel = { index = 18, glyph = glyphs['one_half_solid'], foreground = colors['grey'], background = colors['black'] },
	mud = { index = 19, glyph = glyphs['three_quarters_solid'], foreground = colors['earth_brown'], background = colors['black'] },

	lychen = { index = 1024, glyph = glyphs['percent'], foreground = colors['lychen'], background = colors['black'] },
	moss = { index = 1025, glyph = glyphs['three_quarters_solid'], foreground = colors['moss'], background = colors['black'] },
	cactus = { index = 1026, glyph = glyphs['sun'], foreground = colors['cactus'], background = colors['black'] },
	grass = { index = 1027, glyph = glyphs['one_half_solid'], foreground = colors['grass_green'], background = colors['black'] },
	grass_sparse = { index = 1028, glyph = glyphs['one_quarter_solid'], foreground = colors['grass_green'], background = colors['black'] },
	reeds = { index = 1029, glyph = glyphs['percent'], foreground = colors['grass_green'], background = colors['black'] },
	gorse = { index = 1030, glyph = glyphs['quote'], foreground = colors['grass_green'], background = colors['black'] },
	wildflower = { index = 1031, glyph = glyphs['quote'], foreground = colors['yellow'], background = colors['black'] },
	thistle = { index = 1032, glyph = glyphs['quote'], foreground = colors['grass_green'], background = colors['black'] },
	shrub = { index = 1033, glyph = glyphs['sun'], foreground = colors['grass_green'], background = colors['black'] },
	heather = { index = 1034, glyph = glyphs['sun'], foreground = colors['heather'], background = colors['black'] },

	cordex = { index = 4096, glyph = glyphs['sun'], foreground = colors['cyan'], background = colors['black'] },
	scanner_console = { index = 4097, glyph = glyphs['ramp-left'], foreground = colors['cyan'], background = colors['black'] },
	defense_console = { index = 4098, glyph = glyphs['ramp-right'], foreground = colors['cyan'], background = colors['black'] },
	education_console = { index = 4099, glyph = glyphs['ramp-up'], foreground = colors['cyan'], background = colors['black'] },
	communications_console = { index = 4100, glyph = glyphs['ramp-down'], foreground = colors['cyan'], background = colors['black'] },
	ship_floor = { index = 4101, glyph = glyphs['one_quarter_solid'], foreground = colors['grey'], background = colors['dark_grey'] },
	ship_wall_ew = { index = 4102, glyph = glyphs['double_wall_ew'], foreground = colors['white'], background = colors['black'] },
	ship_wall_ns = { index = 4103, glyph = glyphs['double_wall_ns'], foreground = colors['white'], background = colors['black'] },
	ship_wall_se = { index = 4104, glyph = glyphs['double_wall_se'], foreground = colors['white'], background = colors['black'] },
	ship_wall_ne = { index = 4105, glyph = glyphs['double_wall_ne'], foreground = colors['white'], background = colors['black'] },
	ship_wall_nw = { index = 4106, glyph = glyphs['double_wall_nw'], foreground = colors['white'], background = colors['black'] },
	ship_wall_sw = { index = 4107, glyph = glyphs['double_wall_sw'], foreground = colors['white'], background = colors['black'] },
	ship_wall = { index = 4108, glyph = glyphs['solid_square'], foreground = colors['white'], background = colors['black'] },
	ship_updown = { index = 4109, glyph = glyphs['X'], foreground = colors['white'], background = colors['black']},
	ship_down = { index = 4110, glyph = glyphs['less_than'], foreground = colors['white'], background = colors['black'] },
	ship_up = { index = 4111, glyph = glyphs['greater_than'], foreground = colors['white'], background = colors['black'] },
	solar_panel = { index = 4112, glyph = glyphs['one_quarter_solid'], foreground = colors['yellow'], background = colors['cyan'] },
	cryo_bed = { index = 4113, glyph = glyphs['zero'], foreground = colors['white'], background = colors['black'] },
	storage_locker = { index = 4114, glyph = glyphs['infinity'], foreground = colors['white'], background = colors['black'] },
	battery = { index = 4115, glyph = glyphs['plus_minus'], foreground = colors['white'], background = colors['black'] },
	rtg = { index = 4116, glyph = glyphs['double_pillar'], foreground = colors['orange'], background = colors['black'] },
	replicator = { index = 4117, glyph = glyphs['sigma'], foreground = colors['magenta'], background = colors['black'] }

}
