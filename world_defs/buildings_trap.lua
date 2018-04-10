-- Trap buildings

buildings["cage_trap"] = {
    name = "Cage Trap",
    description = "A wicked trap designed to catch invaders inside a cage.",
    components = { { item="cage", qty=1}, { item="mechanism", qty=1 } },
    skill = { name="Construction", difficulty=10 },
    provides = { cage_trap={energy_cost=0} },
    render_rex = "cagetrap.xp",
    vox = voxelId("cage")
};

buildings["stonefall_trap"] = {
    name = "Stonefall Trap",
    description = "A simple trap that drops a rock on an invader's head.",
    components = { { item="stone_boulder", qty=1}, { item="mechanism", qty=1 } },
    skill = { name="Construction", difficulty=10 },
    provides = { stonefall_trap={energy_cost=0} },
    render_rex = "cagetrap.xp",
    vox = voxelId("stonefall")
};

buildings["weapon_trap"] = {
    name = "Blade Trap",
    description = "A nasty trap designed to slice and dice invaders.",
    components = { { item="trap_blade", qty=1}, { item="mechanism", qty=1 } },
    skill = { name="Construction", difficulty=10 },
    provides = { blade_trap={energy_cost=0} },
    render_rex = "cagetrap.xp",
    vox = voxelId("blades_retracted")
};

buildings["spike_trap"] = {
    name = "Retractable Spikes",
    description = "Sharp spikes that can be retracted into the ground, or extended to damage all present.",
    components = { { item="retractable_spike", qty=1}, { item="mechanism", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    provides = { spike_trap={energy_cost=0} },
    render_rex = "cagetrap.xp",
    vox = voxelId("spikes_retracted")
};

buildings["lever"] = {
    name = "Lever",
    description = "The most basic input form for a machine, levers can be linked to other devices to operate them.",
    components = { { item="mechanism", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    provides = { lever={energy_cost=0} },
    render = {
        width=1, height=1, tiles={
            {glyph=glyphs['lever_off'], foreground = colors['red'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles={
            {glyph=glyphs['marker'], foreground = colors['red'], background = colors['black']}
        }
    },
    vox = voxelId("lever_off")
};

buildings["pressure_plate"] = {
    name = "Pressure Plate",
    description = "A simple pressure plate that triggers when something stands on it.",
    components = { { item="mechanism", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    provides = { pressure_plate={energy_cost=0} },
    render_rex = "pressureplate.xp",
    vox = voxelId("pressure_plate")
};

buildings["float_sensor"] = {
    name = "Float sensor",
    description = "A simple water level gauge, triggering if the water rises above a certain level.",
    components = { { item="mechanism", qty=1 }, { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    provides = { float_sensor={energy_cost=0} },
    render_rex = "pressureplate.xp",
    vox = voxelId("float_gauge")
};

buildings["proximity_sensor"] = {
    name = "Proximity sensor",
    description = "An electronic proximity sensor that triggers if movement occurs nearby.",
    components = { { item="mechanism", qty=1 }, { item="circuit", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    provides = { proximity_sensor={energy_cost=10} },
    render_rex = "pressureplate.xp",
    vox = voxelId("proximity_sensor")
};

buildings["oscillator"] = {
    name = "Pulse Oscillator",
    description = "An electronic device that changes state on a steady pulse.",
    components = { { item="mechanism", qty=1 }, { item="circuit", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    provides = { oscillator={energy_cost=0} },
    render_rex = "pressureplate.xp",
    vox = voxelId("oscillator")
};

buildings["gate_and"] = {
    name = "Gate: AND",
    description = "An electronic device receives 2 or more signals, and is active if they are all true",
    components = { { item="mechanism", qty=1 }, { item="circuit", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    provides = { gate_and={energy_cost=0} },
    render_rex = "pressureplate.xp",
    vox = voxelId("gate_and")
};

buildings["gate_or"] = {
    name = "Gate: OR",
    description = "An electronic device receives 2 or more signals, and is active if any are true",
    components = { { item="mechanism", qty=1 }, { item="circuit", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    provides = { gate_or={energy_cost=0} },
    render_rex = "pressureplate.xp",
    vox = voxelId("gate_or")
};

buildings["gate_not"] = {
    name = "Gate: NOT",
    description = "An electronic device receives a signal and inverts it.",
    components = { { item="mechanism", qty=1 }, { item="circuit", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    provides = { gate_not={energy_cost=0} },
    render_rex = "pressureplate.xp",
    vox = voxelId("gate_not")
};

buildings["gate_nand"] = {
    name = "Gate: NAND",
    description = "An electronic device receives 2 or more signals, and is active if any are false.",
    components = { { item="mechanism", qty=1 }, { item="circuit", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    provides = { gate_nand={energy_cost=0} },
    render_rex = "pressureplate.xp",
    vox = voxelId("gate_nand")
};

buildings["gate_nor"] = {
    name = "Gate: NOR",
    description = "An electronic device receives 2 or more signals, and is active if all are false.",
    components = { { item="mechanism", qty=1 }, { item="circuit", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    provides = { gate_nor={energy_cost=0} },
    render_rex = "pressureplate.xp",
    vox = voxelId("gate_nor")
};

buildings["gate_xor"] = {
    name = "Gate: XOR",
    description = "An electronic device receives 2 or more signals, and is active if either but not all inputs are true.",
    components = { { item="mechanism", qty=1 }, { item="circuit", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    provides = { gate_xor={energy_cost=0} },
    render_rex = "pressureplate.xp",
    vox = voxelId("gate_xor")
};

buildings["support"] = {
    name = "Structural Support",
    description = "Cross-bracing that can hold considerable weight, and be remotely demolished.",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    provides = { support={energy_cost=0} },
    render_rex = "pressureplate.xp",
    vox = voxelId("support")
};
