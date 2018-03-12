#pragma once

#include "../fbo/gbuffer.hpp"
#include <boost/container/flat_set.hpp>
#include <glm/detail/type_mat.hpp>

namespace vox
{
	struct voxel_render_buffer_t;
}

namespace render {
	void build_voxel_render_list();
	void render_voxel_models(gbuffer_t * gbuffer, glm::mat4 &camera_projection_matrix, glm::mat4 &camera_modelview_matrix);

	extern bool models_changed;
	extern std::vector<std::unique_ptr<vox::voxel_render_buffer_t>> model_buffers;

	void invalidate_composite_cache_for_entity(const int &id);
}