#include "worldgen.hpp"
#include "../bengine/IconsFontAwesome.h"
#include "../bengine/imgui.h"
#include "../bengine/imgui_impl_glfw_gl3.h"
#include "../bengine/telemetry.hpp"
#include "../global_assets/texture_storage.hpp"
#include "../bengine/simple_sprite.hpp"
#include "../bengine/textures.hpp"
#include "../bengine/main_window.hpp"
#include "main_menu.hpp"
#include "../planet/planet_builder.hpp"
#include "../global_assets/game_planet.hpp"
#include "../global_assets/shader_storage.hpp"
#define GLM_COMPILER 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace worldgen {
    enum world_gen_mode_t { WG_MENU, WG_RUNNING };
    bool initialized = false;
    int seed;
    int plains = 3;
    int water = 3;
    int starting_settlers = 10;
    bool strict_beamdown = false;
    world_gen_mode_t mode = WG_MENU;
    std::unique_ptr<std::thread> world_thread;
    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;
    glm::mat4 model_matrix;
    unsigned int vbo = 0;
    unsigned int projloc = -1;
    unsigned int viewloc = -1;
    unsigned int modelloc = -1;

    float world_spin = 0.0f;

    void init() {
        call_home("WorldGen", "Opened");
        initialized = true;
    }

    void start_thread() {
        int screen_w, screen_h;
        glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);

        setup_build_planet();

        assert(assets::worldgenshader != 0);
        projection_matrix = glm::perspective(90.0, 1.0, 1.0, 300.0);
        view_matrix = glm::lookAt(glm::vec3(100.0f, 100.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_matrix = glm::mat4(); // Identity
        projloc = glGetUniformLocation(assets::worldgenshader, "projection_mat");
        viewloc = glGetUniformLocation(assets::worldgenshader, "view");
        modelloc = glGetUniformLocation(assets::worldgenshader, "model");
        assert(projloc != -1);
        assert(viewloc != -1);
        glUniformMatrix4fv(projloc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view_matrix));
        glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model_matrix));
        std::cout << "Set uniforms " << projloc << "/" << viewloc << "\n";

        world_thread = std::make_unique<std::thread>(build_planet, seed, water, plains, starting_settlers, strict_beamdown);
    }

    void render_menu() {
        bengine::display_sprite(assets::background_image->texture_id);
        ImGui::Begin("World Generation", nullptr, ImVec2{600,125}, 0.5f, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);

        ImGui::InputInt("World Seed", &seed);
        ImGui::SliderInt("Water Level", &water, 1, 4);
        ImGui::SliderInt("Plains Level", &plains, 1, 4);
        ImGui::SliderInt("Starting Settlers", &starting_settlers, 1, 20);
        ImGui::Checkbox("Require Teleport Beacon for Beamdown", &strict_beamdown);
        if (ImGui::Button("Create World")) {
            mode = WG_RUNNING;
            start_thread();
        }
        ImGui::SameLine();
        if (ImGui::Button("Back to main menu")) {
            bengine::main_func = main_menu::tick;
        }
        ImGui::End();
    }

    constexpr float latitude_per_tile = 180.0f / WORLD_HEIGHT;
    constexpr float longitude_per_tile = 360.0f / WORLD_WIDTH;
    constexpr float PI = 3.14159265359F;
    constexpr float TO_RADIANS = PI / 180.0f;

    float latitude_for_world_tile(const int y) {
        return (static_cast<float>(y) * latitude_per_tile);
    }

    float longitude_for_world_tile(const int &x) {
        return (static_cast<float>(x) * longitude_per_tile);
    }

    void render_globe() {
        int screen_w, screen_h;
        glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);
        model_matrix = glm::mat4();
        model_matrix = glm::rotate(model_matrix, world_spin, glm::vec3(1.0, 0.0, 0.0));

        planet_builder_lock.lock();

        std::vector<float> vertices;

        for (int y=0; y<WORLD_HEIGHT-1; ++y) {
            const float lat = latitude_for_world_tile(y);
            const float lat1 = latitude_for_world_tile(y+1);
            const float phi = lat * TO_RADIANS;
            const float phi1 = lat1 * TO_RADIANS;
            for (int x = 0; x < WORLD_WIDTH - 1; ++x) {
                const float lon = longitude_for_world_tile(x);
                const float lon1 = longitude_for_world_tile(x+1);
                const int idx = planet.idx(x,y);
                const float altitude = 50.0f + ((*planet_builder_display.get())[idx].altitude/2.0f);
                //const float altitude = 50.0f;

                const float theta = lon * TO_RADIANS;
                const float theta1 = lon1 * TO_RADIANS;

                const float X = altitude * sin(phi) * cos(theta);
                const float Y = altitude * sin(phi) * sin(theta);
                const float Z = altitude * cos(phi);

                //std::cout << X << "/" << Y << "/" << Z << "\n";

                const float X1 = altitude * sin(phi1) * cos(theta1);
                const float Y1 = altitude * sin(phi1) * sin(theta1);
                const float Z1 = altitude * cos(phi1);

                vertices.emplace_back(X); vertices.emplace_back(Y); vertices.emplace_back(Z);
            }
        }
        //std::cout << "Points to render: " << vertices.size() << "\n";

        planet_builder_lock.unlock();

        glViewport(0, 0, screen_w, screen_h);
        glUseProgram(assets::worldgenshader);
        if (vbo == 0) glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glUniformMatrix4fv(projloc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view_matrix));
        glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model_matrix));

        glFlush();
        glDrawArrays(GL_POINTS, 0, vertices.size());

        glUseProgram(0);
    }

    double run_time = 0.0f;

    void tick(const double &duration_ms) {
        if (!initialized) init();

        ImGui_ImplGlfwGL3_NewFrame();

        if (mode == WG_MENU) {
            render_menu();
        } else {
            // Render status
            planet_builder_lock.lock();
            ImGui::Begin("World Generation Progress");
            ImGui::Text("%s",planet_builder_status.c_str());
            ImGui::End();
            planet_builder_lock.unlock();

            run_time += duration_ms;
            if (run_time > 33.0) {
                run_time = 0;
                world_spin += 0.01f;
                if (world_spin > 360.0f) world_spin = 0.0f;
            }
            render_globe();

            if (is_planet_build_complete()) {
                glDeleteBuffers(1, &vbo);
                bengine::main_func = main_menu::tick;
            }
        }

        ImGui::Render();
    }
}