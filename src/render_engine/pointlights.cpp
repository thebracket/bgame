#include "pointlights.hpp"
#include "../bengine/gl_include.hpp"
#include "../bengine/ecs.hpp"
#include "../components/lightsource.hpp"
#include "../components/position.hpp"
#include "fbo/point_light_buffer.hpp"
#include "../bengine/main_window.hpp"
#include "camera.hpp"
#include "renderbuffers.hpp"
#include "../global_assets/shader_storage.hpp"
#include "chunks/chunks.hpp"
#include "fbo/buffertest.hpp"
#include "../global_assets/game_camera.hpp"
#include "../global_assets/game_calendar.hpp"
#include <map>
#include <memory>

namespace render {
	struct pointlight_t {
		int size = 128;
		bool new_light = true;
		uint8_t cycle_tick = 0;
		glm::vec3 light_pos;
		glm::vec3 light_col;
		float radius;
		std::vector<glm::mat4> shadowTransforms;
		std::unique_ptr<point_light_buffer_t> buffer;

		void make_mats() {
			glm::mat4 light_projection_matrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, radius);
			shadowTransforms.push_back(light_projection_matrix * glm::lookAt(light_pos, light_pos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
			shadowTransforms.push_back(light_projection_matrix * glm::lookAt(light_pos, light_pos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
			shadowTransforms.push_back(light_projection_matrix * glm::lookAt(light_pos, light_pos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
			shadowTransforms.push_back(light_projection_matrix * glm::lookAt(light_pos, light_pos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
			shadowTransforms.push_back(light_projection_matrix * glm::lookAt(light_pos, light_pos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
			shadowTransforms.push_back(light_projection_matrix * glm::lookAt(light_pos, light_pos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
		}

		void make_buffer() {
			if (!buffer) buffer = std::make_unique<point_light_buffer_t>(size, size);
		}

		void draw_depth_buffer() {
			int screen_w, screen_h;
			glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);

			glBindFramebuffer(GL_FRAMEBUFFER, pointlight_buffer->fbo_id);
			glUseProgram(assets::pointlight_shader);
			glUniform3f(glGetUniformLocation(assets::pointlight_shader, "lightPos"), light_pos.x, light_pos.y, light_pos.z);
			glUniform1f(glGetUniformLocation(assets::pointlight_shader, "far_plane"), radius);
			glUniformMatrix4fv(glGetUniformLocation(assets::pointlight_shader, "shadowMatrices[0]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[0]));
			glUniformMatrix4fv(glGetUniformLocation(assets::pointlight_shader, "shadowMatrices[1]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[1]));
			glUniformMatrix4fv(glGetUniformLocation(assets::pointlight_shader, "shadowMatrices[2]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[2]));
			glUniformMatrix4fv(glGetUniformLocation(assets::pointlight_shader, "shadowMatrices[3]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[3]));
			glUniformMatrix4fv(glGetUniformLocation(assets::pointlight_shader, "shadowMatrices[4]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[4]));
			glUniformMatrix4fv(glGetUniformLocation(assets::pointlight_shader, "shadowMatrices[5]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[5]));

			// Render everything to it - chunks
			glViewport(0, 0, size, size);
			glClear(GL_DEPTH_BUFFER_BIT);
			for (const auto &idx : visible_chunks) {
				chunks::chunk_t * target = &chunks::chunks[idx];
				if (target->has_geometry) {
					size_t n_elements = 0;
					for (int z = 0; z<chunks::CHUNK_SIZE; ++z) {
						const int layer_z = z + target->base_z;
						n_elements += target->layers[z].n_elements;
					}

					if (n_elements > 0) {
						glBindVertexArray(target->vao);
						glDrawArrays(GL_TRIANGLES, 0, n_elements);
					}
				}
			}
			//render_voxel_models_shadow(projection, modelview);
			glViewport(0, 0, screen_w, screen_h);
			new_light = false;
		}

		void render_light() {
			glUseProgram(assets::lighter_shader);
			glBindFramebuffer(GL_FRAMEBUFFER, light_stage_buffer->fbo_id);

			// Set uniforms
			glUniform3f(glGetUniformLocation(assets::lighter_shader, "light_position"), light_pos.x, light_pos.y, light_pos.z);
			glUniform3f(glGetUniformLocation(assets::lighter_shader, "camera_position"), (float)camera_position->region_x, (float)camera_position->region_z, (float)camera_position->region_y);
			glUniform3f(glGetUniformLocation(assets::lighter_shader, "light_color"), light_col.r, light_col.g, light_col.b);
			glUniform1f(glGetUniformLocation(assets::lighter_shader, "far_plane"), radius);
			glUniform1i(glGetUniformLocation(assets::lighter_shader, "world_position"), 0);
			glUniform1i(glGetUniformLocation(assets::lighter_shader, "depthMap"), 1);
			glUniform1i(glGetUniformLocation(assets::lighter_shader, "albedo_tex"), 2);
			glUniform1i(glGetUniformLocation(assets::lighter_shader, "normal_tex"), 3);
			glUniform1i(glGetUniformLocation(assets::lighter_shader, "ao_tex"), 4);

			// Bind textures
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gbuffer->position_tex);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, pointlight_buffer->depth_cubemap);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, gbuffer->albedo_tex);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, gbuffer->normal_tex);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, gbuffer->ao_tex);

			// TODO: Setup a blend mode
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);

			// Render it
			render_buffer_quad();
			glCheckError();

			glDisable(GL_BLEND);
		}
	};

	static std::map<std::size_t, pointlight_t> pointlights;
	static uint8_t cycle = 0;

	static void add_lightsource(const std::size_t &id, const lightsource_t &l, const position_t &pos) {
		pointlights[id] = pointlight_t{};

		pointlights[id].size = 128;
		pointlights[id].light_pos = glm::vec3{ (float)pos.x, (float)pos.z, (float)pos.y };
		pointlights[id].light_col = glm::vec3{ l.color.r, l.color.g, l.color.b };
		pointlights[id].radius = l.radius;
		pointlights[id].cycle_tick = id % 20;
		pointlights[id].make_mats();
		pointlights[id].make_buffer();
	}

	static bool first_run = true;

	void update_pointlights() {
		using namespace bengine;

		if (first_run) {
			const std::size_t id = std::numeric_limits<std::size_t>::max();
			pointlights[id] = pointlight_t{};

			pointlights[id].size = 256;
			pointlights[id].light_pos = glm::vec3{ calendar->sun_x, calendar->sun_y, calendar->sun_z };
			pointlights[id].light_col = glm::vec3{ 1.0f, 1.0f, 1.0f };
			pointlights[id].radius = 200.0f;
			pointlights[id].cycle_tick = id % 20;
			pointlights[id].make_mats();
			pointlights[id].make_buffer();

			first_run = false;
		}

		// List current lights
		each<lightsource_t, position_t>([](entity_t &e, lightsource_t &l, position_t &pos) {
			auto finder = pointlights.find(e.id);
			if (finder == pointlights.end()) {
				// Insert a light
				add_lightsource(e.id, l, pos);
			}
		});

		for (auto &l : pointlights) {
			if (l.second.new_light || l.second.cycle_tick == cycle) {
				if (l.first == std::numeric_limits<std::size_t>::max()) {
					pointlights[std::numeric_limits<std::size_t>::max()].light_pos = glm::vec3{ calendar->sun_x, calendar->sun_y, calendar->sun_z };
					//std::cout << "UPDATING THE SUN\n";
					//std::cout << calendar->sun_x << "," << calendar->sun_y << "," << calendar->sun_z << "\n";
					l.second.make_mats();
				}
				l.second.draw_depth_buffer();
			}
		}

		++cycle;
		if (cycle > 20) cycle = 0;
	}

	void render_pointlights() {
		for (auto &l : pointlights) {
			l.second.render_light();
		}
	}
}