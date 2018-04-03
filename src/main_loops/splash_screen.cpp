#include "stdafx.h"
#include "splash_screen.hpp"
#include "../global_assets/texture_storage.hpp"
#include "../bengine/textures.hpp"
#include "../global_assets/shader_storage.hpp"
#include "../bengine/simple_sprite.hpp"
#include "../bengine/imgui.h"
#include "../bengine/imgui_impl_glfw_gl3.h"
#include "../raws/raws.hpp"
#include "../raws/biomes.hpp"
#include "../bengine/main_window.hpp"
#include "main_menu.hpp"
#include "../bengine/telemetry.hpp"
#include "../global_assets/game_config.hpp"
#include "../bengine/stb_image.h"
#include "../bengine/stb_image_resize.h"
#include "../raws/materials.hpp"
#include "../render_engine/vox/voxreader.hpp"
#include <boost/filesystem/operations.hpp>
#include <thread>

using namespace bengine;
using namespace assets;

namespace splash_screen {
    static double run_time = 0.0;
    static float scale = 0.0f;
    static float angle = 6.28319f / 2.0f;
    static constexpr float angle_step = 0.0174533f * 5.0f;
    static float darken = 0.0f;

    static bool initialized_thread_pool = false;
    static std::atomic<bool> initialized_raws{false};
    static std::atomic<bool> raw_load_started{false};
    static bool loaded_textures = false;
    static bool started_telemetry = false;
    static bool sent_telemetry = false;
    static bool loaded_worldgen = false;
    static bool loaded_chunk = false;
    static int tex_idx = 0;

    /* Loads enough to get things started. */
    void init() noexcept {
        init_simple_sprite();
        bracket_logo = std::make_unique<texture_t>("game_assets/bracket-logo.jpg");
        ascii_texture = std::make_unique<texture_t>("game_assets/hack_square_64x64.jpg", false);
		assets::initialize_shaders();
	}

    static void init_raws() {
        load_raws();
        initialized_raws.store(true);
    }

    static inline void load_worldgen_textures() {
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

    constexpr int CURSOR_SIZE = 128;

    static inline std::tuple<unsigned char *, int, int, int> load_texture_to_ram(const std::string &filename) {
        int width, height, bpp;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *image_data = stbi_load(filename.c_str(), &width, &height, &bpp, STBI_rgb);
        if (image_data == nullptr) throw std::runtime_error(std::string("Cannot open: ") + filename);
        //std::cout << "Loaded " << filename << ", " << width << ", " << height << ", " << bpp << "\n";
        //assert(width == TEX_SIZE && height == TEX_SIZE);

		if (width != config::game_config.texture_size || height != config::game_config.texture_size)
		{
			// Resize the image
			unsigned char * new_image_data = (unsigned char *)malloc(config::game_config.texture_size * config::game_config.texture_size * 3);
			stbir_resize(image_data, width, height, 0, new_image_data, config::game_config.texture_size, config::game_config.texture_size, 0, STBIR_TYPE_UINT8, 3, 0, 0, STBIR_EDGE_ZERO, STBIR_EDGE_ZERO, STBIR_FILTER_CUBICBSPLINE, STBIR_FILTER_CUBICBSPLINE, STBIR_COLORSPACE_SRGB, nullptr);
			stbi_image_free(image_data);
			image_data = new_image_data;
		}

        return std::make_tuple(image_data, width, height, bpp);
    }

	static inline std::tuple<unsigned char *, int, int, int> load_cursor_texture_to_ram(const std::string &filename) {
        int width, height, bpp;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *image_data = stbi_load(filename.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
        if (image_data == nullptr) throw std::runtime_error(std::string("Cannot open: ") + filename);
        //std::cout << "Loaded " << filename << ", " << width << ", " << height << ", " << bpp << "\n";
        assert(width == CURSOR_SIZE && height == CURSOR_SIZE);
        return std::make_tuple(image_data, width, height, bpp);
    }

	static inline void load_chunk_textures() {
        const int num_actual_textures = static_cast<int>(material_textures.size() * 3);
        //std::cout << "# Textures in array: " << num_actual_textures << "\n";

        glGenTextures(1, &assets::chunk_texture_array);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, assets::chunk_texture_array);

		const auto max_mip_levels = std::min(static_cast<int>(1 + std::floor(std::log2(config::game_config.texture_size))), GL_TEXTURE_MAX_LEVEL-1);
		const auto n_mip_levels = config::game_config.mip_levels == 0 ? max_mip_levels : config::game_config.mip_levels;

        glTexStorage3D( GL_TEXTURE_2D_ARRAY,
			n_mip_levels, // 4-levels of mipmap
            GL_SRGB8, // Internal format
			config::game_config.texture_size, config::game_config.texture_size, // Width and height
            num_actual_textures
        );

        auto load_index = 0;
        for (std::size_t i=0; i<material_textures.size(); ++i) {
            const auto stem = material_textures[i];
            const auto albedo = stem + "-t.jpg";
            const auto normal = stem + "-n.jpg";
            const auto occlusion = stem + "-ao.jpg";
            const auto metal = stem + "-m.jpg";
            const auto rough = stem + "-r.jpg";

            auto albedo_tex = load_texture_to_ram(albedo);
            auto normal_tex = load_texture_to_ram(normal);
            auto occlusion_tex = load_texture_to_ram(occlusion);

			std::tuple<unsigned char *, int, int, int> metal_tex;
			if (boost::filesystem::exists(metal)) {
				metal_tex = load_texture_to_ram(metal);
			} else
			{
				metal_tex = load_texture_to_ram("game_assets/terrain/metal-template.jpg");
			}
            auto rough_tex = load_texture_to_ram(rough);

            // Albedo and normal are stored directly as idx+0, idx+1
            glTexSubImage3D(
                GL_TEXTURE_2D_ARRAY,
                0, // Mipmap number
                0, 0, load_index, // x/y/z offsets
				config::game_config.texture_size, config::game_config.texture_size, 1, // width, height, depth
                GL_RGB, // format
                GL_UNSIGNED_BYTE, // type
                std::get<0>(albedo_tex) // Color data
            );
            //std::cout << albedo << " = " << load_index << "\n";
            glTexSubImage3D(
                GL_TEXTURE_2D_ARRAY,
                0, // Mipmap number
                0, 0, load_index+1, // x/y/z offsets
				config::game_config.texture_size, config::game_config.texture_size, 1, // width, height, depth
                GL_RGB, // format
                GL_UNSIGNED_BYTE, // type
                std::get<0>(normal_tex) // Color data
            );
            //std::cout << normal << " = " << load_index+1 << "\n";

            // We need to combine occlusion, metal and rough into one texture
            std::vector<uint8_t> texbytes;
            const int num_bytes = config::game_config.texture_size * config::game_config.texture_size * 3;
            texbytes.resize(num_bytes);
            for (int i=0; i<num_bytes; i+=3) {
                texbytes[i] = std::get<0>(occlusion_tex)[i];
                texbytes[i+1] = std::get<0>(metal_tex)[i];
                texbytes[i+2] = std::get<0>(rough_tex)[i];
            }
            glTexSubImage3D(
                GL_TEXTURE_2D_ARRAY,
                0, // Mipmap number
                0, 0, load_index+2, // x/y/z offsets
				config::game_config.texture_size, config::game_config.texture_size, 1, // width, height, depth
                GL_RGB, // format
                GL_UNSIGNED_BYTE, // type
                &texbytes[0] // Color data
            );
            //std::cout << "Combined = " << load_index+2 << "\n";

            stbi_image_free(std::get<0>(albedo_tex));
            stbi_image_free(std::get<0>(normal_tex));
            stbi_image_free(std::get<0>(occlusion_tex));
            stbi_image_free(std::get<0>(metal_tex));
            stbi_image_free(std::get<0>(rough_tex));

            load_index += 3;
        }

        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    }

	static inline void load_cursor_textures() {
        std::vector<std::string> cursor_textures{ "base_cursor.png", "chop_cursor.png", "tree_cursor.png", "farm_cursor.png", "guard_cursor.png",
                                                  "dig_cursor.png", "channel_cursor.png", "ramp_cursor.png", "downstairs_cursor.png", "upstairs_cursor.png", "updownstairs_cursor.png",
                                                  "wall_cursor.png", "floor_cursor.png", "bridge_cursor.png"};
        const auto num_actual_textures = static_cast<int>(cursor_textures.size());
        //std::cout << "# CursorTextures in array: " << num_actual_textures << "\n";

        glGenTextures(1, &assets::cursor_texture_array);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, assets::cursor_texture_array);

        glTexStorage3D(GL_TEXTURE_2D_ARRAY,
                       2, // 4-levels of mipmap
                       GL_RGBA8, // Internal format
                       CURSOR_SIZE, CURSOR_SIZE, // Width and height
                       num_actual_textures
        );

        auto load_index = 0;
        for (std::size_t i = 0; i<cursor_textures.size(); ++i) {
            const auto stem = cursor_textures[i];
            const auto albedo = "game_assets/cursors/" + stem;

            auto albedo_tex = load_cursor_texture_to_ram(albedo);

            // Albedo and normal are stored directly as idx+0, idx+1
            glTexSubImage3D(
                    GL_TEXTURE_2D_ARRAY,
                    0, // Mipmap number
                    0, 0, load_index, // x/y/z offsets
                    CURSOR_SIZE, CURSOR_SIZE, 1, // width, height, depth
                    GL_RGBA, // format
                    GL_UNSIGNED_BYTE, // type
                    std::get<0>(albedo_tex) // Color data
            );
            //std::cout << albedo << " = " << load_index << "\n";

            stbi_image_free(std::get<0>(albedo_tex));

            load_index += 1;
        }

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    }

	static inline void load_voxel_models() {

        for (const auto &vm : voxel_models_to_load) {
            vox::load_vox(vm.second, vm.first);
        }
		vox::build_master_geometry();
    }

    void tick(const double &duration_ms) noexcept {
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

        display_sprite(bracket_logo->texture_id, scale, scale, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

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

        ImGui::End();
        ImGui::Render();

        if (!initialized_thread_pool) {
            initialized_thread_pool = true;
        } else {
            if (!initialized_raws && !raw_load_started) {
                raw_load_started.store(true);
				std::thread loader_thread(std::ref(init_raws));
				loader_thread.detach();
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
            call_home("game", "Startup");
            call_home("game", "Startup-FullScreen", std::to_string(config::game_config.fullscreen));
            call_home("game", "Startup-Resolution", std::to_string(config::game_config.window_width) + std::string("x") + std::to_string(config::game_config.window_height));
            call_home("game", "Startup-GUI-Font", config::game_config.gui_ttf + std::string(" / ") + std::to_string(config::game_config.gui_ttf_size));
            call_home("game", "Startup-Scaling", std::to_string(config::game_config.scale_factor));
            sent_telemetry = true;
        }

        if (initialized_raws && !loaded_worldgen) {
            load_worldgen_textures();
            loaded_worldgen = true;
        }

        if (loaded_worldgen && !loaded_chunk) {
            load_chunk_textures();
            load_voxel_models();
            load_cursor_textures();
            loaded_chunk = true;
        }

        if (initialized_thread_pool && initialized_raws && loaded_textures && started_telemetry
            && sent_telemetry && loaded_worldgen && loaded_chunk) {
            // We're done - time to move on!
            main_func = main_menu::tick;
        }
    }
}