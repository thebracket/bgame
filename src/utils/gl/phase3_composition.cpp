#include "phase3_composition.hpp"
#include <rltk.hpp>
#include "main_fbo.hpp"
#include "sun_fbo.hpp"
#include "phase1_sunmoon.hpp"

namespace map_render {

    bool loaded_render_shader = false;
    std::unique_ptr<gl::base_shader_t> render_shader;

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

    void render_mixed_texture(float left, float top, float right, float bottom) {
        glUseProgram(map_render::render_shader->program_id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, map_render::render_texture); // Texture slot 0 = albedo
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, map_render::interpolated_pos_texture); // Texture slot 1 = position
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, map_render::normal_texture); // Texture slot 2 = normal
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, map_render::light_render_pos); // Texture slot 3 = light position
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, map_render::light_render_col); // Texture slot 4 = light color

        glUniform1i(glGetUniformLocation(map_render::render_shader->program_id, "albedo_tex"), 0);
        glUniform1i(glGetUniformLocation(map_render::render_shader->program_id, "position_tex"), 1);
        glUniform1i(glGetUniformLocation(map_render::render_shader->program_id, "normal_tex"), 2);
        glUniform1i(glGetUniformLocation(map_render::render_shader->program_id, "light_position"), 3);
        glUniform1i(glGetUniformLocation(map_render::render_shader->program_id, "light_color"), 4);

        glUniform3fv(glGetUniformLocation(map_render::render_shader->program_id, "ambient_color"), 1, glm::value_ptr(ambient_color));

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

    void load_render_shader() {
        render_shader = std::make_unique<gl::base_shader_t>("world_defs/shaders/render_vertex.glsl",
                                                            "world_defs/shaders/render_fragment.glsl");
        loaded_render_shader = true;
    }

    void render_phase_three_composition() {
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
            render_test_texture(0.0f, 0.0f, W / 2.0f, H / 2.0f, map_render::sun_depth_texture);
            render_test_texture(W / 2.0f, 0.0f, W, H / 2.0f, map_render::render_texture);
            render_test_texture(0.0f, H / 2.0f, W / 2.0f, H, map_render::light_render_col);
        } else {
            render_mixed_texture(0.0f, 0.0f, W, H);
            //render_test_texture(0.0f, 0.0f, W / 2.0f, H / 2.0f, map_render::interpolated_pos_texture);
            //render_test_texture(0.0f, H / 2.0f, W / 2.0f, H, map_render::sun_render);
        }
    }
}
