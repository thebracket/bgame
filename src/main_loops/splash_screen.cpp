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
#include "../stdafx.h"
#include "asset_loader.hpp"
#include "../bengine/random_number_generator.hpp"

using namespace bengine;
using namespace assets;

namespace splash_screen {

    bool initialized_thread_pool = false;
    std::atomic<bool> initialized_raws{false};
    std::atomic<bool> raw_load_started{false};        
    int tex_idx = 0;
	random_number_generator splash_rng;

    /* Loads enough to get things started. */
    void init() {
        bracket_logo = std::make_unique<texture_t>("game_assets/bracket-logo.jpg");
		ascii_texture = std::make_unique<texture_t>("game_assets/hack_square_64x64.png");
		bracket = std::make_unique<texture_t>("game_assets/bracket.jpg");
        spriteshader = load_shaders("game_assets/spriteshader_vertex.glsl", "game_assets/spriteshader_fragment.glsl");
        init_simple_sprite();
        worldgenshader = load_shaders("game_assets/worldgenshader_vertex.glsl", "game_assets/worldgenshader_fragment.glsl");
		chunkshader = std::make_unique<chunk_shader_t>();
        depthquad_shader = load_shaders("game_assets/depthquad_vertex.glsl", "game_assets/depthquad_fragment.glsl");
		lightstage_shader = std::make_unique<lightstage_shader_t>();
        tonemap_shader = load_shaders("game_assets/tonemap_vertex.glsl", "game_assets/tonemap_fragment.glsl");
        bloom_shader = load_shaders("game_assets/bloom_vertex.glsl", "game_assets/bloom_fragment.glsl");
		sprite_shader = load_shaders("game_assets/sprite_shader_vertex.glsl", "game_assets/sprite_shader_fragment.glsl");
		voxel_shader = std::make_unique<voxel_shader_t>();
		voxel_shadow_shader = std::make_unique<voxel_shadow_shader_t>();
		cursor_shader = load_shaders("game_assets/highlight_vertex.glsl", "game_assets/highlight_fragment.glsl");
		particle_shader = load_shaders("game_assets/particle_vertex.glsl", "game_assets/particle_fragment.glsl");
		dirlight_shader = load_shaders("game_assets/dirlight_vertex.glsl", "game_assets/dirlight_fragment.glsl");
	}

    void init_raws(int id) {
        std::cout << "RAW INIT - Seen thread " << id << "\n";
        load_raws();
        initialized_raws.store(true);
		std::cout << "RAW INIT DONE\n";
    }    

	static bool coroutine_continues = true;

    void tick(const double &duration_ms) {

		for (int i = 0; i < 128; ++i) {
			const float x = ((float)splash_rng.roll_dice(1, 10000) / 5000.0f) - 1.0f;
			const float y = ((float)splash_rng.roll_dice(1, 10000) / 5000.0f) - 1.0f;
			const float dark = (float)splash_rng.roll_dice(1, 256) / 256.0f;
			bengine::display_sprite(bracket->texture_id, 0.01f, 0.01f, x, y, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, dark);
		}
		bengine::display_sprite(bracket_logo->texture_id, 0.4f, 0.2f, 0.5f, 0.7f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);

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
        if (!loaded_chunk) {
            ImGui::BulletText("%s", "Loading terrain textures");
        } else {
            ImGui::BulletText("%s", "Loaded terrain textures");
        }
		ImGui::BulletText("Loaded %d Voxel Models", loaded_vox_count);

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

		if (initialized_raws) {
			if (coroutine_continues) coroutine_continues = loader_tick();
		}

        if (initialized_thread_pool && initialized_raws && started_telemetry
                && sent_telemetry && loaded_worldgen && loaded_chunk && !coroutine_continues) {
            // We're done - time to move on!
            main_func = main_menu::tick;
        }
    }
}