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
		unsigned int vbo_v_id = 0;
		unsigned int ssbo_id = 0;
		bool created_ssbo = false;
		int n_elements = 0;

		void build_model();
		void render_instances(const voxel_render_buffer_t &buffer) const;
		void build_buffer(std::vector<instance_t> &instances, voxel_render_buffer_t * render);

	private:
		std::vector<float> vertices_;

		void build_vbo();
	};

	struct voxel_render_buffer_t {
		unsigned int tmp_vao = 0;
		int n_instances = 0;
		voxel_model *model = nullptr;

		voxel_render_buffer_t() {
			glGenVertexArrays(1, &tmp_vao);
			//std::cout << "Created VAO #" << tmp_vao << "\n";
		}
	};
}