#pragma once

#include <memory>
#include "base_shader.hpp"
#include "chunks/chunk.hpp"
#define GLM_COMPILER 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace map_render {

    extern glm::vec3 ambient_color;
    extern glm::vec3 sun_position;
    extern glm::vec3 sun_color;

    void place_sun_moon();
}