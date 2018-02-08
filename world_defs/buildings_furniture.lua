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
    name = "A storage locker",
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
    components = { { item="table", qty=1 } },
    skill = { name="Construction", difficulty=5 },
    provides = { table={energy_cost=0} },
    render_rex = "table.xp",
    vox=voxelId("table")
};

buildings["chair"] = {
    name = "Chair",
    components = { { item="chair", qty=1 } },
    skill = { name="Construction", difficulty=5 },
    provides = { table={energy_cost=0} },
    render_rex = "chair.xp",
    vox=voxelId("chair")
};

buildings["door"] = {
    name = "Door",
    components = { { item="door", qty=1 } },
    skill = { name="Construction", difficulty=10 },
    provides = { door={energy_cost=0}, signal={energy_cost=0} },
    render_rex = "door.xp",
    vox=voxelId("door_ground")
};

buildings["energy_door"] = {
    name = "Force Door",
    components = { { item="door", qty=1 } },
    skill = { name="Construction", difficulty=10 },
    provides = { door={energy_cost=10} },
    render_rex = "door.xp",
    vox=voxelId("door_ground")
};

buildings["bed_simple"] = {
    name = "Simple Bed",
    components = { { item="bed_item", qty=1} },
    skill = { name="Construction", difficulty=10 },
    provides = { sleep={energy_cost=0} },
    render_rex = "cryobed.xp",
    vox=voxelId("bed")
};
