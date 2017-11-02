#include "particle_system.hpp"
#include <vector>
#include "../../bengine/ecs.hpp"
#include "../../components/smoke_emitter.hpp"
#include "../../global_assets/game_pause.hpp"
#include "../../components/position.hpp"
#include "../../global_assets/rng.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../global_assets/shader_storage.hpp"
#include "../../render_engine/fbo/gbuffer.hpp"

namespace systems {
	namespace particles {

		using namespace bengine;

		unsigned int vao = 0;
		unsigned int vbo = 0;

		struct particle_t {
			particle_t() {}
			particle_t(float X, float Y, float Z, float R, float G, float B, float SZ, uint8_t M, float A) : x(X), y(Y), z(Z), r(R), g(G), b(B), size(SZ), pmode(M), age(A) {}

			float x, y, z;
			float r, g, b;
			float size;
			uint8_t pmode;
			float age;
		};

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

		void run(const double &duration_ms) {
			// Automated particle spawners
			if (major_tick) {
				// Smoke from fires
				each<smoke_emitter_t, position_t>([] (entity_t &e, smoke_emitter_t &smoke, position_t &pos) {
					const int n_smoke = rng.roll_dice(3, 10);
					for (int i = 0; i < n_smoke; ++i) {
						float x = static_cast<float>(pos.x) + (static_cast<float>(rng.roll_dice(1, 3)) / 10.0f) - 0.5f;
						float y = static_cast<float>(pos.z) + (static_cast<float>(rng.roll_dice(1, 3)) / 10.0f);
						float z = static_cast<float>(pos.y) + (static_cast<float>(rng.roll_dice(1, 3)) / 10.0f) - 0.5f;
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
			glUseProgram(assets::particle_shader);
			glBindVertexArray(vao);

			// Set uniforms
			glUniformMatrix4fv(glGetUniformLocation(assets::particle_shader, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(camera_projection_matrix));
			glUniformMatrix4fv(glGetUniformLocation(assets::particle_shader, "view_matrix"), 1, GL_FALSE, glm::value_ptr(camera_modelview_matrix));
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