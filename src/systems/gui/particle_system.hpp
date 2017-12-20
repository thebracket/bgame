#pragma once

#include "../../stdafx.h"
#include <stdint.h>
#include "../../components/position.hpp"
#include "../../bengine/color_t.hpp"

namespace systems {
	namespace particles {
		constexpr uint8_t PARTICLE_SMOKE = 1;
		constexpr uint8_t PARTICLE_LUMBERJACK = 2;

		void run(const double &duration_ms);
		void emit_particle(float x, float y, float z, float r, float g, float b, float size, uint8_t mode);
		void block_destruction_effect(int x, int y, int z, float r, float g, float b, uint8_t mode);
		void ranged_attack(position_t &start, position_t &end, bengine::color_t &color);
		void melee_attack(position_t &start, position_t &end, bengine::color_t &color);
		void build_buffers();
		void render_particles(glm::mat4 &camera_projection_matrix, glm::mat4 &camera_modelview_matrix);
	}
}