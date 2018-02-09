-- Mechanical Workshops

------------------------------------------------------------------------------------------------------------------------
-- Mechanic shops are used to make mechanisms
------------------------------------------------------------------------------------------------------------------------
buildings["mechanic"] = {
    name = "Mechanic Workshop",
    description = "A workshop dedicated to the production of mechanical devices and circuitry.",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "mechanic.xp",
    vox = voxelId("mechanic")
};

reactions["make_simple_mechanism"] = {
    name = "Make Simple Mechanism",
    workshop = "mechanic",
    inputs = { { item="block", qty=1} },
    outputs = { { item="mechanism", qty=1 } },
    skill = "Mechanics",
    difficulty = 10,
    automatic = false
};

reactions["make_simple_circuit"] = {
    name = "Make Simple Circuit",
    workshop = "mechanic",
    inputs = { { item="raw_silicon", qty=1}, { item="block", mat_type="metal", qty=1 } },
    outputs = { { item="circuit", qty=1 } },
    skill = "Mechanics",
    difficulty = 10,
    automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Machine shops are used to create advanced mechanical devices
------------------------------------------------------------------------------------------------------------------------
buildings["machine_shop"] = {
    name = "Machine Shop",
    description = "An advanced mechanic shop, capable of working on the most complex of machines.",
    components = { { item="block", qty=1 }, { item="simple_lathe", qty=1 }, { item="tool_and_die", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "mechanic.xp",
    vox = voxelId("mechanic")
};