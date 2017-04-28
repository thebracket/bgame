#include "raws.hpp"
#include "lua_bridge.hpp"
#include "../components/position.hpp"
#include "../components/renderable.hpp"
#include "../components/item.hpp"
#include "../components/item_stored.hpp"
#include "string_table.hpp"
#include "native_population.hpp"
#include "creatures.hpp"
#include "species.hpp"
#include "biomes.hpp"
#include "plants.hpp"
#include "life_events.hpp"
#include "profession.hpp"
#include "buildings.hpp"
#include "reactions.hpp"
#include "../systems/ai/movement_system.hpp"

std::unique_ptr<lua_lifecycle> lua_handle;

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
    sanity_check_stockpiles();
}

void load_game_tables() {
    std::ofstream tech_tree_file("tech_tree.gv");
    tech_tree_file << "digraph G {\n";
    tech_tree_file << "\"cut trees\" -> wood_logs\n";

    read_material_types(tech_tree_file);
    read_clothing(tech_tree_file);
    read_life_events(tech_tree_file);
    read_professions(tech_tree_file);
    read_stockpiles();
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
    load_string_table("world_defs/newarrival.txt", new_arrival_quips);

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
}

void spawn_item_on_ground(const int x, const int y, const int z, const std::string &tag, const std::size_t &material) {
    auto finder = item_defs.find(tag);
    if (finder == item_defs.end()) throw std::runtime_error(std::string("Unknown item tag: ") + tag);

    auto mat = get_material(material);
    if (!mat) throw std::runtime_error(std::string("Unknown material tag: ") + std::to_string(material));

    auto entity = create_entity()
        ->assign(position_t{ x,y,z })
        ->assign(renderable_t{ finder->second.glyph, finder->second.glyph_ascii, mat->fg, mat->bg })
        ->assign(item_t{tag, finder->second.name, finder->second.categories, material, finder->second.stack_size});
    //std::cout << "Spawned item on ground: " << entity->id << ", " << entity->component<item_t>()->item_tag << "\n";
    entity_octree.add_node(octree_location_t{x,y,z,entity->id});
}

entity_t * spawn_item_on_ground_ret(const int x, const int y, const int z, const std::string &tag, const std::size_t &material) {
    auto finder = item_defs.find(tag);
    if (finder == item_defs.end()) throw std::runtime_error(std::string("Unknown item tag: ") + tag);

    auto mat = get_material(material);
    if (!mat) throw std::runtime_error(std::string("Unknown material tag: ") + std::to_string(material));

    auto entity = create_entity()
            ->assign(position_t{ x,y,z })
            ->assign(renderable_t{ finder->second.glyph, finder->second.glyph_ascii, mat->fg, mat->bg })
            ->assign(item_t{tag, finder->second.name, finder->second.categories, material, finder->second.stack_size});
    entity_octree.add_node(octree_location_t{x,y,z,entity->id});
    return entity;
}

void spawn_item_in_container(const std::size_t container_id, const std::string &tag, const std::size_t &material) {
    auto finder = item_defs.find(tag);
    if (finder == item_defs.end()) throw std::runtime_error(std::string("Unknown item tag: ") + tag);

    auto mat = get_material(material);

    std::cout << "Spawning [" << tag << "], glyph " << +finder->second.glyph << "\n";

    create_entity()
        ->assign(item_stored_t{ container_id })
        ->assign(renderable_t{ finder->second.glyph, finder->second.glyph_ascii, mat->fg, mat->bg })
        ->assign(item_t{tag, finder->second.name, finder->second.categories, material, finder->second.stack_size});
}

void spawn_item_carried(const std::size_t holder_id, const std::string &tag, const std::size_t &material, const item_location_t &loc) {
    auto finder = item_defs.find(tag);
    if (finder == item_defs.end()) throw std::runtime_error(std::string("Unknown item tag: ") + tag);

    auto mat = get_material(material);

    create_entity()
        ->assign(item_carried_t{ loc, holder_id })
        ->assign(renderable_t{ finder->second.glyph, finder->second.glyph_ascii, mat->fg, mat->bg })
        ->assign(item_t{tag, finder->second.name, finder->second.categories, material, finder->second.stack_size});
}
