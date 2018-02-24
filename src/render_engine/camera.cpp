#include "camera.hpp"
#include "../global_assets/game_camera.hpp"
#include "chunks/chunks.hpp"
#include "../bengine/main_window.hpp"
#include "../bengine/geometry.hpp"

namespace render
{
	bool camera_moved = true;
	bool sun_moved = true;
	glm::mat4 camera_projection_matrix;
	glm::mat4 camera_modelview_matrix;
	glm::mat4 camera_proj_model_view_matrix;
	Frustrum frustrum;

	std::vector<visible_chunk_t> visible_chunks;
	std::set<int> visible_chunk_set;

	constexpr float NINETY_DEGREES_IN_RADIANS = 1.5708f;

	void update_camera()
	{
		int screen_w, screen_h;
		glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);
		const auto camera_x = static_cast<float>(camera_position->region_x);
		const auto camera_y = static_cast<float>(camera_position->region_y);
		const auto camera_z = static_cast<float>(camera_position->region_z);
		const auto camera_zoom = static_cast<float>(camera->zoom_level);
		const auto aspect_ratio = static_cast<float>(screen_w) / static_cast<float>(screen_h);

		if (camera->perspective)
		{
			camera_projection_matrix = glm::perspective(glm::radians(90.0f), aspect_ratio , 1.0f, 300.0f);
		} else
		{
			const auto camera_zoom_x = static_cast<float>(camera->zoom_level);
			const auto x_bounds = (- camera_zoom_x) * aspect_ratio;
			const auto y_bounds = (camera_zoom_x);

			camera_projection_matrix = glm::ortho(x_bounds, -x_bounds, -y_bounds, y_bounds, 1.0f, 300.0f);
		}

		const glm::vec3 up{0.0f, 1.0f, 0.0f};
		const glm::vec3 target{camera_x, camera_z, camera_y };
		glm::vec3 camera_position_v;

		switch (camera->camera_mode)
		{
		case game_camera_mode_t::FRONT:
			{
				// Nice X-perspective view
				camera_position_v = {camera_x, (camera_z + camera_zoom), (camera_y) + (camera_zoom / 3.0f)};
			}
			break;
		case game_camera_mode_t::TOP_DOWN:
			{
				// Top-down
				camera_position_v = {camera_x, (camera_z) + camera_zoom, (camera_y) + 0.1f};
			}
			break;
		case game_camera_mode_t::DIAGONAL_LOOK_NW:
			{
				// Diagonal
				camera_position_v = {camera_x + camera_zoom, (camera_z) + camera_zoom, (camera_y) + camera_zoom};
			}
			break;
		case game_camera_mode_t::DIAGONAL_LOOK_NE:
			{
				// Diagonal
				camera_position_v = {camera_x - camera_zoom, (camera_z) + camera_zoom, (camera_y) + camera_zoom};
			}
			break;
		case game_camera_mode_t::DIAGONAL_LOOK_SW:
			{
				// Diagonal
				camera_position_v = {camera_x + camera_zoom, (camera_z) + camera_zoom, (camera_y) - camera_zoom};
			}
			break;
		case game_camera_mode_t::DIAGONAL_LOOK_SE:
			{
				// Diagonal
				camera_position_v = {camera_x - camera_zoom, (camera_z) + camera_zoom, (camera_y) - camera_zoom};
			}
			break;
		}
		camera_modelview_matrix = glm::lookAt(camera_position_v, target, up);
		camera_proj_model_view_matrix = camera_projection_matrix * camera_modelview_matrix;

		frustrum.update(camera_proj_model_view_matrix);
		visible_chunks.clear();
		for (const auto& chunk : chunks::chunks)
		{
			if (frustrum.checkSphere(glm::vec3(chunk.base_x, chunk.base_y, chunk.base_z), chunks::CHUNK_SIZE * 2))
			{
				visible_chunks.emplace_back(visible_chunk_t{
					bengine::distance3d_squared(chunk.base_x, chunk.base_y, chunk.base_z, camera_position->region_x,
					                            camera_position->region_y, camera_position->region_z),
												chunk.index
				});
				visible_chunk_set.insert(chunk.index);
			}
		}
		std::sort(visible_chunks.begin(), visible_chunks.end(), [](const visible_chunk_t& a, const visible_chunk_t& b)
		{
			return a.distance_from_camera < b.distance_from_camera;
		});

		camera_moved = false;
	}
}
