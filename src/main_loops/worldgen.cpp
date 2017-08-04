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
#include "../global_assets/rng.hpp"

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
    unsigned int vao = 0;
    unsigned int projloc = -1;
    unsigned int viewloc = -1;
    unsigned int modelloc = -1;
    int screen_w, screen_h;
    int n_vertices = 0;

    float world_spin = 0.0f;

    void init() {
        call_home("WorldGen", "Opened");
        initialized = true;
        seed = rng.roll_dice(1, std::numeric_limits<int>::max());
        glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);
        if (vbo == 0) glGenBuffers(1, &vbo);
        if (vao == 0) glGenVertexArrays(1, &vao);
    }

    void start_thread() {
        setup_build_planet();

        assert(assets::worldgenshader != 0);
        projection_matrix = glm::perspective(90.0, (double)screen_w/(double)screen_h, 1.0, 300.0);
        view_matrix = glm::lookAt(glm::vec3(1.0f, 100.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_matrix = glm::mat4(); // Identity
        projloc = glGetUniformLocation(assets::worldgenshader, "projection_mat");
        viewloc = glGetUniformLocation(assets::worldgenshader, "view");
        modelloc = glGetUniformLocation(assets::worldgenshader, "model");
        assert(projloc != -1);
        assert(viewloc != -1);

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

    constexpr float TO_RADIANS = 0.0174533;
    constexpr float latitude_per_tile = (180.0f / WORLD_HEIGHT) * TO_RADIANS;
    constexpr float longitude_per_tile = (360.0f / WORLD_WIDTH) * TO_RADIANS;
    constexpr float ONE_EIGHTY_RAD = 180.0f * TO_RADIANS;
    constexpr float THREE_SIXY_RAD = 360.0f * TO_RADIANS;
    constexpr float ALTITUDE_DIVISOR = 3.0f;
    constexpr float ALTITUDE_BASE = 50.0f;

    void build_globe_buffer() {
        planet_builder_lock.lock();

        std::vector<float> vertices;
        vertices.reserve(589824);

        int y = 0;
        for (float b=0.0f; b<=ONE_EIGHTY_RAD; b+=latitude_per_tile) {
            int x = 0;
            for (float a=0.0f; a<=THREE_SIXY_RAD; a+=longitude_per_tile) {
                const int idx = planet.idx(x,y);
                const float texture_id = (*planet_builder_display.get())[idx].texture_id;

                float altitude = ALTITUDE_BASE + ((*planet_builder_display.get())[idx].altitude/ALTITUDE_DIVISOR);
                vertices.emplace_back(altitude * cos(b));
                vertices.emplace_back(altitude * cos(a) * sin(b));
                vertices.emplace_back(altitude * sin(a) * sin(b));
                vertices.emplace_back(0.0f); vertices.emplace_back(0.0f); vertices.emplace_back(texture_id);

                altitude = ALTITUDE_BASE + ((*planet_builder_display.get())[planet.idx(x,y+1)].altitude/ALTITUDE_DIVISOR);
                vertices.emplace_back(altitude * cos(b + latitude_per_tile));
                vertices.emplace_back(altitude * cos(a) * sin(b + latitude_per_tile));
                vertices.emplace_back(altitude * sin(a) * sin(b + latitude_per_tile));
                vertices.emplace_back(0.0f); vertices.emplace_back(1.0f); vertices.emplace_back(texture_id);

                altitude = ALTITUDE_BASE + ((*planet_builder_display.get())[planet.idx(x+1,y+1)].altitude/ALTITUDE_DIVISOR);
                vertices.emplace_back(altitude * cos(b + latitude_per_tile));
                vertices.emplace_back(altitude * cos(a + longitude_per_tile) * sin(b + latitude_per_tile));
                vertices.emplace_back(altitude * sin(a + longitude_per_tile) * sin(b + latitude_per_tile));
                vertices.emplace_back(1.0f); vertices.emplace_back(1.0f); vertices.emplace_back(texture_id);

                // Second triangle
                altitude = ALTITUDE_BASE + ((*planet_builder_display.get())[planet.idx(x+1,y+1)].altitude/ALTITUDE_DIVISOR);
                vertices.emplace_back(altitude * cos(b + latitude_per_tile));
                vertices.emplace_back(altitude * cos(a + longitude_per_tile) * sin(b + latitude_per_tile));
                vertices.emplace_back(altitude * sin(a + longitude_per_tile) * sin(b + latitude_per_tile));
                vertices.emplace_back(1.0f); vertices.emplace_back(1.0f); vertices.emplace_back(texture_id);

                altitude = ALTITUDE_BASE + ((*planet_builder_display.get())[planet.idx(x+1,y)].altitude/ALTITUDE_DIVISOR);
                vertices.emplace_back(altitude * cos(b));
                vertices.emplace_back(altitude * cos(a + longitude_per_tile) * sin(b));
                vertices.emplace_back(altitude * sin(a + longitude_per_tile) * sin(b));
                vertices.emplace_back(1.0f); vertices.emplace_back(0.0f); vertices.emplace_back(texture_id);

                altitude = ALTITUDE_BASE + ((*planet_builder_display.get())[idx].altitude/ALTITUDE_DIVISOR);
                vertices.emplace_back(altitude * cos(b));
                vertices.emplace_back(altitude * cos(a) * sin(b));
                vertices.emplace_back(altitude * sin(a) * sin(b));
                vertices.emplace_back(0.0f); vertices.emplace_back(0.0f); vertices.emplace_back(texture_id);

                ++x;
            }
            ++y;
        }

        planet_builder_lock.unlock();

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        glBindVertexArray(vao);

        // Send location
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Send texture location
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), ((char *) nullptr + 3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        n_vertices = (int)vertices.size();
    }

    void render_globe() {
        bengine::display_sprite(assets::starfield->texture_id);

        model_matrix = glm::mat4();
        model_matrix = glm::rotate(model_matrix, world_spin, glm::vec3(1.0, 0.0, 0.0));

        glUseProgram(assets::worldgenshader);
        glBindVertexArray(vao);

        glUniformMatrix4fv(projloc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view_matrix));
        glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model_matrix));

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glDrawArrays(GL_TRIANGLES, 0, n_vertices/3);
        glDisable(GL_DEPTH_TEST);
        glUseProgram(0);
    }

    double run_time = 0.0f;

    void tick(const double &duration_ms) {
        if (!initialized) init();

        ImGui_ImplGlfwGL3_NewFrame();

        if (mode == WG_MENU) {
            render_menu();
        } else {
            build_globe_buffer();

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