#include "shader_storage.hpp"
#include "terrain_chunk_shader.hpp"
#include "static_model_shader.hpp"
#include "renderable_shader.hpp"
#include "render_shader.hpp"

namespace gl {
    std::unique_ptr<terrain_chunk_shader_t> terrain_chunk_shader;
    std::unique_ptr<static_model_shader_t> static_model_shader;
    std::unique_ptr<renderable_shader_t> renderable_shader;
    std::unique_ptr<render_shader_t> render_shader;

    void load_shaders() {
        terrain_chunk_shader = std::make_unique<terrain_chunk_shader_t>();
        static_model_shader = std::make_unique<static_model_shader_t>();
        renderable_shader = std::make_unique<renderable_shader_t>();
        render_shader = std::make_unique<render_shader_t>();
    }
}