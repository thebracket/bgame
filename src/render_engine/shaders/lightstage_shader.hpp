#pragma once

#include "../../bengine/shaders.hpp"
#include <glm/glm.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "../../bengine/gl_include.hpp"

struct lightstage_shader_t : public bengine::base_shader_t {
	lightstage_shader_t();

	unsigned int albedo_tex;
	unsigned int normal_tex;
	unsigned int position_tex;
	unsigned int ao_tex;
	unsigned int gbuffer_depth_tex;
	unsigned int useSSAO;

	unsigned int camera_position;
	unsigned int moon_color;
	unsigned int projection;

	unsigned int terrain_flags;

	void set_vec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(shader_id, name.c_str()), 1, &value[0]);
	}
};
