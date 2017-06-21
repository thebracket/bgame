#include "phase2_terrain.hpp"
#include "../../main/game_camera.hpp"
#include "main_fbo.hpp"
#include "phase1_sunmoon.hpp"
#include "gl_utils.hpp"

namespace map_render {
    bool loaded_terrain_shader = false;
    std::unique_ptr<gl::base_shader_t> terrain_chunk_shader;
    glm::mat4 camera_projection_matrix;
    glm::mat4 camera_modelview_matrix;

    // Uniform/attribute locations
    GLint terrain_world_position_loc;
    GLint terrain_normal_loc;
    GLint terrain_color_loc;
    GLint terrain_texture_position_loc;
    GLint terrain_projection_matrix_loc;
    GLint terrain_view_matrix_loc;
    GLint terrain_camera_position_loc;
    GLint terrain_my_color_texture_loc;

    void load_terrain_shader() {
        terrain_chunk_shader = std::make_unique<gl::base_shader_t>("world_defs/shaders/terrain_vertex.glsl",
                                                                   "world_defs/shaders/terrain_fragment.glsl");
        loaded_terrain_shader = true;

        terrain_world_position_loc = terrain_chunk_shader->get_attribute_location("world_position");
        terrain_normal_loc = terrain_chunk_shader->get_attribute_location("normal");
        terrain_color_loc = terrain_chunk_shader->get_attribute_location("color");
        terrain_texture_position_loc = terrain_chunk_shader->get_attribute_location("texture_position");
        terrain_projection_matrix_loc = terrain_chunk_shader->get_uniform_location("projection_matrix");
        terrain_view_matrix_loc = terrain_chunk_shader->get_uniform_location("view_matrix");
        terrain_camera_position_loc = terrain_chunk_shader->get_uniform_location("camera_position");
        terrain_my_color_texture_loc = terrain_chunk_shader->get_uniform_location("my_color_texture");
    }

    void setup_matrices() {
        auto screen_size = rltk::get_window()->getSize();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT);
        glShadeModel(GL_SMOOTH);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glViewport(0,0,screen_size.x,screen_size.y);

        //glMatrixMode(GL_PROJECTION);
        //glLoadIdentity();
        //gluPerspective(90.f, 1.f, 1.f, 300.0f);
        camera_projection_matrix = glm::perspective(90.0f, 1.0f, 1.0f, 300.0f);

        //glMatrixMode(GL_MODELVIEW);
        //glLoadIdentity();

        const glm::vec3 up{0.0f, 1.0f, 0.0f};
        const glm::vec3 target{(float) camera_position->region_x, (float) camera_position->region_z, (float) camera_position->region_y};
        glm::vec3 position;

        switch (camera->camera_mode) {
            case FRONT : {
                // Nice X-perspective view
                position = { (float) camera_position->region_x, ((float) camera_position->region_z) + (float)camera->zoom_level,((float) camera_position->region_y) + ((float)camera->zoom_level/3.0f) };
            } break;

            case TOP_DOWN : {
                // Top-down
                position = {(float) camera_position->region_x, ((float) camera_position->region_z) + (float)camera->zoom_level, ((float) camera_position->region_y) + 0.1f};
            } break;

            case DIAGONAL : {
                // Diagonal
                position = {(float) camera_position->region_x + (float)camera->zoom_level, ((float) camera_position->region_z) + (float)camera->zoom_level, ((float) camera_position->region_y) + (float)camera->zoom_level};
            } break;
        }
        camera_modelview_matrix = glm::lookAt(position, target, up);
    }

    void render_terrain_chunk(const gl::chunk_t &chunk, bool set_uniforms) {

        if (chunk.base_z+gl::CHUNK_SIZE < camera_position->region_z-10) return; // Not interested in chunks below the camera
        if (chunk.base_z > camera_position->region_z) return; // Not interested in chunks below the camera

        glBindBuffer(GL_ARRAY_BUFFER, chunk.vbo_id);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, gl::n_floats*sizeof(float), 0);

        if (set_uniforms) {
            glVertexAttribPointer(terrain_world_position_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 3 * sizeof(float)));
            glEnableVertexAttribArray(terrain_world_position_loc);

            glVertexAttribPointer(terrain_normal_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 6 * sizeof(float)));
            glEnableVertexAttribArray(terrain_normal_loc);

            glVertexAttribPointer(terrain_color_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 9 * sizeof(float)));
            glEnableVertexAttribArray(terrain_color_loc);

            glVertexAttribPointer(terrain_texture_position_loc, 2, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 12 * sizeof(float)));
            glEnableVertexAttribArray(terrain_texture_position_loc);

        }

        int cull_pos = chunk.n_quads;
        auto finder = chunk.z_offsets.find(camera_position->region_z);
        if (finder != chunk.z_offsets.end()) {
            cull_pos = finder->second;
            //std::cout << cull_pos << "\n";
        }

        if (cull_pos > 0)
            glDrawArrays(GL_QUADS, 0, cull_pos);

        glDisableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void render_phase_two_terrain() {
        glUseProgram(map_render::terrain_chunk_shader->program_id);
        glBindFramebuffer(GL_FRAMEBUFFER, map_render::mouse_pick_fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        map_render::setup_matrices();

        // Pass along the matrices
        glUniformMatrix4fv(terrain_projection_matrix_loc, 1, false, glm::value_ptr( map_render::camera_projection_matrix ));
        glUniformMatrix4fv(terrain_view_matrix_loc, 1, false, glm::value_ptr( map_render::camera_modelview_matrix ));

        // Pass along the camera information
        glUniform3f(terrain_camera_position_loc, (float)camera_position->region_x, (float)camera_position->region_y, (float)camera_position->region_z);

        // Texture
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        sf::Texture::bind(rltk::get_texture("materials"));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glUniform1i(terrain_my_color_texture_loc, 0);

        Frustrum frustrum;
        frustrum.update(map_render::camera_projection_matrix * map_render::camera_modelview_matrix);

        for (const gl::chunk_t &chunk : gl::chunks) {
            if (chunk.has_geometry && chunk.generated_vbo &&
                    frustrum.checkSphere(glm::vec3(chunk.base_x, chunk.base_z, chunk.base_y), gl::CHUNK_SIZE))
            {
                map_render::render_terrain_chunk(chunk);
            }
        }
        glUseProgram(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }

}
