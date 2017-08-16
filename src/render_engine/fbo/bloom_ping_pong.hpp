#pragma once

namespace render {
    struct bloom_pingpong_t {
        bloom_pingpong_t(const int &w, const int &h);

        int height, width;

        unsigned int fbo1_id = 0;
        unsigned int blur_tex1 = 0;
        unsigned int rbo1_id = 0;

        unsigned int fbo2_id = 0;
        unsigned int blur_tex2 = 0;
        unsigned int rbo2_id = 0;

        void gen_buffer1();
        void gen_buffer2();
    };
}