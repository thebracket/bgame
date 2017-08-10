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
#include "fbo/depth_fbo.hpp"
#include "../bengine/main_window.hpp"
#include "sunlight.hpp"
#include "fbo/buffertest.hpp"
#include "fbo/gbuffer.hpp"
#include "fbo/lbuffer.hpp"
#include "fbo/base_lit_buffer.hpp"

namespace render {
    bool camera_moved = true;
    glm::mat4 camera_projection_matrix;
    glm::mat4 camera_modelview_matrix;
    glm::mat4 camera_proj_model_view_matrix;
    boost::container::flat_set<int, std::greater<int>> visible_chunks;
    Frustrum frustrum;
    int projection_mat_loc = -1;
    int view_mat_loc = -1;
    int dirlight_projection_loc = -1;
    int dirlight_view_loc = -1;
    int dirlight_lightpos_loc = -1;
    int dirlight_lightmatrix_loc = -1;
    int dirlight_lightcol_loc = -1;
    std::unique_ptr<gbuffer_t> gbuffer;
    std::unique_ptr<lbuffer_t> lbuffer;
    std::unique_ptr<base_lit_buffer_t> light_stage_buffer;

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
            if (frustrum.checkSphere(glm::vec3(chunk.base_x, chunk.base_y, chunk.base_z), chunks::CHUNK_SIZE*2)) {
                visible_chunks.insert(chunk.index);
            }
        }

        camera_moved = false;
    }

    inline void do_chunk_render() {
        for (const auto &idx : visible_chunks) {
            if (chunks::chunks[idx].has_geometry && chunks::chunks[idx].ready) {
                // Render backwards to maximize z-buffer efficiency
                for (int z=chunks::CHUNK_SIZE-1; z>=0; --z) {
                    const int layer_z = z + chunks::chunks[idx].base_z;
                    if (layer_z <= camera_position->region_z && layer_z > camera_position->region_z-10 && chunks::chunks[idx].layers[z].vao > 0) {
                        glBindVertexArray(chunks::chunks[idx].layers[z].vao);
                        glDrawArrays(GL_TRIANGLES, 0, chunks::chunks[idx].layers[z].n_elements);
                    }
                }
            }
        }
    }

    inline void render_chunks() {
        // Use the program
        glUseProgram(assets::chunkshader);
        glBindFramebuffer(GL_FRAMEBUFFER, gbuffer->fbo_id);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // Assign the uniforms
        glUniformMatrix4fv(projection_mat_loc, 1, GL_FALSE, glm::value_ptr(camera_projection_matrix));
        glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, glm::value_ptr(camera_modelview_matrix));

        // Assign the texture array
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, assets::chunk_texture_array);

        do_chunk_render();
    }

    inline void set_uniform_locs() {
        if (projection_mat_loc < 1) {
            projection_mat_loc = glGetUniformLocation(assets::chunkshader, "projection_matrix");
            assert(projection_mat_loc > -1);
        }
        if (view_mat_loc < 1) {
            view_mat_loc = glGetUniformLocation(assets::chunkshader, "view_matrix");
            assert(view_mat_loc > -1);
        }
        if (dirlight_lightpos_loc < 1) {
            dirlight_lightpos_loc = glGetUniformLocation(assets::dirlight_apply_shader, "lightPos");
            assert(dirlight_lightpos_loc > -1);
        }
        if (dirlight_projection_loc < 1) {
            dirlight_projection_loc = glGetUniformLocation(assets::dirlight_apply_shader, "projection_matrix");
            assert(dirlight_projection_loc > -1);
        }
        if (dirlight_view_loc < 1) {
            dirlight_view_loc = glGetUniformLocation(assets::dirlight_apply_shader, "view_matrix");
            assert(dirlight_view_loc > -1);
        }
        if (dirlight_lightmatrix_loc < 1) {
            dirlight_lightmatrix_loc = glGetUniformLocation(assets::dirlight_apply_shader, "lightSpaceMatrix");
            assert(dirlight_lightmatrix_loc > -0);
        }
        if (dirlight_lightcol_loc < 1) {
            dirlight_lightcol_loc = glGetUniformLocation(assets::dirlight_apply_shader, "lightColor");
            assert(dirlight_lightcol_loc > -0);
        }
    }

    void render_sun_buffer() {
        // Render sunlight to the lbuffer
        glUseProgram(assets::dirlight_apply_shader);
        glBindFramebuffer(GL_FRAMEBUFFER, lbuffer->fbo_id);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glUniform3fv(dirlight_lightpos_loc, 1, glm::value_ptr(sunlight::light_position));
        glUniformMatrix4fv(dirlight_projection_loc, 1, GL_FALSE, glm::value_ptr(camera_projection_matrix));
        glUniformMatrix4fv(dirlight_view_loc, 1, GL_FALSE, glm::value_ptr(camera_modelview_matrix));
        glUniformMatrix4fv(dirlight_lightmatrix_loc, 1, GL_FALSE, glm::value_ptr(sunlight::lightSpaceMatrix));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sunlight::sun_fbo->depth_map);
        do_chunk_render();
    }

    void render_gl(const double &duration_ms) {
        glCheckError();
        int screen_w, screen_h;
        glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);

        if (!gbuffer) {
            gbuffer = std::make_unique<gbuffer_t>(screen_w, screen_h);
            set_uniform_locs();
            if (!lbuffer) lbuffer = std::make_unique<lbuffer_t>(screen_w, screen_h);
            if (!light_stage_buffer) light_stage_buffer = std::make_unique<base_lit_buffer_t>(screen_w, screen_h);
        }

        chunk_maintenance();
        if (camera_moved) update_camera();

        // Update lighting buffers
        sunlight::update(screen_w, screen_h, duration_ms);

        // Render a pre-pass to put color, normal, etc. into the gbuffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        render_chunks();

        // Render sunlight
        render_sun_buffer();
        // TODO: Render other lights to the gbuffer
        // Stop writing to the gbuffer and depth-testing
        glDisable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Render the combined light buffer
        glUseProgram(assets::lightstage_shader);
        glBindFramebuffer(GL_FRAMEBUFFER, light_stage_buffer->fbo_id);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gbuffer->albedo_tex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gbuffer->normal_tex);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gbuffer->position_tex);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, lbuffer->position_tex);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, lbuffer->color_tex);
        render_buffer_quad();

        // Render some test results
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        render_test_quad(light_stage_buffer->color_tex);

        // TODO: Final combination and post-process

        glCheckError();
    }
}