#pragma once

#include "base_shader.hpp"

namespace gl {
    struct final_shader_t : public base_shader_t {
        final_shader_t();

        GLint intermediate_tex_loc;
        GLint exposure_loc;
    };
}
