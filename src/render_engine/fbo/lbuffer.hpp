#pragma once

namespace render {
    struct lbuffer_t {
        lbuffer_t(const int &w, const int &h);

        int width, height;
        unsigned int fbo_id = 0;
        unsigned int rbo_id = 0;

        unsigned int position_tex = 0;
        unsigned int color_tex = 0;
    };
}