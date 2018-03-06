#pragma once

#include <glm/glm.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>

namespace render {
	void update_pointlights();
	void render_pointlights();
	void render_ascii_pointlights(unsigned int &ascii_fbo, unsigned int &ascii_vao, unsigned int &texture_id, std::size_t buffer_size, glm::mat4 &camera_projection_matrix, glm::mat4 &camera_modelview_matrix);

	struct point_light_buffer_t;

	struct pointlight_t {
		bool new_light = true;
		uint8_t cycle_tick = 0;
		glm::vec3 light_pos;
		glm::vec3 light_col;
		float radius = 0.0f;
		std::vector<glm::mat4> shadow_transforms;
		std::unique_ptr<point_light_buffer_t> buffer;
		unsigned int ubo_index = 0;

		void make_mats();
		void make_buffer();
		void draw_depth_buffer();
		void render_light() const;
	};
}