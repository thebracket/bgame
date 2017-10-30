#include "cursors.hpp"
#include "../../systems/mouse.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../global_assets/texture_storage.hpp"
#include "../../global_assets/shader_storage.hpp"
#include "../fbo/base_lit_buffer.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../systems/gui/particle_system.hpp"
#include <vector>

namespace render {
	unsigned int cursors_vao = 0;
	unsigned int cursors_vbo = 0;

	void add_cube_geometry(std::vector<float> &v, int &n_elements, const float &x, const float &y, const float &z,
		const float &width, const float &height, const float &texture_id)
	{
		const float x0 = -0.5f + x;
		const float x1 = x0 + width;
		const float y0 = -0.5f + z;
		const float y1 = y0 + 0.98f;
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

	int n_elements_cursor_elements = 0;

	void build_cursor_geometry() {
		n_elements_cursor_elements = 0;

		// Generate GL resources if needed
		if (cursors_vao == 0) glGenVertexArrays(1, &cursors_vao);
		if (cursors_vbo == 0) glGenBuffers(1, &cursors_vbo);

		// Build the cursor geometry
		std::vector<float> data;

		// Add an entry for the mouse cursor
		float mouse_x = static_cast<float>(systems::mouse_wx);
		float mouse_y = static_cast<float>(systems::mouse_wy);
		float mouse_z = static_cast<float>(systems::mouse_wz);
		add_cube_geometry(data, n_elements_cursor_elements, mouse_x, mouse_y, mouse_z, 1, 1, 0);
		// TEMPORARY
		//systems::particles::block_destruction_effect(mouse_x, mouse_y, mouse_z, 1.0f, 1.0f, 1.0f, systems::particles::PARTICLE_LUMBERJACK);

		// Lumberjacking
		if (game_master_mode == DESIGN && game_design_mode == CHOPPING) {
			for (size_t i = 0; i < REGION_TILES_COUNT; ++i) {
				auto tree_id = region::tree_id(i);
				if (tree_id > 0) {
					int x, y, z;
					std::tie(x, y, z) = idxmap(i);
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
		glUseProgram(assets::cursor_shader);
		glUniformMatrix4fv(glGetUniformLocation(assets::cursor_shader, "projection_matrix") , 1, GL_FALSE, glm::value_ptr(camera_projection_matrix));
		glUniformMatrix4fv(glGetUniformLocation(assets::cursor_shader, "view_matrix"), 1, GL_FALSE, glm::value_ptr(camera_modelview_matrix));
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