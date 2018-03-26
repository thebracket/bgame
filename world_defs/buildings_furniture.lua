-- This file defines furniture buildings.

buildings["cryo_bed"] = {
    name = "Cryogenic Bed",
    description = "An uncomfortable cryogenic bed, designed for long trips through the stars. It can double as a regular bed if you don't want to freeze anyone.",
    components = { { item="cryo_bed_kit", qty=1} },
    skill = { name="Construction", difficulty=10 },
    provides = { sleep={energy_cost=0} },
    render_rex = "cryobed.xp",
    vox=voxelId("cryobed")
};

buildings["storage_locker"] = {
    name = "Storage locker",
    description = "A ship-mounted storage locker, pre-shipped with enough to survive settling a new world.",
    components = { { item="storage_locker", qty=1} },
    skill = { name="Construction", difficulty=10 },
    provides = { storage={energy_cost=0} },
    render_rex = "cabinet.xp",
    vox=voxelId("cabinet")
};

buildings["personal_survival_shelter"] = {
    name = "Tent",
    description = "A tent, conveniently packaged in a crate. It is self-assembling.",
    components = { { item="personal_survival_shelter_kit", qty=1 } },
    skill = { name="Construction", difficulty=5 },
    provides = { sleep={energy_cost=0} },
    render_rex = "tent.xp",
    vox=voxelId("tent")
};

buildings["table"] = {
    name = "Table",
    description = "A simple table",
    components = { { item="table", qty=1 } },
    skill = { name="Construction", difficulty=5 },
    provides = { table={energy_cost=0} },
    render_rex = "table.xp",
    vox=voxelId("table")
};

buildings["chair"] = {
    name = "Chair",
    description = "A simple char",
    components = { { item="chair", qty=1 } },
    skill = { name="Construction", difficulty=5 },
    provides = { table={energy_cost=0} },
    render_rex = "chair.xp",
    vox=voxelId("chair")
};

buildings["door"] = {
    name = "Door",
    description = "A simple door",
    components = { { item="door", qty=1 } },
    skill = { name="Construction", difficulty=10 },
    provides = { door={energy_cost=0, alternate_vox=voxelId("door_closed")}, signal={energy_cost=0} },
    render_rex = "door.xp",
    vox=voxelId("door_open")
};

buildings["energy_door"] = {
    name = "Force Door",
    description = "A force-field door, ideal for keeping out bad guys",
    components = { { item="door", qty=1 } },
    skill = { name="Construction", difficulty=10 },
    provides = { door={energy_cost=10, alternate_vox=voxelId("energy_door_closed")} },
    render_rex = "door.xp",
    vox=voxelId("energy_door_open")
};

buildings["floodgate"] = {
    name = "Floodgate",
    description = "A heavy floodgate, designed to keep water out.",
    components = { { item="floodgate", qty=1 } },
    skill = { name="Construction", difficulty=10 },
    provides = { door={energy_cost=10, alternate_vox=voxelId("floodgate_closed")} },
    render_rex = "door.xp",
    vox=voxelId("floodgate_open")
};

buildings["bed_simple"] = {
    name = "Simple Bed",
    description = "A simple bed frame with straw matress. Not very comfortable, and prone to weevils.",
    components = { { item="bed_item", qty=1} },
    skill = { name="Construction", difficulty=10 },
    provides = { sleep={energy_cost=0} },
    render_rex = "cryobed.xp",
    vox=voxelId("bed")
};
