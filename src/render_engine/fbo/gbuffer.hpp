#pragma once

namespace render {
    struct gbuffer_t {
        gbuffer_t(const int &w, const int &h);

        int width, height;
        unsigned int fbo_id = 0;
        unsigned int rbo_id = 0;

        unsigned int position_tex = 0;
        unsigned int albedo_tex = 0;
        unsigned int normal_tex = 0;
        unsigned int ao_tex = 0;
		unsigned int depth_tex = 0;
    };
}