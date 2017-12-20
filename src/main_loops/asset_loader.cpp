#include "asset_loader.hpp"
#include "../stdafx.h"
#include "../raws/materials.hpp"
#include "../render_engine/vox/voxreader.hpp"
#include "../global_assets/texture_storage.hpp"
#include "../bengine/textures.hpp"
#include "../bengine/telemetry.hpp"
#include "../global_assets/game_config.hpp"
#include "../bengine/stb_image.h"
#include "../raws/biomes.hpp"
#include "../utils/coroutine_generator.hpp"
#include <experimental/coroutine>

namespace splash_screen {	

	enum LOADER_SECTION { TELEMETRY, SEND_TELEMETRY, STATIC_TEXTURES, WORLDGEN_TEXTURES, CHUNK_TEXTURES, MOB_TEXTURES, CURSOR_TEXTURES, VOXELS, DONE };
	static LOADER_SECTION loader_section = TELEMETRY;
	static std::size_t loader_subsection = 0;
	bool loaded_textures = false;
	bool started_telemetry = false;
	bool sent_telemetry = false;
	bool loaded_worldgen = false;
	bool loaded_chunk = false;
	int loaded_vox_count = 0;

	constexpr int TEX_SIZE = 256; // This is probably too high
	constexpr int MOB_SIZE = 512;
	constexpr int CURSOR_SIZE = 128;

	std::tuple<unsigned char *, int, int, int> load_texture_to_ram(const std::string filename) {
		int width, height, bpp;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *image_data = stbi_load(filename.c_str(), &width, &height, &bpp, STBI_rgb);
		if (image_data == nullptr) throw std::runtime_error(std::string("Cannot open: ") + filename);
		assert(width == TEX_SIZE && height == TEX_SIZE);
		return std::make_tuple(image_data, width, height, bpp);
	}

	std::tuple<unsigned char *, int, int, int> load_mobtexture_to_ram(const std::string filename) {
		int width, height, bpp;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *image_data = stbi_load(filename.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
		if (image_data == nullptr) throw std::runtime_error(std::string("Cannot open: ") + filename);
		assert(width == MOB_SIZE && height == MOB_SIZE);
		return std::make_tuple(image_data, width, height, bpp);
	}

	std::tuple<unsigned char *, int, int, int> load_cursor_texture_to_ram(const std::string filename) {
		int width, height, bpp;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *image_data = stbi_load(filename.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
		if (image_data == nullptr) throw std::runtime_error(std::string("Cannot open: ") + filename);
		assert(width == CURSOR_SIZE && height == CURSOR_SIZE);
		return std::make_tuple(image_data, width, height, bpp);
	}

	static generator asset_loader() {
		using namespace assets;
		using namespace bengine;

		while (loader_section != DONE) {

			/* Handle telemetry */
			if (loader_section == TELEMETRY) {
				start_telemetry();
				started_telemetry = true;
				loader_section = SEND_TELEMETRY;
				loader_subsection = 0;
				co_yield loader_section;
			}
			if (loader_section == SEND_TELEMETRY) {
				call_home("game", "Startup");
				call_home("game", "Startup-FullScreen", std::to_string(config::game_config.fullscreen));
				call_home("game", "Startup-Resolution", std::to_string(config::game_config.window_width) + std::string("x") + std::to_string(config::game_config.window_height));
				call_home("game", "Startup-GUI-Font", config::game_config.gui_ttf + std::string(" / ") + std::to_string(config::game_config.gui_ttf_size));
				call_home("game", "Startup-Scaling", std::to_string(config::game_config.scale_factor));
				sent_telemetry = true;
				loader_section = STATIC_TEXTURES;
				loader_subsection = 0;
				co_yield loader_section;
			}

			/* Handle static textures*/
			if (loader_section == STATIC_TEXTURES) {

				switch (loader_subsection) {
				case 0: background_image = std::make_unique<texture_t>("game_assets/background_image.jpg"); break;
				case 1: game_logo = std::make_unique<texture_t>("game_assets/gamelogo.png"); break;
				case 2: starfield = std::make_unique<texture_t>("game_assets/starfield.jpg"); break;
				default: {
					loader_section = WORLDGEN_TEXTURES;
					loader_subsection = 0;
					loaded_textures = true;
					co_yield loader_section;
					}
				}
				++loader_subsection;				
			}

			/* World-gen textures */
			if (loader_section == WORLDGEN_TEXTURES) {
				glGenTextures(1, &assets::worldgen_texture_array);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D_ARRAY, assets::worldgen_texture_array);

				glTexStorage3D(GL_TEXTURE_2D_ARRAY,
					8, // 8-levels of mipmap
					GL_RGBA8, // Internal format
					128, 128, // Width and height
					biome_textures.size()
				);
				co_yield loader_section;

				for (unsigned int i = 0; i<biome_textures.size(); ++i) {
					glBindTexture(GL_TEXTURE_2D_ARRAY, assets::worldgen_texture_array);
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
					co_yield loader_section;
				}

				glBindTexture(GL_TEXTURE_2D_ARRAY, assets::worldgen_texture_array);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

				loaded_worldgen = true;
				loader_section = CHUNK_TEXTURES;
				loader_subsection = 0;
				co_yield loader_section;
			}

			/* Handle chunk textures */
			if (loader_section == CHUNK_TEXTURES) {
				const int num_actual_textures = static_cast<int>(material_textures.size() * 3);

				glGenTextures(1, &assets::chunk_texture_array);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D_ARRAY, assets::chunk_texture_array);

				glTexStorage3D(GL_TEXTURE_2D_ARRAY,
					2, // 4-levels of mipmap
					GL_RGB8, // Internal format
					TEX_SIZE, TEX_SIZE, // Width and height
					num_actual_textures
				);

				int load_index = 0;
				for (std::size_t i = 0; i<material_textures.size(); ++i) {
					glBindTexture(GL_TEXTURE_2D_ARRAY, assets::chunk_texture_array);
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
					glTexSubImage3D(
						GL_TEXTURE_2D_ARRAY,
						0, // Mipmap number
						0, 0, load_index + 1, // x/y/z offsets
						TEX_SIZE, TEX_SIZE, 1, // width, height, depth
						GL_RGB, // format
						GL_UNSIGNED_BYTE, // type
						std::get<0>(normal_tex) // Color data
					);

					// We need to combine occlusion, metal and rough into one texture
					std::vector<uint8_t> texbytes;
					constexpr int num_bytes = TEX_SIZE * TEX_SIZE * 3;
					texbytes.resize(num_bytes);
					for (int i = 0; i<num_bytes; i += 3) {
						texbytes[i] = std::get<0>(occlusion_tex)[i];
						texbytes[i + 1] = std::get<0>(metal_tex)[i];
						texbytes[i + 2] = std::get<0>(rough_tex)[i];
					}
					glTexSubImage3D(
						GL_TEXTURE_2D_ARRAY,
						0, // Mipmap number
						0, 0, load_index + 2, // x/y/z offsets
						TEX_SIZE, TEX_SIZE, 1, // width, height, depth
						GL_RGB, // format
						GL_UNSIGNED_BYTE, // type
						&texbytes[0] // Color data
					);

					stbi_image_free(std::get<0>(albedo_tex));
					stbi_image_free(std::get<0>(normal_tex));
					stbi_image_free(std::get<0>(occlusion_tex));
					stbi_image_free(std::get<0>(metal_tex));
					stbi_image_free(std::get<0>(rough_tex));

					load_index += 3;
					co_yield loader_section;
				}

				glBindTexture(GL_TEXTURE_2D_ARRAY, assets::chunk_texture_array);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

				loader_section = MOB_TEXTURES;
				loader_subsection = 0;
				co_yield loader_section;
			}

			/* Handle mob textures */
			if (loader_section == MOB_TEXTURES) {
				const int num_actual_textures = static_cast<int>(mob_textures.size());

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
					glBindTexture(GL_TEXTURE_2D_ARRAY, assets::mob_texture_array);
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

					stbi_image_free(std::get<0>(albedo_tex));

					load_index += 1;
					co_yield loader_section;
				}

				glBindTexture(GL_TEXTURE_2D_ARRAY, assets::mob_texture_array);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

				loader_section = CURSOR_TEXTURES;
				loader_subsection = 0;
				co_yield loader_section;
			}

			/* Handle cursor textures */
			if (loader_section == CURSOR_TEXTURES) {
				std::vector<std::string> cursor_textures{ "base_cursor.png", "chop_cursor.png", "tree_cursor.png", "farm_cursor.png", "guard_cursor.png",
					"dig_cursor.png", "channel_cursor.png", "ramp_cursor.png", "downstairs_cursor.png", "upstairs_cursor.png", "updownstairs_cursor.png",
					"wall_cursor.png", "floor_cursor.png", "bridge_cursor.png" };
				const int num_actual_textures = static_cast<int>(cursor_textures.size());

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
					glBindTexture(GL_TEXTURE_2D_ARRAY, assets::cursor_texture_array);
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

					stbi_image_free(std::get<0>(albedo_tex));

					load_index += 1;
					co_yield loader_section;
				}

				glBindTexture(GL_TEXTURE_2D_ARRAY, assets::cursor_texture_array);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

				loaded_chunk = true;
				loaded_worldgen = true;
				loader_section = VOXELS;
				loader_subsection = 0;
				co_yield loader_section;
			}

			/* Handle voxel models */
			if (loader_section == VOXELS) {
				if (loader_subsection < voxel_models_to_load.size()) {
					const auto &vm = voxel_models_to_load[loader_subsection];
					//vox::load_vox(vm.second, vm.first);
					vox::vox_to_load = vm.second;
					vox::vox_to_load_tag = vm.first;
					auto voxgen = vox::load_vox();
					while (voxgen.move_next()) {
						auto result = voxgen.current_value();
						co_yield loader_section;
					}

					++loader_subsection;
					++loaded_vox_count;
					co_yield loader_section;
				}
				else {
					loader_subsection = 0;
					loader_section = DONE;
				}
			}
		}
	}

	static auto g = asset_loader();

	bool loader_tick() {
		auto result = g.current_value();
		return g.move_next();
	}
}