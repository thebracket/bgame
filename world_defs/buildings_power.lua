-- This file defines buildings relating to power generation or storage

buildings["solar_panel"] = {
    name = "Solar Panel",
    description = "An array of cells that convert solar energy into useful power, and wirelessly transmits it into Cordex's power banks.",
    components = { { item="solar_panel_kit", qty=1} },
    skill = { name="Construction", difficulty=15 },
    provides = { generator={energy_cost=10} },
    render_rex = "solar_panel.xp",
    vox=voxelId("solar_panel")
};

buildings["battery"] = {
    name = "A battery bank",
    description = "An array of large batteries, used for storing power.",
    components = { { item="battery_kit", qty=1} },
    skill = { name="Construction", difficulty=10 },
    provides = { power_storage={energy_cost=20} },
    render_rex = "battery.xp",
    vox=voxelId("battery")
};

buildings["rtg"] = {
    name = "A small nuclear power generator",
    description = "Highly enriched plutonium in a shielded core, gently warming water and producing power through the Sterling effect.",
    components = { {item="refined_plutonium", qty=1} },
    skill = { name="Construction", difficulty=15 },
    provides = { storage={energy_cost=0} },
    render_rex = "rtg.xp",
    vox=voxelId("rtg")
};
