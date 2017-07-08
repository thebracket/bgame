#pragma once

#include <memory>
#include "shaders/base_shader.hpp"
#include "chunks/chunk.hpp"
#define GLM_COMPILER 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace map_render {

    extern glm::vec3 ambient_color;
    extern glm::vec3 sun_position;
    extern glm::vec3 sun_color;
    extern glm::vec3 camera_position_v; // This needs to be used in the shader for camera position, rather than the raw pos to provide real reflectivity

    void place_sun_moon();
}