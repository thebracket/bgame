-- This file defines buildings relating to power generation or storage

buildings["solar_panel"] = {
    name = "Solar Panel",
    components = { { item="solar_panel_kit", qty=1} },
    skill = { name="Construction", difficulty=15 },
    provides = { generator={energy_cost=10} },
    render_rex = "solar_panel.xp",
    vox=voxelId("solar_panel")
};

buildings["battery"] = {
    name = "A battery bank",
    components = { { item="battery_kit", qty=1} },
    skill = { name="Construction", difficulty=10 },
    provides = { power_storage={energy_cost=20} },
    render_rex = "battery.xp",
    vox=voxelId("battery")
};

buildings["rtg"] = {
    name = "A small nuclear power generator",
    components = { {item="refined_plutonium", qty=1} },
    skill = { name="Construction", difficulty=15 },
    provides = { storage={energy_cost=0} },
    render_rex = "rtg.xp",
    vox=voxelId("rtg")
};
