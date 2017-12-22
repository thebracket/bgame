#pragma once

#include "../../bengine/gl_include.hpp"

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
		unsigned int vbo_id = 0;
		unsigned int instance_vbo_id = 0;
		int n_elements = 0;

		void build_model();
		void render_instances(voxel_render_buffer_t &buffer);
		void build_buffer(std::vector<instance_t> &instances, voxel_render_buffer_t * render);

	private:
		std::vector<float> geometry;

		void add_cube_geometry(std::vector<float> &v, const subvoxel &voxel,
							   const float &width, const float &height, const float &depth, const float &texture_id);

		void build_vbo(std::vector<float> &v);
	};

	struct voxel_render_group_t {
		unsigned int vao = 0;
	};

	struct voxel_render_buffer_t {
		unsigned int tmp_vao = 0;
		std::size_t n_instances;
		voxel_model *model;

		voxel_render_buffer_t() {
			glGenVertexArrays(1, &tmp_vao);
			//std::cout << "Created VAO #" << tmp_vao << "\n";
		}

		~voxel_render_buffer_t() {
			if (tmp_vao > 0) {
				//std::cout << "Deleting VAO #" << tmp_vao << "\n";
				glDeleteVertexArrays(1, &tmp_vao);
			}
		}
	};
}