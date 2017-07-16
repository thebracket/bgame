#include "base_geometry.hpp"
#include <vector>
#include <iostream>
#include "../gl_include.hpp"

namespace gl {
    unsigned int triangle_vbo = 0;
    std::vector<float> simple_quad {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // TL
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // TR
             0.5f, -0.5f,  0.5f, 1.0f, 1.0f, // BR
             0.5f, -0.5f,  0.5f, 1.0f, 1.0f, // BR
            -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, // BL
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f // TL
    };

    void build_base_geometry() {
        glGenBuffers(1, &triangle_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * simple_quad.size(), &simple_quad[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}