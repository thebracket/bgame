#include "fluid_system.hpp"
#include "../../planet/region/region.hpp"
#include "../../global_assets/game_planet.hpp"
#include "../../global_assets/rng.hpp"
#include "../../components/position.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/health.hpp"
#include "../../components/water_spawner.hpp"
#include "../../render_engine/chunks/chunks.hpp"
#include "../../bengine/ecs.hpp"
#include "../../global_assets/game_ecs.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../global_assets/shader_storage.hpp"
#include "../gui/particle_system.hpp"

using namespace bengine;
using namespace region;
using namespace tile_flags;

namespace systems {
	namespace fluids {

		bool water_dirty = true;
		
		struct water_particle_t
		{
			int idx;
			uint8_t level;
		};
		constexpr int MAX_WATER_SIZE = REGION_TILES_COUNT * sizeof(water_particle_t);

		static std::vector<water_particle_t> build_water_as_particles()
		{
			std::vector<water_particle_t> water;
			for (auto i=0; i < REGION_TILES_COUNT; ++i)
			{
				const auto wl = water_level(i);
				if (wl > 0)
				{
					water.emplace_back(water_particle_t{ 
						i,
						wl
					});
				}
			}
			water_dirty = false;
			return water;
		}

		static void water_swap(const int &idx, const int &dest_idx)
		{
			const auto my_level = region::water_level(idx);
			const auto their_level = region::water_level(dest_idx);
			const auto amount_to_move = 1;
			region::set_water_level(idx, my_level - amount_to_move);
			region::set_water_level(dest_idx, their_level + amount_to_move);
		}

		void run(const double &duration_ms) {
			// Water creation and destruction

			// Build particle list
			const auto wp = build_water_as_particles();
			const auto evaporation_roll = (rng.roll_dice(1, 20) == 1);

			// Water falling, evaporation and so on
			for (const auto &w : wp)
			{
				const auto[x, y, z] = idxmap(w.idx);
				const auto idx_below = w.idx - 65536;
				const auto idx_west = w.idx - 1;
				const auto idx_east = w.idx + 1;
				const auto idx_north = w.idx - 256;
				const auto idx_south = w.idx + 256;

				if (region::flag(w.idx, SOLID))
				{
					// Water shouldn't be here - remove it
					region::set_water_level(w.idx, 0);

				}
				else {
					// Can it fall?
					if (z > 1 && !region::flag(idx_below, SOLID) && !region::flag(w.idx, CAN_STAND_HERE) && region::water_level(idx_below) < 10)
					{
						// It can fall
						water_swap(w.idx, idx_below);
						//particles::emit_particle(x, y, z, 0.25f, 0.25f, 1.0f, 1.0f, particles::PARTICLE_SMOKE);
					}
					else if (x > 1 && !region::flag(idx_west, SOLID) && region::water_level(idx_west) < 10)
					{
						// It can go west
						water_swap(w.idx, idx_west);
						//particles::emit_particle(x, y, z, 0.25f, 0.25f, 1.0f, 1.0f, particles::PARTICLE_SMOKE);
					}
					else if (x < REGION_WIDTH-1 && !region::flag(idx_east, SOLID) && region::water_level(idx_east) < 10)
					{
						// It can go west
						water_swap(w.idx, idx_east);
						//particles::emit_particle(x, y, z, 0.25f, 0.25f, 1.0f, 1.0f, particles::PARTICLE_SMOKE);
					}
					else if (y > 1 && !region::flag(idx_north, SOLID) && region::water_level(idx_north) < 10)
					{
						// It can go west
						water_swap(w.idx, idx_north);
						//particles::emit_particle(x, y, z, 0.25f, 0.25f, 1.0f, 1.0f, particles::PARTICLE_SMOKE);
					}
					else if (y < REGION_HEIGHT-1 && !region::flag(idx_south, SOLID) && region::water_level(idx_south) < 10)
					{
						// It can go west
						water_swap(w.idx, idx_south);
						//particles::emit_particle(x, y, z, 0.25f, 0.25f, 1.0f, 1.0f, particles::PARTICLE_SMOKE);
					}
					else if (evaporation_roll && w.level == 1)
					{
						// Remove the water
						set_water_level(w.idx, 0);
						//particles::emit_particle(x, y, z, 0.25f, 0.25f, 1.0f, 1.0f, particles::PARTICLE_SMOKE);
					}
				}
			}

			// Swimming/drowning

			each<position_t>([](entity_t &e, position_t &pos) {
				if (water_level(mapidx(pos)) > 7) {
					bool is_drowning = true;

					auto stats = e.component<game_stats_t>();
					if (stats) {
						if (skill_roll(e.id, *stats, rng, "Swimming", DIFFICULTY_AVERAGE) > FAIL) is_drowning = false;
					}

					if (is_drowning) {
						auto health = e.component<health_t>();
						if (health) {
							//TODO: Implement inflict damage
							//emit_deferred(inflict_damage_message{ e.id, rng.roll_dice(1,4), "Drowning" });
						}
					}
				}
			});
		}
	}
}