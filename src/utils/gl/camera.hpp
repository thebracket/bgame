#pragma once

#define GLM_COMPILER 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gl {
    extern glm::mat4 camera_projection_matrix;
    extern glm::mat4 camera_modelview_matrix;

    void setup_camera();
}