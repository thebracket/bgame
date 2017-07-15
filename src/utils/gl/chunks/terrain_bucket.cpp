#include "terrain_bucket.hpp"
#include "../textures/texture.hpp"
#include <iostream>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif
#include "../shaders/shader_storage.hpp"
#include "../shaders/terrain_chunk_shader.hpp"
#include "base_geometry.hpp"

namespace gl {
    constexpr float tex_x = 0.0f;
    constexpr float tex_y = 0.0f;
    constexpr float tex_width = 1.0f;
    constexpr float tex_height = 1.0f;

    void terrain_bucket_t::add_floor(const float x, const float y, const float z, float r, float g, float b,
                                     const int &idx, const bool &above_ground,
                                     const float &light_r, const float &light_g, const float &light_b,
                                     const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                                     const int &texture_id, const int &normal_id, const int &specular_id, const int &displacement_id)
    {
        const float ground_indicator = above_ground ? 255.0f : 0.0f;

        add_to_items(x, y, z); // World position
        add_to_items(0.0f, 0.0f, 0.0f, 0.0f); // Rotation
        add_to_items(r, g, b); // Color
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        ++n_quads;
    }

    void terrain_bucket_t::add_water(const float x, const float y, const float z, float r, float g, float b,
                                     const int &idx, const bool &above_ground,
                                     const float &light_r, const float &light_g, const float &light_b,
                                     const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                                     const int &texture_id, const int &normal_id, const uint8_t &water_level, const int &specular_id, const int &displacement_id)
    {
        /*const float ground_indicator = above_ground ? 255.0f : 0.0f;
        const float height = ((float)water_level * 0.1f) - 0.5f;

        add_to_items(-0.5f, height, -0.5f);        // Vertex 0
        add_to_items(x, y, z);                    // World position 0
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 0
        add_to_items(r, g, b);                    // Color 0
        add_to_items(tex_x, tex_y);                     // Texture 0
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(-0.5f, height,  0.5f);        // Vertex 1
        add_to_items(x, y, z);                    // World position 1
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 1
        add_to_items(r, g, b);                    // Color 1
        add_to_items(tex_x, tex_y + tex_height);           // Texture 1
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, height,  0.5f);         // Vertex 2
        add_to_items(x, y, z);                    // World position 2
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 2
        add_to_items(r, g, b);                    // Color 2
        add_to_items(tex_x + tex_width, tex_y + tex_height); // Texture 2
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, height, -0.5f);         // Vertex 3
        add_to_items(x, y, z);                    // World position 3
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 3
        add_to_items(r, g, b);                    // Color 3
        add_to_items(tex_x + tex_width, tex_y);           // Texture 3
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        n_quads += 4;*/
    }

    void terrain_bucket_t::add_veg(const float x, const float y, const float z, float r, float g, float b,
                                   const int &idx, const bool &above_ground,
                                   const float &light_r, const float &light_g, const float &light_b,
                                   const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                                   const float &wang, const int &texture_id, const int &normal_id, const int &specular_id, const int &displacement_id)
    {
        /*
        const float ground_indicator = above_ground ? 255.0f : 0.0f;

        add_to_items(-0.5f, -0.5f, -0.5f);        // Vertex 0
        add_to_items(x, y, z);                    // World position 0
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 0
        add_to_items(r, g, b);                    // Color 0
        add_to_items(tex_x, tex_y);                     // Texture 0
        add_to_items(ground_indicator, shininess, wang);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(-0.5f, -0.5f,  0.5f);        // Vertex 1
        add_to_items(x, y, z);                    // World position 1
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 1
        add_to_items(r, g, b);                    // Color 1
        add_to_items(tex_x, tex_y + tex_height);           // Texture 1
        add_to_items(ground_indicator, shininess, wang);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, -0.5f,  0.5f);         // Vertex 2
        add_to_items(x, y, z);                    // World position 2
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 2
        add_to_items(r, g, b);                    // Color 2
        add_to_items(tex_x + tex_width, tex_y + tex_height); // Texture 2
        add_to_items(ground_indicator, shininess, wang);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, -0.5f, -0.5f);         // Vertex 3
        add_to_items(x, y, z);                    // World position 3
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 3
        add_to_items(r, g, b);                    // Color 3
        add_to_items(tex_x + tex_width, tex_y);           // Texture 3
        add_to_items(ground_indicator, shininess, wang);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        n_quads += 4;*/
    }

    void terrain_bucket_t::add_renderable(const float x, const float y, const float z, float r, float g, float b,
                                          const int &idx, const bool &above_ground,
                                          const float &light_r, const float &light_g, const float &light_b,
                                          const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                                          const float &tex_x, const float &tex_y)
    {
        /*
        const float ground_indicator = above_ground ? 255.0f : 0.0f;
        constexpr float tsize_sprite_x = 24.0f / 384.0f;
        constexpr float tsize_sprite_y = 24.0f / 768.0f;

        add_to_items(-0.5f, -0.5f, -0.5f);        // Vertex 0
        add_to_items(x, y, z);                    // World position 0
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 0
        add_to_items(r, g, b);                    // Color 0
        add_to_items(tex_x, tex_y);                     // Texture 0
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(-0.5f, -0.5f,  0.5f);        // Vertex 1
        add_to_items(x, y, z);                    // World position 1
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 1
        add_to_items(r, g, b);                    // Color 1
        add_to_items(tex_x, tex_y + tsize_sprite_y);           // Texture 1
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, -0.5f,  0.5f);         // Vertex 2
        add_to_items(x, y, z);                    // World position 2
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 2
        add_to_items(r, g, b);                    // Color 2
        add_to_items(tex_x + tsize_sprite_x, tex_y + tsize_sprite_y); // Texture 2
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, -0.5f, -0.5f);         // Vertex 3
        add_to_items(x, y, z);                    // World position 3
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 3
        add_to_items(r, g, b);                    // Color 3
        add_to_items(tex_x + tsize_sprite_x, tex_y);           // Texture 3
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        n_quads += 4;
         */
    }

    void terrain_bucket_t::add_billboard(const float x, const float y, const float z, float r, float g, float b,
                                         const int &idx, const bool &above_ground,
                                         const float &light_r, const float &light_g, const float &light_b,
                                         const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                                         const float &tex_x, const float &tex_y)
    {
        /*
        const float ground_indicator = above_ground ? 255.0f : 0.0f;
        constexpr float tsize_sprite_x = 24.0f / 384.0f;
        constexpr float tsize_sprite_y = 24.0f / 768.0f;
        constexpr float billboard = 1.0f;

        add_to_items(-0.5f, -0.5f, 0.0f);        // Vertex 0
        add_to_items(x, y, z);                    // World position 0
        add_to_items(0.0f, 0.0f, -1.0f);          // Normal 0
        add_to_items(r, g, b);                    // Color 0
        add_to_items(tex_x, tex_y + tsize_sprite_y);           // Texture 0
        add_to_items(ground_indicator, shininess, billboard);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, -0.5f,  0.0f);        // Vertex 1
        add_to_items(x, y, z);                    // World position 1
        add_to_items(0.0f, 0.0f, -1.0f);           // Normal 1
        add_to_items(r, g, b);                    // Color 1
        add_to_items(tex_x + tsize_sprite_x, tex_y + tsize_sprite_y); // Texture 1
        add_to_items(ground_indicator, shininess, billboard);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, 0.5f,  0.0f);         // Vertex 2
        add_to_items(x, y, z);                    // World position 2
        add_to_items(0.0f, 0.0f, -1.0f);          // Normal 2
        add_to_items(r, g, b);                    // Color 2
        add_to_items(tex_x + tsize_sprite_x, tex_y);           // Texture 2
        add_to_items(ground_indicator, shininess, billboard);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(-0.5f, 0.5f, 0.0f);         // Vertex 3
        add_to_items(x, y, z);                    // World position 3
        add_to_items(0.0f, 0.0f, -1.0f);           // Normal 3
        add_to_items(r, g, b);                    // Color 3
        add_to_items(tex_x, tex_y);                     // Texture 3
        add_to_items(ground_indicator, shininess, billboard);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        n_quads += 4;
         */
    }

    void terrain_bucket_t::add_left(const float x, const float y, const float z, float r, float g, float b,
                                    const int &idx, const bool &above_ground,
                                    const float &light_r, const float &light_g, const float &light_b,
                                    const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                                    const int &texture_id, const int &normal_id, const int &specular_id, const int &displacement_id)
    {
        const float ground_indicator = above_ground ? 255.0f : 0.0f;

        add_to_items(x, y, z); // World position
        add_to_items(0.0f, 0.0f, -1.0f, 1.5708f); // Rotation
        add_to_items(r, g, b); // Color
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        ++n_quads;
    }

    void terrain_bucket_t::add_right(const float x, const float y, const float z, float r, float g, float b,
                                     const int &idx, const bool &above_ground,
                                     const float &light_r, const float &light_g, const float &light_b,
                                     const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                                     const int &texture_id, const int &normal_id, const int &specular_id, const int &displacement_id)
    {
        const float ground_indicator = above_ground ? 255.0f : 0.0f;

        add_to_items(x, y, z); // World position
        add_to_items(0.0f, 0.0f, 1.0f, 1.5708f); // Rotation
        add_to_items(r, g, b); // Color
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        ++n_quads;
    }

    void terrain_bucket_t::add_north(const float x, const float y, const float z, float r, float g, float b,
                                     const int &idx, const bool &above_ground,
                                     const float &light_r, const float &light_g, const float &light_b,
                                     const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                                     const int &texture_id, const int &normal_id, const int &specular_id, const int &displacement_id)
    {
        const float ground_indicator = above_ground ? 255.0f : 0.0f;

        add_to_items(x, y, z); // World position
        add_to_items(-1.0f, 0.0f, 0.0f, 1.5708f); // Rotation
        add_to_items(r, g, b); // Color
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        ++n_quads;
    }

    void terrain_bucket_t::add_south(const float x, const float y, const float z, float r, float g, float b,
                                     const int &idx, const bool &above_ground,
                                     const float &light_r, const float &light_g, const float &light_b,
                                     const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                                     const int &texture_id, const int &normal_id, const int &specular_id, const int &displacement_id)
    {
        const float ground_indicator = above_ground ? 255.0f : 0.0f;

        add_to_items(x, y, z); // World position
        add_to_items(1.0f, 0.0f, 0.0f, 1.5708f); // Rotation
        add_to_items(r, g, b); // Color
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        ++n_quads;
    }

    void terrain_bucket_t::add_slope(const float x, const float y, const float z, float r, float g, float b,
                                     const int &idx, const bool &above_ground,
                                     const float &light_r, const float &light_g, const float &light_b,
                                     const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                                     const int &texture_id, const int &normal_id, const int &specular_id, const int &displacement_id,
                                     const float nwy, const float ney, const float swy, const float sey)
    {
        /*
        const float ground_indicator = above_ground ? 255.0f : 0.0f;

        const float x_normal = ney - nwy;
        const float y_normal = ney - sey;

        add_to_items(-0.5f, -0.5f + swy, -0.5f);        // Vertex 0
        add_to_items(x, y, z);                    // World position 0
        add_to_items(x_normal, 1.0f, y_normal);           // Normal 0
        add_to_items(r, g, b);                    // Color 0
        add_to_items(tex_x, tex_y);                     // Texture 0
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(-0.5f, -0.5f + nwy,  0.5f);        // Vertex 1
        add_to_items(x, y, z);                    // World position 1
        add_to_items(x_normal, 1.0f, y_normal);           // Normal 1
        add_to_items(r, g, b);                    // Color 1
        add_to_items(tex_x, tex_y + tex_height);           // Texture 1
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, -0.5f + ney,  0.5f);         // Vertex 2
        add_to_items(x, y, z);                    // World position 2
        add_to_items(x_normal, 1.0f, y_normal);           // Normal 2
        add_to_items(r, g, b);                    // Color 2
        add_to_items(tex_x + tex_width, tex_y + tex_height); // Texture 2
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, -0.5f + sey, -0.5f);         // Vertex 3
        add_to_items(x, y, z);                    // World position 3
        add_to_items(x_normal, 1.0f, y_normal);           // Normal 3
        add_to_items(r, g, b);                    // Color 3
        add_to_items(tex_x + tex_width, tex_y);           // Texture 3
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        n_quads += 4;*/
    }

    void terrain_bucket_t::make_vbo() {
        if (generated_vbo) {
            glDeleteBuffers(1, &vbo_id);
        }

        if (items.size() > 0) {
            if (vao_id == 0) glGenVertexArraysAPPLE(1, &vao_id);
            //std::cout << "Created VAO #" << vao_id << "\n";
            glBindVertexArrayAPPLE(vao_id);

            // Bind the minimal geometry to repeat
            glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo);
            glEnableVertexAttribArray(terrain_chunk_shader->in_position_loc);
            glVertexAttribPointer(terrain_chunk_shader->in_position_loc, 3, GL_FLOAT, GL_FALSE, gl::n_triangle_floats * sizeof(float), 0);

            glEnableVertexAttribArray(terrain_chunk_shader->texture_position_loc);
            glVertexAttribPointer(terrain_chunk_shader->texture_position_loc, 2, GL_FLOAT, GL_FALSE, gl::n_triangle_floats * sizeof(float),
                                  ((char *) nullptr + 3 * sizeof(float)));

            // Populate the per-instance array
            if (vbo_id == 0) glGenBuffers(1, &vbo_id); // Generate the VBO
            glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * items.size(), &items[0], GL_STATIC_DRAW);

            /////////////////
            // Apply bindings
            glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

            // Position 0 = World Position
            glEnableVertexAttribArray(terrain_chunk_shader->world_position_loc);
            glVertexAttribPointer(terrain_chunk_shader->world_position_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float), 0);
            glVertexAttribDivisorARB(terrain_chunk_shader->world_position_loc, 1);

            // Position 3 = World Rotation
            glEnableVertexAttribArray(terrain_chunk_shader->rotation_loc);
            glVertexAttribPointer(terrain_chunk_shader->rotation_loc, 4, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 3 * sizeof(float)));
            glVertexAttribDivisorARB(terrain_chunk_shader->rotation_loc, 1);

            // Position 7 = Color
            glEnableVertexAttribArray(terrain_chunk_shader->color_loc);
            glVertexAttribPointer(terrain_chunk_shader->color_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 7 * sizeof(float)));
            glVertexAttribDivisorARB(terrain_chunk_shader->color_loc, 1);

            // Position 10 = Flags
            glEnableVertexAttribArray(terrain_chunk_shader->flags_loc);
            glVertexAttribPointer(terrain_chunk_shader->flags_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 10 * sizeof(float)));
            glVertexAttribDivisorARB(terrain_chunk_shader->flags_loc, 1);

            // Position 13 = Light Position
            glEnableVertexAttribArray(terrain_chunk_shader->light_position_loc);
            glVertexAttribPointer(terrain_chunk_shader->light_position_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 13 * sizeof(float)));
            glVertexAttribDivisorARB(terrain_chunk_shader->light_position_loc, 1);

            // Position 16 = Light Color
            glEnableVertexAttribArray(terrain_chunk_shader->light_color_loc);
            glVertexAttribPointer(terrain_chunk_shader->light_color_loc, 3, GL_FLOAT, GL_FALSE, gl::n_floats * sizeof(float),
                                  ((char *) nullptr + 16 * sizeof(float)));
            glVertexAttribDivisorARB(terrain_chunk_shader->light_color_loc, 1);

            // Clean up
            glBindVertexArrayAPPLE(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            generated_vbo = true;
        }
    }

}