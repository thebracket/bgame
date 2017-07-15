#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif
#include "populate_gbuffer.hpp"
#include "../../main/game_camera.hpp"
#include "main_fbo.hpp"
#include "sun_moon.hpp"
#include "gl_utils.hpp"
#include "textures/texture.hpp"
#include "chunks/constants.hpp"
#include "models/model_loader.hpp"
#include "../../systems/render/renderables_system.hpp"
#include "shaders/shader_storage.hpp"
#include "shaders/terrain_chunk_shader.hpp"
#include "shaders/static_model_shader.hpp"
#include "shaders/renderable_shader.hpp"
#include "camera.hpp"

namespace map_render {

    void setup_matrices() {
        auto screen_size = rltk::get_window()->getSize();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT);
        glShadeModel(GL_SMOOTH);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glViewport(0,0,screen_size.x,screen_size.y);
        glEnable(GL_POLYGON_SMOOTH);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

        gl::setup_camera();
    }

    void render_bucket(const gl::chunk_t &chunk, const gl::terrain_bucket_t &bucket ) {
        using namespace gl;
        const int offset = std::min(camera_position->region_z - chunk.base_z, gl::CHUNK_SIZE-1);
        if (offset > 0) {

            glBindVertexArrayAPPLE(bucket.vao_id);

            // Rendering

            int cull_pos = bucket.z_offsets[offset];
            if (cull_pos > 0) {
               // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                glDrawArraysInstancedARB(GL_TRIANGLES, 0, 6, cull_pos);
                //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                //std::cout << "Emitted " << cull_pos << " quads for rendering\n";
            }

            glBindVertexArrayAPPLE(0);
        }
    }

    void render_terrain_chunk(const gl::chunk_t &chunk) {

        if (chunk.base_z+gl::CHUNK_SIZE < camera_position->region_z-10) return; // Not interested in chunks below the camera
        if (chunk.base_z > camera_position->region_z) return; // Not interested in chunks below the camera

        for (const auto &bucket : chunk.geometry->buckets) {
            auto tex = textures::get_texture_by_id(bucket.first);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex->texture_id);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tex->normal_id);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, tex->specular_id);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, tex->displacement_id);
            render_bucket(chunk, bucket.second);

        }
        for (const auto &bucket : chunk.vegetation->buckets) {
            if (chunk.vegetation) {
                auto tex = textures::get_texture_by_id(bucket.first);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, tex->texture_id);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, tex->normal_id);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, tex->specular_id);
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, tex->displacement_id);
                render_bucket(chunk, bucket.second);
            }
        }
    }

    void render_terrain_to_gbuffer(gl::model_request_t &models) {
        using namespace gl;
        glUseProgram(terrain_chunk_shader->program_id);
        glBindFramebuffer(GL_FRAMEBUFFER, map_render::mouse_pick_fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        map_render::setup_matrices();

        // Pass along the matrices
        glUniformMatrix4fv(terrain_chunk_shader->projection_matrix_loc, 1, false, glm::value_ptr( gl::camera_projection_matrix ));
        glUniformMatrix4fv(terrain_chunk_shader->view_matrix_loc, 1, false, glm::value_ptr( gl::camera_modelview_matrix ));
        glUniform3f(terrain_chunk_shader->camera_position_loc, camera_position->region_x, camera_position->region_z, camera_position->region_y);

        Frustrum frustrum;
        frustrum.update(gl::camera_projection_matrix * gl::camera_modelview_matrix);

        // Bind the textures bucket
        glUniform1i(terrain_chunk_shader->my_color_texture_loc, 0);
        glUniform1i(terrain_chunk_shader->my_normal_texture_loc, 1);
        glUniform1i(terrain_chunk_shader->my_specular_texture_loc, 2);
        glUniform1i(terrain_chunk_shader->my_displacement_texture_loc, 3);

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
        glUseProgram(0);
    }

    void render_static_models(gl::model_request_t &models) {
        using namespace gl;
        glUseProgram(static_model_shader->program_id);
        //glBindFramebuffer(GL_FRAMEBUFFER, map_render::mouse_pick_fbo);

        // Pass along the matrices
        glUniformMatrix4fv(static_model_shader->projection_matrix_loc, 1, false, glm::value_ptr( gl::camera_projection_matrix ));
        glUniformMatrix4fv(static_model_shader->view_matrix_loc, 1, false, glm::value_ptr( gl::camera_modelview_matrix ));

        // Pass along the camera information
        glUniform3f(static_model_shader->camera_position_loc, (float)camera_position->region_x, (float)camera_position->region_y, (float)camera_position->region_z);

        for (auto it = models.models.begin(); it != models.models.end(); ++it) {
            auto m = gl::get_model(it->first);

            // Setup a texture
            const int texture_id = 9;
            const auto tex = textures::get_texture_by_id(texture_id);
            glActiveTexture(GL_TEXTURE0);
            glEnable(GL_TEXTURE_2D);
            //textures::bind_atlas();

            glUniform1i(static_model_shader->my_color_texture_loc, 0);

            glBindVertexArrayAPPLE(m->vao_id);

            for (const auto &model : it->second) {
                glUniform3f(static_model_shader->world_position_loc, model.x, model.y, model.z);
                glUniform3f(static_model_shader->flags_loc, model.above_ground ? 255.0f : 1.0f, 8.0f, 0.0f);
                glUniform3f(static_model_shader->light_position_loc, model.light_x, model.light_y, model.light_z);
                glUniform3f(static_model_shader->light_color_loc, model.light_r, model.light_g, model.light_b);

                // Render it
                glDrawArrays(GL_TRIANGLES, 0, m->items.size() / gl::num_model_items);
            }

            glBindVertexArrayAPPLE(0);
        }

        glDisableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glUseProgram(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void add_renderables(gl::model_request_t &models) {
        using namespace gl;
        glUseProgram(renderable_shader->program_id);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_TRUE);

        // Pass along the matrices
        glUniformMatrix4fv(renderable_shader->projection_matrix_loc, 1, false, glm::value_ptr( gl::camera_projection_matrix ));
        glUniformMatrix4fv(renderable_shader->view_matrix_loc, 1, false, glm::value_ptr( gl::camera_modelview_matrix ));

        // Pass along the camera information
        glUniform3f(renderable_shader->camera_position_loc, (float)camera_position->region_x, (float)camera_position->region_y, (float)camera_position->region_z);

        Frustrum frustrum;
        frustrum.update(gl::camera_projection_matrix * gl::camera_modelview_matrix);

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
                                    r.x + r.offsetX,
                                    r.y + r.offsetY,
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
                                    r.x + r.offsetX,
                                    r.y + r.offsetY,
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

        //bucket.make_vbo(); // Doing this manually to avoid creating a VAO we'll simply delete
        glGenBuffers(1, &bucket.vbo_id); // Generate the VBO
        glBindBuffer(GL_ARRAY_BUFFER, bucket.vbo_id);

        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * bucket.items.size(), &bucket.items[0], GL_STATIC_DRAW);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, gl::n_floats * sizeof(float), 0);

        glVertexAttribPointer(renderable_shader->world_position_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                              ((char *) nullptr + 3 * sizeof(float)));
        glEnableVertexAttribArray(renderable_shader->world_position_loc);

        glVertexAttribPointer(renderable_shader->normal_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                              ((char *) nullptr + 6 * sizeof(float)));
        glEnableVertexAttribArray(renderable_shader->normal_loc);

        glVertexAttribPointer(renderable_shader->color_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                              ((char *) nullptr + 9 * sizeof(float)));
        glEnableVertexAttribArray(renderable_shader->color_loc);

        glVertexAttribPointer(renderable_shader->texture_position_loc, 2, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                              ((char *) nullptr + 12 * sizeof(float)));
        glEnableVertexAttribArray(renderable_shader->texture_position_loc);

        glVertexAttribPointer(renderable_shader->flags_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                              ((char *) nullptr + 14 * sizeof(float)));
        glEnableVertexAttribArray(renderable_shader->flags_loc);

        glVertexAttribPointer(renderable_shader->light_position_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                              ((char *) nullptr + 17 * sizeof(float)));
        glEnableVertexAttribArray(renderable_shader->light_position_loc);

        glVertexAttribPointer(renderable_shader->light_color_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                              ((char *) nullptr + 20 * sizeof(float)));
        glEnableVertexAttribArray(renderable_shader->light_color_loc);

        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        sf::Texture::bind(rltk::get_texture(term(1)->get_font_tag()));
        glUniform1i(renderable_shader->my_color_texture_loc, 0);
        
        // Render
        glDrawArrays(GL_QUADS, 0, (bucket.items.size()/n_floats));

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
