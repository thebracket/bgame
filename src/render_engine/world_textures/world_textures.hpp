#pragma once

namespace render {
    extern unsigned int outdoors_y_z_tex;
    extern unsigned int lightpos_tex;
    extern unsigned int lightcol_tex;
    extern bool world_changed;
    extern bool light_changed;

    void update_world_textures();
}
