#pragma once

#include "frustrum.hpp"
#include <boost/container/flat_set.hpp>
#include <functional>

namespace render {
	extern bool camera_moved;
	extern bool sun_moved;
	extern glm::mat4 camera_projection_matrix;
	extern glm::mat4 camera_modelview_matrix;
	extern glm::mat4 camera_proj_model_view_matrix;
	extern Frustrum frustrum;

	struct visible_chunk_t
	{
		float distance_from_camera;
		int chunk_id;
	};

	extern std::vector<visible_chunk_t> visible_chunks;

	void update_camera();
}
