#pragma once

#include "../../stdafx.h"

namespace render {
	void build_cursor_geometry();
	void render_cursor(glm::mat4 &camera_projection_matrix, glm::mat4 &camera_modelview_matrix);
}