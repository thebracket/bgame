#pragma once

#include <string>

namespace bengine {
	struct base_shader_t {
		void load_shader(const std::string vertex, const std::string fragment);
		void use();
		unsigned int shader_id;
		int get_uniform(const std::string name);
	};

    unsigned int load_shaders(const char * vertex_file_path,const char * fragment_file_path);
	unsigned int load_shaders(const char * vertex_file_path, const char * fragment_file_path, const char * geometry_file_path);
}