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

namespace render {
    bool camera_moved = true;
    glm::mat4 camera_projection_matrix;
    glm::mat4 camera_modelview_matrix;
    glm::mat4 camera_proj_model_view_matrix;
    boost::container::flat_set<int, std::greater<int>> visible_chunks;
    Frustrum frustrum;
    int projection_mat_loc = -1;
    int view_mat_loc = -1;
    std::unique_ptr<gbuffer_t> gbuffer;
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
                    if (layer_z <= camera_position->region_z && layer_z > camera_position->region_z-10) {
                        if (chunks::chunks[idx].layers[z].vao > 0
                            && chunks::chunks[idx].layers[z].n_elements > 0) {
                            glBindVertexArray(chunks::chunks[idx].layers[z].vao);
                            glDrawArrays(GL_TRIANGLES, 0, chunks::chunks[idx].layers[z].n_elements);
                            //glCheckError();
                        }
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

        glUniform1i(glGetUniformLocation(assets::chunkshader, "textureArray"), 0);

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
    }

    void render_gl(const double &duration_ms) {
        glCheckError();
        int screen_w, screen_h;
        glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);

        if (!gbuffer) {
            gbuffer = std::make_unique<gbuffer_t>(screen_w, screen_h);
            set_uniform_locs();
            if (!light_stage_buffer) light_stage_buffer = std::make_unique<base_lit_buffer_t>(screen_w, screen_h);
        }

        chunk_maintenance();
        if (camera_moved) update_camera();

        // Render a pre-pass to put color, normal, etc. into the gbuffer. Also puts sunlight in place.
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);
        render_chunks();
        glDisable(GL_CULL_FACE);
        glCheckError();

        // TODO: Render other lights to the gbuffer
        // Stop writing to the gbuffer and depth-testing
        glDisable(GL_DEPTH_TEST);

        // Render the combined light buffer
        glUseProgram(assets::lightstage_shader);
        glBindFramebuffer(GL_FRAMEBUFFER, light_stage_buffer->fbo_id);
        glUniform1i(glGetUniformLocation(assets::lightstage_shader, "albedo_tex"), 0);
        glUniform1i(glGetUniformLocation(assets::lightstage_shader, "normal_tex"), 1);
        glUniform1i(glGetUniformLocation(assets::lightstage_shader, "position_tex"), 2);
        glUniform1i(glGetUniformLocation(assets::lightstage_shader, "light_position_tex"), 3);
        glUniform1i(glGetUniformLocation(assets::lightstage_shader, "light_color_tex"), 4);
        glUniform3f(glGetUniformLocation(assets::lightstage_shader, "camera_position"), (float)camera_position->region_x, (float)camera_position->region_z, (float)camera_position->region_y);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gbuffer->albedo_tex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gbuffer->normal_tex);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gbuffer->position_tex);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, gbuffer->lposition_tex);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, gbuffer->lcolor_tex);
        render_buffer_quad();

        // Render some test results
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        //render_test_quad(light_stage_buffer->color_tex);
        render_test_quad(gbuffer->albedo_tex);

        // TODO: Final combination and post-process

        glCheckError();
    }
}