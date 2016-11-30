#include "native_population.hpp"
#include <boost/container/flat_map.hpp>
#include "lua_bridge.hpp"
#include "../utils/string_utils.hpp"

boost::container::flat_map<std::string, std::vector<native_population_t>> native_pop_defs;

/*
 * Retrieve a list of native populations matching a given tag
 */
boost::optional<std::vector<native_population_t> &> get_native_professions(const std::string &tag) noexcept
{
    boost::optional<std::vector<native_population_t> &> result;

    auto finder = native_pop_defs.find(tag);
    if (finder != native_pop_defs.end()) {
        result = finder->second;
    }
    return result;
}

void read_native_population_types(std::ofstream &tech_tree_file) noexcept {
    lua_getglobal(lua_state, "native_populations");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        std::string key = lua_tostring(lua_state, -2);

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            native_population_t p;

            // We need to iterate the array
            lua_pushnil(lua_state);
            lua_gettable(lua_state, -2);
            while (lua_next(lua_state, -2) != 0) {

                std::string field = lua_tostring(lua_state, -2);
                if (field == "title") {
                    p.name = lua_tostring(lua_state, -1);
                }
                if (field == "aggression") p.aggression = lua_tonumber(lua_state, -1);
                if (field == "melee") p.melee = lua_tostring(lua_state, -1);
                if (field == "ranged") p.ranged = lua_tostring(lua_state, -1);
                if (field == "ammo") p.ammo = lua_tostring(lua_state, -1);
                // Stat mods
                if (field == "modifiers") {
                    lua_pushstring(lua_state, field.c_str());
                    lua_gettable(lua_state, -2);
                    while (lua_next(lua_state, -2) != 0) {
                        std::string stat = lua_tostring(lua_state, -2);
                        int modifier = lua_tonumber(lua_state, -1);
                        if (stat == "str") p.strength = modifier;
                        if (stat == "dex") p.dexterity = modifier;
                        if (stat == "con") p.constitution = modifier;
                        if (stat == "int") p.intelligence = modifier;
                        if (stat == "wis") p.wisdom = modifier;
                        if (stat == "cha") p.charisma = modifier;
                        if (stat == "com") p.comeliness = modifier;
                        if (stat == "eth") p.ethics = modifier;
                        lua_pop(lua_state, 1);
                    }
                }

                // Starting clothes
                if (field == "clothing") {
                    lua_pushstring(lua_state, field.c_str());
                    lua_gettable(lua_state, -2);
                    while (lua_next(lua_state, -2) != 0) {
                        const std::string gender_specifier = lua_tostring(lua_state, -2);
                        lua_pushstring(lua_state, gender_specifier.c_str());
                        lua_gettable(lua_state, -2);
                        while (lua_next(lua_state, -2) != 0) {
                            const std::string slot = lua_tostring(lua_state, -2);
                            const std::string item = lua_tostring(lua_state, -1);
                            int gender_tag = 0;
                            if (gender_specifier == "male") gender_tag = 1;
                            if (gender_specifier == "female") gender_tag = 2;
                            p.starting_clothes.push_back( std::make_tuple(gender_tag, slot, item));
                            lua_pop(lua_state, 1);
                        }
                        lua_pop(lua_state, 1);
                    }
                }

                lua_pop(lua_state, 1);
            }
            //std::cout << key << ":" << p.name << "\n";
            auto finder = native_pop_defs.find(key);
            if (finder == native_pop_defs.end()) {
                native_pop_defs[key] = { p };
            } else {
                finder->second.push_back(p);
            }
            lua_pop(lua_state, 1);
        }
        lua_pop(lua_state, 1);
    }
}

void sanity_check_natives() noexcept
{
    /*for (auto it = native_pop_defs.begin(); it != native_pop_defs.end(); ++it) {
        for (const auto &n : it->second) {
            if (n.name.empty()) std::cout << "WARNING: Native pop type with no name\n";
            for (const std::tuple< uint8_t, std::string, std::string > &cloth : n.starting_clothes) {
                if (std::get<0>(cloth) > 3) std::cout << "WARNING: " << n.name << " clothing item has invalid gender tag\n";
                if (std::get<1>(cloth) != "head" && std::get<1>(cloth) != "torso" && std::get<1>(cloth) != "legs" && std::get<1>(cloth) != "shoes") {
                    std::cout << "WARNING: " << n.name << " has an invalid slot: " << std::get<1>(cloth) << "\n";
                }

                if (!str_contains(std::get<2>(cloth), "/")) {
                    std::cout << "WARNING: No material specified for clothing belonging to " << n.name << "\n";
                } else {
                    auto cs = split(std::get<2>(cloth), '/');

                    auto finder = clothing_types.find(cs[0]);
                    if (finder == clothing_types.end()) std::cout << "WARNING: " << n.name << " has non-existent clothing type: " << std::get<2>(cloth) << "\n";

                    auto finder2 = get_material_by_tag(cs[1]);
                    if (!finder2) std::cout << "WARNING: " << n.name << " has non-existent clothing material: " << cs[1] << "\n";
                }
            }

            if (n.melee.empty()) std::cout << "WARNING: " << n.name << " has no melee weapon.\n";
            if (!n.melee.empty()) {
                auto cs = split(n.melee, '/');
                if (item_defs.find(cs[0]) == item_defs.end()) {
                    std::cout << "WARNING: " << n.name << " has an invalid melee weapon, " << n.melee << ".\n";
                }
                if (!get_material_by_tag(cs[1])) {
                    std::cout << "WARNING: " << n.name << " has an invalid melee material, " << n.melee << ".\n";
                }
            }

            if (!n.ranged.empty()) {
                auto cs = split(n.ranged, '/');
                if (item_defs.find(cs[0]) == item_defs.end()) {
                    std::cout << "WARNING: " << n.name << " has an invalid ranged weapon, " << n.ranged << ".\n";
                }
                if (!get_material_by_tag(cs[1])) {
                    std::cout << "WARNING: " << n.name << " has an invalid ranged material, " << n.ranged << ".\n";
                }
            }

            if (!n.ammo.empty()) {
                auto cs = split(n.ammo, '/');
                if (item_defs.find(cs[0]) == item_defs.end()) {
                    std::cout << "WARNING: " << n.name << " has an invalid ammo weapon, " << n.ammo << ".\n";
                }
                if (!get_material_by_tag(cs[1])) {
                    std::cout << "WARNING: " << n.name << " has an invalid ammo material, " << n.ammo << ".\n";
                }
            }
        }
    }*/
}