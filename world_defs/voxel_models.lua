function vfn(name) return "game_assets/vox/" .. name .. ".vox" end

voxel_models = {
    cryobed = { model=vfn("cryobed32"), id=1 },
    floorlight = { model=vfn("floorlight"), id=2 },
    rtg = { model=vfn("rtg"), id=3 },
    cabinet = { model=vfn("cabinet"), id=4 },
    battery = { model=vfn("battery"), id=5 },
    cordex = { model=vfn("cordex"), id=6 },
    replicator_small = { model=vfn("replicator-small"), id=7 },
    solar_panel = { model=vfn("solarpanel"), id=8 },
    wood_logs = { model=vfn("woodlogs"), id=9 },
    turret = { model=vfn("turret"), id=10 },
    axe = { model=vfn("simpleaxe"), id=11 },
    campfire_kit = { model=vfn("crate-fakefire"), id=12 },
    tent_kit = { model=vfn("crate-tent"), id=13 },
    fakefire = { model=vfn("campfire-fake"), id=14 },
    tent = { model=vfn("tent"), id=15 },
    sawmill = { model=vfn("sawmill"), id=16 },
    block = { model=vfn("block"), id=17 },
    woodwaste = { model=vfn("wastewood"), id=18 },
    bonecarver = { model=vfn("bonecarver"), id=19 },
    butcher = { model=vfn("butcher"), id=20 },
    carpenter = { model=vfn("carpenter"), id=21 },
    chemist = { model=vfn("chemist"), id=22 },
    stairs_up = { model=vfn("stairs-up"), id=23 },
    stairs_down = { model=vfn("stairs-down"), id=24 },
    stairs_updown = { model=vfn("stairs-updown"), id=25 },
    tailor = { model=vfn("tailor"), id=26 },
    forge = { model=vfn("forge"), id=27 },
    pick = { model=vfn("simplepick"), id=28 },
    boulder = { model=vfn("boulder"), id=29 },
    ore = { model=vfn("ore"), id=30 },
    powder = { model=vfn("powder"), id=31 },
    oil = { model=vfn("oil"), id=32 },
    stonecutter = { model=vfn("stonecutter"), id=33 },
    smelter = { model=vfn("smelter"), id=34 },
    deer = { model=vfn("deer"), id=35 },
    horse = { model=vfn("horse"), id=36 },
    armadillo = { model=vfn("armadillo"), id=37 },
    badger = { model=vfn("badger"), id=38 },
    kiln = { model=vfn("kiln"), id=39 },
    charcoal_hut = { model=vfn("charcoal_hut"), id=40 },
    still = { model=vfn("still"), id=41 },
    loom = { model=vfn("loom"), id=42 },
    glass_furnace = { model=vfn("glass_furnace"), id=43 },
    mason = { model=vfn("mason"), id=44 },
    mechanic = { model=vfn("mechanic"), id=45 },
    tanner = { model=vfn("tanner"), id=46 },
    charcoal = { model=vfn("charcoal"), id=47 },
}

function voxelId(name)
    if voxel_models[name] ~= nil then
        voxid = voxel_models[name].id
        print(name .. " : " .. voxid)
        return voxid
    else
        print("Unknown model: "..name)
    end
    return 0
end
