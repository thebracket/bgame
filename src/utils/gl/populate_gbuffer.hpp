#pragma once

#include <memory>
#include "base_shader.hpp"
#include "chunks/chunk.hpp"
#define GLM_COMPILER 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace map_render {
    extern bool loaded_terrain_shader;
    extern std::unique_ptr<gl::base_shader_t> terrain_chunk_shader;
    extern glm::mat4 camera_projection_matrix;
    extern glm::mat4 camera_modelview_matrix;

    void load_terrain_shader();
    void setup_matrices();
    void render_terrain_chunk(const gl::chunk_t &chunk);
    void render_terrain_to_gbuffer();
}
