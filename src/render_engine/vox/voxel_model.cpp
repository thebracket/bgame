#include "voxel_model.hpp"
#include <map>
#include "../../global_assets/shader_storage.hpp"
#include "../shaders/voxel_shader.hpp"
#include "../shaders/voxel_shadow_shader.hpp"
#include "../ubo/first_stage_ubo.hpp"
#include "../renderbuffers.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "../../bengine/main_window.hpp"
#include "renderables.hpp"
#include "../ubo/lighting_ubo.hpp"

namespace vox {

	std::vector<float> master_vertices;
	unsigned int voxel_geometry_vbo;
	unsigned int voxel_geometry_vao;
	static bool made_voxel_vao = false;
	unsigned int instance_ssbo;
	static bool made_instance_ssbo = false;
	std::vector<instance_t> instance_buffer;

	constexpr static int voxidx(const int &w, const int &h, const int &d, const int &x, const int &y, const int &z) noexcept
	{
		return (w * h * z) + (w * y) + x;
	}

	static void add_cube_geometry(std::vector<float> &v, const subvoxel &voxel, const float &W, const float &H, const float &D) noexcept
	{
		const auto x0 = -0.5f + voxel.x;
		const auto x1 = x0 + W;
		const auto y0 = -0.5f + voxel.z;
		const auto y1 = y0 + D;
		const auto z0 = -0.5f + voxel.y;
		const auto z1 = z0 + H;

		v.insert(v.end(), {
			
			x0, y0, z0, 0.0f, 0.0f, -1.0f,voxel.r, voxel.g, voxel.b,
			x1, y1, z0,	0.0f, 0.0f, -1.0f,voxel.r, voxel.g, voxel.b,
			x1, y0, z0,	0.0f, 0.0f, -1.0f,voxel.r, voxel.g, voxel.b,
			x1, y1, z0,	0.0f, 0.0f, -1.0f,voxel.r, voxel.g, voxel.b,
			x0, y0, z0,	0.0f, 0.0f, -1.0f,voxel.r, voxel.g, voxel.b,
			x0, y1, z0,	0.0f, 0.0f, -1.0f,voxel.r, voxel.g, voxel.b,

			x0, y0, z1,	0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,
			x1, y0, z1,	0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,
			x1, y1, z1,	0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,
			x1, y1, z1,	0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,
			x0, y1, z1,	0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,
			x0, y0, z1,	0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,

			x0, y1, z1,	-1.0f, 0.0f, 0.0f,voxel.r, voxel.g, voxel.b,
			x0, y1, z0,	-1.0f, 0.0f, 0.0f,voxel.r, voxel.g, voxel.b,
			x0, y0, z0,	-1.0f, 0.0f, 0.0f,voxel.r, voxel.g, voxel.b,
			x0, y0, z0,	-1.0f, 0.0f, 0.0f,voxel.r, voxel.g, voxel.b,
			x0, y0, z1,	-1.0f, 0.0f, 0.0f,voxel.r, voxel.g, voxel.b,
			x0, y1, z1,	-1.0f, 0.0f, 0.0f,voxel.r, voxel.g, voxel.b,

			x1, y1, z1, 	1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
			x1, y0, z0, 	1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
			x1, y1, z0, 	1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
			x1, y0, z0, 	1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
			x1, y1, z1, 	1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
			x1, y0, z1, 	1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,

			x0, y0, z0,	0.0f, -1.0f, 0.0f,voxel.r, voxel.g, voxel.b,
			x1, y0, z0,	0.0f, -1.0f, 0.0f,voxel.r, voxel.g, voxel.b,
			x1, y0, z1,	0.0f, -1.0f, 0.0f,voxel.r, voxel.g, voxel.b,
			x1, y0, z1,	0.0f, -1.0f, 0.0f,voxel.r, voxel.g, voxel.b,
			x0, y0, z1,	0.0f, -1.0f, 0.0f,voxel.r, voxel.g, voxel.b,
			x0, y0, z0,	0.0f, -1.0f, 0.0f,voxel.r, voxel.g, voxel.b,

			x0, y1, z0,	0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
			x1, y1, z1,	0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
			x1, y1, z0,	0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
			x1, y1, z1,	0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
			x0, y1, z0,	0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
			x0, y1, z1,	0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b,

			});
	}

	static bool is_same(const subvoxel &a, const subvoxel &b) noexcept
	{
		return a.r == b.r && a.g == b.g && a.b == b.b;
	}

	void voxel_model::build_model() {
		start_index = master_vertices.size();

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

			add_cube_geometry(master_vertices, voxel_info, static_cast<float>(W), static_cast<float>(H), static_cast<float>(D));
			++cube_count;
		}
		n_elements = (master_vertices.size() - start_index) / 9;

		voxels.clear();
		voxels.shrink_to_fit();
	}	

	void build_master_geometry()
	{
		glGenBuffers(1, &voxel_geometry_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, voxel_geometry_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * master_vertices.size(), &master_vertices[0], GL_STATIC_DRAW);

		// Clear all the buffers!
		master_vertices.clear();
	}

	void voxel_model::build_buffer(std::vector<instance_t> &instances, voxel_render_buffer_t * render)
	{
		render->instance_offset = static_cast<unsigned int>(instance_buffer.size());

		// Append to the master instance list
		instance_buffer.insert(instance_buffer.end(), instances.begin(), instances.end());

		if (!made_voxel_vao) {
			// Create the VAO to hold this data
			glGenVertexArrays(1, &voxel_geometry_vao);
			glBindVertexArray(voxel_geometry_vao);

			// Bind the consistent elements
			glBindBuffer(GL_ARRAY_BUFFER, voxel_geometry_vbo);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0); // 0 = Vertex Position
			glVertexAttribDivisor(0, 0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (char *) nullptr + 3 * sizeof(float));
			glEnableVertexAttribArray(1); // 1 = Normals
			glVertexAttribDivisor(1, 0);

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (char *) nullptr + 6 * sizeof(float));
			glEnableVertexAttribArray(2); // 2 = Color
			glVertexAttribDivisor(2, 0);

			//glCheckError();
			glBindVertexArray(0);
			made_voxel_vao = true;
		}

		render->n_instances = static_cast<int>(instances.size());
		render->model = this;
	}

	 struct DrawArraysIndirectCommand {
		unsigned int count;
		unsigned int instanceCount;
		unsigned int first;
		unsigned int baseInstance;
	};

	void bulk_render(const std::vector<std::unique_ptr<vox::voxel_render_buffer_t>> &model_buffers)
	{
		assets::voxel_shader->use();
		glBindFramebuffer(GL_FRAMEBUFFER, render::gbuffer->fbo_id);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, render::camera_ubo::ubo);
		glUniformBlockBinding(assets::voxel_shader->shader_id, 0, assets::voxel_shader->camera_block_index);
		glEnable(GL_CULL_FACE);

		glBindVertexArray(vox::voxel_geometry_vao);
		glShaderStorageBlockBinding(assets::voxel_shader->shader_id, assets::voxel_shader->instance_block_index, 2);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, instance_ssbo);

		for (const auto &m : model_buffers) {
			glDrawArraysInstancedBaseInstance(GL_TRIANGLES, m->model->start_index / 9, m->model->n_elements, m->n_instances, m->instance_offset);
		}

		glBindVertexArray(0);
		glDisable(GL_CULL_FACE);
		glCheckError();
	}

	void bulk_shadow_render(const float radius, const std::vector<glm::mat4> &shadowTransforms, const glm::vec3 &light_pos, const unsigned int fbo_id, const unsigned int ubo_index)
	{
		assets::voxel_shadow_shader->use();
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, render::lighting_ubo::ubo);
		glUniformBlockBinding(assets::voxel_shadow_shader->shader_id, 0, assets::voxel_shadow_shader->light_block_index);
		glCheckError();
		glUniform1ui(assets::voxel_shadow_shader->light_index, ubo_index);
		glCheckError();

		// Shadow rendering goes here
		glBindVertexArray(vox::voxel_geometry_vao);
		glShaderStorageBlockBinding(assets::voxel_shadow_shader->shader_id, assets::voxel_shadow_shader->instance_block_index, 2);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, instance_ssbo);
		//glClear(GL_DEPTH_BUFFER_BIT);

		for (const auto &m : render::model_buffers) {
			glDrawArraysInstancedBaseInstance(GL_TRIANGLES, m->model->start_index / 9, m->model->n_elements, m->n_instances, m->instance_offset);
		}

		// Cleanup
		glBindVertexArray(0);
		glCheckError();
	}

	void start_buffer_accumulation()
	{
		instance_buffer.clear();
		instance_buffer.reserve(1000);
	}

	void finish_instance_buffers()
	{
		if (!made_instance_ssbo)
		{
			made_instance_ssbo = true;
			glGenBuffers(1, &instance_ssbo);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, instance_ssbo);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(instance_t) * instance_buffer.size(), &instance_buffer[0], GL_DYNAMIC_COPY);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			glCheckError();
		}
		else
		{			
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, instance_ssbo);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(instance_t) * instance_buffer.size(), &instance_buffer[0], GL_DYNAMIC_COPY);
		}
		glCheckError();
	}
}
