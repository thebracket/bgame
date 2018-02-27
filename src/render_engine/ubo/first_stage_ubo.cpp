#include "first_stage_ubo.hpp"
#include "../../bengine/gl_include.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../global_assets/game_camera.hpp"

namespace render
{
	namespace camera_ubo {
		unsigned int ubo = 0;
		static bool made_ubo = false;

		struct shader_data_t
		{
			float projection_matrix[16];
			float view_matrix[16];
			float proj_view_matrix[16];
			float camera_position[3];
		};

		shader_data_t shader_data;		

		void create_ubo()
		{
			glGenBuffers(1, &ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, ubo);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(shader_data), &shader_data, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void update_ubo()
		{
			glBindBuffer(GL_UNIFORM_BUFFER, ubo);
			GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
			glCheckError();
			memcpy(p, &shader_data, sizeof(shader_data_t));
			glUnmapBuffer(GL_UNIFORM_BUFFER);
		}

		void set_ubo_camera(const glm::mat4 &camera_projection_matrix, const glm::mat4 &camera_modelview_matrix, const glm::mat4 &camera_combined_matrix)
		{
			// Fast copy the matrices to the UBO
			memcpy(shader_data.projection_matrix, glm::value_ptr(camera_projection_matrix), sizeof(shader_data.projection_matrix));
			memcpy(shader_data.view_matrix, glm::value_ptr(camera_modelview_matrix), sizeof(shader_data.view_matrix));
			memcpy(shader_data.proj_view_matrix, glm::value_ptr(camera_combined_matrix), sizeof(shader_data.proj_view_matrix));
			shader_data.camera_position[0] = static_cast<float>(camera_position->region_x);
			shader_data.camera_position[1] = static_cast<float>(camera_position->region_z);
			shader_data.camera_position[2] = static_cast<float>(camera_position->region_y);

			// Create/Update
			if (!made_ubo)
			{
				made_ubo = true;
				create_ubo();
			}
			else
			{
				update_ubo();
			}
		}
	}
}
