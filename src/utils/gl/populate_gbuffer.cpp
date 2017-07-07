#include "populate_gbuffer.hpp"
#include "../../main/game_camera.hpp"
#include "main_fbo.hpp"
#include "sun_moon.hpp"
#include "gl_utils.hpp"
#include "textures/texture.hpp"
#include "chunks/constants.hpp"
#include "models/model_loader.hpp"
#include "../../systems/render/renderables_system.hpp"

namespace map_render {
    bool loaded_terrain_shader = false;
    std::unique_ptr<gl::base_shader_t> terrain_chunk_shader;
    std::unique_ptr<gl::base_shader_t> static_model_shader;
    std::unique_ptr<gl::base_shader_t> renderable_shader;
    glm::mat4 camera_projection_matrix;
    glm::mat4 camera_modelview_matrix;

    // Uniform/attribute locations
    GLint terrain_world_position_loc;
    GLint terrain_normal_loc;
    GLint terrain_color_loc;
    GLint terrain_texture_position_loc;
    GLint terrain_projection_matrix_loc;
    GLint terrain_view_matrix_loc;
    GLint terrain_my_color_texture_loc;
    GLint terrain_flags_loc;
    GLint terrain_light_position_loc;
    GLint terrain_light_color_loc;
    GLint terrain_normal_position_loc;
    GLint terrain_camera_position_loc;

    GLint model_world_position_loc;
    GLint model_normal_loc;
    GLint model_texture_position_loc;
    GLint model_projection_matrix_loc;
    GLint model_view_matrix_loc;
    GLint model_camera_position_loc;
    GLint model_my_color_texture_loc;
    GLint model_flags_loc;
    GLint model_light_position_loc;
    GLint model_light_color_loc;

    GLint renderable_world_position_loc;
    GLint renderable_normal_loc;
    GLint renderable_color_loc;
    GLint renderable_texture_position_loc;
    GLint renderable_projection_matrix_loc;
    GLint renderable_view_matrix_loc;
    GLint renderable_camera_position_loc;
    GLint renderable_my_color_texture_loc;
    GLint renderable_flags_loc;
    GLint renderable_light_position_loc;
    GLint renderable_light_color_loc;

    void load_terrain_shader() {
        terrain_chunk_shader = std::make_unique<gl::base_shader_t>("world_defs/shaders/terrain_vertex.glsl",
                                                                   "world_defs/shaders/terrain_fragment.glsl");
        static_model_shader = std::make_unique<gl::base_shader_t>("world_defs/shaders/model_vertex.glsl",
                                                                   "world_defs/shaders/model_fragment.glsl");
        renderable_shader = std::make_unique<gl::base_shader_t>("world_defs/shaders/renderable_vertex.glsl",
                                                                  "world_defs/shaders/renderable_fragment.glsl");
        loaded_terrain_shader = true;

        terrain_world_position_loc = terrain_chunk_shader->get_attribute_location("world_position");
        terrain_normal_loc = terrain_chunk_shader->get_attribute_location("normal");
        terrain_color_loc = terrain_chunk_shader->get_attribute_location("color");
        terrain_texture_position_loc = terrain_chunk_shader->get_attribute_location("texture_position");
        terrain_projection_matrix_loc = terrain_chunk_shader->get_uniform_location("projection_matrix");
        terrain_view_matrix_loc = terrain_chunk_shader->get_uniform_location("view_matrix");
        terrain_my_color_texture_loc = terrain_chunk_shader->get_uniform_location("my_color_texture");
        terrain_flags_loc = terrain_chunk_shader->get_attribute_location("flags");
        terrain_light_position_loc = terrain_chunk_shader->get_attribute_location("light_position");
        terrain_light_color_loc = terrain_chunk_shader->get_attribute_location("light_color");
        terrain_normal_position_loc = terrain_chunk_shader->get_attribute_location("normal_position");
        terrain_camera_position_loc = terrain_chunk_shader->get_uniform_location("camera_position");

        model_world_position_loc = static_model_shader->get_uniform_location("world_position");
        model_normal_loc = static_model_shader->get_attribute_location("normal");
        model_texture_position_loc = static_model_shader->get_attribute_location("texture_position");
        model_projection_matrix_loc = static_model_shader->get_uniform_location("projection_matrix");
        model_view_matrix_loc = static_model_shader->get_uniform_location("view_matrix");
        model_camera_position_loc = static_model_shader->get_uniform_location("camera_position");
        model_my_color_texture_loc = static_model_shader->get_uniform_location("my_color_texture");
        model_flags_loc = static_model_shader->get_uniform_location("flags");
        model_light_position_loc = static_model_shader->get_uniform_location("light_position");
        model_light_color_loc = static_model_shader->get_uniform_location("light_color");

        renderable_world_position_loc = renderable_shader->get_attribute_location("world_position");
        renderable_normal_loc = renderable_shader->get_attribute_location("normal");
        renderable_color_loc = renderable_shader->get_attribute_location("color");
        renderable_texture_position_loc = renderable_shader->get_attribute_location("texture_position");
        renderable_projection_matrix_loc = renderable_shader->get_uniform_location("projection_matrix");
        renderable_view_matrix_loc = renderable_shader->get_uniform_location("view_matrix");
        renderable_camera_position_loc = renderable_shader->get_uniform_location("camera_position");
        renderable_my_color_texture_loc = renderable_shader->get_uniform_location("my_color_texture");
        renderable_flags_loc = renderable_shader->get_attribute_location("flags");
        renderable_light_position_loc = renderable_shader->get_attribute_location("light_position");
        renderable_light_color_loc = renderable_shader->get_attribute_location("light_color");
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
    }

    void render_bucket(const gl::chunk_t &chunk, const gl::terrain_bucket_t &bucket ) {
        const int offset = std::min(camera_position->region_z - chunk.base_z, gl::CHUNK_SIZE-1);
        if (offset > 0) {

            // Bind the texture VBO and map the attributes
            glBindBuffer(GL_ARRAY_BUFFER, bucket.vbo_id);

            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, gl::n_floats * sizeof(float), 0);

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

            glVertexAttribPointer(terrain_flags_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 14 * sizeof(float)));
            glEnableVertexAttribArray(terrain_flags_loc);

            glVertexAttribPointer(terrain_light_position_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 17 * sizeof(float)));
            glEnableVertexAttribArray(terrain_light_position_loc);

            glVertexAttribPointer(terrain_light_color_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 20 * sizeof(float)));
            glEnableVertexAttribArray(terrain_light_color_loc);

            glVertexAttribPointer(terrain_normal_position_loc, 2, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 23 * sizeof(float)));
            glEnableVertexAttribArray(terrain_normal_position_loc);

            // Rendering

            int cull_pos = bucket.z_offsets[offset];
            if (cull_pos > 0) {
                //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                glDrawArrays(GL_QUADS, 0, cull_pos);
                //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                //std::cout << "Emitted " << cull_pos << " quads for rendering\n";
            }
        }
    }

    void render_terrain_chunk(const gl::chunk_t &chunk) {

        if (chunk.base_z+gl::CHUNK_SIZE < camera_position->region_z-10) return; // Not interested in chunks below the camera
        if (chunk.base_z > camera_position->region_z) return; // Not interested in chunks below the camera

        render_bucket(chunk, chunk.geometry->bucket);
        if (chunk.vegetation) {
            render_bucket(chunk, chunk.vegetation->bucket);
        }
    }

    void render_terrain_to_gbuffer(gl::model_request_t &models) {
        glUseProgram(map_render::terrain_chunk_shader->program_id);
        glBindFramebuffer(GL_FRAMEBUFFER, map_render::mouse_pick_fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        map_render::setup_matrices();

        // Pass along the matrices
        glUniformMatrix4fv(terrain_projection_matrix_loc, 1, false, glm::value_ptr( map_render::camera_projection_matrix ));
        glUniformMatrix4fv(terrain_view_matrix_loc, 1, false, glm::value_ptr( map_render::camera_modelview_matrix ));
        glUniform3f(terrain_camera_position_loc, camera_position->region_x, camera_position->region_z, camera_position->region_y);

        Frustrum frustrum;
        frustrum.update(map_render::camera_projection_matrix * map_render::camera_modelview_matrix);

        // Bind the textures bucket
        glActiveTexture(GL_TEXTURE0);
        textures::bind_atlas();
        glUniform1i(terrain_my_color_texture_loc, 0);

        for (const gl::chunk_t &chunk : gl::chunks) {
            if (chunk.has_geometry &&
                    frustrum.checkSphere(glm::vec3(chunk.base_x, chunk.base_z, chunk.base_y), gl::CHUNK_SIZE))
            {
                map_render::render_terrain_chunk(chunk);
                // Copy the chunk's models over to the render queue
                for (const auto &m : chunk.static_models) {
                    if (m.z <= camera_position->region_z) models.add_model_request(m.model_id, m);
                }
            }
        }
        glDisableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(terrain_world_position_loc);
        glDisableVertexAttribArray(terrain_normal_loc);
        glDisableVertexAttribArray(terrain_texture_position_loc);
        glDisableVertexAttribArray(terrain_flags_loc);
        glDisableVertexAttribArray(terrain_light_position_loc);
        glDisableVertexAttribArray(terrain_light_color_loc);
        glDisableVertexAttribArray(terrain_normal_position_loc);
        glUseProgram(0);
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
    }

    void render_static_models(gl::model_request_t &models) {
        glUseProgram(map_render::static_model_shader->program_id);
        //glBindFramebuffer(GL_FRAMEBUFFER, map_render::mouse_pick_fbo);

        // Pass along the matrices
        glUniformMatrix4fv(model_projection_matrix_loc, 1, false, glm::value_ptr( map_render::camera_projection_matrix ));
        glUniformMatrix4fv(model_view_matrix_loc, 1, false, glm::value_ptr( map_render::camera_modelview_matrix ));

        // Pass along the camera information
        glUniform3f(model_camera_position_loc, (float)camera_position->region_x, (float)camera_position->region_y, (float)camera_position->region_z);

        for (auto it = models.models.begin(); it != models.models.end(); ++it) {
            auto m = gl::get_model(it->first);

            // Setup a texture
            const int texture_id = 9;
            const auto tex = textures::get_texture_by_id(texture_id);
            glActiveTexture(GL_TEXTURE0);
            glEnable(GL_TEXTURE_2D);
            textures::bind_atlas();

            glUniform1i(model_my_color_texture_loc, 0);

            glBindBuffer(GL_ARRAY_BUFFER, m->vbo_id);

            // Bind the vertex buffer
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, gl::num_model_items * sizeof(float), 0);

            glVertexAttribPointer(model_normal_loc, 3, GL_FLOAT, GL_FALSE, gl::num_model_items * sizeof(float),
                                  ((char *) nullptr + 3 * sizeof(float)));
            glEnableVertexAttribArray(model_normal_loc);

            glVertexAttribPointer(model_texture_position_loc, 2, GL_FLOAT, GL_FALSE,
                                  gl::num_model_items * sizeof(float),
                                  ((char *) nullptr + 6 * sizeof(float)));
            glEnableVertexAttribArray(model_texture_position_loc);

            for (const auto &model : it->second) {
                glUniform3f(model_world_position_loc, model.x, model.y, model.z);
                glUniform3f(model_flags_loc, model.above_ground ? 255.0f : 1.0f, 8.0f, 0.0f);
                glUniform3f(model_light_position_loc, model.light_x, model.light_y, model.light_z);
                glUniform3f(model_light_color_loc, model.light_r, model.light_g, model.light_b);

                // Render it
                glDrawArrays(GL_TRIANGLES, 0, m->items.size() / gl::num_model_items);
            }
        }

        glDisableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glUseProgram(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void add_renderables(gl::model_request_t &models) {
        glUseProgram(map_render::renderable_shader->program_id);
        //glBindFramebuffer(GL_FRAMEBUFFER, map_render::mouse_pick_fbo);
        //map_render::setup_matrices();

        // Pass along the matrices
        glUniformMatrix4fv(renderable_projection_matrix_loc, 1, false, glm::value_ptr( map_render::camera_projection_matrix ));
        glUniformMatrix4fv(renderable_view_matrix_loc, 1, false, glm::value_ptr( map_render::camera_modelview_matrix ));

        // Pass along the camera information
        glUniform3f(renderable_camera_position_loc, (float)camera_position->region_x, (float)camera_position->region_y, (float)camera_position->region_z);

        Frustrum frustrum;
        frustrum.update(map_render::camera_projection_matrix * map_render::camera_modelview_matrix);

        gl::terrain_bucket_t bucket;

        for (auto it = renderables.begin(); it != renderables.end(); ++it) {
            int x,y,z;
            std::tie(x,y,z) = idxmap(it->first);

            if (z <= camera_position->region_z && frustrum.checkSphere(glm::vec3((float)x, (float)z, (float)y), 16.0f)) {
                float light_r, light_g, light_b, light_x, light_y, light_z;
                gl::set_light(it->first, light_r, light_g, light_b, light_x, light_y, light_z);
                for (const auto &r : it->second) {
                    float tx = ((r.c.glyph % 16) * 24.0f) / 384.0f;
                    float ty = ((r.c.glyph / 16) * 24.0f) / 768.0f;
                    bucket.add_renderable(
                        r.x,
                        r.y,
                        (float)z,
                        (float)r.c.foreground.r / 255.0f,
                        (float)r.c.foreground.g / 255.0f,
                        (float)r.c.foreground.b / 255.0f,
                        it->first,
                        region::above_ground(it->first),
                        light_r, light_g, light_b, light_x, light_y, light_z, 8.0f, tx, ty
                    );
                }
            }
        }

        for (auto it = composite_renderables.begin(); it != composite_renderables.end(); ++it) {
            int x,y,z;
            std::tie(x,y,z) = idxmap(it->first);

            if (z <= camera_position->region_z && frustrum.checkSphere(glm::vec3((float)x, (float)z, (float)y), 16.0f)) {
                for (const auto &rl : it->second) {
                    float light_r, light_g, light_b, light_x, light_y, light_z;
                    gl::set_light(it->first, light_r, light_g, light_b, light_x, light_y, light_z);

                    for (const auto &r : rl) {
                        float tx = ((r.c.glyph % 16) * 24.0f) / 384.0f;
                        float ty = ((r.c.glyph / 16) * 24.0f) / 768.0f;

                        if (camera->camera_mode == TOP_DOWN) {
                            bucket.add_renderable(
                                    r.x,
                                    r.y,
                                    (float) z,
                                    (float) r.c.foreground.r / 255.0f,
                                    (float) r.c.foreground.g / 255.0f,
                                    (float) r.c.foreground.b / 255.0f,
                                    it->first,
                                    region::above_ground(it->first),
                                    light_r, light_g, light_b, light_x, light_y, light_z, 8.0f, tx, ty
                            );
                        } else {
                            bucket.add_billboard(
                                    r.x,
                                    r.y,
                                    (float) z,
                                    (float) r.c.foreground.r / 255.0f,
                                    (float) r.c.foreground.g / 255.0f,
                                    (float) r.c.foreground.b / 255.0f,
                                    it->first,
                                    region::above_ground(it->first),
                                    light_r, light_g, light_b, light_x, light_y, light_z, 8.0f, tx, ty
                            );
                        }
                    }
                }
            }
        }
        //std::cout << i << " composite renderables to enqueue\n";

        bucket.make_vbo();

        // Bind attributes and texture
        glBindBuffer(GL_ARRAY_BUFFER, bucket.vbo_id);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, gl::n_floats * sizeof(float), 0);

        glVertexAttribPointer(renderable_world_position_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                              ((char *) nullptr + 3 * sizeof(float)));
        glEnableVertexAttribArray(renderable_world_position_loc);

        glVertexAttribPointer(renderable_normal_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                              ((char *) nullptr + 6 * sizeof(float)));
        glEnableVertexAttribArray(renderable_normal_loc);

        glVertexAttribPointer(renderable_color_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                              ((char *) nullptr + 9 * sizeof(float)));
        glEnableVertexAttribArray(renderable_color_loc);

        glVertexAttribPointer(renderable_texture_position_loc, 2, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                              ((char *) nullptr + 12 * sizeof(float)));
        glEnableVertexAttribArray(renderable_texture_position_loc);

        glVertexAttribPointer(renderable_flags_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                              ((char *) nullptr + 14 * sizeof(float)));
        glEnableVertexAttribArray(renderable_flags_loc);

        glVertexAttribPointer(renderable_light_position_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                              ((char *) nullptr + 17 * sizeof(float)));
        glEnableVertexAttribArray(renderable_light_position_loc);

        glVertexAttribPointer(renderable_light_color_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                              ((char *) nullptr + 20 * sizeof(float)));
        glEnableVertexAttribArray(renderable_light_color_loc);

        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        sf::Texture::bind(rltk::get_texture(term(1)->get_font_tag()));
        glUniform1i(renderable_my_color_texture_loc, 0);
        
        // Render
        glDrawArrays(GL_QUADS, 0, bucket.n_quads);

        // Cleanup
        glDeleteBuffers(1, &bucket.vbo_id);

        glDisableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        //glUseProgram(0);
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (const auto &loc : renderable_models) {
            for (const auto &m : loc.second) {
                models.add_model_request(m.model_id, m);
            }
        }
    }

}
