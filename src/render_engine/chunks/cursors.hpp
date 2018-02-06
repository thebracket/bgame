#pragma once

#include <glm/glm.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace render {
	void build_cursor_geometry();
	void render_cursor(glm::mat4 &camera_projection_matrix, glm::mat4 &camera_modelview_matrix);
}