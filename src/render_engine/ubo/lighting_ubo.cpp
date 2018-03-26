#include "lighting_ubo.hpp"
#include "../../bengine/gl_include.hpp"

namespace render
{
	namespace lighting_ubo
	{
		struct light_shader_ubo_t
		{
			float shadow_matrices[96];
			float light_position[3];
			float far_plane;
		};

		unsigned int ubo;
		static std::vector<light_shader_ubo_t> shader_data;
		static bool made_ubo = false;

		static void update_ubo()
		{
			glBindBuffer(GL_UNIFORM_BUFFER, ubo);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(light_shader_ubo_t) * shader_data.size(), &shader_data[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		static void create_ubo()
		{
			glGenBuffers(1, &ubo);
			update_ubo();
		}

		void update_lights(std::map<std::size_t, pointlight_t> &pointlights)
		{
			shader_data.clear();
			std::size_t i = 0;
			for (auto &l : pointlights)
			{
				light_shader_ubo_t d{};
				d.far_plane = l.second.radius;
				d.light_position[0] = l.second.light_pos.x;
				d.light_position[1] = l.second.light_pos.y;
				d.light_position[2] = l.second.light_pos.z;
				for (auto count = 0; count < 6; count++) {
					memcpy(&d.shadow_matrices[count*16], glm::value_ptr(l.second.shadow_transforms[count]), sizeof(l.second.shadow_transforms[count]));
				}
				shader_data.emplace_back(d);
				l.second.ubo_index = i;
				++i;
			}

			if (!made_ubo)
			{
				create_ubo();
			} else
			{
				update_ubo();
			}
		}
	}
}