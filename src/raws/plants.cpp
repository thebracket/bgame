#include "plants.hpp"
#include "lua_bridge.hpp"
#include <boost/container/flat_map.hpp>

boost::container::flat_map<std::string, std::size_t> plant_defs_idx;
std::vector<plant_t> plant_defs;

boost::optional<std::size_t> get_plant_idx(const std::string &tag) noexcept
{
    boost::optional<std::size_t> result;
    auto finder = plant_defs_idx.find(tag);
    if (finder != plant_defs_idx.end()) {
        result = finder->second;
    }
    return result;
}

plant_t& get_plant_def(const std::size_t &index) noexcept
{
    return plant_defs[index];
}

std::vector<plant_t>& get_plant_defs() noexcept {
    return plant_defs;
}

void sanity_check_plants() noexcept
{
    for (const auto &p : plant_defs) {
        if (p.name.empty()) std::cout << "WARNING: No plant name\n";
    }
}

void read_plant_types(std::ofstream &tech_tree_file) noexcept
{
    lua_getglobal(lua_state, "vegetation");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        std::string key = lua_tostring(lua_state, -2);

        plant_t p;
        p.tag = key;
        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while(lua_next(lua_state, -2) != 0)
        {
            std::string field = lua_tostring(lua_state, -2);
            if (field == "name") p.name = lua_tostring(lua_state, -1);
            if (field == "glyph") p.glyph = lua_tonumber(lua_state, -1);
            if (field == "fg") p.fg = read_lua_color("fg");
            if (field == "bg") p.bg = read_lua_color("bg");
            if (field == "provides") p.provides = lua_tostring(lua_state, -1);

            lua_pop(lua_state, 1);
        }
        plant_defs.push_back(p);
        if (p.provides.size() > 1) {
            tech_tree_file << key << " -> farming -> item_" << p.provides << "\n";
        }
        lua_pop(lua_state, 1);
    }

    std::sort(plant_defs.begin(), plant_defs.end(), [] (plant_t a, plant_t b) { return a.tag < b.tag; });
    for (std::size_t i=0; i<plant_defs.size(); ++i) {
        plant_defs_idx[plant_defs[i].tag] = i;
    }
}