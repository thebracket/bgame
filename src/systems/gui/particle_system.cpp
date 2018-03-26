#include "stdafx.h"
#include "particle_system.hpp"
#include "../../global_assets/game_pause.hpp"
#include "../../global_assets/rng.hpp"
#include "../../global_assets/shader_storage.hpp"
#include "../../bengine/geometry.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../render_engine/shaders/particle_shader.hpp"
#include "../../render_engine/ubo/first_stage_ubo.hpp"

namespace systems {
	namespace particles {

		using namespace bengine;

		unsigned int vao = 0;
		unsigned int vbo = 0;

		std::vector<particle_t> positions;

		void emit_particle(float x, float y, float z, float r, float g, float b, float size, uint8_t mode) {
			positions.emplace_back(particle_t{ x, z, y, r, g, b, size, mode, 0.0f });
		}

		void block_destruction_effect(int x, int y, int z, float r, float g, float b, uint8_t mode) {
			for (float i = 0.0f; i < 1.0f; i += 0.1f) {
				for (float j = 0.0f; j < 1.0f; j += 0.1f) {
					for (float k = 0.0f; k < 1.0f; k += 0.1f) {
						const float X = static_cast<float>(x) +i;
						const float Y = static_cast<float>(y) + j;
						const float Z = static_cast<float>(z) + k;

						positions.emplace_back(particle_t{ X, Z, Y, r, g, b, 1.0f, mode, 500.0f });
					}
				}
			}
		}

		static void particle_line(const float x1, const float y1, const float z1, const float x2, const float y2, const float z2, const bengine::color_t &color) {
			const float length = bengine::distance3d(x1, y1, z1, x2, y2, z2);
			const float num_steps = length * 100.0f;
			const float x_diff = x2 - x1;
			const float y_diff = y2 - y1;
			const float z_diff = z2 - z1;
			const float x_step = x_diff / num_steps;
			const float y_step = y_diff / num_steps;
			const float z_step = z_diff / num_steps;
			const int n_steps = static_cast<int>(num_steps);

			float x = x1;
			float y = y1;
			float z = z1;

			for (int i = 0; i < n_steps; ++i) {

				positions.emplace_back(particle_t{ x, y, z, color.r, color.g, color.b, 1.0f, PARTICLE_LUMBERJACK, 900.0f });

				x += x_step;
				y += y_step;
				z += z_step;
			}
		}

		void ranged_attack(position_t &start, position_t &end, bengine::color_t &color) {
			particle_line(start.x, start.z, start.y, end.x, end.z, end.y, color);
		}

		void melee_attack(position_t &start, position_t &end, bengine::color_t &color) {
			particle_line(start.x, start.z, start.y, end.x, end.z, end.y, color);
		}

		void run(const double &duration_ms) {
			// Automated particle spawners
			if (major_tick) {
				// Smoke from fires
				each<smoke_emitter_t, position_t>([] (entity_t &e, smoke_emitter_t &smoke, position_t &pos) {
					const int n_smoke = rng.roll_dice(3, 10);
					for (int i = 0; i < n_smoke; ++i) {
						float x = static_cast<float>(pos.x) + (static_cast<float>(rng.roll_dice(1, 3)) / 10.0f);
						float y = static_cast<float>(pos.z) + (static_cast<float>(rng.roll_dice(1, 3)) / 10.0f);
						float z = static_cast<float>(pos.y) + (static_cast<float>(rng.roll_dice(1, 3)) / 10.0f);
						float grey = static_cast<float>(rng.roll_dice(1, 255)) / 255.0f;
						positions.emplace_back(particle_t{ x, y, z, grey, grey, grey, 1.0f, PARTICLE_SMOKE, -2000.0f });
					}
				});

				// TODO: Smoke from damaged buildings
			}

			// Expand particles
			for (auto &p : positions) {
				p.age += duration_ms;

				if (p.pmode == PARTICLE_SMOKE || p.pmode == PARTICLE_LUMBERJACK) {
					p.pmode == PARTICLE_SMOKE ? p.y += 0.05f : p.y += 0.01f;
					p.size += 0.1f;
					p.r = std::max(0.0f, p.r - 0.01f);
					p.g = std::max(0.0f, p.g - 0.01f);
					p.b = std::max(0.0f, p.b - 0.01f);
				}
			}

			// Delete all particles with age > 1000
			positions.erase(std::remove_if(positions.begin(), positions.end(), [](auto &p) { return p.age > 1000.0f; }), positions.end());
		}

		void build_buffers() {
			if (positions.empty()) return;

			if (vao == 0) glGenVertexArrays(1, &vao);
			if (vbo == 0) glGenBuffers(1, &vbo);

			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(particle_t) * positions.size(), &positions[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(particle_t), (void*)0);
			glEnableVertexAttribArray(0); // 0 = Position

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(particle_t), (char *) nullptr + 3 * sizeof(float));
			glEnableVertexAttribArray(1); // 1 = R/G/B

			glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(particle_t), (char *) nullptr + 6 * sizeof(float));
			glEnableVertexAttribArray(2); // 2 = Size (1 float)

			// We're skipping mode and age

			glBindVertexArray(0);
			glCheckError();
		}

		void render_particles(glm::mat4 &camera_projection_matrix, glm::mat4 &camera_modelview_matrix) {
			if (positions.empty()) return;

			// Use the particle shader
			glUseProgram(assets::particle_shader->shader_id);
			glBindVertexArray(vao);

			// Set uniforms
			glBindBufferBase(GL_UNIFORM_BUFFER, 0, render::camera_ubo::ubo);
			glUniformBlockBinding(assets::particle_shader->shader_id, 0, assets::particle_shader->camera_block_index);
			glCheckError();
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

			// Splat out particle info
			glDrawArrays(GL_POINTS, 0, positions.size());
			//std::cout << "Rendered " << positions.size() << " particles\n";

			// Cleanup
			glBindVertexArray(0);
			glCheckError();
		}
	}
}
