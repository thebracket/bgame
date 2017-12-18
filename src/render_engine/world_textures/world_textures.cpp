#include <vector>
#include "world_textures.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../planet/constants.hpp"
#include "../../planet/region/region.hpp"
#include "../../bengine/color_t.hpp"
#include "../../components/lightsource.hpp"
#include "../../bengine/geometry.hpp"
#include "../../bengine/ecs.hpp"
#include "../../global_assets/game_camera.hpp"
#include "../../global_assets/game_designations.hpp"

namespace render {
    constexpr int tex_width = REGION_WIDTH;
    constexpr int tex_height = REGION_HEIGHT;
    constexpr int tex_depth = REGION_DEPTH;
    constexpr int n_texels = tex_width * tex_height * tex_depth * 3;

    bool generated_world_textures = false;
    unsigned int outdoors_y_z_tex = 0;
    bool world_changed = true;

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


    void update_world_textures() {
        if (!generated_world_textures) {
            glGenTextures(1, &outdoors_y_z_tex);
            generated_world_textures = true;
        }

        build_world_flags();
    }
}