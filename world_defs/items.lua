items = {
	personal_survival_shelter_kit = {
		name = "Personal Survival Shelter Kit",
		description = "A folded-up personal shelter kit, known in non-corporate parlance as a camping set: a tent, bedroll and sleeping bag.",
		itemtype = {"component"},
		glyph = glyphs['kit'],
		foreground = colors['green'],
		background = colors['black']
	},
	camp_fire_kit = {
		name = "Camping Fire Kit",
		description = "A plastic campfire, powered by a tiny nuclear reactor. Ideal for your off-world vacation.",
		itemtype = {"component"},
		glyph = glyphs['kit'],
		foreground = colors['red'],
		background = colors['black']
	},
	fire_axe = {
		name = "Fire Axe",
		description = "A simple axe. It has the word FIRE burned into it.",
		itemtype = {"tool-chopping"},
		glyph = glyphs['paragraph'],
		foreground = colors['white'],
		background = colors['black']
	},
	battleaxe = {
		name = "Battleaxe",
		description = "An intimidating looking axe with a large blade.",
		itemtype = {"tool-chopping", "weapon-melee"},
		glyph = glyphs['paragraph'],
		foreground = colors['white'],
		background = colors['black'],
		damage_n=1, damage_d=8, damage_mod=0, damage_stat="str"
	},
	pickaxe = {
		name = "Pickaxe",
		description = "A simple pickaxe, ideal for digging holes.",
		itemtype = {"tool-digging"},
		glyph = glyphs['pick'],
		foreground = colors['white'],
		background = colors['black']
	},
	wood_log = {
		name = "Wood Log",
		description = "Wood from a felled tree",
		itemtype = {"component"},
		glyph = glyphs['double_wall_ew'],
		foreground = colors['wood_brown'],
		background = colors['black']
	},
	stone_boulder = {
		name = "Stone Boulder",	description = "A large stone boulder",
		itemtype = {"component"}, glyph = glyphs['small_circle'], foreground = colors['grey'], background = colors['black']
	},
	aluminium_ore = {
		name = "Aluminium Ore", description = "A lump of unrefined aluminium",
		itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['aluminium_blue'], background = colors['black']
	},
	aluminium_bar = {
		name = "Aluminium Bars", description = "Aluminium Bars",
		itemtype = {"component"}, glyph = glyphs['three_bar'], foreground = colors['aluminium_blue'], background = colors['black']
	},
	bismuth_ore = {
		name = "Bismuth Ore", description = "A lump of unrefined bismuth",
		itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['bismuth_yellow'], background = colors['black']
	},
	bronze_bar = {
		name = "Bronze Bars", description = "Bronze Bars",
		itemtype = {"component"}, glyph = glyphs['three_bar'], foreground = colors['bismuth_yellow'], background = colors['black']
	},
	tin_ore = {
		name = "Tin Ore", description = "A lump of unrefined tin",
		itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['tin_blue'], background = colors['black']
	},
	tin_bar = {
		name = "Tin Bars", description = "Tin Bars",
		itemtype = {"component"}, glyph = glyphs['three_bar'], foreground = colors['tin_blue'], background = colors['black']
	},
	chromium_ore = {
		name = "Chromium Ore", description = "A lump of unrefined chromium",
		itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['chromium_blue'], background = colors['black']
	},
	chromium_bar = {
		name = "Chromium Bars", description = "Chromium Bars",
		itemtype = {"component"}, glyph = glyphs['three_bar'], foreground = colors['chromium_blue'], background = colors['black']
	},
	mercury_ore = {
		name = "Mercury Ore", description = "A lump of unrefined mercury",
		itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['grey'], background = colors['black']
	},
	copper_ore = {
		name = "Copper Ore", description = "A lump of unrefined copper",
		itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['copper'], background = colors['black']
	},
	copper_bar = {
		name = "Copper Bars", description = "Copper Bars",
		itemtype = {"component"}, glyph = glyphs['three_bar'], foreground = colors['copper'], background = colors['black']
	},
	lead_ore = {
		name = "Lead Ore", description = "A lump of unrefined lead",
		itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['grey'], background = colors['black']
	},
	lead_bar = {
		name = "Lead Bars", description = "Lead Bars",
		itemtype = {"component"}, glyph = glyphs['three_bar'], foreground = colors['grey'], background = colors['black']
	},
	gold_ore = {
		name = "Gold Ore", description = "A lump of unrefined gold",
		itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['yellow'], background = colors['black']
	},
	gold_bar = {
		name = "Gold Bars", description = "Gold Bars",
		itemtype = {"component"}, glyph = glyphs['three_bar'], foreground = colors['yellow'], background = colors['black']
	},
	iron_ore = {
		name = "Iron Ore", description = "A lump of unrefined iron",
		itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['dark_red'], background = colors['black']
	},
	iron_bar = {
		name = "Iron Bars", description = "Iron Bars",
		itemtype = {"component"}, glyph = glyphs['three_bar'], foreground = colors['grey'], background = colors['black']
	},
	uranium_ore = {
		name = "Uranium Ore", description = "A lump of unrefined uranium",
		itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['green'], background = colors['black']
	},
	platinum_ore = {
		name = "Platinum Ore", description = "A lump of unrefined platinum",
		itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['white'], background = colors['black']
	},
	platinum_bar = {
		name = "Platinum Bars", description = "Platinum Bars",
		itemtype = {"component"}, glyph = glyphs['three_bar'], foreground = colors['white'], background = colors['black']
	},
	silver_ore = {
		name = "Silver Ore", description = "A lump of unrefined silver",
		itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['white'], background = colors['black']
	},
	silver_bar = {
		name = "Silver Bars", description = "Silver Bars",
		itemtype = {"component"}, glyph = glyphs['three_bar'], foreground = colors['white'], background = colors['black']
	},
	zinc_ore = {
		name = "Zinc Ore", description = "A lump of unrefined zinc",
		itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['zinc'], background = colors['black']
	},
	zinc_bar = {
		name = "Zinc Bars", description = "Zinc Bars",
		itemtype = {"component"}, glyph = glyphs['three_bar'], foreground = colors['zinc'], background = colors['black']
	},
	saltpeter_ore = {
		name = "Saltpeter Ore", description = "A lump of unrefined saltpeter",
		itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['white'], background = colors['black']
	},
	alabaster_powder = {
		name = "Alabaster Powder", description = "A lump of unrefined alabaster powder",
		itemtype = {"component"}, glyph = glyphs['inset_square'], foreground = colors['white'], background = colors['black']
	},
	gypsum_powder = {
		name = "Gypsum Powder", description = "A lump of unrefined gypsum powder",
		itemtype = {"component"}, glyph = glyphs['inset_square'], foreground = colors['white'], background = colors['black']
	},
	coal = {
		name = "Coal", description = "A lump of unrefined coal",
		itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['dark_grey'], background = colors['black']
	},
	sulphur = {
		name = "Sulphur", description = "A lump of sulphur",
		itemtype = {"component"}, glyph = glyphs['inset_square'], foreground = colors['yellow'], background = colors['black']
	},
	flint = {
		name = "Flint", description = "A piece of flint",
		itemtype = {"component"}, glyph = glyphs['inset_square'], foreground = colors['brown'], background = colors['black']
	},
	sand = {
		name = "Sand", description = "A lump of raw sand",
		itemtype = {"component"}, glyph = glyphs['double_wave'], foreground = colors['yellow'], background = colors['black']
	},
	oil = {
		name = "Oil", description = "A blob of viscous oil",
		itemtype = {"component"}, glyph = glyphs['double_wave'], foreground = colors['dark_grey'], background = colors['black']
	},
	clay = {
		name = "Clay", description = "A blob of clay",
		itemtype = {"component"}, glyph = glyphs['inset_square'], foreground = colors['dark_red'], background = colors['black']
	},
	topsoil = {
		name = "Topsoil", description = "A lump of rich topsoil",
		itemtype = {"component"}, glyph = glyphs['double_wave'], foreground = colors['brown'], background = colors['black']
	},
	wood_plank = {
		name = "Wood Plank",
		description = "Wood, cut into a convenient shape",
		itemtype = {"component"},
		glyph = glyphs['single_wall_ew'],
		foreground = colors['wood_brown'],
		background = colors['black']
	},
	charcoal = {
		name = "Charcoal",
		description = "Wood, processed to burn at high temperature",
		itemtype = {"component"},
		glyph = glyphs['inset_square'],
		foreground = colors['wood_brown'],
		background = colors['black']
	},
	ash = {
		name = "Ash",
		description = "Ash, remnants of burned wood",
		itemtype = {"component"},
		glyph = glyphs['one_quarter_solid'],
		foreground = colors['grey'],
		background = colors['black']
	},
	raw_glass = {
		name = "Raw Glass",
		description = "Raw glass, ready to turn into something useful",
		itemtype = {"component"},
		glyph = glyphs['inset_square'],
		foreground = colors['green'],
		background = colors['black']
	},
	raw_silicon = {
		name = "Raw Silicon",
		description = "Silicon, suitable for use in circuitry and chips",
		itemtype = {"component"},
		glyph = glyphs['inset_square'],
		foreground = colors['green'],
		background = colors['black']
	},
	wood_waste = {
		name = "Wood Offcuts",
		description = "Wood off-cuts",
		itemtype = {"component"},
		glyph = glyphs['three_bar'],
		foreground = colors['wood_brown'],
		background = colors['black']
	},
	stone_block = {
		name = "Stone Block",
		description = "Stone, cut into a convenient building block",
		itemtype = {"component"},
		glyph = glyphs['single_wall_ew'],
		foreground = colors['grey'],
		background = colors['black']
	},
	wood_table = {
		name = "Wooden Table",
		description = "A wooden picnic table",
		itemtype = {"furniture"},
		glyph = glyphs['table'],
		foreground = colors['wood_brown'],
		background = colors['black']
	},
	stone_table = {
		name = "Stone Table",
		description = "A stone picnic table",
		itemtype = {"furniture"},
		glyph = glyphs['table'],
		foreground = colors['grey'],
		background = colors['black']
	},
	wood_chair = {
		name = "Wooden Chair",
		description = "A wooden picnic chair",
		itemtype = {"furniture"},
		glyph = glyphs['chair'],
		foreground = colors['wood_brown'],
		background = colors['black']
	},
	stone_chair = {
		name = "Stone Chair",
		description = "A stone picnic chair",
		itemtype = {"furniture"},
		glyph = glyphs['chair'],
		foreground = colors['grey'],
		background = colors['black']
	},
	wood_door = {
		name = "Wooden Door",
		description = "A simple wooden door",
		itemtype = {"component"},
		glyph = glyphs['door'],
		foreground = colors['wood_brown'],
		background = colors['black']
	},
	stone_door = {
		name = "Stone Door",
		description = "A simple stone door",
		itemtype = {"component"},
		glyph = glyphs['door'],
		foreground = colors['grey'],
		background = colors['black']
	},
	tiny_marshmallow = {
		name = "Tiny Marshmallow",
		description = "A tiny lump of congealed sugar",
		itemtype = {"food"},
		glyph = glyphs['squared'],
		foreground = colors['magenta'],
		background = colors['black']
	},
	tea_replicated = {
		name = "Tea, Earl-Grey Lukewarm",
		description = "A replicated substance that tastes somewhat like tea",
		itemtype = {"drink"},
		glyph = glyphs['squared'],
		foreground = colors['wood_brown'],
		background = colors['black']
	},
	sandwich_replicated = {
		name = "Stale-tasting sandwich",
		description = "A replicated sandwich that sort-of tastes like chicken",
		itemtype = {"food"},
		glyph = glyphs['squared'],
		foreground = colors['wood_brown'],
		background = colors['black']
	},
	club = {
		name = "Club",
		description = "A simple club",
		itemtype = {"weapon-melee"},
		glyph = glyphs['axe'],
		foreground = colors['white'],
		background = colors['black'],
		damage_n=1, damage_d=4, damage_mod=0, damage_stat="str"
	},
	pointy_stick = {
		name = "Pointy Stick",
		description = "A sharpened stick",
		itemtype = {"weapon-melee"},
		glyph = glyphs['axe'],
		foreground = colors['white'],
		background = colors['black'],
		damage_n=1, damage_d=4, damage_mod=0, damage_stat="str"
	},
	sword = {
		name = "Sword",
		description = "A simple sword",
		itemtype = {"weapon-melee"},
		glyph = glyphs['axe'],
		foreground = colors['cyan'],
		background = colors['black'],
		damage_n=1, damage_d=8, damage_mod=0, damage_stat="str"
	},
	atlatl = {
		name = "Atlatl",
		description = "A curved piece of wood used to launch darts at high velocity",
		itemtype = {"weapon-ranged"},
		glyph = glyphs['axe'],
		foreground = colors['wood_brown'],
		background = colors['black'],
		damage_mod=0, damage_stat="str", range=6, ammo="dart"
	},
	wood_dart = {
		name = "Wooden Dart",
		description = "A simple wooden dart",
		itemtype = {"ammo-dart"},
		glyph = glyphs['axe'],
		foreground = colors['wood_brown'],
		background = colors['black'],
		damage_n=1, damage_d=4, damage_mod=0, stack_size=20
	},
	axe_blade = {
		name = "Axe Blade",
		description = "An axe blade",
		itemtype = {"component"},
		glyph = glyphs['axe'],
		foreground = colors['white'],
		background = colors['black']
	},
	pickaxe_blade = {
		name = "Pick-Axe Blade",
		description = "A pickaxe blade",
		itemtype = {"component"},
		glyph = glyphs['axe'],
		foreground = colors['white'],
		background = colors['black']
	},
	sword_blade = {
		name = "Sword Blade",
		description = "A sword blade",
		itemtype = {"component"},
		glyph = glyphs['axe'],
		foreground = colors['white'],
		background = colors['black']
	},
	axe_haft = {
		name = "Axe Haft",
		description = "An axe haft",
		itemtype = {"component"},
		glyph = glyphs['axe'],
		foreground = colors['wood_brown'],
		background = colors['black']
	},
	sword_hilt = {
		name = "Sword Hilt",
		description = "A sword hilt",
		itemtype = {"component"},
		glyph = glyphs['axe'],
		foreground = colors['wood_brown'],
		background = colors['black']
	},
	forge_parts = {
		name = "Forge Parts",
		description = "Components required for an advanced forge",
		itemtype = {"component"},
		glyph = glyphs['axe'],
		foreground = colors['grey'],
		background = colors['black']
	}
}
