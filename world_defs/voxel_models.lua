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
    ant = { model=vfn("ant"), id=48 },

    person_base = { model=vfn("person_base"), id=49 },
    person_hair_short = { model=vfn("person_hair_short"), id=50 },
    person_hair_long = { model=vfn("person_hair_long"), id=51 },
    person_hair_pigtails = { model=vfn("person_hair_pigtails"), id=52 },
    person_hair_mohawk = { model=vfn("person_hair_mohawk"), id=53 },
    person_hair_balding = { model=vfn("person_hair_balding"), id=54 },
    person_hair_triangle = { model=vfn("person_hair_triangle"), id=55 },

    clothes_batman_hat = { model=vfn("clothes_batman_hat"), id=56 },
    clothes_police_hat = { model=vfn("clothes_police_hat"), id=57 },
    clothes_cat_ears = { model=vfn("clothes_cat_ears"), id=58 },
    clothes_tiara = { model=vfn("clothes_tiara"), id=59 },
    clothes_formal_shirt = { model=vfn("clothes_formal_shirt"), id=60 },
    clothes_bathrobe = { model=vfn("clothes_bathrobe"), id=61 },
    clothes_spandex_shirt = { model=vfn("clothes_spandex_shirt"), id=62 },
    clothes_sports_shirt = { model=vfn("clothes_sports_shirt"), id=63 },
    clothes_tshirt = { model=vfn("clothes_tshirt"), id=64 },
    clothes_cargopants = { model=vfn("clothes_cargopants"), id=65 },
    clothes_longskirt = { model=vfn("clothes_longskirt"), id=66 },
    clothes_hawaiian_shorts = { model=vfn("clothes_hawaiian_shorts"), id=67 },
    clothes_spandex_pants = { model=vfn("clothes_spandex_pants"), id=68 },
    clothes_miniskirt = { model=vfn("clothes_miniskirt"), id=69 },
    clothes_shoes = { model=vfn("clothes_shoes"), id=70 },
    clothes_boots = { model=vfn("clothes_boots"), id=71 },
    
}

function voxelId(name)
    if voxel_models[name] ~= nil then
        voxid = voxel_models[name].id
        --print(name .. " : " .. voxid)
        return voxid
    else
        print("Unknown model: "..name)
    end
    return 0
end
