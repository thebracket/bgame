#pragma once

#include <memory>

namespace gl {

    struct terrain_chunk_shader_t; // Forward
    struct static_model_shader_t; // Forward
    struct renderable_shader_t; // Forward
    struct render_shader_t; // Forward

    extern std::unique_ptr<terrain_chunk_shader_t> terrain_chunk_shader;
    extern std::unique_ptr<static_model_shader_t> static_model_shader;
    extern std::unique_ptr<renderable_shader_t> renderable_shader;
    extern std::unique_ptr<render_shader_t> render_shader;

    void load_shaders();

}