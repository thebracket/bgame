#pragma once

#include "../../stdafx.h"

namespace render {
	void build_water_geometry();
	void render_water(glm::mat4 &camera_projection_matrix, glm::mat4 &camera_modelview_matrix);
}
