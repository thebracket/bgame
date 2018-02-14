#include "voxel_model.hpp"
#include <unordered_map>
#include <assert.h>
#include <map>

namespace vox {

	constexpr int voxidx(const int &w, const int &h, const int &d, const int &x, const int &y, const int &z) {
		return (w * h * z) + (w * y) + x;
	}

	bool is_same(const subvoxel &a, const subvoxel &b) {
		return a.r == b.r && a.g == b.g && a.b == b.b;
	}

	void voxel_model::build_model() {
		// Build a cube map
		std::map<int, subvoxel> cubes;
		for (const auto cube : voxels) {
			const int idx = voxidx(width, depth, height, cube.x, cube.z, cube.y);
			cubes[idx] = cube;
		}

		// Perform greedy voxels on it
		int cube_count = 0;
		while (!cubes.empty()) {
			const auto first_cube = cubes.begin();
			const auto base_idx = first_cube->first;
			const auto voxel_info = first_cube->second;

			int W = 1;
			int H = 1;
			int D = 1;
			cubes.erase(base_idx);

			int idx_grow_right = base_idx + 1;
			int x_coordinate = voxel_info.x;
			auto right_finder = cubes.find(idx_grow_right);
			while (x_coordinate < width && right_finder != cubes.end() && is_same(voxel_info, right_finder->second)) {
				cubes.erase(idx_grow_right);
				++W;
				++idx_grow_right;
				++x_coordinate;
				right_finder = cubes.find(idx_grow_right);
			}

			if (voxel_info.y < height) {
				int y_progress = voxel_info.y + 1;

				bool possible = true;
				while (possible && y_progress < height) {
					for (int gx = voxel_info.x; gx < voxel_info.x + width; ++gx) {
						const int candidate_idx = voxidx(width, height, depth, gx, y_progress, voxel_info.z);
						auto vfinder = cubes.find(candidate_idx);
						if (!(vfinder != cubes.end()) || !is_same(voxel_info, vfinder->second)) possible = false;
						if (!possible) break;
					}
					if (possible) {
						++H;
						for (int gx = voxel_info.x; gx < voxel_info.x + width; ++gx) {
							const int candidate_idx = voxidx(width, height, depth, gx, y_progress, voxel_info.z);
							cubes.erase(candidate_idx);
						}
					}

					++y_progress;
				}
			}

			if (voxel_info.z < depth) {
				int z_progress = voxel_info.z + 1;

				bool possible = true;
				while (possible && z_progress < depth) {
					for (int gy = voxel_info.y; gy < voxel_info.y + height; ++gy) {
						for (int gx = voxel_info.x; gx < voxel_info.x + width; ++gx) {
							const int candidate_idx = voxidx(width, height, depth, gx, gy, z_progress);
							auto vfinder = cubes.find(candidate_idx);
							if (!(vfinder != cubes.end()) || !is_same(voxel_info, vfinder->second)) possible = false;
							if (!possible) break;
						}
					}
					if (possible) {
						++D;
						for (int gy = voxel_info.y; gy < voxel_info.y + height; ++gy) {
							for (int gx = voxel_info.x; gx < voxel_info.x + width; ++gx) {
								const int candidate_idx = voxidx(width, height, depth, gx, gy, z_progress);
								cubes.erase(candidate_idx);
							}
						}
					}

					++z_progress;
				}
			}

			add_cube_geometry(geometry, voxel_info, static_cast<float>(W), static_cast<float>(H), static_cast<float>(D), 3);
			++cube_count;
		}

		// Build VAO/VBO and associate geometry with it
		build_vbo(geometry);

		// voxels.clear();
	}

	void voxel_model::add_cube_geometry(std::vector<float> &v, const subvoxel &voxel,
										const float &W, const float &H, const float &D, const float &texture_id) {
		const float x0 = -0.5f + voxel.x;
		const float x1 = x0 + W;
		const float y0 = -0.5f + voxel.z;
		const float y1 = y0 + D;
		const float z0 = -0.5f + voxel.y;
		const float z1 = z0 + H;

		//std::cout << "Cube: " << x0 << "-" << y0 << "-" << z0 << " " << W << "x" << H << "\n";

		v.insert(v.end(), {
				x0, y0, z0, 0.0f, 0.0f, -1.0f, voxel.r, voxel.g, voxel.b,
				x1, y0, z0, 0.0f, 0.0f, -1.0f, voxel.r, voxel.g, voxel.b,
				x1, y1, z0, 0.0f, 0.0f, -1.0f, voxel.r, voxel.g, voxel.b,
				x1, y1, z0, 0.0f, 0.0f, -1.0f, voxel.r, voxel.g, voxel.b,
				x0, y1, z0, 0.0f, 0.0f, -1.0f, voxel.r, voxel.g, voxel.b,
				x0, y1, z0, 0.0f, 0.0f, -1.0f, voxel.r, voxel.g, voxel.b,

				x0, y0, z1, 0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,
				x1, y0, z1, 0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,
				x1, y1, z1, 0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,
				x1, y1, z1, 0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,
				x0, y1, z1, 0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,
				x0, y0, z1, 0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,

				x0, y1, z1, -1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x0, y1, z0, -1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x0, y0, z0, -1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x0, y0, z0, -1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x0, y0, z1, -1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x0, y1, z1, -1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,

				x1, y1, z1, 1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x1, y1, z0, 1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x1, y0, z0, 1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x1, y0, z0, 1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x1, y0, z1, 1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x1, y1, z1, 1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,

				x0, y0, z0, 0.0f, -1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x1, y0, z0, 0.0f, -1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x1, y0, z1, 0.0f, -1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x1, y0, z1, 0.0f, -1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x0, y0, z1, 0.0f, -1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x0, y0, z0, 0.0f, -1.0f, 0.0f, voxel.r, voxel.g, voxel.b,

				x0, y1, z0, 0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x1, y1, z0, 0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x1, y1, z1, 0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x1, y1, z1, 0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x0, y1, z1, 0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
				x0, y1, z0, 0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b
		});
	}

	void voxel_model::build_vbo(std::vector<float> &v) {
		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v.size(), &v[0], GL_STATIC_DRAW);
		n_elements = (int)v.size() / 6;
	}

	void voxel_model::build_buffer(std::vector<instance_t> &instances, voxel_render_buffer_t * render)
	{
		constexpr size_t instance_t_size_bytes = 10 * sizeof(float);
		assert(render->tmp_vao > 0);

		// Build the instance buffer
		if (instance_vbo_id < 1) glGenBuffers(1, &instance_vbo_id);
		glInvalidateBufferData(instance_vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_id);
		glBufferData(GL_ARRAY_BUFFER, instance_t_size_bytes * instances.size(), &instances[0], GL_DYNAMIC_DRAW);
		//glCheckError();

		// Create the VAO to hold this data
		glBindVertexArray(render->tmp_vao);
		//glCheckError();

		// Bind the consistent elements
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0); // 0 = Vertex Position

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (char *) nullptr + 3 * sizeof(float));
		glEnableVertexAttribArray(1); // 1 = Normals
		//glCheckError();

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (char *) nullptr + 6 * sizeof(float));
		glEnableVertexAttribArray(2); // 2 = Color
		//glCheckError();

		// Bind the per-element items
		glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_id);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(3); // 0 = Instance Position
		glVertexAttribDivisor(3, 1);

		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (char *) nullptr + 3 * sizeof(float));
		glEnableVertexAttribArray(4); // 1 = Instance Rotataion (vec4)
		glVertexAttribDivisor(4, 1);

		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (char *) nullptr + 7 * sizeof(float));
		glEnableVertexAttribArray(5); // 2 = Tint
		glVertexAttribDivisor(5, 1);

		//glCheckError();
		glBindVertexArray(0);
		glCheckError();

		render->n_instances = instances.size();
		render->model = this;
	}

	void voxel_model::render_instances(voxel_render_buffer_t &buffer) {
		glBindVertexArray(buffer.tmp_vao);
		glDrawArraysInstanced(GL_TRIANGLES, 0, n_elements, (int)buffer.n_instances);
		glCheckError();

		glBindVertexArray(0);
	}
}