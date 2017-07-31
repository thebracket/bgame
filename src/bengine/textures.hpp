#pragma once
#include "gl_include.hpp"
#include <string>

namespace bengine {

    struct texture_t {
        texture_t(const std::string &filename);
        ~texture_t();

        int width, height, bpp;
        GLuint texture_id;
    };

}