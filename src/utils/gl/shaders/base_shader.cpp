#include "base_shader.hpp"

namespace gl {
    void base_shader_t::load(const char * vertex_file_path,const char * fragment_file_path)
    {
        program_id = LoadShaders(vertex_file_path, fragment_file_path);
    }

    GLint base_shader_t::get_uniform_location(const char * name) {
        GLint result = glGetUniformLocation(program_id, name);
        if (result == -1) throw std::runtime_error(std::string("Unknown uniform: ") + std::string(name));
        return result;
    }

    GLint base_shader_t::get_attribute_location(const char * name) {
        GLint result = glGetAttribLocation(program_id, name);
        if (result == -1) throw std::runtime_error(std::string("Unknown attribute: ") + std::string(name));
        return result;
    }
}