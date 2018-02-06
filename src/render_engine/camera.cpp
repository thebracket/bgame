#include "camera.hpp"
#include "../global_assets/game_camera.hpp"
#include "../global_assets/game_calendar.hpp"
#include "chunks/chunks.hpp"
#include "../systems/mouse.hpp"
#include "../bengine/main_window.hpp"

namespace render {
	bool camera_moved = true;
	bool sun_moved = true;
	glm::mat4 camera_projection_matrix;
	glm::mat4 camera_modelview_matrix;
	glm::mat4 camera_proj_model_view_matrix;
	Frustrum frustrum;
	boost::container::flat_set<int, std::greater<int>> visible_chunks;

	constexpr float NINETY_DEGREES = 1.5708f;

	glm::mat4 MakeInfReversedZProjRH(float fovY_radians, float aspectWbyH, float zNear)
	{
		float f = 1.0f / tan(fovY_radians / 2.0f);
		return glm::mat4(
			f / aspectWbyH, 0.0f, 0.0f, 0.0f,
			0.0f, f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, zNear, 0.0f);
	}

	void update_camera() {
		int screen_w, screen_h;
		glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);
		camera_projection_matrix = glm::perspective(glm::radians(90.0f), (float)screen_w / (float)screen_h, 1.0f, 300.0f);
		//camera_projection_matrix = MakeInfReversedZProjRH(NINETY_DEGREES, 1.0f, 1.0f);

		const glm::vec3 up{ 0.0f, 1.0f, 0.0f };
		const glm::vec3 target{ (float)camera_position->region_x, (float)camera_position->region_z, (float)camera_position->region_y };
		glm::vec3 camera_position_v;
		//camera->camera_mode = DIAGONAL;
		//camera->zoom_level = 2;

		switch (camera->camera_mode) {
		case FRONT: {
			// Nice X-perspective view
			camera_position_v = { (float)camera_position->region_x, ((float)camera_position->region_z) + (float)camera->zoom_level,((float)camera_position->region_y) + ((float)camera->zoom_level / 3.0f) };
		} break;

		case TOP_DOWN: {
			// Top-down
			camera_position_v = { (float)camera_position->region_x, ((float)camera_position->region_z) + (float)camera->zoom_level, ((float)camera_position->region_y) + 0.1f };
		} break;

		case DIAGONAL: {
			// Diagonal
			camera_position_v = { (float)camera_position->region_x + (float)camera->zoom_level, ((float)camera_position->region_z) + (float)camera->zoom_level, ((float)camera_position->region_y) + (float)camera->zoom_level };
		} break;
		}
		camera_modelview_matrix = glm::lookAt(camera_position_v, target, up);
		camera_proj_model_view_matrix = camera_projection_matrix * camera_modelview_matrix;

		frustrum.update(camera_proj_model_view_matrix);
		visible_chunks.clear();
		for (const auto &chunk : chunks::chunks) {
			if (frustrum.checkSphere(glm::vec3(chunk.base_x, chunk.base_y, chunk.base_z), chunks::CHUNK_SIZE * 2))
			{
				visible_chunks.insert(chunk.index);
			}
		}

		camera_moved = false;
	}
}