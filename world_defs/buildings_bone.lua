-- Boneworking

------------------------------------------------------------------------------------------------------------------------
-- Bonecarvers are used to turn bone into something useful.
------------------------------------------------------------------------------------------------------------------------

buildings["bonecarver"] = {
    name = "Bonecarver",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render_rex = "bonecarver.xp",
    vox=voxelId("bonecarver")    
};

