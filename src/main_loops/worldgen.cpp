#include "stdafx.h"
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
#include "main_menu.hpp"
#include <glm/glm.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../render_engine/shaders/worldgen_shader.hpp"

namespace worldgen {
    enum world_gen_mode_t { WG_MENU, WG_RUNNING, WG_MAP };
    bool initialized = false;
    int seed;
    int plains = 3;
    int water = 3;
    int starting_settlers = 10;
    bool strict_beamdown = false;
	bool ascii_mode = false;
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
    float world_flatten_lerp = 0.0f;

    float world_spin = 0.0f;

    static void init() {
        call_home("WorldGen", "Opened");
        initialized = true;
        seed = rng.roll_dice(1, std::numeric_limits<int>::max());
        glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);
        if (vbo == 0) glGenBuffers(1, &vbo);
        if (vao == 0) glGenVertexArrays(1, &vao);
    }

    static void start_thread() {
        setup_build_planet();

        projection_matrix = glm::perspective(90.0, static_cast<double>(screen_w)/static_cast<double>(screen_h), 1.0, 300.0);
        view_matrix = glm::lookAt(glm::vec3(1.0f, 100.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_matrix = glm::mat4(); // Identity
		projloc = assets::worldgenshader->projection_matrix;
		viewloc = assets::worldgenshader->view_matrix;
		modelloc = assets::worldgenshader->model_matrix;
        assert(projloc != -1);
        assert(viewloc != -1);

        world_thread = std::make_unique<std::thread>(build_planet, seed, water, plains, starting_settlers, strict_beamdown, ascii_mode);
    }

    static void render_menu() {
        bengine::display_sprite(assets::background_image->texture_id, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
        ImGui::Begin("World Generation", nullptr, ImVec2{600,125}, 0.5f, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);

        ImGui::InputInt("World Seed", &seed);
        ImGui::SliderInt("Water Level", &water, 1, 4);
        ImGui::SliderInt("Plains Level", &plains, 1, 4);
        ImGui::SliderInt("Starting Settlers", &starting_settlers, 1, 20);
        ImGui::Checkbox("Require Teleport Beacon for Beamdown", &strict_beamdown);
		ImGui::Checkbox("Play in ASCII mode", &ascii_mode);
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
    constexpr float ALTITUDE_BASE = 60.0f;

    static void lerp_vertex(std::vector<float> &vertices, const int &world_x, const int &world_y, const int idx, const float &a, const float &b, const bool &rivers, const float &texture_id, const float &tx, const float &ty, const float &amount) {
		const int idx_real = planet.idx(world_x, world_y);
        const float altitude = ALTITUDE_BASE + ((*planet_builder_display)[idx_real].altitude/ALTITUDE_DIVISOR);
        const float x_sphere = altitude * cos(b);
        const float y_sphere = altitude * cos(a) * sin(b);
        const float z_sphere = altitude * sin(a) * sin(b);

        const float x_map = 90.0f - altitude;
        const float z_map = (static_cast<float>(world_x) + tx - (WORLD_WIDTH/2.0f)) * 1.5f;
        const float y_map = (static_cast<float>(world_y) + ty - (WORLD_HEIGHT/2.0f)) * 1.5f;

        const float xdiff = x_map - x_sphere;
        const float ydiff = y_map - y_sphere;
        const float zdiff = z_map - z_sphere;

        float x = x_sphere + (xdiff * amount);
        float y = y_sphere + (ydiff * amount);
        float z = z_sphere + (zdiff * amount);

        vertices.emplace_back(x); vertices.emplace_back(y); vertices.emplace_back(z);
        vertices.emplace_back(tx); vertices.emplace_back(ty); vertices.emplace_back(texture_id);
        vertices.emplace_back(rivers);
    }

    static void lerp_world_tile(std::vector<float> &vertices, const float &amount, const int &x, const int &y, const float &a, const float &b) {
        const auto idx = planet.idx(x,y);
        const auto texture_id = static_cast<float>((*planet_builder_display)[idx].texture_id);
        const auto rivers = (*planet_builder_display)[idx].rivers ? 1.0f : 0.0f;

        lerp_vertex(vertices, x, y, idx, a, b, rivers, texture_id, 0.0f, 0.0f, amount);
        lerp_vertex(vertices, x, y, planet.idx(x, y+1), a, b+latitude_per_tile, rivers, texture_id, 0.0f, 1.0f, amount);
        lerp_vertex(vertices, x, y, planet.idx(x+1, y+1), a+longitude_per_tile, b+latitude_per_tile, rivers, texture_id, 1.0f, 1.0f, amount);
        // Second triangle
        lerp_vertex(vertices, x, y, planet.idx(x+1, y+1), a+longitude_per_tile, b+latitude_per_tile, rivers, texture_id, 1.0f, 1.0f, amount);
        lerp_vertex(vertices, x, y, planet.idx(x+1, y), a+longitude_per_tile, b, rivers, texture_id, 1.0f, 0.0f, amount);
        lerp_vertex(vertices, x, y, idx, a, b, rivers, texture_id, 0.0f, 0.0f, amount);
    }

    static void build_globe_buffer() {
        planet_builder_lock.lock();

        std::vector<float> vertices;
        vertices.reserve(589824);

        auto y = 0;
        for (auto b=0.0f; b<=ONE_EIGHTY_RAD; b+=latitude_per_tile) {
            auto x = 0;
            for (auto a=0.0f; a<=THREE_SIXY_RAD; a+=longitude_per_tile) {
                lerp_world_tile(vertices, world_flatten_lerp, x, y, a, b);

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Send texture location
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), ((char *) nullptr + 3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Send terrain indicator
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), ((char *) nullptr + 6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        n_vertices = (int)vertices.size();
    }

    static void render_globe() {
        bengine::display_sprite(assets::starfield->texture_id, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f - world_flatten_lerp);

        model_matrix = glm::mat4();
        model_matrix = glm::rotate(model_matrix, world_spin, glm::vec3(1.0, 0.0, 0.0));

        glUseProgram(assets::worldgenshader->shader_id);
        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, assets::worldgen_texture_array);

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

    void tick(const double &duration_ms) noexcept {
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
            ImGui::Text("Rotation: %f degrees.", world_spin);
            ImGui::End();
            if (planet_builder_status == "Initializing starting settlements") mode = WG_MAP;
            planet_builder_lock.unlock();

            run_time += duration_ms;
            if (run_time > 33.0) {
                run_time = 0;
                if (mode == WG_RUNNING) {
                    world_spin += 0.01f;
                    if (world_spin > 360.0f) world_spin = 0.0f;
                }
                if (mode == WG_MAP) {
                    world_flatten_lerp += 0.02f;
                    if (world_flatten_lerp > 1.0f) world_flatten_lerp = 1.0f;
                }
            }
            render_globe();

            if (is_planet_build_complete()) {
                glDeleteBuffers(1, &vbo);
				main_menu::check_world_exists();
                bengine::main_func = main_menu::tick;
            }
        }

        ImGui::Render();
    }
}