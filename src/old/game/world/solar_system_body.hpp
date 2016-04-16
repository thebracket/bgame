#pragma once

#include <cstdint>
#include <memory>
#include <fstream>

enum system_body_t
{
	ICE_WORLD,
	VOLCANIC_WORLD,
	GAS_CLOUD,
	ASTEROIDS,
	WARP_HUSK,
	HABITABLE_PLANET,
	SPACE_STATION
};

/*
 * Represents a planet or other body orbiting a star system.
 */
struct solar_system_body_t
{
	/* Index within the solar system. */
	uint8_t system_idx;

	/* Type of system element. */
	system_body_t body_type;

	// Serialization
	void save(std::fstream &lbfile) const;
	void load(std::fstream &lbfile);
};
