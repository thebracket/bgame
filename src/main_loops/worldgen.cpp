#include "worldgen.hpp"
#include "../bengine/IconsFontAwesome.h"
#include "../bengine/imgui.h"
#include "../bengine/imgui_impl_glfw_gl3.h"
#include "../bengine/telemetry.hpp"

namespace worldgen {
    bool initialized = false;

    void init() {
        call_home("WorldGen", "Opened");
        initialized = true;
    }

    void tick(const double &duration_ms) {
        if (!initialized) init();

        ImGui_ImplGlfwGL3_NewFrame();

        ImGui::Begin("World Generation");
        ImGui::Text("%s", "Coming soon...");
        ImGui::End();

        ImGui::Render();
    }
}