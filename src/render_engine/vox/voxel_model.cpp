#include "voxel_model.hpp"
#include <map>
#include <iostream>
#include "../../global_assets/shader_storage.hpp"
#include "../shaders/voxel_shader.hpp"

namespace vox {

	constexpr static int voxidx(const int &w, const int &h, const int &d, const int &x, const int &y, const int &z) noexcept
	{
		return (w * h * z) + (w * y) + x;
	}

	static unsigned int us(const int &n) noexcept
	{
		return static_cast<unsigned int>(n);
	}

	static void add_cube_geometry(std::vector<float> &v, std::vector<float> &n, std::vector<float> &c, const subvoxel &voxel, const float &W, const float &H, const float &D) noexcept
	{
		const auto x0 = -0.5f + voxel.x;
		const auto x1 = x0 + W;
		const auto y0 = -0.5f + voxel.z;
		const auto y1 = y0 + D;
		const auto z0 = -0.5f + voxel.y;
		const auto z1 = z0 + H;

		const unsigned int base = v.size() / 3;
		/*
	-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
	1.0f, 1.0f, -1.0f,		0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
	1.0f, -1.0f, -1.0f,		0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
	1.0f, 1.0f, -1.0f,		0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
	-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
	-1.0f, 1.0f, -1.0f,		0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // top-left
														// front face
	-1.0f, -1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
	1.0f, -1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom-right
	1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
	1.0f, 1.0f, 1.0f,		 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
	-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top-left
	-1.0f, -1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
														// left face
	-1.0f, 1.0f, 1.0f,		-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
	-1.0f, 1.0f, -1.0f,		-1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
	-1.0f, -1.0f, -1.0f,	-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
	-1.0f, -1.0f, -1.0f,	-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
	-1.0f, -1.0f, 1.0f,		-1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
	-1.0f, 1.0f, 1.0f,		-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
														// right face
	1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
	1.0f, -1.0f, -1.0f,		1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
	1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
	1.0f, -1.0f, -1.0f,		1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
	1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
	1.0f, -1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
														// bottom face
	-1.0f, -1.0f, -1.0f,	0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
	1.0f, -1.0f, -1.0f,		0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
	1.0f, -1.0f, 1.0f,		0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
	1.0f, -1.0f, 1.0f,		0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
	-1.0f, -1.0f, 1.0f,		0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
	-1.0f, -1.0f, -1.0f,	0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
														// top face
	-1.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
	1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
	1.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
	1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
	-1.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
	-1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 0.0f, 0.0f  // bottom-left 
	*/


		v.insert(v.end(), {
			
			x0, y0, z0,
			x1, y1, z0,
			x1, y0, z0,
			x1, y1, z0,
			x0, y0, z0,
			x0, y1, z0,

			x0, y0, z1,
			x1, y0, z1,
			x1, y1, z1,
			x1, y1, z1,
			x0, y1, z1,
			x0, y0, z1,

			x0, y1, z1,
			x0, y1, z0,
			x0, y0, z0,
			x0, y0, z0,
			x0, y0, z1,
			x0, y1, z1,

			x1, y1, z1,
			x1, y0, z0,
			x1, y1, z0,
			x1, y1, z0,
			x1, y1, z1,
			x1, y0, z1,

			x0, y0, z0,
			x1, y0, z0,
			x1, y0, z1,
			x1, y0, z1,
			x0, y0, z1,
			x0, y0, z0,

			x0, y1, z0,
			x1, y1, z1,
			x1, y1, z0,
			x1, y1, z1,
			x0, y1, z0,
			x0, y1, z1,

			});
		n.insert(n.end(), {
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,

			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,

			-1.0f, 0.0f, 0.0f,
			- 1.0f, 0.0f, 0.0f,
			- 1.0f, 0.0f, 0.0f,
			- 1.0f, 0.0f, 0.0f,
			- 1.0f, 0.0f, 0.0f,
			- 1.0f, 0.0f, 0.0f,

			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,

			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			});
		c.insert(c.end(), {
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,

			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,

			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,

			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,

			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,

			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
			voxel.r, voxel.g, voxel.b,
		});

	}

	static bool is_same(const subvoxel &a, const subvoxel &b) noexcept
	{
		return a.r == b.r && a.g == b.g && a.b == b.b;
	}

	void voxel_model::build_model() {
		// Build a cube map
		std::map<int, subvoxel> cubes;
		for (const auto cube : voxels) {
			const auto idx = voxidx(width, depth, height, cube.x, cube.z, cube.y);
			cubes[idx] = cube;
		}

		// Perform greedy voxels on it
		auto cube_count = 0;
		while (!cubes.empty()) {
			const auto first_cube = cubes.begin();
			const auto base_idx = first_cube->first;
			const auto voxel_info = first_cube->second;

			auto W = 1;
			auto H = 1;
			auto D = 1;
			cubes.erase(base_idx);

			// Grow on the X
			auto idx_grow_right = base_idx + 1;
			auto x_coordinate = voxel_info.x;
			auto right_finder = cubes.find(idx_grow_right);
			while (x_coordinate < width && right_finder != cubes.end() && is_same(voxel_info, right_finder->second)) {
				cubes.erase(idx_grow_right);
				++W;
				++idx_grow_right;
				++x_coordinate;
				right_finder = cubes.find(idx_grow_right);
			}
						
			// Grow on the Z (not Y!)
			if (voxel_info.z < depth) {
				auto z_progress = voxel_info.z + 1;

				auto possible = true;
				while (possible && z_progress < depth) {
					for (auto gx = voxel_info.x; gx < voxel_info.x + width; ++gx) {
						const auto candidate_idx = voxidx(width, height, depth, gx, voxel_info.y, z_progress);
						const auto vfinder = cubes.find(candidate_idx);
						if (!(vfinder != cubes.end()) || !is_same(voxel_info, vfinder->second)) possible = false;
						if (!possible) break;
					}
					if (possible) {
						++D;
						for (auto gx = voxel_info.x; gx < voxel_info.x + width; ++gx) {
							const auto candidate_idx = voxidx(width, height, depth, gx, voxel_info.y, z_progress);
							cubes.erase(candidate_idx);
						}
					}

					++z_progress;
				}
			}	

			add_cube_geometry(vertices_, normals_, colors_, voxel_info, static_cast<float>(W), static_cast<float>(H), static_cast<float>(D));
			++cube_count;
		}
		n_elements = vertices_.size() / 3;

		// Build VAO/VBO and associate geometry with it
		build_vbo();

		voxels.clear();
		voxels.shrink_to_fit();
	}	

	void voxel_model::build_vbo() {
		glGenBuffers(1, &vbo_v_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_v_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_.size(), &vertices_[0], GL_STATIC_DRAW);

		glGenBuffers(1, &vbo_n_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_n_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals_.size(), &normals_[0], GL_STATIC_DRAW);

		glGenBuffers(1, &vbo_c_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_c_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors_.size(), &colors_[0], GL_STATIC_DRAW);

		// Clear all the buffers!
		vertices_.clear();
		normals_.clear();
		colors_.clear();
	}

	void voxel_model::build_buffer(std::vector<instance_t> &instances, voxel_render_buffer_t * render)
	{
		constexpr auto instance_t_size_bytes = 10 * sizeof(float);
		assert(render->tmp_vao > 0);

		// Upload the instance buffer into an SSBO
		if (!created_ssbo)
		{
			created_ssbo = true;
			glGenBuffers(1, &ssbo_id);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_id);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(instance_t) * instances.size(), &instances[0], GL_DYNAMIC_COPY);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		} else
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_id);
			GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
			memcpy(p, &instances[0], sizeof(instance_t) * instances.size());
			glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		}

		// Create the VAO to hold this data
		glBindVertexArray(render->tmp_vao);

		// Bind the consistent elements
		glBindBuffer(GL_ARRAY_BUFFER, vbo_v_id);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0); // 0 = Vertex Position
		glVertexAttribDivisor(0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_n_id);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1); // 1 = Normals
		glVertexAttribDivisor(1, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_c_id);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2); // 2 = Color
		glVertexAttribDivisor(2, 0);		

		//glCheckError();
		glBindVertexArray(0);
		glCheckError();

		render->n_instances = static_cast<int>(instances.size());
		render->model = this;
	}

	void voxel_model::render_instances(const voxel_render_buffer_t &buffer) const {
		glEnable(GL_CULL_FACE);
		glBindVertexArray(buffer.tmp_vao);
		glCheckError();
		glShaderStorageBlockBinding(assets::voxel_shader->shader_id, assets::voxel_shader->instance_block_index, 2);
		glCheckError();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo_id);
		glCheckError();

		glDrawArraysInstanced(GL_TRIANGLES, 0, n_elements, buffer.n_instances);
		glCheckError();

		glBindVertexArray(0);
		glDisable(GL_CULL_FACE);
	}
}
