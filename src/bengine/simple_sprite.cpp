#include "simple_sprite.hpp"
#include "gl_include.hpp"
#include "../global_assets/shader_storage.hpp"

namespace bengine {
    namespace simple_sprite {
        unsigned int vbo;
        unsigned int vao;
        unsigned int ebo;

        int scale_loc = 0;
        int translate_loc = 0;
        int rotate_loc = 0;
        int darken_loc = 0;
        bool done_locations = false;
    }


    void init_simple_sprite() {
        float vertices[] = {
                // positions         // texture coords
                1.0f,  1.0f, 0.0f,   1.0f, 1.0f,   // top right
                1.0f, -1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
                -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,   // bottom left
                -1.0f,  1.0f, 0.0f,  0.0f, 1.0f    // top left
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (char *) nullptr + 3 * sizeof(float));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0); // Clean up
    }

    void display_sprite(const unsigned int &texture_id,
                        const float &scale_x,
                        const float &scale_y,
                        const float &x,
                        const float &y,
                        const float &z,
                        const float &rotate_x,
                        const float &rotate_y,
                        const float &rotate_z,
                        const float &angle,
                        const float &darken)
    {
        if (!simple_sprite::done_locations) {
            simple_sprite::scale_loc = glGetUniformLocation(assets::spriteshader, "scale");
            simple_sprite::translate_loc = glGetUniformLocation(assets::spriteshader, "translate");
            simple_sprite::rotate_loc = glGetUniformLocation(assets::spriteshader, "rotate");
            simple_sprite::darken_loc = glGetUniformLocation(assets::spriteshader, "darken");
            simple_sprite::done_locations = true;
        }

        glUseProgram(assets::spriteshader);
        glBindVertexArray(simple_sprite::vao);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glUniform2f(simple_sprite::scale_loc, scale_x, scale_y);
        glUniform3f(simple_sprite::translate_loc, x, y, z);
        glUniform4f(simple_sprite::rotate_loc, rotate_x, rotate_y, rotate_z, angle);
        glUniform1f(simple_sprite::darken_loc, darken);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}