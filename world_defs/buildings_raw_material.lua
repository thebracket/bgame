-- Workshops that mostly just convert raw materials into other materials

------------------------------------------------------------------------------------------------------------------------
-- Dungheaps turn dung into manure
------------------------------------------------------------------------------------------------------------------------

buildings["dung_heap"] = {
    name = "Dung Heap",
    description = "A smelly pile of manure.",
    components = { { item="block", qty=1, mat_type="wood" } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "dungheap.xp",
    vox = voxelId("dung_heap")
};

reactions["make-manure"] = {
    name = "Make Manure",
    workshop = "dung_heap",
    inputs = { { item="dung", qty=1 }, {item="hay", qty=1} },
    outputs = { { item="manure", qty=1 } },
    skill = "Farming",
    difficulty = 10,
    automatic = false
 };

------------------------------------------------------------------------------------------------------------------------
-- Sawmills are used for converting wood into blocks, and offcuts.
------------------------------------------------------------------------------------------------------------------------

buildings["sawmill"] = {
    name = "Sawmill",
    description = "Two tables and a large cutting saw, designed for turning wooden logs into usable planks.",
    components = { { item="wood_log", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "sawmill.xp",
    vox=voxelId("sawmill")
};

reactions["cut_wooden_planks"] = {
   name = "Cut Wooden Logs into Blocks",
   workshop = "sawmill",
   inputs = { { item="wood_log", qty=1 } },
   outputs = { { item="block", qty=4 }, { item="wood_waste", qty=2} },
   skill = "Carpentry",
   difficulty = 10,
   automatic = true
};

------------------------------------------------------------------------------------------------------------------------
-- Tanneries combine hide with dung, and use it to make leather.
------------------------------------------------------------------------------------------------------------------------

buildings["tanner"] = {
    name = "Tanner",
    description = "A series of racks for hanging hide, applying tannin and turning it into good leather. Smells AWFUL.",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "tanner.xp",
    vox = voxelId("tanner")
};

reactions["tan_leather"] = {
   name = "Tan Leather",
   workshop = "tanner",
   inputs = { { item="hide", qty=1 } },
   outputs = { { item="leather", qty=2, special="tanning" } },
   skill = "Tanning",
   difficulty = 10,
   automatic = true
};

------------------------------------------------------------------------------------------------------------------------
-- Stonecutters are where one cuts stone. Kinda obvious, I guess.
------------------------------------------------------------------------------------------------------------------------
buildings["stonecutter"] = {
    name = "Stonecutters",
    description = "A workshop designed to turn stone into usable blocks. Basically a guy with a hammer and chisel.",
    components = { { item="stone_boulder", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "stonecutter.xp",
    vox = voxelId("stonecutter")
};

reactions["cut_stone"] = {
    name = "Cut Stone Blocks",
    workshop = "stonecutter",
    inputs = { { item="stone_boulder", qty=1 } },
    outputs = { { item="block", qty=4 } },
    skill = "Masonry",
    difficulty = 10,
    automatic = true
};

------------------------------------------------------------------------------------------------------------------------
-- Charcoal huts slow-cook wood to make a super-fuel. They can also be used to refine coal.
------------------------------------------------------------------------------------------------------------------------

buildings["charcoal_hut"] = {
    name = "Charcoal Burner",
    description = "A hut designed to slowly burn waste wood, until it is blackened charcoal - a very high temperature heat source when burned (and useful for drawing on sidewalks).",
    components = { { item="block", qty=1, mat_type="rock" } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "charcoal_hut.xp",
    vox = voxelId("charcoal_hut")
};

reactions["charcoal_waste"] = {
    name = "Make Charcoal from Waste",
    workshop = "charcoal_hut",
    inputs = { { item="wood_waste", qty=1 } },
    outputs = { { item="charcoal", qty=2 }, { item="ash", qty=1} },
    skill = "Furnace Operation",
    difficulty = 10,
    automatic = true,
    emits_smoke = true
};

------------------------------------------------------------------------------------------------------------------------
-- Smelters extract metal from ore.
------------------------------------------------------------------------------------------------------------------------

buildings["smelter"] = {
    name = "Smelter",
    description = "Smelters heat rock until metal elements inside melt and flow out.",
    components = { { item="block", qty=1, mat_type="rock" } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "smelter.xp",
    vox = voxelId("smelter")
};

reactions["smelt_ore"] = {
   name = "Smelt Ore",
   workshop = "smelter",
   inputs = { { item="ore", qty=1 } },
   outputs = { { item="block", qty=2 } },
   skill = "Furnace Operation",
   difficulty = 12,
   automatic = true
};

------------------------------------------------------------------------------------------------------------------------
-- Blast Smelters are used to make advanced metals from other metals.
------------------------------------------------------------------------------------------------------------------------

buildings["blast_smelter"] = {
    name = "Blast Smelter",
    description = "A high-efficiency smelter that super-heats by containing small explosions inside the building.",
    components = { { item="block", qty=1, mat_type="rock" }, { item="tool_and_die", qty=1 }, { item="mechanism", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "smelter.xp",
    vox = voxelId("smelter")
};

------------------------------------------------------------------------------------------------------------------------
-- Glass furnaces heat up sand to make raw glass.
------------------------------------------------------------------------------------------------------------------------
buildings["glass_furnace"] = {
    name = "Glass Furnace",
    description = "A specialized furnace for heating sand, and melting it into glass.",
    components = { { item="block", qty=1, mat_type="rock" } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "glass_furnace.xp",
    vox = voxelId("glass_furnace")
};

reactions["make_raw_glass"] = {
    name = "Make Raw Glass",
    workshop = "glass_furnace",
    inputs = { { item="charcoal", qty=1 }, { item="sand", qty=1} },
    outputs = { { item="raw_glass", qty=2 } },
    skill = "Glassmaking",
    difficulty = 10,
    automatic = false,
    emits_smoke = true
};

reactions["make_glass_spikes"] = {
    name = "Make Glass Spikes",
    workshop = "glass_furnace",
    inputs = { { item="raw_glass", qty=1 } },
    outputs = { { item="retractable_spike", qty=1 } },
    skill = "Glassmaking",
    difficulty = 15,
    automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Silicon refineries convert glass into raw silicon.
------------------------------------------------------------------------------------------------------------------------
buildings["silicon_refinery"] = {
    name = "Silicon Refinery",
    description = "Once processed into glass, silicon can be extracted in a pure form at a refinery. It heats the glass again, and removes impurities.",
    components = { { item="block", qty=1, mat_type="rock" }, { item="raw_glass", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "glass_furnace.xp",
    vox = voxelId("silicon_refinery")
};

reactions["make_raw_silicon"] = {
   name = "Make Raw Silicon",
   workshop = "silicon_refinery",
   inputs = { { item="raw_glass", qty=1} },
   outputs = { { item="raw_silicon", qty=1 } },
   skill = "Glassmaking",
   difficulty = 10,
   automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Kilns are used for baking clay into bricks. They should also be used for pottery.
------------------------------------------------------------------------------------------------------------------------

buildings["kiln"] = {
    name = "Kiln",
    description = "An oven for baking clay.",
    components = { { item="clay", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "kiln.xp",
    vox = voxelId("kiln")
};

reactions["bake_bricks"] = {
    name = "Bake Clay into Bricks",
    workshop = "kiln",
    inputs = { { item="charcoal", qty=1 }, { item="clay", qty=1 } },
    outputs = { { item="block", qty=4 } },
    skill = "Carpentry",
    difficulty = 10,
    automatic = true
};

------------------------------------------------------------------------------------------------------------------------
-- Looms are used to weave thread into cloth
------------------------------------------------------------------------------------------------------------------------

buildings["loom"] = {
    name = "Loom",
    description = "A wooden frame for spinning thread, and then knitting thread together into cloth.",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "loom.xp",
    vox = voxelId("loom")
};

reactions["weave_thread_reed"] = {
    name = "Weave Reed Thread into Cloth",
    workshop = "loom",
    inputs = { { item="reed_thread", qty=1 } },
    outputs = { { item="cloth", qty=4 } },
    skill = "Weaving",
    difficulty = 10,
    automatic = true
};

reactions["weave_thread_burlap"] = {
    name = "Weave Burlap Thread into Cloth",
    workshop = "loom",
    inputs = { { item="burlap_thread", qty=1 } },
    outputs = { { item="cloth", qty=4 } },
    skill = "Weaving",
    difficulty = 10,
    automatic = true
};

reactions["weave_thread_hemp"] = {
    name = "Weave Hemp Thread into Cloth",
    workshop = "loom",
    inputs = { { item="hemp_thread", qty=1 } },
    outputs = { { item="cloth", qty=4 } },
    skill = "Weaving",
    difficulty = 10,
    automatic = true
};

reactions["weave_thread_ramie"] = {
    name = "Weave Ramie Thread into Cloth",
    workshop = "loom",
    inputs = { { item="ramie_thread", qty=1 } },
    outputs = { { item="cloth", qty=4 } },
    skill = "Weaving",
    difficulty = 10,
    automatic = true
};

reactions["weave_thread_kenaf"] = {
    name = "Weave Kenaf Thread into Cloth",
    workshop = "loom",
    inputs = { { item="kenaf_thread", qty=1 } },
    outputs = { { item="cloth", qty=4 } },
    skill = "Weaving",
    difficulty = 10,
    automatic = true
};

------------------------------------------------------------------------------------------------------------------------
-- Oil refineries take oil as input, let it settle, and produce various oil byproducts
------------------------------------------------------------------------------------------------------------------------
buildings["oil_refinery"] = {
    name = "Oil Refinery",
    description = "An oil tank, in which oil settles by molecular weight. Different products are then extracted from different depths.",
    components = { { item="block", qty=1 }, { item="tool_and_die", qty=1 }, { item="oil_separation_tank", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "refinery.xp",
    vox = voxelId("refinery")
};

reactions["refine_oil"] = {
    name = "Refine Oil",
    workshop = "oil_refinery",
    inputs = { { item="oil", qty=1 }, { item="block", qty=1 } },
    outputs = {
        { item="liquified_petroleum_gas", qty=4 },
        { item="gasolene_fuel", qty=4 },
        { item="naptha", qty=4 },
        { item="lubricating_oil", qty=4 },
        { item="paraffin_wax", qty=4 },
        { item="tar", qty=4 },
        { item="charcoal", qty=1 },
        { item="sulphur", qty=4 }
    },
    skill = "Weaving",
    difficulty = 10,
    automatic = true,
    power_drain = 20,
};
