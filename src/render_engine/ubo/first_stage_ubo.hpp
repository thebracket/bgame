#pragma once
#include <glm/mat4x4.hpp>

namespace render
{
	namespace camera_ubo
	{
		extern unsigned int ubo;
		void set_ubo_camera(const glm::mat4 &camera_projection_matrix, const glm::mat4 &camera_modelview_matrix, const glm::mat4 &camera_combined_matrix);
	}
}
