#pragma once

#include "../fbo/gbuffer.hpp"
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>

namespace render {
	void build_voxel_render_list();
	void render_voxel_models(gbuffer_t * gbuffer, glm::mat4 &camera_projection_matrix, glm::mat4 &camera_modelview_matrix);

	extern bool models_changed;

}