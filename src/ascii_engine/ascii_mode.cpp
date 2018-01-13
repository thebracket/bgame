#include "stdafx.h"
#include "ascii_mode.hpp"
#include "../bengine/main_window.hpp"
#include "../planet/region/region.hpp"
#include "../global_assets/game_camera.hpp"
#include "../raws/materials.hpp"
#include "../raws/defs/material_def_t.hpp"
#include <array>

namespace render {

	namespace ascii {

		struct vertex_t {
			float x;
			float y;
			float world_x;
			float world_y;
			float tx;
			float ty;
			float br;
			float bg;
			float bb;
			float r;
			float g;
			float b;
		};
		constexpr int N_VERTICES = 12;

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

		void populate_ascii() {
			const int z = camera_position->region_z;

			// Add terrain
			for (int y = 0; y < REGION_HEIGHT; ++y) {
				for (int x = 0; x < REGION_WIDTH; ++x) {
					const int idx = mapidx(x, y, z);
					const int tidx = termidx(x, y);
					
					// Put in terrain
					const uint8_t ttype = region::tile_type(tidx);
					switch (ttype) {
					case tile_type::SEMI_MOLTEN_ROCK: terminal[tidx] = glyph_t{ 177, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f }; break;
					case tile_type::SOLID: terminal[tidx] = get_material_glyph(idx); break;
					case tile_type::WALL: terminal[tidx] = get_material_glyph(idx, 219); break;
					case tile_type::RAMP: terminal[tidx] = get_material_glyph(idx, 30); break;
					case tile_type::STAIRS_UP: terminal[tidx] = get_material_glyph(idx, '<'); break;
					case tile_type::STAIRS_DOWN: terminal[tidx] = get_material_glyph(idx, '>'); break;
					case tile_type::STAIRS_UPDOWN: terminal[tidx] = get_material_glyph(idx, 'X'); break;
					case tile_type::FLOOR: terminal[tidx] = get_material_glyph(idx, '.'); break;
					case tile_type::TREE_TRUNK: terminal[tidx] = glyph_t{ 186, 1.0f, 1.0f, 0.5f, 0, 0, 0 }; break;
					case tile_type::TREE_LEAF: terminal[tidx] = glyph_t{ 5, 1.0f, 0, 0, 0, 0, 0 }; break;
					case tile_type::WINDOW: terminal[tidx] = get_material_glyph(idx, 176); break;
					case tile_type::CLOSED_DOOR: terminal[tidx] = get_material_glyph(idx, '+'); break;
					case tile_type::OPEN_SPACE: terminal[tidx] = glyph_t{ ' ', 0, 0, 0, 0, 0, 0 }; break; // TODO: Add deep-diving
					default : terminal[tidx] = glyph_t{ ' ', 0, 0, 0, 0, 0, 0 };
					}

					// Add water
					
				}
			}

			// Add buildings

			// Add renderables
		}

		void render_ascii() {
			glInvalidateBufferData(ascii_vbo);
			// Map the data
		}

		void render_cursors() {

		}

		void present() {

		}
	}

	void ascii_render(const double &duration_ms) {
		// If necessary, build the ASCII grid buffers
		if (ascii::ascii_vao == 0) ascii::build_buffers();

		// Compile the ASCII render data
		ascii::populate_ascii();
		ascii::ascii_camera();

		// Draw it
		ascii::render_ascii();

		// Cursor handling
		ascii::render_cursors();

		// Present the FBO to the screen
		ascii::present();
	}
}