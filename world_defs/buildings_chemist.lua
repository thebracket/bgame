-- Chemistry facilities

------------------------------------------------------------------------------------------------------------------------
-- Simple chemists handle early chemistry tasks
------------------------------------------------------------------------------------------------------------------------
buildings["simple_chemist"] = {
    name = "Simple Chemistry Workshop",
    description = "My first chemistry kit, nicely assembled in a room for simple Chemistry. Ideal for making explosives.",
    components = { { item="block", qty=1 }, { item="raw_glass", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "chemist.xp",
    vox=voxelId("chemist")
};

reactions["make_black_powder"] = {
    name = "Make Black Powder",
    workshop = "simple_chemist",
    inputs = { { item="sulphur", qty=1 }, { item="saltpeter_ore", qty=1 } },
    outputs = { { item="black_powder", qty=1 } },
    skill = "Chemistry",
    difficulty = 15,
    automatic = false
};

reactions["make_nitric_acid"] = {
    name = "Make Nitric Acid",
    workshop = "simple_chemist",
    inputs = { { item="sulphur", qty=1 }, { item="saltpeter_ore", qty=1 } },
    outputs = { { item="nitric_acid", qty=1 } },
    skill = "Chemistry",
    difficulty = 15,
    automatic = false
};

reactions["refine_dung_acid"] = {
    name = "Refine Dung into Nitric Acid",
    workshop = "simple_chemist",
    inputs = { { item="dung", qty=1 } },
    outputs = { { item="nitric_acid", qty=1 } },
    skill = "Chemistry",
    difficulty = 15,
    automatic = false
};

reactions["refine_dung_saltpeter"] = {
    name = "Refine Dung into Saltpeter",
    workshop = "simple_chemist",
    inputs = { { item="dung", qty=1 } },
    outputs = { { item="saltpeter_ore", qty=1 } },
    skill = "Chemistry",
    difficulty = 15,
    automatic = false
};

reactions["make_raw_dynamite"] = {
    name = "Make Raw Dynamite",
    workshop = "simple_chemist",
    inputs = { { item="sulphur", qty=1 }, { item="nitric_acid", qty=1 } },
    outputs = { { item="raw_dynamite", qty=1 } },
    skill = "Chemistry",
    difficulty = 15,
    automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Implements the Birkeland-Eyde process (https://en.wikipedia.org/wiki/Birkeland%E2%80%93Eyde_process) to make
-- nitric acid out of the air.
------------------------------------------------------------------------------------------------------------------------
buildings["nitrogen_extractor"] = {
    name = "Nitrogen Extractor",
    description = "A contraption that implements the Birkeland-Eyde process to extract nitrogen from the atmosphere. Much cleaner than using excrement.",
    components = { { item="block", qty=2 }, { item="circuit", qty=1 } },
    skill = { name="Construction", difficulty=17 },
    render_rex = "nitrogen_extractor.xp",
    vox = voxelId("nitrogen_extractor")
};

reactions["extract_atmospheric_nitrogen"] = {
    name = "Refine Air into Nitric Acid",
    workshop = "nitrogen_extractor",
    inputs = {  },
    outputs = { { item="nitric_acid", qty=1 } },
    skill = "Construction",
    difficulty = 5,
    automatic = false,
    power_drain = 75
};
