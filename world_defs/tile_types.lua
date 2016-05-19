print('Setting up tile types')
tile_types = {
	nothing = { index = 0, glyph = glyphs['solid_square'], render=false, foreground=colors['black'], background=colors['white'] },
	semi_molten_rock = { index = 1, glyph = glyphs['three_quarters_solid'], render=true, foreground=colors['red'], background=colors['yellow'] },
	rock_default = { index = 2, glyph = glyphs['solid_square'], render=true, foreground=colors['grey'], background=colors['black'] },
	soil_default = { index = 3, glyph = glyphs['one_quarter_solid'], render=true, foreground=colors['earth_brown'], background=colors['black'] },
	yellow_sand = { index = 4, glyph = glyphs['one_quarter_solid'], render=true, foreground=colors['yellow_sand'], background=colors['black'] },
	red_sand = { index = 5, glyph = glyphs['one_quarter_solid'], render=true, foreground=colors['red_sand'], background=colors['black'] },
	white_sand = { index = 6, glyph = glyphs['one_quarter_solid'], render=true, foreground=colors['white_sand'], background=colors['black'] }
}
