#pragma once

#include "fbo/depth_fbo.hpp"
#include <memory>
#include <atomic>
#include <glm/detail/type_mat.hpp>
#include <glm/vec3.hpp>

namespace render {
    namespace sunlight {
        extern std::unique_ptr<depth_fbo_t> sun_fbo;
        extern std::atomic<bool> sun_changed;
        extern glm::mat4 lightProjection;
        extern glm::mat4 lightView;
        extern glm::mat4 lightSpaceMatrix;
        extern glm::vec3 light_position;
        extern glm::vec3 light_color;

        void update(const int &screen_w, const int &screen_h, const double &duration_ms);
    }
}