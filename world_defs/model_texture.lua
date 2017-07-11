texture_index = {
    girder = { index = 1, texture = "girder_t.jpg", normal = "girder_n.jpg", specular="girder_s.jpg", bump="girder_d.jpg" },
    tiled_floor = { index = 2, texture = "tiled_floor_t.jpg", normal = "tiled_floor_n.jpg", specular="tiled_floor_s.jpg", bump="tiled_floor_d.jpg" },
    soil = { index = 3, texture = "soil_t.jpg", normal = "soil_n.jpg", specular="soil_s.jpg", bump="soil_d.jpg" },
    soil_constructed = { index = 4, texture = "soil_constructed_t.jpg", normal = "soil_constructed_n.jpg", specular="soil_constructed_s.jpg", bump="soil_constructed_d.jpg" },
    soil_wall = { index = 5, texture = "soil_wall_t.jpg", normal = "soil_wall_n.jpg", specular="soil_wall_s.jpg", bump="soil_wall_d.jpg" },
    sand = { index = 6, texture = "sand_t.jpg", normal = "sand_n.jpg", specular="sand_s.jpg", bump="sand_d.jpg" },
    silt = { index = 7, texture = "silt_t.jpg", normal = "silt_n.jpg", specular="silt_s.jpg", bump="silt_d.jpg" },
    grass = { index = 8, texture = "foliage_t.jpg", normal = "foliage_n.jpg", specular="foliage_s.jpg", bump="foliage_d.jpg" },
    steel = { index = 9, texture = "steel_t.jpg", normal = "steel_n.jpg", specular="steel_s.jpg", bump="steel_d.jpg" },
    bark = { index = 10, texture = "bark_t.jpg", normal = "bark_n.jpg", specular="bark_s.jpg", bump="bark_d.jpg" },
    foliage = { index = 11, texture = "foliage_t.jpg", normal = "foliage_n.jpg", specular="foliage_s.jpg", bump="foliage_d.jpg" },
    girder_floor = { index = 12, texture = "girderfloor_t.jpg", normal = "girderfloor_n.jpg", specular="girderfloor_s.jpg", bump="girderfloor_d.jpg" },
    rock_face = { index = 13, texture = "rock_face_t.jpg", normal = "rock_face_n.jpg", specular="rock_face_s.jpg", bump="rock_face_d.jpg" },
    rock_floor = { index = 14, texture = "rock_floor_t.jpg", normal = "rock_floor_n.jpg", specular="rock_floor_s.jpg", bump="rock_floor_d.jpg" },
    water = { index = 15, texture = "water_t.jpg", normal = "water_n.jpg", specular="water_s.jpg", bump="water_d.jpg" },
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