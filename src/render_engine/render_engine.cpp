#include "../bengine/gl_include.hpp"
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <boost/container/flat_set.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "render_engine.hpp"
#include "chunks/chunks.hpp"
#include "../global_assets/game_camera.hpp"
#include "frustrum.hpp"
#include "../global_assets/shader_storage.hpp"
#include "../global_assets/texture_storage.hpp"
#include "../bengine/main_window.hpp"
#include "fbo/buffertest.hpp"
#include "fbo/gbuffer.hpp"
#include "fbo/base_lit_buffer.hpp"
#include "world_textures/world_textures.hpp"
#include "fbo/hdr_buffer.hpp"
#include "fbo/bloom_ping_pong.hpp"
#include "vox/voxreader.hpp"
#include "vox/voxel_model.hpp"
#include "../components/position.hpp"
#include "../components/building.hpp"
#include "../components/renderable_composite.hpp"
#include "../global_assets/game_calendar.hpp"
#include "vox/renderables.hpp"
#include "../systems/mouse.hpp"
#include "chunks/cursors.hpp"

namespace render {
    bool camera_moved = true;
    glm::mat4 camera_projection_matrix;
    glm::mat4 camera_modelview_matrix;
    glm::mat4 camera_proj_model_view_matrix;
    boost::container::flat_set<int, std::greater<int>> visible_chunks;
    Frustrum frustrum;
    std::unique_ptr<gbuffer_t> gbuffer;
    std::unique_ptr<base_lit_buffer_t> light_stage_buffer;
    std::unique_ptr<hdr_buffer_t> hdr_buffer;
    std::unique_ptr<bloom_pingpong_t> bloom_buffer;

    inline void chunk_maintenance() {
        if (!chunks::chunks_initialized) {
            chunks::initialize_chunks();
        }
        chunks::update_dirty_chunks();
        chunks::update_buffers();
    }

    inline void update_camera() {
        camera_projection_matrix = glm::perspective(90.0f, 1.0f, 1.0f, 300.0f);

        const glm::vec3 up{0.0f, 1.0f, 0.0f};
        const glm::vec3 target{(float) camera_position->region_x, (float) camera_position->region_z, (float) camera_position->region_y};
        glm::vec3 camera_position_v;
        //camera->camera_mode = DIAGONAL;
        //camera->zoom_level = 2;

        switch (camera->camera_mode) {
            case FRONT : {
                // Nice X-perspective view
                camera_position_v = { (float) camera_position->region_x, ((float) camera_position->region_z) + (float)camera->zoom_level,((float) camera_position->region_y) + ((float)camera->zoom_level/3.0f) };
            } break;

            case TOP_DOWN : {
                // Top-down
                camera_position_v = {(float) camera_position->region_x, ((float) camera_position->region_z) + (float)camera->zoom_level, ((float) camera_position->region_y) + 0.1f};
            } break;

            case DIAGONAL : {
                // Diagonal
                camera_position_v = {(float) camera_position->region_x + (float)camera->zoom_level, ((float) camera_position->region_z) + (float)camera->zoom_level, ((float) camera_position->region_y) + (float)camera->zoom_level};
            } break;
        }
        camera_modelview_matrix = glm::lookAt(camera_position_v, target, up);
        camera_proj_model_view_matrix = camera_projection_matrix * camera_modelview_matrix;

        frustrum.update(camera_proj_model_view_matrix);
        visible_chunks.clear();
        for (const auto &chunk : chunks::chunks) {
            if (frustrum.checkSphere(glm::vec3(chunk.base_x, chunk.base_y, chunk.base_z), chunks::CHUNK_SIZE*2))
            {
                visible_chunks.insert(chunk.index);
            }
        }

        camera_moved = false;
    }

    inline void do_chunk_render() {
        for (const auto &idx : visible_chunks) {
            chunks::chunk_t * target = &chunks::chunks[idx];
            if (target->ready && target->has_geometry) {
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

	inline void do_trans_chunk_render() {
		for (const auto &idx : visible_chunks) {
			chunks::chunk_t * target = &chunks::chunks[idx];
			if (target->ready && target->has_transparency) {
				size_t n_elements = 0;
				for (int z = 0; z<chunks::CHUNK_SIZE; ++z) {
					const int layer_z = z + target->base_z;
					if (layer_z <= camera_position->region_z && layer_z > camera_position->region_z - 10) {
						n_elements += target->layers[z].n_trans;
					}
				}

				if (n_elements > 0) {
					glBindVertexArray(target->tvao);
					glDrawArrays(GL_TRIANGLES, 0, n_elements);
				}
			}
		}
	}	

    inline void render_chunks() {
        // Use the program
		assets::chunkshader->use();
        glBindFramebuffer(GL_FRAMEBUFFER, gbuffer->fbo_id);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // Assign the uniforms
        glUniformMatrix4fv(assets::chunkshader->projection_matrix, 1, GL_FALSE, glm::value_ptr(camera_projection_matrix));
        glUniformMatrix4fv(assets::chunkshader->view_matrix, 1, GL_FALSE, glm::value_ptr(camera_modelview_matrix));

        // Assign the texture array
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, assets::chunk_texture_array);

        glUniform1i(assets::chunkshader->textureArray, 0);

        do_chunk_render();
		do_trans_chunk_render();
    }

    void render_to_light_buffer() {
        glUseProgram(assets::lightstage_shader);
        glBindFramebuffer(GL_FRAMEBUFFER, light_stage_buffer->fbo_id);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glUniform1i(glGetUniformLocation(assets::lightstage_shader, "albedo_tex"), 0);
        glUniform1i(glGetUniformLocation(assets::lightstage_shader, "normal_tex"), 1);
        glUniform1i(glGetUniformLocation(assets::lightstage_shader, "position_tex"), 2);
        glUniform1i(glGetUniformLocation(assets::lightstage_shader, "ao_tex"), 3);
        glUniform1i(glGetUniformLocation(assets::lightstage_shader, "info_tex"), 4);
        glUniform1i(glGetUniformLocation(assets::lightstage_shader, "light_pos_tex"), 5);
        glUniform1i(glGetUniformLocation(assets::lightstage_shader, "light_col_tex"), 6);
        glUniform3f(glGetUniformLocation(assets::lightstage_shader, "camera_position"), (float)camera_position->region_x, (float)camera_position->region_z, (float)camera_position->region_y);
		glUniform3f(glGetUniformLocation(assets::lightstage_shader, "sun_direction"), calendar->sun_x, calendar->sun_y, calendar->sun_z);
		glUniform3f(glGetUniformLocation(assets::lightstage_shader, "moon_direction"), calendar->moon_x, calendar->moon_y, calendar->moon_z);
		glUniform3f(glGetUniformLocation(assets::lightstage_shader, "sun_color"), calendar->sun_r, calendar->sun_g, calendar->sun_b);
		glUniform3f(glGetUniformLocation(assets::lightstage_shader, "moon_color"), calendar->moon_r, calendar->moon_g, calendar->moon_b);
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gbuffer->albedo_tex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gbuffer->normal_tex);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gbuffer->position_tex);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, gbuffer->ao_tex);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_3D, outdoors_y_z_tex);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_3D, lightpos_tex);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_3D, lightcol_tex);
        render_buffer_quad();
    }

    /*
    void bloom_blur() {
        glUseProgram(assets::bloom_shader);
        glActiveTexture(GL_TEXTURE0);
        bool horizontal = true, first_iteration = true;
        constexpr int amount = 10;
        for (unsigned int i=0; i<amount; ++i) {
            if (horizontal) {
                glBindFramebuffer(GL_FRAMEBUFFER, bloom_buffer->fbo1_id);
                glUniform1i(glGetUniformLocation(assets::bloom_shader, "horizontal"), 1);
                if (first_iteration) {
                    glBindTexture(GL_TEXTURE_2D, light_stage_buffer->bright_tex);
                } else {
                    glBindTexture(GL_TEXTURE_2D, bloom_buffer->blur_tex2);
                }
            } else {
                glBindFramebuffer(GL_FRAMEBUFFER, bloom_buffer->fbo2_id);
                glUniform1i(glGetUniformLocation(assets::bloom_shader, "horizontal"), 0);
                glBindTexture(GL_TEXTURE_2D, bloom_buffer->blur_tex1);
            }
            render_buffer_quad();
            horizontal = !horizontal;
            if (first_iteration) first_iteration = false;
        }
    }*/

    void tone_map_scene() {
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

    void render_gl(const double &duration_ms) {
        glCheckError();
        int screen_w, screen_h;
        glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);

        if (!gbuffer) {
            gbuffer = std::make_unique<gbuffer_t>(screen_w, screen_h);
            if (!light_stage_buffer) light_stage_buffer = std::make_unique<base_lit_buffer_t>(screen_w, screen_h);
            if (!hdr_buffer) hdr_buffer = std::make_unique<hdr_buffer_t>(screen_w, screen_h);
            if (!bloom_buffer) bloom_buffer = std::make_unique<bloom_pingpong_t>(screen_w, screen_h);
        }

        chunk_maintenance();
        if (camera_moved) update_camera();
        update_world_textures();
		build_voxel_render_list();
		build_cursor_geometry();
		glCheckError();

        // Render a pre-pass to put color, normal, etc. into the gbuffer. Also puts sunlight in place.
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        //glEnable(GL_CULL_FACE);
        render_chunks();
        //glDisable(GL_CULL_FACE);
        glCheckError();

        render_voxel_models(gbuffer.get(), camera_projection_matrix, camera_modelview_matrix);

        // TODO: Render windows and other transparency

        // Stop writing to the gbuffer and depth-testing
        glDisable(GL_DEPTH_TEST);

        // Render the combined light buffer
        render_to_light_buffer();

		// Add in translucent cursors
		glBindFramebuffer(GL_FRAMEBUFFER, light_stage_buffer->fbo_id);
		render_cursor(camera_projection_matrix, camera_modelview_matrix);

        // Tone mapping
        tone_map_scene();

        // Render some test results
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        render_test_quad(hdr_buffer->color_tex);
        //render_test_quad(gbuffer->ao_tex);

        // TODO: Final combination and post-process

        glCheckError();
    }
}