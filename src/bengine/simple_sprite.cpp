#include "simple_sprite.hpp"
#include "gl_include.hpp"
#include "../global_assets/shader_storage.hpp"

namespace bengine {
    namespace simple_sprite {
        unsigned int vbo;
        unsigned int vao;
        unsigned int ebo;
    }


    void init_simple_sprite() {
        float vertices[] = {
                // positions          // colors           // texture coords
                0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
                0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
        };
        unsigned int indices[] = {
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
        };

        glGenVertexArrays(1, &simple_sprite::vao);
        glGenBuffers(1, &simple_sprite::vbo);
        glGenBuffers(1, &simple_sprite::ebo);

        glBindVertexArray(simple_sprite::vao);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, simple_sprite::ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, simple_sprite::vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char *) nullptr + 6 * sizeof(float));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0); // Clean up
    }

    void display_sprite(const unsigned int &texture_id) {
        glUseProgram(assets::spriteshader);
        glBindVertexArray(simple_sprite::vao);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}