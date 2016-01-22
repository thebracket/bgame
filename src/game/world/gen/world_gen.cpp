#include "world_gen.hpp"
#include "world_height_map.hpp"
#include "world_gen_biomes.hpp"
#include "world_gen_hydrology.hpp"
#include "../../../engine/rng.hpp"
#include "world_gen_layer_cake.hpp"
#include "world_gen_noise.hpp"
#include <iostream>
#include "../../game.h"
#include "../universe.hpp"
#include "../../raws/raws.h"
#include "../../../engine/colors.hpp"
#include "world_gen_settlers.hpp"

using engine::entity;
using namespace engine;

inline void really_hollow(const location_t &loc)
{
	tile_t * tile = world::planet->get_tile(loc);
	tile->flags.reset(TILE_OPTIONS::SOLID);
	tile->flags.reset(TILE_OPTIONS::WALK_BLOCKED);
	tile->flags.reset(TILE_OPTIONS::VIEW_BLOCKED);
	tile->base_tile_type = tile_type::EMPTY_SPACE;
}

void hollow(const location_t &loc)
{
	really_hollow(loc);

	location_t above =
	{ loc.region, loc.x, loc.y, static_cast<uint8_t>(loc.z - 1) };
	location_t below =
	{ loc.region, loc.x, loc.y, static_cast<uint8_t>(loc.z + 1) };

	really_hollow(above);
	really_hollow(below);
	raws::create_structure_from_raws("Ship Superstructure", above);
	raws::create_structure_from_raws("Ship Superstructure", below);
}

void crash_the_ship(const uint8_t start_x, const uint8_t start_y, const uint8_t start_z, const uint8_t planet_idx, planet_t * planet)
{
	for (uint8_t X = start_x / 2; X < start_x + 6; ++X)
	{
		const int half_width = 5 + (X / 64);
		for (uint8_t Y = start_y - half_width; Y < start_y + half_width; ++Y)
		{
			uint8_t Z = 1;
			bool found = false;
			while (!found)
			{
				tile_t * candidate = planet->get_tile(location_t{ planet_idx, X, Y, Z });
				if (candidate->flags.test(TILE_OPTIONS::SOLID))
				{
					++Z;
				}
				else
				{
					found = true;
					candidate->covering = tile_covering::BARE;
					if (candidate->tree > 0) {
						candidate->tree = tree_potential::FORMER_TREE;
						// We need to spawn some wood
						int wood_id = raws::create_item_from_raws("Wood Logs");
						ECS->add_component<position_component3d>(*ECS->get_entity_by_handle(wood_id), position_component3d(location_t{ planet_idx, X, Y, Z }, OMNI));
					}
				}
			}
		}
	}
}

void grow_trees(const uint8_t planet_idx, planet_t * planet) {
	for (uint8_t z=0; z<REGION_DEPTH; ++z) {
		for (int16_t y=0; y<REGION_HEIGHT; ++y) {
			for (int16_t x=0; x<REGION_WIDTH; ++x) {
				tile_t * target = planet->get_tile(location_t{ planet_idx, x, y, z });
				if (target->tree > 1) {
					// Time to grow a tree!
					entity tree = ECS->add_entity();
					ECS->add_component(tree, position_component3d({ planet_idx, x, y, z }, OMNI));
					ECS->add_component(tree, renderable_component(6, color_t{64,255,64}, color_t{0,0,0}, 0));
					ECS->add_component(tree, tree_component());
					ECS->add_component(tree, debug_name_component("Pine Tree"));
					ECS->add_component(tree, description_component("A pine tree"));
					ECS->add_component(tree, obstruction_component(true, true));
				}
			}
		}
	}
}


void add_camera(const uint8_t start_x, const uint8_t start_y,
		const uint8_t start_z, const uint8_t planet_idx)
{
	entity camera = ECS->add_entity();
	ECS->add_component(camera, position_component3d({ planet_idx, start_x, start_y, start_z }, OMNI));
	universe->globals.camera_handle = camera.handle;
}

void add_cordex(const uint8_t start_x, const uint8_t start_y,
		const uint8_t start_z, const uint8_t planet_idx)
{
	entity cordex = ECS->add_entity();
	universe->globals.cordex_handle = cordex.handle;
	ECS->add_component(cordex, debug_name_component("Cordex"));
	ECS->add_component(cordex, position_component3d(location_t{ planet_idx, start_x, start_y, start_z }, OMNI));
	ECS->add_component(cordex, viewshed_component(penetrating, 16));
	ECS->add_component(cordex, calendar_component(0L));
	ECS->add_component(cordex, renderable_component(15, cyan, black, 17));
	ECS->add_component(cordex, obstruction_component(true, false));
	ECS->add_component(cordex, description_component("You! The ship-board AI responsible for keeping these barely-functional hairless ape-descendants alive."));
	ECS->add_component(cordex, point_light_component(8));
	hollow(location_t{ planet_idx, start_x, start_y, start_z });
}

void add_structural_element(const location_t &loc, unsigned char glyph,
		bool block = true)
{
	hollow(loc);
	switch (glyph)
	{
	case 16:
		raws::create_structure_from_raws("Ship Front", loc);
		break;
	case 186:
		raws::create_structure_from_raws("Ship Wall NS", loc);
		break;
	case 200:
		raws::create_structure_from_raws("Ship Wall NE", loc);
		break;
	case 201:
		raws::create_structure_from_raws("Ship Wall SE", loc);
		break;
	case 219:
		raws::create_structure_from_raws("Ship Superstructure", loc);
		break;
	case 46:
		raws::create_structure_from_raws("Ship Floor", loc);
		break;
	case 205:
		raws::create_structure_from_raws("Ship Wall EW", loc);
		break;
	default:
		std::cout << "Oops - missed a structure, code " << +glyph << "\n";
	}
}

void add_solar_collector(const uint8_t x, const uint8_t y, const uint8_t z,
		const uint8_t planet_idx)
{
	add_structural_element(location_t
	{ planet_idx, x, y, static_cast<uint8_t>(z - 1) }, '.');
	raws::create_structure_from_raws("Solar Collector", location_t
	{ planet_idx, x, y, z });
	//hollow(location_t { planet_idx, x, y, z-1 });
}

void add_food_replicator(const uint8_t x, const uint8_t y, const uint8_t z,
		const uint8_t planet_idx)
{
	raws::create_structure_from_raws("Small Replicator", location_t
	{ planet_idx, x, y, z });
	hollow(location_t
	{ planet_idx, x, y, z });
}

void add_storage_unit(const uint8_t x, const uint8_t y, const uint8_t z,
		const uint8_t planet_idx)
{
	int container_id = raws::create_structure_from_raws("Storage Unit",
			location_t
			{ planet_idx, x, y, z });
	hollow(location_t
	{ planet_idx, x, y, z });

	for (int i = 0; i < 3; ++i)
	{
		int tent_kit = raws::create_item_from_raws(
				"Personal Survival Shelter Kit");
		ECS->add_component<item_storage_component>(
				*ECS->get_entity_by_handle(tent_kit),
				item_storage_component(container_id));
	}
	int fire_kit = raws::create_item_from_raws("Camping Fire Kit");
	ECS->add_component<item_storage_component>(*ECS->get_entity_by_handle(fire_kit), item_storage_component(container_id));
	int fire_axe = raws::create_item_from_raws("Fire Axe");
	ECS->add_component<item_storage_component>(*ECS->get_entity_by_handle(fire_axe),item_storage_component(container_id));
	int pick_axe = raws::create_item_from_raws("Pickaxe");
	ECS->add_component<item_storage_component>(*ECS->get_entity_by_handle(pick_axe),item_storage_component(container_id));
}

void make_entities(planet_t * planet)
{
	const uint8_t planet_idx = planet->planet_idx(WORLD_WIDTH / 2,
			WORLD_HEIGHT - 1);

	// Determine starting location
	uint8_t start_x = REGION_WIDTH / 2;
	uint8_t start_y = REGION_HEIGHT / 2;
	uint8_t start_z = 1;

	planet->load_region(planet_idx);
	bool found = false;
	while (!found)
	{
		// -4 to ensure that the back door is usable
		tile_t * candidate = planet->get_tile(location_t
		{ planet_idx, static_cast<uint8_t>(start_x - 4), start_y, start_z });
		if (candidate->flags.test(TILE_OPTIONS::SOLID))
		{
			++start_z;
		}
		else
		{
			found = true;
		}
	}
	std::cout << "Starting at: " << +start_x << "/" << +start_y << "/"
			<< +start_z << "\n";

	// Clear a crash trail
	crash_the_ship(start_x, start_y, start_z, planet_idx, planet);
	grow_trees(planet_idx, planet);

	// TODO: Hollow out the landing zone

	// Add ship hull, superstructure, doors, power, sensors, cordex
	add_cordex(start_x, start_y, start_z, planet_idx);
	// Solar collector at (x-1,y-1), (x+1,y-1), (x-1,y+1), (x+1,y+1)
	add_solar_collector(static_cast<uint8_t>(start_x - 1),
			static_cast<uint8_t>(start_y - 1),
			static_cast<uint8_t>(start_z + 1), planet_idx);
	add_solar_collector(static_cast<uint8_t>(start_x + 1),
			static_cast<uint8_t>(start_y - 1),
			static_cast<uint8_t>(start_z + 1), planet_idx);
	add_solar_collector(static_cast<uint8_t>(start_x - 1),
			static_cast<uint8_t>(start_y + 1),
			static_cast<uint8_t>(start_z + 1), planet_idx);
	add_solar_collector(static_cast<uint8_t>(start_x + 1),
			static_cast<uint8_t>(start_y + 1),
			static_cast<uint8_t>(start_z + 1), planet_idx);

	// Console constructions at (x-1,y), (x+1,y), (x,y-1), (x,y+1)
	raws::create_structure_from_raws("Education Console", location_t
	{ planet_idx, static_cast<uint8_t>(start_x - 1), start_y, start_z });
	raws::create_structure_from_raws("Scanner Console", location_t
	{ planet_idx, static_cast<uint8_t>(start_x + 1), start_y, start_z });
	raws::create_structure_from_raws("Defense Console", location_t
	{ planet_idx, start_x, static_cast<uint8_t>(start_y - 1), start_z });
	raws::create_structure_from_raws("Communications Console", location_t
	{ planet_idx, start_x, static_cast<uint8_t>(start_y + 1), start_z });
	raws::create_structure_from_raws("Water Purifier",
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 3),
					static_cast<uint8_t>(start_y - 2), start_z });
	raws::create_structure_from_raws("Food Dispenser",
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 3),
					static_cast<uint8_t>(start_y + 2), start_z });
	hollow(location_t
	{ planet_idx, static_cast<uint8_t>(start_x - 1), start_y, start_z });
	hollow(location_t
	{ planet_idx, static_cast<uint8_t>(start_x + 1), start_y, start_z });
	hollow(location_t
	{ planet_idx, start_x, static_cast<uint8_t>(start_y - 1), start_z });
	hollow(location_t
	{ planet_idx, start_x, static_cast<uint8_t>(start_y + 1), start_z });
	hollow(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 3),
					static_cast<uint8_t>(start_y - 2), start_z });
	hollow(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 3),
					static_cast<uint8_t>(start_y + 2), start_z });

	// Refridgerator/Food Replicator at (x+4,y)
	add_food_replicator(start_x + 4, start_y, start_z, planet_idx);

	// Storage unit at (x+4,y-1) and (x+4,y+1)
	add_storage_unit(start_x + 4, start_y - 1, start_z, planet_idx);
	add_storage_unit(start_x + 4, start_y + 1, start_z, planet_idx);

	// Escape pod structure
	add_structural_element(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x - 4),
					static_cast<uint8_t>(start_y - 3), start_z }, 201); // TL Corner
	add_structural_element(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x - 4),
					static_cast<uint8_t>(start_y + 3), start_z }, 200); // BL Corner
	for (int i = 0; i < 6; ++i)
	{
		if (i != 3)
		{
			add_structural_element(location_t
			{ planet_idx, static_cast<uint8_t>(start_x - 3 + i),
					static_cast<uint8_t>(start_y - 3), start_z }, 205); // Upper hull wall
			add_structural_element(location_t
			{ planet_idx, static_cast<uint8_t>(start_x - 3 + i),
					static_cast<uint8_t>(start_y + 3), start_z }, 205); // Lower hull wall
		}
	}
	for (int i = 0; i < 5; ++i)
	{
		if (i != 2)
		{
			add_structural_element(location_t
			{ planet_idx, static_cast<uint8_t>(start_x - 4),
					static_cast<uint8_t>(start_y - 2 + i), start_z }, 186); // Add left wall
		}
		else
		{
			add_structural_element(location_t
			{ planet_idx, static_cast<uint8_t>(start_x - 4),
					static_cast<uint8_t>(start_y - 2 + i), start_z }, '.');
		}
	}

	add_structural_element(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 3),
					static_cast<uint8_t>(start_y - 3), start_z }, 16); // Front
	add_structural_element(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 4),
					static_cast<uint8_t>(start_y - 2), start_z }, 16); // Front
	add_structural_element(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 5),
					static_cast<uint8_t>(start_y - 1), start_z }, 16); // Front
	add_structural_element(location_t
	{ planet_idx, static_cast<uint8_t>(start_x + 6),
			static_cast<uint8_t>(start_y), start_z }, 16); // Front
	add_structural_element(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 5),
					static_cast<uint8_t>(start_y + 1), start_z }, 16); // Front
	add_structural_element(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 4),
					static_cast<uint8_t>(start_y + 2), start_z }, 16); // Front
	add_structural_element(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 3),
					static_cast<uint8_t>(start_y + 3), start_z }, 16); // Front

	for (int i = 0; i < 5; ++i)
	{
		add_structural_element(
				location_t
				{ planet_idx, static_cast<uint8_t>(start_x - 3),
						static_cast<uint8_t>(start_y - 2 + i), start_z }, '.',
				false);
		add_structural_element(
				location_t
				{ planet_idx, static_cast<uint8_t>(start_x - 2),
						static_cast<uint8_t>(start_y - 2 + i), start_z }, '.',
				false);
		add_structural_element(
				location_t
				{ planet_idx, static_cast<uint8_t>(start_x + 2),
						static_cast<uint8_t>(start_y - 2 + i), start_z }, '.',
				false);
		if (i > 0 and i < 4)
			add_structural_element(location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 3),
					static_cast<uint8_t>(start_y - 2 + i), start_z }, '.',
					false);
	}
	add_structural_element(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x - 1),
					static_cast<uint8_t>(start_y - 2), start_z }, '.', false);
	add_structural_element(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x),
					static_cast<uint8_t>(start_y - 2), start_z }, '.', false);
	add_structural_element(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 1),
					static_cast<uint8_t>(start_y - 2), start_z }, '.', false);
	add_structural_element(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x - 1),
					static_cast<uint8_t>(start_y + 2), start_z }, '.', false);
	add_structural_element(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x),
					static_cast<uint8_t>(start_y + 2), start_z }, '.', false);
	add_structural_element(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 1),
					static_cast<uint8_t>(start_y + 2), start_z }, '.', false);
	add_structural_element(location_t
	{ planet_idx, static_cast<uint8_t>(start_x + 5),
			static_cast<uint8_t>(start_y), start_z }, 219);

	// Add random settlers    
	make_settler(location_t
	{ planet_idx, start_x, static_cast<uint8_t>(start_y - 2), start_z });
	make_settler(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 1),
					static_cast<uint8_t>(start_y - 2), start_z });
	make_settler(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 2),
					static_cast<uint8_t>(start_y - 2), start_z });
	make_settler(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x),
					static_cast<uint8_t>(start_y + 2), start_z });
	make_settler(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 1),
					static_cast<uint8_t>(start_y + 2), start_z });
	make_settler(
			location_t
			{ planet_idx, static_cast<uint8_t>(start_x + 2),
					static_cast<uint8_t>(start_y + 2), start_z });

	add_camera(start_x, start_y, start_z, planet_idx);
	universe->globals.stored_power = 25;

	planet->save_region(planet_idx);
	ECS->save_game("world/savegame3d.dat");
	ECS->done();
}

void world_gen_phase_1()
{
	std::cout << "World gen running\n";
	engine::random_number_generator rng;

	std::unique_ptr<heightmap_t> base_map = get_height_map();
	std::cout << "Zero Height Map\n";
	zero_height_map(base_map.get());

	std::cout << "Making a noise-based height-map\n";
	make_noisy_heightmap(base_map.get(), &rng);
	smooth_height_map(base_map.get());

	std::cout << "Hydrology\n";
	std::unique_ptr<water_level_map_t> water = perform_hydrology(base_map.get(),
			&rng);

	std::cout << "Biomes\n";
	biome_map_t biomes = make_biome_map(base_map.get(), &rng, water.get());

	std::cout << "Layercake\n";
	std::unique_ptr<planet_t> planet = make_world_layers(base_map.get(), rng,
			water.get(), &biomes);

	std::cout << "Making starting entites\n";
	ECS->init();
	make_entities(planet.get());

	std::cout << "World gen done\n";
}
