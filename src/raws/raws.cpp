#include "raws.hpp"
#include "lua_bridge.hpp"
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
#include "../global_assets/spatial_db.hpp"
#include "graphviz.hpp"
#include "defs/item_def_t.hpp"
#include "defs/material_def_t.hpp"
#include "defs/clothing_t.hpp"
#include "items.hpp"
#include "../render_engine/vox/renderables.hpp"

static std::unique_ptr<lua_lifecycle> lua_handle;

static void sanity_check_raws() noexcept {
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

static void load_game_tables() noexcept
{
    read_material_types();
    read_material_textures();
	read_voxel_models();
    read_clothing();
    read_life_events();
    read_professions();
    read_stockpiles();
    read_items();
    read_buildings();
    read_reactions();
    read_plant_types();
    read_biome_types();
    read_biome_textures();
    read_species_types();
    read_creature_types();

    sanity_check_raws();

    //if (game_config.build_tech_trees) {
    //    build_tech_tree_files();
    //}
}

void load_raws() noexcept {
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
	for (const auto &filename : string_table(-1)->strings) {
		load_lua_script("world_defs/" + filename);
	}
	//std::cout << "Loading tables\n";

	// Extract game tables
	load_game_tables();
}

void decorate_item_categories(bengine::entity_t &item, std::bitset<NUMBER_OF_ITEM_CATEGORIES> &categories) noexcept
{
	if (categories.test(TOOL_CHOPPING)) item.assign(item_chopping_t{});
	if (categories.test(TOOL_DIGGING)) item.assign(item_digging_t{});
	if (categories.test(WEAPON_MELEE)) item.assign(item_melee_t{});
	if (categories.test(WEAPON_RANGED)) item.assign(item_ranged_t{});
	if (categories.test(WEAPON_AMMO)) item.assign(item_ammo_t{});
	if (categories.test(ITEM_FOOD)) item.assign(item_food_t{});
	if (categories.test(ITEM_SPICE)) item.assign(item_spice_t{});
	if (categories.test(ITEM_DRINK)) item.assign(item_drink_t{});
	if (categories.test(ITEM_HIDE)) item.assign(item_hide_t{});
	if (categories.test(ITEM_BONE)) item.assign(item_bone_t{});
	if (categories.test(ITEM_SKULL)) item.assign(item_skull_t{});
	if (categories.test(ITEM_LEATHER)) item.assign(item_leather_t{});
	if (categories.test(ITEM_FARMING)) item.assign(item_farming_t{});
	if (categories.test(ITEM_SEED)) item.assign(item_seed_t{});
	if (categories.test(ITEM_TOPSOIL)) item.assign(item_topsoil_t{});
	if (categories.test(ITEM_FERTILIZER)) item.assign(item_fertilizer_t{});
	if (categories.test(ITEM_FOOD_PREPARED)) item.assign(item_food_prepared_t{});
}

void spawn_item_on_ground(const int x, const int y, const int z, const std::string &tag, const std::size_t &material, uint8_t quality, uint8_t wear, int creator_id, std::string creator_name) noexcept
{
    auto finder = get_item_def(tag);
	if (finder == nullptr) {
		glog(log_target::GAME, log_severity::warning, "Unknown item tag {0}", tag);
	}

    const auto mat = get_material(material);
	if (!mat) {
		glog(log_target::GAME, log_severity::warning, "Unknown material tag {0}", material);
	}

    const auto entity = bengine::create_entity()
        ->assign(position_t{ x,y,z })
        ->assign(renderable_t{ finder->glyph, finder->glyph_ascii, mat->fg, mat->bg, finder->voxel_model })
        ->assign(item_t{tag, finder->name, material, finder->stack_size, finder->clothing_layer})
		->assign(item_quality_t{ quality })
		->assign(item_wear_t{ wear })
		->assign(item_creator_t{ creator_id, creator_name });
	decorate_item_categories(*entity, finder->categories);

    //std::cout << "Spawned item on ground: " << entity->id << ", " << entity->component<item_t>()->item_tag << "\n";
    entity_octree.add_node(octree_location_t{x,y,z,entity->id});
	render::models_changed = true;
}

bengine::entity_t * spawn_item_on_ground_ret(const int x, const int y, const int z, const std::string &tag, const std::size_t &material, uint8_t quality, uint8_t wear, int creator_id, std::string creator_name) noexcept
{
    auto finder = get_item_def(tag);
	if (finder == nullptr) {
		glog(log_target::GAME, log_severity::warning, "Unknown item tag {0}", tag);
	}

    const auto mat = get_material(material);
	if (!mat) {
		glog(log_target::GAME, log_severity::warning, "Unknown material tag {0}", material);
	}

	const auto entity = bengine::create_entity()
		->assign(position_t{ x,y,z })
		->assign(renderable_t{ finder->glyph, finder->glyph_ascii, mat->fg, mat->bg, finder->voxel_model })
		->assign(item_t{ tag, finder->name, material, finder->stack_size, finder->clothing_layer })
		->assign(item_quality_t{ quality })
		->assign(item_wear_t{ wear })
		->assign(item_creator_t{ creator_id, creator_name });
	decorate_item_categories(*entity, finder->categories);
    entity_octree.add_node(octree_location_t{x,y,z,entity->id});
	render::models_changed = true;
    return entity;
}

void spawn_item_in_container(const int container_id, const std::string &tag, const std::size_t &material, uint8_t quality, uint8_t wear, int creator_id, std::string creator_name) noexcept
{
    auto finder = get_item_def(tag);
	if (finder == nullptr) {
		glog(log_target::GAME, log_severity::warning, "Unknown item tag {0}", tag);
	}

    const auto mat = get_material(material);

    //std::cout << "Spawning [" << tag << "], glyph " << +finder->glyph << "\n";

	const auto entity = bengine::create_entity()
		->assign(item_stored_t{ container_id })
		->assign(renderable_t{ finder->glyph, finder->glyph_ascii, mat->fg, mat->bg, finder->voxel_model })
		->assign(item_t{ tag, finder->name, material, finder->stack_size, finder->clothing_layer })
		->assign(item_quality_t{ quality })
		->assign(item_wear_t{ wear })
		->assign(item_creator_t{ creator_id, creator_name });
	decorate_item_categories(*entity, finder->categories);
}

void spawn_item_carried(const int holder_id, const std::string &tag, const std::size_t &material, const item_location_t &loc, uint8_t quality, uint8_t wear, int creator_id, std::string creator_name) noexcept
{
	const auto mat = get_material(material);
	const auto clothing_finder = get_clothing_by_tag(tag);
	if (clothing_finder) {
		// Clothing needs to be handled differently
		auto entity = bengine::create_entity()
			->assign(item_carried_t{ loc, holder_id })
			->assign(renderable_t{ clothing_finder->clothing_glyph, clothing_finder->clothing_glyph, mat->fg, mat->bg, clothing_finder->voxel_model })
			->assign(item_t{ tag })
			->assign(item_quality_t{ quality })
			->assign(item_wear_t{ wear })
			->assign(item_creator_t{ creator_id, creator_name });
		entity->component<item_t>()->material = material;
	} else {
		auto finder = get_item_def(tag);
		if (finder == nullptr) {
			glog(log_target::GAME, log_severity::warning, "Unknown item tag {0}", tag);
		}

		const auto entity = bengine::create_entity()
			->assign(item_carried_t{ loc, holder_id })
			->assign(renderable_t{ finder->glyph, finder->glyph_ascii, mat->fg, mat->bg, finder->voxel_model })
			->assign(item_t{ tag, finder->name, material, finder->stack_size, finder->clothing_layer })
			->assign(item_quality_t{ quality })
			->assign(item_wear_t{ wear })
			->assign(item_creator_t{ creator_id, creator_name });
		decorate_item_categories(*entity, finder->categories);
	}
}
