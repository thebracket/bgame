#include <vector>
#include "world_textures.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../planet/constants.hpp"
#include "../../planet/region/region.hpp"
#include "../../bengine/color_t.hpp"
#include "../../components/lightsource.hpp"
#include "../../bengine/geometry.hpp"
#include "../../bengine/ecs.hpp"

namespace render {
    constexpr int tex_width = REGION_WIDTH;
    constexpr int tex_height = REGION_HEIGHT;
    constexpr int tex_depth = REGION_DEPTH;
    constexpr int n_texels = tex_width * tex_height * tex_depth * 3;

    bool generated_world_textures = false;
    unsigned int outdoors_y_z_tex = 0;
    unsigned int lightpos_tex = 0;
    unsigned int lightcol_tex = 0;
    bool world_changed = true;
    bool light_changed = true;

    std::unordered_map<int, std::pair<int, bengine::color_t>> lit_tiles;

    inline void build_world_flags() {
        if (!world_changed) return;

        std::vector<uint8_t> texels;
        texels.resize(n_texels);
        std::fill(texels.begin(), texels.end(), 0); // Clear the buffer to 0
        std::size_t index = 0;
        for (int z=0; z<REGION_DEPTH; ++z) {
            for (int y=0; y<REGION_HEIGHT; ++y) {
                for (int x=0; x<REGION_WIDTH; ++x) {
                    const int idx = mapidx(x,y,z);
                    texels[index] = region::above_ground(idx) ? 255 : 0;
                    texels[index+1] = 0;
                    texels[index+2] = 0;
                    index += 3;
                }
            }
        }

        glBindTexture(GL_TEXTURE_3D, outdoors_y_z_tex);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, tex_width, tex_height, tex_depth, 0, GL_RGB, GL_UNSIGNED_BYTE, &texels[0]);

        world_changed = false;
    }

    inline void reveal(const int &idx, const lightsource_t view, const int light_pos) {
        auto finder = lit_tiles.find(idx);
        if (finder == lit_tiles.end()) {
            lit_tiles[idx] = std::make_pair(light_pos, view.color);
        } else {
            const int new_light_level = view.color.r + view.color.g + view.color.b;
            const int old_light_level = finder->second.second.r + finder->second.second.g + finder->second.second.b;
            if (new_light_level > old_light_level) {
                finder->second.first = light_pos;
                finder->second.second = view.color;
            }
        }
    }

    inline void internal_light_to(position_t &pos, lightsource_t &view, int x, int y, int z) {
        const float dist_square = (float)(view.radius * view.radius);

        bengine::line_func_3d_cancellable(pos.x, pos.y, pos.z, pos.x+x, pos.y+y, pos.z+z, [&view, &pos, &dist_square] (int X, int Y, int Z) {
            const auto idx = mapidx(X, Y, Z);
            const float distance = bengine::distance3d_squared(pos.x, pos.y, pos.z, X, Y, Z);
            if (distance > dist_square) {
                return false;
            }
            bool blocked = region::opaque(idx);
            reveal(idx, view, mapidx(pos));
            return !blocked;
        });
    }

    void update_normal_light(bengine::entity_t &e, position_t &pos, lightsource_t &view) {
        if (view.alert_status) {
            view.color = bengine::color_t{1.0f, 0.0f, 0.0f};
        }
        const int idx = mapidx(pos);
        lit_tiles[idx] = std::make_pair(idx, view.color); // Always light yourself
        for (int z=(0-view.radius); z<view.radius; ++z) {
            for (int i=0-view.radius; i<view.radius; ++i) {
                internal_light_to(pos, view, i, 0-view.radius, z);
                internal_light_to(pos, view, i, view.radius, z);
                internal_light_to(pos, view, 0-view.radius, i, z);
                internal_light_to(pos, view, view.radius, i, z);
            }
        }
    }

    void build_world_light() {
        if (!light_changed) return;

        lit_tiles.clear();
        bengine::each<position_t, lightsource_t>(update_normal_light);

        std::vector<uint8_t> texels_loc;
        texels_loc.resize(n_texels);
        std::fill(texels_loc.begin(), texels_loc.end(), 0); // Clear the buffer to 0

        std::vector<uint8_t> texels_col;
        texels_col.resize(n_texels);
        std::fill(texels_col.begin(), texels_col.end(), 0); // Clear the buffer to 0

        for (auto &ls : lit_tiles) {
            int tile_x, tile_y, tile_z;
            std::tie(tile_x, tile_y, tile_z) = idxmap(ls.first);
            int light_x, light_y, light_z;
            std::tie(light_x, light_y, light_z) = idxmap(ls.second.first);
            auto color = ls.second.second;

            const int texidx = (((tex_width * tex_height) * tile_z) + (tex_width * tile_y) + tile_x) * 3;
            texels_loc[texidx] = (uint8_t)light_x;
            texels_loc[texidx+1] = (uint8_t)light_z;
            texels_loc[texidx+2] = (uint8_t)light_y;
            texels_col[texidx] = 255;
            texels_col[texidx+1] = 255;
            texels_col[texidx+2] = 255;
            //std::cout << "Light tile " << tile_x << "/" << tile_y << "/" << tile_z << " with light from " << light_x << "/" << light_y << "/" << light_z << "\n";
        }

        glBindTexture(GL_TEXTURE_3D, lightpos_tex);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, tex_width, tex_height, tex_depth, 0, GL_RGB, GL_UNSIGNED_BYTE, &texels_loc[0]);

        glBindTexture(GL_TEXTURE_3D, lightcol_tex);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, tex_width, tex_height, tex_depth, 0, GL_RGB, GL_UNSIGNED_BYTE, &texels_col[0]);

        light_changed = false;
    }

    void update_world_textures() {
        if (!generated_world_textures) {
            glGenTextures(1, &outdoors_y_z_tex);
            glGenTextures(1, &lightpos_tex);
            glGenTextures(1, &lightcol_tex);
            generated_world_textures = true;
        }

        build_world_flags();
        build_world_light();
    }
}