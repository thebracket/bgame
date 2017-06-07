#include "map_render.hpp"
#include "gl_utils.hpp"
#include <rltk.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <iostream>
#include "gl_utils.hpp"
#include "../../systems/render/lighting_system.hpp"
#include "../../systems/render/renderables_system.hpp"
#include "../../systems/render/map_render_system.hpp"
#include "world_scene.hpp"
#include "frustrum.hpp"
#include "../../systems/input/mouse_input_system.hpp"
#include "render_block.hpp"
#include "../../messages/messages.hpp"
#include "../../systems/gui/mode_design_system.hpp"
#include "../../main/game_clipping.hpp"
#include "../../planet/region/region.hpp"
#include "../../main/game_designations.hpp"
#include "../../main/game_camera.hpp"
#include "../../main/game_mode.hpp"
#include "../../main/game_selections.hpp"
#include "chunk.hpp"
#include "base_shader.hpp"
#define GLM_COMPILER 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace map_render_sys;
using namespace region;

bool world_changed = true;

namespace map_render
{
    bool loaded_terrain_shader = false;
    bool loaded_shadow_shader = false;
    bool loaded_render_shader = false;
    bool loaded_fbo = false;
    bool loaded_sun_fbo = false;
    bool built_chunk_buffer = false;
    std::unique_ptr<gl::base_shader_t> terrain_chunk_shader;
    std::unique_ptr<gl::base_shader_t> shadow_shader;
    std::unique_ptr<gl::base_shader_t> render_shader;

    void load_terrain_shader() {
        terrain_chunk_shader = std::make_unique<gl::base_shader_t>("world_defs/shaders/terrain_vertex.glsl",
                                                                   "world_defs/shaders/terrain_fragment.glsl");
        loaded_terrain_shader = true;
    }

    void load_shadow_shader() {
        shadow_shader = std::make_unique<gl::base_shader_t>("world_defs/shaders/shadow_vertex.glsl",
                                                                   "world_defs/shaders/shadow_fragment.glsl");
        loaded_shadow_shader = true;
    }

    void load_render_shader() {
        render_shader = std::make_unique<gl::base_shader_t>("world_defs/shaders/render_vertex.glsl",
                                                            "world_defs/shaders/render_fragment.glsl");
        loaded_render_shader = true;
    }

    GLuint mouse_pick_texture;
    GLuint mouse_pick_fbo;
    GLuint mouse_pick_depth;
    GLuint render_texture;
    GLuint normal_texture;

    // Shadow-based lighting
    GLuint sun_fbo;
    GLuint sun_depth_buffer;
    GLuint sun_depth_texture;
    GLuint sun_render;

    void load_sun_fbo() {
        const auto screen_size = rltk::get_window()->getSize();
        glGenFramebuffers(1, &sun_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, sun_fbo);
        std::cout << "Sun FBO: " << sun_fbo << "\n";

        // Depth buffer
        glGenRenderbuffers(1, &sun_depth_buffer);
        std::cout << "Sun depth buffer: " << sun_depth_buffer << "\n";
        glBindRenderbuffer(GL_RENDERBUFFER, sun_depth_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_size.x, screen_size.y);
        glBindFramebuffer(GL_FRAMEBUFFER, sun_fbo);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, sun_depth_buffer);

        // Depth texture
        glGenTextures(1, &sun_depth_texture);
        glBindTexture(GL_TEXTURE_2D, sun_depth_texture);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screen_size.x, screen_size.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
        std::cout << "Sun depth texture: " << sun_depth_texture << "\n";
        glBindFramebuffer(GL_FRAMEBUFFER, sun_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sun_depth_texture, 0);

        // Render texture
        glGenTextures(1, &sun_render);
        glBindTexture(GL_TEXTURE_2D, sun_render);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_size.x, screen_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, sun_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sun_render, 0);

        GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT  };
        glDrawBuffers(1, buffers);

        // Return to regular render mode
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        loaded_sun_fbo = true;
        std::cout << "Setup Shadow/Sun FBO\n";
    }

    void load_fbo() {
        // Create and bind the framebuffer for mouse-picking output
        glGenFramebuffers(1, &mouse_pick_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);

        // Create the mouse picker render target texture
        const auto screen_size = rltk::get_window()->getSize();
        glGenTextures(1, &mouse_pick_texture);
        glBindTexture(GL_TEXTURE_2D, mouse_pick_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_size.x, screen_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mouse_pick_texture, 0);

        // Create the render target texture
        glGenTextures(1, &render_texture);
        glBindTexture(GL_TEXTURE_2D, render_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_size.x, screen_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, render_texture, 0);

        // Create the normal target texture
        glGenTextures(1, &normal_texture);
        glBindTexture(GL_TEXTURE_2D, normal_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_size.x, screen_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, normal_texture, 0);

        // Create a depth-buffer for the render target
        glGenRenderbuffers(1, &mouse_pick_depth);
        glBindRenderbuffer(GL_RENDERBUFFER, mouse_pick_depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_size.x, screen_size.y);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mouse_pick_depth);

        GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT };
        glDrawBuffers(3, buffers);

        // Return to regular render mode
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        loaded_fbo = true;
    }

    void build_chunk_buffer() {
        std::cout << "Building chunk buffer\n";
        gl::build_chunk_buffer();
        built_chunk_buffer = true;
    }

    std::tuple<int,int,int> readback_texture_pixel(const int &x, const int &y) {
        return std::make_tuple(0,0,0);
    };

    glm::mat4 camera_projection_matrix;
    glm::mat4 camera_modelview_matrix;

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
        glm::vec3 target{(float) camera_position->region_x, (float) camera_position->region_z, (float) camera_position->region_y};
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

    void render_terrain_chunk(const gl::chunk_t &chunk, bool set_uniforms = true) {
        if (!chunk.has_geometry) return;
        if (!chunk.generated_vbo) return;

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

        glDrawArrays(GL_QUADS, 0, chunk.n_quads);

        glDisableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void render_sun_chunk(const gl::chunk_t &chunk, bool set_uniforms = true) {
        if (!chunk.has_geometry) return;
        if (!chunk.generated_vbo) return;

        GLint world_position;

        if (set_uniforms) {
            world_position = glGetAttribLocation(shadow_shader->program_id, "world_position");
            if (world_position == -1) throw std::runtime_error("Invalid world position in shader");
        }

        glBindBuffer(GL_ARRAY_BUFFER, chunk.vbo_id);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, gl::n_floats*sizeof(float), 0);

        if (set_uniforms) {
            glVertexAttribPointer(world_position, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 3 * sizeof(float)));
            glEnableVertexAttribArray(world_position);
        }

        glDrawArrays(GL_QUADS, 0, chunk.n_quads);

        glDisableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void render_test_texture(float left, float top, float right, float bottom, GLuint &target_texture) {
    glEnableClientState(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, target_texture);

    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);

    glTexCoord2f(0, 1);
    glVertex2f(left, bottom);

    glTexCoord2f(0, 0);
    glVertex2f(left, top);

    glTexCoord2f(1, 0);
    glVertex2f(right, top);

    glTexCoord2f(1, 1);
    glVertex2f(right, bottom);

    glEnd();
}

void render_mixed_texture(float left, float top, float right, float bottom) {
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);

    glTexCoord2f(0, 1);
    glVertex2f(left, bottom);

    glTexCoord2f(0, 0);
    glVertex2f(left, top);

    glTexCoord2f(1, 0);
    glVertex2f(right, top);

    glTexCoord2f(1, 1);
    glVertex2f(right, bottom);

    glEnd();
}

void map_render_t::render() {
    // Check that the environment is ready
    if (!map_render::loaded_terrain_shader) map_render::load_terrain_shader();
    if (!map_render::loaded_shadow_shader) map_render::load_shadow_shader();
    if (!map_render::loaded_render_shader) map_render::load_render_shader();
    if (!map_render::loaded_fbo) map_render::load_fbo();
    if (!map_render::loaded_sun_fbo) map_render::load_sun_fbo();
    if (!map_render::built_chunk_buffer) map_render::build_chunk_buffer();

    // Update any chunks of the world that are dirty
    gl::update_dirty_chunks();

    // Push state
    push_gl_states();

    // TODO: Phase 0 render - draw to the sun!
    // Use sun program
    glUseProgram(map_render::shadow_shader->program_id);

    // Use sun framebuffer and clear
    glBindFramebuffer(GL_FRAMEBUFFER, map_render::sun_fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // setup matrices for sun rendering
    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

    // We move the near plane just a bit to make the depth texture a bit more visible.
    // It also increases the precision.
    auto screen_size = rltk::get_window()->getSize();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glViewport(0,0,screen_size.x,screen_size.y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 300.0f);

    // Set the light position
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt((float) camera_position->region_x + (float)camera->zoom_level, ((float) camera_position->region_z) + (float)camera->zoom_level,
              ((float) camera_position->region_y) + (float)camera->zoom_level, // Camera
              (float) camera_position->region_x, (float) camera_position->region_z,
              (float) camera_position->region_y, // Target
              0.0f, 1.0f, 0.0f // Up
    );
    glFrontFace(GL_CW);

    int sun_projection = glGetUniformLocation(map_render::shadow_shader->program_id, "projection_matrix");
    if (sun_projection == -1) throw std::runtime_error("Unknown uniform slot - projection matrix");
    float sproj[16];
    glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&sproj);
    glUniformMatrix4fv(sun_projection, 1, false, (GLfloat*)&sproj);

    int sun_view = glGetUniformLocation(map_render::shadow_shader->program_id, "view_matrix");
    if (sun_view == -1) throw std::runtime_error("Unknown uniform slot - view matrix");
    float sview[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&sview);
    glUniformMatrix4fv(sun_view, 1, false, (GLfloat*)&sview);

    // render the terrain
    for (const gl::chunk_t &chunk : gl::chunks) {
        map_render::render_sun_chunk(chunk);
    }

    // save matrices
    float worldToLightViewMatrix[16];
    float lightViewToProjectionMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, worldToLightViewMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, lightViewToProjectionMatrix);

    // Re-set the projection to the default one we have pushed on the stack
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // Set the camera position
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(20, 1, 0, 0);
    glTranslatef(0.0f,-6.5f,-11.0f);
    glFrontFace(GL_CCW);

    float worldToCameraViewMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, worldToCameraViewMatrix);

    // clean up
    glUseProgram(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: Phase 1 render: draw terrain chunks
    glUseProgram(map_render::terrain_chunk_shader->program_id);
    glBindFramebuffer(GL_FRAMEBUFFER, map_render::mouse_pick_fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    map_render::setup_matrices();
    gl_frustrum::extract();

    // Pass along the matrices
    int projection_matrix_loc = glGetUniformLocation(map_render::terrain_chunk_shader->program_id, "projection_matrix");
    if (projection_matrix_loc == -1) throw std::runtime_error("Unknown uniform slot - projection matrix");
    glUniformMatrix4fv(projection_matrix_loc, 1, false, glm::value_ptr( map_render::camera_projection_matrix ));

    int view_matrix_loc = glGetUniformLocation(map_render::terrain_chunk_shader->program_id, "view_matrix");
    if (view_matrix_loc == -1) throw std::runtime_error("Unknown uniform slot - view matrix");
    glUniformMatrix4fv(view_matrix_loc, 1, false, glm::value_ptr( map_render::camera_modelview_matrix ));

    // Pass along the camera information
    int camera_pos = glGetUniformLocation(map_render::terrain_chunk_shader->program_id, "camera_position");
    if (camera_pos == -1) throw std::runtime_error("Unknown uniform slot - camera_pos");
    glUniform3f(camera_pos, (float)camera_position->region_x, (float)camera_position->region_y, (float)camera_position->region_z);

    // Texture
    glEnable(GL_TEXTURE_2D);
    sf::Texture::bind(rltk::get_texture(term(1)->get_font_tag()));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (const gl::chunk_t &chunk : gl::chunks) {
        map_render::render_terrain_chunk(chunk);
    }
    glUseProgram(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Splat render
    // Render the index buffer
    auto sz = rltk::get_window()->getSize();
    const float W = (float)sz.x;
    const float H = (float)sz.y;
    push_gl_states();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, sz.x, sz.y);
    glOrtho(0, sz.x, 0, sz.y, 0.0f, 1.0f);
    render_test_texture(0.0f, 0.0f, W/2.0f, H/2.0f, map_render::mouse_pick_texture);
    render_test_texture(W/2.0f, 0.0f, W, H/2.0f, map_render::render_texture);
    render_test_texture(0.0f, H/2.0f, W/2.0f, H, map_render::normal_texture);
    render_test_texture(W/2.0f, H/2.0f, W, H, map_render::sun_depth_texture);

    /*
    // Render out the finished screen
    glUseProgram(map_render::render_shader->program_id);

    // Bind the uniforms
    //int r_worldToLightViewMatrix = glGetUniformLocation(map_render::render_shader->program_id, "worldToLightViewMatrix");
    //glUniformMatrix4fv(r_worldToLightViewMatrix, 1, false, (GLfloat*)&worldToLightViewMatrix);
    //int r_lightViewToProjectionMatrix = glGetUniformLocation(map_render::render_shader->program_id, "lightViewToProjectionMatrix");
    //glUniformMatrix4fv(r_lightViewToProjectionMatrix, 1, false, (GLfloat*)&lightViewToProjectionMatrix);
    //int r_worldToCameraViewMatrix = glGetUniformLocation(map_render::render_shader->program_id, "worldToCameraViewMatrix");
    glUniformMatrix4fv(r_worldToCameraViewMatrix, 1, false, (GLfloat*)&view);
    int r_camera = glGetUniformLocation(map_render::render_shader->program_id, "cameraPosition");
    glUniform3f(r_camera, camera_position->region_x, camera_position->region_z, camera_position->region_y);

    // Bind all the info textures. We send the normal map, position map,
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, map_render::render_texture); // Texture slot 0 = albedo
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, map_render::position_texture); // Texture slot 1 = position
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, map_render::normal_texture); // Texture slot 2 = normal
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, map_render::sun_depth_texture); // Texture slot 3 = normal
    glUniform1i(glGetUniformLocation(map_render::render_shader->program_id, "albedo_tex"), 0);
    glUniform1i(glGetUniformLocation(map_render::render_shader->program_id, "position_tex"), 1);
    glUniform1i(glGetUniformLocation(map_render::render_shader->program_id, "normal_tex"), 2);
    glUniform1i(glGetUniformLocation(map_render::render_shader->program_id, "shadow_map"), 3);

    // Render out the quad
    render_mixed_texture(0.0f, 0.0f, W, H);
    */

    // Done
    glUseProgram(0);

    pop_gl_states();

    // Restore state
    pop_gl_states();
}


    /*
    bool loaded_program = false;
    GLuint deferred_id;
    GLuint mouse_pick_texture;
    GLuint mouse_pick_fbo;
    GLuint mouse_pick_depth;
    GLuint render_texture;

    inline bool has_floor(uint8_t tiletype) {
        if (tiletype != tile_type::OPEN_SPACE) {
            return true;
        }
        return false;
    }

    inline bool has_ceiling(uint8_t tiletype) {
        if (tiletype == tile_type::TREE_TRUNK || tiletype == tile_type::WALL || tiletype == tile_type::SEMI_MOLTEN_ROCK
            || tiletype == tile_type::SOLID || tiletype == tile_type::TREE_LEAF || tiletype == tile_type::WINDOW ||
                tiletype == tile_type::CLOSED_DOOR) {
            return true;
        }
        return false;
    }

    inline std::tuple<int, int, int, int> calculate_edges() noexcept {
        const int z2 = camera->zoom_level * 2;
        return std::make_tuple(
                std::max(1, camera_position->region_x - z2),
                std::min(REGION_WIDTH-1, camera_position->region_x + z2),
                std::max(1, camera_position->region_y - z2),
                std::min(REGION_HEIGHT-1, camera_position->region_y + z2)
        );
    }

    template <typename F>
    inline void render_area_loop(const int &start_z, const int &end_z, const std::tuple<int, int, int, int> &edges, const F &&func) {
        int start_x, end_x, start_y, end_y;
        std::tie(start_x, end_x, start_y, end_y) = calculate_edges();

        for (int z=start_z; z<end_z; ++z) {
            for (int y = start_y; y < end_y; ++y) {
                for (int x = start_x; x < end_x; ++x) {
                    if (gl_frustrum::point_in_frustrum(x, z, y)) {
                        const int idx = mapidx(x, y, z);
                        func(x, y, z, idx, revealed(idx), region::tile_type(idx));
                    }
                }
            }
        }
    }

    const vchar bloodstain{ 340, color_t{255,255,255}, color_t{255,255,255} };

    void populate_tile_render(const int &x, const int &y, const int &z, const int &idx, const bool &revealed, const uint8_t &tiletype) {
        if (revealed) {
            if (tiletype != tile_type::OPEN_SPACE) {
                if (tiletype == tile_type::RAMP) {
                    world_scene::add_world_fractional_cube(x, y, z, render_cache(idx), idx, 0.5f);
                } else if (has_ceiling(tiletype)) {
                    if (game_master_mode == DESIGN && game_design_mode == CHOPPING && tree_id(idx)>0
                        && designations->chopping.find(tree_id(idx))!=designations->chopping.end()) {
                        world_scene::add_world_cube(x, y, z, vchar{'*', colors::RED, colors::RED}, idx);
                    } else {
                        if (tiletype == tile_type::TREE_TRUNK) {
                            world_scene::add_composite_renderable(x, y, z, render_cache(idx), idx);
                        } else {
                            world_scene::add_world_cube(x, y, z, render_cache(idx), idx);
                        }
                    }
                } else if (has_floor(tiletype)) {
                    world_scene::add_world_floor(x, y, z, render_cache(idx), idx);
                }

                // Add water
                const float water_depth = (float)water_level(idx) / 10.0f;
                if (water_depth>0) {
                    world_scene::add_world_fractional_cube(x, y, z, render_cache(idx), idx, water_depth);
                }

                // Add vegetation
                auto vc = veg_cache(idx);
                if (vc.glyph > 0) {
                    world_scene::add_vegetation(x, y, z, vc, idx);
                }

                // Add blood stains
                if (blood_stain(idx)) {
                    world_scene::add_decal(x, y, z, bloodstain, idx);
                }

                // Add renderables
                auto rfinder = renderables.find(idx);
                if (rfinder != renderables.end()) {
                    screen_render_t &sr = rfinder->second[glyph_cycle % rfinder->second.size()];
                    world_scene::add_simple_renderable(x, y, z, sr.c, idx);
                }

                // Add composite renderables
                auto cfinder = composite_renderables.find(idx);
                if (cfinder != composite_renderables.end()) {
                    for (const auto &sr : cfinder->second[glyph_cycle % cfinder->second.size()]) {
                        world_scene::add_composite_renderable(x, y, z, sr.c, idx);
                    }
                }

                // Stockpiles
                if (game_master_mode == DESIGN && game_design_mode == STOCKPILES && current_stockpile>0 && stockpile_id(idx)==current_stockpile) {
                    world_scene::add_decal(x, y, z, vchar{'#', colors::MAGENTA, colors::MAGENTA}, idx);
                }

                // Mining
                if (game_master_mode == DESIGN && game_design_mode == DIGGING) {
                    auto mf = designations->mining.find(idx);
                    if (mf != designations->mining.end()) {
                        vchar result{'!', colors::YELLOW, colors::YELLOW};
                        switch (mf->second) {
                            case 1 : result.glyph = 177; break;
                            case 2 : result.glyph = 31; break;
                            case 3 : result.glyph = 30; break;
                            case 4 : result.glyph = '<'; break;
                            case 5 : result.glyph = '>'; break;
                            case 6 : result.glyph = 'X'; break;
                        }

                        if (mf->second != 5) {
                            world_scene::add_world_cube(x, y, z, result, idx);
                        } else {
                            world_scene::add_decal(x, y, z, result, idx);
                        }
                    }
                }

                // Buildings
                if (game_master_mode == DESIGN && game_design_mode == BUILDING && has_build_mode_building) {
                    const int building_left_x = mouse::mouse_world_x;
                    const int building_top_y = mouse::mouse_world_y;
                    const int building_right_x = mouse::mouse_world_x + build_mode_building.width;
                    const int building_bottom_y = mouse::mouse_world_y + build_mode_building.height;

                    if (x >= building_left_x && x < building_right_x && y >= building_top_y && y < building_bottom_y) {

                        vchar result{'*', colors::BLACK, colors::BLACK};
                        if ((!solid(idx) &&
                             flag(idx, CAN_STAND_HERE)
                             && !flag(idx, CONSTRUCTION)
                             && !(region::tile_type(idx) == tile_type::STAIRS_DOWN)
                             && !(region::tile_type(idx) == tile_type::STAIRS_UP)
                             && !(region::tile_type(idx) == tile_type::STAIRS_UPDOWN))
                            || (build_mode_building.tag == "floor"
                                && !flag(idx, CONSTRUCTION) &&
                                !(region::tile_type(idx) == tile_type::STAIRS_DOWN)
                                && !(region::tile_type(idx) == tile_type::STAIRS_UP)
                                && !(region::tile_type(idx) == tile_type::STAIRS_UPDOWN)
                                && (flag(idx, CAN_STAND_HERE) ||
                                    flag(idx - 1, CAN_STAND_HERE) ||
                                    flag(idx + 1, CAN_STAND_HERE) ||
                                    flag(idx - REGION_WIDTH, CAN_STAND_HERE) ||
                                    flag(idx + REGION_WIDTH, CAN_STAND_HERE)
                                )
                            )
                                ) {
                            result.foreground = rltk::colors::GREEN;
                        } else {
                            result.foreground = rltk::colors::RED;
                            building_possible = false;
                        }
                        world_scene::add_decal(x, y, z, result, idx);
                    }
                }

                // Architecure Mode
                if (game_master_mode == DESIGN && game_design_mode == ARCHITECTURE) {
                    vchar result{' ', colors::GREY, colors::GREY};

                    // Display existing architecture
                    auto mf = designations->architecture.find(idx);
                    if (mf != designations->architecture.end()) {
                        result.foreground = rltk::colors::YELLOW;
                        result.background = rltk::colors::GREY;
                        switch (mf->second) {
                            case 0 : result.glyph = '#'; break;
                            case 1 : result.glyph = '.'; break;
                            case 2 : result.glyph = '<'; break;
                            case 3 : result.glyph = '>'; break;
                            case 4 : result.glyph = 'X'; break;
                            case 5 : result.glyph = '^'; break;
                            case 6 : result.glyph = '='; break;
                        }
                    }

                    if (arch_available && mf == designations->architecture.end()) {
                        const int building_left_x = mouse::mouse_world_x;
                        const int building_top_y = mouse::mouse_world_y;
                        const int building_right_x = mouse::mouse_world_x + arch_width;
                        const int building_bottom_y = mouse::mouse_world_y + arch_height;

                        if (arch_filled) {

                            if (x >= building_left_x && x < building_right_x &&
                                y >= building_top_y && y < building_bottom_y) {
                                result.background = rltk::colors::BLACK;
                                result.glyph = 177;

                                if (arch_possible && !solid(idx) &&
                                    !flag(idx, CONSTRUCTION) && bridge_id(idx)==0) {
                                    result.foreground = rltk::colors::GREEN;
                                } else {
                                    arch_possible = false;
                                    result.foreground = rltk::colors::RED;
                                }
                            }
                        } else {
                            if (x >= building_left_x && x < building_right_x &&
                                y >= building_top_y && y < building_bottom_y) {
                                bool interior = false;
                                if (arch_width > 2 && x+clip_left >= building_left_x+1 && x+clip_left < building_right_x-1 ) {
                                    interior = true;
                                }
                                if (arch_height > 2 && y+clip_top >= building_top_y+1 && y+clip_top < building_bottom_y-1) {
                                    interior = true;
                                }
                                if (x+clip_left == building_left_x) interior=false;
                                if (x+clip_left == building_right_x-1) interior = false;
                                if (y+clip_top == building_top_y) interior = false;
                                if (y+clip_top == building_bottom_y-1) interior = false;

                                if (!interior) {
                                    result.background = rltk::colors::BLACK;
                                    result.glyph = 177;

                                    if (arch_possible && !solid(idx) &&
                                        !flag(idx, CONSTRUCTION) && bridge_id(idx)==0) {
                                        result.foreground = rltk::colors::GREEN;
                                    } else {
                                        arch_possible = false;
                                        result.foreground = rltk::colors::RED;
                                    }
                                }
                            }
                        }
                    }

                    if (result.glyph != ' ') {
                        world_scene::add_world_cube(x, y, z, result, idx);
                    }
                }
            } else {
                // Add renderables in open space
                auto rfinder = renderables.find(idx);
                if (rfinder != renderables.end()) {
                    screen_render_t &sr = rfinder->second[glyph_cycle % rfinder->second.size()];
                    world_scene::add_simple_renderable(x, y, z, sr.c, idx);
                }

                // Add composite renderables in open space
                auto cfinder = composite_renderables.find(idx);
                if (cfinder != composite_renderables.end()) {
                    for (const auto &sr : cfinder->second[glyph_cycle % cfinder->second.size()]) {
                        world_scene::add_composite_renderable(x, y, z, sr.c, idx);
                    }
                }
            }
        } else if (game_master_mode == DESIGN && game_design_mode == DIGGING) {
            const uint8_t tiletype = region::tile_type(idx);
            if (tiletype != tile_type::OPEN_SPACE) {
                vchar result{257, colors::CYAN, colors::GREY};
                auto mf = designations->mining.find(idx);
                if (mf != designations->mining.end()) {
                    result.foreground = colors::YELLOW;
                    switch (mf->second) {
                        case 1 :
                            result.glyph = 177;
                            break;
                        case 2 :
                            result.glyph = 31;
                            break;
                        case 3 :
                            result.glyph = 30;
                            break;
                        case 4 :
                            result.glyph = '<';
                            break;
                        case 5 :
                            result.glyph = '>';
                            break;
                        case 6 :
                            result.glyph = 'X';
                            break;
                    }

                }
                world_scene::add_world_cube(x, y, z, result, idx);
                world_changed = true;
            }
        }
    }

    void populate_ascii_render(const int &x, const int &y, const int &z, const int &idx, const bool &revealed, const uint8_t &tiletype) {
        if (revealed) {
            rltk::vchar tile{' ', colors::BLACK, colors::BLACK};

            if (tiletype != tile_type::OPEN_SPACE) {
                if (tiletype == tile_type::RAMP) {
                    tile = render_cache(idx);
                } else if (has_ceiling(tiletype)) {
                    if (game_master_mode == DESIGN && game_design_mode == CHOPPING && tree_id(idx)>0
                        && designations->chopping.find(tree_id(idx))!=designations->chopping.end())
                    {
                        tile.glyph = '*';
                        tile.foreground = colors::RED;
                    } else {
                        tile = render_cache(idx);
                    }
                } else if (has_floor(tiletype)) {
                    tile = render_cache(idx);
                }

                // Add water
                const float water_depth = (float)water_level(idx) / 10.0f;
                if (water_depth>0) {
                    tile = render_cache(idx);
                }

                // Add vegetation
                auto vc = veg_cache(idx);
                if (vc.glyph > 0) {
                    tile = vc;
                    tile.background = rltk::colors::BLACK;
                }

                // Add blood stains
                if (blood_stain(idx)) {
                    tile.background = colors::RED;
                }

                // Add renderables
                auto rfinder = renderables.find(idx);
                if (rfinder != renderables.end()) {
                    screen_render_t &sr = rfinder->second[glyph_cycle % rfinder->second.size()];
                    tile = sr.c;
                }

                // Add composite renderables
                auto cfinder = composite_renderables.find(idx);
                if (cfinder != composite_renderables.end()) {
                    tile = cfinder->second[glyph_cycle % cfinder->second.size()][0].c;
                }

                // Stockpiles
                if (game_master_mode == DESIGN && game_design_mode == STOCKPILES && current_stockpile>0 && stockpile_id(idx)==current_stockpile) {
                    tile = vchar{'#', colors::MAGENTA, colors::MAGENTA};
                }

                // Mining
                if (game_master_mode == DESIGN && game_design_mode == DIGGING) {
                    auto mf = designations->mining.find(idx);
                    if (mf != designations->mining.end()) {
                        vchar result{'!', colors::YELLOW, colors::YELLOW};
                        switch (mf->second) {
                            case 1 : result.glyph = 177; break;
                            case 2 : result.glyph = 31; break;
                            case 3 : result.glyph = 30; break;
                            case 4 : result.glyph = '<'; break;
                            case 5 : result.glyph = '>'; break;
                            case 6 : result.glyph = 'X'; break;
                        }

                        tile = result;
                    }
                }

                // Buildings
                if (game_master_mode == DESIGN && game_design_mode == BUILDING && has_build_mode_building) {
                    const int building_left_x = mouse::mouse_world_x;
                    const int building_top_y = mouse::mouse_world_y;
                    const int building_right_x = mouse::mouse_world_x + build_mode_building.width;
                    const int building_bottom_y = mouse::mouse_world_y + build_mode_building.height;

                    if (x >= building_left_x && x < building_right_x && y >= building_top_y && y < building_bottom_y) {

                        vchar result{'*', colors::BLACK, colors::BLACK};
                        if ((!solid(idx) &&
                                flag(idx, CAN_STAND_HERE)
                             && !flag(idx, CONSTRUCTION)
                             && !(region::tile_type(idx) == tile_type::STAIRS_DOWN)
                             && !(region::tile_type(idx) == tile_type::STAIRS_UP)
                             && !(region::tile_type(idx) == tile_type::STAIRS_UPDOWN))
                            || (build_mode_building.tag == "floor"
                                && !flag(idx, CONSTRUCTION) &&
                                !(region::tile_type(idx) == tile_type::STAIRS_DOWN)
                                && !(region::tile_type(idx) == tile_type::STAIRS_UP)
                                && !(region::tile_type(idx) == tile_type::STAIRS_UPDOWN)
                                && (flag(idx, CAN_STAND_HERE) ||
                                flag(idx - 1, CAN_STAND_HERE) ||
                                flag(idx + 1, CAN_STAND_HERE) ||
                                flag(idx - REGION_WIDTH, CAN_STAND_HERE) ||
                                flag(idx + REGION_WIDTH, CAN_STAND_HERE)
                                )
                            )
                                ) {
                            result.foreground = rltk::colors::GREEN;
                        } else {
                            result.foreground = rltk::colors::RED;
                            building_possible = false;
                        }
                        tile = result;
                    }
                }

                // Architecure Mode
                if (game_master_mode == DESIGN && game_design_mode == ARCHITECTURE) {
                    vchar result{' ', colors::GREY, colors::GREY};

                    // Display existing architecture
                    auto mf = designations->architecture.find(idx);
                    if (mf != designations->architecture.end()) {
                        result.foreground = rltk::colors::YELLOW;
                        result.background = rltk::colors::GREY;
                        switch (mf->second) {
                            case 0 : result.glyph = '#'; break;
                            case 1 : result.glyph = '.'; break;
                            case 2 : result.glyph = '<'; break;
                            case 3 : result.glyph = '>'; break;
                            case 4 : result.glyph = 'X'; break;
                            case 5 : result.glyph = '^'; break;
                            case 6 : result.glyph = '='; break;
                        }
                    }

                    if (arch_available && mf == designations->architecture.end()) {
                        const int building_left_x = mouse::mouse_world_x;
                        const int building_top_y = mouse::mouse_world_y;
                        const int building_right_x = mouse::mouse_world_x + arch_width;
                        const int building_bottom_y = mouse::mouse_world_y + arch_height;

                        if (arch_filled) {

                            if (x >= building_left_x && x < building_right_x &&
                                y >= building_top_y && y < building_bottom_y) {
                                result.background = rltk::colors::BLACK;
                                result.glyph = 177;

                                if (arch_possible && !solid(idx) &&
                                    !flag(idx, CONSTRUCTION) && bridge_id(idx)==0) {
                                    result.foreground = rltk::colors::GREEN;
                                } else {
                                    arch_possible = false;
                                    result.foreground = rltk::colors::RED;
                                }
                            }
                        } else {
                            if (x >= building_left_x && x < building_right_x &&
                                y >= building_top_y && y < building_bottom_y) {
                                bool interior = false;
                                if (arch_width > 2 && x+clip_left >= building_left_x+1 && x+clip_left < building_right_x-1 ) {
                                    interior = true;
                                }
                                if (arch_height > 2 && y+clip_top >= building_top_y+1 && y+clip_top < building_bottom_y-1) {
                                    interior = true;
                                }
                                if (x+clip_left == building_left_x) interior=false;
                                if (x+clip_left == building_right_x-1) interior = false;
                                if (y+clip_top == building_top_y) interior = false;
                                if (y+clip_top == building_bottom_y-1) interior = false;

                                if (!interior) {
                                    result.background = rltk::colors::BLACK;
                                    result.glyph = 177;

                                    if (arch_possible && !solid(idx) &&
                                        !flag(idx, CONSTRUCTION) && bridge_id(idx)==0) {
                                        result.foreground = rltk::colors::GREEN;
                                    } else {
                                        arch_possible = false;
                                        result.foreground = rltk::colors::RED;
                                    }
                                }
                            }
                        }
                    }

                    if (result.glyph != ' ') {
                        tile = result;
                    }
                }
                world_scene::add_ascii_tile(x, y, z, tile, idx);
            } else {
                // Add renderables
                auto rfinder = renderables.find(idx);
                if (rfinder != renderables.end()) {
                    screen_render_t &sr = rfinder->second[glyph_cycle % rfinder->second.size()];
                    tile = sr.c;
                }

                // Add composite renderables
                auto cfinder = composite_renderables.find(idx);
                if (cfinder != composite_renderables.end()) {
                    tile = cfinder->second[glyph_cycle % cfinder->second.size()][0].c;
                }
            } // end open space
        } else if (game_master_mode == DESIGN && game_design_mode == DIGGING) {
            const uint8_t tiletype = region::tile_type(idx);
            if (tiletype != tile_type::OPEN_SPACE) {
                vchar result{257, colors::CYAN, colors::GREY};
                auto mf = designations->mining.find(idx);
                if (mf != designations->mining.end()) {
                    result.foreground = colors::YELLOW;
                    switch (mf->second) {
                        case 1 :
                            result.glyph = 177;
                            break;
                        case 2 :
                            result.glyph = 31;
                            break;
                        case 3 :
                            result.glyph = 30;
                            break;
                        case 4 :
                            result.glyph = '<';
                            break;
                        case 5 :
                            result.glyph = '>';
                            break;
                        case 6 :
                            result.glyph = 'X';
                            break;
                    }

                }
                world_scene::add_ascii_tile(x, y, z, result, idx);
                world_changed = true;
            }
        }
    }

    inline void populate_vertex_buffer(const int &start_z, const int &end_z) {
        const auto edges = calculate_edges();
        if (camera->ascii_mode) {
            render_area_loop(start_z, end_z, edges, populate_ascii_render);
        } else {
            render_area_loop(start_z, end_z, edges, populate_tile_render);
        }
    }

    inline void build_world_vertex_buffer() {
        world_scene::reset_all();

        // Add world geometry, depending upon game mode
        const bool single_z_layer = !(game_master_mode == PLAY || game_master_mode == ROGUE);

        if (game_master_mode == DESIGN && game_design_mode == BUILDING && has_build_mode_building) building_possible = true;

        if (single_z_layer) {
            populate_vertex_buffer(camera_position->region_z, camera_position->region_z + 1);
        } else {
            populate_vertex_buffer(std::max(1, camera_position->region_z - 7),
                                         std::min(REGION_DEPTH - 1, camera_position->region_z + 1));
        }

        // Add marker types in design mode as needed
        if (game_master_mode == DESIGN) {
            if (game_design_mode == GUARDPOINTS) {
                for (const auto &gp : designations->guard_points) {
                    world_scene::add_decal(gp.second.x, gp.second.y, gp.second.z,
                                           vchar{'G', colors::CYAN, colors::CYAN}, mapidx(gp.second));
                }
            } else if (game_design_mode == HARVEST) {
                for (const auto &h : designations->harvest) {
                    world_scene::add_decal(h.second.x, h.second.y, h.second.z,
                                           vchar{'H', colors::CYAN, colors::CYAN}, mapidx(h.second));
                }
            }
        }

        if (game_master_mode == DESIGN && game_design_mode == BUILDING) {
            if (mouse::clicked) {
                emit(build_request_message{mouse::mouse_world_x, mouse::mouse_world_y, camera_position->region_z, build_mode_building});
                emit(refresh_available_buildings_message{});
                emit(map_dirty_message{});
                emit(renderables_changed_message{});
            }
        }
    }

    void gl_states() {
        auto screen_size = rltk::get_window()->getSize();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT);
        glShadeModel(GL_SMOOTH);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glViewport(0,0,screen_size.x,screen_size.y);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(90.f, 1.f, 1.f, 300.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        switch (camera->camera_mode) {
            case FRONT : {
                // Nice X-perspective view
                gluLookAt((float) camera_position->region_x, ((float) camera_position->region_z) + (float)camera->zoom_level,
                          ((float) camera_position->region_y) + ((float)camera->zoom_level/3.0f), // Camera
                          (float) camera_position->region_x, (float) camera_position->region_z,
                          (float) camera_position->region_y, // Target
                          0.0f, 1.0f, 0.0f // Up
                );
            } break;

            case TOP_DOWN : {
                // Top-down
                gluLookAt((float) camera_position->region_x, ((float) camera_position->region_z) + (float)camera->zoom_level,
                          ((float) camera_position->region_y) + 0.1f, // Camera
                          (float) camera_position->region_x, (float) camera_position->region_z,
                          (float) camera_position->region_y, // Target
                          0.0f, 1.0f, 0.0f // Up
                );
            } break;

            case DIAGONAL : {
                // Diagonal
                gluLookAt((float) camera_position->region_x + (float)camera->zoom_level, ((float) camera_position->region_z) + (float)camera->zoom_level,
                          ((float) camera_position->region_y) + (float)camera->zoom_level, // Camera
                          (float) camera_position->region_x, (float) camera_position->region_z,
                          (float) camera_position->region_y, // Target
                          0.0f, 1.0f, 0.0f // Up
                );
            } break;
        }

        glEnable(GL_TEXTURE_2D);
        //glActiveTexture(GL_TEXTURE0);
        sf::Texture::bind(rltk::get_texture(term(1)->get_font_tag()));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
        glDrawBuffers(2, buffers);
    }

    inline void setup_fbo() {
        //program_id = LoadShaders("world_defs/vertex.glsl", "world_defs/fragment.glsl");
        deferred_id = LoadShaders("world_defs/vertex-deferred.glsl", "world_defs/fragment-deferred.glsl");
        //index_program_id = LoadShaders("world_defs/vertex-index.glsl", "world_defs/fragment-index.glsl");
        loaded_program = true;
        calculate_texture_info();

        // Create and bind the framebuffer for mouse-picking output
        glGenFramebuffers(1, &mouse_pick_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);

        // Create the mouse picker render target texture
        const auto screen_size = rltk::get_window()->getSize();
        glGenTextures(1, &mouse_pick_texture);
        glBindTexture(GL_TEXTURE_2D, mouse_pick_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_size.x, screen_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mouse_pick_texture, 0);

        // Create the render target texture
        glGenTextures(1, &render_texture);
        glBindTexture(GL_TEXTURE_2D, render_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_size.x, screen_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, render_texture, 0);

        // Create a depth-buffer for the render target
        glGenRenderbuffers(1, &mouse_pick_depth);
        glBindRenderbuffer(GL_RENDERBUFFER, mouse_pick_depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_size.x, screen_size.y);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mouse_pick_depth);

        GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
        glDrawBuffers(2, buffers);

        // Return to regular render mode
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    inline void draw_world() {
        if (!loaded_program) {
            setup_fbo();
        }

        gl_frustrum::extract();
        build_world_vertex_buffer();

        // Render the main visible scene
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gl_states();
        world_scene::render_world(deferred_id);
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Return to screen rendering
    }

    std::tuple<int,int,int> readback_texture_pixel(const int &x, const int &y) {
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        uint8_t pixel[4] = {0,0,0,0};
        glReadPixels(x, get_window()->getSize().y - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel[0]);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        int mx = pixel[0];
        int my = pixel[1];
        int mz = pixel[2];
        //std::cout << "Mouse: " << x << "/" << y << " = " << mx << "/" << my << "/" << mz << "\n";
        return std::make_tuple(mx, my, mz);
        //return std::make_tuple(0, 0, 0);
    };

}

void map_render_t::render() {
    push_gl_states();

    glUseProgram(map_render::deferred_id);

    map_render::gl_states();

    map_render::draw_world();

    pop_gl_states();

    // Render the index buffer
    auto sz = rltk::get_window()->getSize();
    const float W = (float)sz.x;
    const float H = (float)sz.y;
    push_gl_states();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, sz.x, sz.y);
    glOrtho(0, sz.x, 0, sz.y, 0.0f, 1.0f);
    glEnableClientState(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, map_render::render_texture);

    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);

    glTexCoord2f(0, 1);
    glVertex2f(0.0f, H);

    glTexCoord2f(0, 0);
    glVertex2f(0.0f, 0.0f);

    glTexCoord2f(1, 0);
    glVertex2f(W, 0.0f);

    glTexCoord2f(1, 1);
    glVertex2f(W, H);

    glEnd();
    pop_gl_states();
}
*/