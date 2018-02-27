#include "cursors.hpp"
#include "../../systems/mouse.hpp"
#include "../../global_assets/texture_storage.hpp"
#include "../../global_assets/shader_storage.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../systems/gui/particle_system.hpp"
#include "../../global_assets/game_camera.hpp"
#include "../../global_assets/game_mining.hpp"
#include "../../systems/gui/design_mining.hpp"
#include "../../systems/gui/design_harvest.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../systems/gui/design_architecture.hpp"
#include "../../planet/region/region.hpp"
#include "../shaders/cursor_shader.hpp"

namespace render {
	static unsigned int cursors_vao = 0;
	static unsigned int cursors_vbo = 0;

	static void add_cube_geometry(std::vector<float> &v, int &n_elements, const float &x, const float &y, const float &z,
		const float &width, const float &height, const float &texture_id, const float depth = 0.98f)
	{
		const float x0 = -0.5f + x;
		const float x1 = x0 + width;
		const float y0 = -0.5f + z;
		const float y1 = y0 + depth;
		const float z0 = -0.5f + y;
		const float z1 = z0 + height;
		const float TI = texture_id;
		constexpr float T0 = 0.0f;
		const float TW = width;
		const float TH = height;
		n_elements += 36;

		v.insert(v.end(), {
			x0, y0, z0, T0, T0, TI,  0.0f,  0.0f, -1.0f,
			x1, y0, z0, TW, T0, TI,  0.0f,  0.0f, -1.0f,
			x1, y1, z0, TW, TH, TI,  0.0f,  0.0f, -1.0f,
			x1, y1, z0, TW, TH, TI,  0.0f,  0.0f, -1.0f,
			x0, y1, z0, T0, TH, TI,  0.0f,  0.0f, -1.0f,
			x0, y1, z0, T0, TH, TI,  0.0f,  0.0f, -1.0f,

			x0, y0, z1, T0, T0, TI,  0.0f,  0.0f, 1.0f,
			x1, y0, z1, TW, T0, TI,  0.0f,  0.0f, 1.0f,
			x1, y1, z1, TW, TH, TI,  0.0f,  0.0f, 1.0f,
			x1, y1, z1, TW, TH, TI,  0.0f,  0.0f, 1.0f,
			x0, y1, z1, T0, TH, TI,  0.0f,  0.0f, 1.0f,
			x0, y0, z1, T0, T0, TI,  0.0f,  0.0f, 1.0f,

			x0, y1, z1, TW, TH, TI, -1.0f,  0.0f,  0.0f,
			x0, y1, z0, TW, T0, TI, -1.0f,  0.0f,  0.0f,
			x0, y0, z0, T0, T0, TI, -1.0f,  0.0f,  0.0f,
			x0, y0, z0, T0, T0, TI, -1.0f,  0.0f,  0.0f,
			x0, y0, z1, T0, TH, TI, -1.0f,  0.0f,  0.0f,
			x0, y1, z1, TW, TH, TI, -1.0f,  0.0f,  0.0f,

			x1, y1, z1, TW, TH, TI,  1.0f,  0.0f,  0.0f,
			x1, y1, z0, TW, T0, TI,  1.0f,  0.0f,  0.0f,
			x1, y0, z0, T0, T0, TI,  1.0f,  0.0f,  0.0f,
			x1, y0, z0, T0, T0, TI,  1.0f,  0.0f,  0.0f,
			x1, y0, z1, T0, TH, TI,  1.0f,  0.0f,  0.0f,
			x1, y1, z1, TW, TH, TI,  1.0f,  0.0f,  0.0f,

			x0, y0, z0, T0, T0, TI,  0.0f, -1.0f,  0.0f,
			x1, y0, z0, TW, T0, TI,  0.0f, -1.0f,  0.0f,
			x1, y0, z1, TW, TH, TI,  0.0f, -1.0f,  0.0f,
			x1, y0, z1, TW, TH, TI,  0.0f, -1.0f,  0.0f,
			x0, y0, z1, T0, TH, TI,  0.0f, -1.0f,  0.0f,
			x0, y0, z0, T0, T0, TI,  0.0f, -1.0f,  0.0f,

			x0, y1, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f,
			x1, y1, z0, TW, T0, TI,  0.0f,  1.0f,  0.0f,
			x1, y1, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f,
			x1, y1, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f,
			x0, y1, z1, T0, TH, TI,  0.0f,  1.0f,  0.0f,
			x0, y1, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f
		});
	}

	static int n_elements_cursor_elements = 0;

	void build_cursor_geometry() {
		n_elements_cursor_elements = 0;

		// Generate GL resources if needed
		if (cursors_vao == 0) glGenVertexArrays(1, &cursors_vao);
		if (cursors_vbo == 0) glGenBuffers(1, &cursors_vbo);

		// Build the cursor geometry
		std::vector<float> data;

		// Add an entry for the mouse cursor
		const auto mouse_x = std::floor(systems::mouse_wx);
		const auto mouse_y = std::floor(systems::mouse_wy);
		const auto mouse_z = std::floor(systems::mouse_wz);
		if (!(game_master_mode == DESIGN && game_design_mode == BUILDING)) {
			add_cube_geometry(data, n_elements_cursor_elements, mouse_x, mouse_y, mouse_z, 1, 1, 0);
		}
		// TEMPORARY
		//systems::particles::block_destruction_effect(mouse_x, mouse_y, mouse_z, 1.0f, 1.0f, 1.0f, systems::particles::PARTICLE_LUMBERJACK);

		// Lumberjacking
		if (game_master_mode == DESIGN) {
			if (game_design_mode == CHOPPING) {
				for (size_t i = 0; i < REGION_TILES_COUNT; ++i) {
					auto tree_id = region::tree_id(i);
					if (tree_id > 0) {
						auto[x, y, z] = idxmap(i);
						if (designations->chopping.find(tree_id) != designations->chopping.end()) {
							add_cube_geometry(data, n_elements_cursor_elements, x, y, z, 1, 1, 1);
							//std::cout << "Highlighting tree at " << x << ", " << y << ", " << z << "\n";
						}
						//else {
						//	add_cube_geometry(data, n_elements_cursor_elements, x, y, z, 1, 1, 2);
						//}
					}
				}
			}
			else if (game_design_mode == GUARDPOINTS) {
				for (const auto& gp : designations->guard_points) {
					add_cube_geometry(data, n_elements_cursor_elements, gp.second.x, gp.second.y, gp.second.z, 1, 1, 4);
				}
			}
			else if (game_design_mode == HARVEST) {
				for (const auto &idx : systems::design_harvest::cursors) {
					auto[x, y, z] = idxmap(idx);
					add_cube_geometry(data, n_elements_cursor_elements, x, y, z, 1, 1, 3);
				}
			}
			else if (game_design_mode == STOCKPILES) {
				for (size_t i = 0; i < REGION_TILES_COUNT; ++i) {
					auto stockpile_id = region::stockpile_id(i);
					if (stockpile_id > 0 && stockpile_id == current_stockpile) {
						auto[x, y, z] = idxmap(i);
						add_cube_geometry(data, n_elements_cursor_elements, x, y, z, 1, 1, 1);
					}
				}
			}
			else if (game_design_mode == ARCHITECTURE) {
				for (const auto &arch : systems::design_architecture::architecture_cursors) {
					auto[x, y, z] = idxmap(arch.first);
					if (z == camera_position->region_z) {
						int glyph = 1;
						switch (arch.second) {
						case 0: glyph = 11; break; // Wall
						case 1: glyph = 12; break; // Floor
						case 2: glyph = 8; break; // Up
						case 3: glyph = 9; break; // Down
						case 4: glyph = 10; break; // Up-Down
						case 5: glyph = 7; break; // Ramp
						case 6: glyph = 13; break; // Bridge
						}

						if (glyph == 12)
						{
							add_cube_geometry(data, n_elements_cursor_elements, x, y, z, 1, 1, glyph, 0.25f);
						}
						else {
							add_cube_geometry(data, n_elements_cursor_elements, x, y, z, 1, 1, glyph);
						}
					}
				}
			}
			else if (game_design_mode == DIGGING) {
				// Add the "hot" list
				for (const auto &idx : systems::design_mining::mining_cursor_list) {
					auto[x, y, z] = idxmap(idx.first);
					if (z == camera_position->region_z) {
						int glyph = 1;
						switch (idx.second) {
						case MINE_DIG: glyph = 5; break;
						case MINE_CHANNEL: glyph = 6; break;
						case MINE_RAMP: glyph = 7; break;
						case MINE_STAIRS_UP: glyph = 8; break;
						case MINE_STAIRS_DOWN: glyph = 9; break;
						case MINE_STAIRS_UPDOWN: glyph = 10; break;
						}
						add_cube_geometry(data, n_elements_cursor_elements, x, y, z, 1, 1, glyph);
					}
				}

				// Actual designations
				for (const auto &idx : mining_designations->mining_targets) {
						auto[x, y, z] = idxmap(idx.first);
						if (z == camera_position->region_z) {
							int glyph = 1;
							switch (idx.second) {
							case MINE_DIG: glyph = 5; break;
							case MINE_CHANNEL: glyph = 6; break;
							case MINE_RAMP: glyph = 7; break;
							case MINE_STAIRS_UP: glyph = 8; break;
							case MINE_STAIRS_DOWN: glyph = 9; break;
							case MINE_STAIRS_UPDOWN: glyph = 10; break;
							}
							add_cube_geometry(data, n_elements_cursor_elements, x, y, z, 1, 1, glyph);
					}
				}
			}
		}

		// Assign geometry
		glBindVertexArray(cursors_vao);
		glBindBuffer(GL_ARRAY_BUFFER, cursors_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, cursors_vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0); // 0 = Vertex Position

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (char *) nullptr + 3 * sizeof(float));
		glEnableVertexAttribArray(1); // 1 = TexX/Y/ID

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (char *) nullptr + 6 * sizeof(float));
		glEnableVertexAttribArray(2); // 2 = Normals

		glBindVertexArray(0);
		glCheckError();
	}

	void render_cursor(glm::mat4 &camera_projection_matrix, glm::mat4 &camera_modelview_matrix) {
		// Assign the VAO, texture array, uniforms etc.
		glUseProgram(assets::cursor_shader->shader_id);
		glUniformMatrix4fv(assets::cursor_shader->projection_matrix, 1, GL_FALSE, glm::value_ptr(camera_projection_matrix));
		glUniformMatrix4fv(assets::cursor_shader->view_matrix, 1, GL_FALSE, glm::value_ptr(camera_modelview_matrix));
		glUniform1i(assets::cursor_shader->texture_array, 0);
		glBindVertexArray(cursors_vao);		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, assets::cursor_texture_array);

		// Render it
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawArrays(GL_TRIANGLES, 0, n_elements_cursor_elements);
		glDisable(GL_BLEND);
		glCheckError();
		glBindVertexArray(0);
	}
}