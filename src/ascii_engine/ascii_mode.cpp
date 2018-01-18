#include "stdafx.h"
#include "ascii_mode.hpp"
#include "../bengine/main_window.hpp"
#include "../planet/region/region.hpp"
#include "../global_assets/game_camera.hpp"
#include "../raws/materials.hpp"
#include "../raws/defs/material_def_t.hpp"
#include "../global_assets/texture_storage.hpp"
#include "../global_assets/shader_storage.hpp"
#include "../bengine/textures.hpp"
#include "../render_engine/fbo/buffertest.hpp"
#include "../raws/plants.hpp"
#include "../raws/defs/plant_t.hpp"
#include "../components/renderable.hpp"
#include "../components/building.hpp"
#include "../components/renderable_composite.hpp"
#include <array>

namespace render {

	namespace ascii {

		struct vertex_t {
			float x;
			float y;
			float world_x;
			float world_y;
			float world_z;
			float tx;
			float ty;
			float br;
			float bg;
			float bb;
			float r;
			float g;
			float b;
		};
		constexpr int N_VERTICES = 13;

		unsigned int ascii_vao = 0;
		unsigned int ascii_vbo = 0;
		unsigned int ascii_fbo = 0;
		unsigned int position_tex = 0;
		unsigned int albedo_tex = 0;

		struct glyph_t {
			uint8_t glyph;
			float r, g, b;
			float br, bg, bb;
		};

		std::array<glyph_t, REGION_WIDTH * REGION_HEIGHT> terminal;
		std::array<vertex_t, REGION_WIDTH * REGION_HEIGHT * 6> buffer;
		glm::mat4 camera_projection_matrix;
		glm::mat4 camera_modelview_matrix;

		void build_buffers() {
			int width, height;
			glfwGetWindowSize(bengine::main_window, &width, &height);

			// Make the FBO
			glGenFramebuffers(1, &ascii_fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, ascii_fbo);

			// position color buffer
			glGenTextures(1, &position_tex);
			glBindTexture(GL_TEXTURE_2D, position_tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position_tex, 0);

			// color buffer
			glGenTextures(1, &albedo_tex);
			glBindTexture(GL_TEXTURE_2D, albedo_tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, albedo_tex, 0);

			unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
			glDrawBuffers(2, attachments);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// Make the VBO and VAO combination
			glGenVertexArrays(1, &ascii_vao);
			glGenBuffers(1, &ascii_vbo);

			/*
			ASCII vertex:
			0,1			Position
			2,3,4		World Position
			5,6			Texture Co-Ordinates
			7,8,9		Background Color
			10,11,12	Foreground Color
			*/
			glBindVertexArray(ascii_vao);
			glBindBuffer(GL_ARRAY_BUFFER, ascii_vbo);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, N_VERTICES * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0); // 0 = Vertex Position

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, N_VERTICES * sizeof(float), (char *) nullptr + 2 * sizeof(float));
			glEnableVertexAttribArray(1); // 1 = World Position

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, N_VERTICES * sizeof(float), (char *) nullptr + 5 * sizeof(float));
			glEnableVertexAttribArray(2); // 2 = Texture

			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, N_VERTICES * sizeof(float), (char *) nullptr + 7 * sizeof(float));
			glEnableVertexAttribArray(3); // 3 = Background

			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, N_VERTICES * sizeof(float), (char *) nullptr + 10 * sizeof(float));
			glEnableVertexAttribArray(4); // 4 = Foreground

			glBindVertexArray(0);
			glCheckError();
		}

		void ascii_camera() {
			int screen_w, screen_h;
			glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);
			camera_projection_matrix = glm::perspective(glm::radians(90.0f), (float)screen_w / (float)screen_h, 1.0f, 300.0f);
			const glm::vec3 up{ 0.0f, 1.0f, 0.0f };
			const glm::vec3 target{ (float)camera_position->region_x, (float)camera_position->region_z, (float)camera_position->region_y };
			glm::vec3 camera_position_v;
			camera_position_v = { (float)camera_position->region_x, ((float)camera_position->region_z) + (float)camera->zoom_level, ((float)camera_position->region_y) + 0.1f };
			camera_modelview_matrix = glm::lookAt(camera_position_v, target, up);
		}

		constexpr int termidx(const int x, const int y) {
			return (y * REGION_HEIGHT) + x;
		}

		inline glyph_t get_material_glyph(const int &idx, uint8_t glyph_override = 0) {
			const std::size_t material_index = region::material(idx);
			const auto mat = get_material(material_index);
			if (mat) {
				return glyph_t{ glyph_override > 0 ? glyph_override : static_cast<uint8_t>(mat->glyph), mat->fg.r, mat->fg.g, mat->fg.b, mat->bg.r, mat->bg.g, mat->bg.b };
			}
			else {
				return glyph_t{ glyph_override, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f };
			}
		}

		glyph_t get_floor_tile(const int &idx) {
			if (region::flag(idx, CONSTRUCTION)) {
				return get_material_glyph(idx, '+');
			}
			else {
				if (region::veg_type(idx) > 0) {
					return glyph_t{ '"', 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
				}
				else {
					return get_material_glyph(idx, '.');
				}
			}
		}

		glyph_t get_wall_tile(const int &idx) {
			uint8_t wall_mask = 0;
			if (region::tile_type(idx - 1) == tile_type::WALL) wall_mask += 1;
			if (region::tile_type(idx + 1) == tile_type::WALL) wall_mask += 2;
			if (region::tile_type(idx - REGION_WIDTH) == tile_type::WALL) wall_mask += 4;
			if (region::tile_type(idx + REGION_WIDTH) == tile_type::WALL) wall_mask += 8;

			uint8_t glyph = 0;
			switch (wall_mask) {
			case 0:
				glyph = 79;
				break; // Isolated
			case 1:
				glyph = 181;
				break; // West only
			case 2:
				glyph = 198;
				break; // East only
			case 3:
				glyph = 205;
				break; // East and West
			case 4:
				glyph = 208;
				break; // North only
			case 5:
				glyph = 188;
				break; // North and west
			case 6:
				glyph = 200;
				break; // North and east
			case 7:
				glyph = 202;
				break; // North and east/west
			case 8:
				glyph = 210;
				break; // South only
			case 9:
				glyph = 187;
				break; // South and west
			case 10:
				glyph = 201;
				break; // South and east
			case 11:
				glyph = 203;
				break; // South east/west
			case 12:
				glyph = 186;
				break; // North and South
			case 13:
				glyph = 185;
				break; // North/South/West
			case 14:
				glyph = 204;
				break; // North/South/East
			case 15:
				glyph = 206;
				break; // All
			default: {
				std::cout << "WARNING: Wall calculator hit a case of " << +wall_mask << "\n";
				glyph = 79;
			}
			}
			return get_material_glyph(idx, glyph);
		}

		void populate_ascii() {
			const int z = camera_position->region_z;

			// Add terrain
			for (int y = 0; y < REGION_HEIGHT; ++y) {
				for (int x = 0; x < REGION_WIDTH; ++x) {
					const int tidx = termidx(x, y);
					const int idx = mapidx(x, y, z);

					// Put in terrain
					if (region::revealed(idx)) {
						const uint8_t ttype = region::tile_type(idx);
						switch (ttype) {
						case tile_type::SEMI_MOLTEN_ROCK: terminal[tidx] = glyph_t{ 177, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f }; break;
						case tile_type::SOLID: terminal[tidx] = get_material_glyph(idx); break;
						case tile_type::WALL: terminal[tidx] = get_wall_tile(idx); break;
						case tile_type::RAMP: terminal[tidx] = get_material_glyph(idx, 30); break;
						case tile_type::STAIRS_UP: terminal[tidx] = get_material_glyph(idx, '<'); break;
						case tile_type::STAIRS_DOWN: terminal[tidx] = get_material_glyph(idx, '>'); break;
						case tile_type::STAIRS_UPDOWN: terminal[tidx] = get_material_glyph(idx, 'X'); break;
						case tile_type::FLOOR: terminal[tidx] = get_floor_tile(idx); break;
						case tile_type::TREE_TRUNK: terminal[tidx] = glyph_t{ 186, 1.0f, 1.0f, 0.5f, 0, 0, 0 }; break;
						case tile_type::TREE_LEAF: terminal[tidx] = glyph_t{ 5, 0.0f, 1.0f, 0, 0, 0, 0 }; break;
						case tile_type::WINDOW: terminal[tidx] = get_material_glyph(idx, 176); break;
						case tile_type::CLOSED_DOOR: terminal[tidx] = get_material_glyph(idx, '+'); break;
						case tile_type::OPEN_SPACE: terminal[tidx] = glyph_t{ ' ', 0, 0, 0, 0, 0, 0 }; break; // TODO: Add deep-diving
						default: terminal[tidx] = glyph_t{ ' ', 0, 0, 0, 0, 0, 0 };
						}

						// Add water - TODO: Add some variance
						switch (region::water_level(idx)) {
						case 1: terminal[tidx] = glyph_t{ '1', 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }; break;
						case 2: terminal[tidx] = glyph_t{ '2', 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }; break;
						case 3: terminal[tidx] = glyph_t{ '3', 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }; break;
						case 4: terminal[tidx] = glyph_t{ '4', 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }; break;
						case 5: terminal[tidx] = glyph_t{ '5', 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }; break;
						case 6: terminal[tidx] = glyph_t{ '6', 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }; break;
						case 7: terminal[tidx] = glyph_t{ '7', 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }; break;
						case 8: terminal[tidx] = glyph_t{ '8', 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }; break;
						case 9: terminal[tidx] = glyph_t{ '9', 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }; break;
						case 10: terminal[tidx] = glyph_t{ '0', 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }; break;
						}
					}
					else {
						terminal[tidx] = glyph_t{ ' ', 0, 0, 0, 0, 0, 0 };
					}
				}
			}

			// Add buildings
			bengine::each<building_t, position_t>([](bengine::entity_t &e, building_t &b, position_t &pos) {
				if (pos.z == camera_position->region_z) {
					if (b.glyphs_ascii.empty()) {
						std::cout << "WARNING: Building [" << b.tag << "] is lacking ASCII render data.\n";
						return;
					}
					if (b.width == 1 && b.height == 1) {
						terminal[termidx(pos.x, pos.y)] = glyph_t{ static_cast<uint8_t>(b.glyphs_ascii[0].glyph), b.glyphs_ascii[0].foreground.r, b.glyphs_ascii[0].foreground.g, b.glyphs_ascii[0].foreground.b, b.glyphs_ascii[0].background.r, b.glyphs_ascii[0].background.g, b.glyphs_ascii[0].background.b };
					}
					else {
						int i = 0;
						int offX = b.width == 3 ? -1 : 0;
						int offY = b.height == 3 ? -1 : 0;
						for (int y = 0; y < b.height; ++y) {
							for (int x = 0; x < b.width; ++x) {
								terminal[termidx(pos.x + x + offX, pos.y + y + offY)] = glyph_t{ static_cast<uint8_t>(b.glyphs_ascii[i].glyph), b.glyphs_ascii[i].foreground.r, b.glyphs_ascii[i].foreground.g, b.glyphs_ascii[i].foreground.b, b.glyphs_ascii[i].background.r, b.glyphs_ascii[i].background.g, b.glyphs_ascii[i].background.b };
								++i;
							}
						}
					}
				}
			});

			// Add renderables
			bengine::each<renderable_t, position_t>([](bengine::entity_t &e, renderable_t &r, position_t &pos) {
				if (pos.z == camera_position->region_z) {
					terminal[termidx(pos.x, pos.y)] = glyph_t{ static_cast<uint8_t>(r.glyph_ascii), r.foreground.r, r.foreground.g, r.foreground.b, r.background.r, r.background.g, r.background.b };
				}
			});
			bengine::each<renderable_composite_t, position_t>([](bengine::entity_t &e, renderable_composite_t &r, position_t &pos) {
				if (pos.z == camera_position->region_z) {
					terminal[termidx(pos.x, pos.y)] = glyph_t{ static_cast<uint8_t>(r.ascii_char), 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f };
				}
			});
		}

		void render_cursors() {

		}

		void render_ascii()
		{
			constexpr float glyph_width_texture_space = 1.0f / 16.0f;

			const float wz = static_cast<float>(camera_position->region_z);
			constexpr float width = 1.0f;
			constexpr float height = 1.0f;
			std::size_t buffer_idx = 0;
			for (int y = 0; y < REGION_HEIGHT; ++y) {
				for (int x = 0; x < REGION_WIDTH; ++x) {
					const int tidx = termidx(x, y);
					const float wx = static_cast<float>(x);
					const float wy = static_cast<float>(y);

					const float x0 = -0.5f + wx;
					const float x1 = x0 + width;
					const float z0 = -0.5f + wy;
					const float z1 = z0 + height;

					const float TX0 = static_cast<float>(terminal[tidx].glyph % 16) * glyph_width_texture_space;
					const float TY0 = (terminal[tidx].glyph / 16) * glyph_width_texture_space;
					const float TW = TX0 + glyph_width_texture_space;
					const float TH = TY0 + glyph_width_texture_space;
					const float R = terminal[tidx].r;
					const float G = terminal[tidx].g;
					const float B = terminal[tidx].b;
					const float BR = terminal[tidx].br;
					const float BG = terminal[tidx].bg;
					const float BB = terminal[tidx].bb;

					buffer[buffer_idx] =   vertex_t{ x1, z1, wx, wy, wz, TW, TH, R, G, B, BR, BG, BB};
					buffer[buffer_idx +1] = vertex_t{ x1, z0, wx, wy, wz, TW, TY0, R, G, B, BR, BG, BB };
					buffer[buffer_idx +2] = vertex_t{ x0, z0, wx, wy, wz, TX0, TY0, R, G, B, BR, BG, BB };
					buffer[buffer_idx +3] = vertex_t{ x0, z0, wx, wy, wz, TX0, TY0, R, G, B, BR, BG, BB };
					buffer[buffer_idx +4] = vertex_t{ x0, z1, wx, wy, wz, TX0, TH, R, G, B, BR, BG, BB };
					buffer[buffer_idx +5] = vertex_t{ x1, z1, wx, wy, wz, TW, TH, R, G, B, BR, BG, BB };

					buffer_idx += 6;
				}
			}


			// Map the data
			glInvalidateBufferData(ascii_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, ascii_vbo);
			glBufferData(GL_ARRAY_BUFFER, buffer.size() * N_VERTICES * sizeof(float), &buffer[0], GL_DYNAMIC_DRAW);
			glCheckError();
		}

		void present() {
			glDisable(GL_DEPTH_TEST);
			glBindFramebuffer(GL_FRAMEBUFFER, ascii_fbo);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glCheckError();
			glUseProgram(assets::ascii_shader);
			glCheckError();
			glBindVertexArray(ascii_vao);
			glCheckError();
			glUniformMatrix4fv(glGetUniformLocation(assets::ascii_shader, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(camera_projection_matrix));
			glUniformMatrix4fv(glGetUniformLocation(assets::ascii_shader, "view_matrix"), 1, GL_FALSE, glm::value_ptr(camera_modelview_matrix));
			glUniform1i(glGetUniformLocation(assets::ascii_shader, "ascii_tex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, assets::ascii_texture->texture_id);
			glDrawArrays(GL_TRIANGLES, 0, buffer.size());
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void ascii_render(const double &duration_ms) {
		// If necessary, build the ASCII grid buffers
		if (ascii::ascii_vao == 0) ascii::build_buffers();

		// Compile the ASCII render data
		ascii::ascii_camera();
		ascii::populate_ascii();

		// Cursor handling
		ascii::render_cursors();

		// Draw it
		ascii::render_ascii();

		// Present the FBO to the screen
		ascii::present();
		render_test_quad(ascii::albedo_tex);
	}
}