#pragma once

#include "solar_system.hpp"
#include "shared_data.hpp"
#include "../game.h"
#include "../components/position_component3d.hpp"
#include "planet.hpp"
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <memory>

/* Maximum height of the universe. */
constexpr uint8_t UNIVERSE_HEIGHT = 48;

/* Maximum width of the universe. */
constexpr uint8_t UNIVERSE_WIDTH = 64;

/*
 * Holds the universe, at least in an abstract sense. There are certain constants maintained at the
 * "universe" level, which are really globals. This is left deliberately vague, since we aren't anywhere
 * close to actually needing much of the universe yet.
 */
struct universe_t
{
	// Defining the universe
	std::unordered_map<uint16_t, solar_system_t> solar_systems;

	// Global state
	shared_data_t globals;

	// Serialization
	void save() const;
	void load();
};

/*
 * Get a universe index, a location point for a space at universe-scale.
 */
inline uint16_t universe_idx(const int &x, const int &y)
{
	return (y * UNIVERSE_WIDTH) + x;
}

/*
 * The universe gets to be a global variable.
 */
extern std::unique_ptr<universe_t> universe;

/*
 * Serialize universal globals
 */
void load_universe_state(std::fstream &lbfile);

/*
 * De-serialize universal globals
 */
void save_universe_state(std::fstream &lbfile);

/*
 * Retrieves the current position of the camera
 */
inline position_component3d * get_camera_position()
{
	const int camera_component_id = universe->globals.camera_handle;
	return ECS->find_entity_component<position_component3d>(camera_component_id);
}

inline region_t * get_current_region()
{
	position_component3d * camera_pos = get_camera_position();
	region_t * current_region = world::planet->get_region(camera_pos->pos.region);
	return current_region;
}
