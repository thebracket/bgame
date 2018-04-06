#include "stdafx.h"
#include "fluid_system.hpp"
#include "../../planet/region/region.hpp"
#include "../../global_assets/rng.hpp"
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
		
		static std::vector<GLuint> build_water_as_particles_with_evaporation()
		{
			//const auto evaporate = rng.roll_dice(1, 100) == 1;

			std::vector<GLuint> water;
			for (auto i=0; i < REGION_TILES_COUNT; ++i)
			{
				auto wl = water_level(i);
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

			wp = build_water_as_particles_with_evaporation();

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

		void run(const double &duration_ms) {
			glUseProgram(assets::fluid_shader);

			glShaderStorageBlockBinding(assets::fluid_shader, terrain_flags_buffer_idx, 4);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, chunks::flags_ssbo);

			glShaderStorageBlockBinding(assets::fluid_shader, water_level_idx, 5);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, water_level_ssbo);

			glShaderStorageBlockBinding(assets::fluid_shader, water_particles_idx, 6);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, water_particles_ssbo);

			glDispatchCompute(wp.size(), 1, 1);

			// Swimming/drowning

			each<position_t>([](entity_t &e, position_t &pos) {
				if (pos.x > 0 && pos.x < REGION_WIDTH-1 && pos.y > 0 && pos.y < REGION_HEIGHT-1 && pos.z > 0 && pos.z < REGION_DEPTH-1) {
					if (water_level(mapidx(pos)) > 7) {
						auto is_drowning = true;

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
				}
			});
		}
	}
}