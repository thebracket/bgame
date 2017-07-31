#include "raws.hpp"
#include "lua_bridge.hpp"
//#include "../components/position.hpp"
//#include "../components/renderable.hpp"
//#include "../components/item.hpp"
//#include "../components/item_stored.hpp"
#include "string_table.hpp"
#include "creatures.hpp"
#include "species.hpp"
#include "biomes.hpp"
#include "plants.hpp"
#include "life_events.hpp"
#include "profession.hpp"
#include "buildings.hpp"
#include "reactions.hpp"
#include "clothing.hpp"
#include "materials.hpp"
//#include "../systems/ai/movement_system.hpp"
#include "graphviz.hpp"
//#include "../main/game_config.hpp"
#include "defs/item_def_t.hpp"
#include "defs/material_def_t.hpp"
#include "items.hpp"

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
    sanity_check_stockpiles();
}

void build_tech_tree_files() {
    std::cout << "Building DOT files\n";

    graphviz_t mats("material_tree.gv");
    build_material_acquisition_tech_tree(&mats);

    graphviz_t master("tech_tree.gv");
    build_material_tech_tree(&master);
    build_reaction_tree(&master);
    make_building_tree(&master);

    graphviz_t civs("civ_tree.gv");
    make_civ_tree(&civs);

    std::ofstream script(get_save_path() + std::string("/build-tech-tree.sh"));
    script << "#!/bin/bash\n";
    script << "dot -Tpng material_tree.gv -o material_tree.png\n";
    script << "dot -Tpng tech_tree.gv -o tech_tree.png\n";
    script << "dot -Tpng civ_tree.gv -o civ_tree.png\n";
    script.close();
}

void load_game_tables()
{
    read_material_types();
    read_clothing();
    read_life_events();
    read_professions();
    read_stockpiles();
    read_items();
    read_buildings();
    read_reactions();
    read_plant_types();
    read_biome_types();
    read_species_types();
    read_creature_types();

    sanity_check_raws();

    //if (game_config.build_tech_trees) {
    //    build_tech_tree_files();
    //}
}

void load_raws() {
    using namespace string_tables;

	// Load string tables for first names and last names
	load_string_table(FIRST_NAMES_MALE, "world_defs/first_names_male.txt");
	load_string_table(FIRST_NAMES_FEMALE, "world_defs/first_names_female.txt");
	load_string_table(LAST_NAMES, "world_defs/last_names.txt");
    load_string_table(NEW_ARRIVAL_QUIPS, "world_defs/newarrival.txt");
    load_string_table(MENU_SUBTITLES, "world_defs/menu_text.txt");

	// Setup LUA
	lua_handle = std::make_unique<lua_lifecycle>();

	// Load game data via LUA
	load_string_table(-1, "world_defs/index.txt");
	for (const std::string &filename : string_table(-1)->strings) {
		load_lua_script("world_defs/" + filename);
	}

	// Extract game tables
	load_game_tables();
}

/*
void spawn_item_on_ground(const int x, const int y, const int z, const std::string &tag, const std::size_t &material) {
    auto finder = get_item_def(tag);
    if (finder == nullptr) throw std::runtime_error(std::string("Unknown item tag: ") + tag);

    auto mat = get_material(material);
    if (!mat) throw std::runtime_error(std::string("Unknown material tag: ") + std::to_string(material));

    auto entity = create_entity()
        ->assign(position_t{ x,y,z })
        ->assign(renderable_t{ finder->glyph, finder->glyph_ascii, mat->fg, mat->bg })
        ->assign(item_t{tag, finder->name, finder->categories, material, finder->stack_size});
    //std::cout << "Spawned item on ground: " << entity->id << ", " << entity->component<item_t>()->item_tag << "\n";
    entity_octree.add_node(octree_location_t{x,y,z,entity->id});
}

entity_t * spawn_item_on_ground_ret(const int x, const int y, const int z, const std::string &tag, const std::size_t &material) {
    auto finder = get_item_def(tag);
    if (finder == nullptr) throw std::runtime_error(std::string("Unknown item tag: ") + tag);

    auto mat = get_material(material);
    if (!mat) throw std::runtime_error(std::string("Unknown material tag: ") + std::to_string(material));

    auto entity = create_entity()
            ->assign(position_t{ x,y,z })
            ->assign(renderable_t{ finder->glyph, finder->glyph_ascii, mat->fg, mat->bg })
            ->assign(item_t{tag, finder->name, finder->categories, material, finder->stack_size});
    entity_octree.add_node(octree_location_t{x,y,z,entity->id});
    return entity;
}

void spawn_item_in_container(const std::size_t container_id, const std::string &tag, const std::size_t &material) {
    auto finder = get_item_def(tag);
    if (finder == nullptr) throw std::runtime_error(std::string("Unknown item tag: ") + tag);

    auto mat = get_material(material);

    std::cout << "Spawning [" << tag << "], glyph " << +finder->glyph << "\n";

    create_entity()
        ->assign(item_stored_t{ container_id })
        ->assign(renderable_t{ finder->glyph, finder->glyph_ascii, mat->fg, mat->bg })
        ->assign(item_t{tag, finder->name, finder->categories, material, finder->stack_size});
}

void spawn_item_carried(const std::size_t holder_id, const std::string &tag, const std::size_t &material, const item_location_t &loc) {
    auto finder = get_item_def(tag);
    if (finder == nullptr) throw std::runtime_error(std::string("Unknown item tag: ") + tag);

    auto mat = get_material(material);

    create_entity()
        ->assign(item_carried_t{ loc, holder_id })
        ->assign(renderable_t{ finder->glyph, finder->glyph_ascii, mat->fg, mat->bg })
        ->assign(item_t{tag, finder->name, finder->categories, material, finder->stack_size});
}
*/