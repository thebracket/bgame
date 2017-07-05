#include "composition.hpp"
#include <rltk.hpp>
#include "main_fbo.hpp"
#include "sun_moon.hpp"
#include "textures/texture.hpp"

namespace map_render {

    bool loaded_render_shader = false;
    std::unique_ptr<gl::base_shader_t> render_shader;

    GLint render_albedo_tex_loc;
    GLint render_position_tex_loc;
    GLint render_normal_tex_loc;
    GLint render_ambient_color_loc;
    GLint render_light_pos_tex_loc;
    GLint render_light_col_tex_loc;
    GLint render_sun_moon_position_loc;
    GLint render_sun_moon_color_loc;
    GLint render_flag_tex_loc;
    GLint render_camera_position_loc;

    void load_render_shader() {
        render_shader = std::make_unique<gl::base_shader_t>("world_defs/shaders/render_vertex.glsl",
                                                            "world_defs/shaders/render_fragment.glsl");
        loaded_render_shader = true;

        render_albedo_tex_loc = render_shader->get_uniform_location("albedo_tex");
        render_position_tex_loc = render_shader->get_uniform_location("position_tex");
        render_normal_tex_loc = render_shader->get_uniform_location("normal_tex");
        render_ambient_color_loc = render_shader->get_uniform_location("ambient_color");
        render_light_pos_tex_loc = render_shader->get_uniform_location("light_pos_tex");
        render_light_col_tex_loc = render_shader->get_uniform_location("light_col_tex");
        render_sun_moon_position_loc = render_shader->get_uniform_location("sun_moon_position");
        render_sun_moon_color_loc = render_shader->get_uniform_location("sun_moon_color");
        render_flag_tex_loc = render_shader->get_uniform_location("flag_tex");
        render_camera_position_loc = render_shader->get_uniform_location("cameraPosition");
    }

    void render_test_texture(float left, float top, float right, float bottom, GLuint &target_texture) {
        glActiveTexture(GL_TEXTURE0);
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

    void render_texture_atlas(float left, float top, float right, float bottom) {
        glActiveTexture(GL_TEXTURE0);
        glEnableClientState(GL_TEXTURE_2D);
        textures::bind_atlas();

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
        glUseProgram(map_render::render_shader->program_id);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, map_render::render_texture); // Texture slot 0 = albedo
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, map_render::interpolated_pos_texture); // Texture slot 1 = position
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, map_render::normal_texture); // Texture slot 2 = normal
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, map_render::light_position_texture); // Texture slot 3 = light position (outdoor in alpha)
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, map_render::light_color_texture); // Texture slot 4 = light color
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, map_render::flag_texture); // Texture slot 4 = light color

        glUniform1i(render_albedo_tex_loc, 0);
        glUniform1i(render_position_tex_loc, 1);
        glUniform1i(render_normal_tex_loc, 2);
        glUniform1i(render_light_pos_tex_loc, 3);
        glUniform1i(render_light_col_tex_loc, 4);
        glUniform1i(render_flag_tex_loc, 5);

        glUniform3fv(render_ambient_color_loc, 1, glm::value_ptr(ambient_color));
        glUniform3fv(render_sun_moon_position_loc, 1, glm::value_ptr(sun_position));
        glUniform3fv(render_sun_moon_color_loc, 1, glm::value_ptr(sun_color));
        glUniform3fv(render_camera_position_loc, 1, glm::value_ptr(camera_position_v));

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


        glUseProgram(0);
    }

    void render_composition() {
        constexpr bool test_mode = false;

        auto sz = rltk::get_window()->getSize();
        const float W = (float)sz.x;
        const float H = (float)sz.y;
        push_gl_states();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(0, 0, sz.x, sz.y);
        glOrtho(0, sz.x, 0, sz.y, 0.0f, 1.0f);

        if (test_mode) {
            render_test_texture(W / 2.0f, 0.0f, W, H / 2.0f, map_render::render_texture);
        } else {
            render_mixed_texture(0.0f, 0.0f, W, H);
            //render_test_texture(0.0f, 0.0f, W, H, map_render::render_texture);
            //render_test_texture(0.0f, H / 2.0f, W / 2.0f, H, map_render::normal_texture);
            //render_texture_atlas(W / 2.0f, 0.0f, W, H / 2.0f);
        }
    }
}
