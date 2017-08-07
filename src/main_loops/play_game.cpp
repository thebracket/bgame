#include "play_game.hpp"
#include "../bengine/imgui.h"
#include "../bengine/imgui_impl_glfw_gl3.h"

namespace play_game {
    void tick(const double &duration_ms) {
        ImGui_ImplGlfwGL3_NewFrame();

        ImGui::Begin("Temp");
        ImGui::Text("Play game");
        ImGui::End();

        ImGui::Render();
    }
}