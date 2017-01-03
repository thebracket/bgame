items = {
	personal_survival_shelter_kit = {
		name = "Personal Survival Shelter Kit",
		description = "A folded-up personal shelter kit, known in non-corporate parlance as a camping set: a tent, bedroll and sleeping bag.",
		itemtype = {"component"},
		glyph = glyphs['tent_kit'],
		foreground = colors['green'],
		background = colors['black'],
		stockpile = stockpiles['kit'].id
	},
	camp_fire_kit = {
		name = "Camping Fire Kit",
		description = "A plastic campfire, powered by a tiny nuclear reactor. Ideal for your off-world vacation.",
		itemtype = {"component"},
		glyph = glyphs['campfire_kit'],
		foreground = colors['red'],
		background = colors['black'],
		stockpile = stockpiles['kit'].id
	},
	fire_axe = {
		name = "Fire Axe",
		description = "A simple axe. It has the word FIRE burned into it.",
		itemtype = {"tool-chopping"},
		glyph = glyphs['axe_chopping'],
		foreground = colors['white'],
		background = colors['black'],
		stockpile = stockpiles['tool'].id
	},
	pickaxe = {
		name = "Pickaxe",
		description = "A simple pickaxe, ideal for digging holes.",
		itemtype = {"tool-digging"},
		glyph = glyphs['axe_pickaxe'],
		foreground = colors['white'],
		background = colors['black'],
		stockpile = stockpiles['tool'].id
	},
	wood_log = {
		name = "Log",
		description = "Wood from a felled tree",
		itemtype = {"component"},
		glyph = glyphs['wood_log'],
		foreground = colors['white'],
		background = colors['black'],
		stockpile = stockpiles['wood'].id
	},
	stone_boulder = {
		name = "Boulder",	description = "A large stone boulder",
		itemtype = {"component"}, glyph = glyphs['boulder'], foreground = colors['white'], background = colors['black'],
		stockpile = stockpiles['stone'].id
	},
	ore = {
		name = "Ore", description = "A lump of unrefined metal",
		itemtype = {"component"}, glyph = glyphs['ore'], foreground = colors['white'], background = colors['black'],
		stockpile = stockpiles['ore'].id
	},
	saltpeter_ore = {
		name = "Saltpeter Ore", description = "A lump of unrefined saltpeter",
		itemtype = {"component"}, glyph = glyphs['star'], foreground = colors['white'], background = colors['black'],
		stockpile = stockpiles['ore'].id
	},
	alabaster_powder = {
		name = "Alabaster Powder", description = "A lump of unrefined alabaster powder",
		itemtype = {"component"}, glyph = glyphs['inset_square'], foreground = colors['white'], background = colors['black'],
		stockpile = stockpiles['powder'].id
	},
	gypsum_powder = {
		name = "Gypsum Powder", description = "A lump of unrefined gypsum powder",
		itemtype = {"component"}, glyph = glyphs['inset_square'], foreground = colors['white'], background = colors['black'],
		stockpile = stockpiles['powder'].id
	},
	coal = {
		name = "Coal", description = "A lump of unrefined coal",
		itemtype = {"component"}, glyph = glyphs['ore'], foreground = colors['dark_grey'], background = colors['black'],
		stockpile = stockpiles['stone'].id
	},
	sulphur = {
		name = "Sulphur", description = "A lump of sulphur",
		itemtype = {"component"}, glyph = glyphs['inset_square'], foreground = colors['yellow'], background = colors['black'],
		stockpile = stockpiles['stone'].id
	},
	flint = {
		name = "Flint", description = "A piece of flint",
		itemtype = {"component"}, glyph = glyphs['inset_square'], foreground = colors['brown'], background = colors['black'],
		stockpile = stockpiles['stone'].id
	},
	sand = {
		name = "Sand", description = "A lump of raw sand",
		itemtype = {"component"}, glyph = glyphs['double_wave'], foreground = colors['yellow'], background = colors['black'],
		stockpile = stockpiles['sand'].id
	},
	oil = {
		name = "Oil", description = "A blob of viscous oil",
		itemtype = {"component"}, glyph = glyphs['oil'], foreground = colors['white'], background = colors['black'],
		stockpile = stockpiles['oil'].id
	},
	clay = {
		name = "Clay", description = "A blob of clay",
		itemtype = {"component"}, glyph = glyphs['inset_square'], foreground = colors['dark_red'], background = colors['black'],
		stockpile = stockpiles['stone'].id
	},
	topsoil = {
		name = "Topsoil", description = "A lump of rich topsoil",
		itemtype = {"component"}, glyph = glyphs['double_wave'], foreground = colors['brown'], background = colors['black'],
		stockpile = stockpiles['soil'].id
	},
	charcoal = {
		name = "Charcoal",
		description = "Wood, processed to burn at high temperature",
		itemtype = {"component"},
		glyph = glyphs['inset_square'],
		foreground = colors['wood_brown'],
		background = colors['black'],
		stockpile = stockpiles['fuel'].id
	},
	ash = {
		name = "Ash",
		description = "Ash, remnants of burned wood",
		itemtype = {"component"},
		glyph = glyphs['one_quarter_solid'],
		foreground = colors['grey'],
		background = colors['black'],
		stockpile = stockpiles['powder'].id
	},
	raw_glass = {
		name = "Raw Glass",
		description = "Raw glass, ready to turn into something useful",
		itemtype = {"component"},
		glyph = glyphs['inset_square'],
		foreground = colors['green'],
		background = colors['black'],
		stockpile = stockpiles['glass'].id
	},
	raw_silicon = {
		name = "Raw Silicon",
		description = "Silicon, suitable for use in circuitry and chips",
		itemtype = {"component"},
		glyph = glyphs['inset_square'],
		foreground = colors['green'],
		background = colors['black'],
		stockpile = stockpiles['silicon'].id
	},
	wood_waste = {
		name = "Offcuts",
		description = "Wood off-cuts",
		itemtype = {"component"},
		glyph = glyphs['three_bar'],
		foreground = colors['wood_brown'],
		background = colors['black'],
		stockpile = stockpiles['waste'].id
	},
	block = {
		name = "Block",
		description = "A convenient building block",
		itemtype = {"component"},
		glyph = glyphs['block'],
		foreground = colors['grey'],
		background = colors['black'],
		stockpile = stockpiles['block'].id
	},
	table = {
		name = "Table",
		description = "A table",
		itemtype = {"furniture"},
		glyph = glyphs['table'],
		foreground = colors['wood_brown'],
		background = colors['black'],
		stockpile = stockpiles['furniture'].id
	},
	chair = {
		name = "Chair",
		description = "A chair",
		itemtype = {"furniture"},
		glyph = glyphs['chair'],
		foreground = colors['wood_brown'],
		background = colors['black'],
		stockpile = stockpiles['furniture'].id
	},
	door = {
		name = "Door",
		description = "A simple door",
		itemtype = {"component"},
		glyph = glyphs['door'],
		foreground = colors['wood_brown'],
		background = colors['black'],
		stockpile = stockpiles['furniture'].id
	},
	tiny_marshmallow = {
		name = "Tiny Marshmallow",
		description = "A tiny lump of congealed sugar",
		itemtype = {"food"},
		glyph = glyphs['squared'],
		foreground = colors['magenta'],
		background = colors['black'],
		stockpile = stockpiles['food'].id
	},
	tea_replicated = {
		name = "Tea, Earl-Grey Lukewarm",
		description = "A replicated substance that tastes somewhat like tea",
		itemtype = {"drink"},
		glyph = glyphs['squared'],
		foreground = colors['wood_brown'],
		background = colors['black'],
		stockpile = stockpiles['food'].id
	},
	sandwich_replicated = {
		name = "Stale-tasting sandwich",
		description = "A replicated sandwich that sort-of tastes like chicken",
		itemtype = {"food"},
		glyph = glyphs['squared'],
		foreground = colors['wood_brown'],
		background = colors['black'],
		stockpile = stockpiles['food'].id
	},
	club = {
		name = "Club",
		description = "A simple club",
		itemtype = {"weapon-melee"},
		glyph = glyphs['axe'],
		foreground = colors['white'],
		background = colors['black'],
		damage_n=1, damage_d=4, damage_mod=0, damage_stat="str",
		stockpile = stockpiles['weapon'].id
	},
	pointy_stick = {
		name = "Pointy Stick",
		description = "A sharpened stick",
		itemtype = {"weapon-melee"},
		glyph = glyphs['axe'],
		foreground = colors['white'],
		background = colors['black'],
		damage_n=1, damage_d=4, damage_mod=0, damage_stat="str",
		stockpile = stockpiles['weapon'].id
	},
	warhammer = {
		name = "Warhammer",
		description = "A heavy hammer design for bashing people",
		itemtype = {"weapon-melee"},
		glyph = glyphs['axe'],
		foreground = colors['grey'],
		background = colors['black'],
		damage_n=1, damage_d=4, damage_mod=0, damage_stat="str",
		stockpile = stockpiles['weapon'].id
	},
	battleaxe = {
		name = "Battle Axe",
		description = "A large axe, designed for chopping people rather than trees",
		itemtype = {"weapon-melee"},
		glyph = glyphs['axe'],
		foreground = colors['grey'],
		background = colors['black'],
		damage_n=1, damage_d=8, damage_mod=0, damage_stat="str", initiative_penalty = 6,
		stockpile = stockpiles['weapon'].id
	},
	handaxe = {
		name = "Hand Axe",
		description = "A small axe, optimized for hitting people quickly",
		itemtype = {"weapon-melee"},
		glyph = glyphs['axe'],
		foreground = colors['grey'],
		background = colors['black'],
		damage_n=1, damage_d=6, damage_mod=0, damage_stat="str", initiative_penalty = 6,
		stockpile = stockpiles['weapon'].id
	},
	shortsword = {
		name = "Short Sword",
		description = "A short sword, designed for thrusting",
		itemtype = {"weapon-melee"},
		glyph = glyphs['axe'],
		foreground = colors['bismuth_yellow'],
		background = colors['black'],
		damage_n=1, damage_d=6, damage_mod=0, damage_stat="str", initiative_penalty = 5,
		stockpile = stockpiles['weapon'].id
	},
	longsword = {
		name = "Long Sword",
		description = "A long sword, designed for slashing",
		itemtype = {"weapon-melee"},
		glyph = glyphs['axe'],
		foreground = colors['bismuth_yellow'],
		background = colors['black'],
		damage_n=1, damage_d=6, damage_mod=0, damage_stat="str", initiative_penalty = 5,
		stockpile = stockpiles['weapon'].id
	},
	knife = {
		name = "Knife",
		description = "A simple knife",
		itemtype = {"weapon-melee"},
		glyph = glyphs['axe'],
		foreground = colors['grey'],
		background = colors['black'],
		damage_n=1, damage_d=3, damage_mod=0, damage_stat="str", initiative_penalty = 2,
		stockpile = stockpiles['weapon'].id
	},
	dagger = {
		name = "Dagger",
		description = "A simple dagger",
		itemtype = {"weapon-melee"},
		glyph = glyphs['axe'],
		foreground = colors['grey'],
		background = colors['black'],
		damage_n=1, damage_d=4, damage_mod=0, damage_stat="str", initiative_penalty = 2,
		stockpile = stockpiles['weapon']
	},
	atlatl = {
		name = "Atlatl",
		description = "A curved piece of wood used to launch darts at high velocity",
		itemtype = {"weapon-ranged"},
		glyph = glyphs['axe'],
		foreground = colors['wood_brown'],
		background = colors['black'],
		damage_mod=0, damage_stat="str", range=6, ammo="dart", initiative_penalty = 6,
		stockpile = stockpiles['weapon']
	},
	ray_pistol = {
		name = "Ray Pistol",
		description = "A tiny, one handed raygun, that looks suspiciously like a toy.",
		itemtype = {"weapon-ranged"},
		glyph = glyphs['axe'],
		foreground = colors['wood_brown'],
		background = colors['black'],
		damage_mod=0, damage_stat="dex", range=4, ammo="smallcell", initiative_penalty = 4,
		stockpile = stockpiles['weapon'].id
	},
	shortbow = {
		name = "Shortbow",
		description = "A simple shortbow",
		itemtype = {"weapon-ranged"},
		glyph = glyphs['axe'],
		foreground = colors['wood_brown'],
		background = colors['black'],
		damage_mod=0, damage_stat="dex", range=4, ammo="arrow", initiative_penalty = 4,
		stockpile = stockpiles['weapon'].id
	},
	dart = {
		name = "Dart",
		description = "A simple dart",
		itemtype = {"ammo"},
		glyph = glyphs['axe'],
		foreground = colors['wood_brown'],
		background = colors['black'],
		damage_n=1, damage_d=4, damage_mod=0, stack_size=20, ammo="dart",
		stockpile = stockpiles['ammo'].id
	},
	small_energy_cell = {
		name = "Energy Cell",
		description = "A small energy cell, used for powering laser weaponry.",
		itemtype = {"ammo"},
		glyph = glyphs['axe'],
		foreground = colors['red'],
		background = colors['black'],
		damage_n=1, damage_d=4, damage_mod=0, stack_size=20, ammo="smallcell",
		stockpile = stockpiles['ammo'].id
	},
	arrow = {
		name = "Arrow",
		description = "A simple arrow",
		itemtype = {"ammo"},
		glyph = glyphs['axe'],
		foreground = colors['wood_brown'],
		background = colors['black'],
		damage_n=1, damage_d=4, damage_mod=0, stack_size=20, ammo="arrow",
		stockpile = stockpiles['ammo'].id
	},
	axe_blade = {
		name = "Axe Blade",
		description = "An axe blade",
		itemtype = {"component"},
		glyph = glyphs['axe'],
		foreground = colors['white'],
		background = colors['black'],
		stockpile = stockpiles['component'].id
	},
	forge_parts = {
		name = "Forge Parts",
		description = "Components required for an advanced forge",
		itemtype = {"component"},
		glyph = glyphs['axe'],
		foreground = colors['grey'],
		background = colors['black'],
		stockpile = stockpiles['component'].id
	},

	bone = {
		name = "Bone",
		description = "Bones",
		itemtype = {"component"},
		glyph = glyphs['single_wall_ew'],
		foreground = colors['white'],
		background = colors['black'],
		stockpile = stockpiles['bone'].id
	},
	hide = {
		name = "Hide",
		description = "Untanned Hide",
		itemtype = {"component"},
		glyph = glyphs['single_wall_ew'],
		foreground = colors['wood_brown'],
		background = colors['black'],
		stockpile = stockpiles['leather'].id
	},
	meat = {
		name = "Meat",
		description = "Raw Meat",
		itemtype = {"food"},
		glyph = glyphs['single_wall_ew'],
		foreground = colors['red'],
		background = colors['black'],
		stockpile = stockpiles['food'].id
	},
	skull = {
		name = "Skull",
		description = "A skull",
		itemtype = {"component"},
		glyph = glyphs['happy_face'],
		foreground = colors['white'],
		background = colors['black'],
		stockpile = stockpiles['bone'].id
	},
	leather = {
		name = "Leather",
		description = "Tanned leather, ready to make into something useful",
		itemtype = {"component"},
		glyph = glyphs['single_wall_ew'],
		foreground = colors['wood_brown'],
		background = colors['black'],
		stockpile = stockpiles['leather'].id
	},

	-- Vegetation items
	hay = { 
		name="Hay", 
		description = "Dried grass, suitable for animal fodder", 
		itemtype = { "feed" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['yellow'], background=colors['black'], 
		stockpile=stockpiles['fodder'] 
	},
	reed_thread = { 
		name="Reed Fiber", 
		description = "Fibers from a reed, suitable for use in cloth or wicker", 
		itemtype = { "thread" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['white'], background=colors['black'], 
		stockpile=stockpiles['thread'] 
	},
	reed_paper = { 
		name="Reed Papyrus", 
		description = "Fibers from a reed, suitable for making paper", 
		itemtype = { "thread" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['white'], background=colors['black'], 
		stockpile=stockpiles['thread'] 
	},
	artichoke_veg = { 
		name="Raw Artichoke", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	asparagus_veg = { 
		name="Raw Asparagus", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	string_bean_veg = { 
		name="Raw String Bean", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	broad_bean_veg = { 
		name="Raw Broad Bean", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	beetroot_veg = { 
		name="Raw Beetroot", 
		description = "A root vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['purple'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	goya_veg = { 
		name="Raw Goya", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	cabbage_veg = { 
		name="Raw Cabbage", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	caper_veg = { 
		name="Raw Caper", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	carrot_veg = { 
		name="Raw Carrot", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['orange'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	cassava_veg = { 
		name="Raw Cassava", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	celery_veg = { 
		name="Raw Celery", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	chickpea_veg = { 
		name="Raw Chickpea", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	cowpea_veg = { 
		name="Raw Cowpea", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	cucumber_veg = { 
		name="Raw Cucumber", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	eggplant_veg = { 
		name="Raw Eggplant", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	cress_veg = { 
		name="Raw Cress", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	melon_veg = { 
		name="Raw Melon", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	leek_veg = { 
		name="Raw Leek", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	lentil_veg = { 
		name="Raw Lentils", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	lettuce_veg = { 
		name="Raw Lettuce", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	mung_bean_veg = { 
		name="Raw Mung Bean", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	onion_veg = { 
		name="Raw Onion", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	parsnip_veg = { 
		name="Raw Parsnip", 
		description = "A root vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	pea_veg = { 
		name="Raw Peas", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	pepper_veg = { 
		name="Raw Bell Pepper", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	potato_veg = { 
		name="Raw Potato", 
		description = "A root vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	radish_veg = { 
		name="Raw Radish", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	red_bean_veg = { 
		name="Raw Red Beans", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	spinach_veg = { 
		name="Raw Spinach", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	soybean_veg = { 
		name="Raw Soy", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	squash_veg = { 
		name="Raw Squash", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	sweet_potato_veg = { 
		name="Raw Sweet Potato", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['orange'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	tomato_veg = { 
		name="Raw Tomato", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['red'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	tomatillo_veg = { 
		name="Raw Tomatillo", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	turnip_veg = { 
		name="Raw Turnip", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	watermelon_veg = { 
		name="Raw Watermelon", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	yam_veg = { 
		name="Raw Yam", 
		description = "A green vegetable", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['yellow'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	strawberry_fruit = { 
		name="Raw Strawberry", 
		description = "A red fruit", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['red'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	bambara_nut = { 
		name="Bambara Nut", 
		description = "A tasty nut", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['red'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	peanut_nut = { 
		name="Peanut", 
		description = "A tasty nut", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['brown'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	chickory_herb = { 
		name="Raw Chickory", 
		description = "A tasty herb", 
		itemtype = { "spice" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['brown'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	garlic_herb = { 
		name="Raw Garlic", 
		description = "A tasty herb", 
		itemtype = { "spice" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['brown'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	rhubarb_herb = { 
		name="Raw Rhubarb", 
		description = "A tasty herb", 
		itemtype = { "spice" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['brown'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	lavendar_herb = { 
		name="Raw Lavendar", 
		description = "A tasty herb", 
		itemtype = { "spice" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['purple'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	sage_herb = { 
		name="Raw Sage", 
		description = "A tasty herb", 
		itemtype = { "spice" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},

	-- Cooked Food
	roast_simple = { 
		name="Roasted", 
		description = "Roasted food", 
		itemtype = { "food" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['brown'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
	wine = { 
		name="Alcoholic Beverage", 
		description = "Fermented food", 
		itemtype = { "drink" }, glyph = glyphs["inv_exclaim"], 
		foreground=colors['green'], background=colors['black'], 
		stockpile=stockpiles['food'] 
	},
}
