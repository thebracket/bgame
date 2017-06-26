#include "geometry_buffer.hpp"
#include "../../../planet/region/region.hpp"
#include "../../../systems/render/lighting_system.hpp"
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif
#include "../../../raws/defs/material_def_t.hpp"
#include "../../../raws/materials.hpp"
#include "constants.hpp"

namespace gl {

    constexpr float tx = 0.0f;
    constexpr float ty = 0.0f;
    constexpr float tsize_x = 1.0f;
    constexpr float tsize_y = 1.0f;

    void terrain_bucket_t::add_floor(const float x, const float y, const float z, float r, float g, float b,
                   const int &idx, const bool &above_ground,
                   const float &light_r, const float &light_g, const float &light_b,
                   const float &light_x, const float &light_y, const float &light_z, const float &shininess)
    {
        //colorize_and_texture_floor(idx, r, g, b, ty);
        const float ground_indicator = above_ground ? 255.0f : 0.0f;

        add_to_items(-0.5f, -0.5f, -0.5f);        // Vertex 0
        add_to_items(x, y, z);                    // World position 0
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 0
        add_to_items(r, g, b);                    // Color 0
        add_to_items(tx, ty);                     // Texture 0
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(-0.5f, -0.5f,  0.5f);        // Vertex 1
        add_to_items(x, y, z);                    // World position 1
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 1
        add_to_items(r, g, b);                    // Color 1
        add_to_items(tx, ty + tsize_y);           // Texture 1
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, -0.5f,  0.5f);         // Vertex 2
        add_to_items(x, y, z);                    // World position 2
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 2
        add_to_items(r, g, b);                    // Color 2
        add_to_items(tx + tsize_x, ty + tsize_y); // Texture 2
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, -0.5f, -0.5f);         // Vertex 3
        add_to_items(x, y, z);                    // World position 3
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 3
        add_to_items(r, g, b);                    // Color 3
        add_to_items(tx + tsize_x, ty);           // Texture 3
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        n_quads += 4;
    }

    void terrain_bucket_t::add_veg(const float x, const float y, const float z, float r, float g, float b,
                                     const int &idx, const bool &above_ground,
                                     const float &light_r, const float &light_g, const float &light_b,
                                     const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                                     const float &wang)
    {
        //colorize_and_texture_floor(idx, r, g, b, ty);
        const float ground_indicator = above_ground ? 255.0f : 0.0f;

        add_to_items(-0.5f, -0.5f, -0.5f);        // Vertex 0
        add_to_items(x, y, z);                    // World position 0
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 0
        add_to_items(r, g, b);                    // Color 0
        add_to_items(tx, ty);                     // Texture 0
        add_to_items(ground_indicator, shininess, wang);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(-0.5f, -0.5f,  0.5f);        // Vertex 1
        add_to_items(x, y, z);                    // World position 1
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 1
        add_to_items(r, g, b);                    // Color 1
        add_to_items(tx, ty + tsize_y);           // Texture 1
        add_to_items(ground_indicator, shininess, wang);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, -0.5f,  0.5f);         // Vertex 2
        add_to_items(x, y, z);                    // World position 2
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 2
        add_to_items(r, g, b);                    // Color 2
        add_to_items(tx + tsize_x, ty + tsize_y); // Texture 2
        add_to_items(ground_indicator, shininess, wang);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, -0.5f, -0.5f);         // Vertex 3
        add_to_items(x, y, z);                    // World position 3
        add_to_items(0.0f, 1.0f, 0.0f);           // Normal 3
        add_to_items(r, g, b);                    // Color 3
        add_to_items(tx + tsize_x, ty);           // Texture 3
        add_to_items(ground_indicator, shininess, wang);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        n_quads += 4;
    }

    void terrain_bucket_t::add_left(const float x, const float y, const float z, float r, float g, float b,
                  const int &idx, const bool &above_ground,
                  const float &light_r, const float &light_g, const float &light_b,
                  const float &light_x, const float &light_y, const float &light_z, const float &shininess)
    {
        const float ground_indicator = above_ground ? 255.0f : 0.0f;

        add_to_items(-0.5f, -0.5f, -0.5f);        // Vertex 0
        add_to_items(x, y, z);                    // World position 0
        add_to_items(-1.0f, 0.0f, 0.0f);          // Normal 0
        add_to_items(r, g, b);                    // Color 0
        add_to_items(tx, ty + tsize_y);           // Texture 0
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(-0.5f, 0.5f,  -0.5f);        // Vertex 1
        add_to_items(x, y, z);                    // World position 1
        add_to_items(-1.0f, 0.0f, 0.0f);           // Normal 1
        add_to_items(r, g, b);                    // Color 1
        add_to_items(tx + tsize_x, ty + tsize_y); // Texture 1
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(-0.5f, 0.5f,  0.5f);         // Vertex 2
        add_to_items(x, y, z);                    // World position 2
        add_to_items(-1.0f, 0.0f, 0.0f);          // Normal 2
        add_to_items(r, g, b);                    // Color 2
        add_to_items(tx + tsize_x, ty);           // Texture 2
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(-0.5f, -0.5f, 0.5f);         // Vertex 3
        add_to_items(x, y, z);                    // World position 3
        add_to_items(-1.0f, 0.0f, 0.0f);           // Normal 3
        add_to_items(r, g, b);                    // Color 3
        add_to_items(tx, ty);                     // Texture 3
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        n_quads += 4;
    }

    void terrain_bucket_t::add_right(const float x, const float y, const float z, float r, float g, float b,
                   const int &idx, const bool &above_ground,
                   const float &light_r, const float &light_g, const float &light_b,
                   const float &light_x, const float &light_y, const float &light_z, const float &shininess)
    {
        const float ground_indicator = above_ground ? 255.0f : 0.0f;

        add_to_items(0.5f, -0.5f, -0.5f);        // Vertex 0
        add_to_items(x, y, z);                    // World position 0
        add_to_items(1.0f, 0.0f, 0.0f);          // Normal 0
        add_to_items(r, g, b);                    // Color 0
        add_to_items(tx, ty + tsize_y);           // Texture 0
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, 0.5f,  -0.5f);        // Vertex 1
        add_to_items(x, y, z);                    // World position 1
        add_to_items(1.0f, 0.0f, 0.0f);           // Normal 1
        add_to_items(r, g, b);                    // Color 1
        add_to_items(tx + tsize_x, ty + tsize_y); // Texture 1
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, 0.5f,  0.5f);         // Vertex 2
        add_to_items(x, y, z);                    // World position 2
        add_to_items(1.0f, 0.0f, 0.0f);          // Normal 2
        add_to_items(r, g, b);                    // Color 2
        add_to_items(tx + tsize_x, ty);           // Texture 2
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, -0.5f, 0.5f);         // Vertex 3
        add_to_items(x, y, z);                    // World position 3
        add_to_items(1.0f, 0.0f, 0.0f);           // Normal 3
        add_to_items(r, g, b);                    // Color 3
        add_to_items(tx, ty);                     // Texture 3
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        n_quads += 4;
    }

    void terrain_bucket_t::add_north(const float x, const float y, const float z, float r, float g, float b,
                   const int &idx, const bool &above_ground,
                   const float &light_r, const float &light_g, const float &light_b,
                   const float &light_x, const float &light_y, const float &light_z, const float &shininess)
    {
        const float ground_indicator = above_ground ? 255.0f : 0.0f;

        add_to_items(-0.5f, -0.5f, -0.5f);        // Vertex 0
        add_to_items(x, y, z);                    // World position 0
        add_to_items(0.0f, 0.0f, -1.0f);          // Normal 0
        add_to_items(r, g, b);                    // Color 0
        add_to_items(tx, ty + tsize_y);           // Texture 0
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, -0.5f,  -0.5f);        // Vertex 1
        add_to_items(x, y, z);                    // World position 1
        add_to_items(0.0f, 0.0f, -1.0f);           // Normal 1
        add_to_items(r, g, b);                    // Color 1
        add_to_items(tx + tsize_x, ty + tsize_y); // Texture 1
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, 0.5f,  -0.5f);         // Vertex 2
        add_to_items(x, y, z);                    // World position 2
        add_to_items(0.0f, 0.0f, -1.0f);          // Normal 2
        add_to_items(r, g, b);                    // Color 2
        add_to_items(tx + tsize_x, ty);           // Texture 2
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(-0.5f, 0.5f, -0.5f);         // Vertex 3
        add_to_items(x, y, z);                    // World position 3
        add_to_items(0.0f, 0.0f, -1.0f);           // Normal 3
        add_to_items(r, g, b);                    // Color 3
        add_to_items(tx, ty);                     // Texture 3
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        n_quads += 4;
    }

    void terrain_bucket_t::add_south(const float x, const float y, const float z, float r, float g, float b,
                   const int &idx, const bool &above_ground,
                   const float &light_r, const float &light_g, const float &light_b,
                   const float &light_x, const float &light_y, const float &light_z, const float &shininess)
    {
        const float ground_indicator = above_ground ? 255.0f : 0.0f;

        add_to_items(-0.5f, -0.5f, 0.5f);        // Vertex 0
        add_to_items(x, y, z);                    // World position 0
        add_to_items(0.0f, 0.0f, 1.0f);          // Normal 0
        add_to_items(r, g, b);                    // Color 0
        add_to_items(tx, ty + tsize_y);           // Texture 0
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, -0.5f,  0.5f);        // Vertex 1
        add_to_items(x, y, z);                    // World position 1
        add_to_items(0.0f, 0.0f, 1.0f);           // Normal 1
        add_to_items(r, g, b);                    // Color 1
        add_to_items(tx + tsize_x, ty + tsize_y); // Texture 1
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(0.5f, 0.5f,  0.5f);         // Vertex 2
        add_to_items(x, y, z);                    // World position 2
        add_to_items(0.0f, 0.0f, 1.0f);          // Normal 2
        add_to_items(r, g, b);                    // Color 2
        add_to_items(tx + tsize_x, ty);           // Texture 2
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        add_to_items(-0.5f, 0.5f, 0.5f);         // Vertex 3
        add_to_items(x, y, z);                    // World position 3
        add_to_items(0.0f, 0.0f, 1.0f);           // Normal 3
        add_to_items(r, g, b);                    // Color 3
        add_to_items(tx, ty);                     // Texture 3
        add_to_items(ground_indicator, shininess, 0.0f);
        add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

        n_quads += 4;
    }

    void terrain_bucket_t::make_vbo() {
        if (generated_vbo) {
            glDeleteBuffers(1, &vbo_id);
        }

        if (items.size() > 0) {
            glGenBuffers(1, &vbo_id); // Generate the VBO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_id);

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * items.size(),
                         &items[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            generated_vbo = true;
        }
    }

    void geometry_buffer_t::add_cube(const float x, const float y, const float z)
    {
        const int idx = mapidx(x,y,z);
        const std::size_t material_idx = region::material(idx);
        const bool constructed = region::flag(idx, CONSTRUCTION);
        const material_def_t * mat = get_material(material_idx);
        int floor_texture_idx = constructed ? mat->constructed_floor_texture : mat->floor_texture;
        int wall_texture_idx = constructed ? mat->constructed_wall_texture : mat->wall_texture;
        const float shininess = mat->shininess;
        const float r = (float)mat->fg.r/255.0f;
        const float g = (float)mat->fg.g/255.0f;
        const float b = (float)mat->fg.b/255.0f;

        bool above_ground = region::above_ground(idx);
        //if (above_ground) std::cout << "Above ground\n";

        float light_r, light_g, light_b, light_x, light_y, light_z;
        auto light_finder = lit_tiles.find(idx);
        if (light_finder != lit_tiles.end()) {
            light_r = (float)light_finder->second.second.r / 255.0f;
            light_g = (float)light_finder->second.second.g / 255.0f;
            light_b = (float)light_finder->second.second.b / 255.0f;
            int lx,ly,lz;
            std::tie(lx,ly,lz) = idxmap(light_finder->second.first);
            light_x = (float)lx / 255.0f;
            light_y = (float)lz / 255.0f;
            light_z = (float)ly / 255.0f;
            //std::cout << "Light hit at " << light_x << "/" << light_y << "/" << light_z << "\n";
        } else {
            light_r = 0.0f;
            light_g = 0.0f;
            light_b = 0.0f;
            light_x = 0.0f;
            light_y = 0.0f;
            light_z = 0.0f;
        }

        // Add floor and ceiling with the appropriate texture
        buckets[floor_texture_idx].add_floor(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess);
        buckets[floor_texture_idx].add_floor(x, y, z+1.0f, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess);

        // Add walls with the appropriate texture
        if (!region::solid(mapidx(x-1, y, z))) buckets[wall_texture_idx].add_left(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess);
        if (!region::solid(mapidx(x+1, y, z))) buckets[wall_texture_idx].add_right(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess);
        if (!region::solid(mapidx(x, y-1, z))) buckets[wall_texture_idx].add_north(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess);
        if (!region::solid(mapidx(x, y+1, z))) buckets[wall_texture_idx].add_south(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess);
    }

    void geometry_buffer_t::add_floor(const float x, const float y, const float z,
                                      const bool &above_ground, const float &light_r,
                                      const float &light_g, const float &light_b, const float &light_x,
                                      const float &light_y, const float &light_z)
    {
        const int idx = mapidx(x,y,z);
        const std::size_t material_idx = region::material(idx);
        const bool constructed = region::flag(idx, CONSTRUCTION);
        const material_def_t * mat = get_material(material_idx);
        const float shininess = mat->shininess;
        const int floor_texture_idx = constructed ? mat->constructed_floor_texture : mat->floor_texture;
        const float r = (float)mat->fg.r/255.0f;
        const float g = (float)mat->fg.g/255.0f;
        const float b = (float)mat->fg.b/255.0f;
        buckets[floor_texture_idx].add_floor(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess);
    }

    void geometry_buffer_t::add_veg(const float x, const float y, const float z,
                                      const bool &above_ground, const float &light_r,
                                      const float &light_g, const float &light_b, const float &light_x,
                                      const float &light_y, const float &light_z, const int &wang)
    {
        const int idx = mapidx(x,y,z);
        constexpr float shininess = 8.0f;
        constexpr int floor_texture_idx = 8;
        constexpr float r = 1.0f;
        constexpr float g = 1.0f;
        constexpr float b = 1.0f;
        buckets[floor_texture_idx].add_veg(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess, (float)wang);
    }

    void geometry_buffer_t::mark_z_level_end(const int &z) {
        for (auto it = buckets.begin(); it != buckets.end(); ++it) {
            const int offset = z - base_z;
            for (int i=offset; i<CHUNK_SIZE; ++i) {
                it->second.z_offsets[i] = it->second.n_quads;
            }
        }
    }

    void geometry_buffer_t::make_vbos() {
        for (auto it = buckets.begin(); it != buckets.end(); ++it) {
            it->second.make_vbo();
        }
    }

    void geometry_buffer_t::finish_z_map(const int &base_z) {
        for (auto it = buckets.begin(); it != buckets.end(); ++it) {
            it->second.z_offsets[CHUNK_SIZE-1] = it->second.n_quads;
        }
    }
}