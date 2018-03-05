#include "render_engine.hpp"
#include "chunks/chunks.hpp"
#include "../global_assets/game_camera.hpp"
#include "../global_assets/shader_storage.hpp"
#include "../global_assets/texture_storage.hpp"
#include "../bengine/main_window.hpp"
#include "fbo/buffertest.hpp"
#include "../components/buildings/building.hpp"
#include "../global_assets/game_calendar.hpp"
#include "vox/renderables.hpp"
#include "chunks/cursors.hpp"
#include "../systems/gui/particle_system.hpp"
#include "camera.hpp"
#include "renderbuffers.hpp"
#include "../global_assets/game_mode.hpp"
#include "design_render.hpp"
#include "ssao.hpp"
#include "pointlights.hpp"
#include "chunks/water_render.hpp"
#include "../global_assets/game_config.hpp"
#include "shaders/chunk_shader.hpp"
#include "shaders/chunk_depth_shader.hpp"
#include "shaders/lightstage_shader.hpp"
#include "shaders/tonemap_shader.hpp"
#include "ubo/first_stage_ubo.hpp"

namespace render {

    static void chunk_maintenance() {
        if (!chunks::chunks_initialized) {
            chunks::initialize_chunks();
        }
        chunks::update_dirty_chunks();
        chunks::update_buffers();
    }

    static void do_chunk_render() {
		const auto camera_ceiling = camera->following>0 && camera->fps ? camera_position->region_z+20 : camera_position->region_z;
        for (const auto &idx : visible_chunks) {
            const auto * target = &chunks::chunks[idx.chunk_id];
            if (target->has_geometry) {
                size_t n_elements = 0;
                for (auto z=0; z<chunks::CHUNK_SIZE; ++z) {
                    const auto layer_z = z + target->base_z;
                    if (layer_z <= camera_ceiling && layer_z > camera_position->region_z-10) {
                        n_elements += target->layers[z].n_elements;
                    }
                }

                if (n_elements > 0) {
                    glBindVertexArray(target->vao);
                    glDrawArrays(GL_TRIANGLES, 0, n_elements);
                }
            }
        }
    }

	static const glm::mat3 identity_matrix{ 1.0 };
	static const glm::mat3 normal_matrix = glm::transpose(glm::inverse(identity_matrix));

    static void render_chunks() {
		glDepthMask(GL_FALSE);
        // Use the program
		assets::chunkshader->use();
        glBindFramebuffer(GL_FRAMEBUFFER, gbuffer->fbo_id);
		//glClearDepth(0.0f); // We're using an inverted Z-buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Assign the uniforms
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_ubo::ubo);
		glUniformBlockBinding(assets::chunkshader->shader_id, 0, assets::chunkshader->camera_block_index);
		glCheckError();

        // Assign the texture array
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, assets::chunk_texture_array);

        glUniform1i(assets::chunkshader->textureArray, 0);
		glEnable(GL_CULL_FACE);
		if (game_master_mode == DESIGN) {
			// Render single layer
			do_design_render();
		}
		else {
			do_chunk_render();
		}
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_TRUE);
    }

	static void render_depth_prepass()
    {
		glDepthMask(GL_TRUE);
		glCheckError();

		// Use the program
		assets::chunkdepthshader->use();
		glCheckError();
		glBindFramebuffer(GL_FRAMEBUFFER, gbuffer->fbo_id);
		glCheckError();
		//glClearDepth(0.0f); // We're using an inverted Z-buffer
		glClear(GL_DEPTH_BUFFER_BIT);
		glCheckError();

		// Assign the uniforms
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_ubo::ubo);
		glUniformBlockBinding(assets::chunkdepthshader->shader_id, 0, assets::chunkdepthshader->camera_block_index);
		glCheckError();

		glColorMaski(0, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glCheckError();

		if (game_master_mode == DESIGN) {
			// Render single layer
			do_design_render();
		}
		else {
			do_chunk_render();
		}
		glCheckError();

		glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glCheckError();
    }

    static void render_to_light_buffer() {
		using assets::lightstage_shader;
		lightstage_shader->use();
        glBindFramebuffer(GL_FRAMEBUFFER, light_stage_buffer->fbo_id);
        //glClear(GL_COLOR_BUFFER_BIT);

        glUniform1i(lightstage_shader->albedo_tex, 0);
        glUniform1i(lightstage_shader->normal_tex, 1);
        glUniform1i(lightstage_shader->position_tex, 2);
        glUniform1i(lightstage_shader->ao_tex, 3);
        glUniform3f(lightstage_shader->camera_position, static_cast<float>(camera_position->region_x), static_cast<float>(camera_position->region_z), static_cast<float>(camera_position->region_y));
		if (config::game_config.disable_lighting)
		{
			glUniform3f(lightstage_shader->moon_color, 1.0f, 1.0f, 1.0f);
		}
		else {
			glUniform3f(lightstage_shader->moon_color, calendar->moon_r, calendar->moon_g, calendar->moon_b);
		}
		glUniformMatrix4fv(lightstage_shader->projection, 1, GL_FALSE, glm::value_ptr(camera_projection_matrix * camera_modelview_matrix));
		glUniform1i(lightstage_shader->gbuffer_depth_tex, 4);
		if (config::game_config.disable_ssao)
		{
			glUniform1f(lightstage_shader->useSSAO, 0);
		}
		else {
			glUniform1f(lightstage_shader->useSSAO, 1);
		}
		send_samples_to_shader();

		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gbuffer->albedo_tex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gbuffer->normal_tex);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gbuffer->position_tex);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, gbuffer->ao_tex);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, gbuffer->depth_tex);
        render_buffer_quad();
    }

	static void tone_map_scene() {
        glUseProgram(assets::tonemap_shader->shader_id);
        glBindFramebuffer(GL_FRAMEBUFFER, hdr_buffer->fbo_id);
        // Setup uniforms
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUniform1i(assets::tonemap_shader->hdr_tex, 0);
        //glUniform1i(glGetUniformLocation(assets::tonemap_shader, "blur_tex"), 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, light_stage_buffer->color_tex);
        render_buffer_quad();
    }

	void update_buffers() {
		chunk_maintenance();

		build_water_geometry();
		if (camera_moved) update_camera();
		build_voxel_render_list();
		build_cursor_geometry();
		systems::particles::build_buffers();
		if (game_master_mode == DESIGN) update_design_buffers();
		glCheckError();
	}

	bool depth_test_render = false;	

	const static double MS_PER_RENDERTICK = 33.0;
	static double time_count = 0.0;
	static bool first_time = true;

    void render_gl(const double &duration_ms) {
		bool tick = false;
		time_count += duration_ms;
		if (time_count > MS_PER_RENDERTICK) {
			time_count = 0.0;
			tick = true;
		}
		if (first_time)
		{
			first_time = false;
			tick = true;
		}
		//if (!camera_moved && !models_changed && !systems::mouse_moved)
		//{
			// If nothing changed, why bother rendering?
		//	tick = false;
		//}
		if (!tick) {
			if (!config::game_config.disable_hdr) {
				render_fullscreen_quad(hdr_buffer->color_tex);
			}
			else
			{
				glEnable(GL_FRAMEBUFFER_SRGB);
				render_fullscreen_quad(light_stage_buffer->color_tex);
				glDisable(GL_FRAMEBUFFER_SRGB);
			}
			return;
		}

        glCheckError();
        int screen_w, screen_h;
        glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);

        if (!gbuffer) {
			build_framebuffers(screen_w, screen_h);
        }
		if (!ssao_setup) {
			setup_ssao();
		}

		// Handle building all of our GL buffers
		update_buffers();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		//update_light_buffers(screen_w, screen_h, camera_projection_matrix, camera_modelview_matrix);
		if (!config::game_config.disable_lighting) {
			glColorMaski(0, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			update_pointlights();
			glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		}

		glDepthFunc(GL_LEQUAL);

		// perform a depth pre-pass
		render_depth_prepass();
		
		// Render a pre-pass to put color, normal, etc. into the gbuffer. Also puts sunlight in place.
        render_chunks();
        //glCheckError();
		render_voxel_models(gbuffer, camera_projection_matrix, camera_modelview_matrix);
		//glCheckError();
		render_water(camera_projection_matrix, camera_modelview_matrix);

		// Stop writing to the gbuffer and depth-testing
        glDisable(GL_DEPTH_TEST);

		// Render some sky
		glUseProgram(assets::skyshader);
		glBindFramebuffer(GL_FRAMEBUFFER, light_stage_buffer->fbo_id);
		const auto sun_pos = glm::normalize(glm::vec3{ calendar->sun_x, calendar->sun_y, calendar->sun_z });
		glUniform3f(glGetUniformLocation(assets::skyshader, "uSunPos"), sun_pos.x, sun_pos.y, sun_pos.z);
		render_fullscreen_quad_untextured();

        // Render the combined light buffer
        render_to_light_buffer();
		if (!config::game_config.disable_lighting) {
			render_pointlights();
		}		

		// Add in translucent cursors
		glBindFramebuffer(GL_FRAMEBUFFER, light_stage_buffer->fbo_id);
		render_cursor(camera_projection_matrix, camera_modelview_matrix);
		systems::particles::render_particles(camera_projection_matrix, camera_modelview_matrix);

		// TODO: Downsample buffers here

        // Tone mapping
		if (!config::game_config.disable_hdr) {
			tone_map_scene();
		}

        // Render some test results
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		if (!config::game_config.disable_hdr) {
			render_fullscreen_quad(hdr_buffer->color_tex);
		} else
		{
			glEnable(GL_FRAMEBUFFER_SRGB);
			render_fullscreen_quad(light_stage_buffer->color_tex);
			glDisable(GL_FRAMEBUFFER_SRGB);
		}

		if (depth_test_render) render_fullscreen_quad(light_stage_buffer->shiny_tex);


        // TODO: Final combination and post-process

        glCheckError();
    }
}
