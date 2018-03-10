#include "fluid_system.hpp"
#include "../../planet/region/region.hpp"
#include "../../global_assets/rng.hpp"
#include "../../components/position.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/health.hpp"
#include "../../components/water_spawner.hpp"
#include "../../bengine/ecs.hpp"
#include "../../global_assets/game_ecs.hpp"
#include "../../bengine/gl_include.hpp"
#include "../gui/particle_system.hpp"
#include "../damage/damage_system.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../global_assets/shader_storage.hpp"
#include "../../render_engine/chunks/chunks.hpp"

using namespace bengine;
using namespace region;
using namespace tile_flags;

namespace systems {
	namespace fluids {

		bool water_dirty = true;
		static bool made_ssbos = false;
		unsigned int water_level_ssbo;
		unsigned int water_particles_ssbo;

		unsigned int terrain_flags_buffer_idx;
		unsigned int water_level_idx;
		unsigned int water_particles_idx;
		
		static std::vector<GLuint> build_water_as_particles()
		{
			std::vector<GLuint> water;
			for (auto i=0; i < REGION_TILES_COUNT; ++i)
			{
				const auto wl = water_level(i);
				if (wl > 0)
				{
					water.emplace_back(static_cast<GLuint>(i));
				}
			}
			return water;
		}

		static std::vector<GLuint> wp;

		void copy_to_gpu()
		{
			// Water creation and destruction (We're doing this now so that we're copying the correct data to the GPU)
			bengine::each<water_spawner_t, position_t>([](entity_t &e, water_spawner_t &w, position_t &pos)
			{
				switch (w.spawner_type)
				{
				case 0: // 0 deletes all water
				{
					const auto idx = mapidx(pos);
					if (region::water_level(idx) > 0) region::set_water_level(idx, 0);
				} break;
				case 1: // 1 adds water
				{
					const auto idx = mapidx(pos);
					if (region::water_level(idx) < 10) region::set_water_level(idx, 10);
				} break;
				}
			});

			wp = build_water_as_particles();

			if (!made_ssbos)
			{
				made_ssbos = true;
				glGenBuffers(1, &water_level_ssbo);
				glGenBuffers(1, &water_particles_ssbo);

				glBindBuffer(GL_SHADER_STORAGE_BUFFER, water_level_ssbo);
				glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * region::get_water_level()->size(), &region::get_water_level()->operator[](0), GL_DYNAMIC_COPY);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

				glBindBuffer(GL_SHADER_STORAGE_BUFFER, water_particles_ssbo);
				glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * wp.size(), &wp[0], GL_DYNAMIC_COPY);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

				terrain_flags_buffer_idx = glGetProgramResourceIndex(assets::fluid_shader, GL_SHADER_STORAGE_BLOCK, "terrain_flags");
				water_level_idx = glGetProgramResourceIndex(assets::fluid_shader, GL_SHADER_STORAGE_BLOCK, "water_level");
				water_particles_idx = glGetProgramResourceIndex(assets::fluid_shader, GL_SHADER_STORAGE_BLOCK, "water_particles");
			} else
			{
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, water_level_ssbo);
				GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
				memcpy(p, &region::get_water_level()->operator[](0), sizeof(uint32_t) * region::get_water_level()->size());
				glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

				// Note to self: It's variable size, so we can't memcpy
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, water_particles_ssbo);
				glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * wp.size(), &wp[0], GL_DYNAMIC_COPY);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			}
		}

		void copy_from_gpu()
		{
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, water_level_ssbo);
			GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
			memcpy(&region::get_water_level()->operator[](0), p, sizeof(uint32_t) * region::get_water_level()->size());
			glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
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
			glUseProgram(assets::fluid_shader);

			glShaderStorageBlockBinding(assets::fluid_shader, terrain_flags_buffer_idx, 4);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, chunks::flags_ssbo);

			glShaderStorageBlockBinding(assets::fluid_shader, water_level_idx, 5);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, water_level_ssbo);

			glShaderStorageBlockBinding(assets::fluid_shader, water_particles_idx, 6);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, water_particles_ssbo);

			glDispatchCompute(wp.size(), 1, 1);

			/*
			constexpr auto add_particles = true;			

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
						if constexpr (add_particles) particles::emit_particle(x, y, z, 0.25f, 0.25f, 1.0f, 1.0f, particles::PARTICLE_SMOKE);
					}
					else if (x > 1 && !region::flag(idx_west, SOLID) && region::water_level(idx_west) < 10)
					{
						// It can go west
						water_swap(w.idx, idx_west);
						if constexpr (add_particles)particles::emit_particle(x, y, z, 0.25f, 0.25f, 1.0f, 1.0f, particles::PARTICLE_SMOKE);
					}
					else if (x < REGION_WIDTH-1 && !region::flag(idx_east, SOLID) && region::water_level(idx_east) < 10)
					{
						// It can go west
						water_swap(w.idx, idx_east);
						if constexpr (add_particles)particles::emit_particle(x, y, z, 0.25f, 0.25f, 1.0f, 1.0f, particles::PARTICLE_SMOKE);
					}
					else if (y > 1 && !region::flag(idx_north, SOLID) && region::water_level(idx_north) < 10)
					{
						// It can go west
						water_swap(w.idx, idx_north);
						if constexpr (add_particles)particles::emit_particle(x, y, z, 0.25f, 0.25f, 1.0f, 1.0f, particles::PARTICLE_SMOKE);
					}
					else if (y < REGION_HEIGHT-1 && !region::flag(idx_south, SOLID) && region::water_level(idx_south) < 10)
					{
						// It can go west
						water_swap(w.idx, idx_south);
						if constexpr (add_particles)particles::emit_particle(x, y, z, 0.25f, 0.25f, 1.0f, 1.0f, particles::PARTICLE_SMOKE);
					}
					else if (evaporation_roll && w.level == 1)
					{
						// Remove the water
						set_water_level(w.idx, 0);
						if constexpr (add_particles)particles::emit_particle(x, y, z, 0.25f, 0.25f, 1.0f, 1.0f, particles::PARTICLE_SMOKE);
					}
				}
			}
			*/

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
							damage_system::inflict_damage_message msg{ e.id, rng.roll_dice(1,4), "Drowning" };
							damage_system::inflict_damage(msg);
						}
					}
				}
			});
		}
	}
}