#pragma once

#include <string>
#include "gl_utils.hpp"

namespace gl {

    struct base_shader_t {
        base_shader_t(const char * vertex_file_path,const char * fragment_file_path)
        {
            program_id = LoadShaders(vertex_file_path, fragment_file_path);
        }


        GLuint program_id;
    };

}