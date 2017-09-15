#include "camera_system.hpp"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"
#include "../../bengine/gl_include.hpp"
#include "../../global_assets/game_camera.hpp"
#include "../../planet/constants.hpp"
#include "../../render_engine/render_engine.hpp"
#include "../keydamper.hpp"

namespace systems {
    namespace camerasys {
        void run(const double &duration_ms) {
            ImGuiIO& io = ImGui::GetIO();

            if (is_key_down(GLFW_KEY_LEFT, false)) {
                --camera_position->region_x;
                if (camera_position->region_x < 0) camera_position->region_x = 0;
                render::camera_moved = true;
                render::models_changed = true;
            }
            if (is_key_down(GLFW_KEY_RIGHT, false)) {
                ++camera_position->region_x;
                if (camera_position->region_x > REGION_WIDTH-1) camera_position->region_x = REGION_WIDTH-1;
                render::camera_moved = true;
                render::models_changed = true;
            }

            if (is_key_down(GLFW_KEY_UP, false)) {
                --camera_position->region_y;
                if (camera_position->region_y < 0) camera_position->region_y = 0;
                render::camera_moved = true;
                render::models_changed = true;
            }

            if (is_key_down(GLFW_KEY_DOWN, false)) {
                ++camera_position->region_y;
                if (camera_position->region_y > REGION_HEIGHT-1) camera_position->region_y = REGION_HEIGHT-1;
                render::camera_moved = true;
                render::models_changed = true;
            }

            if (is_key_down(GLFW_KEY_COMMA)) {
                ++camera_position->region_z;
                if (camera_position->region_z > REGION_DEPTH-1) camera_position->region_z = REGION_DEPTH-1;
                render::camera_moved = true;
                render::models_changed = true;
            }

            if (is_key_down(GLFW_KEY_PERIOD)) {
                --camera_position->region_z;
                if (camera_position->region_z < 1) camera_position->region_z = 1;
                render::camera_moved = true;
                render::models_changed = true;
            }

            if (is_key_down(GLFW_KEY_TAB)) {
                switch (camera->camera_mode) {
                    case DIAGONAL : camera->camera_mode = FRONT; break;
                    case FRONT : camera->camera_mode = TOP_DOWN; break;
                    case TOP_DOWN : camera->camera_mode = DIAGONAL; break;
                }
                render::camera_moved = true;
                render::models_changed = true;
            }

            if (is_key_down(GLFW_KEY_PAGE_UP, false)) {
                --camera->zoom_level;
                if (camera->zoom_level < 2) camera->zoom_level = 2;
                render::camera_moved = true;
                render::models_changed = true;
            }

            if (is_key_down(GLFW_KEY_PAGE_DOWN, false)) {
                ++camera->zoom_level;
                if (camera->zoom_level > 50) camera->zoom_level = 50;
                render::camera_moved = true;
                render::models_changed = true;
            }
        }
    }
}