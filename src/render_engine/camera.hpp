#pragma once

#include "../stdafx.h"
#include "frustrum.hpp"

namespace render {
	extern bool camera_moved;
	extern bool sun_moved;
	extern glm::mat4 camera_projection_matrix;
	extern glm::mat4 camera_modelview_matrix;
	extern glm::mat4 camera_proj_model_view_matrix;
	extern Frustrum frustrum;
	extern boost::container::flat_set<int, std::greater<int>> visible_chunks;

	void update_camera();
}
