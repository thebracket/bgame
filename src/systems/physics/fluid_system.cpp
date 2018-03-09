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

using namespace bengine;
using namespace region;
using namespace tile_flags;

namespace systems {
	namespace fluids {

		bool made_water_level_ssbo = false;
		unsigned int water_level_ssbo;
		unsigned int flags_idx;
		unsigned int water_idx;
		unsigned int offset_idx;

		void copy_to_shader()
		{
			/*
			if (!made_water_level_ssbo)
			{
				glGenBuffers(1, &water_level_ssbo);
				made_water_level_ssbo = true;

				glBindBuffer(GL_SHADER_STORAGE_BUFFER, water_level_ssbo);
				glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * region::get_water_level()->size(), &region::get_water_level()->operator[](0), GL_DYNAMIC_COPY);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

				flags_idx = glGetProgramResourceIndex(assets::fluid_shader, GL_SHADER_STORAGE_BLOCK, "terrain_flags");
				water_idx = glGetProgramResourceIndex(assets::fluid_shader, GL_SHADER_STORAGE_BLOCK, "water_level");
				offset_idx = glGetUniformLocation(assets::fluid_shader, "offset");
			}
			else
			{
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, water_level_ssbo);
				GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
				memcpy(p, &region::get_water_level()->operator[](0), sizeof(uint32_t) * region::get_water_level()->size());
				glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
			}*/

		}

		void copy_back_to_world()
		{
			/*
			glMemoryBarrier(GL_ALL_BARRIER_BITS);
			
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, water_level_ssbo);
			GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
			memcpy(&region::get_water_level()->operator[](0), p, sizeof(uint32_t) * REGION_TILES_COUNT);
			glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
			glCheckError();
			*/
		}

		static unsigned offset_counter = 0;

		static void run_compute_shader()
		{
			/*
			glUseProgram(assets::fluid_shader);

			glShaderStorageBlockBinding(assets::fluid_shader, flags_idx, 4);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, chunks::flags_ssbo);

			glShaderStorageBlockBinding(assets::fluid_shader, water_idx, 5);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, water_level_ssbo);
			
			glUniform1ui(offset_idx, offset_counter);

			constexpr unsigned offset_max = REGION_DEPTH-1;
			constexpr auto x_runs = REGION_WIDTH;
			constexpr auto y_runs = REGION_HEIGHT;
			constexpr auto z_runs = 1;
			glMemoryBarrier(GL_ALL_BARRIER_BITS);
			glDispatchCompute(x_runs, y_runs, z_runs);

			bool has_water = false;
			auto count = 0;
			while (!has_water && count < REGION_DEPTH) {
				++offset_counter;
				offset_counter = offset_counter % offset_max;
				for (auto y = 0; y < REGION_HEIGHT; ++y) {
					for (auto x = 0; x < REGION_WIDTH; ++x) {
						const auto idx = mapidx(x, y, offset_counter);
						if (region::water_level(idx) > 0) has_water = true;
					}
				}
				++count;
			}
			*/
		}

		void run(const double &duration_ms) {

			run_compute_shader();

			// Evaporation
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