#include "render_engine.hpp"
#include "chunks/chunks.hpp"
#include "../global_assets/game_camera.hpp"
#include "../global_assets/shader_storage.hpp"
#include "../global_assets/texture_storage.hpp"
#include "../bengine/main_window.hpp"
#include "fbo/buffertest.hpp"
#include "vox/voxreader.hpp"
#include "vox/voxel_model.hpp"
#include "../components/position.hpp"
#include "../components/buildings/building.hpp"
#include "../components/renderable_composite.hpp"
#include "../global_assets/game_calendar.hpp"
#include "vox/renderables.hpp"
#include "../systems/mouse.hpp"
#include "chunks/cursors.hpp"
#include "../systems/gui/particle_system.hpp"
#include "camera.hpp"
#include "renderbuffers.hpp"
#include "../global_assets/game_mode.hpp"
#include "design_render.hpp"
#include "ssao.hpp"
#include "../components/lightsource.hpp"
#include "../systems/mouse.hpp"
#include "pointlights.hpp"
#include "chunks/water_render.hpp"
#include "../global_assets/game_config.hpp"

namespace render {

    static void chunk_maintenance() {
        if (!chunks::chunks_initialized) {
            chunks::initialize_chunks();
        }
        chunks::update_dirty_chunks();
        chunks::update_buffers();
    }

    static void do_chunk_render() {
        for (const auto &idx : visible_chunks) {
            chunks::chunk_t * target = &chunks::chunks[idx];
            if (target->has_geometry) {
                size_t n_elements = 0;
                for (int z=0; z<chunks::CHUNK_SIZE; ++z) {
                    const int layer_z = z + target->base_z;
                    if (layer_z <= camera_position->region_z && layer_z > camera_position->region_z-10) {
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
        // Use the program
		assets::chunkshader->use();
        glBindFramebuffer(GL_FRAMEBUFFER, gbuffer->fbo_id);
		//glClearDepth(0.0f); // We're using an inverted Z-buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Assign the uniforms
        glUniformMatrix4fv(assets::chunkshader->projection_matrix, 1, GL_FALSE, glm::value_ptr(camera_projection_matrix));
        glUniformMatrix4fv(assets::chunkshader->view_matrix, 1, GL_FALSE, glm::value_ptr(camera_modelview_matrix));
		glUniformMatrix3fv(assets::chunkshader->normal_matrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
		glUniform3f(assets::chunkshader->camera_position, camera_position->region_x, camera_position->region_z, camera_position->region_y);
		if (config::game_config.parallax)
		{
			glUniform1f(assets::chunkshader->use_parallax, 1.0f);
		} else
		{
			glUniform1f(assets::chunkshader->use_parallax, 0.0f);
		}

        // Assign the texture array
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, assets::chunk_texture_array);

        glUniform1i(assets::chunkshader->textureArray, 0);

		if (game_master_mode == DESIGN) {
			// Render single layer
			do_design_render();
		}
		else {
			do_chunk_render();
		}
    }

	static void render_depth_prepass()
    {
		// Use the program
		assets::chunkdepthshader->use();
		glBindFramebuffer(GL_FRAMEBUFFER, gbuffer->fbo_id);
		//glClearDepth(0.0f); // We're using an inverted Z-buffer
		glClear(GL_DEPTH_BUFFER_BIT);

		// Assign the uniforms
		glUniformMatrix4fv(assets::chunkdepthshader->projection_matrix, 1, GL_FALSE, glm::value_ptr(camera_projection_matrix));
		glUniformMatrix4fv(assets::chunkdepthshader->view_matrix, 1, GL_FALSE, glm::value_ptr(camera_modelview_matrix));
		glUniformMatrix3fv(assets::chunkdepthshader->normal_matrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
		glUniform3f(assets::chunkdepthshader->camera_position, camera_position->region_x, camera_position->region_z, camera_position->region_y);

		if (game_master_mode == DESIGN) {
			// Render single layer
			do_design_render();
		}
		else {
			do_chunk_render();
		}
    }

    static void render_to_light_buffer() {
		using assets::lightstage_shader;
		lightstage_shader->use();
        glBindFramebuffer(GL_FRAMEBUFFER, light_stage_buffer->fbo_id);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glUniform1i(lightstage_shader->albedo_tex, 0);
        glUniform1i(lightstage_shader->normal_tex, 1);
        glUniform1i(lightstage_shader->position_tex, 2);
        glUniform1i(lightstage_shader->ao_tex, 3);
        glUniform3f(lightstage_shader->camera_position, (float)camera_position->region_x, (float)camera_position->region_z, (float)camera_position->region_y);
		if (config::game_config.disable_lighting)
		{
			glUniform3f(lightstage_shader->moon_color, 1.0f, 1.0f, 1.0f);
		}
		else {
			glUniform3f(lightstage_shader->moon_color, calendar->moon_r, calendar->moon_g, calendar->moon_b);
		}
		glUniform1i(lightstage_shader->noise_tex, 4);
		glUniformMatrix4fv(lightstage_shader->projection, 1, GL_FALSE, glm::value_ptr(camera_projection_matrix * camera_modelview_matrix));
		glUniform1i(lightstage_shader->gbuffer_depth_tex, 5);
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
		glBindTexture(GL_TEXTURE_2D, noise_tex);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, gbuffer->depth_tex);
        render_buffer_quad();
    }

	static void tone_map_scene() {
        glUseProgram(assets::tonemap_shader);
        glBindFramebuffer(GL_FRAMEBUFFER, hdr_buffer->fbo_id);
        // Setup uniforms
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUniform1i(glGetUniformLocation(assets::tonemap_shader, "hdr_tex"), 0);
        //glUniform1i(glGetUniformLocation(assets::tonemap_shader, "blur_tex"), 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, light_stage_buffer->color_tex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, bloom_buffer->blur_tex1);
        render_buffer_quad();
    }

	void update_buffers() {
		chunk_maintenance();

		build_water_geometry();
		if (camera_moved) update_camera();
		build_voxel_render_list(visible_chunks);
		build_cursor_geometry();
		systems::particles::build_buffers();
		if (game_master_mode == DESIGN) update_design_buffers();
		glCheckError();
	}

	bool depth_test_render = false;	

    void render_gl(const double &duration_ms) {
        glCheckError();
        int screen_w, screen_h;
        glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);

        if (!gbuffer) {
			build_framebuffers(screen_w, screen_h);
        }
		if (!ssao_setup) {
			setup_ssao();
		}
		swap_buffers(); // Cheap double-buffering to speed up glRead back speed.

		// Handle building all of our GL buffers
		update_buffers();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// perform a depth pre-pass
		render_depth_prepass();

		//update_light_buffers(screen_w, screen_h, camera_projection_matrix, camera_modelview_matrix);
		if (!config::game_config.disable_lighting) {
			update_pointlights();
		}

		glDepthFunc(GL_LEQUAL);
        // Render a pre-pass to put color, normal, etc. into the gbuffer. Also puts sunlight in place.
        render_chunks();
        //glCheckError();
		render_voxel_models(gbuffer, camera_projection_matrix, camera_modelview_matrix);
		//glCheckError();
		render_water(camera_projection_matrix, camera_modelview_matrix);

		// Stop writing to the gbuffer and depth-testing
        glDisable(GL_DEPTH_TEST);

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
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		if (!config::game_config.disable_hdr) {
			render_test_quad(hdr_buffer->color_tex);
		} else
		{
			glEnable(GL_FRAMEBUFFER_SRGB);
			render_test_quad(light_stage_buffer->color_tex);
			glDisable(GL_FRAMEBUFFER_SRGB);
		}

		if (depth_test_render) render_test_quad(light_stage_buffer->shiny_tex);

        // TODO: Final combination and post-process

        glCheckError();
    }
}
