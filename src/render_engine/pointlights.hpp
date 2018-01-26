#pragma once

namespace render {
	void update_pointlights();
	void render_pointlights();
	void render_ascii_pointlights(unsigned int &ascii_fbo, unsigned int &ascii_vao, unsigned int &texture_id, std::size_t buffer_size, glm::mat4 &camera_projection_matrix, glm::mat4 &camera_modelview_matrix);
}