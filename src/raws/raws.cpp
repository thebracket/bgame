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
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>

std::unique_ptr<lua_lifecycle> lua_handle;

boost::container::flat_map<std::string, clothing_t> clothing_types;
std::vector<profession_t> starting_professions;

boost::container::flat_map<std::string, item_def_t> item_defs;
boost::container::flat_map<std::string, building_def_t> building_defs;
boost::container::flat_map<std::string, reaction_t> reaction_defs;
boost::container::flat_map<std::string, std::vector<std::string>> reaction_building_defs;

void read_clothing(std::ofstream &tech_tree_file) {
    std::string tag = "";
    clothing_t c;
    read_lua_table("clothing", 
        [&c, &tag] (const auto &key) { tag = key; c = clothing_t{}; },
        [&c, &tag] (const auto &key) { clothing_types[key] = c; }, 
        lua_parser{
            {"name",        [&c] () { c.name = lua_str(); }},
            {"slot",        [&c] () { c.slot = lua_str(); }},
            {"description", [&c] () { c.description = lua_str(); }},
            {"ac",          [&c] () { c.armor_class = lua_float(); }},
            {"colors",      [&c] () {
                read_lua_table_inner( "colors", [&c] (auto col) { c.colors.push_back(col); });
            }}
        }
    );
}

void read_professions(std::ofstream &tech_tree_file) {

    std::string tag;
    profession_t p;

    read_lua_table("starting_professions",
        [&p, &tag] (const auto &key) { tag = key; p = profession_t{}; },
        [&p, &tag] (const auto &key) { starting_professions.push_back(p); },
        lua_parser{
            {"name", [&p] () { p.name = lua_str(); }},
            {"modifiers", [&p] ()     {
                read_stat_modifiers("modifiers", p);
            }},
            {"clothing", [&p] ()      {
                lua_pushstring(lua_state, "clothing");
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
            }}
        }
    );
}

void read_items(std::ofstream &tech_tree_file) {

    std::string tag;
    item_def_t c;

    read_lua_table("items",
        [&c, &tag] (const auto &key) { tag=key; c=item_def_t{}; c.tag = tag; },
        [&c, &tag] (const auto &key) { item_defs[tag] = c; },
        lua_parser{
            { "name", [&c] ()           { c.name = lua_str(); }},
            { "description", [&c] ()    { c.description = lua_str(); }},
            { "background", [&c] ()     { c.bg = read_lua_color("background"); }},
            { "foreground", [&c] ()     { c.bg = read_lua_color("foreground"); }},
            { "glyph", [&c] ()          { c.glyph = lua_int(); }},
            { "damage_n", [&c] ()       { c.damage_n = lua_int(); }},
            { "damage_d", [&c] ()       { c.damage_d = lua_int(); }},
            { "damage_mod", [&c] ()     { c.damage_mod = lua_int(); }},
            { "damage_stat", [&c] ()    { c.damage_stat = lua_str(); }},
            { "ammo", [&c] ()           { c.ammo = lua_str(); }},
            { "range", [&c] ()          { c.range = lua_int(); }},
            { "stack_size", [&c] ()     { c.stack_size = lua_int(); }},
            { "initiative_penalty", [&c] ()    { c.initiative_penalty = lua_int(); }},
            { "itemtype", [&c] () {
                read_lua_table_inner( "itemtype", [&c] (auto type) { 
                    if (type == "component") c.categories.set(COMPONENT);
                    if (type == "tool-chopping") c.categories.set(TOOL_CHOPPING);
                    if (type == "tool-digging") c.categories.set(TOOL_DIGGING);
                    if (type == "weapon-melee") c.categories.set(WEAPON_MELEE);
                    if (type == "weapon-ranged") c.categories.set(WEAPON_RANGED);
                    if (type == "ammo") c.categories.set(WEAPON_AMMO);
                 });
            }}
        }
    );
}

void read_buildings(std::ofstream &tech_tree_file) {
    lua_getglobal(lua_state, "buildings");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        building_def_t c;

        std::string key = lua_tostring(lua_state, -2);
        c.tag = key;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") c.name = lua_tostring(lua_state, -1);
            if (field == "emits_smoke") c.emits_smoke = lua_toboolean(lua_state, -1);
            if (field == "components") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {

                    lua_pushnil(lua_state);
                    lua_gettable(lua_state, -2);

                    reaction_input_t comp;
                    comp.required_material = boost::optional<std::size_t>{};
                    comp.required_material_type = boost::optional<material_def_spawn_type_t>{};
                    while (lua_next(lua_state, -2) != 0) {
                        std::string f = lua_tostring(lua_state, -2);

                        if (f == "item") comp.tag = lua_tostring(lua_state, -1);
                        if (f == "qty") comp.quantity = lua_tonumber(lua_state, -1);
                        if (f == "material") {
                            std::string mat_name = lua_tostring(lua_state, -1);
                            auto matfinder = get_material_by_tag(mat_name);
                            if (!matfinder) {
                                std::cout << "WARNING: Reaction " << c.name << " references unknown material " << mat_name << "\n";
                            } else {
                                comp.required_material = matfinder.get();
                            }
                        }
                        if (f == "mat_type") {
                            std::string type_s = lua_tostring(lua_state, -1);
                            if (type_s == "cluster_rock") {
                                comp.required_material_type = cluster_rock;
                            } else if (type_s == "rock") {
                                comp.required_material_type = rock;
                            } else if (type_s == "soil") {
                                comp.required_material_type = soil;
                            } else if (type_s == "sand") {
                                comp.required_material_type = sand;
                            } else if (type_s == "metal") {
                                comp.required_material_type = metal;
                            } else if (type_s == "synthetic") {
                                comp.required_material_type = synthetic;
                            } else if (type_s == "organic") {
                                comp.required_material_type = organic;
                            } else if (type_s == "leather") {
                                comp.required_material_type = leather;
                            } else {
                                std::cout << "WARNING: Unknown material type: " << type_s << "\n";
                            }
                        }
                        lua_pop(lua_state, 1);
                    }

                    c.components.push_back(comp);
                    tech_tree_file << "item_" << comp.tag << " -> " <<  key << "\n";
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "skill") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string type = lua_tostring(lua_state, -2);
                    if (type == "name") c.skill.first = lua_tostring(lua_state, -1);
                    if (type == "difficulty") c.skill.second = lua_tonumber(lua_state, -1);
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "provides") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);

                while (lua_next(lua_state, -2) != 0) {
                    building_provides_t provisions;
                    std::string type = lua_tostring(lua_state, -2);
                    if (type == "table") provisions.provides = provides_desk;
                    if (type == "wall") provisions.provides = provides_wall;
                    if (type == "door") provisions.provides = provides_door;
                    if (type == "food") provisions.provides = provides_food;
                    if (type == "sleep") provisions.provides = provides_sleep;
                    if (type == "floor") provisions.provides = provides_floor;
                    if (type == "stairs_up") provisions.provides = provides_stairs_up;
                    if (type == "stairs_down") provisions.provides = provides_stairs_down;
                    if (type == "stairs_updown") provisions.provides = provides_stairs_updown;
                    if (type == "ramp") provisions.provides = provides_ramp;
                    if (type == "light") provisions.provides = provides_light;

                    lua_pushstring(lua_state, type.c_str());
                    lua_gettable(lua_state, -2);
                    while (lua_next(lua_state, -2) != 0) {
                        std::string inner_type = lua_tostring(lua_state, -2);
                        if (inner_type == "energy_cost") provisions.energy_cost = lua_tonumber(lua_state, -1);
                        if (inner_type == "radius") provisions.radius = lua_tonumber(lua_state, -1);
                        if (inner_type == "color") provisions.color = read_lua_color("color");
                        lua_pop(lua_state, 1);
                    }

                    c.provides.push_back(provisions);
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "render") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string type = lua_tostring(lua_state, -2);
                    if (type == "width") c.width = lua_tonumber(lua_state, -1);
                    if (type == "height") c.height = lua_tonumber(lua_state, -1);
                    if (type == "tiles") {
                        lua_pushstring(lua_state, type.c_str());
                        lua_gettable(lua_state, -2);
                        int i = 0;
                        while (lua_next(lua_state, -2) != 0) {
                            rltk::vchar render;
                            lua_pushnumber(lua_state, i);
                            lua_gettable(lua_state, -2);
                            while (lua_next(lua_state, -2) != 0) {
                                std::string tiletag = lua_tostring(lua_state, -2);
                                if (tiletag == "glyph") render.glyph = lua_tonumber(lua_state, -1);
                                if (tiletag == "foreground") render.foreground = read_lua_color("foreground");
                                if (tiletag == "background") render.background = read_lua_color("background");
                                lua_pop(lua_state, 1);
                            }                       
                            lua_pop(lua_state, 1);
                            ++i;
                            c.glyphs.push_back(render);
                        }
                    }
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "render_rex") {
                std::string filename = "rex/" + std::string(lua_tostring(lua_state, -1));
                xp::rex_sprite sprite(filename);
                c.width = sprite.get_width();
                c.height = sprite.get_height();
                for (int y=0; y<c.height; ++y) {
                    for (int x=0; x<c.width; ++x) {
                        c.glyphs.push_back(*sprite.get_tile(0,x,y));
                    }
                }
            }

            lua_pop(lua_state, 1);
        }
        building_defs[key] = c;
        lua_pop(lua_state, 1);
    }
}

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

void sanity_check_clothing() {
    for (auto it = clothing_types.begin(); it != clothing_types.end(); ++it) {
        if (it->first.empty()) std::cout << "WARNING: Empty clothing string\n";
        if (it->second.name.empty()) std::cout << "WARNING: Empty clothing name\n";
        if (it->second.colors.empty()) std::cout << "WARNING: " << it->first << " contains no color options\n";
        if (it->second.slot.empty()) std::cout << "WARNING: " << it->first << " has no slot defined.\n";         
    }
}

void sanity_check_professions() {
    for (const auto &prof : starting_professions) {
        if (prof.name.empty()) std::cout << "WARNING: Profession with no name\n";
        for (const std::tuple< uint8_t, std::string, std::string > &cloth : prof.starting_clothes) {
            if (std::get<0>(cloth) > 3) std::cout << "WARNING: " << prof.name << " clothing item has invalid gender tag\n";
            if (std::get<1>(cloth) != "head" && std::get<1>(cloth) != "torso" && std::get<1>(cloth) != "legs" && std::get<1>(cloth) != "shoes")
                std::cout << "WARNING: " << prof.name << " has an invalid slot: " << std::get<1>(cloth) << "\n";
            auto finder = clothing_types.find(std::get<2>(cloth));
            if (finder == clothing_types.end()) std::cout << "WARNING: " << prof.name << " has non-existent clothing type: " << std::get<2>(cloth) << "\n";
        }
    }
}

void sanity_check_items() {
    for (auto it=item_defs.begin(); it!=item_defs.end(); ++it) {
        if (it->first.empty()) std::cout << "WARNING: Item has no name\n";
        if (it->second.tag.empty()) std::cout << "WARNING: Empty item tag\n";
        if (it->second.name.empty()) std::cout << "WARNING: Empty item name, tag: " << it->second.tag << "\n";
    }
}

void sanity_check_buildings() {
    for (auto it=building_defs.begin(); it!=building_defs.end(); ++it) {
        if (it->first.empty()) std::cout << "WARNING: Empty building tag\n";
        if (it->second.name.empty()) std::cout << "WARNING: Building " << it->first << " has no name.\n";
        for (const reaction_input_t &comp : it->second.components) {
            if (comp.tag.empty()) std::cout << "WARNING: Empty component for building: " << it->first << "\n";
            auto finder = item_defs.find(comp.tag);
            if (finder == item_defs.end()) {
                std::cout << "WARNING: No item definition for component " << comp.tag << ", for building: " << it->first << "\n";
            }
        }
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
            auto finder2 = clothing_types.find(output.first);
            if (finder == item_defs.end() && finder2 == clothing_types.end()) std::cout << "WARNING: Unknown item tag in output: " << output.first << ", reaction tag: " << it->first << "\n";
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
