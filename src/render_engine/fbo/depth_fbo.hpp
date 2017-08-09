#pragma once

namespace render {
    struct depth_fbo_t {
        depth_fbo_t(int size);
        ~depth_fbo_t();

        unsigned int depth_map_fbo;
        unsigned int depth_map;

        int width;
        int height;
    };
}
