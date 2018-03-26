#include "stdafx.h"
#include "settler_spawner_system.hpp"
#include "../../planet/region/region.hpp"
#include "../../global_assets/game_planet.hpp"
#include "../../global_assets/rng.hpp"
#include "../../planet/builder/settler_builder.hpp"
#include "../gui/log_system.hpp"
#include "../gui/particle_system.hpp"
#include "../../global_assets/game_pause.hpp"

namespace systems {
	namespace settler_spawner {
		void run(const double &duration_ms) {
			// New arrivals
			if (day_elapsed && planet.remaining_settlers > 0) {
				++planet.migrant_counter;
				if (planet.migrant_counter > 14 && !planet.strict_beamdown) { // Every 2 weeks
					const int crash_x = REGION_WIDTH / 2;
					const int crash_y = REGION_HEIGHT / 2;
					const int crash_z = region::ground_z(crash_x, crash_y);

					const std::vector<position_t> settler_arrival_points{
						{ crash_x - 2, crash_y - 2, crash_z + 1 },
						{ crash_x - 1, crash_y - 2, crash_z + 1 },
						{ crash_x, crash_y - 2, crash_z + 1 },
						{ crash_x + 1, crash_y - 2, crash_z + 1 },
						{ crash_x - 3, crash_y, crash_z + 1 },
						{ crash_x - 2, crash_y, crash_z + 1 },
						{ crash_x - 1, crash_y, crash_z + 1 },
						{ crash_x, crash_y, crash_z + 1 },
						{ crash_x + 1, crash_y, crash_z + 1 }
					};


					planet.migrant_counter = 0;
					const int new_settler_count = std::min(planet.remaining_settlers, rng.roll_dice(1, 6));
					planet.remaining_settlers -= new_settler_count;
					fmt::MemoryWriter ss;
					ss << "Ark auto-repair has managed to make the teleporter work! " << new_settler_count << " settlers have beamed down.";
					logging::log_message lmsg{ LOG{}.text(ss.str())->chars };
					logging::log(lmsg);

					for (auto i = 0; i < new_settler_count; ++i) {
						const position_t spawn_point = settler_arrival_points[i % settler_arrival_points.size()];
						create_settler(planet, spawn_point.x, spawn_point.y, spawn_point.z, rng, rng.roll_dice(1, 3) - 1);
						particles::block_destruction_effect(spawn_point.x, spawn_point.y, spawn_point.z, 1.0f, 1.0f, 1.0f, particles::PARTICLE_SMOKE);
					}
				}
			}
		}
	}
}