#pragma once

#include <stdint.h>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace systems {
	namespace particles {
		constexpr uint8_t PARTICLE_SMOKE = 1;

		void run(const double &duration_ms);
		void emit_particle(float x, float y, float z, float r, float g, float b, float size, uint8_t mode);
		void build_buffers();
		void render_particles(glm::mat4 &camera_projection_matrix, glm::mat4 &camera_modelview_matrix);
	}
}