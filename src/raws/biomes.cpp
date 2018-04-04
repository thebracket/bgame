#include "biomes.hpp"
#include "lua_bridge.hpp"
#include "creatures.hpp"
#include "defs/biome_type_t.hpp"
#include <map>
#include "../utils/system_log.hpp"

std::vector<biome_type_t> biome_defs;
std::vector<std::string> biome_textures;

biome_type_t * get_biome_def(const std::size_t &index) noexcept {
    return &biome_defs[index];
}

void each_biome(const std::function<void(biome_type_t *)> &func) noexcept {
    for (auto &b : biome_defs) {
        func(&b);
    }
}

void sanity_check_biomes() noexcept
{
    for (const auto &b : biome_defs) {
        if (b.name.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Empty biome name");
		if (b.occurs.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Biome {0} has no occurences! ", b.name);
        for (const auto &o : b.occurs) {
            if (o <1 || o > 10)  glog(log_target::LOADER, log_severity::warning, "WARNING: Biome {0} has invalid occurences! ", b.name);
        }
        if (b.plants.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Biome {0} has no plants! ", b.name);
        for (const auto &p : b.plants) {
            if (p.first == "none") break;
            /*auto finder = plant_defs_idx.find(p.first);
            if (finder == plant_defs_idx.end()) {
                std::cout << "WARNING: Biome " << b.name << " has invalid plant: " << p.first << "\n";
            } else {
                if (finder->second > plant_defs.size()) std::cout << "WARNING: Biome " << b.name << " has invalid plant: " << p.first << "\n";
            }*/
        }
        if (b.wildlife.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Biome {0} has no wildlife! ", b.name);
        for (const auto &w : b.wildlife) {
			if (!get_creature_def(w)) glog(log_target::LOADER, log_severity::info, "WARNING: Biome {0} has invalid wildlife {1}", b.name, w);
        }
    }
}

void read_biome_types() noexcept
{
    lua_getglobal(lua_state, "biomes");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        std::string key = lua_tostring(lua_state, -2);

        biome_type_t b;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") b.name = lua_tostring(lua_state, -1);
            if (field == "min_temp") b.min_temp = static_cast<int8_t>(lua_tonumber(lua_state, -1));
            if (field == "max_temp") b.max_temp = static_cast<int8_t>(lua_tonumber(lua_state, -1));
            if (field == "min_rain") b.min_rain = static_cast<int8_t>(lua_tonumber(lua_state, -1));
            if (field == "max_rain") b.max_rain = static_cast<int8_t>(lua_tonumber(lua_state, -1));
            if (field == "min_mutation") b.min_mutation = static_cast<uint8_t>(lua_tonumber(lua_state, -1));
            if (field == "max_mutation") b.max_mutation = static_cast<uint8_t>(lua_tonumber(lua_state, -1));
            if (field == "worldgen_texture_index") b.worldgen_texture_index = static_cast<unsigned int>(lua_tonumber(lua_state, -1));
            if (field == "soils") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    const std::string soil_type = lua_tostring(lua_state, -2);
                    if (soil_type == "soil" ) b.soil_pct = static_cast<uint8_t>(lua_tonumber(lua_state, -1));
                    if (soil_type == "sand" ) b.soil_pct = static_cast<uint8_t>(lua_tonumber(lua_state, -1));
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "occurs") {
                // List of biome type indices
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    b.occurs.push_back(static_cast<uint8_t>(lua_tonumber(lua_state, -1)));
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "plants") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string plant_name = lua_tostring(lua_state, -2);
                    auto frequency = static_cast<int>(lua_tonumber(lua_state, -1));
                    b.plants.emplace_back(std::make_pair(plant_name, frequency));
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "trees") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    const std::string tree_type = lua_tostring(lua_state, -2);
                    const auto frequency = static_cast<int>(lua_tonumber(lua_state, -1));
                    if (tree_type == "deciduous") b.deciduous_tree_chance = frequency;
                    if (tree_type == "evergreen") b.evergreen_tree_chance = frequency;
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "wildlife") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    const std::string critter = lua_tostring(lua_state, -1);
                    b.wildlife.push_back(critter);
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "nouns") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    const std::string noun = lua_tostring(lua_state, -1);
                    b.nouns.push_back(noun);
                    lua_pop(lua_state, 1);
                }
            }

            lua_pop(lua_state, 1);
        }

        biome_defs.push_back(b);

        lua_pop(lua_state, 1);
    }
}

void read_biome_textures() noexcept {
    std::map<int, std::pair<std::string, std::string>> tmp_tex;

    lua_getglobal(lua_state, "biome_textures");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0) {
        std::string key = lua_tostring(lua_state, -2);
        //std::cout << key << "\n";

        auto idx = 0;
        std::string tex;
        std::string norm;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            const std::string field = lua_tostring(lua_state, -2);
            //std::cout << field << "\n";

            if (field == "index") idx = static_cast<int>(lua_tonumber(lua_state, -1));
            if (field == "texture") tex = lua_tostring(lua_state, -1);
            if (field == "normal") norm = lua_tostring(lua_state, -1);

            lua_pop(lua_state, 1);
        }
        tmp_tex[idx] = std::make_pair(tex, norm);

        lua_pop(lua_state, 1);
    }

    biome_textures.clear();
	for (auto &i : tmp_tex) {
        biome_textures.emplace_back(i.second.first);
        biome_textures.emplace_back(i.second.second);
    }
}
