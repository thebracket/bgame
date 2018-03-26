#include "pointlights.hpp"
#include "fbo/point_light_buffer.hpp"
#include "../bengine/main_window.hpp"
#include "camera.hpp"
#include "renderbuffers.hpp"
#include "../global_assets/shader_storage.hpp"
#include "chunks/chunks.hpp"
#include "fbo/buffertest.hpp"
#include "../global_assets/game_camera.hpp"
#include "../global_assets/game_calendar.hpp"
#include "vox/renderables.hpp"
#include "../global_assets/game_config.hpp"
#include "shaders/ascii_light_shader.hpp"
#include "shaders/lighter_shader.hpp"
#include "shaders/pointlight_shader.hpp"
#include "vox/voxreader.hpp"
#include "ubo/lighting_ubo.hpp"
#include "../global_assets/game_ecs.hpp"

namespace render {
	void pointlight_t::make_mats() {
		shadow_transforms.clear();
		const glm::mat4 light_projection_matrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.25f, radius);
		shadow_transforms.push_back(light_projection_matrix * glm::lookAt(light_pos, light_pos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadow_transforms.push_back(light_projection_matrix * glm::lookAt(light_pos, light_pos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadow_transforms.push_back(light_projection_matrix * glm::lookAt(light_pos, light_pos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadow_transforms.push_back(light_projection_matrix * glm::lookAt(light_pos, light_pos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadow_transforms.push_back(light_projection_matrix * glm::lookAt(light_pos, light_pos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadow_transforms.push_back(light_projection_matrix * glm::lookAt(light_pos, light_pos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
	}

	void pointlight_t::make_buffer() {
		if (!buffer) buffer = std::make_unique<point_light_buffer_t>(config::game_config.shadow_map_size, config::game_config.shadow_map_size);
	}

	void pointlight_t::draw_depth_buffer() {
		int screen_w, screen_h;
		glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);
		assets::pointlight_shader->use();

		glBindFramebuffer(GL_FRAMEBUFFER, buffer->fbo_id);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, lighting_ubo::ubo);
		glUniformBlockBinding(assets::pointlight_shader->shader_id, 0, assets::pointlight_shader->light_block_index);
		glCheckError();
		glUniform1ui(assets::pointlight_shader->light_index, ubo_index);
		glCheckError();

		// Render everything to it - chunks
		glViewport(0, 0, config::game_config.shadow_map_size, config::game_config.shadow_map_size);
		glClear(GL_DEPTH_BUFFER_BIT);
		for (const auto &idx : visible_chunks) {
			chunks::chunk_t * target = &chunks::chunks[idx.chunk_id];
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
		glCheckError();
		vox::bulk_shadow_render(radius, shadow_transforms, light_pos, buffer->fbo_id, ubo_index);

		glViewport(0, 0, screen_w, screen_h);
		new_light = false;
	}

	void pointlight_t::render_light() const {
		glUseProgram(assets::lighter_shader->shader_id);
		glBindFramebuffer(GL_FRAMEBUFFER, light_stage_buffer->fbo_id);

		// Set uniforms
		glUniform3f(assets::lighter_shader->light_position, light_pos.x, light_pos.y, light_pos.z);
		glUniform3f(assets::lighter_shader->camera_position, (float)camera_position->region_x, (float)camera_position->region_z, (float)camera_position->region_y);
		glUniform3f(assets::lighter_shader->light_color, light_col.r, light_col.g, light_col.b);
		glUniform1f(assets::lighter_shader->far_plane, radius);
		glUniform1i(assets::lighter_shader->world_position, 0);
		glUniform1i(assets::lighter_shader->depth_map, 1);
		glUniform1i(assets::lighter_shader->albedo_tex, 2);
		glUniform1i(assets::lighter_shader->normal_tex, 3);
		glUniform1i(assets::lighter_shader->ao_tex, 4);

		// Bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gbuffer->position_tex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, buffer->depth_cubemap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gbuffer->albedo_tex);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gbuffer->normal_tex);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, gbuffer->ao_tex);

		// Setup a blend mode
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		// Render it
		render_buffer_quad();
		glCheckError();

		glDisable(GL_BLEND);
	}

	static std::map<std::size_t, pointlight_t> pointlights;
	static uint8_t cycle = 0;
	static bool first_run = true;

	static void add_lightsource(const std::size_t &id, const lightsource_t &l, const position_t &pos) {
		pointlights[id] = pointlight_t{};

		pointlights[id].light_pos = glm::vec3{ (float)pos.x, (float)pos.z, (float)pos.y };
		pointlights[id].light_col = glm::vec3{ l.color.r, l.color.g, l.color.b };
		pointlights[id].radius = l.radius;
		pointlights[id].cycle_tick = id % 20;
		pointlights[id].make_mats();
		pointlights[id].make_buffer();
	}

	void update_pointlights() {
		using namespace bengine;

		// List current lights
		bool did_something = false;
		each<lightsource_t, position_t>([&did_something](entity_t &e, lightsource_t &l, position_t &pos) {
			auto finder = pointlights.find(e.id);
			if (finder == pointlights.end()) {
				// Insert a light
				add_lightsource(e.id, l, pos);
				did_something = true;
			}
			else {				
				auto changed = false;
				if (pos.x != finder->second.light_pos.x) { changed = true; finder->second.light_pos.x = (float)pos.x; }
				if (pos.y != finder->second.light_pos.z) { changed = true; finder->second.light_pos.z = (float)pos.y; }
				if (pos.z != finder->second.light_pos.y) { changed = true; finder->second.light_pos.y = (float)pos.z; }
				if (changed) {
					//std::cout << "Light updated!\n";
					finder->second.make_mats();
					did_something = true;
				}
				if (l.color.r != finder->second.light_col.r) finder->second.light_col.r = l.color.r;
				if (l.color.g != finder->second.light_col.g) finder->second.light_col.g = l.color.g;
				if (l.color.b != finder->second.light_col.b) finder->second.light_col.b = l.color.b;
			}
		});
		if (did_something) render::lighting_ubo::update_lights(pointlights);

		glEnable(GL_CULL_FACE);
		const auto camera_z_minus_4 = camera_position->region_z - 4;
		const auto camera_z_plus_4 = camera_position->region_z + 4;
		for (auto &l : pointlights) {
			auto visible = true;
			if (l.second.light_pos.y < camera_z_minus_4) visible = false;
			if (visible && l.second.light_pos.y > camera_z_plus_4) visible = false;
			if (visible && !frustrum.checkSphere(glm::vec3(l.second.light_pos.x, l.second.light_pos.y, l.second.light_pos.z), l.second.radius)) visible = false;

			if (l.second.new_light || visible && (config::game_config.always_update_shadows ||l.second.cycle_tick == cycle)) {
				l.second.draw_depth_buffer();
			}
		}
		glDisable(GL_CULL_FACE);

		++cycle;
		if (cycle > config::game_config.shadow_divisor) cycle = 0;
	}

	void render_pointlights() {
		for (auto &l : pointlights) {
			if (l.first == std::numeric_limits<std::size_t>::max() && (calendar->hour < 7 || calendar->hour > 17)) break;
			l.second.render_light();
		}
	}

	void render_ascii_pointlights(unsigned int &ascii_fbo, unsigned int &ascii_vao, unsigned int &texture_id, std::size_t buffer_size, glm::mat4 &ascii_projection_matrix, glm::mat4 &ascii_modelview_matrix) {
		for (auto &l : pointlights) {
			if (l.first == std::numeric_limits<std::size_t>::max() && (calendar->hour < 7 || calendar->hour > 17)) break;

			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			glDisable(GL_DEPTH_TEST);
			glBindFramebuffer(GL_FRAMEBUFFER, ascii_fbo);
			glCheckError();
			glUseProgram(assets::ascii_light_shader->shader_id);
			glCheckError();
			glBindVertexArray(ascii_vao);
			glCheckError();
			glUniformMatrix4fv(assets::ascii_light_shader->projection_matrix, 1, GL_FALSE, glm::value_ptr(ascii_projection_matrix));
			glUniformMatrix4fv(assets::ascii_light_shader->view_matrix, 1, GL_FALSE, glm::value_ptr(ascii_modelview_matrix));
			glUniform1i(assets::ascii_light_shader->ascii_tex, 0);
			glUniform3f(assets::ascii_light_shader->light_position, l.second.light_pos.x, l.second.light_pos.y, l.second.light_pos.z);
			glUniform1f(assets::ascii_light_shader->far_plane, l.second.radius);
			glUniform1i(assets::ascii_light_shader->depth_map, 1);
			glUniform3f(assets::ascii_light_shader->light_color, l.second.light_col.r, l.second.light_col.g, l.second.light_col.b);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_id);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, l.second.buffer->depth_cubemap);
			glDrawArrays(GL_TRIANGLES, 0, buffer_size);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_BLEND);
		}
	}
}
