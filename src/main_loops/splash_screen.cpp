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
#include "../raws/materials.hpp"
#include "../render_engine/vox/voxreader.hpp"
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
    bool loaded_chunk = false;
    int tex_idx = 0;

    /* Loads enough to get things started. */
    void init() {
        bracket_logo = std::make_unique<texture_t>("game_assets/bracket-logo.jpg");
        spriteshader = load_shaders("game_assets/spriteshader_vertex.glsl", "game_assets/spriteshader_fragment.glsl");
        init_simple_sprite();
        worldgenshader = load_shaders("game_assets/worldgenshader_vertex.glsl", "game_assets/worldgenshader_fragment.glsl");
		chunkshader = std::make_unique<chunk_shader_t>();
        depthquad_shader = load_shaders("game_assets/depthquad_vertex.glsl", "game_assets/depthquad_fragment.glsl");
        lightstage_shader = load_shaders("game_assets/lightpass_vertex.glsl", "game_assets/lightpass_fragment.glsl");
        tonemap_shader = load_shaders("game_assets/tonemap_vertex.glsl", "game_assets/tonemap_fragment.glsl");
        bloom_shader = load_shaders("game_assets/bloom_vertex.glsl", "game_assets/bloom_fragment.glsl");
		sprite_shader = load_shaders("game_assets/sprite_shader_vertex.glsl", "game_assets/sprite_shader_fragment.glsl");
		voxel_shader = std::make_unique<voxel_shader_t>();
		cursor_shader = load_shaders("game_assets/highlight_vertex.glsl", "game_assets/highlight_fragment.glsl");
		particle_shader = load_shaders("game_assets/particle_vertex.glsl", "game_assets/particle_fragment.glsl");
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

    constexpr int TEX_SIZE = 256; // This is probably too high
	constexpr int MOB_SIZE = 512;
	constexpr int CURSOR_SIZE = 128;

    std::tuple<unsigned char *, int, int, int> load_texture_to_ram(const std::string filename) {
        int width, height, bpp;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *image_data = stbi_load(filename.c_str(), &width, &height, &bpp, STBI_rgb);
        if (image_data == nullptr) throw std::runtime_error(std::string("Cannot open: ") + filename);
        std::cout << "Loaded " << filename << ", " << width << ", " << height << ", " << bpp << "\n";
        assert(width == TEX_SIZE && height == TEX_SIZE);
        return std::make_tuple(image_data, width, height, bpp);
    }

	std::tuple<unsigned char *, int, int, int> load_mobtexture_to_ram(const std::string filename) {
		int width, height, bpp;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *image_data = stbi_load(filename.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
		if (image_data == nullptr) throw std::runtime_error(std::string("Cannot open: ") + filename);
		std::cout << "Loaded " << filename << ", " << width << ", " << height << ", " << bpp << "\n";
		assert(width == MOB_SIZE && height == MOB_SIZE);
		return std::make_tuple(image_data, width, height, bpp);
	}

	std::tuple<unsigned char *, int, int, int> load_cursor_texture_to_ram(const std::string filename) {
		int width, height, bpp;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *image_data = stbi_load(filename.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
		if (image_data == nullptr) throw std::runtime_error(std::string("Cannot open: ") + filename);
		std::cout << "Loaded " << filename << ", " << width << ", " << height << ", " << bpp << "\n";
		assert(width == CURSOR_SIZE && height == CURSOR_SIZE);
		return std::make_tuple(image_data, width, height, bpp);
	}

    void load_chunk_textures() {
        const int num_actual_textures = static_cast<int>(material_textures.size() * 3);
		std::cout << "# Textures in array: " << num_actual_textures << "\n";

        glGenTextures(1, &assets::chunk_texture_array);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, assets::chunk_texture_array);

        glTexStorage3D( GL_TEXTURE_2D_ARRAY,
                        2, // 4-levels of mipmap
                        GL_RGB8, // Internal format
                        TEX_SIZE, TEX_SIZE, // Width and height
                        num_actual_textures
        );

        int load_index = 0;
        for (std::size_t i=0; i<material_textures.size(); ++i) {
            const std::string stem = material_textures[i];
            const std::string albedo = stem + "-t.jpg";
            const std::string normal = stem + "-n.jpg";
            const std::string occlusion = stem + "-ao.jpg";
            const std::string metal = stem + "-m.jpg";
            const std::string rough = stem + "-r.jpg";

            auto albedo_tex = load_texture_to_ram(albedo);
            auto normal_tex = load_texture_to_ram(normal);
            auto occlusion_tex = load_texture_to_ram(occlusion);
            auto metal_tex = load_texture_to_ram(metal);
            auto rough_tex = load_texture_to_ram(rough);

            // Albedo and normal are stored directly as idx+0, idx+1
            glTexSubImage3D(
                    GL_TEXTURE_2D_ARRAY,
                    0, // Mipmap number
                    0, 0, load_index, // x/y/z offsets
                    TEX_SIZE, TEX_SIZE, 1, // width, height, depth
                    GL_RGB, // format
                    GL_UNSIGNED_BYTE, // type
                    std::get<0>(albedo_tex) // Color data
            );
			std::cout << albedo << " = " << load_index << "\n";
            glTexSubImage3D(
                    GL_TEXTURE_2D_ARRAY,
                    0, // Mipmap number
                    0, 0, load_index+1, // x/y/z offsets
                    TEX_SIZE, TEX_SIZE, 1, // width, height, depth
                    GL_RGB, // format
                    GL_UNSIGNED_BYTE, // type
                    std::get<0>(normal_tex) // Color data
            );
			std::cout << normal << " = " << load_index+1 << "\n";

            // We need to combine occlusion, metal and rough into one texture
            std::vector<uint8_t> texbytes;
            constexpr int num_bytes = TEX_SIZE * TEX_SIZE * 3;
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
                    TEX_SIZE, TEX_SIZE, 1, // width, height, depth
                    GL_RGB, // format
                    GL_UNSIGNED_BYTE, // type
                    &texbytes[0] // Color data
            );
			std::cout << "Combined = " << load_index+2 << "\n";

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

	void load_mob_textures() {
		const int num_actual_textures = static_cast<int>(mob_textures.size());
		std::cout << "# MobTextures in array: " << num_actual_textures << "\n";

		glGenTextures(1, &assets::mob_texture_array);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, assets::mob_texture_array);

		glTexStorage3D(GL_TEXTURE_2D_ARRAY,
			2, // 4-levels of mipmap
			GL_RGBA8, // Internal format
			MOB_SIZE, MOB_SIZE, // Width and height
			num_actual_textures
		);

		int load_index = 0;
		for (std::size_t i = 0; i<mob_textures.size(); ++i) {
			const std::string stem = mob_textures[i];
			const std::string albedo = "game_assets/mobs/original/" + stem;

			auto albedo_tex = load_mobtexture_to_ram(albedo);

			// Albedo and normal are stored directly as idx+0, idx+1
			glTexSubImage3D(
				GL_TEXTURE_2D_ARRAY,
				0, // Mipmap number
				0, 0, load_index, // x/y/z offsets
				MOB_SIZE, MOB_SIZE, 1, // width, height, depth
				GL_RGBA, // format
				GL_UNSIGNED_BYTE, // type
				std::get<0>(albedo_tex) // Color data
			);
			std::cout << albedo << " = " << load_index << "\n";

			stbi_image_free(std::get<0>(albedo_tex));

			load_index += 1;
		}

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	}

	void load_cursor_textures() {
		std::vector<std::string> cursor_textures{ "base_cursor.png", "chop_cursor.png", "tree_cursor.png" };
		const int num_actual_textures = static_cast<int>(cursor_textures.size());
		std::cout << "# CursorTextures in array: " << num_actual_textures << "\n";

		glGenTextures(1, &assets::cursor_texture_array);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, assets::cursor_texture_array);

		glTexStorage3D(GL_TEXTURE_2D_ARRAY,
			2, // 4-levels of mipmap
			GL_RGBA8, // Internal format
			CURSOR_SIZE, CURSOR_SIZE, // Width and height
			num_actual_textures
		);

		int load_index = 0;
		for (std::size_t i = 0; i<cursor_textures.size(); ++i) {
			const std::string stem = cursor_textures[i];
			const std::string albedo = "game_assets/cursors/" + stem;

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
			std::cout << albedo << " = " << load_index << "\n";

			stbi_image_free(std::get<0>(albedo_tex));

			load_index += 1;
		}

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	}

    void load_voxel_models() {
		
		/*
        vox::load_vox("game_assets/vox/cryobed32.vox", "cryobed");
        vox::load_vox("game_assets/vox/floorlight.vox", "floorlight");
        vox::load_vox("game_assets/vox/rtg.vox", "rtg");
        vox::load_vox("game_assets/vox/cabinet.vox", "cabinet");
        vox::load_vox("game_assets/vox/battery.vox", "battery");
        vox::load_vox("game_assets/vox/cordex.vox", "cordex");
        vox::load_vox("game_assets/vox/person-base.vox", "person-base");
        vox::load_vox("game_assets/vox/replicator-small.vox", "replicator-small");
        vox::load_vox("game_assets/vox/solarpanel.vox", "solarpanel");
		vox::load_vox("game_assets/vox/person-hair-short.vox", "shorthair");
		vox::load_vox("game_assets/vox/person-hair-long.vox", "longhair");
		vox::load_vox("game_assets/vox/person-hair-pigtails.vox", "pigtails");
		vox::load_vox("game_assets/vox/person-hair-mohawk.vox", "mohawk");
		*/

		for (const auto &vm : voxel_models_to_load) {
			vox::load_vox(vm.second, vm.first);
		}
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

        if (loaded_worldgen && !loaded_chunk) {
            load_chunk_textures();
            load_voxel_models();
			load_mob_textures();
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