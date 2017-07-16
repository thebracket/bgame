#include "geometry_buffer.hpp"
#include "../../../planet/region/region.hpp"
#include "../../../systems/render/lighting_system.hpp"
#include "../gl_include.hpp"
#include "../../../raws/defs/material_def_t.hpp"
#include "../../../raws/materials.hpp"
#include "constants.hpp"
#include "../textures/texture.hpp"
#include "../../../main/game_mode.hpp"
#include "../../../main/game_designations.hpp"
#include "../sun_moon.hpp"

namespace gl {

    void geometry_buffer_t::add_cube(const float x, const float y, const float z)
    {
        const int idx = mapidx(x,y,z);
        const std::size_t material_idx = region::material(idx);
        const bool constructed = region::flag(idx, CONSTRUCTION);
        const material_def_t * mat = get_material(material_idx);
        const float r = (float)mat->fg.r/255.0f;
        const float g = (float)mat->fg.g/255.0f;
        const float b = (float)mat->fg.b/255.0f;
        float shininess = mat->shininess;
        int floor_texture_idx = constructed ? mat->constructed_floor_texture : mat->floor_texture;
        int wall_texture_idx = constructed ? mat->constructed_wall_texture : mat->wall_texture;
        if (region::tile_type(idx) == tile_type::TREE_TRUNK) {
            floor_texture_idx = 10;
            wall_texture_idx = 10;
        } else if (region::tile_type(idx) == tile_type::TREE_LEAF) {
            floor_texture_idx = 11;
            wall_texture_idx = 11;
        }

        auto floor_tex = textures::get_texture_by_id(floor_texture_idx);
        int floor_texid = floor_tex->texture_id;
        int floor_normalid = floor_tex->normal_id;
        int floor_specularid = floor_tex->specular_id;
        int floor_displaceid = floor_tex->displacement_id;

        auto wall_tex = textures::get_texture_by_id(wall_texture_idx);
        int wall_texid = wall_tex->texture_id;
        int wall_normalid = wall_tex->normal_id;
        int wall_specularid = wall_tex->specular_id;
        int wall_displaceid = wall_tex->displacement_id;

        //std::cout << "Material: " << mat->name << ", floor # " << floor_texture_idx << ", tile " << floor_texid << "\n";

        bool above_ground = region::above_ground(idx);
        //if (above_ground) std::cout << "Above ground\n";

        // Add floor and ceiling with the appropriate texture
        bucket.add_floor(x, y, z, r, g, b, idx, above_ground, shininess, floor_texid, floor_normalid, floor_specularid, floor_displaceid);
        bucket.add_floor(x, y, z+1.0f, r, g, b, idx, above_ground, shininess, floor_texid, floor_normalid, floor_specularid, floor_displaceid);

        // Add walls with the appropriate texture
        if (!region::solid(mapidx(x+1, y, z))) bucket.add_left(x, y, z, r, g, b, idx, above_ground, shininess, wall_texid, wall_normalid, wall_specularid, wall_displaceid);
        if (!region::solid(mapidx(x-1, y, z))) bucket.add_right(x, y, z, r, g, b, idx, above_ground, shininess, wall_texid, wall_normalid, wall_specularid, wall_displaceid);
        if (!region::solid(mapidx(x, y-1, z))) bucket.add_north(x, y, z, r, g, b, idx, above_ground, shininess, wall_texid, wall_normalid, wall_specularid, wall_displaceid);
        if (!region::solid(mapidx(x, y+1, z))) bucket.add_south(x, y, z, r, g, b, idx, above_ground, shininess, wall_texid, wall_normalid, wall_specularid, wall_displaceid);
    }

    void geometry_buffer_t::add_ramp(const float x, const float y, const float z)
    {
        const int idx = mapidx(x,y,z);
        const std::size_t material_idx = region::material(idx);
        const bool constructed = region::flag(idx, CONSTRUCTION);
        const material_def_t * mat = get_material(material_idx);
        int floor_texture_idx = constructed ? mat->constructed_floor_texture : mat->floor_texture;
        int wall_texture_idx = constructed ? mat->constructed_wall_texture : mat->wall_texture;
        if (region::tile_type(idx) == tile_type::TREE_TRUNK) {
            floor_texture_idx = 10;
            wall_texture_idx = 10;
        } else if (region::tile_type(idx) == tile_type::TREE_LEAF) {
            floor_texture_idx = 11;
            wall_texture_idx = 11;
        }
        const float shininess = mat->shininess;
        const float r = (float)mat->fg.r/255.0f;
        const float g = (float)mat->fg.g/255.0f;
        const float b = (float)mat->fg.b/255.0f;

        auto floor_tex = textures::get_texture_by_id(floor_texture_idx);
        int floor_texid = floor_tex->texture_id;
        int floor_normalid = floor_tex->normal_id;
        int floor_specularid = floor_tex->specular_id;
        int floor_displaceid = floor_tex->displacement_id;

        auto wall_tex = textures::get_texture_by_id(wall_texture_idx);
        int wall_texid = wall_tex->texture_id;
        int wall_normalid = wall_tex->normal_id;
        int wall_specularid = wall_tex->specular_id;
        int wall_displaceid = wall_tex->displacement_id;

        //std::cout << "Material: " << mat->name << ", floor # " << floor_texture_idx << ", tile " << floor_texid << "\n";

        bool above_ground = region::above_ground(idx);
        //if (above_ground) std::cout << "Above ground\n";

        // Add floor and ceiling with the appropriate texture
        bucket.add_floor(x, y, z, r, g, b, idx, above_ground, shininess, floor_texid, floor_normalid, floor_specularid, floor_displaceid);

        float nwy = 0.0f;
        float ney = 0.0f;
        float swy = 0.0f;
        float sey = 0.0f;

        if (region::solid(mapidx(x, y+1, z))) {
            ney = 1.0f; nwy = 1.0f;
        } else  if (region::solid(mapidx(x+1, y, z))) {
            ney = 1.0f; sey = 1.0f;
        } else if (region::solid(mapidx(x-1, y, z))) {
            nwy = 1.0f; swy = 1.0f;
        } else {
            sey = 1.0f; swy = 1.0f;
        }

        //buckets[floor_texid].add_slope(x, y, z, r, g, b, idx, above_ground, shininess, floor_texid, floor_normalid, floor_specularid, floor_displaceid, nwy,ney,swy,sey);
    }

    void geometry_buffer_t::add_floor(const float x, const float y, const float z,
                                      const bool &above_ground)
    {
        const int idx = mapidx(x,y,z);
        const std::size_t material_idx = region::material(idx);
        const bool constructed = region::flag(idx, CONSTRUCTION);
        const material_def_t * mat = get_material(material_idx);
        const float shininess = mat->shininess;
        const int floor_texture_idx = constructed ? mat->constructed_floor_texture : mat->floor_texture;
        float r = (float)mat->fg.r/255.0f;
        float g = (float)mat->fg.g/255.0f;
        float b = (float)mat->fg.b/255.0f;

        auto floor_tex = textures::get_texture_by_id(floor_texture_idx);
        int floor_texid = floor_tex->texture_id;
        int floor_normalid = floor_tex->normal_id;
        int floor_specularid = floor_tex->specular_id;
        int floor_displaceid = floor_tex->displacement_id;

        bucket.add_floor(x, y, z, r, g, b, idx, above_ground, shininess, floor_texid, floor_normalid, floor_specularid, floor_displaceid);
    }

    void geometry_buffer_t::mark_z_level_end(const int &z) {
        const int offset = z - base_z;
        for (int i = offset; i < CHUNK_SIZE; ++i) {
            bucket.z_offsets[i] = bucket.n_quads;
        }
    }

    void geometry_buffer_t::make_vbos() {
        bucket.make_vbo();
        //std::cout << "Bucket size: " << bucket.items.size() << "\n";
    }

    void geometry_buffer_t::finish_z_map(const int &base_z) {
        bucket.z_offsets[CHUNK_SIZE - 1] = bucket.n_quads;
    }
}