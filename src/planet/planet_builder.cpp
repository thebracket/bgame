#include "planet_builder.hpp"
#include <atomic>
#include <iostream>

std::atomic<bool> planet_build_done;

bool is_planet_build_complete() {
	return planet_build_done.load();
}

void build_planet() {
	planet_build_done.store(false);

	// Make a zero-height map
	// Noise-based world map
	// Make a biome map
	// Save it to disk

	planet_build_done.store(true);
}