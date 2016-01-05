#include "global_illumination_system.hpp"
#include "../world/universe.hpp"

void global_illumination_system::tick(const double &duration_ms) {
	if (universe->globals.paused) return;
}
