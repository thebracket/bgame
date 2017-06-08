#pragma once

#include <memory>
#include "base_shader.hpp"
#include "chunk.hpp"
#define GLM_COMPILER 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace map_render {

    extern bool loaded_render_shader;
    extern std::unique_ptr<gl::base_shader_t> render_shader;

    void render_test_texture(float left, float top, float right, float bottom, GLuint &target_texture);
    void render_mixed_texture(float left, float top, float right, float bottom);
    void load_render_shader();
    void render_phase_three_composition();
}
