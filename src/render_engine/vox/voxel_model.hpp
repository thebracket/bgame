#pragma once

#include "../../bengine/gl_include.hpp"
#include <vector>
#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>

namespace vox {

	struct subvoxel {
		int x, y, z;
		float r, g, b;
	};

	struct instance_t {
		float x,y,z;
		float axis1, axis2, axis3, rot_angle;
		float tint_r, tint_g, tint_b;
	};

	struct voxel_render_buffer_t;

	struct voxel_model {
		int width, height, depth;
		std::vector<subvoxel> voxels;
		unsigned int n_elements = 0;
		unsigned int start_index = 0;

		void build_model();
		void build_buffer(std::vector<instance_t> &instances, voxel_render_buffer_t * render); // Returns the instance offset
	};

	struct voxel_render_buffer_t {
		unsigned int n_instances = 0;
		voxel_model *model = nullptr;
		unsigned int instance_offset = 0;
	};

	void build_master_geometry();
	void start_buffer_accumulation();
	void finish_instance_buffers();
	void bulk_render(const std::vector<std::unique_ptr<vox::voxel_render_buffer_t>> &model_buffers);
	void bulk_shadow_render(const float radius, const std::vector<glm::mat4> &shadowTransforms, const glm::vec3 &light_pos, const unsigned int fbo_id, const unsigned int ubo_index);
	extern unsigned int voxel_geometry_vao;
}
