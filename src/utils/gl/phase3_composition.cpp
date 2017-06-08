#include "phase3_composition.hpp"
#include <rltk.hpp>
#include "main_fbo.hpp"
#include "sun_fbo.hpp"

namespace map_render {

    bool loaded_render_shader = false;
    std::unique_ptr<gl::base_shader_t> render_shader;

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

    void load_render_shader() {
        render_shader = std::make_unique<gl::base_shader_t>("world_defs/shaders/render_vertex.glsl",
                                                            "world_defs/shaders/render_fragment.glsl");
        loaded_render_shader = true;
    }

    void render_phase_three_composition() {
        auto sz = rltk::get_window()->getSize();
        const float W = (float)sz.x;
        const float H = (float)sz.y;
        push_gl_states();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(0, 0, sz.x, sz.y);
        glOrtho(0, sz.x, 0, sz.y, 0.0f, 1.0f);
        render_test_texture(0.0f, 0.0f, W/2.0f, H/2.0f, map_render::lit_texture);
        render_test_texture(W/2.0f, 0.0f, W, H/2.0f, map_render::render_texture);
        render_test_texture(0.0f, H/2.0f, W/2.0f, H, map_render::sun_depth_texture);
        render_test_texture(W/2.0f, H/2.0f, W, H, map_render::sun_render);
    }
}
