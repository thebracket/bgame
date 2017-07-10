#pragma once

#include <memory>
#include "shaders/base_shader.hpp"
#include "chunks/chunk.hpp"
#define GLM_COMPILER 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "model_requests_t.hpp"

namespace map_render {
    void setup_matrices();
    void render_terrain_chunk(const gl::chunk_t &chunk);
    void render_terrain_to_gbuffer(gl::model_request_t &models);
    void add_renderables(gl::model_request_t &models);
    void render_static_models(gl::model_request_t &models);
}
