#pragma once

#include <vector>
#include <string>
#include <tuple>

/*
 * Provides an automated texture atlas.
 */
namespace textures {

    constexpr int TEX_IN_ATLAS_WIDTH = 128;
    constexpr int TEX_IN_ATLAS_HEIGHT = 128;
    constexpr int SHEET_CHARS = 16;
    constexpr int ATLAS_WIDTH = TEX_IN_ATLAS_WIDTH * SHEET_CHARS;
    constexpr int ATLAS_HEIGHT = TEX_IN_ATLAS_HEIGHT * SHEET_CHARS;
    constexpr float ATLAS_WIDTH_F = (float)ATLAS_WIDTH;
    constexpr float ATLAS_HEIGHT_F = (float)ATLAS_HEIGHT;

    struct texture_t {
        texture_t() {}
        texture_t(int tid, int nid, bool normal, int sid, bool spec, int did, bool dis) : texture_id(tid),
                                                                                          normal_id(nid),
                                                                                          has_normal(normal),
                                                                                          specular_id(sid),
                                                                                          has_specular(spec),
                                                                                          displacement_id(did),
                                                                                          has_displacement(dis)
        {}

        int texture_id = 0;
        int normal_id = 0;
        int specular_id = 0;
        int displacement_id = 0;
        bool has_normal = false;
        bool has_specular = false;
        bool has_displacement = false;
    };

    void load_textures(std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> textures);
    texture_t * get_texture_by_id(const int &id);
}