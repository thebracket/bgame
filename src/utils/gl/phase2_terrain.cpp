#include "phase2_terrain.hpp"
#include "../../main/game_camera.hpp"
#include "main_fbo.hpp"
#include "sun_fbo.hpp"
#include "phase1_sunmoon.hpp"

namespace map_render {
    bool loaded_terrain_shader = false;
    std::unique_ptr<gl::base_shader_t> terrain_chunk_shader;
    glm::mat4 camera_projection_matrix;
    glm::mat4 camera_modelview_matrix;


    void load_terrain_shader() {
        terrain_chunk_shader = std::make_unique<gl::base_shader_t>("world_defs/shaders/terrain_vertex.glsl",
                                                                   "world_defs/shaders/terrain_fragment.glsl");
        loaded_terrain_shader = true;
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
        if (!chunk.has_geometry) return;
        if (!chunk.generated_vbo) return;

        if (chunk.base_z+gl::CHUNK_SIZE < camera_position->region_z-10) return; // Not interested in chunks below the camera
        if (chunk.base_z > camera_position->region_z) return; // Not interested in chunks below the camera

        GLint world_position;
        GLint normal_position;
        GLint color_position;
        GLint texture_position;

        if (set_uniforms) {
            world_position = glGetAttribLocation(terrain_chunk_shader->program_id, "world_position");
            if (world_position == -1) throw std::runtime_error("Invalid world position in shader");
            normal_position = glGetAttribLocation(terrain_chunk_shader->program_id, "normal");
            if (normal_position == -1) throw std::runtime_error("Invalid normal position in shader");
            color_position = glGetAttribLocation(terrain_chunk_shader->program_id, "color");
            if (color_position == -1) throw std::runtime_error("Invalid color position in shader");
            texture_position = glGetAttribLocation(terrain_chunk_shader->program_id, "texture_position");
            if (texture_position == -1) throw std::runtime_error("Invalid texture position in shader");
        }

        glBindBuffer(GL_ARRAY_BUFFER, chunk.vbo_id);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, gl::n_floats*sizeof(float), 0);

        if (set_uniforms) {
            glVertexAttribPointer(world_position, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 3 * sizeof(float)));
            glEnableVertexAttribArray(world_position);

            glVertexAttribPointer(normal_position, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 6 * sizeof(float)));
            glEnableVertexAttribArray(normal_position);

            glVertexAttribPointer(color_position, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 9 * sizeof(float)));
            glEnableVertexAttribArray(color_position);

            glVertexAttribPointer(texture_position, 2, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 12 * sizeof(float)));
            glEnableVertexAttribArray(texture_position);

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
        int projection_matrix_loc = glGetUniformLocation(map_render::terrain_chunk_shader->program_id, "projection_matrix");
        if (projection_matrix_loc == -1) throw std::runtime_error("Unknown uniform slot - projection matrix");
        glUniformMatrix4fv(projection_matrix_loc, 1, false, glm::value_ptr( map_render::camera_projection_matrix ));

        int view_matrix_loc = glGetUniformLocation(map_render::terrain_chunk_shader->program_id, "view_matrix");
        if (view_matrix_loc == -1) throw std::runtime_error("Unknown uniform slot - view matrix");
        glUniformMatrix4fv(view_matrix_loc, 1, false, glm::value_ptr( map_render::camera_modelview_matrix ));

        int light_matrix_loc = glGetUniformLocation(map_render::terrain_chunk_shader->program_id, "light_space_matrix");
        if (light_matrix_loc == -1) throw std::runtime_error("Unknown uniform slot - light space matrix");
        glm::mat4 light_matrix = map_render::sun_projection_matrix * map_render::sun_modelview_matrix;
        glUniformMatrix4fv(light_matrix_loc, 1, false, glm::value_ptr( light_matrix ));

        // Pass along the camera information
        int camera_pos = glGetUniformLocation(map_render::terrain_chunk_shader->program_id, "camera_position");
        if (camera_pos == -1) throw std::runtime_error("Unknown uniform slot - camera_pos");
        glUniform3f(camera_pos, (float)camera_position->region_x, (float)camera_position->region_y, (float)camera_position->region_z);

        // Texture
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        sf::Texture::bind(rltk::get_texture("materials"));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, map_render::sun_depth_texture);

        int tex1loc = glGetUniformLocation(map_render::terrain_chunk_shader->program_id, "my_color_texture");
        if (tex1loc == -1) throw std::runtime_error("Unknown uniform slot - texture 0");
        glUniform1i(tex1loc, 0);

        int tex2loc = glGetUniformLocation(map_render::terrain_chunk_shader->program_id, "shadow_map");
        if (tex2loc == -1) throw std::runtime_error("Unknown uniform slot - texture 1");
        glUniform1i(tex2loc, 1);

        for (const gl::chunk_t &chunk : gl::chunks) {
            map_render::render_terrain_chunk(chunk);
        }
        glUseProgram(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }

}
