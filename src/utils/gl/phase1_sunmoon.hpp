#pragma once

#include <memory>
#include "base_shader.hpp"
#include "chunk.hpp"
#define GLM_COMPILER 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace map_render {
    extern std::unique_ptr<gl::base_shader_t> shadow_shader;
    extern std::unique_ptr<gl::base_shader_t> light_shader;
    extern bool loaded_shadow_shader;

    extern glm::mat4 sun_projection_matrix;
    extern glm::mat4 sun_modelview_matrix;
    extern glm::vec3 ambient_color;
    extern glm::vec3 sun_moon_color;
    extern glm::vec3 sun_moon_position;

    void load_shadow_shader();
    void render_sun_chunk(const gl::chunk_t &chunk, bool set_uniforms = true);
    void place_sun_moon();
    void place_light(int direction, float range, const float sun_x, const float sun_y, const float sun_z, const float r, const float g, const float b);
    void render_phase_one_sun_moon();

    void render_to_light_buffer(const bool clear);
}