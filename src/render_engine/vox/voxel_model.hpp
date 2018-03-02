#pragma once

#include "../../bengine/gl_include.hpp"
#include <vector>

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
		unsigned int ssbo_id = 0;
		bool created_ssbo = false;
		unsigned int n_elements = 0;
		unsigned int start_index = 0;

		void build_model();
		void render_instances(const voxel_render_buffer_t &buffer) const;
		void build_buffer(std::vector<instance_t> &instances, voxel_render_buffer_t * render);
	};

	struct voxel_render_buffer_t {
		int n_instances = 0;
		voxel_model *model = nullptr;
	};

	void build_master_geometry();
	void bulk_render(const std::vector<std::unique_ptr<vox::voxel_render_buffer_t>> &model_buffers);
	extern unsigned int voxel_geometry_vao;
}