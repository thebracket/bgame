#pragma once

#include <string>

namespace bengine {
	struct base_shader_t {
		void load_shader(const std::string vertex, const std::string fragment);
		void load_shader(const std::string vertex, const std::string fragment, const std::string geometry);
		void use() const;
		unsigned int shader_id;
		int get_uniform(const std::string name) const;
		unsigned int get_block_index(const std::string & name) const;
		unsigned int get_resource_index(const std::string &name) const;
	};

	unsigned int load_shaders(const char * file_path);
    unsigned int load_shaders(const char * vertex_file_path,const char * fragment_file_path);
	unsigned int load_shaders(const char * vertex_file_path, const char * fragment_file_path, const char * geometry_file_path);
}