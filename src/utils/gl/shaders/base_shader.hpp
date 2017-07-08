#pragma once

#include <string>
#include "../gl_utils.hpp"

namespace gl {

    struct base_shader_t {
        void load(const char * vertex_file_path,const char * fragment_file_path);
        GLint get_uniform_location(const char * name);
        GLint get_attribute_location(const char * name);

        GLuint program_id;
    };

}