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

namespace render {
    bool camera_moved = true;
    glm::mat4 camera_projection_matrix;
    glm::mat4 camera_modelview_matrix;
    glm::mat4 camera_proj_model_view_matrix;
    boost::container::flat_set<int, std::greater<int>> visible_chunks;
    Frustrum frustrum;
    int projection_mat_loc = -1;
    int view_mat_loc = -1;



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

    inline void render_chunks() {
        // Use the program
        glUseProgram(assets::chunkshader);

        // Assign the uniforms
        glUniformMatrix4fv(projection_mat_loc, 1, GL_FALSE, glm::value_ptr(camera_projection_matrix));
        glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, glm::value_ptr(camera_modelview_matrix));

        // Assign the texture array
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, assets::chunk_texture_array);

        // This is temporary and will change a lot!
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        for (const auto &idx : visible_chunks) {
            //std::cout << idx << ", " << chunks::chunks[idx].has_geometry << "\n";
            if (chunks::chunks[idx].has_geometry && chunks::chunks[idx].ready) {
                // Render backwards to maximize z-buffer efficiency
                for (int z=chunks::CHUNK_SIZE-1; z>=0; --z) {
                    const int layer_z = z + chunks::chunks[idx].base_z;
                    if (layer_z <= camera_position->region_z && layer_z > camera_position->region_z-10 && chunks::chunks[idx].layers[z].vao > 0) {
                        //std::cout << "Activating array " << idx << ":" << layer_z << ", " << chunks::chunks[idx].layers[z].n_elements << "\n";
                        glBindVertexArray(chunks::chunks[idx].layers[z].vao);
                        glDrawArrays(GL_TRIANGLES, 0, chunks::chunks[idx].layers[z].n_elements);
                    }
                }
            }
        }
        //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }


    void render_gl() {
        int screen_w, screen_h;
        glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);

        if (projection_mat_loc < 1) {
            projection_mat_loc = glGetUniformLocation(assets::chunkshader, "projection_matrix");
            assert(projection_mat_loc > -1);
        }
        if (view_mat_loc < 1) {
            view_mat_loc = glGetUniformLocation(assets::chunkshader, "view_matrix");
            assert(view_mat_loc > -1);
        }

        chunk_maintenance();
        if (camera_moved) update_camera();
        sunlight::update(screen_w, screen_h);


        /*glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        render_chunks();
        glDisable(GL_DEPTH_TEST);*/

        sunlight::render_test_quad();



    }
}