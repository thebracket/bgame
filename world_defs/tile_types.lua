tile_types = {
	nothing = { index = 0, glyph = glyphs['solid_square'], render=false, foreground=colors['black'], background=colors['white'], name = "Nothing" },
	semi_molten_rock = { index = 1, glyph = glyphs['three_quarters_solid'], render=true, foreground=colors['red'], background=colors['yellow'], name="Semi-Molten Rock" },
	rock_default = { index = 2, glyph = glyphs['solid_square'], render=true, foreground=colors['grey'], background=colors['black'], name="Rock" },
	soil_default = { index = 3, glyph = glyphs['tilde'], render=true, foreground=colors['earth_brown'], background=colors['black'], name="Soil" },
	yellow_sand = { index = 4, glyph = glyphs['double_wave'], render=true, foreground=colors['yellow_sand'], background=colors['black'], name="Yellow Sand" },
	red_sand = { index = 5, glyph = glyphs['double_wave'], render=true, foreground=colors['red_sand'], background=colors['black'], name="Red Sand" },
	white_sand = { index = 6, glyph = glyphs['double_wave'], render=true, foreground=colors['white_sand'], background=colors['black'], name="White Sand" },
	tree_trunk = { index = 7, glyph = glyphs['double_circle'], foreground = colors['wood_brown'], background = colors['black'], name="Tree Trunk" },
	tree_leaves = { index = 8, glyph = glyphs['one_quarter_solid'], foreground = colors['tree_green'], background = colors['black'], name="Tree Foliage" },
}
