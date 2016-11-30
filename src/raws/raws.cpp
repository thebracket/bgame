#include "raws.hpp"
#include "lua_bridge.hpp"
#include "../utils/string_utils.hpp"
#include "../components/species.hpp"
#include "../components/position.hpp"
#include "../components/renderable.hpp"
#include "../components/item.hpp"
#include "../components/item_stored.hpp"
#include "string_table.hpp"
#include "materials.hpp"
#include "native_population.hpp"
#include "creatures.hpp"
#include "species.hpp"
#include "biomes.hpp"
#include "plants.hpp"
#include "life_events.hpp"
#include "clothing.hpp"
#include "profession.hpp"
#include "items.hpp"
#include "buildings.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>

std::unique_ptr<lua_lifecycle> lua_handle;

boost::container::flat_map<std::string, reaction_t> reaction_defs;
boost::container::flat_map<std::string, std::vector<std::string>> reaction_building_defs;

void read_reactions(std::ofstream &tech_tree_file) {
    lua_getglobal(lua_state, "reactions");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        reaction_t c;

        std::string key = lua_tostring(lua_state, -2);
        c.tag = key;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") c.name = lua_tostring(lua_state, -1);
            if (field == "emits_smoke") c.emits_smoke = lua_toboolean(lua_state, -1);
            if (field == "workshop") c.workshop = lua_tostring(lua_state, -1);
            if (field == "skill") c.skill = lua_tostring(lua_state, -1);
            if (field == "difficulty") c.difficulty = lua_tonumber(lua_state, -1);
            if (field == "inputs") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    lua_pushnil(lua_state);
                    lua_gettable(lua_state, -2);

                    reaction_input_t input;
                    input.required_material = boost::optional<std::size_t>{};
                    input.required_material_type = boost::optional<material_def_spawn_type_t>{};
                    while (lua_next(lua_state, -2) != 0) {
                        std::string f = lua_tostring(lua_state, -2);
                        if (f == "item") input.tag = lua_tostring(lua_state, -1);
                        if (f == "qty") input.quantity = lua_tonumber(lua_state, -1);
                        if (f == "material") {
                            std::string mat_name = lua_tostring(lua_state, -1);
                            auto matfinder = get_material_by_tag(mat_name);
                            if (!matfinder) {
                                std::cout << "WARNING: Reaction " << c.name << " references unknown material " << mat_name << "\n";
                            } else {
                                input.required_material = matfinder.get();
                            }
                        }
                        if (f == "mat_type") {
                            std::string type_s = lua_tostring(lua_state, -1);
                            if (type_s == "cluster_rock") {
                                input.required_material_type = cluster_rock;
                            } else if (type_s == "rock") {
                                input.required_material_type = rock;
                            } else if (type_s == "soil") {
                                input.required_material_type = soil;
                            } else if (type_s == "sand") {
                                input.required_material_type = sand;
                            } else if (type_s == "metal") {
                                input.required_material_type = metal;
                            } else if (type_s == "synthetic") {
                                input.required_material_type = synthetic;
                            } else if (type_s == "organic") {
                                input.required_material_type = organic;
                            } else if (type_s == "leather") {
                                input.required_material_type = leather;
                            } else {
                                std::cout << "WARNING: Unknown material type: " << type_s << "\n";
                            }
                        }
                        lua_pop(lua_state, 1);
                    }
                    c.inputs.push_back(input);

                    lua_pop(lua_state, 1);
                }
            }
            if (field == "outputs") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    lua_pushnil(lua_state);
                    lua_gettable(lua_state, -2);
                    std::pair<std::string, int> comp;
                    while (lua_next(lua_state, -2) != 0) {
                        std::string f = lua_tostring(lua_state, -2);
                        if (f == "item") comp.first = lua_tostring(lua_state, -1);
                        if (f == "qty") comp.second = lua_tonumber(lua_state, -1);
                        lua_pop(lua_state, 1);
                    }
                    c.outputs.push_back(comp);
                    //tech_tree_file << "\"" << key << "\"" << comp.first << "\"\n";

                    lua_pop(lua_state, 1);
                }
            }
            if (field == "automatic") c.automatic = lua_toboolean(lua_state, -1);
            if (field == "power_drain") c.power_drain = lua_tonumber(lua_state, -1);

            lua_pop(lua_state, 1);
        }
        reaction_defs[key] = c;
        reaction_building_defs[c.workshop].push_back(key);
        for (const auto &input : c.inputs) {
            tech_tree_file << "item_" << input.tag << " -> " << c.workshop << "\n";
        }
        for (const auto &output : c.outputs) {
            tech_tree_file << c.workshop << " -> item_" << output.first << "\n";
        }

        lua_pop(lua_state, 1);
    }
}

void sanity_check_reactions() {
    for (auto it=reaction_defs.begin(); it!=reaction_defs.end(); ++it) {
        if (it->first.empty()) std::cout << "WARNING: Empty reaction name\n";
        if (it->second.tag.empty()) std::cout << "WARNING: Empty reaction tag\n";
        if (it->second.name.empty()) std::cout << "WARNING: Empty reaction name, tag: " << it->first << "\n";
        if (it->second.workshop.empty()) std::cout << "WARNING: Empty workshop name, tag: " << it->first << "\n";
        auto bf = building_defs.find(it->second.workshop);
        if (bf == building_defs.end()) std::cout << "WARNING: Undefined workshop, tag: " << it->first << "\n";
        for (const auto &input : it->second.inputs) {
            auto finder = item_defs.find(input.tag);
            if (finder == item_defs.end()) std::cout << "WARNING: Unknown item tag in input: " << input.tag << ", reaction tag: " << it->first << "\n";
        }
        for (const auto &output : it->second.outputs) {
            auto finder = item_defs.find(output.first);
            auto finder2 = get_clothing_by_tag(output.first);
            if (finder == item_defs.end() && !finder2) std::cout << "WARNING: Unknown item tag in output: " << output.first << ", reaction tag: " << it->first << "\n";
        }
    }
}

void sanity_check_raws() {
    sanity_check_materials();
    sanity_check_clothing();
    sanity_check_professions();
    sanity_check_items();
    sanity_check_buildings();
    sanity_check_reactions();
    sanity_check_plants();
    sanity_check_biomes();
    sanity_check_species();
    sanity_check_creatures();
    sanity_check_natives();
}

void load_game_tables() {
    std::ofstream tech_tree_file("tech_tree.gv");
    tech_tree_file << "digraph G {\n";
    tech_tree_file << "\"cut trees\" -> wood_logs\n";

    read_material_types(tech_tree_file);
    read_clothing(tech_tree_file);
    read_life_events(tech_tree_file);
    read_professions(tech_tree_file);
    read_items(tech_tree_file);
    read_buildings(tech_tree_file);
    read_reactions(tech_tree_file);
    read_plant_types(tech_tree_file);
    read_biome_types(tech_tree_file);
    read_species_types(tech_tree_file);
    read_creature_types(tech_tree_file);
    read_native_population_types(tech_tree_file);

    tech_tree_file << "}\n";
    tech_tree_file.close();

    sanity_check_raws();
}

void load_raws() {
	// Load string tables for first names and last names
	load_string_table("world_defs/first_names_male.txt", first_names_male);
	load_string_table("world_defs/first_names_female.txt", first_names_female);
	load_string_table("world_defs/last_names.txt", last_names);

	// Setup LUA
	lua_handle = std::make_unique<lua_lifecycle>();

	// Load game data via LUA
	string_table_t raw_index;
	load_string_table("world_defs/index.txt", raw_index);
	for (const std::string &filename : raw_index.strings) {
		load_lua_script("world_defs/" + filename);
	}

	// Extract game tables
	load_game_tables();

    // Quit LUA
    lua_handle.reset();
}

void spawn_item_on_ground(const int x, const int y, const int z, const std::string &tag, const std::size_t &material) {
    auto finder = item_defs.find(tag);
    if (finder == item_defs.end()) throw std::runtime_error(std::string("Unknown item tag: ") + tag);

    auto mat = get_material(material);

    create_entity()
        ->assign(position_t{ x,y,z })
        ->assign(renderable_t{ finder->second.glyph, mat.get().fg, mat.get().bg })
        ->assign(item_t{tag, finder->second.name, finder->second.categories, material, finder->second.stack_size});
}

void spawn_item_in_container(const std::size_t container_id, const std::string &tag, const std::size_t &material) {
    auto finder = item_defs.find(tag);
    if (finder == item_defs.end()) throw std::runtime_error(std::string("Unknown item tag: ") + tag);

    auto mat = get_material(material);

    create_entity()
        ->assign(item_stored_t{ container_id })
        ->assign(renderable_t{ finder->second.glyph, mat.get().fg, mat.get().bg })
        ->assign(item_t{tag, finder->second.name, finder->second.categories, material, finder->second.stack_size});
}

void spawn_item_carried(const std::size_t holder_id, const std::string &tag, const std::size_t &material, const item_location_t &loc) {
    auto finder = item_defs.find(tag);
    if (finder == item_defs.end()) throw std::runtime_error(std::string("Unknown item tag: ") + tag);

    auto mat = get_material(material);

    create_entity()
        ->assign(item_carried_t{ loc, holder_id })
        ->assign(renderable_t{ finder->second.glyph, mat.get().fg, mat.get().bg })
        ->assign(item_t{tag, finder->second.name, finder->second.categories, material, finder->second.stack_size});
}
