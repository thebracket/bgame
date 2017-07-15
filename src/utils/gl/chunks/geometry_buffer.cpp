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

        float light_r, light_g, light_b, light_x, light_y, light_z;
        auto light_finder = lit_tiles.find(idx);
        if (light_finder != lit_tiles.end()) {
            light_r = (float)light_finder->second.light_color.r / 255.0f;
            light_g = (float)light_finder->second.light_color.g / 255.0f;
            light_b = (float)light_finder->second.light_color.b / 255.0f;
            int lx,ly,lz;
            std::tie(lx,ly,lz) = idxmap(light_finder->second.light_position);
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
        buckets[floor_texid].add_floor(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess, floor_texid, floor_normalid, floor_specularid, floor_displaceid);
        buckets[floor_texid].add_floor(x, y, z+1.0f, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess, floor_texid, floor_normalid, floor_specularid, floor_displaceid);

        // Add walls with the appropriate texture
        if (!region::solid(mapidx(x-1, y, z))) buckets[wall_texid].add_left(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess, wall_texid, wall_normalid, wall_specularid, wall_displaceid);
        if (!region::solid(mapidx(x+1, y, z))) buckets[wall_texid].add_right(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess, wall_texid, wall_normalid, wall_specularid, wall_displaceid);
        if (!region::solid(mapidx(x, y-1, z))) buckets[wall_texid].add_north(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess, wall_texid, wall_normalid, wall_specularid, wall_displaceid);
        if (!region::solid(mapidx(x, y+1, z))) buckets[wall_texid].add_south(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess, wall_texid, wall_normalid, wall_specularid, wall_displaceid);
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

        float light_r, light_g, light_b, light_x, light_y, light_z;
        auto light_finder = lit_tiles.find(idx);
        if (light_finder != lit_tiles.end()) {
            light_r = (float)light_finder->second.light_color.r / 255.0f;
            light_g = (float)light_finder->second.light_color.g / 255.0f;
            light_b = (float)light_finder->second.light_color.b / 255.0f;
            int lx,ly,lz;
            std::tie(lx,ly,lz) = idxmap(light_finder->second.light_position);
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
        buckets[floor_texid].add_floor(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess, floor_texid, floor_normalid, floor_specularid, floor_displaceid);

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

        buckets[floor_texid].add_slope(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess, floor_texid, floor_normalid, floor_specularid, floor_displaceid, nwy,ney,swy,sey);
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
        float r = (float)mat->fg.r/255.0f;
        float g = (float)mat->fg.g/255.0f;
        float b = (float)mat->fg.b/255.0f;

        auto floor_tex = textures::get_texture_by_id(floor_texture_idx);
        int floor_texid = floor_tex->texture_id;
        int floor_normalid = floor_tex->normal_id;
        int floor_specularid = floor_tex->specular_id;
        int floor_displaceid = floor_tex->displacement_id;

        buckets[floor_texid].add_floor(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess, floor_texid, floor_normalid, floor_specularid, floor_displaceid);
    }

    void geometry_buffer_t::add_water(const float x, const float y, const float z,
                                      const bool &above_ground, const float &light_r,
                                      const float &light_g, const float &light_b, const float &light_x,
                                      const float &light_y, const float &light_z, const uint8_t water_level)
    {
        const int idx = mapidx(x,y,z);
        const float shininess = 1.0f;
        constexpr float r = 1.0f;
        constexpr float g = 1.0f;
        constexpr float b = 1.0f;

        auto floor_tex = textures::get_texture_by_id(15);
        int floor_texid = floor_tex->texture_id;
        int floor_normalid = floor_tex->normal_id;
        int floor_specularid = floor_tex->specular_id;
        int floor_displaceid = floor_tex->displacement_id;

        buckets[floor_texid].add_water(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess, floor_texid, floor_normalid, water_level, floor_specularid, floor_displaceid);
    }

    void geometry_buffer_t::add_veg(const float x, const float y, const float z,
                                      const bool &above_ground, const float &light_r,
                                      const float &light_g, const float &light_b, const float &light_x,
                                      const float &light_y, const float &light_z, const int &wang, const int texid, const int normid, const int specid, const int dispid)
    {
        const int idx = mapidx(x,y,z);
        constexpr float shininess = 8.0f;
        constexpr float r = 1.0f;
        constexpr float g = 1.0f;
        constexpr float b = 1.0f;
        buckets[texid].add_veg(x, y, z, r, g, b, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z, shininess, (float)wang, texid, normid, specid, dispid);
    }

    void geometry_buffer_t::mark_z_level_end(const int &z) {
        for (auto &bucket : buckets) {
            const int offset = z - base_z;
            for (int i = offset; i < CHUNK_SIZE; ++i) {
                bucket.second.z_offsets[i] = bucket.second.n_quads;
            }
        }
    }

    void geometry_buffer_t::make_vbos() {
        for (auto &bucket : buckets) {
            bucket.second.make_vbo();
        }
        //std::cout << "Bucket size: " << bucket.items.size() << "\n";
    }

    void geometry_buffer_t::finish_z_map(const int &base_z) {
        for (auto &bucket : buckets) {
            bucket.second.z_offsets[CHUNK_SIZE - 1] = bucket.second.n_quads;
        }
    }
}