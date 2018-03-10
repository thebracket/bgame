#include "water_render.hpp"
#include "geometry_helper.hpp"
#include "../../planet/region/region.hpp"
#include "../../global_assets/shader_storage.hpp"
#include "../../global_assets/texture_storage.hpp"
#include "../renderbuffers.hpp"
#include "../../bengine/FastNoise.h"
#include "../../global_assets/game_camera.hpp"
#include <array>
#include "chunks.hpp"
#include "../camera.hpp"
#include "../../bengine/gl_include.hpp"
#include "../shaders/chunk_shader.hpp"
#include "../ubo/first_stage_ubo.hpp"
#include "../../systems/physics/fluid_system.hpp"

namespace render {
	static unsigned int water_vao = 0;
	static unsigned int water_vbo = 0;
	static int n_elements_water_elements = 0;
	bool reset_wet_vao = true;
	static uint8_t wc = 0;

	void build_water_geometry() {
		++wc;
		//if (water_vao > 0 && wc % 5 != 0) return;
		systems::fluids::water_dirty = false;

		n_elements_water_elements = 0;

		// Generate GL resources if needed
		if (water_vao == 0) glGenVertexArrays(1, &water_vao);
		if (water_vbo == 0) glGenBuffers(1, &water_vbo);

		// Build the cursor geometry
		std::vector<float> data;

		for (int z = camera_position->region_z - 10; z < camera_position->region_z+1; ++z) {
			for (int y = 0; y < REGION_HEIGHT; ++y) {
				for (int x = 0; x < REGION_WIDTH; ++x) {
					const int chunkidx = chunks::chunk_id_by_world_pos(x, y, z);
					if (visible_chunk_set.find(chunkidx) == visible_chunk_set.end()) break;
					const int idx = mapidx(x, y, z);
					const auto waterlvl = region::water_level(idx);
					if (waterlvl > 0) {
						const auto[x, y, z] = idxmap(idx);
						const auto X = static_cast<float>(x);
						const auto Y = static_cast<float>(y);
						const auto Z = static_cast<float>(z);

						//n_elements_water_elements += chunks::add_water_geometry(data, X, Y, Z, 1.0f, 1.0f, 12, waterlvl);
						n_elements_water_elements += chunks::add_cube_geometry(data, X, Y, Z, 1.0f, 1.0f, 12, (static_cast<float>(waterlvl)/10.0f) - 0.02);
					}
				}
			}
		}

		glBindVertexArray(water_vao);
		glBindBuffer(GL_ARRAY_BUFFER, water_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_DYNAMIC_DRAW);

		if (reset_wet_vao) {
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0); // 0 = Vertex Position

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char *) nullptr + 4 * sizeof(float));
			glEnableVertexAttribArray(1); // 1 = TexX/Y/ID

			glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char *) nullptr + 7 * sizeof(float));
			glEnableVertexAttribArray(2); // 2 = Normals

			reset_wet_vao = false;
		}

		glBindVertexArray(0);
		glCheckError();
	}

	void render_water(glm::mat4 &camera_projection_matrix, glm::mat4 &camera_modelview_matrix) {
		assets::chunkshader->use();
		glBindFramebuffer(GL_FRAMEBUFFER, gbuffer->fbo_id);

		// Assign the uniforms
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_ubo::ubo);
		glUniformBlockBinding(assets::chunkshader->shader_id, 0, assets::chunkshader->camera_block_index);
		glCheckError();

		// Assign the texture array
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, assets::chunk_texture_array);

		glUniform1i(assets::chunkshader->textureArray, 0);

		glBindVertexArray(water_vao);
		glDrawArrays(GL_TRIANGLES, 0, n_elements_water_elements);
	}
}
