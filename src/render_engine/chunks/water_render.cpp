#include "water_render.hpp"
#include "geometry_helper.hpp"
#include "../../planet/region/region.hpp"
#include "../../global_assets/shader_storage.hpp"
#include "../../global_assets/texture_storage.hpp"
#include "../renderbuffers.hpp"
#include "../../bengine/FastNoise.h"
#include "../../global_assets/game_camera.hpp"
#include <array>

namespace render {
	static unsigned int water_vao = 0;
	static unsigned int water_vbo = 0;
	static int n_elements_water_elements = 0;
	bool reset_wet_vao = true;
	bool made_water_noise = false;
	static uint8_t wc = 0;
	static std::array<float, REGION_WIDTH * REGION_HEIGHT> noise_level;

	void build_water_geometry() {
		++wc;
		if (water_vao > 0 && wc % 5 != 0) return;

		n_elements_water_elements = 0;

		if (!made_water_noise) {
			constexpr unsigned int octaves = 5;
			constexpr float persistence = 0.5F;
			constexpr float frequency = 4.0F;

			FastNoise noise;
			noise.SetNoiseType(FastNoise::GradientFractal);
			noise.SetFractalType(FastNoise::FBM);
			noise.SetFractalOctaves(octaves);
			noise.SetFractalGain(persistence);
			noise.SetFractalLacunarity(frequency);

			int idx = 0;
			for (int y = 0; y < REGION_HEIGHT; ++y) {
				for (int x = 0; x < REGION_WIDTH; ++x) {
					noise_level[idx] = noise.GetGradientFractal(x, y);
					++idx;
				}
			}

			made_water_noise = true;
		}

		// Generate GL resources if needed
		if (water_vao == 0) glGenVertexArrays(1, &water_vao);
		if (water_vbo == 0) glGenBuffers(1, &water_vbo);

		// Build the cursor geometry
		std::vector<float> data;

		for (int z = camera_position->region_z - 10; z < camera_position->region_z; ++z) {
			for (int y = 0; y < REGION_HEIGHT; ++y) {
				for (int x = 0; x < REGION_WIDTH; ++x) {
					const int idx = mapidx(x, y, z);
					const uint8_t waterlvl = region::water_level(idx);
					if (waterlvl > 0) {
						const float water_offset = static_cast<float>(waterlvl) / 10.0f;
						const auto[x, y, z] = idxmap(idx);
						const float X = static_cast<float>(x);
						const float Y = static_cast<float>(y);
						const float Z = static_cast<float>(z);
						const std::size_t perturb_idx = (y * REGION_WIDTH) + x + wc % (REGION_WIDTH * REGION_HEIGHT);
						const float perturb = noise_level[perturb_idx];

						n_elements_water_elements += chunks::add_cube_geometry(data, X, Y, Z, 1.0f, 1.0f, 12, water_offset + perturb);
					}
				}
			}
		}

		glBindVertexArray(water_vao);
		glBindBuffer(GL_ARRAY_BUFFER, water_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_DYNAMIC_DRAW);

		if (reset_wet_vao) {
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0); // 0 = Vertex Position

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (char *) nullptr + 3 * sizeof(float));
			glEnableVertexAttribArray(1); // 1 = TexX/Y/ID

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (char *) nullptr + 6 * sizeof(float));
			glEnableVertexAttribArray(2); // 2 = Normals

			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (char *) nullptr + 9 * sizeof(float));
			glEnableVertexAttribArray(3); // 3 = Tangent

			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (char *) nullptr + 12 * sizeof(float));
			glEnableVertexAttribArray(4); // 4 = Binormal

			reset_wet_vao = false;
		}

		glBindVertexArray(0);
		glCheckError();
	}

	void render_water(glm::mat4 &camera_projection_matrix, glm::mat4 &camera_modelview_matrix) {
		assets::chunkshader->use();
		glBindFramebuffer(GL_FRAMEBUFFER, gbuffer->fbo_id);

		// Assign the uniforms
		glUniformMatrix4fv(assets::chunkshader->projection_matrix, 1, GL_FALSE, glm::value_ptr(camera_projection_matrix));
		glUniformMatrix4fv(assets::chunkshader->view_matrix, 1, GL_FALSE, glm::value_ptr(camera_modelview_matrix));

		// Assign the texture array
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, assets::chunk_texture_array);

		glUniform1i(assets::chunkshader->textureArray, 0);

		glBindVertexArray(water_vao);
		glDrawArrays(GL_TRIANGLES, 0, n_elements_water_elements);
	}
}
