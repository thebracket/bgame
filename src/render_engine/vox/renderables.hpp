#pragma once

#include "../../stdafx.h"
#include "../fbo/gbuffer.hpp"

namespace render {
	void build_voxel_render_list(const boost::container::flat_set<int, std::greater<int>> &visible_chunks);
	void render_voxel_models(gbuffer_t * gbuffer, glm::mat4 &camera_projection_matrix, glm::mat4 &camera_modelview_matrix);
	void render_voxel_models_shadow(float &radius, glm::vec3 &light_pos, std::vector<glm::mat4> &shadowTransforms, unsigned int textureId);

	extern bool models_changed;

}