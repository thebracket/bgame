#pragma once

#include <string>
#include "gl_utils.hpp"

namespace gl {

    struct base_shader_t {
        base_shader_t(const char * vertex_file_path,const char * fragment_file_path)
        {
            program_id = LoadShaders(vertex_file_path, fragment_file_path);
        }

        GLint get_uniform_location(const char * name) {
            GLint result = glGetUniformLocation(program_id, name);
            if (result == -1) throw std::runtime_error(std::string("Unknown uniform: ") + std::string(name));
            return result;
        }

        GLint get_attribute_location(const char * name) {
            GLint result = glGetAttribLocation(program_id, name);
            if (result == -1) throw std::runtime_error(std::string("Unknown attribute: ") + std::string(name));
            return result;
        }

        GLuint program_id;
    };

}