#include "splash_screen.hpp"
#include "../global_assets/texture_storage.hpp"
#include "../bengine/textures.hpp"
#include "../global_assets/shader_storage.hpp"
#include "../bengine/shaders.hpp"
#include "../bengine/simple_sprite.hpp"
#include "../bengine/imgui.h"
#include "../bengine/imgui_impl_glfw_gl3.h"
#include "../bengine/threadpool.h"
#include "../raws/raws.hpp"
#include "../raws/biomes.hpp"
#include "../bengine/main_window.hpp"
#include "main_menu.hpp"
#include "../bengine/telemetry.hpp"
#include "../global_assets/game_config.hpp"
#include "../bengine/stb_image.h"
#include <iostream>
#include <sstream>

using namespace bengine;
using namespace assets;

namespace splash_screen {
    double run_time = 0.0;
    float scale = 0.0f;
    float angle = 6.28319f / 2.0f;
    constexpr float angle_step = 0.0174533f * 5.0f;
    float darken = 0.0f;

    bool initialized_thread_pool = false;
    std::atomic<bool> initialized_raws{false};
    std::atomic<bool> raw_load_started{false};
    bool loaded_textures = false;
    bool started_telemetry = false;
    bool sent_telemetry = false;
    bool loaded_worldgen = false;
    int tex_idx = 0;

    /* Loads enough to get things started. */
    void init() {
        bracket_logo = std::make_unique<texture_t>("game_assets/bracket-logo.jpg");
        spriteshader = load_shaders("game_assets/spriteshader_vertex.glsl", "game_assets/spriteshader_fragment.glsl");
        init_simple_sprite();
        worldgenshader = load_shaders("game_assets/worldgenshader_vertex.glsl", "game_assets/worldgenshader_fragment.glsl");
    }

    void init_raws(int id) {
        std::cout << "Seen thread " << id << "\n";
        load_raws();
        initialized_raws.store(true);
    }

    void load_worldgen_textures() {
        glGenTextures(1, &assets::worldgen_texture_array);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, assets::worldgen_texture_array);

        glTexStorage3D( GL_TEXTURE_2D_ARRAY,
                        8, // 8-levels of mipmap
                        GL_RGBA8, // Internal format
                        128, 128, // Width and height
                        biome_textures.size()
        );

        for (unsigned int i=0; i<biome_textures.size(); ++i) {
            int width, height, bpp;
            stbi_set_flip_vertically_on_load(true);
            unsigned char *image_data = stbi_load(biome_textures[i].c_str(), &width, &height, &bpp, STBI_rgb_alpha);
            if (image_data == nullptr) throw std::runtime_error(std::string("Cannot open: ") + std::string(biome_textures[i]));

            glTexSubImage3D(
                GL_TEXTURE_2D_ARRAY,
                0, // Mipmap number
                0, 0, i, // x/y/z offsets
                128, 128, 1, // width, height, depth
                GL_RGBA, // format
                GL_UNSIGNED_BYTE, // type
                image_data // Color data
            );

            stbi_image_free(image_data);
        }

        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    }

    void tick(const double &duration_ms) {
        run_time += duration_ms;
        if (run_time > 0.1 && run_time < 500.0f) {
            scale = std::min(0.5f, scale + 0.01f);
            angle -= angle_step;
            if (angle < 0) angle = 0.0f;
            darken += 0.01f;
            if (darken > 1.0f) darken = 1.0f;
        } else {
            darken -= 0.01f;
            if (darken < 0.0f) darken = 0.0f;
        }

        display_sprite(bracket_logo->texture_id, scale, scale, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, angle, darken);

        ImGui_ImplGlfwGL3_NewFrame();
        ImGui::Begin("Nox Futura is loading", nullptr, ImVec2{600,125}, 0.5f, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
        if (initialized_thread_pool) {
            ImGui::BulletText("%s", "Initialized thread pool");
        } else {
            ImGui::BulletText("%s", "Initializing thread pool");
        }
        if (initialized_raws) {
            ImGui::BulletText("%s", "Loaded RAW files");
        } else {
            ImGui::BulletText("%s", "Loading RAW files");
        }
        if (!loaded_textures) {
            ImGui::BulletText("%s", "Loading game images");
        } else {
            ImGui::BulletText("%s", "Loaded game images");
        }
        if (!started_telemetry) {
            ImGui::BulletText("%s", "Starting telemetry");
        } else {
            ImGui::BulletText("%s", "Started telemetry (if permitted)");
        }
        if (!loaded_worldgen) {
            ImGui::BulletText("%s", "Loading worldgen textures");
        } else {
            ImGui::BulletText("%s", "Loaded worldgen textures");
        }

        ImGui::End();
        ImGui::Render();

        if (!initialized_thread_pool) {
            initialized_thread_pool = true;
            init_thread_pool();
        } else {
            if (!initialized_raws && !raw_load_started) {
                raw_load_started.store(true);
                thread_pool->push(std::ref(init_raws));
            }
        }

        if (!loaded_textures) {
            switch (tex_idx) {
                case 0 : background_image = std::make_unique<texture_t>("game_assets/background_image.jpg"); break;
                case 1 : game_logo = std::make_unique<texture_t>("game_assets/gamelogo.png"); break;
                case 2 : starfield = std::make_unique<texture_t>("game_assets/starfield.jpg"); break;
                default: loaded_textures = true;
            }
            ++tex_idx;
        }

        if (!started_telemetry) {
            start_telemetry();
            started_telemetry = true;
        } else if (!sent_telemetry) {
            call_home("Startup");
            call_home("Startup-FullScreen", std::to_string(config::game_config.fullscreen));
            call_home("Startup-Resolution", std::to_string(config::game_config.window_width) + std::string("x") + std::to_string(config::game_config.window_height));
            call_home("Startup-GUI-Font", config::game_config.gui_ttf + std::string(" / ") + std::to_string(config::game_config.gui_ttf_size));
            call_home("Startup-Scaling", std::to_string(config::game_config.scale_factor));
            sent_telemetry = true;
        }

        if (initialized_raws && !loaded_worldgen) {
            load_worldgen_textures();
            loaded_worldgen = true;
        }

        if (initialized_thread_pool && initialized_raws && loaded_textures && angle==0.0f && started_telemetry && sent_telemetry && loaded_worldgen) {
            // We're done - time to move on!
            main_func = main_menu::tick;
        }
    }
}