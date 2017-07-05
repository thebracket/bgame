texture_index = {
    girder = { index = 1, texture = "girder_t.jpg", normal = "girder_n.jpg" },
    tiled_floor = { index = 2, texture = "tiled_floor_t.jpg", normal = "tiled_floor_n.jpg" },
    soil = { index = 3, texture = "soil_t.jpg", normal = "soil_n.jpg" },
    soil_constructed = { index = 4, texture = "soil_constructed_t.jpg", normal = "soil_constructed_n.jpg" },
    soil_wall = { index = 5, texture = "soil_wall_t.jpg", normal = "soil_wall_n.jpg" },
    sand = { index = 6, texture = "sand_t.jpg", normal = "sand_n.jpg" },
    silt = { index = 7, texture = "silt_t.jpg", normal = "silt_n.jpg" },
    grass = { index = 8, texture = "grass_t.jpg", normal = "grass_n.jpg" },
    steel = { index = 9, texture = "steel_t.jpg", normal = "steel_n.jpg" },
    bark = { index = 10, texture = "bark_t.jpg", normal = "bark_n.jpg" },
    foliage = { index = 11, texture = "foliage_t.png", normal = "foliage_n.png" },
}

function tex(name)
    return texture_index[name].index
end

model_index = {
    ladder = { index = 1, model = "ladder_down.obj", texture = tex("steel") },
    battery_bank = { index = 2, model = "battery_bank.obj", texture = tex("steel") }
}

function model(name)
    return model_index[name].index
end