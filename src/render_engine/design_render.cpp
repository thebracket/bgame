#include "design_render.hpp"
#include "../global_assets/game_camera.hpp"
#include "../planet/region/region.hpp"
#include "chunks/geometry_helper.hpp"
#include "../bengine/gl_include.hpp"
#include <boost/container/flat_map.hpp>

using namespace tile_flags;

namespace render {

	bool mode_change = true;
	static unsigned int design_vbo = 0;
	static unsigned int design_vao = 0;
	static std::vector<float> design_geometry;
	static int n_elements = 0;
	static boost::container::flat_map<int, std::tuple<int, int, int>> static_design_voxel_models;

	void update_design_buffers() {
		if (!mode_change) return;
		mode_change = false;

		// Create the buffers if needed
		if (design_vao == 0) {
			glGenBuffers(1, &design_vbo);
			glGenVertexArrays(1, &design_vao);
		}

		//glInvalidateBufferData(design_vbo); // Invalidate the buffer
		design_geometry.clear();
		n_elements = 0;

		constexpr float DESIGN_DEPTH = 0.2f;

		for (int y = 0; y < REGION_HEIGHT; ++y) {
			for (int x = 0; x < REGION_WIDTH; ++x) {
				const int idx = mapidx(x, y, camera_position->region_z);

				if (region::tile_type(idx) != tile_type::OPEN_SPACE) {
					if (!region::flag(idx, REVEALED)) {
						// Add an unrevealed cube
						n_elements += chunks::add_cube_geometry(design_geometry, x, y, camera_position->region_z, 1.0f, 1.0f, 3, DESIGN_DEPTH);
					}
					else {
						// Determine what to render
						const uint8_t tt = region::tile_type(idx);
						if (tt == tile_type::WINDOW) {
							n_elements += chunks::add_cube_geometry(design_geometry, x, y, camera_position->region_z, 1.0f, 1.0f, 15, DESIGN_DEPTH);
						}
						else if (tt == tile_type::FLOOR) {
							n_elements += chunks::add_floor_geometry(design_geometry, x, y, camera_position->region_z, 1.0f, 1.0f, chunks::get_floor_tex(idx));
						}
						else if (chunks::is_cube(tt)) {
							n_elements += chunks::add_cube_geometry(design_geometry, x, y, camera_position->region_z, 1.0f, 1.0f, chunks::get_cube_tex(idx), DESIGN_DEPTH);
						}
						else if (tt == tile_type::RAMP) {
							// TODO - write me!
							float ne = 0.0f, se = 0.0f, sw = 0.0f, nw = 0.0f;
							if (region::flag(idx - 1, SOLID)) { sw = 1.0f; nw = 1.0f; }
							else if (region::flag(idx + 1, SOLID)) { se = 1.0f; ne = 1.0f; }
							else if (region::flag(idx + REGION_WIDTH, SOLID)) { nw = 1.0f; ne = 1.0f; }
							else if (region::flag(idx - REGION_WIDTH, SOLID)) { sw = 1.0f; se = 1.0f; }
							n_elements += chunks::add_ramp_geometry(design_geometry, x, y, camera_position->region_z, 1.0f, 1.0f, chunks::get_floor_tex(idx), ne, se, sw, nw);
						}
						else if (tt == tile_type::STAIRS_DOWN) {
							static_design_voxel_models.insert(std::make_pair(24, std::make_tuple(x, y, camera_position->region_z)));
						}
						else if (tt == tile_type::STAIRS_UP) {
							static_design_voxel_models.insert(std::make_pair(23, std::make_tuple(x, y, camera_position->region_z)));
						}
						else if (tt == tile_type::STAIRS_UPDOWN) {
							static_design_voxel_models.insert(std::make_pair(25, std::make_tuple(x, y, camera_position->region_z)));
						}
					}
				} else
				{
					n_elements += chunks::add_floor_geometry(design_geometry, x, y, camera_position->region_z, 1.0f, 1.0f, 3);
				}
			}
		}

		glBindVertexArray(design_vao);
		//glInvalidateBufferData(design_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, design_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * design_geometry.size(), &design_geometry[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, design_vbo);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0); // 0 = Vertex Position

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char *) nullptr + 4 * sizeof(float));
		glEnableVertexAttribArray(1); // 1 = TexX/Y/ID

		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char *) nullptr + 7 * sizeof(float));
		glEnableVertexAttribArray(2); // 2 = Normals

		glBindVertexArray(0);
		glCheckError();
	}

	void do_design_render() {
		glBindVertexArray(design_vao);
		glDrawArrays(GL_TRIANGLES, 0, n_elements);
	}
}
