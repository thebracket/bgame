#pragma once

namespace render {
    struct base_lit_buffer_t {
        base_lit_buffer_t(const int &w, const int &h);

        int width, height;
        unsigned int fbo_id = 0;
        unsigned int rbo_id = 0;

        unsigned int color_tex = 0;
        unsigned int bright_tex = 0;
		unsigned int shiny_tex = 0;
    };
}